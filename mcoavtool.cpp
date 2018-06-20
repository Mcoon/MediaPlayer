#include "mcoavtool.h"
#include "mcosuper.h"
extern "C"
{
	#include <libavformat/avformat.h>
	#include <libavcodec/avcodec.h>
	#include <libswscale/swscale.h>
	#include <libswresample/swresample.h>
}
#pragma comment(lib,"avformat.lib")
#pragma comment(lib,"avcodec.lib")
#pragma comment(lib,"avutil.lib")
#pragma comment(lib,"swscale.lib")
#pragma comment(lib,"swresample.lib")
McoAVTool::McoAVTool()
{
    McoSuper::getObject();
	swsCtx = NULL;
	vCodecContext = NULL;
	aCodecContext = NULL;
}

static double r2d(AVRational *r)
{
    return r->den&&r->num?(double)r->num / (double)r->den:0.;
}

bool McoAVTool::openAVFile(const char *url)
{
    closeAVFile();
    lock.lock();
    int re = avformat_open_input(&ps, url, NULL, NULL);
    if(re != 0)
    {
		av_strerror(re, errorBuff, sizeof(errorBuff));
        lock.unlock();
        return false;
    }
    re = avformat_find_stream_info(ps,NULL);
    if(re<0)
    {
        av_strerror(re,errorBuff,sizeof(errorBuff));
        lock.unlock();
        return false;
    }
    videoStream = av_find_best_stream(ps,AVMEDIA_TYPE_VIDEO,-1,-1,NULL,0);
    audioStream = av_find_best_stream(ps,AVMEDIA_TYPE_AUDIO,-1,-1,NULL,0);

    if(videoStream == AVERROR_STREAM_NOT_FOUND || videoStream == AVERROR_DECODER_NOT_FOUND ) videoStream = -1;
    if(audioStream == AVERROR_STREAM_NOT_FOUND || audioStream == AVERROR_DECODER_NOT_FOUND ) audioStream = -1;

    if(videoStream == -1 && audioStream == -1)
    {
        strcpy(errorBuff,"video is empty!");
        lock.unlock();
        return false;
    }
    if(videoStream != -1)
    {
		width = ps->streams[videoStream]->codecpar->width;
		height = ps->streams[videoStream]->codecpar->height;
		fps = r2d(&ps->streams[videoStream]->avg_frame_rate);
       
    }
	if (audioStream != -1)
	{
		sampleRate = ps->streams[audioStream]->codecpar->sample_rate;
		channels = ps->streams[audioStream]->codecpar->channels;
		channelLayout = ps->streams[audioStream]->codecpar->channel_layout;
	}
	duration = ps->duration / (AV_TIME_BASE / 1000);//毫秒

    lock.unlock();
    return true;
}

bool McoAVTool::openCodec()
{
    lock.lock();

    if(!ps)
    {
        strcpy(errorBuff,"stream is not opened!");
        lock.unlock();
        return false;
    }

    if(videoStream != -1)
    {
		if (!openCodecFromParamters(ps->streams[videoStream]->codecpar))
		{
			lock.unlock();
			return false;
		}
    }
    if(audioStream != -1)
    {
		if (!openCodecFromParamters(ps->streams[audioStream]->codecpar))
		{
			lock.unlock();
			return false;
		}
    }

    lock.unlock();
    return true;
}

bool McoAVTool::openCodecFromParamters(AVCodecParameters * par)
{
	AVCodec *codec = NULL;
	codec = avcodec_find_decoder(par->codec_id);
	AVCodecContext *tempCtx;
	if (par->codec_type == AVMEDIA_TYPE_VIDEO)
	{
		if (vCodecContext) avcodec_free_context(&vCodecContext);
		vCodecContext = avcodec_alloc_context3(codec);
		tempCtx = vCodecContext;
	}
	else if (par->codec_type == AVMEDIA_TYPE_AUDIO)
	{
		if (aCodecContext) avcodec_free_context(&aCodecContext);
		aCodecContext = avcodec_alloc_context3(codec);
		tempCtx = aCodecContext;
	}
	else
	{
		return false;
	}

	
	int re = avcodec_parameters_to_context(tempCtx, par);
	if (re < 0)
	{
		strcpy(errorBuff, "vCodecContext copy error!");
		return false;
	}
	tempCtx->thread_count = 8;

	re = avcodec_open2(tempCtx, codec, NULL);
	if (re != 0)
	{
		av_strerror(re, errorBuff, sizeof(errorBuff));
		return false;
	}

	return true;
}

