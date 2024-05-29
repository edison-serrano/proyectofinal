#include "sprite2.h"

Sprite2::Sprite2(QGraphicsItem* parent)
    : QGraphicsPixmapItem(parent),
    currentFrame(2),  // Starting at the third frame (index 2)
    frameWidth(88),  // Adjust as per your sprite sheet dimensions
    frameHeight(170),
    totalFrames(2),  // Number of frames you want to use
    scaleFactor(0.3),
    parabolicMoving(false), // Inicializar la variable
    parabolicStep(0),
    parabolicHeight(50),  // Altura del salto
    parabolicDuration(30)  // Duración del salto
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
    setPixmap(spriteSheet.copy(x, 0, frameWidth, frameHeight).scaled(frameWidth * scaleFactor, frameHeight * scaleFactor));
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
        double t = (double)parabolicStep / parabolicDuration;
        double newX = x() + 2; // Mover hacia la derecha con una velocidad constante (ajusta según sea necesario)
        double newY = initialY - 4 * parabolicHeight * t * (1 - t); // Calcular la posición en el eje Y
        setPos(newX, newY); // Actualizar la posición del sprite en ambos ejes
        parabolicStep++;
        QTimer::singleShot(30, this, &Sprite2::moveParabolic);  // Ajustar la duración del paso
    } else {
        parabolicMoving = false;
    }
}

