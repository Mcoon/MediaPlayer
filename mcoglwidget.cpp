#include "mcoglwidget.h"
#include <QPainter>
#include <iostream>
#include <qmessagebox.h>
using namespace std;

McoGLWidget::McoGLWidget(QWidget *parent) : QOpenGLWidget(parent)
{
	image = NULL;
	timer = new QTimer();
	connect(timer, SIGNAL(timeout()), this, SLOT(play()));
}

void McoGLWidget::paintEvent(QPaintEvent *e)
{
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

		timer->start(1000 / thread.fps);
		play();
	}

	QOpenGLWidget::resizeEvent(e);
	
}

void McoGLWidget::startPlay()
{
	timer->start(1000/thread.fps);
	thread.start();
}

void McoGLWidget::suspend(bool b)
{
	if(b) timer->stop();
	else timer->start(1000 / thread.fps);
	thread.suspend(b);
}

void McoGLWidget::saveScreenShot(char * saveurl)
{
	if (!image)
	{
		QMessageBox::critical(this, "save error", "please play the video first", "ok");
		return;
	}
	suspend(true);
	if (image->save(saveurl)) QMessageBox::information(this,"save success","success","ok");
	else QMessageBox::critical(this,"save error", "error,try again", "ok");
	suspend(false);
}

void McoGLWidget::updateImage()
{
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
		if (w * thread.height > h * thread.width)
		{
			w = h * thread.width / thread.height;
		}
		else
		{
			h = w * thread.height / thread.width;
		}

		uchar *buf = new uchar[w * h * 4];
		image = new QImage(buf, w, h, QImage::Format_ARGB32);
	}

	
}

void McoGLWidget::close()
{
	timer->stop();
	thread.close();
}

void McoGLWidget::play()
{
	if (!image) return;
	
		bool ok = thread.getRGB((char*)image->bits(), image->width(), image->height());
		if (ok)
		{
			update();
		}
}

bool McoGLWidget::initGL(const char *url)
{
	timer->stop();
	if (!thread.openAVFile(url))
	{
		//cout << thread.getErrorInfo() << endl;
		return false;
	}
	if (!thread.openCodec())
	{
		
		//cout << thread.getErrorInfo() << endl;
		return false;
	}

	updateImage();


	return true;
}
