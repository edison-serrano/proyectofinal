#include "vidaextra.h"

VidaExtra::VidaExtra(QObject *parent) : QObject(parent)
{
    QPixmap originalPixmap(":/vidaextra.png");
    int scaledWidth = 25;  // Ancho deseado
    int scaledHeight = 25; // Alto deseado
    pixmap = new QPixmap(originalPixmap.scaled(scaledWidth, scaledHeight, Qt::KeepAspectRatio));
}

QRectF VidaExtra::boundingRect() const
{
    return QRectF(-16, -16, 25, 25);  // Ajusta el tamaño según tu imagen escalada
}

void VidaExtra::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->drawPixmap(-16, -16, *pixmap);
}
