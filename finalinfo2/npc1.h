#ifndef NPC1_H
#define NPC1_H

#include <QGraphicsPixmapItem>
#include <QTimer>

class sprite; // Adelantar la declaración de la clase sprite

class npc1 : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT
public:
    npc1(QGraphicsItem* parent = nullptr);

    QRectF boundingRect() const override; // Declaración del método boundingRect

signals:
    void collisionOccurred();

public slots:
    void updateFrame();
    void detectCollision(sprite *yuri);

private:
    QTimer timer;
    QPixmap spriteSheet;
    int currentFrame;
    int frameWidth;
    int frameHeight;
    int totalFrames;
};

#endif // NPC1_H