bool McoAVTool::readPacket(bool *pktIsVideo, qint64 *time)
{
    lock.lock();

    if(!ps)
    {
        strcpy(errorBuff,"stream is not opened!");
        lock.unlock();
        return false;
    }
    if(!pkt)
        pkt = av_packet_alloc();

    int re = av_read_frame(ps,pkt);
    if(re != 0)
    {
        av_strerror(re,errorBuff,sizeof(errorBuff));
        lock.unlock();
        return false;
    }
	if (pkt->stream_index == videoStream) *pktIsVideo = true;
	else *pktIsVideo = false;

	*time = pkt->pts * r2d(&ps->streams[pkt->stream_index]->time_base) * 1000;

    lock.unlock();
    return true;
}

bool McoAVTool::readPacketsToBuff(int len, int * vl, int * al)
{
	lock.lock();
	if (!ps)
	{
		strcpy(errorBuff, "stream is not opened!");
		lock.unlock();
		return false;
	}
	if (len <= 0 || vl == NULL || al == NULL)
	{
		strcpy(errorBuff, "Incorrect input!");
		lock.unlock();
		return false;
	}

	if (videoPacketList.length() >= len || audioPacketList.length() >= len)
	{
		*vl = videoPacketList.length();
		*al = audioPacketList.length();
		lock.unlock();
		return true;
	}

	while ((videoPacketList.length() < len) && (audioPacketList.length() < len))
	{
		AVPacket *tempPacket = av_packet_alloc();
		int re = av_read_frame(ps, tempPacket);
		if (re != 0)
		{
			if (videoPacketList.isEmpty() && audioPacketList.isEmpty())
			{
				av_strerror(re, errorBuff, sizeof(errorBuff));
				av_packet_free(&tempPacket);
				lock.unlock();
				return false;
			}
			else
			{
				*vl = videoPacketList.length();
				*al = audioPacketList.length();
				av_packet_free(&tempPacket);
				lock.unlock();
				return true;
			}
		}

		if (tempPacket->stream_index == videoStream) videoPacketList.push_back(tempPacket);
		else if (tempPacket->stream_index == audioStream) audioPacketList.push_back(tempPacket);
		else av_packet_free(&tempPacket);
	}

	*vl = videoPacketList.length();
	*al = audioPacketList.length();
	
	lock.unlock();
	return true;
}

void McoAVTool::closeAVFile()
{
    lock.lock();

	clearPacketLists(&videoPacketList);
	clearPacketLists(&audioPacketList);

	if (ps) 
		avformat_close_input(&ps);    
	if (pkt)
		av_packet_free(&pkt);
	if (vFrame)
		av_frame_free(&vFrame);
	if (aFrame)
		av_frame_free(&aFrame);
	if (vCodecContext) 
		avcodec_free_context(&vCodecContext);
	if (aCodecContext)
		avcodec_free_context(&aCodecContext);
	if (swsCtx)
	{
		sws_freeContext(swsCtx);
		swsCtx = NULL;
	}

    memset(errorBuff,0,sizeof(errorBuff));
    videoStream = -1;
    audioStream = -1;
    width = 0;
    height = 0;
    duration = 0;
    fps = 0;
	sampleRate = 0;
	channels = 0;
	speed = 1;
    lock.unlock();
}

bool McoAVTool::decoder(bool *pktIsVideo)
{
    lock.lock();

    if(!ps)
    {
        strcpy(errorBuff,"stream is not opened!");
        lock.unlock();
        return false;
    }

    if(!pkt)
    {
        strcpy(errorBuff,"stream don`t read!");
        lock.unlock();
        return false;
    }

    if(pkt->stream_index != videoStream && pkt->stream_index != audioStream)
    {
        strcpy(errorBuff,"pkt error!");
        av_packet_free(&pkt);
        lock.unlock();
        return false;
    }

	AVCodecContext *tempCodecContex = NULL;
	if (pkt->stream_index == videoStream)
	{
		tempCodecContex = vCodecContext;
	}
	else if (pkt->stream_index == audioStream)
	{
		tempCodecContex = aCodecContext;
	}
	if (!tempCodecContex)
	{
		strcpy(errorBuff, "tempCodecContex error!");
		av_packet_free(&pkt);
		lock.unlock();
		return false;
	}

    int re = avcodec_send_packet(tempCodecContex,pkt);
    if(re != 0)
    {
        strcpy(errorBuff,"pkt send error!");
        av_packet_free(&pkt);
        lock.unlock();
        return false;
    }

	if (pkt->stream_index == videoStream)
	{
		if (vFrame) av_frame_free(&vFrame);
		vFrame = av_frame_alloc();
		re = avcodec_receive_frame(tempCodecContex, vFrame);
		*pktIsVideo = true;
	}
	else if (pkt->stream_index == audioStream)
	{
		if (aFrame) av_frame_free(&aFrame);
		aFrame = av_frame_alloc();
		re = avcodec_receive_frame(tempCodecContex, aFrame);
		*pktIsVideo = false;
	}

    
    if(re != 0)
    {
        if(re == AVERROR(EAGAIN))
            strcpy(errorBuff,"AVERROR(EAGAIN) receive error!");
        else if(re == AVERROR_EOF)
            strcpy(errorBuff,"AVERROR_EOF receive error!");
        else if(re == AVERROR(EINVAL))
            strcpy(errorBuff,"AVERROR(EINVAL) receive error!");
        else
            strcpy(errorBuff,"pkt receive error!");
        av_packet_free(&pkt);
        lock.unlock();
        return false;
    }



    //av_packet_free(&pkt);

    lock.unlock();
    return true;
}

