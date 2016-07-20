#include "myLabel.h"
#include "qpainter.h"


mylabel::mylabel(QWidget* parent) : QLabel(parent)
{
    //connect(this, SIGNAL(clicked()), this, SLOT(slotClicked()));
    //connect(this, SIGNAL(doubleclicked()), this, SLOT(slotDoubleClicked()));
    //connect(this, SIGNAL(keypressed()), this, SLOT(slotKeyPressed()));

}

void mylabel::slotPressed()
{
}

void mylabel::slotReleased()
{
}


void mylabel::slotKeyPressed()
{
}


void mylabel::mouseReleaseEvent(QMouseEvent* event)
{
    if( rect().contains(event->pos()) )
    {

        if(event->button() == Qt::LeftButton)
            emit released(event->pos().x(), event->pos().y(), false);

        if(event->button() == Qt::RightButton)
            emit released(event->pos().x(), event->pos().y(), true);
    }
}

void mylabel::mousePressEvent(QMouseEvent* event)
{
    if( rect().contains(event->pos()) )
    {

        if(event->button() == Qt::LeftButton)
            emit pressed(event->pos().x(), event->pos().y(), false);

        if(event->button() == Qt::RightButton)
            emit pressed(event->pos().x(), event->pos().y(), true);
    }
}

void mylabel::keyReleaseEvent(QKeyEvent * event)
{
    emit keypressed(event);
}


void mylabel::paintEvent(QPaintEvent *aEvent)
{
    QLabel::paintEvent(aEvent);
    _displayImage();
}

void mylabel::setPixmap(QPixmap aPicture)
{

    _qpSource = _qpCurrent = aPicture;
    repaint();
}
//ty stackoverflow
void mylabel::_displayImage()
{
    if (_qpSource.isNull()) //no image was set, don't draw anything
        return;

    float cw = width(), ch = height();
    float pw = _qpCurrent.width(), ph = _qpCurrent.height();

    if (pw > cw && ph > ch && pw/cw > ph/ch || //both width and high are bigger, ratio at high is bigger or
        pw > cw && ph <= ch || //only the width is bigger or
        pw < cw && ph < ch && cw/pw < ch/ph //both width and height is smaller, ratio at width is smaller
        )
        _qpCurrent = _qpSource.scaledToWidth(cw, Qt::TransformationMode::FastTransformation);
    else if (pw > cw && ph > ch && pw/cw <= ph/ch || //both width and high are bigger, ratio at width is bigger or
        ph > ch && pw <= cw || //only the height is bigger or
        pw < cw && ph < ch && cw/pw > ch/ph //both width and height is smaller, ratio at height is smaller
        )
        _qpCurrent = _qpSource.scaledToHeight(ch, Qt::TransformationMode::FastTransformation);

    int x = (cw - _qpCurrent.width())/2, y = (ch - _qpCurrent.height())/2;

    QPainter paint(this);
    paint.setRenderHint(QPainter::Antialiasing);
    paint.drawPixmap(x, y, _qpCurrent);
}
