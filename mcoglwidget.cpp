#include "mcoglwidget.h"
#include <QPainter>
#include <iostream>
#include <qmessagebox.h>
using namespace std;

McoGLWidget::McoGLWidget(QWidget *parent) : QOpenGLWidget(parent)
{
	tool = new McoAVTool;
	audio = new McoAudio;
	thread = new Thread(tool,audio);
	image = NULL;
	timer = new QTimer();
	connect(timer, SIGNAL(timeout()), this, SLOT(play()));
	connect(thread,SIGNAL(videoTimeChanged(int)),this,SIGNAL(videoTimeChanged(int)));
}

void McoGLWidget::paintEvent(QPaintEvent *e)
{
	//QPainter painter;
	//painter.begin(this);
	//QImage *i = new QImage("icon.png");
	//
	//painter.drawImage(QRect((width()- (i->width() / 3))/2,(height()-(i->height() / 3))/2, i->width()/3,i->height()/3),*i);
	//painter.end();

    if(image != NULL)
    { 
		QPainter painter;
		painter.begin(this);
		painter.drawImage((width()- image->width())/2,(height() - image->height())/2, *image);
		painter.end();
    }


}

void McoGLWidget::resizeEvent(QResizeEvent * e)
{
	
	if (image)
	{
		timer->stop();
		updateImage();
		timer->start(20);
		play();
	}

	QOpenGLWidget::resizeEvent(e);
	
}

void McoGLWidget::startPlay()
{
	timer->start(20);
	thread->startThread();
}

void McoGLWidget::suspend(bool b)
{
	if(b) timer->stop();
	else timer->start(20);
	thread->suspend(b);
}

void McoGLWidget::saveScreenShot(char * saveurl)
{
	if (!image)
	{
		QMessageBox::critical(this, "save error", "please play the video first", "ok");
		return;
	}
	timer->stop();
	suspend(true);
	if (image->save(saveurl)) QMessageBox::information(this,"save success","success","ok");
	else QMessageBox::critical(this,"save error", "error,try again", "ok");
	suspend(false);
	timer->start(20);
	play();
}

void McoGLWidget::updateImage()
{

	if (tool->getWidth() == 0 || tool->getHeight() == 0 || width() == 0 || height() == 0) return;

	if (image)
	{
		delete image->bits();
		delete image;
		image = NULL;
	}
	if (!image)
	{
		int w = width();
		int h = height();
		if (w * tool->getHeight() > h * tool->getWidth())
		{
			w = h * tool->getWidth() / tool->getHeight();
		}
		else
		{
			h = w * tool->getHeight() / tool->getWidth();
		}

		w = (w >> 4) << 4;
		h = (h >> 4) << 4;

		uchar *buf = new uchar[w * h * 4];
		image = new QImage(buf, w, h, QImage::Format_ARGB32);
	}

	
}

double McoGLWidget::getSpeed()
{
	return tool->getSpeed();
}

void McoGLWidget::setSpeed(double s)
{
	tool->setSpeed(s);
	if (tool->getAudioStream() != -1) audio->initAudio(tool->getSampleRate(), tool->getChannels());
}

void McoGLWidget::closeMedia()
{
	thread->stopThread();
	audio->freeAudio();
	tool->closeAVFile();
	timer->stop();
}

bool McoGLWidget::seekMedia(double pos)
{
	return tool->seek(pos);
}

double McoGLWidget::getPos()
{
	int dur = tool->getDuration();
	if (dur <= 0)
		return 0;
	int now = tool->getNextVideoTime();
	if (now < 0)
		return 0;
	double re = (double)now / (double)dur;
	return re;
}

int McoGLWidget::getNowTime()
{
	return tool->getNextVideoTime() / 1000;
}

int McoGLWidget::getMediaTime()
{
	return tool->getDuration() / 1000;
}

void McoGLWidget::setVoiceNum(double v)
{
	audio->setVoiceNum(v);
}

double McoGLWidget::getVoiceNum()
{
	return audio->getVoiceNum();
}

void McoGLWidget::play()
{
	if (!image) return;
	if (!tool->getRGB((char*)image->bits(), image->width(), image->height())) return;
	
	update();
}

bool McoGLWidget::openMedia(QString path)
{
	closeMedia();
	if (!tool->openAVFile(path.toStdString().c_str()))
	{
		return false;
	}
	if (tool->getAudioStream() != -1)
	{
		if (!audio->initAudio(tool->getSampleRate(), tool->getChannels()))
			return false;
	}
	if (!tool->openCodec())
	{
		return false;
	}
	if (!thread->startThread())
	{
		return false;
	}

	updateImage();
	timer->start(20);


	return true;
}
