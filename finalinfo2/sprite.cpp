#include "sprite.h"

sprite::sprite(QObject *parent) : QObject(parent)
{
    timer = new QTimer();
    filas = 0;
    columnas = 9 * 32;  // Iniciar en la imagen 10
    pixmap = new QPixmap(":/yurisprite.png");

    ancho = 32;
    alto  = 32;

    currentDirection = Right;  // Inicialmente mirando a la derecha

    timer->start(200);  // Modifica la velocidad en que itera entre las imágenes

    connect(timer, &QTimer::timeout, this, &sprite::Actualizacion);
}

void sprite::Actualizacion()
{
    columnas += ancho;
    switch (currentDirection) {
    case Right:
        if (columnas >= (12 * 32)) {  // Si se excede la imagen 12 (número 13), regresar a la imagen 10
            columnas = 9 * 32;  // Regresar a la imagen 10
        }
        break;
    case Left:
        if (columnas >= (12 * 32)) {
            columnas = 9 * 32;  // Regresar a la imagen 10
        }
        break;
    case Down:
        if (columnas >= (12 * 32)) {
            columnas = 9 * 32;  // Regresar a la imagen 10
        }
        break;
    case Up:
        if (columnas >= (12 * 32)) {
            columnas = 9 * 32;  // Regresar a la imagen 10
        }
        break;
    }

    this->update(-ancho / 2, -alto / 2, ancho, alto);
}

void sprite::setDirection(Direction direction)
{
    if (currentDirection != direction) {
        currentDirection = direction;
        switch (direction) {
        case Right:
            filas = 2 * 32;  // Tercera fila
            columnas = 9 * 32;  // Comenzar desde la imagen 10
            break;
        case Left:
            filas = 1 * 32;  // Segunda fila
            columnas = 9 * 32;  // Comenzar desde la imagen 10
            break;
        case Down:
            filas = 0 * 32;  // Primera fila
            columnas = 9 * 32;  // Comenzar desde la imagen 10
            break;
        case Up:
            filas = 3 * 32;  // Cuarta fila
            columnas = 9 * 32;  // Comenzar desde la imagen 10
            break;
        }
    }
}

QRectF sprite::boundingRect() const
{
    return QRectF(-ancho / 2, -alto / 2, ancho, alto);
}

void sprite::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->drawPixmap(-ancho / 2, -alto / 2, *pixmap, columnas, filas, ancho, alto);
}
