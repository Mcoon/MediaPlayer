#include "mcoavtool.h"
#include "mcosuper.h"
extern "C"
{
    #include <libavformat/avformat.h>
    #include <libavcodec/avcodec.h>
    #include <libswscale/swscale.h>
}
#pragma comment(lib,"avformat.lib")
#pragma comment(lib,"avcodec.lib")
#pragma comment(lib,"avutil.lib")
#pragma comment(lib,"swscale.lib")
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
    mutex.lock();
    int re = avformat_open_input(&ps, url, NULL, NULL);
    if(re != 0)
    {
		av_strerror(re, errorBuff, sizeof(errorBuff));
        mutex.unlock();
        return false;
    }
    re = avformat_find_stream_info(ps,NULL);
    if(re<0)
    {
        av_strerror(re,errorBuff,sizeof(errorBuff));
        mutex.unlock();
        return false;
    }
    videoStream = av_find_best_stream(ps,AVMEDIA_TYPE_VIDEO,-1,-1,NULL,0);
    audioStream = av_find_best_stream(ps,AVMEDIA_TYPE_AUDIO,-1,-1,NULL,0);

    if(videoStream == AVERROR_STREAM_NOT_FOUND || videoStream == AVERROR_DECODER_NOT_FOUND ) videoStream = -1;
    if(audioStream == AVERROR_STREAM_NOT_FOUND || audioStream == AVERROR_DECODER_NOT_FOUND ) audioStream = -1;

    if(videoStream == -1 && audioStream == -1)
    {
        strcpy(errorBuff,"video is empty!");
        mutex.unlock();
        return false;
    }
    if(videoStream != -1)
    {
        width = ps->streams[videoStream]->codecpar->width;
        height =  ps->streams[videoStream]->codecpar->height;
        fps = r2d(&ps->streams[videoStream]->avg_frame_rate);
       
    }

	duration = ps->duration / (AV_TIME_BASE / 1000);//毫秒

    mutex.unlock();
    return true;
}

bool McoAVTool::openCodec()
{
    mutex.lock();

    if(!ps)
    {
        strcpy(errorBuff,"stream is not opened!");
        mutex.unlock();
        return false;
    }

    if(videoStream != -1)
    {
        AVCodec *codec = NULL;
        codec = avcodec_find_decoder(ps->streams[videoStream]->codecpar->codec_id);

		if (vCodecContext) avcodec_free_context(&vCodecContext);
		vCodecContext = avcodec_alloc_context3(codec);
		int re = avcodec_parameters_to_context(vCodecContext, ps->streams[videoStream]->codecpar);
		if (re < 0)
		{
			strcpy(errorBuff, "vCodecContext copy error!");
			mutex.unlock();
			return false;
		}
		vCodecContext->thread_count = 8;

        re = avcodec_open2(vCodecContext,codec,NULL);
        if(re != 0)
        {
            av_strerror(re,errorBuff,sizeof(errorBuff));
            mutex.unlock();
            return false;
        }
    }
    if(audioStream != -1)
    {
        AVCodec *codec = NULL;
        codec = avcodec_find_decoder(ps->streams[audioStream]->codecpar->codec_id);

		if (aCodecContext) avcodec_free_context(&aCodecContext);
		aCodecContext = avcodec_alloc_context3(codec);
		int re = avcodec_parameters_to_context(aCodecContext, ps->streams[audioStream]->codecpar);
		if (re < 0)
		{
			strcpy(errorBuff, "aCodecContext copy error!");
			mutex.unlock();
			return false;
		}
		aCodecContext->thread_count = 8;

        re = avcodec_open2(aCodecContext,codec,NULL);
        if(re != 0)
        {
            av_strerror(re,errorBuff,sizeof(errorBuff));
            mutex.unlock();
            return false;
        }
    }

    mutex.unlock();
    return true;
}

bool McoAVTool::readPacket(bool *pktIsVideo, qint64 *time)
{
    mutex.lock();

    if(!ps)
    {
        strcpy(errorBuff,"stream is not opened!");
        mutex.unlock();
        return false;
    }
    if(!pkt)
        pkt = av_packet_alloc();

    int re = av_read_frame(ps,pkt);
    if(re != 0)
    {
        av_strerror(re,errorBuff,sizeof(errorBuff));
        mutex.unlock();
        return false;
    }
	if (pkt->stream_index == videoStream) *pktIsVideo = true;
	else *pktIsVideo = false;

	*time = pkt->pts * r2d(&ps->streams[pkt->stream_index]->time_base) * 1000;

    mutex.unlock();
    return true;
}

void McoAVTool::closeAVFile()
{
    mutex.lock();
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
    mutex.unlock();
}

bool McoAVTool::decoder(bool *pktIsVideo)
{
    mutex.lock();

    if(!ps)
    {
        strcpy(errorBuff,"stream is not opened!");
        mutex.unlock();
        return false;
    }

    if(!pkt)
    {
        strcpy(errorBuff,"stream don`t read!");
        mutex.unlock();
        return false;
    }

    if(pkt->stream_index != videoStream && pkt->stream_index != audioStream)
    {
        strcpy(errorBuff,"pkt error!");
        av_packet_free(&pkt);
        mutex.unlock();
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
		mutex.unlock();
		return false;
	}

    int re = avcodec_send_packet(tempCodecContex,pkt);
    if(re != 0)
    {
        strcpy(errorBuff,"pkt send error!");
        av_packet_free(&pkt);
        mutex.unlock();
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
        mutex.unlock();
        return false;
    }



    //av_packet_free(&pkt);

    mutex.unlock();
    return true;
}

bool McoAVTool::getRGB(char *outdata, int width, int height)
{
    mutex.lock();

	if (!ps)
	{
		strcpy(errorBuff, "stream is not opened!");
		mutex.unlock();
		return false;
	}

	if (!vFrame)
	{
		strcpy(errorBuff, "vFrame is not existed!");
		mutex.unlock();
		return false;
	}
	if (!pkt)
	{
		strcpy(errorBuff, "stream is not opened!");
		mutex.unlock();
		return false;
	}
	if (pkt->size < 0)
	{
		strcpy(errorBuff, "stream is not opened!");
		mutex.unlock();
		return false;
	}

    swsCtx = sws_getCachedContext(swsCtx,vFrame->width,vFrame->height,(AVPixelFormat)ps->streams[videoStream]->codecpar->format,
                                  width,height,AV_PIX_FMT_BGRA,SWS_BICUBIC,NULL,NULL,NULL);
    if(!swsCtx)
    {
        strcpy(errorBuff,"get cached context error!");
        mutex.unlock();
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
        mutex.unlock();
        return false;
    }

    mutex.unlock();
    return true;
}

string McoAVTool::getErrorInfo()
{
    mutex.lock();
    string str = errorBuff;
    mutex.unlock();
    return str;
}
