#include "pared.h"

pared::pared()
{

}

pared::pared(int x, int y, int w, int h)
{
    this->posx=x;
    this->posy=y;
    this->h=h;
    this->w=w;
}

QRectF pared::boundingRect() const
{
    return QRectF(posx,posy,w,h);
}

void pared::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    // Configurar el color de relleno
    painter->setBrush(QColor(65, 105, 225)); // Azul rey (RoyalBlue)

    // Configurar el color del borde igual al color de relleno
    painter->setPen(QColor(65, 105, 225)); // Borde azul rey

    painter->drawRect(boundingRect());
}
