#ifndef SPRITE2_H
#define SPRITE2_H

#include <QGraphicsPixmapItem>
#include <QTimer>
#include <QObject>
#include <QGraphicsItem>
#include <QPixmap>
#include <QPainter>
#include <QGraphicsScene>

class Sprite2 : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT
public:
    Sprite2(QGraphicsItem* parent = nullptr);
    void startAnimation();
    void startParabolicMovement();
    bool checkPlatformCollision(double newX, double newY);

private slots:
    void updateFrame();
    void moveParabolic();

private:
    QPixmap spriteSheet;
    int currentFrame;
    QTimer timer;
    const int frameWidth;
    const int frameHeight;
    const int totalFrames;
    const float scaleFactor;

    // Variables para el movimiento parabólico
    bool parabolicMoving;
    int initialY;
    int parabolicStep;
    int parabolicHeight;
    int parabolicDuration;
};

#endif // SPRITE2_H
