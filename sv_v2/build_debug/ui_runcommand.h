/********************************************************************************
** Form generated from reading UI file 'runcommand.ui'
**
** Created by: Qt User Interface Compiler version 5.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RUNCOMMAND_H
#define UI_RUNCOMMAND_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>

QT_BEGIN_NAMESPACE

class Ui_Runcommand
{
public:
    QPushButton *pushButton;
    QTextEdit *textEdit;

    void setupUi(QDialog *Runcommand)
    {
        if (Runcommand->objectName().isEmpty())
            Runcommand->setObjectName(QStringLiteral("Runcommand"));
        Runcommand->resize(354, 56);
        pushButton = new QPushButton(Runcommand);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(260, 10, 81, 31));
        textEdit = new QTextEdit(Runcommand);
        textEdit->setObjectName(QStringLiteral("textEdit"));
        textEdit->setGeometry(QRect(20, 10, 221, 31));
        textEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        retranslateUi(Runcommand);

        QMetaObject::connectSlotsByName(Runcommand);
    } // setupUi

    void retranslateUi(QDialog *Runcommand)
    {
        Runcommand->setWindowTitle(QApplication::translate("Runcommand", "Dialog", 0));
        pushButton->setText(QApplication::translate("Runcommand", "Run", 0));
    } // retranslateUi

};

namespace Ui {
    class Runcommand: public Ui_Runcommand {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RUNCOMMAND_H
