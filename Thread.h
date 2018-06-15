#ifndef THREAD_H  
#define THREAD_H 

#include <QThread>
#include "mcoavtool.h"
#include <qtimer.h>

class Thread : public QThread
{
	Q_OBJECT
public:
	McoAVTool tool;
	Thread();
	~Thread();
	virtual void run();
	bool openAVFile(const char *url);
	bool openCodec();
	bool getRGB(char *outdata, int width, int height);
	int width = 0;
	int height = 0;
	int duration = 0;
	int fps = 0;
	void suspend(bool b);
	void close();
private:
	bool isSuspend;
	bool isExited;

signals:
	void sendMsg();

};

#endif // THREAD_H  