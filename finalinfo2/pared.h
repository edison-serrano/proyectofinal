#ifndef PARED_H
#define PARED_H

#include <QGraphicsItem>
#include <QPainter>

class pared : public QGraphicsItem
{
public:
    pared(qreal x, qreal y, qreal width, qreal height, QColor color = QColor(0, 0, 0), QGraphicsItem *parent = nullptr)
        : QGraphicsItem(parent), m_color(color)
    {
        setPos(x, y);
        m_rect = QRectF(0, 0, width, height);
    }

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

private:
    QRectF m_rect;
    QColor m_color;
};

#endif // PARED_H
