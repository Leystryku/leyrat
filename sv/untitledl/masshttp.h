#ifndef MASSHTTP_H
#define MASSHTTP_H

#include <QDialog>

namespace Ui {
class Masshttp;
}

class Masshttp : public QDialog
{
    Q_OBJECT

public:
    explicit Masshttp(QWidget *parent = 0);
    ~Masshttp();

private slots:
    void on_pushButton_pressed();

    void on_pushButton_2_pressed();

    void on_pushButton_3_pressed();

private:
    Ui::Masshttp *ui;
};

#endif // MASSHTTP_H
