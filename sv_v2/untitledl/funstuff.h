#ifndef FUNSTUFF_H
#define FUNSTUFF_H

#include <QDialog>

namespace Ui {
class Funstuff;
}

class Funstuff : public QDialog
{
    Q_OBJECT

public:
    explicit Funstuff(QWidget *parent = 0);
    ~Funstuff();

private slots:
    void on_pushButton_pressed();

    void on_pushButton_2_pressed();

    void on_pushButton_3_pressed();

    void on_pushButton_4_pressed();

    void on_pushButton_5_pressed();

    void on_pushButton_6_pressed();

    void on_pushButton_11_pressed();

    void on_pushButton_12_pressed();

    void on_pushButton_7_pressed();

    void on_pushButton_8_pressed();

    void on_pushButton_9_pressed();

    void on_pushButton_10_pressed();

    void on_pushButton_13_pressed();

    void on_pushButton_14_pressed();

    void on_pushButton_15_pressed();

    void on_pushButton_16_pressed();

private:
    Ui::Funstuff *ui;
};

#endif // FUNSTUFF_H
