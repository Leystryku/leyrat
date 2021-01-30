#ifndef SCREENSHARE_H
#define SCREENSHARE_H

#include <QDialog>
#include "ui_screenshare.h"

class Screenshare : public QDialog
{
    Q_OBJECT

public:
    explicit Screenshare(QWidget *parent = 0);
    ~Screenshare();

private slots:
    void slotNewImage(QPixmap img);

    void on_label_pressed(int x, int y, bool is_right);
    void on_label_released(int x, int y, bool is_right);

    void on_plainTextEdit_textChanged();

    void on_pushButton_pressed();

    virtual void closeEvent(QCloseEvent *event);

    virtual void hideEvent(QHideEvent *event);

    void on_comboBox_currentTextChanged(const QString &arg1);

    void wheelEvent(QWheelEvent*event);
    void on_comboBox_2_currentTextChanged(const QString &arg1);

signals:
    void NewImage(QPixmap img);

private:
    Ui::Screenshare *ui;
    char*ip;
};

#endif // SCREENSHARE_H
