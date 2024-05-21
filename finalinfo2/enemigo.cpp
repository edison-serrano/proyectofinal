#include "enemigo.h"

enemigo::enemigo()
{

}


enemigo::enemigo(float posmx, float posmy,int tam, int vel)
{
    size=tam;
    auxx=posmx;
    auxy=posmy;
    v=vel;
    setPixmap(QPixmap(":/enemigo.png").scaled(size,size));
    setPos(auxx,auxy);
}

void enemigo::movimiento()
{
    if (the>=360) the=0;
    x=auxx+r*cosf(the*PI/180);
    y=auxy+r*sinf(the*PI/180);
    the+=v;

    setPos(x,y);
}
