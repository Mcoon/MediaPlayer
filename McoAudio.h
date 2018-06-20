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
	//��������
	void setVoiceNum(double v);
	//��ȡ����
	double getVoiceNum();
protected:
	QAudioOutput *output = NULL;
	QIODevice *io = NULL;
	mutex lock;
};

#endif // MCOAVTOOL_H