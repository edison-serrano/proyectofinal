#include "puerta.h"

puerta::puerta()
{
}

puerta::puerta(int x, int y, int w, int h)
{
    this->posx = x;
    this->posy = y;
    this->w = w;
    this->h = h;
}

QRectF puerta::boundingRect() const
{
    return QRectF(posx, posy, w, h);
}

void puerta::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setBrush(Qt::green); // Color de la puerta
    painter->setPen(Qt::green);
    painter->drawRect(boundingRect());
}
