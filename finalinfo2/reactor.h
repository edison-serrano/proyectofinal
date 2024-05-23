#ifndef REACTOR_H
#define REACTOR_H

#include <QGraphicsPixmapItem>
#include <QTimer>
#include <QPixmap>
#include <QVector>

class Reactor : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT
public:
    Reactor(const QString &spriteSheetPath, int frameWidth, int frameHeight, int frameCount, int frameTime, QGraphicsItem *parent = nullptr);

public slots:
    void nextFrame();

private:
    QVector<QPixmap> frames;
    int currentFrame;
    QTimer *timer;
};

#endif // REACTOR_H
