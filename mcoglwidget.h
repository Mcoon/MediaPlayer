#ifndef MCOGLWIDGET_H
#define MCOGLWIDGET_H

#include <QOpenGLWidget>
#include <qtimer.h>
#include "Thread.h"
#include "mcoavtool.h"
#include "McoAudio.h"
class McoGLWidget : public QOpenGLWidget
{
	Q_OBJECT
		enum MediaSpeed
		{
			Speed_x0_8,
			Speed_x1,
			Speed_x1_25,
			Speed_x1_5,
			Speed_x1_75,
			Speed_x2,
			Speed_Undefined
		};
public:
    explicit McoGLWidget(QWidget *parent = nullptr);

	bool openMedia(QString path);

	void startPlay();

	void suspend(bool b);

	void saveScreenShot(char* saveurl);

	void closeMedia();

	bool seekMedia(double pos);
	double getPos();
	//·µ»Øs
	int getNowTime();
	//·µ»Øs
	int getMediaTime();

	void setVoiceNum(double v);
	
	double getVoiceNum();
	double getSpeed();
	void setSpeed(double s);

private:
    QImage *image;
	QTimer *timer;
	Thread  *thread;
	McoAVTool *tool;
	McoAudio *audio;
	void paintEvent(QPaintEvent *e);
	void resizeEvent(QResizeEvent *e);
	void updateImage();
	
signals:
	void videoTimeChanged(int time);
private slots:
	void play();

};

#endif // MCOGLWIDGET_H
