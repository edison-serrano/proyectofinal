#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "pared.h"
#include "puerta.h"
//#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    scene = new QGraphicsScene();
    scene->setSceneRect(0, 0, 775, 315);
    ui->setupUi(this);

    ui->graphicsView->setScene(scene);

    // paredes externas
    paredes.push_back(new pared(0, 0, 770, 10)); // pared arriba
    scene->addItem(paredes.back());
    paredes.push_back(new pared(0, 390, 770, 10)); // pared abajo
    scene->addItem(paredes.back());
    paredes.push_back(new pared(0, 0, 10, 390)); // pared izquierda
    scene->addItem(paredes.back());
    paredes.push_back(new pared(760, 0, 10, 390)); // pared derecha
    scene->addItem(paredes.back());

    //paredes internas
    // Paredes horizontales
    paredes.push_back(new pared(0, 195, 257, 10)); // Pared horizontal izquierda
    scene->addItem(paredes.back());
    paredes.push_back(new pared(513, 195, 257, 10)); // Pared horizontal derecha
    scene->addItem(paredes.back());
    paredes.push_back(new pared(310, 260, 160, 10)); // Pared horizontal centro inferior
    scene->addItem(paredes.back());
    paredes.push_back(new pared(310, 130, 160, 10)); // Pared horizontal centro superior
    scene->addItem(paredes.back());

    // Paredes verticales
    paredes.push_back(new pared(385, 0, 10, 130)); // Pared vertical superior
    scene->addItem(paredes.back());
    paredes.push_back(new pared(513, 130, 10, 140)); // Pared vertical derecha
    scene->addItem(paredes.back());
    paredes.push_back(new pared(257, 130, 10, 140)); // Pared vertical izquierda
    scene->addItem(paredes.back());
    paredes.push_back(new pared(385, 260, 10, 130)); // Pared vertical inferior
    scene->addItem(paredes.back());

    // Puertas
    puertas.push_back(new puerta(268, 130, 40, 5)); // Puerta superior centro izq
    scene->addItem(puertas.back());
    puertas.push_back(new puerta(268, 265, 40, 5)); // Puerta inferior centro der
    scene->addItem(puertas.back());
    puertas.push_back(new puerta(472, 265, 40, 5)); // Puerta inferior centro izq
    scene->addItem(puertas.back());
    puertas.push_back(new puerta(472, 130, 40, 5)); // Puerta superior centro der
    scene->addItem(puertas.back());

    // Guardar posiciones originales de las puertas
    foreach (puerta *p, puertas) {
        puertaOriginalPositions[p] = p->pos();
    }

    // para personaje principal
    Yuri = new sprite();
    scene->addItem(Yuri);
    Yuri->setPos(50, 50);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    int newX = Yuri->x();
    int newY = Yuri->y();

    switch (event->key()) {
    case Qt::Key_W:
        newY -= 2;  // Mover hacia arriba
        Yuri->setDirection(sprite::Up);  // Cambiar dirección
        break;
    case Qt::Key_S:
        newY += 2;  // Mover hacia abajo
        Yuri->setDirection(sprite::Down);  // Cambiar dirección
        break;
    case Qt::Key_A:
        newX -= 2;  // Mover hacia la izquierda
        Yuri->setDirection(sprite::Left);  // Cambiar dirección
        break;
    case Qt::Key_D:
        newX += 2;  // Mover hacia la derecha
        Yuri->setDirection(sprite::Right);  // Cambiar dirección
        break;
    default:
        QMainWindow::keyPressEvent(event);
        return;
    }

    // Verificar colisión
    if (!Yuri->checkCollision(newX, newY)) {
        Yuri->setPos(newX, newY);
    }

    // Verificar colisión con puertas
    foreach (puerta *p, puertas) {
        if (Yuri->collidesWithItem(p)) {
            //qDebug() << "Colisión con puerta detectada en posición:" << p->pos();

            // Mover la puerta 25 píxeles a la derecha
            p->setPos(p->x() + 20, p->y());
            //qDebug() << "Puerta movida a la posición:" << p->pos();

            // Si la puerta ya tiene un temporizador, detenerlo
            if (puertaTimers.contains(p)) {
                puertaTimers[p]->stop();
                delete puertaTimers[p];
                puertaTimers.remove(p);
            }

            // Crear un nuevo temporizador para cerrar la puerta después de 3 segundos
            QTimer *timer = new QTimer(this);
            connect(timer, &QTimer::timeout, this, [=]() {
                closeDoor(p);
            });
            timer->start(3000); // 3 segundos
            puertaTimers[p] = timer;

            break;
        }
    }
}

void MainWindow::closeDoor(puerta *p)
{
    // Mover la puerta a su posición original
    p->setPos(puertaOriginalPositions[p]);
    //qDebug() << "Puerta cerrada a la posición:" << p->pos();

    // Detener y eliminar el temporizador
    puertaTimers[p]->stop();
    delete puertaTimers[p];
    puertaTimers.remove(p);
}
