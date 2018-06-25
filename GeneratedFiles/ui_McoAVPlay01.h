/********************************************************************************
** Form generated from reading UI file 'McoAVPlay01.ui'
**
** Created by: Qt User Interface Compiler version 5.11.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MCOAVPLAY01_H
#define UI_MCOAVPLAY01_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QWidget>
#include "mcoglwidget.h"
#include "myhslider.h"

QT_BEGIN_NAMESPACE

class Ui_McoAVPlay01Class
{
public:
    McoGLWidget *openGLWidget;
    QWidget *controlWidget;
    QPushButton *playBtn;
    QPushButton *continueBtn;
    QPushButton *suspendBtn;
    QPushButton *screenShotBtn;
    QPushButton *openBtn;
    MyHSlider *playSlider;
    QLabel *leftLable;
    QLabel *rightLable;
    QPushButton *speedLessBtn;
    QPushButton *speedMoreBtn;
    QLabel *speedLessLable;
    QLabel *speedMoreLeble;
    QSlider *voiceSlider;

    void setupUi(QWidget *McoAVPlay01Class)
    {
        if (McoAVPlay01Class->objectName().isEmpty())
            McoAVPlay01Class->setObjectName(QStringLiteral("McoAVPlay01Class"));
        McoAVPlay01Class->resize(672, 488);
        openGLWidget = new McoGLWidget(McoAVPlay01Class);
        openGLWidget->setObjectName(QStringLiteral("openGLWidget"));
        openGLWidget->setGeometry(QRect(0, 0, 601, 391));
        openGLWidget->setStyleSheet(QLatin1String("McoGLWidget:focus QPushButton{\n"
"border-color: #606060;\n"
"border-width: 2px;\n"
"border-style: solid;\n"
"color:#ffffff;\n"
"}"));
        controlWidget = new QWidget(McoAVPlay01Class);
        controlWidget->setObjectName(QStringLiteral("controlWidget"));
        controlWidget->setGeometry(QRect(0, 320, 601, 71));
        playBtn = new QPushButton(controlWidget);
        playBtn->setObjectName(QStringLiteral("playBtn"));
        playBtn->setGeometry(QRect(220, 40, 75, 23));
        playBtn->setStyleSheet(QLatin1String("QPushButton{\n"
"border-color: #606060;\n"
"border-width: 2px;\n"
"border-style: solid;\n"
"color:#ffffff;\n"
"}\n"
"\n"
"QPushButton:focus{\n"
"border-color: #800000;\n"
"border-width: 2px;\n"
"border-style: solid;\n"
"color:#ffffff;\n"
"}"));
        continueBtn = new QPushButton(controlWidget);
        continueBtn->setObjectName(QStringLiteral("continueBtn"));
        continueBtn->setGeometry(QRect(300, 40, 75, 23));
        continueBtn->setStyleSheet(QLatin1String("QPushButton{\n"
"border-color: #606060;\n"
"border-width: 2px;\n"
"border-style: solid;\n"
"color:#ffffff;\n"
"}\n"
"\n"
"QPushButton:focus{\n"
"border-color: #800000;\n"
"border-width: 2px;\n"
"border-style: solid;\n"
"color:#ffffff;\n"
"}"));
        suspendBtn = new QPushButton(controlWidget);
        suspendBtn->setObjectName(QStringLiteral("suspendBtn"));
        suspendBtn->setGeometry(QRect(130, 40, 75, 23));
        suspendBtn->setStyleSheet(QLatin1String("QPushButton{\n"
"border-color: #606060;\n"
"border-width: 2px;\n"
"border-style: solid;\n"
"color:#ffffff;\n"
"}\n"
"\n"
"QPushButton:focus{\n"
"border-color: #800000;\n"
"border-width: 2px;\n"
"border-style: solid;\n"
"color:#ffffff;\n"
"}"));
        screenShotBtn = new QPushButton(controlWidget);
        screenShotBtn->setObjectName(QStringLiteral("screenShotBtn"));
        screenShotBtn->setGeometry(QRect(380, 40, 51, 23));
        screenShotBtn->setStyleSheet(QLatin1String("QPushButton{\n"
"border-color: #606060;\n"
"border-width: 2px;\n"
"border-style: solid;\n"
"color:#ffffff;\n"
"}\n"
"\n"
"QPushButton:focus{\n"
"border-color: #800000;\n"
"border-width: 2px;\n"
"border-style: solid;\n"
"color:#ffffff;\n"
"}"));
        openBtn = new QPushButton(controlWidget);
        openBtn->setObjectName(QStringLiteral("openBtn"));
        openBtn->setGeometry(QRect(440, 40, 61, 23));
        openBtn->setStyleSheet(QLatin1String("QPushButton{\n"
"border-color: #606060;\n"
"border-width: 2px;\n"
"border-style: solid;\n"
"color:#ffffff;\n"
"}\n"
"\n"
"QPushButton:focus{\n"
"border-color: #800000;\n"
"border-width: 2px;\n"
"border-style: solid;\n"
"color:#ffffff;\n"
"}"));
        playSlider = new MyHSlider(controlWidget);
        playSlider->setObjectName(QStringLiteral("playSlider"));
        playSlider->setGeometry(QRect(60, 10, 471, 22));
        playSlider->setStyleSheet(QLatin1String("QSlider::add-page:Horizontal\n"
"     {\n"
"        background-color: rgb(87, 97, 106);\n"
"        height:4px;\n"
"     }\n"
"     QSlider::sub-page:Horizontal\n"
"    {\n"
"        background-color:qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(231,80,229, 255), stop:1 rgba(7,208,255, 255));\n"
"        height:4px; \n"
"     } \n"
"    QSlider::groove:Horizontal \n"
"    {\n"
"        background:transparent;\n"
"        height:6px;  \n"
"    } \n"
"    QSlider::handle:Horizontal  \n"
"    { \n"
"    background: qradialgradient(spread:pad, cx:0.5, cy:0.5, radius:0.5, fx:0.5, fy:0.5,   \n"
"    stop:0.6 #45ADED, stop:0.778409 rgba(255, 255, 255, 255));  \n"
"  \n"
"    width: 11px;  \n"
"    margin-top: -3px;  \n"
"    margin-bottom: -3px;  \n"
"    border-radius: 5px;   \n"
"    }"));
        playSlider->setOrientation(Qt::Horizontal);
        leftLable = new QLabel(controlWidget);
        leftLable->setObjectName(QStringLiteral("leftLable"));
        leftLable->setGeometry(QRect(10, 40, 71, 20));
        leftLable->setAlignment(Qt::AlignCenter);
        rightLable = new QLabel(controlWidget);
        rightLable->setObjectName(QStringLiteral("rightLable"));
        rightLable->setGeometry(QRect(530, 40, 61, 20));
        rightLable->setAlignment(Qt::AlignCenter);
        speedLessBtn = new QPushButton(controlWidget);
        speedLessBtn->setObjectName(QStringLiteral("speedLessBtn"));
        speedLessBtn->setGeometry(QRect(10, 10, 21, 23));
        speedLessBtn->setStyleSheet(QLatin1String("QPushButton{\n"
"border-color: #606060;\n"
"border-width: 2px;\n"
"border-style: solid;\n"
"color:#ffffff;\n"
"}\n"
"\n"
"QPushButton:focus{\n"
"border-color: #800000;\n"
"border-width: 2px;\n"
"border-style: solid;\n"
"color:#ffffff;\n"
"}"));
        speedMoreBtn = new QPushButton(controlWidget);
        speedMoreBtn->setObjectName(QStringLiteral("speedMoreBtn"));
        speedMoreBtn->setGeometry(QRect(570, 10, 21, 23));
        speedMoreBtn->setStyleSheet(QLatin1String("QPushButton{\n"
"border-color: #606060;\n"
"border-width: 2px;\n"
"border-style: solid;\n"
"color:#ffffff;\n"
"}\n"
"\n"
"QPushButton:focus{\n"
"border-color: #800000;\n"
"border-width: 2px;\n"
"border-style: solid;\n"
"color:#ffffff;\n"
"}"));
        speedLessLable = new QLabel(controlWidget);
        speedLessLable->setObjectName(QStringLiteral("speedLessLable"));
        speedLessLable->setGeometry(QRect(36, 10, 20, 21));
        speedLessLable->setAlignment(Qt::AlignCenter);
        speedMoreLeble = new QLabel(controlWidget);
        speedMoreLeble->setObjectName(QStringLiteral("speedMoreLeble"));
        speedMoreLeble->setGeometry(QRect(540, 10, 20, 21));
        speedMoreLeble->setAlignment(Qt::AlignCenter);
        voiceSlider = new QSlider(controlWidget);
        voiceSlider->setObjectName(QStringLiteral("voiceSlider"));
        voiceSlider->setGeometry(QRect(360, 30, 160, 22));
        voiceSlider->setStyleSheet(QLatin1String("QSlider::add-page:Horizontal\n"
"     {\n"
"        background-color: rgb(87, 97, 106);\n"
"        height:4px;\n"
"     }\n"
"     QSlider::sub-page:Horizontal\n"
"    {\n"
"        background-color:qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(231,80,229, 255), stop:1 rgba(7,208,255, 255));\n"
"        height:4px; \n"
"     } \n"
"    QSlider::groove:Horizontal \n"
"    {\n"
"        background:transparent;\n"
"        height:6px;  \n"
"    } \n"
"    QSlider::handle:Horizontal  \n"
"    { \n"
"    background: qradialgradient(spread:pad, cx:0.5, cy:0.5, radius:0.5, fx:0.5, fy:0.5,   \n"
"    stop:0.6 #45ADED, stop:0.778409 rgba(255, 255, 255, 255));  \n"
"  \n"
"    width: 11px;  \n"
"    margin-top: -3px;  \n"
"    margin-bottom: -3px;  \n"
"    border-radius: 5px;   \n"
"    }"));
        voiceSlider->setMaximum(100);
        voiceSlider->setValue(100);
        voiceSlider->setOrientation(Qt::Horizontal);
        voiceSlider->setTickPosition(QSlider::NoTicks);
        playSlider->raise();
        leftLable->raise();
        rightLable->raise();
        speedLessBtn->raise();
        speedMoreBtn->raise();
        speedLessLable->raise();
        speedMoreLeble->raise();
        suspendBtn->raise();
        playBtn->raise();
        continueBtn->raise();
        screenShotBtn->raise();
        openBtn->raise();
        voiceSlider->raise();

        retranslateUi(McoAVPlay01Class);

        QMetaObject::connectSlotsByName(McoAVPlay01Class);
    } // setupUi

    void retranslateUi(QWidget *McoAVPlay01Class)
    {
        McoAVPlay01Class->setWindowTitle(QApplication::translate("McoAVPlay01Class", "Mco Player", nullptr));
        playBtn->setText(QApplication::translate("McoAVPlay01Class", "\345\274\200\345\247\213\346\222\255\346\224\276", nullptr));
        continueBtn->setText(QApplication::translate("McoAVPlay01Class", "\346\222\255\346\224\276", nullptr));
        suspendBtn->setText(QApplication::translate("McoAVPlay01Class", "\346\232\202\345\201\234", nullptr));
        screenShotBtn->setText(QApplication::translate("McoAVPlay01Class", "\346\210\252\345\233\276", nullptr));
        openBtn->setText(QApplication::translate("McoAVPlay01Class", "\346\211\223\345\274\200\346\226\207\344\273\266", nullptr));
        leftLable->setText(QApplication::translate("McoAVPlay01Class", "00:00:00", nullptr));
        rightLable->setText(QApplication::translate("McoAVPlay01Class", "00:00:00", nullptr));
        speedLessBtn->setText(QApplication::translate("McoAVPlay01Class", "<<", nullptr));
        speedMoreBtn->setText(QApplication::translate("McoAVPlay01Class", ">>", nullptr));
        speedLessLable->setText(QApplication::translate("McoAVPlay01Class", "x1", nullptr));
        speedMoreLeble->setText(QApplication::translate("McoAVPlay01Class", "x1", nullptr));
    } // retranslateUi

};

namespace Ui {
    class McoAVPlay01Class: public Ui_McoAVPlay01Class {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MCOAVPLAY01_H
