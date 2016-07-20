#ifndef MYLABEL_H
#define MYLABEL_H

#include <QLabel>
#include <QMouseEvent>

class mylabel : public QLabel
{
        Q_OBJECT
public:
        mylabel(QWidget* parent = 0);
        ~mylabel(){}

signals:
        void released(int x, int y, bool is_right);
        void pressed(int x, int y, bool is_right);
        void keypressed(QKeyEvent * event);
        void NewImage(void*img, unsigned int size);
public slots:
        void slotPressed();
        void slotReleased();
        void slotKeyPressed();
protected:
        void mouseReleaseEvent(QMouseEvent* event);
        void mousePressEvent(QMouseEvent* event);
        void keyReleaseEvent(QKeyEvent * event);

private:
    QPixmap _qpSource; //preserve the original, so multiple resize events won't break the quality
    QPixmap _qpCurrent;

    void _displayImage();

public:
    void setPixmap(QPixmap aPicture);
    void paintEvent(QPaintEvent *aEvent);
};

#endif // MYLABEL_H
