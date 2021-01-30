/********************************************************************************
** Form generated from reading UI file 'funstuff.ui'
**
** Created by: Qt User Interface Compiler version 5.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FUNSTUFF_H
#define UI_FUNSTUFF_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_Funstuff
{
public:
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QPushButton *pushButton_3;
    QPushButton *pushButton_4;
    QPushButton *pushButton_5;
    QPushButton *pushButton_6;
    QPushButton *pushButton_7;
    QPushButton *pushButton_8;
    QPushButton *pushButton_9;
    QPushButton *pushButton_10;
    QPushButton *pushButton_11;
    QPushButton *pushButton_12;
    QPushButton *pushButton_13;
    QPushButton *pushButton_14;
    QPushButton *pushButton_15;
    QPushButton *pushButton_16;
    QFrame *line;
    QFrame *line_2;

    void setupUi(QDialog *Funstuff)
    {
        if (Funstuff->objectName().isEmpty())
            Funstuff->setObjectName(QStringLiteral("Funstuff"));
        Funstuff->resize(215, 261);
        pushButton = new QPushButton(Funstuff);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(10, 10, 91, 23));
        pushButton_2 = new QPushButton(Funstuff);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        pushButton_2->setGeometry(QRect(110, 10, 101, 23));
        pushButton_3 = new QPushButton(Funstuff);
        pushButton_3->setObjectName(QStringLiteral("pushButton_3"));
        pushButton_3->setGeometry(QRect(10, 40, 91, 23));
        pushButton_4 = new QPushButton(Funstuff);
        pushButton_4->setObjectName(QStringLiteral("pushButton_4"));
        pushButton_4->setGeometry(QRect(110, 40, 101, 23));
        pushButton_5 = new QPushButton(Funstuff);
        pushButton_5->setObjectName(QStringLiteral("pushButton_5"));
        pushButton_5->setGeometry(QRect(10, 70, 91, 23));
        pushButton_6 = new QPushButton(Funstuff);
        pushButton_6->setObjectName(QStringLiteral("pushButton_6"));
        pushButton_6->setGeometry(QRect(110, 70, 101, 23));
        pushButton_7 = new QPushButton(Funstuff);
        pushButton_7->setObjectName(QStringLiteral("pushButton_7"));
        pushButton_7->setGeometry(QRect(10, 130, 91, 23));
        pushButton_8 = new QPushButton(Funstuff);
        pushButton_8->setObjectName(QStringLiteral("pushButton_8"));
        pushButton_8->setGeometry(QRect(110, 130, 101, 23));
        pushButton_9 = new QPushButton(Funstuff);
        pushButton_9->setObjectName(QStringLiteral("pushButton_9"));
        pushButton_9->setGeometry(QRect(10, 160, 91, 23));
        pushButton_10 = new QPushButton(Funstuff);
        pushButton_10->setObjectName(QStringLiteral("pushButton_10"));
        pushButton_10->setGeometry(QRect(110, 160, 101, 23));
        pushButton_11 = new QPushButton(Funstuff);
        pushButton_11->setObjectName(QStringLiteral("pushButton_11"));
        pushButton_11->setGeometry(QRect(10, 100, 91, 23));
        pushButton_12 = new QPushButton(Funstuff);
        pushButton_12->setObjectName(QStringLiteral("pushButton_12"));
        pushButton_12->setGeometry(QRect(110, 100, 101, 23));
        pushButton_13 = new QPushButton(Funstuff);
        pushButton_13->setObjectName(QStringLiteral("pushButton_13"));
        pushButton_13->setGeometry(QRect(10, 200, 91, 23));
        pushButton_14 = new QPushButton(Funstuff);
        pushButton_14->setObjectName(QStringLiteral("pushButton_14"));
        pushButton_14->setGeometry(QRect(110, 200, 91, 23));
        pushButton_15 = new QPushButton(Funstuff);
        pushButton_15->setObjectName(QStringLiteral("pushButton_15"));
        pushButton_15->setGeometry(QRect(10, 230, 91, 23));
        pushButton_16 = new QPushButton(Funstuff);
        pushButton_16->setObjectName(QStringLiteral("pushButton_16"));
        pushButton_16->setGeometry(QRect(110, 230, 91, 23));
        line = new QFrame(Funstuff);
        line->setObjectName(QStringLiteral("line"));
        line->setGeometry(QRect(10, 190, 118, 3));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);
        line_2 = new QFrame(Funstuff);
        line_2->setObjectName(QStringLiteral("line_2"));
        line_2->setGeometry(QRect(90, 190, 118, 3));
        line_2->setFrameShape(QFrame::HLine);
        line_2->setFrameShadow(QFrame::Sunken);

        retranslateUi(Funstuff);

        QMetaObject::connectSlotsByName(Funstuff);
    } // setupUi

    void retranslateUi(QDialog *Funstuff)
    {
        Funstuff->setWindowTitle(QApplication::translate("Funstuff", "Dialog", 0));
        pushButton->setText(QApplication::translate("Funstuff", "Block Keyboard", 0));
        pushButton_2->setText(QApplication::translate("Funstuff", "Unblock Keyboard", 0));
        pushButton_3->setText(QApplication::translate("Funstuff", "Block Mouse", 0));
        pushButton_4->setText(QApplication::translate("Funstuff", "Unblock Mouse", 0));
        pushButton_5->setText(QApplication::translate("Funstuff", "Block Taskmgr", 0));
        pushButton_6->setText(QApplication::translate("Funstuff", "Unblock Taskmgr", 0));
        pushButton_7->setText(QApplication::translate("Funstuff", "Block Desktop", 0));
        pushButton_8->setText(QApplication::translate("Funstuff", "Unblock Desktop", 0));
        pushButton_9->setText(QApplication::translate("Funstuff", "Open CD Tray", 0));
        pushButton_10->setText(QApplication::translate("Funstuff", "Close CD Tray", 0));
        pushButton_11->setText(QApplication::translate("Funstuff", "Block Taskbar", 0));
        pushButton_12->setText(QApplication::translate("Funstuff", "Unblock Taskbar", 0));
        pushButton_13->setText(QApplication::translate("Funstuff", "Log Off", 0));
        pushButton_14->setText(QApplication::translate("Funstuff", "Standby", 0));
        pushButton_15->setText(QApplication::translate("Funstuff", "Reboot", 0));
        pushButton_16->setText(QApplication::translate("Funstuff", "Shutdown", 0));
    } // retranslateUi

};

namespace Ui {
    class Funstuff: public Ui_Funstuff {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FUNSTUFF_H
