// npc1.cpp

#include "npc1.h"
#include <QGraphicsScene>
#include "sprite.h"
#include <QDebug>

npc1::npc1(QGraphicsItem* parent)
    : QGraphicsPixmapItem(parent),
    currentFrame(0),
    frameWidth(128 / 4),  // 4 frames in the sprite sheet
    frameHeight(42),
    totalFrames(4)
{
    spriteSheet.load(":/npc1.png");  // Ensure the path is correct
    setPixmap(spriteSheet.copy(currentFrame * frameWidth, 0, frameWidth, frameHeight));

    connect(&timer, &QTimer::timeout, this, &npc1::updateFrame);
    timer.start(200);  // Adjust the interval as needed for smooth animation
}

void npc1::updateFrame() {
    currentFrame = (currentFrame + 1) % totalFrames;
    int x = currentFrame * frameWidth;
    setPixmap(spriteSheet.copy(x, 0, frameWidth, frameHeight));
}


void npc1::detectCollision(sprite *yuri)
{
    if (collidesWithItem(yuri))
    {
        qDebug() << "Colisión detectada con Yuri.";
        emit collisionOccurred(); // Emitir la señal cuando ocurre una colisión con Yuri
    }
}
