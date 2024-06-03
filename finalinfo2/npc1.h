// npc1.h

#ifndef NPC1_H
#define NPC1_H

#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QTimer>
#include "sprite.h"
class npc1 : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT

public:
    npc1(QGraphicsItem* parent = nullptr);

signals:
    void collisionOccurred();


public slots:
    void updateFrame();
    void detectCollision(sprite *yuri);

private:
    QPixmap spriteSheet;
    int currentFrame;
    int frameWidth;
    int frameHeight;
    int totalFrames;
    QTimer timer;
};


#endif // NPC1_H
