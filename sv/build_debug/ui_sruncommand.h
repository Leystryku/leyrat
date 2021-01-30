/********************************************************************************
** Form generated from reading UI file 'sruncommand.ui'
**
** Created by: Qt User Interface Compiler version 5.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SRUNCOMMAND_H
#define UI_SRUNCOMMAND_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_SRuncommand
{
public:
    QPlainTextEdit *plainTextEdit;
    QPlainTextEdit *plainTextEdit_2;
    QPushButton *pushButton;

    void setupUi(QDialog *SRuncommand)
    {
        if (SRuncommand->objectName().isEmpty())
            SRuncommand->setObjectName(QStringLiteral("SRuncommand"));
        SRuncommand->resize(456, 396);
        plainTextEdit = new QPlainTextEdit(SRuncommand);
        plainTextEdit->setObjectName(QStringLiteral("plainTextEdit"));
        plainTextEdit->setGeometry(QRect(10, 10, 431, 331));
        plainTextEdit->setReadOnly(true);
        plainTextEdit_2 = new QPlainTextEdit(SRuncommand);
        plainTextEdit_2->setObjectName(QStringLiteral("plainTextEdit_2"));
        plainTextEdit_2->setGeometry(QRect(10, 350, 411, 41));
        plainTextEdit_2->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        plainTextEdit_2->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        plainTextEdit_2->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
        plainTextEdit_2->setLineWrapMode(QPlainTextEdit::NoWrap);
        pushButton = new QPushButton(SRuncommand);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(430, 350, 21, 41));

        retranslateUi(SRuncommand);

        QMetaObject::connectSlotsByName(SRuncommand);
    } // setupUi

    void retranslateUi(QDialog *SRuncommand)
    {
        SRuncommand->setWindowTitle(QApplication::translate("SRuncommand", "Dialog", 0));
        pushButton->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class SRuncommand: public Ui_SRuncommand {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SRUNCOMMAND_H
