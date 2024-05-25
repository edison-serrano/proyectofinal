#include "sprite2.h"

Sprite2::Sprite2(QGraphicsItem* parent)
    : QGraphicsPixmapItem(parent),
    currentFrame(2),  // Starting at the third frame (index 2)
    frameWidth(88),  // Adjust as per your sprite sheet dimensions
    frameHeight(170),
    totalFrames(4)  // Number of frames you want to use
{
    spriteSheet.load(":/sprite2.png");  // Ensure the path is correct
    setPixmap(spriteSheet.copy(currentFrame * frameWidth, 0, frameWidth, frameHeight).scaled(frameWidth - 10, frameHeight - 10));

    connect(&timer, &QTimer::timeout, this, &Sprite2::updateFrame);
}

void Sprite2::startAnimation() {
    timer.start(200);  // Adjust the interval as needed for smooth animation
}

void Sprite2::updateFrame() {
    currentFrame = (currentFrame + 1) % totalFrames + 2;  // Cycling through frames 3, 4, 5, and 6
    int x = currentFrame * frameWidth;
    setPixmap(spriteSheet.copy(x, 0, frameWidth, frameHeight).scaled(frameWidth - 30, frameHeight - 30));
}
