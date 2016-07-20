#ifndef ADDUSER_H
#define ADDUSER_H

#include <QDialog>

namespace Ui {
class Adduser;
}

class Adduser : public QDialog
{
    Q_OBJECT

public:
    explicit Adduser(QWidget *parent = 0);
    ~Adduser();

private:
    Ui::Adduser *ui;
};

#endif // ADDUSER_H
