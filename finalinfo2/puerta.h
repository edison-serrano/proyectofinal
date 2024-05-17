#ifndef PUERTA_H
#define PUERTA_H

#include <QGraphicsItem>
#include <QPainter>

class puerta : public QGraphicsItem
{
    int posx, posy, w, h;

public:
    puerta();
    puerta(int x, int y, int w, int h);

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr);
};

#endif // PUERTA_H
