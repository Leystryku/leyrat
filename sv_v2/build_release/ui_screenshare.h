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
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPlainTextEdit>
#include "mylabel.h"

QT_BEGIN_NAMESPACE

class Ui_Screenshare
{
public:
    mylabel *label;
    QPlainTextEdit *plainTextEdit;
    QPlainTextEdit *plainTextEdit_2;

    void setupUi(QDialog *Screenshare)
    {
        if (Screenshare->objectName().isEmpty())
            Screenshare->setObjectName(QStringLiteral("Screenshare"));
        Screenshare->resize(390, 349);
        label = new mylabel(Screenshare);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(10, 10, 361, 291));
        plainTextEdit = new QPlainTextEdit(Screenshare);
        plainTextEdit->setObjectName(QStringLiteral("plainTextEdit"));
        plainTextEdit->setGeometry(QRect(10, 320, 61, 21));
        plainTextEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        plainTextEdit->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        plainTextEdit_2 = new QPlainTextEdit(Screenshare);
        plainTextEdit_2->setObjectName(QStringLiteral("plainTextEdit_2"));
        plainTextEdit_2->setGeometry(QRect(310, 320, 61, 21));
        plainTextEdit_2->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        plainTextEdit_2->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        plainTextEdit_2->setLineWrapMode(QPlainTextEdit::WidgetWidth);

        retranslateUi(Screenshare);

        QMetaObject::connectSlotsByName(Screenshare);
    } // setupUi

    void retranslateUi(QDialog *Screenshare)
    {
        Screenshare->setWindowTitle(QApplication::translate("Screenshare", "Dialog", 0));
        label->setText(QString());
#ifndef QT_NO_TOOLTIP
        plainTextEdit->setToolTip(QApplication::translate("Screenshare", "Quality", 0));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_ACCESSIBILITY
        plainTextEdit->setAccessibleName(QString());
#endif // QT_NO_ACCESSIBILITY
        plainTextEdit->setPlainText(QApplication::translate("Screenshare", "50", 0));
#ifndef QT_NO_TOOLTIP
        plainTextEdit_2->setToolTip(QApplication::translate("Screenshare", "Waittime", 0));
#endif // QT_NO_TOOLTIP
        plainTextEdit_2->setPlainText(QApplication::translate("Screenshare", "3000", 0));
    } // retranslateUi

};

namespace Ui {
    class Screenshare: public Ui_Screenshare {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SCREENSHARE_H
