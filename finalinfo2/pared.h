#ifndef PARED_H
#define PARED_H
#include <QGraphicsItem>
#include <QPainter>

class pared: public QGraphicsItem
{

    int posx, posy,w, h;
public:
    pared();
    pared(int x, int y, int w, int h);

    QRectF boundingRect() const;
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,QWidget *widget = nullptr);
};

#endif // PARED_H
