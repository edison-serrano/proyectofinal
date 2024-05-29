#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "pared.h"
#include "puerta.h"
#include "pasarnivel.h"
#include <QLabel>
#include <QPushButton>
#include <QGraphicsPixmapItem>
#include "enemigo.h"
#include "reactor.h"
#include "radiacion.h"
#include "sprite2.h"
#include "pendulo.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    scene2Initialized(false),
    nextLevelActivated(false)
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

    // para personaje principal scene
    Yuri = new sprite();
    scene->addItem(Yuri);
    Yuri->setPos(385, 195);

    // Agregar sprite reactor.png con animación
    Reactor *reactor = new Reactor(":/reactor.png", 96, 96, 16, 100); // 96x96 cada img, 16 en total, 100 ms por img
    reactor->setPos(600, 230);
    scene->addItem(reactor);


    // Crear y agregar un enemigo a la scene
    enemigo *nuevoEnemigo = new enemigo(640, 280, 30, 10); // posición, tamaño 30, velocidad 10
    scene->addItem(nuevoEnemigo);
    // Usa un temporizador para actualizar el movimiento del enemigo
    QTimer *timer = new QTimer(this);
    // Configurar el tiempo de actualización en milisegundos
    int tiempoDeActualizacion = 120;
    connect(timer, &QTimer::timeout, nuevoEnemigo, &enemigo::movimiento);
    timer->start(tiempoDeActualizacion); // Configura el tiempo de actualizacion

    /*
    // Crear y agregar un enemigo a la scene
    enemigo *nuevoEnemigo2 = new enemigo(500, 295, 25, 15); // posición, tamaño 30, velocidad 10
    scene->addItem(nuevoEnemigo2);
    connect(timer, &QTimer::timeout, nuevoEnemigo2, &enemigo::movimiento);
    timer->start(tiempoDeActualizacion); // Configura el tiempo de actualizacion
*/

    // Agregar sprite radiacion.png con animación y movimiento aleatorio
    Radiacion *radiacion = new Radiacion(":/radiacion.png", 192, 192, 5, 4, 100, scene, &paredes); // 192x192 cada img, 5x4 en total, 100 ms por img
    radiacion->setPos(50, 200);
    scene->addItem(radiacion);

    // Objeto pasarnivel
    nextLevelTrigger = new pasarnivel(150, 150, 15, 15);
    scene->addItem(nextLevelTrigger);

    // Etiqueta para mostrar las vidas
    lifeLabel = new QLabel(this);
    lifeLabel->setText("Vida: 100 %");
    lifeLabel->setGeometry(10, 10, 100, 30);
    lifeLabel->show();

    // Conectar la señal de cambio de vidas del sprite con la función de actualización de vidas
    connect(Yuri, &sprite::vidaCambiada, this, &MainWindow::actualizarVidas);


    // QLabel para "Siguiente nivel"
    nextLevelLabel = new QLabel(this);
    nextLevelLabel->setText("Siguiente nivel");
    nextLevelLabel->setGeometry(300, 200, 200, 50); // Ajusta la posición y tamaño según sea necesario
    nextLevelLabel->setStyleSheet("QLabel { background-color : white; color : red; font: bold 24px; }");
    nextLevelLabel->setVisible(false);

    // Crear la siguiente escena
    scene2 = new QGraphicsScene();
    scene2->setSceneRect(0, 0, 775, 315);
    //setupExternalWalls(scene2);
    setupScene2();


    // Temporizador para verificar colisiones
    QTimer *collisionTimer = new QTimer(this);
    connect(collisionTimer, &QTimer::timeout, this, &MainWindow::checkCollisions);
    collisionTimer->start(100);

}

