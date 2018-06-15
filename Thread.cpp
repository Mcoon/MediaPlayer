#include "Thread.h"
#include <qimage.h>
#include <iostream>

extern "C"
{
#include <libavutil\time.h>
}
#pragma comment(lib,"avutil.lib")

using namespace std;


Thread::Thread()
{
	isSuspend = false;
	isExited = false;
}



Thread::~Thread()
{
}

void Thread::run()
{

	bool isVideo;
	qint64 time;
	qint64 startTime = av_gettime() / 1000;//∫¡√Î
	while (!isExited)
	{
		while (isSuspend);
		int vl, al;
		if (!tool.readPacketsToBuff(20,&vl,&al)) return;
		time = tool.getNextVideoTime();

		if (time != -1)
		{
			qint64 temp = av_gettime() / 1000;
			temp -= startTime;
			if (temp < time)
				msleep(time - temp);
		}
		

		tool.autoDecoder(&isVideo);
		
	}

}

bool Thread::openAVFile(const char * url)
{ 
	bool ok = tool.openAVFile(url);
	if (ok)
	{
		width = tool.getWidth();
		height = tool.getHeight();
		fps = tool.getFps();
		duration = tool.getDuration();
	}
	isExited = false;
	return ok;
}

bool Thread::openCodec()
{
	bool ok =  tool.openCodec();


	return ok;
}

bool Thread::getRGB(char * outdata, int width, int height)
{
	return tool.getRGB(outdata,width,height);
}

void Thread::suspend(bool b)
{
	isSuspend = b;
}

void Thread::close()
{
	isExited = true;
	wait(50);
}
