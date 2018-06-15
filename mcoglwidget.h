#ifndef MCOGLWIDGET_H
#define MCOGLWIDGET_H

#include <QOpenGLWidget>
#include <qtimer.h>
#include "Thread.h"

class McoGLWidget : public QOpenGLWidget
{
    Q_OBJECT
public:
    explicit McoGLWidget(QWidget *parent = nullptr);

    void paintEvent(QPaintEvent *e);

	void resizeEvent(QResizeEvent *e);

	bool initGL(const char *url);

	void startPlay();

	void suspend(bool b);

	void saveScreenShot(char* saveurl);

	void updateImage();

	void close();


private:
    QImage *image;
	QTimer* timer;
	Thread  thread;

signals:

private slots:
	void play();

};

#endif // MCOGLWIDGET_H
