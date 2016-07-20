#include "sruncommand.h"
#include "ui_sruncommand.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "leyrat.h"

extern Ui::MainWindow *mainwin;
extern leyrat rat;

Ui::SRuncommand *sruncommandui = 0;

SRuncommand::SRuncommand(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SRuncommand)
{
    ui->setupUi(this);

    this->setWindowTitle("Console");

    this->setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint | Qt::MSWindowsFixedSizeDialogHint);


    ui->pushButton->setFocusPolicy(Qt::NoFocus);
    sruncommandui = ui;

    connect( this, SIGNAL( AddConText( const char *, int ) ), this, SLOT( AddConTextSlot( const char *, int ) ) );
}

SRuncommand::~SRuncommand()
{
    delete ui;
}

void SRuncommand::on_pushButton_pressed()
{
    QList<QTableWidgetItem *> selected = mainwin->tableWidget->selectedItems();
    QTableWidgetItem *item;

    foreach (item, selected) {

        if(item->column() != 2)  continue;
        rat.Request_RunCmd(item->text().toStdString().c_str(), ui->plainTextEdit_2->toPlainText().toStdString().c_str(), true);
        break;
    }
}

void SRuncommand::AddConTextSlot(const char*text, int bytes)
{

    QString converted = QString::fromUtf8(text, bytes );

    ui->plainTextEdit->appendPlainText(converted);

    //delete text;
}
