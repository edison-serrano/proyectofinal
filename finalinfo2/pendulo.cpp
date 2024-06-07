#include "pendulo.h"
#include <cmath>

// Constructor
Pendulo::Pendulo(double longitud, double angulo, double gravedad)
    : longitud(longitud), angulo(angulo), gravedad(gravedad), velocidadAngular(0.0) {}

// Método para actualizar el estado del péndulo usando el método de Euler
void Pendulo::actualizar(double deltaTiempo) {
    double aceleracionAngular = -(gravedad / longitud) * std::sin(angulo);
    velocidadAngular += aceleracionAngular * deltaTiempo;
    angulo += velocidadAngular * deltaTiempo;

    // Asegúrate de llamar a update() para redibujar el péndulo
    update();
}

// Métodos para obtener el estado del péndulo
double Pendulo::getLongitud() const { return longitud; }
double Pendulo::getAngulo() const { return angulo; }
double Pendulo::getVelocidadAngular() const { return velocidadAngular; }

// Método para pintar el péndulo en la escena
void Pendulo::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(option);
    Q_UNUSED(widget);

    // Dibujar el brazo del péndulo
    double x = longitud * std::sin(angulo);
    double y = longitud * std::cos(angulo);

    painter->setPen(QPen(Qt::black, 2));
    painter->drawLine(0, 0, x, y);

    // Dibujar la masa del péndulo
    painter->setBrush(Qt::green);
    painter->drawEllipse(QPointF(x, y), 10, 10);
}

// Método para definir el área de colisión del péndulo
QRectF Pendulo::boundingRect() const {
    // Agregar un margen adicional para incluir la masa del péndulo
    double margin = 10;
    return QRectF(-longitud - margin, -longitud - margin, 2 * longitud + 2 * margin, 2 * longitud + 2 * margin);
}
