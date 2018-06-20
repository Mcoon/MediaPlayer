#include "myhslider.h"
#include <QMouseEvent>
MyHSlider::MyHSlider(QWidget *parent) : QSlider(parent)
{

}

void MyHSlider::mousePressEvent(QMouseEvent *ev)
{
    int value = ev->x() *
            (this->maximum() + 1) /
            this->width() ;
    this->setValue(value);
    QSlider::mousePressEvent(ev);
}
