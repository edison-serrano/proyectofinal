#ifndef RADIACION_H
#define RADIACION_H

#include <QGraphicsPixmapItem>
#include <QTimer>
#include <QPixmap>
#include <QVector>
#include <QGraphicsScene>
#include <QRandomGenerator>
#include "pared.h"

class Radiacion : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT
public:
    Radiacion(const QString &spriteSheetPath, int frameWidth, int frameHeight, int columns, int rows, int frameTime, QGraphicsScene *scene, QList<pared*> *walls, QGraphicsItem *parent = nullptr);

public slots:
    void nextFrame();
    void moveRandomly();

private:
    QVector<QPixmap> frames;
    int currentFrame;
    QTimer *frameTimer;
    QTimer *movementTimer;
    QGraphicsScene *scene;
    QList<pared*> *walls;
};

#endif // RADIACION_H
