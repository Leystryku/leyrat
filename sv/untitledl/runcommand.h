#ifndef RUNCOMMAND_H
#define RUNCOMMAND_H

#include <QDialog>

namespace Ui {
class Runcommand;
}

class Runcommand : public QDialog
{
    Q_OBJECT

public:
    explicit Runcommand(QWidget *parent = 0);
    ~Runcommand();

private slots:
    void on_pushButton_pressed();

private:
    Ui::Runcommand *ui;
};

#endif // RUNCOMMAND_H
