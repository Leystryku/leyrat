#ifndef FILEBROWSER_H
#define FILEBROWSER_H

#include <QDialog>

namespace Ui {
class Filebrowser;
}

class Filebrowser : public QDialog
{
    Q_OBJECT

public:
    explicit Filebrowser(QWidget *parent = 0);
    ~Filebrowser();

private slots:
    void on_pushButton_5_pressed();

    void on_pushButton_pressed();

    void on_pushButton_2_pressed();

    void on_pushButton_4_pressed();

    void on_treeWidget_doubleClicked(const QModelIndex &index);

    void on_pushButton_3_pressed();

    void on_pushButton_6_pressed();

    void on_pushButton_7_pressed();

private:
    Ui::Filebrowser *ui;
    char*ip;
};

#endif // FILEBROWSER_H
