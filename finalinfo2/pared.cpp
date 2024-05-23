#include "pared.h"

QRectF pared::boundingRect() const
{
    return m_rect;
}

void pared::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->setBrush(m_color);
    painter->drawRect(m_rect);
}
