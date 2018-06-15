#pragma once

#include <QtWidgets/QWidget>
#include "ui_McoAVPlay01.h"

class McoAVPlay01 : public QWidget
{
	Q_OBJECT

public:
	McoAVPlay01(QWidget *parent = Q_NULLPTR);

private:
	Ui::McoAVPlay01Class ui;
	void closeEvent(QCloseEvent *e);
private slots:
	void startPlay();
	void openFile();
	void suspend();
	void continuePlay();
	void screenShot();
};
