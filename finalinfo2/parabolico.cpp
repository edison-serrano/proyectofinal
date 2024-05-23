#include "parabolico.h"

double parabolico::getPosx() const
{
    return posx;
}

double parabolico::getPosy() const
{
    return posy;
}

void parabolico::setPosy(double value)
{
    posy = value;
}

void parabolico::setVel(double value)
{
    vel = value;
}

void parabolico::setAng(double value)
{
    ang = value;
}

parabolico::parabolico()
{

}

parabolico::parabolico(double x, double y, double v, double ang)
{
    this->posx=x;
    this->posy=y;
    this->vel=v;
    this->ang=ang;
}

void parabolico::CalcularVelocidad()
{
    vel_x=vel*cos(ang);
    vel_y=vel*sin(ang)-g*DT;
    vel=sqrt(pow(vel_x,2)+pow(vel_y,2));
    ang=atan2(vel_y,vel_x);//usamos "atan2" siempre
}

void parabolico::CalcularPosicion()
{
    posx=posx+vel_x*DT;
    posy=posy+vel_y*DT-0.5*g*DT*DT;
}
