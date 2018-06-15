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
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>
#include "mcoglwidget.h"

QT_BEGIN_NAMESPACE

class Ui_McoAVPlay01Class
{
public:
    QGridLayout *gridLayout;
    McoGLWidget *openGLWidget;
    QPushButton *playBtn;
    QPushButton *continueBtn;
    QPushButton *suspendBtn;
    QPushButton *screenShotBtn;
    QPushButton *openBtn;

    void setupUi(QWidget *McoAVPlay01Class)
    {
        if (McoAVPlay01Class->objectName().isEmpty())
            McoAVPlay01Class->setObjectName(QStringLiteral("McoAVPlay01Class"));
        McoAVPlay01Class->resize(600, 388);
        gridLayout = new QGridLayout(McoAVPlay01Class);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        openGLWidget = new McoGLWidget(McoAVPlay01Class);
        openGLWidget->setObjectName(QStringLiteral("openGLWidget"));

        gridLayout->addWidget(openGLWidget, 0, 0, 1, 5);

        playBtn = new QPushButton(McoAVPlay01Class);
        playBtn->setObjectName(QStringLiteral("playBtn"));

        gridLayout->addWidget(playBtn, 1, 0, 1, 1);

        continueBtn = new QPushButton(McoAVPlay01Class);
        continueBtn->setObjectName(QStringLiteral("continueBtn"));

        gridLayout->addWidget(continueBtn, 1, 1, 1, 1);

        suspendBtn = new QPushButton(McoAVPlay01Class);
        suspendBtn->setObjectName(QStringLiteral("suspendBtn"));

        gridLayout->addWidget(suspendBtn, 1, 2, 1, 1);

        screenShotBtn = new QPushButton(McoAVPlay01Class);
        screenShotBtn->setObjectName(QStringLiteral("screenShotBtn"));

        gridLayout->addWidget(screenShotBtn, 1, 3, 1, 1);

        openBtn = new QPushButton(McoAVPlay01Class);
        openBtn->setObjectName(QStringLiteral("openBtn"));

        gridLayout->addWidget(openBtn, 1, 4, 1, 1);


        retranslateUi(McoAVPlay01Class);

        QMetaObject::connectSlotsByName(McoAVPlay01Class);
    } // setupUi

    void retranslateUi(QWidget *McoAVPlay01Class)
    {
        McoAVPlay01Class->setWindowTitle(QApplication::translate("McoAVPlay01Class", "McoAVPlay01", nullptr));
        playBtn->setText(QApplication::translate("McoAVPlay01Class", "\345\274\200\345\247\213\346\222\255\346\224\276", nullptr));
        continueBtn->setText(QApplication::translate("McoAVPlay01Class", "\346\222\255\346\224\276", nullptr));
        suspendBtn->setText(QApplication::translate("McoAVPlay01Class", "\346\232\202\345\201\234", nullptr));
        screenShotBtn->setText(QApplication::translate("McoAVPlay01Class", "\346\210\252\345\233\276", nullptr));
        openBtn->setText(QApplication::translate("McoAVPlay01Class", "\346\211\223\345\274\200\346\226\207\344\273\266", nullptr));
    } // retranslateUi

};

namespace Ui {
    class McoAVPlay01Class: public Ui_McoAVPlay01Class {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MCOAVPLAY01_H
