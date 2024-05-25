#ifndef SPRITE2_H
#define SPRITE2_H

#include <QGraphicsPixmapItem>
#include <QTimer>

class Sprite2 : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT
public:
    Sprite2(QGraphicsItem* parent = nullptr);
    void startAnimation();

private slots:
    void updateFrame();

private:
    QPixmap spriteSheet;
    int currentFrame;
    QTimer timer;
    const int frameWidth;
    const int frameHeight;
    const int totalFrames;
};

#endif // SPRITE2_H
