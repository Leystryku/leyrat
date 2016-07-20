/********************************************************************************
** Form generated from reading UI file 'screenshare.ui'
**
** Created by: Qt User Interface Compiler version 5.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SCREENSHARE_H
#define UI_SCREENSHARE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include "mylabel.h"

QT_BEGIN_NAMESPACE

class Ui_Screenshare
{
public:
    mylabel *label;
    QPushButton *pushButton;
    QPlainTextEdit *plainTextEdit;
    QComboBox *comboBox;
    QComboBox *comboBox_2;

    void setupUi(QDialog *Screenshare)
    {
        if (Screenshare->objectName().isEmpty())
            Screenshare->setObjectName(QStringLiteral("Screenshare"));
        Screenshare->resize(390, 349);
        label = new mylabel(Screenshare);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(10, 10, 361, 291));
        pushButton = new QPushButton(Screenshare);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(310, 290, 61, 23));
        plainTextEdit = new QPlainTextEdit(Screenshare);
        plainTextEdit->setObjectName(QStringLiteral("plainTextEdit"));
        plainTextEdit->setGeometry(QRect(240, 320, 61, 21));
        plainTextEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        plainTextEdit->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        plainTextEdit->setLineWrapMode(QPlainTextEdit::WidgetWidth);
        comboBox = new QComboBox(Screenshare);
        comboBox->setObjectName(QStringLiteral("comboBox"));
        comboBox->setGeometry(QRect(310, 320, 69, 22));
        comboBox_2 = new QComboBox(Screenshare);
        comboBox_2->setObjectName(QStringLiteral("comboBox_2"));
        comboBox_2->setGeometry(QRect(150, 320, 69, 22));

        retranslateUi(Screenshare);

        QMetaObject::connectSlotsByName(Screenshare);
    } // setupUi

    void retranslateUi(QDialog *Screenshare)
    {
        Screenshare->setWindowTitle(QApplication::translate("Screenshare", "Dialog", 0));
        label->setText(QString());
        pushButton->setText(QApplication::translate("Screenshare", "Mode", 0));
#ifndef QT_NO_TOOLTIP
        plainTextEdit->setToolTip(QApplication::translate("Screenshare", "FPS", 0));
#endif // QT_NO_TOOLTIP
        plainTextEdit->setPlainText(QApplication::translate("Screenshare", "10", 0));
        comboBox->clear();
        comboBox->insertItems(0, QStringList()
         << QApplication::translate("Screenshare", "ultrafast", 0)
         << QApplication::translate("Screenshare", "superfast", 0)
         << QApplication::translate("Screenshare", "veryfast", 0)
         << QApplication::translate("Screenshare", "faster", 0)
         << QApplication::translate("Screenshare", "fast", 0)
         << QApplication::translate("Screenshare", "medium", 0)
         << QApplication::translate("Screenshare", "slow", 0)
         << QApplication::translate("Screenshare", "slower", 0)
        );
        comboBox_2->clear();
        comboBox_2->insertItems(0, QStringList()
         << QApplication::translate("Screenshare", "baseline", 0)
         << QApplication::translate("Screenshare", "main", 0)
         << QApplication::translate("Screenshare", "high", 0)
        );
    } // retranslateUi

};

namespace Ui {
    class Screenshare: public Ui_Screenshare {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SCREENSHARE_H