bool McoAVTool::autoDecoder(bool * pktIsVideo)
{
	lock.lock();

	if (!ps)
	{
		strcpy(errorBuff, "stream is not opened!");
		lock.unlock();
		return false;
	}
	if (pktIsVideo == NULL)
	{
		strcpy(errorBuff, "Incorrect input!");
		lock.unlock();
		return false;
	}
	if (videoPacketList.isEmpty() && audioPacketList.isEmpty())
	{
		lock.unlock();
		return false;
	}

	qint64 vtime = getNextVideoTime();
	qint64 atime = getNextAudioTime();

	if ((vtime < atime && vtime != -1) || atime == -1)
	{
		if (!vCodecContext)
		{
			lock.unlock();
			openCodec();
			
			return false;
		}
		//解码视频
		AVPacket *t = videoPacketList.at(0);
		int re = avcodec_send_packet(vCodecContext,t);
		av_packet_free(&t);
		videoPacketList.pop_front();
		if (re != 0)
		{
			av_strerror(re, errorBuff, sizeof(errorBuff));
			lock.unlock();
			return false;
		}
		if (vFrame) av_frame_free(&vFrame);
		vFrame = av_frame_alloc();
		re = avcodec_receive_frame(vCodecContext,vFrame);
		if (re != 0)
		{
			av_strerror(re, errorBuff, sizeof(errorBuff));
			lock.unlock();
			return false;
		}
		*pktIsVideo = true;
	}
	else if ((atime <= vtime && atime != -1) || vtime == -1)
	{
		if (!aCodecContext)
		{
			lock.unlock();
			openCodec();
			return false;
		}
		//解码音频
		AVPacket *t = audioPacketList.at(0);
		int re = avcodec_send_packet(aCodecContext,t);
		av_packet_free(&t);
		audioPacketList.pop_front();
		if (re != 0)
		{
			av_strerror(re, errorBuff, sizeof(errorBuff));
			lock.unlock();
			return false;
		}
		if (aFrame) av_frame_free(&aFrame);
		aFrame = av_frame_alloc();
		re = avcodec_receive_frame(aCodecContext, aFrame);
		if (re != 0)
		{
			av_strerror(re, errorBuff, sizeof(errorBuff));
			lock.unlock();
			return false;
		}
		*pktIsVideo = false;
	}
	else
	{
		lock.unlock();
		return false;
	}

	lock.unlock();
	return true;
}

bool McoAVTool::getRGB(char *outdata, int width, int height)
{
    lock.lock();

	if (!ps)
	{
		strcpy(errorBuff, "stream is not opened!");
		lock.unlock();
		return false;
	}

	if (!vFrame)
	{
		strcpy(errorBuff, "vFrame is not existed!");
		lock.unlock();
		return false;
	}

	if (vFrame->width <= 0 || vFrame->height <= 0)
	{
		strcpy(errorBuff, "Image is not ready!");
		lock.unlock();
		return false;
	}

    swsCtx = sws_getCachedContext(swsCtx,vFrame->width,vFrame->height,(AVPixelFormat)ps->streams[videoStream]->codecpar->format,
                                  width,height,AV_PIX_FMT_BGRA,SWS_BICUBIC,NULL,NULL,NULL);
    if(!swsCtx)
    {
        strcpy(errorBuff,"get cached context error!");
        lock.unlock();
        return false;
    }

    //转码输出空间
    uint8_t *data[AV_NUM_DATA_POINTERS] = {0};
    data[0]  = (uint8_t*)outdata;
    //图像宽度
    int linesize[AV_NUM_DATA_POINTERS] = {0};
    linesize[0] = width * 4;

    int h = sws_scale(swsCtx,vFrame->data,vFrame->linesize,0,vFrame->height,data,linesize);

    if(h <= 0)
    {
        strcpy(errorBuff,"scale error!");
        lock.unlock();
        return false;
    }
	
	
    lock.unlock();
    return true;
}

