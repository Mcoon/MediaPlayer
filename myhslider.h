#ifndef MYHSLIDER_H
#define MYHSLIDER_H

#include <QSlider>

class MyHSlider : public QSlider
{
    Q_OBJECT
public:
    explicit MyHSlider(QWidget *parent = nullptr);

protected:
    void mousePressEvent(QMouseEvent *ev);
};

#endif // MYHSLIDER_H
