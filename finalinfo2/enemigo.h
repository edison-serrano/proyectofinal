#ifndef ENEMIGO_H
#define ENEMIGO_H
#include "math.h"
#include <QObject>
#include <QGraphicsPixmapItem>
#include <QTimer>
# define PI 3.14159265359



class enemigo:public QObject, public QGraphicsPixmapItem
{
private:
    //atributos de posición
    int size=0;
    float x=0, y=0, auxx=0, auxy=0, r=65, the=0, v=0;

public:
    //constructores
    enemigo();
    enemigo(float posmx, float posmy,int tam,int vel);

    //funciones de movimiento
    void movimiento();
};

#endif // ENEMIGO_H