void MainWindow::setupScene2()
{
    // Establecer una imagen de fondo para la escena2
    QPixmap background(":/Background.png");
    scene2->setBackgroundBrush(QBrush(background));
/*
    // Plataformas
    paredes.push_back(new pared(100, 250, 50, 10, QColor(0, 255, 0))); // Plataforma 1, color verde radioactivo
    scene2->addItem(paredes.back());

    paredes.push_back(new pared(400, 200, 50, 10, QColor(0, 255, 0))); // Plataforma 2
    scene2->addItem(paredes.back());

    paredes.push_back(new pared(200, 150, 50, 10, QColor(0, 255, 0))); // Plataforma 3
    scene2->addItem(paredes.back());

    paredes.push_back(new pared(250, 120, 50, 10, QColor(0, 255, 0))); // Plataforma 3
    scene2->addItem(paredes.back());

    paredes.push_back(new pared(300, 100, 50, 10, QColor(0, 255, 0))); // Plataforma 3
    scene2->addItem(paredes.back());

    paredes.push_back(new pared(100, 120, 50, 10, QColor(0, 255, 0))); // Plataforma 3
    scene2->addItem(paredes.back());

    paredes.push_back(new pared(130, 80, 50, 10,  QColor(0, 255, 0))); // Plataforma 3
    scene2->addItem(paredes.back());

    paredes.push_back(new pared(130, 280, 50, 10, QColor(0, 255, 0))); // Plataforma 3
    scene2->addItem(paredes.back());

    paredes.push_back(new pared(500, 220, 50, 10, QColor(0, 255, 0))); // Plataforma 3
    scene2->addItem(paredes.back());

    paredes.push_back(new pared(110, 330, 50, 10, QColor(0, 255, 0))); // Plataforma 3
    scene2->addItem(paredes.back());

    // Agregar dos paredes verticales con abertura en el centro
    int wallWidth = 10;
    int wallHeight = 250;
    int opening = 50; // Abertura de 50px
    int centerX = (scene2->width() / 2) - (wallWidth / 2);
    int topY = (scene2->height()/1.3) - (wallHeight / 2) - (opening / 2);

    // Pared superior
    paredes.push_back(new pared(centerX, topY, wallWidth, wallHeight / 2));
    scene2->addItem(paredes.back());

    // Pared inferior
    paredes.push_back(new pared(centerX, topY + wallHeight / 2 + opening, wallWidth, wallHeight / 2));
    scene2->addItem(paredes.back());
*/
    // Agregar sprite animado
    sprite2 = new Sprite2();
    sprite2->setPos(25, 354); // Ajusta la posición según sea necesario
    scene2->addItem(sprite2);
    sprite2->startAnimation(); // Iniciar la animación

}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    int newX = Yuri->x();
    int newY = Yuri->y();
    int newX_sprite2 = sprite2->x();
    int newY_sprite2 = sprite2->y();

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
        if (ui->graphicsView->scene() == scene2) {
            newX_sprite2 -= 2;  // Mover sprite2 hacia la izquierda
            sprite2->setPos(newX_sprite2, newY_sprite2);
        } else {
            newX -= 2;  // Mover Yuri hacia la izquierda
            Yuri->setDirection(sprite::Left);  // Cambiar dirección
        }
        break;
    case Qt::Key_D:
        if (ui->graphicsView->scene() == scene2) {
            newX_sprite2 += 2;  // Mover sprite2 hacia la derecha
            sprite2->setPos(newX_sprite2, newY_sprite2);
        } else {
            newX += 2;  // Mover Yuri hacia la derecha
            Yuri->setDirection(sprite::Right);  // Cambiar dirección
        }
        break;
    case Qt::Key_Space:
        if (ui->graphicsView->scene() == scene2) {
            sprite2->startParabolicMovement(); // Iniciar el movimiento parabólico
            sprite2->startAnimation(); // Iniciar la animación en el eje X
        }
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
    if (Yuri->collidesWithItem(nextLevelTrigger) && !nextLevelActivated) {
        nextLevelLabel->setVisible(true);
        QTimer::singleShot(1000, this, &MainWindow::switchToNextScene); // Cambiar de escena después de 1 segundo
        nextLevelActivated = true;
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
    nextLevelLabel->setVisible(false);

    if (!scene2Initialized) {
        // Crear y agregar un enemigo a la scene2 solo si no ha sido inicializada
        enemigo *nuevoEnemigo = new enemigo(500, 100, 50, 2); // posición (500, 100), tamaño 50, velocidad 2
        scene2->addItem(nuevoEnemigo);

        // Usa un temporizador para actualizar el movimiento del enemigo
        QTimer *timer = new QTimer(this);
        // Configurar el tiempo de actualización en milisegundos
        int tiempoDeActualizacion = 120;
        connect(timer, &QTimer::timeout, nuevoEnemigo, &enemigo::movimiento);
        timer->start(tiempoDeActualizacion); // Configura el tiempo de actualizacion

        scene2Initialized = true; // Marcar scene2 como inicializada

        // Agregar Yuri a la escena2 solo si no ha sido inicializada
        //scene2->addItem(Yuri);
        //Yuri->setPos(25, 374); // Resetear la posición de Yuri en la nueva escena
    }
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

void MainWindow::actualizarVidas(int vidas)
{
    lifeLabel->setText(QString("Vida: %1").arg(vidas));
}

void MainWindow::checkCollisions()
{
    QList<QGraphicsItem *> collidingItems = Yuri->collidingItems();
    foreach (QGraphicsItem *item, collidingItems) {
        if (dynamic_cast<enemigo *>(item)) {
            Yuri->decreaseLife();// Restar vida al colisionar con enemigo
            if (Yuri->vidas <= 0) {
                // Reiniciar el juego
                resetGame();
                break;
            }
        } else if (dynamic_cast<Reactor *>(item)) {
            Yuri->decreaseLife(); // Restar vida al colisionar con el reactor
            if (Yuri->vidas <= 0) {
                // Reiniciar el juego
                resetGame();
                break;
            }
        } else if (dynamic_cast<Radiacion *>(item)) {
            Yuri->decreaseLife(); // Restar vida al colisionar con la radiación
            if (Yuri->vidas <= 0) {
                // Reiniciar el juego
                resetGame();
                break;
            }
        }
    }
}


void MainWindow::resetGame()
{
    // Mostrar "GAME OVER" en rojo y fondo transparente
    QLabel *gameOverLabel = new QLabel(this);
    gameOverLabel->setText("GAME OVER");
    gameOverLabel->setGeometry(300, 200, 200, 50); // Ajusta la posición y tamaño según sea necesario
    gameOverLabel->setStyleSheet("QLabel { background-color : transparent; color : red; font: bold 24px; }");
    gameOverLabel->show();

    // Agregar un botón para reintentar
    QPushButton *retryButton = new QPushButton("Reintentar", this);
    retryButton->setGeometry(300, 250, 200, 50); // Ajusta la posición y tamaño según sea necesario
    connect(retryButton, &QPushButton::clicked, this, &MainWindow::restartGame);
    retryButton->show();
}

void MainWindow::restartGame()
{
    // Eliminar las etiquetas y botones creados durante el "GAME OVER"
    foreach (QObject *object, this->children()) {
        if (QLabel *label = qobject_cast<QLabel *>(object)) {
            delete label;
        } else if (QPushButton *button = qobject_cast<QPushButton *>(object)) {
            delete button;
        }
    }
}