long long McoAVTool::getPCM(char * outdata, int len)
{
	lock.lock();

	if (!ps || !aFrame || !aCodecContext)
	{
		strcpy(errorBuff, "stream is not opened!");
		lock.unlock();
		return 0;
	}
	if (outdata == NULL || len <= 0)
	{
		strcpy(errorBuff, "Incorrent input!");
		lock.unlock();
		return 0;
	}

	swrCtx = swr_alloc_set_opts(swrCtx,channelLayout,AV_SAMPLE_FMT_S16,(double)sampleRate * speed,
		aFrame->channel_layout, (AVSampleFormat)aFrame->format, (double)aFrame->sample_rate * speed,0,NULL);
	if (!swrCtx)
	{
		strcpy(errorBuff, "get cached context error!");
		lock.unlock();
		return 0;
	}
	swr_init(swrCtx);

	//转码输出空间
	uint8_t *data[1] = { 0 };
	data[0] = (uint8_t*)outdata;
	
	int l =swr_convert(swrCtx, data, len, (const uint8_t **)aFrame->data, aFrame->nb_samples);
	if (l <= 0)
	{
		lock.unlock();
		return 0;
	}

	long long outSize = av_samples_get_buffer_size(NULL,aFrame->channels,aFrame->nb_samples,AV_SAMPLE_FMT_S16,0);

	av_frame_free(&aFrame);

	lock.unlock();
	return outSize;
}

bool McoAVTool::seek(double pos)
{
	lock.lock();
	if (!ps)
	{
		strcpy(errorBuff, "stream is not ready!");
		lock.unlock();
		return false;
	}

	if (duration <= 0)
	{
		strcpy(errorBuff, "Media is network stream!");
		lock.unlock();
		return false;
	}

	if (pos < 0) pos = 0;
	else if (pos > 1) pos = 1;

	int64_t temp = 0;
	temp = ps->duration * pos;

	int re = av_seek_frame(ps, -1,
		temp,
		AVSEEK_FLAG_BACKWARD |
		AVSEEK_FLAG_FRAME);
	if (re < 0)
	{
		strcpy(errorBuff, "Seek error!");
		lock.unlock();
		return false;
	}

	if (vCodecContext)
		avcodec_flush_buffers(vCodecContext);
	if (aCodecContext)
		avcodec_flush_buffers(aCodecContext);

	clearPacketLists(&videoPacketList);
	clearPacketLists(&audioPacketList);

	lock.unlock();
	return true;

}

qint64 McoAVTool::getNextVideoTime()
{
	if (videoPacketList.isEmpty()) return -1;
	qint64 t = videoPacketList.at(0)->pts * r2d(&ps->streams[videoPacketList.at(0)->stream_index]->time_base) * 1000;
	return t;
}

qint64 McoAVTool::getNextAudioTime()
{
	if (audioPacketList.isEmpty()) return -1;
	qint64 t = audioPacketList.at(0)->pts * r2d(&ps->streams[audioPacketList.at(0)->stream_index]->time_base) * 1000;
	return t;
}

qint64 McoAVTool::getNextTime()
{
	lock.lock();

	qint64 vtime = getNextVideoTime();
	qint64 atime = getNextAudioTime();
	if (atime == -1 && vtime == -1)
	{
		lock.unlock();
		return -1;
	}

	if ((vtime < atime && vtime != -1) || (atime == -1))
	{
		lock.unlock();
		return vtime;
	}
	else if ((atime <= vtime && atime != -1) || vtime == -1)
	{
		lock.unlock();
		return atime;
	}
	else
	{
		lock.unlock();
		return -1;
	}

	lock.unlock();
	return -1;
}

string McoAVTool::getErrorInfo()
{
    lock.lock();
    string str = errorBuff;
    lock.unlock();
    return str;
}

int McoAVTool::getWidth()
{
	return width;
}

int McoAVTool::getHeight()
{
	return height;
}

int McoAVTool::getDuration()
{
	return duration;
}

int McoAVTool::getFps()
{
	return fps;
}

int McoAVTool::getChannels()
{
	return channels;
}

int McoAVTool::getSampleRate()
{
	return (double)sampleRate * speed;
}

int McoAVTool::getVideoStream()
{
	return videoStream;
}

int McoAVTool::getAudioStream()
{
	return audioStream;
}

double McoAVTool::getSpeed()
{
	return speed;
}

void McoAVTool::setSpeed(double s)
{
	speed = s;
}

void McoAVTool::clearPacketLists(QList<AVPacket*>* l)
{
	AVPacket *t = NULL;
	while (!l->isEmpty())
	{
		t = l->at(0);
		av_packet_free(&t);
		l->pop_front();
	}
	l->clear();
}
