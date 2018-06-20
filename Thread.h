#ifndef THREAD_H  
#define THREAD_H 

#include <QThread>
#include "mcoavtool.h"
#include "McoAudio.h"
class Thread : public QThread
{
	Q_OBJECT
public:
	Thread(McoAVTool *t,McoAudio *a);
	~Thread();

	bool startThread();
	void stopThread();
	void suspend(bool b);

protected:
	virtual void run();
private:
	McoAVTool * tool;
	McoAudio *audio;
	int oT = 0; //¾ÉµÄÊ±¼ä
	bool isSuspend;
	bool isExited;
	qint64 startTime;
	qint64 stopTime;

signals:
	void videoTimeChanged(int time);

};

#endif // THREAD_H  