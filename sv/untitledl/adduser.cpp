#include "adduser.h"
#include "ui_adduser.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "leyrat.h"


extern Ui::MainWindow *mainwin;
extern leyrat rat;

Adduser::Adduser(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Adduser)
{
    ui->setupUi(this);
    this->setWindowTitle("Add User");

    this->setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint | Qt::MSWindowsFixedSizeDialogHint);
}

Adduser::~Adduser()
{
    delete ui;
}
