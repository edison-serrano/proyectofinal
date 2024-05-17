#include "sprite.h"

sprite::sprite(QObject *parent) : QObject(parent)
{
    timer = new QTimer();
    filas = 0;
    columnas = 9 * 32;  // Iniciar en la imagen 10
    pixmap = new QPixmap(":/yurisprite.png");

    // Nuevas dimensiones de cada imagen
    ancho = 32;
    alto  = 32;

    timer->start(150); // Modifica la velocidad en que itera entre las imágenes

    connect(timer, &QTimer::timeout, this, &sprite::Actualizacion);
}

void sprite::Actualizacion()
{
    columnas += ancho;
    if (columnas >= (12 * 32)) // Si se excede la imagen 12 (número 13), regresar a la imagen 10
    {
        columnas = 9 * 32; // Regresar a la imagen 10
    }
    this->update(-ancho / 2, -alto / 2, ancho, alto);
}

QRectF sprite::boundingRect() const
{
    return QRectF(-ancho / 2, -alto / 2, ancho, alto);
}

void sprite::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->drawPixmap(-ancho / 2, -alto / 2, *pixmap, columnas, filas, ancho, alto);
}
