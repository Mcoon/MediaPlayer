#pragma once

#include <QtWidgets/QWidget>
#include "ui_McoAVPlay01.h"
#include<QMouseEvent>
#include<QWheelEvent>
class McoAVPlay01 : public QWidget
{
	Q_OBJECT

public:
	McoAVPlay01(QWidget *parent = Q_NULLPTR);

private:
	Ui::McoAVPlay01Class ui;
	int mediaTime = 0;
	void closeEvent(QCloseEvent *e);
private slots:
	void startPlay();
	void openFile();
	void suspend();
	void continuePlay();
	void screenShot();
	void resizeEvent(QResizeEvent *e);
	void mouseMoveEvent(QMouseEvent *e);
	void videoTimeChangedSlot(int t);
	void sliderValueChanged();
	void SpeedChangedBigger();
	void SpeedChangedLower();
	void VoiceNumChangde(int);
	
};
