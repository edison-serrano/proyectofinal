#ifndef SPRITE2_H
#define SPRITE2_H

#include <QGraphicsPixmapItem>
#include <QTimer>

class Sprite2 : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT
public:
    Sprite2(QGraphicsItem* parent = nullptr);
    void startAnimation();
    void startParabolicMovement();  // Nueva función para iniciar el movimiento parabólico

private slots:
    void updateFrame();
    void moveParabolic();  // Nuevo slot para el movimiento parabólico

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
