#ifndef MCOAUDIO_H
#define MCOAUDIO_H

#include <mutex>
#include <QAudioOutput>
#include <qiodevice.h>
using namespace std;

class McoAudio
{
public:
	McoAudio();
	~McoAudio();
	bool initAudio(int sampleRate, int channels);
	bool freeAudio();
	qint64 writeToAudio(char *data,qint64 len);
	int getAudioFreeBytes();
	//设置音量
	void setVoiceNum(double v);
	//获取音量
	double getVoiceNum();
protected:
	QAudioOutput *output = NULL;
	QIODevice *io = NULL;
	mutex lock;
};

#endif // MCOAVTOOL_H