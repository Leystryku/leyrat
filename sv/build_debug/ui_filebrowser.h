/********************************************************************************
** Form generated from reading UI file 'filebrowser.ui'
**
** Created by: Qt User Interface Compiler version 5.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FILEBROWSER_H
#define UI_FILEBROWSER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTreeWidget>

QT_BEGIN_NAMESPACE

class Ui_Filebrowser
{
public:
    QTreeWidget *treeWidget;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QPlainTextEdit *plainTextEdit;
    QPushButton *pushButton_4;
    QPushButton *pushButton_5;
    QPushButton *pushButton_3;
    QLabel *label;
    QLabel *label_2;
    QPushButton *pushButton_6;
    QPushButton *pushButton_7;

    void setupUi(QDialog *Filebrowser)
    {
        if (Filebrowser->objectName().isEmpty())
            Filebrowser->setObjectName(QStringLiteral("Filebrowser"));
        Filebrowser->resize(491, 289);
        treeWidget = new QTreeWidget(Filebrowser);
        treeWidget->setObjectName(QStringLiteral("treeWidget"));
        treeWidget->setGeometry(QRect(10, 10, 361, 221));
        pushButton = new QPushButton(Filebrowser);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(380, 30, 101, 21));
        pushButton_2 = new QPushButton(Filebrowser);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        pushButton_2->setGeometry(QRect(380, 90, 101, 23));
        plainTextEdit = new QPlainTextEdit(Filebrowser);
        plainTextEdit->setObjectName(QStringLiteral("plainTextEdit"));
        plainTextEdit->setGeometry(QRect(10, 250, 361, 31));
        pushButton_4 = new QPushButton(Filebrowser);
        pushButton_4->setObjectName(QStringLiteral("pushButton_4"));
        pushButton_4->setGeometry(QRect(380, 60, 101, 21));
        pushButton_5 = new QPushButton(Filebrowser);
        pushButton_5->setObjectName(QStringLiteral("pushButton_5"));
        pushButton_5->setGeometry(QRect(380, 250, 101, 31));
        pushButton_3 = new QPushButton(Filebrowser);
        pushButton_3->setObjectName(QStringLiteral("pushButton_3"));
        pushButton_3->setGeometry(QRect(380, 150, 101, 23));
        label = new QLabel(Filebrowser);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(390, 120, 81, 21));
        label_2 = new QLabel(Filebrowser);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(390, 0, 71, 21));
        pushButton_6 = new QPushButton(Filebrowser);
        pushButton_6->setObjectName(QStringLiteral("pushButton_6"));
        pushButton_6->setGeometry(QRect(380, 180, 101, 23));
        pushButton_7 = new QPushButton(Filebrowser);
        pushButton_7->setObjectName(QStringLiteral("pushButton_7"));
        pushButton_7->setGeometry(QRect(380, 210, 101, 23));

        retranslateUi(Filebrowser);

        QMetaObject::connectSlotsByName(Filebrowser);
    } // setupUi

    void retranslateUi(QDialog *Filebrowser)
    {
        Filebrowser->setWindowTitle(QApplication::translate("Filebrowser", "Dialog", 0));
        QTreeWidgetItem *___qtreewidgetitem = treeWidget->headerItem();
        ___qtreewidgetitem->setText(0, QApplication::translate("Filebrowser", "C:/", 0));
        pushButton->setText(QApplication::translate("Filebrowser", "Download", 0));
        pushButton_2->setText(QApplication::translate("Filebrowser", "Delete", 0));
#ifndef QT_NO_TOOLTIP
        plainTextEdit->setToolTip(QApplication::translate("Filebrowser", "Use Prefix http:// if HTTP Upload", 0));
#endif // QT_NO_TOOLTIP
        plainTextEdit->setPlainText(QString());
        pushButton_4->setText(QApplication::translate("Filebrowser", "Upload", 0));
        pushButton_5->setText(QApplication::translate("Filebrowser", "File Browser", 0));
        pushButton_3->setText(QApplication::translate("Filebrowser", "Execute", 0));
        label->setText(QApplication::translate("Filebrowser", "HTTP Transfer", 0));
        label_2->setText(QApplication::translate("Filebrowser", "TCP Transfer", 0));
        pushButton_6->setText(QApplication::translate("Filebrowser", "Update", 0));
        pushButton_7->setText(QApplication::translate("Filebrowser", "Upload", 0));
    } // retranslateUi

};

namespace Ui {
    class Filebrowser: public Ui_Filebrowser {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FILEBROWSER_H
