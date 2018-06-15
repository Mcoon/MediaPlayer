#ifndef MCOAVTOOL_H
#define MCOAVTOOL_H

#include <QMutex>
#include <string>
#include <qlist.h>
using namespace std;

struct AVFormatContext;
struct AVPacket;
struct AVFrame;
struct SwsContext;
struct AVCodecContext;
class McoAVTool
{
public:
    int width = 0;
    int height = 0;
    int duration = 0;
    int fps = 0;
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

    string getErrorInfo();
private:
    QMutex mutex;
    AVFormatContext *ps = NULL;//封装的上下文
    SwsContext *swsCtx;//转码器
	AVCodecContext *vCodecContext;
	AVCodecContext *aCodecContext;
    char errorBuff[1024];//错误信息数组
    int videoStream = -1;
    int audioStream = -1;
    AVPacket *pkt = NULL;
    AVFrame *vFrame = NULL;
    AVFrame *aFrame = NULL;
};

#endif // MCOAVTOOL_H
