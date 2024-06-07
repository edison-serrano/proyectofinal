#include "sprite2.h"
#include "plataforma.h"
#include <QGraphicsScene>
#include <QList>

Sprite2::Sprite2(QGraphicsItem* parent)
    : QGraphicsPixmapItem(parent),
    currentFrame(2),  // Starting at the third frame (index 2)
    frameWidth(88),  // Adjust as per your sprite sheet dimensions
    frameHeight(170),
    totalFrames(2),  // Number of frames you want to use
    scaleFactor(0.3),
    parabolicMoving(false), // Inicializar la variable
    parabolicStep(0),
    parabolicHeight(40),  // Altura del salto
    parabolicDuration(30),
    facingRight(true)  // Inicializar la dirección actual
{
    spriteSheet.load(":/sprite2.png");  // Ensure the path is correct
    setPixmap(spriteSheet.copy(currentFrame * frameWidth, 0, frameWidth, frameHeight).scaled(frameWidth * scaleFactor, frameHeight * scaleFactor));

    connect(&timer, &QTimer::timeout, this, &Sprite2::updateFrame);
}

void Sprite2::startAnimation() {
    timer.start(200);  // Adjust the interval as needed for smooth animation
}

void Sprite2::updateFrame() {
    currentFrame = (currentFrame + 1) % totalFrames + 2;  // Cycling through frames 3, 4, 5, and 6
    int x = currentFrame * frameWidth;
    QPixmap frame = spriteSheet.copy(x, 0, frameWidth, frameHeight).scaled(frameWidth * scaleFactor, frameHeight * scaleFactor);

    if (!facingRight) {
        frame = frame.transformed(QTransform().scale(-1, 1));  // Reflejar la imagen horizontalmente
    }

    setPixmap(frame);
}

void Sprite2::startParabolicMovement() {
    if (!parabolicMoving) {
        parabolicMoving = true;
        initialY = y();
        parabolicStep = 0;
        QTimer::singleShot(0, this, &Sprite2::moveParabolic);  // Iniciar el movimiento parabólico inmediatamente
    }
}

void Sprite2::moveParabolic() {
    if (parabolicStep <= parabolicDuration) {
        double t = static_cast<double>(parabolicStep) / parabolicDuration;
        double newX = x() + 2; // Ajusta esto si deseas que se mueva más rápido horizontalmente
        double newY = initialY - 8 * parabolicHeight * t * (1 - t);

        bool onPlatform = false;
        QList<QGraphicsItem *> collidingItemsList = scene()->collidingItems(this);
        foreach (QGraphicsItem *item, collidingItemsList) {
            if (Plataforma *plataforma = dynamic_cast<Plataforma*>(item)) {
                QRectF platformRect = plataforma->boundingRect().translated(plataforma->pos());
                QRectF spriteRect(newX, newY, boundingRect().width(), boundingRect().height());
                if (spriteRect.intersects(platformRect)) {
                    newY = plataforma->y() - boundingRect().height();
                    onPlatform = true;
                    break;
                }
            }
        }

        setPos(newX, newY);

        if (!onPlatform) {
            parabolicStep++;
        } else {
            parabolicMoving = false;
        }

        QTimer::singleShot(12, this, &Sprite2::moveParabolic); // Ajustar el intervalo para una caída más suave
    } else {
        parabolicMoving = false;
    }
}

bool Sprite2::checkPlatformCollision(double newX, double newY)
{
    QRectF newRect(newX, newY, boundingRect().width(), boundingRect().height());
    QList<QGraphicsItem *> collidingItemsList = scene()->collidingItems(this);
    foreach (QGraphicsItem *item, collidingItemsList) {
        if (Plataforma *plataforma = dynamic_cast<Plataforma *>(item)) {
            QRectF platformRect = plataforma->boundingRect().translated(plataforma->pos());
            if (newRect.intersects(platformRect)) {
                return true;
            }
        }
    }
    return false;
}

void Sprite2::setFacingRight(bool facingRight) {
    if (this->facingRight != facingRight) {
        this->facingRight = facingRight;
        updateFrame();  // Actualizar el frame para reflejar la dirección
    }
}
