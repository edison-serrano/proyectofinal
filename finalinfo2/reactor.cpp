#include "reactor.h"

Reactor::Reactor(const QString &spriteSheetPath, int frameWidth, int frameHeight, int frameCount, int frameTime, QGraphicsItem *parent)
    : QGraphicsPixmapItem(parent), currentFrame(0) {
    // Cargar la hoja de sprites
    QPixmap spriteSheet(spriteSheetPath);

    // Dividir la hoja de sprites en cuadros individuales
    for (int i = 0; i < frameCount; ++i) {
        int x = (i % (spriteSheet.width() / frameWidth)) * frameWidth;
        int y = (i / (spriteSheet.width() / frameWidth)) * frameHeight;
        frames.append(spriteSheet.copy(x, y, frameWidth, frameHeight));
    }

    // Establecer el cuadro inicial
    setPixmap(frames[currentFrame]);

    // Configurar el temporizador para la animación del cuadro
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Reactor::nextFrame);
    timer->start(frameTime);
}

void Reactor::nextFrame() {
    // Actualizar el cuadro actual
    currentFrame = (currentFrame + 1) % frames.size();
    setPixmap(frames[currentFrame]);
}
