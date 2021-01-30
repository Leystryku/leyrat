/********************************************************************************
** Form generated from reading UI file 'masshttp.ui'
**
** Created by: Qt User Interface Compiler version 5.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MASSHTTP_H
#define UI_MASSHTTP_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_Masshttp
{
public:
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QPlainTextEdit *plainTextEdit;
    QPushButton *pushButton_3;
    QPlainTextEdit *plainTextEdit_2;

    void setupUi(QDialog *Masshttp)
    {
        if (Masshttp->objectName().isEmpty())
            Masshttp->setObjectName(QStringLiteral("Masshttp"));
        Masshttp->resize(399, 118);
        pushButton = new QPushButton(Masshttp);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(10, 90, 121, 23));
        pushButton_2 = new QPushButton(Masshttp);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        pushButton_2->setGeometry(QRect(140, 90, 121, 23));
        plainTextEdit = new QPlainTextEdit(Masshttp);
        plainTextEdit->setObjectName(QStringLiteral("plainTextEdit"));
        plainTextEdit->setGeometry(QRect(10, 10, 381, 31));
        pushButton_3 = new QPushButton(Masshttp);
        pushButton_3->setObjectName(QStringLiteral("pushButton_3"));
        pushButton_3->setGeometry(QRect(270, 90, 121, 23));
        plainTextEdit_2 = new QPlainTextEdit(Masshttp);
        plainTextEdit_2->setObjectName(QStringLiteral("plainTextEdit_2"));
        plainTextEdit_2->setGeometry(QRect(10, 50, 381, 31));

        retranslateUi(Masshttp);

        QMetaObject::connectSlotsByName(Masshttp);
    } // setupUi

    void retranslateUi(QDialog *Masshttp)
    {
        Masshttp->setWindowTitle(QApplication::translate("Masshttp", "Dialog", 0));
        pushButton->setText(QApplication::translate("Masshttp", "Download and Exec", 0));
        pushButton_2->setText(QApplication::translate("Masshttp", "Download", 0));
        plainTextEdit->setPlainText(QApplication::translate("Masshttp", "http://www.shit.com/penis.exe", 0));
        pushButton_3->setText(QApplication::translate("Masshttp", "Update", 0));
        plainTextEdit_2->setPlainText(QApplication::translate("Masshttp", "C:/hacked.exe", 0));
    } // retranslateUi

};

namespace Ui {
    class Masshttp: public Ui_Masshttp {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MASSHTTP_H
