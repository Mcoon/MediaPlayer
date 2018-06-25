#include "McoAVPlay01.h"
#include <iostream>
#include <qfiledialog.h>
#include <QPropertyAnimation>
using namespace std;

QString fileName;
bool isPlaying = false;
bool isHide = false;
QPropertyAnimation *anim = NULL;
double speed = 1;

McoAVPlay01::McoAVPlay01(QWidget *parent)
	: QWidget(parent)
{
	setWindowIcon(QIcon("icon.png"));
	setMinimumSize(672, 488);
	//setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
	ui.setupUi(this);
	connect(ui.playBtn, SIGNAL(clicked()), this, SLOT(startPlay()));
	connect(ui.openBtn, SIGNAL(clicked()), this, SLOT(openFile()));
	connect(ui.suspendBtn, SIGNAL(clicked()), this, SLOT(suspend()));
	connect(ui.continueBtn, SIGNAL(clicked()), this, SLOT(continuePlay()));
	connect(ui.screenShotBtn, SIGNAL(clicked()), this, SLOT(screenShot()));
	connect(ui.openGLWidget,SIGNAL(videoTimeChanged(int)),this,SLOT(videoTimeChangedSlot(int)));
	connect(ui.playSlider, SIGNAL(sliderPressed()), this, SLOT(sliderValueChanged()));
	connect(ui.voiceSlider, SIGNAL(sliderPressed()), this, SLOT(VoiceNumChangde()));
	connect(ui.speedLessBtn, SIGNAL(clicked()), this, SLOT(SpeedChangedLower()));
	connect(ui.speedMoreBtn, SIGNAL(clicked()), this, SLOT(SpeedChangedBigger()));
	fileName = "";
	ui.openGLWidget->resize(QSize(width(), height()));
	ui.openGLWidget->setMouseTracking(true);
	this->setMouseTracking(true);
	ui.controlWidget->setGeometry(0, height() - 66, width(), 66);
	ui.controlWidget->setAttribute(Qt::WA_TranslucentBackground, true);
	QPalette pa;
	pa.setColor(QPalette::WindowText, Qt::white);
	ui.leftLable->setPalette(pa);
	ui.rightLable->setPalette(pa);
	ui.speedLessLable->setPalette(pa);
	ui.speedMoreLeble->setPalette(pa);
	anim = new QPropertyAnimation(ui.controlWidget, "pos");
	
}

void McoAVPlay01::startPlay()
{
	if (fileName.length() == 0)
		this->openFile();

	if (!ui.openGLWidget->openMedia(fileName)) cout << "init error" << endl;
	else
	{
		cout << "init success" << endl;
		mediaTime = ui.openGLWidget->getMediaTime();
		speed = 1;
		ui.openGLWidget->setSpeed(speed);
		ui.speedMoreLeble->setText("x1");
		ui.speedLessLable->setText("x1");
		ui.openGLWidget->startPlay();
		isPlaying = true;
	}
}

void McoAVPlay01::closeEvent(QCloseEvent * e)
{
	ui.openGLWidget->close();
}

void McoAVPlay01::openFile()
{
	QString str;
	str = QFileDialog::getOpenFileName(this,"Open AV File","D:/","Medio(*.mp4 *.wmv);;All File(*.*)");
	if (str.length() != 0)
	{
		QString tit = "McoAVPlay01 " + str + " - Opened";
		this->setWindowTitle(tit);
		fileName = str;
	}
	
}

void McoAVPlay01::suspend()
{
	ui.openGLWidget->suspend(true);
}

void McoAVPlay01::continuePlay()
{
	ui.openGLWidget->suspend(false);
}

void McoAVPlay01::screenShot()
{
	QString str = QFileDialog::getSaveFileName(this, "Save the ScreenShot", "D:/", "Picture(*.jpg *.png)");
	if (str.length() != 0)
	{
		char*  ch;
		QByteArray ba = str.toLocal8Bit();//有中文要使用这个
		ch = ba.data();
		ui.openGLWidget->saveScreenShot(ch);
	}
}

