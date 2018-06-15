#include "McoAVPlay01.h"
#include <iostream>
#include <qfiledialog.h>
using namespace std;
QString fileName;
McoAVPlay01::McoAVPlay01(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	connect(ui.playBtn, SIGNAL(clicked()), this, SLOT(startPlay()));
	connect(ui.openBtn, SIGNAL(clicked()), this, SLOT(openFile()));
	connect(ui.suspendBtn, SIGNAL(clicked()), this, SLOT(suspend()));
	connect(ui.continueBtn, SIGNAL(clicked()), this, SLOT(continuePlay()));
	connect(ui.screenShotBtn, SIGNAL(clicked()), this, SLOT(screenShot()));
	fileName = "";
}
void McoAVPlay01::startPlay()
{
	if (fileName.length() == 0)
		this->openFile();

	char*  ch;
	QByteArray ba = fileName.toLocal8Bit();//有中文要使用这个
	ch = ba.data();
	if (!ui.openGLWidget->initGL(ch)) cout << "init error" << endl;
	else
	{
		cout << "init success" << endl;
		ui.openGLWidget->startPlay();
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
