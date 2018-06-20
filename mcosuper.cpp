#include "mcosuper.h"
extern "C"
{
    #include <libavformat/avformat.h>
	#include <libavutil\time.h>
}
#pragma comment(lib,"avformat.lib")
#pragma comment(lib,"avutil.lib")
McoSuper::McoSuper()
{
    //注册所有封装格式
    av_register_all();
    //网络格式支持
    avformat_network_init();
}



McoSuper *McoSuper::getObject()
{
    static McoSuper m;
    return &m;
}

long long McoSuper::getNowTime()
{
	long long time = av_gettime() / 1000;
	return time;
}
