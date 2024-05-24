#include "pendulo.h"
#include <cmath>

Pendulo::Pendulo()
    : longitud(1.0), angulo(0.0), velocidadAngular(0.0)
{
}

Pendulo::Pendulo(double longitud, double angulo, double velocidadAngular)
    : longitud(longitud), angulo(angulo), velocidadAngular(velocidadAngular)
{
}

double Pendulo::getLongitud() const
{
    return longitud;
}

double Pendulo::getAngulo() const
{
    return angulo;
}

double Pendulo::getVelocidadAngular() const
{
    return velocidadAngular;
}

void Pendulo::setLongitud(double value)
{
    longitud = value;
}

void Pendulo::setAngulo(double value)
{
    angulo = value;
}

void Pendulo::setVelocidadAngular(double value)
{
    velocidadAngular = value;
}

void Pendulo::CalcularVelocidadAngular()
{
    // Aceleración angular debido a la gravedad: α = - (g / L) * sin(θ)
    double aceleracionAngular = - (g / longitud) * sin(angulo);
    // Actualizar velocidad angular
    velocidadAngular += aceleracionAngular * DT;
}

void Pendulo::CalcularPosicion()
{
    // Actualizar el ángulo
    angulo += velocidadAngular * DT;
}
