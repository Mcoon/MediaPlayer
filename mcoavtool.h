#ifndef MCOAVTOOL_H
#define MCOAVTOOL_H

#include <mutex>
#include <string>
#include <qlist.h>
using namespace std;

struct AVFormatContext;
struct AVPacket;
struct AVFrame;
struct SwsContext;
struct SwrContext;
struct AVCodecContext;
struct AVCodecParameters;
class McoAVTool
{
public:
    McoAVTool();
    /*
     * 打开媒体文件
     * 0-success;-1-error
    */
    bool openAVFile(const char *url);

    /*
     * 打开解码器
     * 0-success;-1-error
    */
    bool openCodec();
	bool openCodecFromParamters(AVCodecParameters *par);

    /*
     * 读取媒体文件
     * 0-success;-1-error
    */
    bool readPacket(bool *pktIsVideo,qint64 *time);
	
	/*
	* 缓冲读取
	*@param len 缓冲区长度
	*@param vl 其中视频的数量
	*@param al 其中音频的数量
	* 0-success;-1-error
	*/
	bool readPacketsToBuff(int len,int *vl,int *al);

    /*
     * 关闭媒体文件
    */
    void closeAVFile();
    /*
     * 解码
    */
    bool decoder(bool *pktIsVideo);
	bool autoDecoder(bool *pktIsVideo);

    /*
     * 转码
     * @param outdata:转码输出控件
     * @param width
     * @param height
     * @return bool 结果
    */
    bool getRGB(char *outdata,int width,int height);

	/*
	* 播放音频
	*/
	long long getPCM(char *outdata, int len);

	//位移
	bool seek(double pos);

	qint64 getNextVideoTime();
	qint64 getNextAudioTime();
	qint64 getNextTime();

    string getErrorInfo();
	int getWidth();
	int getHeight();
	int getDuration();
	int getFps();
	int getChannels();
	int getSampleRate();
	int getVideoStream();
	int getAudioStream();
	double getSpeed();
	void setSpeed(double s);
private:
	void clearPacketLists(QList<AVPacket *> *l);
    mutex lock;
    AVFormatContext *ps = NULL;//封装的上下文
    SwsContext *swsCtx = NULL;//视频转码器
	SwrContext *swrCtx = NULL;//音频解码器
	AVCodecContext *vCodecContext = NULL;
	AVCodecContext *aCodecContext = NULL;
    char errorBuff[1024];//错误信息数组
    int videoStream = -1;
    int audioStream = -1;
	int width = 0;
	int height = 0;
	int duration = 0;
	int fps = 0;
	int sampleRate = 0;
	int channels = 0;
	int channelLayout = 0;
	double speed = 1;
    AVPacket *pkt = NULL;
    AVFrame *vFrame = NULL;
    AVFrame *aFrame = NULL;
	QList<AVPacket *> videoPacketList;
	QList<AVPacket *> audioPacketList;

};

#endif // MCOAVTOOL_H
