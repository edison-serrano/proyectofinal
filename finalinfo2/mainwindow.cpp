#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "pared.h"
#include "puerta.h"
#include "pasarnivel.h"
#include <QLabel>
#include <QGraphicsPixmapItem>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    scene = new QGraphicsScene();
    scene->setSceneRect(0, 0, 775, 315);
    ui->setupUi(this);

    ui->graphicsView->setScene(scene);

    setupExternalWalls(scene);

    // Paredes internas
    paredes.push_back(new pared(0, 195, 257, 10)); // Pared horizontal izquierda
    scene->addItem(paredes.back());
    paredes.push_back(new pared(513, 195, 257, 10)); // Pared horizontal derecha
    scene->addItem(paredes.back());
    paredes.push_back(new pared(310, 260, 160, 10)); // Pared horizontal centro inferior
    scene->addItem(paredes.back());
    paredes.push_back(new pared(310, 130, 160, 10)); // Pared horizontal centro superior
    scene->addItem(paredes.back());

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

    // Objeto pasarnivel
    nextLevelTrigger = new pasarnivel(150, 150, 15, 15);
    scene->addItem(nextLevelTrigger);

    // QLabel para "Siguiente nivel"
    nextLevelLabel = new QLabel(this);
    nextLevelLabel->setText("Siguiente nivel");
    nextLevelLabel->setGeometry(300, 200, 200, 50); // Ajusta la posición y tamaño según sea necesario
    nextLevelLabel->setStyleSheet("QLabel { background-color : white; color : red; font: bold 24px; }");
    nextLevelLabel->setVisible(false);

    // Crear la siguiente escena
    scene2 = new QGraphicsScene();
    scene2->setSceneRect(0, 0, 775, 315);
    setupExternalWalls(scene2);

    // Establecer una imagen de fondo para la escena2
    QPixmap background(":/Background.png");
    scene2->setBackgroundBrush(QBrush(background));
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
            p->setPos(p->x() + 20, p->y());

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

    // Verificar colisión con pasarnivel
    if (Yuri->collidesWithItem(nextLevelTrigger)) {
        nextLevelLabel->setVisible(true);
        QTimer::singleShot(1000, this, &MainWindow::switchToNextScene); // Cambiar de escena después de 1 segundo
    }
}

void MainWindow::closeDoor(puerta *p)
{
    // Mover la puerta a su posición original
    p->setPos(puertaOriginalPositions[p]);

    // Detener y eliminar el temporizador
    puertaTimers[p]->stop();
    delete puertaTimers[p];
    puertaTimers.remove(p);
}

void MainWindow::switchToNextScene()
{
    ui->graphicsView->setScene(scene2); // Cambia a la escena2
    Yuri->setPos(50, 50); // Resetear la posición de Yuri en la nueva escena
    nextLevelLabel->setVisible(false);
}



void MainWindow::setupExternalWalls(QGraphicsScene *scene)
{
    QList<pared *> externalWalls;

    externalWalls.push_back(new pared(0, 0, 770, 10)); // pared arriba
    scene->addItem(externalWalls.back());
    externalWalls.push_back(new pared(0, 390, 770, 10)); // pared abajo
    scene->addItem(externalWalls.back());
    externalWalls.push_back(new pared(0, 0, 10, 390)); // pared izquierda
    scene->addItem(externalWalls.back());
    externalWalls.push_back(new pared(760, 0, 10, 390)); // pared derecha
    scene->addItem(externalWalls.back());

}

