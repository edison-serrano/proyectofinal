#include "radiacion.h"

Radiacion::Radiacion(const QString &spriteSheetPath, int frameWidth, int frameHeight, int columns, int rows, int frameTime, QGraphicsScene *scene, QList<pared*> *walls, QGraphicsItem *parent)
    : QGraphicsPixmapItem(parent), currentFrame(0), scene(scene), walls(walls) {

    QPixmap spriteSheet(spriteSheetPath);
    for (int y = 0; y < rows; ++y) {
        for (int x = 0; x < columns; ++x) {
            frames.append(spriteSheet.copy(x * frameWidth, y * frameHeight, frameWidth, frameHeight));
        }
    }

    setPixmap(frames[currentFrame]);

    frameTimer = new QTimer(this);
    connect(frameTimer, &QTimer::timeout, this, &Radiacion::nextFrame);
    frameTimer->start(frameTime);

    movementTimer = new QTimer(this);
    connect(movementTimer, &QTimer::timeout, this, &Radiacion::moveRandomly);
    movementTimer->start(500); // Cambiar dirección cada 500 ms
}

void Radiacion::nextFrame() {
    currentFrame = (currentFrame + 1) % frames.size();
    setPixmap(frames[currentFrame]);
}

void Radiacion::moveRandomly() {
    int direction = QRandomGenerator::global()->bounded(4);
    qreal dx = 0, dy = 0;
    const qreal stepSize = 10;

    switch (direction) {
    case 0: dy = -stepSize; break; // Arriba
    case 1: dy = stepSize; break;  // Abajo
    case 2: dx = -stepSize; break; // Izquierda
    case 3: dx = stepSize; break;  // Derecha
    }

    // Verificar colisiones con las paredes
    QPointF newPos = pos() + QPointF(dx, dy);
    QRectF newRect(newPos, boundingRect().size());

    bool collision = false;
    for (auto wall : *walls) {
        if (newRect.intersects(wall->boundingRect().translated(wall->pos()))) {
            collision = true;
            break;
        }
    }

    if (!collision && scene->sceneRect().contains(newRect)) {
        setPos(newPos);
    }
}


