#ifndef PARABOLICO_H
#define PARABOLICO_H
#include <math.h>

#define DT 0.05
//#define DT 0.01 (con esto variamos la velocidad en la que se muestra la animación)
#define g -9.8


class parabolico
{
    //atributos de posición
    double posx, posy, vel_x, vel_y, vel, ang;

public:
    //constructores
    parabolico();
    parabolico(double x, double y, double v, double ang);

    //funciones movimiento parabolico
    void CalcularVelocidad();
    void CalcularPosicion();
    double getPosx() const;
    double getPosy() const;

    //setters
    void setPosy(double value);
    void setVel(double value);
    void setAng(double value);
};

#endif // PARABOLICO_H
