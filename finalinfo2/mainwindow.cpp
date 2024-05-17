#include "mainwindow.h"
#include "ui_mainwindow.h"

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
    switch (event->key()) {
    case Qt::Key_W:
        Yuri->setY(Yuri->y() - 2);  // Mover hacia arriba
        Yuri->setDirection(sprite::Up);  // Cambiar dirección
        break;
    case Qt::Key_S:
        Yuri->setY(Yuri->y() + 2);  // Mover hacia abajo
        Yuri->setDirection(sprite::Down);  // Cambiar dirección
        break;
    case Qt::Key_A:
        Yuri->setX(Yuri->x() - 2);  // Mover hacia la izquierda
        Yuri->setDirection(sprite::Left);  // Cambiar dirección
        break;
    case Qt::Key_D:
        Yuri->setX(Yuri->x() + 2);  // Mover hacia la derecha
        Yuri->setDirection(sprite::Right);  // Cambiar dirección
        break;
    default:
        QMainWindow::keyPressEvent(event);
    }
}

