#include "Thread.h"
#include <qimage.h>
#include <iostream>
#include "mcosuper.h"

using namespace std;


Thread::Thread(McoAVTool *t, McoAudio *a)
{
	isSuspend = false;
	isExited = false;
	startTime = 0;
	stopTime = 0;
	tool = NULL;
	tool = t;
	audio = a;
}



Thread::~Thread()
{
}

bool Thread::startThread()
{
	if (!tool || !audio) return false;

	if (!this->isRunning()) this->start();
	startTime = McoSuper::getNowTime();
	stopTime = 0;
	isExited = false;
	isSuspend = false;

	return true;
}

void Thread::stopThread()
{
	isExited = true;
}

void Thread::run()
{

	bool isVideo;
	qint64 time;
	int vl, al;
	char audioBuffer[10000];
	while (!isExited)
	{
		if (!tool || !audio)
		{
			msleep(10);
			continue;
		}
		if (isSuspend)
		{
			stopTime += 10;
			msleep(10);
			continue;
		}
		if (audio->getAudioFreeBytes() < 10000)
		{
			msleep(1);
			continue;
		}
		
		if (!tool->readPacketsToBuff(20,&vl,&al)) break;
		

		qint64 t = tool->getNextVideoTime();
		if (t != -1)
		{
			int nT = t / 1000;
			if (nT != oT)
			{
				oT = nT;
				emit videoTimeChanged(oT);
			}
		}

		if(!tool->autoDecoder(&isVideo))
		{
			msleep(1);
			continue;
		}

		if (!isVideo)
		{
			long long l =tool->getPCM(audioBuffer, 10000);
			audio->writeToAudio(audioBuffer, l);
		}

	}

}


void Thread::suspend(bool b)
{
	isSuspend = b;
}

