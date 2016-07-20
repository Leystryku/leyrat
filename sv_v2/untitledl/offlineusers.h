#ifndef OFFLINEUSERS_H
#define OFFLINEUSERS_H

#include <QDialog>

namespace Ui {
class Offlineusers;
}

class Offlineusers : public QDialog
{
    Q_OBJECT

public:
    explicit Offlineusers(QWidget *parent = 0);
    ~Offlineusers();
    void Refresh();

private slots:
    void on_pushButton_pressed();

private:
    Ui::Offlineusers *ui;
};

#endif // OFFLINEUSERS_H