void McoAVPlay01::resizeEvent(QResizeEvent * e)
{
	if (isHide) isHide = false;
	ui.openGLWidget->resize(QSize(width(), height()));
	ui.controlWidget->setGeometry(0, height() - 66, width(), 66);
	ui.rightLable->setGeometry(width() - ui.leftLable->width() - 10, ui.leftLable->y(), ui.leftLable->width(), ui.leftLable->height());
	ui.suspendBtn->setGeometry(width() / 2 - 66 - 75 - 75 -20, ui.leftLable->y() - 6, 75,23);
	ui.playBtn->setGeometry(width() / 2 - 66 - 75 -10, ui.leftLable->y() - 6, 75, 23);
	ui.continueBtn->setGeometry(width() / 2 - 66, ui.leftLable->y() - 6, 75, 23);
	ui.screenShotBtn->setGeometry(width() / 2 - 66 + 75 + 10, ui.leftLable->y() - 6, 75, 23);
	ui.openBtn->setGeometry(width() / 2 - 66 + 75 + 75 + 20, ui.leftLable->y() - 6, 75, 23);
	ui.speedMoreBtn->setGeometry(width() - ui.speedMoreBtn->width() - 10, ui.speedLessBtn->y(),21,23);
	ui.speedMoreLeble->setGeometry(width() - ui.speedMoreLeble->width() - ui.speedMoreBtn->width() - 12, ui.speedLessLable->y(), 20, 21);
	ui.playSlider->resize(ui.speedMoreLeble->x() - ui.playSlider->x()-10,22);
	ui.voiceSlider->setGeometry(ui.openBtn->x() + ui.openBtn->width() + 1, ui.openBtn->y(), ui.rightLable->x() - ui.openBtn->x() - 66, 22);
	QWidget::resizeEvent(e);
}

void McoAVPlay01::mouseMoveEvent(QMouseEvent *e)
{
	if (isPlaying)
	{		
		if (e->pos().y() < ui.openGLWidget->pos().y() + ui.openGLWidget->height() - 66)
		{
			if (isHide) return;
			isHide = true;
			anim->setDuration(666);
			anim->setStartValue(QPoint(0, ui.controlWidget->pos().y()));
			anim->setEndValue(QPoint(0, ui.openGLWidget->pos().y() + ui.openGLWidget->height()+66));
			anim->start();
		}
		else if (e->pos().y() > ui.openGLWidget->pos().y() + ui.openGLWidget->height() - 66)
		{
			if (!isHide) return;
			isHide = false;
			anim->setDuration(666);
			anim->setStartValue(QPoint(0, ui.controlWidget->pos().y()));
			anim->setEndValue(QPoint(0, ui.openGLWidget->pos().y() + ui.openGLWidget->height() - 66));
			anim->start();
			
		}
	}
	

}

void McoAVPlay01::videoTimeChangedSlot(int t)
{
	int hh, mm, ss;
	hh = t / 3600;
	mm = t % 3600 / 60;
	ss = t % 60;
	QString str;
	str = tr("%1:%2:%3")
		.arg(hh, 2, 10, QLatin1Char('0'))
		.arg(mm, 2, 10, QLatin1Char('0'))
		.arg(ss, 2, 10, QLatin1Char('0'));
	hh = mediaTime / 3600;
	mm = mediaTime % 3600 / 60;
	ss = mediaTime % 60;
	ui.leftLable->setText(str);
	str = tr("%1:%2:%3")
		.arg(hh, 2, 10, QLatin1Char('0'))
		.arg(mm, 2, 10, QLatin1Char('0'))
		.arg(ss, 2, 10, QLatin1Char('0'));
	ui.rightLable->setText(str);
	int value = (double)t /
		(double)mediaTime *
		101;
	if (ui.playSlider->value()
		!= value)
		ui.playSlider
		->setValue(value);
	
}

void McoAVPlay01::sliderValueChanged()
{
	double pos;
	pos = (double)ui.playSlider->value() /(double)ui.playSlider->maximum();
	ui.openGLWidget->seekMedia(pos);
}

void McoAVPlay01::SpeedChangedBigger()
{
	if(speed += 0.25) ui.openGLWidget->setSpeed(speed);
	QString str; 
	str.append(QString::number(speed, 10, 1));
		
	ui.speedMoreLeble->setText(str);
	ui.speedLessLable->setText(str);
}

void McoAVPlay01::SpeedChangedLower()
{
	if (speed -= 0.25)
	{
		if (speed <= 0.75) speed = 0.75;

		ui.openGLWidget->setSpeed(speed);
	}
	QString str;
	str.append(QString::number(speed, 10, 1));

	ui.speedMoreLeble->setText(str);
	ui.speedLessLable->setText(str);
}

void McoAVPlay01::VoiceNumChangde()
{
	ui.openGLWidget->setVoiceNum((double)ui.voiceSlider->value() / (double)ui.voiceSlider->maximum());
}
