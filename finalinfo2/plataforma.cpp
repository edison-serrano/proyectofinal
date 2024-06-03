#include "plataforma.h"
#include <QPainter>

Plataforma::Plataforma(QGraphicsItem *parent) : QGraphicsItem(parent)
{
    // Constructor de la clase Plataforma
}

QRectF Plataforma::boundingRect() const
{
    // Devuelve el rectángulo que delimita el área ocupada por la plataforma
    return QRectF(0, 0, 32, 16); // Ajusta las dimensiones según el tamaño de tu plataforma
}

void Plataforma::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    // Dibuja la plataforma
    painter->drawPixmap(0, 0, QPixmap(":/plataforma1.png").scaled(32, 16));
    // Si prefieres dibujar la plataforma con una forma simple, puedes utilizar un rectángulo en su lugar:
    // painter->drawRect(0, 0, 32, 16);
    Q_UNUSED(option);
    Q_UNUSED(widget);
}

QPainterPath Plataforma::shape() const
{
    // Devuelve la forma de la plataforma para la detección de colisiones
    QPainterPath path;
    path.addRect(0, 0, 32, 16); // Ajusta las dimensiones según el tamaño de tu plataforma
    return path;
}
