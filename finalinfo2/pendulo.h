#ifndef PENDULO_H
#define PENDULO_H

#include <QGraphicsItem>
#include <QPainter>

class Pendulo : public QGraphicsItem {
public:
    Pendulo(double longitud, double angulo, double gravedad = 9.81);

    // Método para actualizar el estado del péndulo usando el método de Euler
    void actualizar(double deltaTiempo);

    // Métodos para obtener el estado del péndulo
    double getLongitud() const;
    double getAngulo() const;
    double getVelocidadAngular() const;

protected:
    // Método para pintar el péndulo en la escena
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

    // Método para definir el área de colisión del péndulo
    QRectF boundingRect() const override;

private:
    double longitud;  // Longitud del brazo del péndulo (en metros)
    double angulo;    // Ángulo del péndulo con la vertical (en radianes)
    double gravedad;  // Aceleración debida a la gravedad (en m/s^2)
    double velocidadAngular;  // Velocidad angular del péndulo (en radianes por segundo)
};

#endif // PENDULO_H
