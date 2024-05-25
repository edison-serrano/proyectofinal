#ifndef SPRITE_H
#define SPRITE_H

#include <QObject>
#include <QGraphicsItem>
#include <QTimer>
#include <QPixmap>
#include <QPainter>
#include <QGraphicsScene>

class sprite : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    explicit sprite(QObject *parent = nullptr);

    QTimer *timer;
    QPixmap *pixmap;

    float filas, columnas;
    float ancho;
    float alto;

    int vidas;  // Añadir el atributo de vidas

    enum Direction { Right, Left, Up, Down };
    Direction currentDirection;

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    void setDirection(Direction direction);
    bool checkCollision(int newX, int newY);

    void decreaseLife();  // Añadir el método para restar vidas

signals:
    void vidaCambiada(int vidas);  // Señal para indicar que la vida ha cambiado

public slots:
    void Actualizacion();
};

#endif // SPRITE_H

