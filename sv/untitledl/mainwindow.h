#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#pragma comment(linker, "/DEFAULTLIB:Ws2_32.lib")
#include <QMainWindow>
#include "dos.h"
#include "filebrowser.h"
#include "sruncommand.h"
#include "runcommand.h"
#include "screenshare.h"
#include "masshttp.h"
#include "offlineusers.h"
#include "adduser.h"
#include "funstuff.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_4_pressed();

    void on_pushButton_3_pressed();

    void on_pushButton_2_pressed();

    void on_pushButton_pressed();

    void on_pushButton_7_pressed();

    void on_pushButton_6_pressed();

    void on_pushButton_9_pressed();

    void on_pushButton_10_pressed();

    void on_pushButton_8_pressed();

    void on_pushButton_5_pressed();

    void on_pushButton_11_pressed();

private:
    Ui::MainWindow *ui;
    Dos *dos;
    Filebrowser *filebrowser;
    Screenshare *screenshare;
    Runcommand *runcommand;
    SRuncommand *sruncommand;
    Masshttp *masshttp;
    Offlineusers* offlineusers;
    Adduser *adduser;
    Funstuff *funstuff;

};

#endif // MAINWINDOW_H
