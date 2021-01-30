#ifndef SRUNCOMMAND_H
#define SRUNCOMMAND_H

#include <QDialog>

namespace Ui {
class SRuncommand;
}

class SRuncommand : public QDialog
{
    Q_OBJECT

public:
    explicit SRuncommand(QWidget *parent = 0);
    ~SRuncommand();


private slots:
    void on_pushButton_pressed();
    void AddConTextSlot(const char*text, int bytes);

private:
    Ui::SRuncommand *ui;


signals:
    void AddConText(const char*text, int bytes);
};

#endif // SRUNCOMMAND_H
