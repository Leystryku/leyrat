#ifndef DOS_H
#define DOS_H

#include <QDialog>

namespace Ui {
class Dos;
}

class Dos : public QDialog
{
    Q_OBJECT

public:
    explicit Dos(QWidget *parent = 0);
    ~Dos();

private slots:
    void on_pushButton_2_pressed();

    void on_pushButton_pressed();

private:
    Ui::Dos *ui;
    char*ip;

};

#endif // DOS_H
