#ifndef PENDULO_H
#define PENDULO_H

class Pendulo
{
public:
    Pendulo();
    Pendulo(double longitud, double angulo, double velocidadAngular);

    double getLongitud() const;
    double getAngulo() const;
    double getVelocidadAngular() const;

    void setLongitud(double value);
    void setAngulo(double value);
    void setVelocidadAngular(double value);

    void CalcularVelocidadAngular();
    void CalcularPosicion();

private:
    double longitud;
    double angulo;
    double velocidadAngular;

    const double g = 9.81; // aceleración gravitacional
    const double DT = 0.01; // paso de tiempo
};

#endif // PENDULO_H
