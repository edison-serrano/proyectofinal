#ifndef PLATAFORMA_H
#define PLATAFORMA_H

#include <QGraphicsItem>

class Plataforma : public QGraphicsItem
{
public:
    Plataforma(QGraphicsItem *parent = nullptr);

    // Métodos de QGraphicsItem que deben ser implementados
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QPainterPath shape() const override;
};

#endif // PLATAFORMA_H
