#include "McoAudio.h"
#include <qaudioformat.h>
McoAudio::McoAudio()
{
}

McoAudio::~McoAudio()
{
}

bool McoAudio::initAudio(int sampleRate, int channels)
{
	freeAudio();
	lock.lock();
	QAudioFormat fmt;
	fmt.setSampleRate(sampleRate);
	fmt.setSampleSize(16);
	fmt.setSampleType(QAudioFormat::SignedInt);
	fmt.setChannelCount(channels);
	fmt.setCodec("audio/pcm");
	fmt.setByteOrder(QAudioFormat::LittleEndian);
	output = new QAudioOutput(fmt);
	output->setBufferSize(80000);
	io = output->start();
	if (!io)
	{
		lock.unlock();
		return false;
	}
	lock.unlock();
	return true;
	
}

bool McoAudio::freeAudio()
{
	lock.lock();

	if (output)
	{
		output->stop();
		delete output;
		output = NULL;
		io = NULL;
	}


	lock.unlock();
	return true;
}

qint64 McoAudio::writeToAudio(char * data, qint64 len)
{
	lock.lock();
	qint64 l = 0;
	if (io)
	{
		l = io->write(data, len);
	}
	lock.unlock();
	return l;
}

int McoAudio::getAudioFreeBytes()
{
	lock.lock();
	int l = 0;
	if(output) l = output->bytesFree();
	lock.unlock();
	return l;
}

void McoAudio::setVoiceNum(double v)
{
	if (output) output->setVolume(v);
}

double McoAudio::getVoiceNum()
{
	if (output) return output->volume();
	else 1;
}