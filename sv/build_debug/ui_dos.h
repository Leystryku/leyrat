/********************************************************************************
** Form generated from reading UI file 'dos.ui'
**
** Created by: Qt User Interface Compiler version 5.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DOS_H
#define UI_DOS_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_Dos
{
public:
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QPlainTextEdit *plainTextEdit;
    QPlainTextEdit *plainTextEdit_2;
    QPlainTextEdit *plainTextEdit_3;
    QPlainTextEdit *plainTextEdit_4;

    void setupUi(QDialog *Dos)
    {
        if (Dos->objectName().isEmpty())
            Dos->setObjectName(QStringLiteral("Dos"));
        Dos->resize(258, 184);
        pushButton = new QPushButton(Dos);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(90, 150, 75, 23));
        pushButton_2 = new QPushButton(Dos);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        pushButton_2->setGeometry(QRect(10, 150, 75, 23));
        plainTextEdit = new QPlainTextEdit(Dos);
        plainTextEdit->setObjectName(QStringLiteral("plainTextEdit"));
        plainTextEdit->setGeometry(QRect(10, 20, 171, 31));
        plainTextEdit_2 = new QPlainTextEdit(Dos);
        plainTextEdit_2->setObjectName(QStringLiteral("plainTextEdit_2"));
        plainTextEdit_2->setGeometry(QRect(190, 20, 61, 31));
        plainTextEdit_3 = new QPlainTextEdit(Dos);
        plainTextEdit_3->setObjectName(QStringLiteral("plainTextEdit_3"));
        plainTextEdit_3->setGeometry(QRect(10, 60, 241, 71));
        plainTextEdit_4 = new QPlainTextEdit(Dos);
        plainTextEdit_4->setObjectName(QStringLiteral("plainTextEdit_4"));
        plainTextEdit_4->setGeometry(QRect(190, 140, 61, 31));

        retranslateUi(Dos);

        QMetaObject::connectSlotsByName(Dos);
    } // setupUi

    void retranslateUi(QDialog *Dos)
    {
        Dos->setWindowTitle(QApplication::translate("Dos", "Dialog", 0));
        pushButton->setText(QApplication::translate("Dos", "HTTP", 0));
        pushButton_2->setText(QApplication::translate("Dos", "UDP", 0));
        plainTextEdit->setPlainText(QApplication::translate("Dos", "1.1.1.1", 0));
        plainTextEdit_2->setPlainText(QApplication::translate("Dos", "1337", 0));
        plainTextEdit_3->setPlainText(QApplication::translate("Dos", "\\x55\\x55\\x55\\cHI DUDE", 0));
        plainTextEdit_4->setPlainText(QApplication::translate("Dos", "60", 0));
    } // retranslateUi

};

namespace Ui {
    class Dos: public Ui_Dos {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DOS_H
