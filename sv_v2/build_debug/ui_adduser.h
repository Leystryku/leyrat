/********************************************************************************
** Form generated from reading UI file 'adduser.ui'
**
** Created by: Qt User Interface Compiler version 5.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ADDUSER_H
#define UI_ADDUSER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>

QT_BEGIN_NAMESPACE

class Ui_Adduser
{
public:

    void setupUi(QDialog *Adduser)
    {
        if (Adduser->objectName().isEmpty())
            Adduser->setObjectName(QStringLiteral("Adduser"));
        Adduser->resize(400, 300);

        retranslateUi(Adduser);

        QMetaObject::connectSlotsByName(Adduser);
    } // setupUi

    void retranslateUi(QDialog *Adduser)
    {
        Adduser->setWindowTitle(QApplication::translate("Adduser", "Dialog", 0));
    } // retranslateUi

};

namespace Ui {
    class Adduser: public Ui_Adduser {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ADDUSER_H
