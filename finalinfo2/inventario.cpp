#include "inventario.h"

Inventario::Inventario(QGraphicsItem* parent)
    : QGraphicsPixmapItem(parent),
    currentFrame(0),
    frameWidth(192 / 8),  // 8 frames in the sprite sheet
    frameHeight(24),
    totalFrames(8)
{
    spriteSheet.load(":/inv1.png");  // Ensure the path is correct
    setPixmap(spriteSheet.copy(currentFrame * frameWidth, 0, frameWidth, frameHeight));

    connect(&timer, &QTimer::timeout, this, &Inventario::updateFrame);
    timer.start(200);  // Adjust the interval as needed for smooth animation
}

void Inventario::updateFrame() {
    currentFrame = (currentFrame + 1) % totalFrames;
    int x = currentFrame * frameWidth;
    setPixmap(spriteSheet.copy(x, 0, frameWidth, frameHeight));
}
