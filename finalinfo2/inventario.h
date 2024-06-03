#ifndef INVENTARIO_H
#define INVENTARIO_H

#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QTimer>

class Inventario : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT

public:
    Inventario(QGraphicsItem* parent = nullptr);

public slots:
    void updateFrame();

private:
    QPixmap spriteSheet;
    int currentFrame;
    int frameWidth;
    int frameHeight;
    int totalFrames;
    QTimer timer;
};

#endif // INVENTARIO_H
