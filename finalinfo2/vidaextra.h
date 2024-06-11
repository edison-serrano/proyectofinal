#ifndef VIDAEXTRA_H
#define VIDAEXTRA_H

#include <QObject>
#include <QGraphicsItem>
#include <QPixmap>
#include <QPainter>

class VidaExtra : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    explicit VidaExtra(QObject *parent = nullptr);

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    QPixmap *pixmap;
};

#endif // VIDAEXTRA_H
