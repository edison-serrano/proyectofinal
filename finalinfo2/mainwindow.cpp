#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "pared.h"
#include "puerta.h"
#include "pasarnivel.h"
#include <QLabel>
#include <QPushButton>
#include <QGraphicsPixmapItem>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QVBoxLayout>
#include "enemigo.h"
#include "reactor.h"
#include "radiacion.h"
#include "sprite2.h"
#include "plataforma.h"
#include "QDebug"
#include "vidaextra.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    scene2Initialized(false),
    nextLevelActivated(false),
    collisionFlag(false),
    messageShown(false),
    numeroDeObjetos(0),
    npc1Object(new npc1()),
    collisionTimer(new QTimer(this)),
    autoShowTimer(new QTimer(this)),
    messageTimer(new QTimer(this)),
    pendulo(new Pendulo(50.0, 0.5))


{

    QTimer *collisionTimer = new QTimer(this);
    connect(collisionTimer, &QTimer::timeout, this, &MainWindow::checkCollisions);
    connect(collisionTimer, &QTimer::timeout, this, &MainWindow::updateSprite2Position);
    collisionTimer->start(100);

    scene = new QGraphicsScene();
    scene->setSceneRect(0, 0, 775, 315);

    // Cargar la imagen del fondo desde el archivo de recursos y reducir su tamaño
    QPixmap fondo(":/baldosa.png");
    fondo = fondo.scaledToWidth(100); // Aquí puedes ajustar el tamaño de la imagen reducida

    // Crear un patrón de repetición para la imagen de fondo
    QBrush brush(fondo);

    // Establecer el patrón de repetición como fondo de la escena
    scene->setBackgroundBrush(brush);

    ui->setupUi(this);


    menu = new Menu();
    connect(menu, &Menu::iniciarJuegoSignal, this, &MainWindow::switchToGameScene);

    // Conecta los botones del archivo .ui con las funciones correspondientes
    connect(ui->startGameButton, &QPushButton::clicked, this, &MainWindow::hideButtons);
    connect(ui->highScoreButton, &QPushButton::clicked, menu, &Menu::mostrarPuntajeMaximo);
    connect(ui->quitButton, &QPushButton::clicked,  menu, &Menu::salir);



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

    // Crear un objeto npc1 y agregarlo a la escena
    npc1Object = new npc1();  // Crear en el heap
    scene->addItem(npc1Object);
    npc1Object->setPos(375, 125); // Nota el uso de -> en lugar de .

    // Conectar la señal collisionOccurred de npc1 con el slot showMessage
    connect(npc1Object, &npc1::collisionOccurred, this, &MainWindow::showMessage);

    // Agregar sprite reactor.png con animación
    Reactor *reactor = new Reactor(":/reactor.png", 96, 96, 16, 100); // 96x96 cada img, 16 en total, 100 ms por img
    reactor->setPos(600, 230);
    scene->addItem(reactor);
//**************************************************************************
    inventario2 = nullptr;

    // Crear y agregar el inventario a la escena principal
    inventario = new Inventario();
    inventario->setPos(10, 30);  // Ajusta la posición
    scene->addItem(inventario);

    // Crear y agregar el segundo inventario a la escena principal
    inventario2 = new Inventario();
    inventario2->setPos(720, 100);  // Ajusta la posición
    scene->addItem(inventario2);

    // Crear y agregar el segundo inventario a la escena principal
    inventario3 = new Inventario();
    inventario3->setPos(360, 45);  // Ajusta la posición
    scene->addItem(inventario3);

    // Crear y agregar el segundo inventario a la escena principal
    inventario4 = new Inventario();
    inventario4->setPos(10, 370);  // Ajusta la posición
    scene->addItem(inventario4);

    // Crear y agregar el segundo inventario a la escena principal
    inventario5 = new Inventario();
    inventario5->setPos(520, 220);  // Ajusta la posición
    scene->addItem(inventario5);


    // Crear y agregar un QLabel para mostrar el número de objetos en el inventario
    inventarioLabel = new QLabel(this);
    actualizarInventarioLabel();
    inventarioLabel->setGeometry(300, 10, 200, 30); // Ajusta la posición y tamaño según sea necesario
    inventarioLabel->show();

//***************************************************************************************
    // Crear y agregar un enemigo a la scene
    enemigo *nuevoEnemigo = new enemigo(640, 280, 30, 10); // posición, tamaño 30, velocidad 10
    scene->addItem(nuevoEnemigo);
    // Usa un temporizador para actualizar el movimiento del enemigo
    QTimer *timer = new QTimer(this);
    // Configurar el tiempo de actualización en milisegundos
    int tiempoDeActualizacion = 120;
    connect(timer, &QTimer::timeout, nuevoEnemigo, &enemigo::movimiento);
    timer->start(tiempoDeActualizacion); // Configura el tiempo de actualizacion


    // Crear y agregar un enemigo a la scene
    enemigo *nuevoEnemigo2 = new enemigo(360, 45, 35, 15); // posición, tamaño, velocidad
    scene->addItem(nuevoEnemigo2);
    connect(timer, &QTimer::timeout, nuevoEnemigo2, &enemigo::movimiento);
    timer->start(tiempoDeActualizacion); // Configura el tiempo de actualizacion


//********************************************************************************

    //escenario .. scene
    QPixmap cajaAPixmap(":/caja1.png");
    QGraphicsPixmapItem *cajaAItem = new QGraphicsPixmapItem(cajaAPixmap);
    cajaAItem->setPos(10, 150);
    scene->addItem(cajaAItem);

    QGraphicsPixmapItem *cajaAItem2 = new QGraphicsPixmapItem(cajaAPixmap);
    cajaAItem2->setPos(10, 120);
    scene->addItem(cajaAItem2);

    QGraphicsPixmapItem *cajaAItem3 = new QGraphicsPixmapItem(cajaAPixmap);
    cajaAItem3->setPos(10, 90);
    scene->addItem(cajaAItem3);

    //********************************************

    QPixmap cajaBPixmap(":/caja2.png");
    QGraphicsPixmapItem *cajaBItem = new QGraphicsPixmapItem(cajaBPixmap);
    cajaBItem->setPos(10, 60);
    scene->addItem(cajaBItem);

    QGraphicsPixmapItem *cajaB1Item = new QGraphicsPixmapItem(cajaBPixmap);
    cajaB1Item->setPos(10, 20);
    scene->addItem(cajaB1Item);

    QGraphicsPixmapItem *cajaB2Item = new QGraphicsPixmapItem(cajaBPixmap);
    cajaB2Item->setPos(700, 100);
    scene->addItem(cajaB2Item);

    //********************************************

    QPixmap armarioPixmap(":/armario1.png");
    QGraphicsPixmapItem *armarioItem = new QGraphicsPixmapItem(armarioPixmap);
    armarioItem->setPos(60, 12);
    scene->addItem(armarioItem);

    QGraphicsPixmapItem *armario1Item = new QGraphicsPixmapItem(armarioPixmap);
    armario1Item->setPos(90, 12);
    scene->addItem(armario1Item);

    QGraphicsPixmapItem *armario2Item = new QGraphicsPixmapItem(armarioPixmap);
    armario2Item->setPos(120, 12);
    scene->addItem(armario2Item);

    QGraphicsPixmapItem *armario3Item = new QGraphicsPixmapItem(armarioPixmap);
    armario3Item->setPos(150, 12);
    scene->addItem(armario3Item);


    // barriles


    QPixmap barrilAPixmap(":/barril1.png");
    QGraphicsPixmapItem *barrilItem = new QGraphicsPixmapItem(barrilAPixmap);
    barrilItem->setPos(350, 45);
    scene->addItem(barrilItem);

    QGraphicsPixmapItem *barril1Item = new QGraphicsPixmapItem(barrilAPixmap);
    barril1Item->setPos(320, 45);
    scene->addItem(barril1Item);

    QGraphicsPixmapItem *barril2Item = new QGraphicsPixmapItem(barrilAPixmap);
    barril2Item->setPos(400, 35);
    scene->addItem(barril2Item);

    QGraphicsPixmapItem *barril3Item = new QGraphicsPixmapItem(barrilAPixmap);
    barril3Item->setPos(400, 65);
    scene->addItem(barril3Item);

    //tuberias

    QPixmap tuberiaPixmap(":/tuberia1.png");
    QGraphicsPixmapItem *tuberiaItem = new QGraphicsPixmapItem(tuberiaPixmap);
    tuberiaItem->setPos(720, 160);
    scene->addItem(tuberiaItem);

    QGraphicsPixmapItem *tuberia1Item = new QGraphicsPixmapItem(tuberiaPixmap);
    tuberia1Item->setPos(720, 130);
    scene->addItem(tuberia1Item);

    QGraphicsPixmapItem *tuberia3Item = new QGraphicsPixmapItem(tuberiaPixmap);
    tuberia3Item->setPos(720, 100);
    scene->addItem(tuberia3Item);

    QGraphicsPixmapItem *tuberia4Item = new QGraphicsPixmapItem(tuberiaPixmap);
    tuberia4Item->setPos(720, 70);
    scene->addItem(tuberia4Item);

    QGraphicsPixmapItem *tuberia5Item = new QGraphicsPixmapItem(tuberiaPixmap);
    tuberia5Item->setPos(690, 100);
    tuberia5Item->setRotation(-90);
    scene->addItem(tuberia5Item);

    QGraphicsPixmapItem *tuberia6Item = new QGraphicsPixmapItem(tuberiaPixmap);
    tuberia6Item->setPos(640, 100);
    tuberia6Item->setRotation(-90);
    scene->addItem(tuberia6Item);

    QGraphicsPixmapItem *tuberia7Item = new QGraphicsPixmapItem(tuberiaPixmap);
    tuberia7Item->setPos(610, 100);
    tuberia7Item->setRotation(-90);
    scene->addItem(tuberia7Item);

    QGraphicsPixmapItem *tuberia8Item = new QGraphicsPixmapItem(tuberiaPixmap);
    tuberia8Item->setPos(580, 100);
    tuberia8Item->setRotation(-90);
    scene->addItem(tuberia8Item);

 //*******************************************************************************

    VidaExtra *vidaExtra = new VidaExtra();
    scene->addItem(vidaExtra);  // Añade la vida extra a la escena
    vidaExtra->setPos(100, 100);  // Coloca la vida extra en una posición deseada en la escena


//*******************************************************************************



    // Agregar sprite radiacion.png con animación y movimiento aleatorio
    Radiacion *radiacion = new Radiacion(":/radiacion.png", 192, 192, 5, 4, 100, scene, &paredes); // 192x192 cada img, 5x4 en total, 100 ms por img
    radiacion->setPos(50, 200);
    scene->addItem(radiacion);

    // Agregar sprite radiacion.png con animación y movimiento aleatorio
    Radiacion *radiacion1 = new Radiacion(":/radiacion.png", 192, 192, 5, 4, 100, scene, &paredes); // 192x192 cada img, 5x4 en total, 100 ms por img
    radiacion1->setPos(580, -50);
    scene->addItem(radiacion1);


    // Objeto pasarnivel
    nextLevelTrigger = new pasarnivel(740, 355, 15, 30);
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
    nextLevelLabel->setGeometry(300, 200, 200, 50); // Ajusta la posición y tamaño
    nextLevelLabel->setStyleSheet("QLabel { background-color : white; color : red; font: bold 24px; }");
    nextLevelLabel->setVisible(false);

    // Crear la siguiente escena
    scene2 = new QGraphicsScene();
    scene2->setSceneRect(0, 0, 775, 315);
    //setupExternalWalls(scene2);
    Radiacion *radiacion2 = new Radiacion(":/radiacion.png", 192, 192, 5, 2, 100, scene2, &paredes); // 192x192 cada img, 5x4 en total, 100 ms por img
    radiacion2->setPos(250, 200);
    scene2->addItem(radiacion2);

    Radiacion *radiacion3 = new Radiacion(":/radiacion.png", 192, 192, 5, 4, 100, scene2, &paredes); // 192x192 cada img, 5x4 en total, 100 ms por img
    radiacion3->setPos(100, 20);
    scene2->addItem(radiacion3);

    Radiacion *radiacion4 = new Radiacion(":/radiacion.png", 192, 192, 5, 4, 80, scene2, &paredes); // 192x192 cada img, 5x4 en total, 100 ms por img
    radiacion4->setPos(124, 340);
    scene2->addItem(radiacion4);

    Radiacion *radiacion5 = new Radiacion(":/radiacion.png", 192, 192, 5, 4, 80, scene2, &paredes); // 192x192 cada img, 5x4 en total, 100 ms por img
    radiacion5->setPos(155, 340);
    scene2->addItem(radiacion5);

    Radiacion *radiacion6 = new Radiacion(":/radiacion.png", 192, 192, 5, 4, 80, scene2, &paredes); // 192x192 cada img, 5x4 en total, 100 ms por img
    radiacion6->setPos(186, 340);
    scene2->addItem(radiacion6);

    Radiacion *radiacion7 = new Radiacion(":/radiacion.png", 192, 192, 5, 4, 80, scene2, &paredes); // 192x192 cada img, 5x4 en total, 100 ms por img
    radiacion7->setPos(217, 340);
    scene2->addItem(radiacion7);

    Radiacion *radiacion8 = new Radiacion(":/radiacion.png", 192, 192, 5, 4, 80, scene2, &paredes); // 192x192 cada img, 5x4 en total, 100 ms por img
    radiacion8->setPos(248, 340);
    scene2->addItem(radiacion8);

    Radiacion *radiacion9 = new Radiacion(":/radiacion.png", 192, 192, 5, 4, 80, scene2, &paredes); // 192x192 cada img, 5x4 en total, 100 ms por img
    radiacion9->setPos(279, 340);
    scene2->addItem(radiacion9);

    Radiacion *radiacion10 = new Radiacion(":/radiacion.png", 192, 192, 5, 4, 80, scene2, &paredes); // 192x192 cada img, 5x4 en total, 100 ms por img
    radiacion10->setPos(310, 340);
    scene2->addItem(radiacion10);

    Radiacion *radiacion11 = new Radiacion(":/radiacion.png", 192, 192, 5, 4, 80, scene2, &paredes); // 192x192 cada img, 5x4 en total, 100 ms por img
    radiacion11->setPos(341, 340);
    scene2->addItem(radiacion11);

    Radiacion *radiacion12 = new Radiacion(":/radiacion.png", 192, 192, 5, 4, 80, scene2, &paredes); // 192x192 cada img, 5x4 en total, 100 ms por img
    radiacion12->setPos(372, 340);
    scene2->addItem(radiacion12);

    Radiacion *radiacion13 = new Radiacion(":/radiacion.png", 192, 192, 5, 4, 80, scene2, &paredes); // 192x192 cada img, 5x4 en total, 100 ms por img
    radiacion13->setPos(403, 340);
    scene2->addItem(radiacion13);

    Radiacion *radiacion14 = new Radiacion(":/radiacion.png", 192, 192, 5, 4, 80, scene2, &paredes); // 192x192 cada img, 5x4 en total, 100 ms por img
    radiacion14->setPos(434, 340);
    scene2->addItem(radiacion14);

    Radiacion *radiacion15 = new Radiacion(":/radiacion.png", 192, 192, 5, 4, 80, scene2, &paredes); // 192x192 cada img, 5x4 en total, 100 ms por img
    radiacion15->setPos(465, 340);
    scene2->addItem(radiacion15);
    setupScene2();

    // Agregar el péndulo a la escena 2
    pendulo->setPos(150, 200); // Ajusta la posición según sea necesario
    scene2->addItem(pendulo);

    // Configurar un temporizador para actualizar el péndulo periódicamente
    QTimer *timer2 = new QTimer(this);
    connect(timer2, &QTimer::timeout, this, &MainWindow::actualizarPendulo);
    timer2->start(16); // Actualizar aproximadamente cada 16 ms (~60 FPS)

    //****************************************************************************

    connect(collisionTimer, &QTimer::timeout, this, &MainWindow::checkCollisions);
    connect(collisionTimer, &QTimer::timeout, this, &MainWindow::updateSprite2Position);
    connect(autoShowTimer, &QTimer::timeout, this, &MainWindow::resetCollisionFlag);
    connect(autoShowTimer, &QTimer::timeout, this, &MainWindow::autoShowMessage); // Conexión del temporizador
    connect(messageTimer, &QTimer::timeout, this, &MainWindow::resetMessageTimer);

    // Inicia los temporizadores
    autoShowTimer->setInterval(10000);
    collisionTimer->start(100);
    autoShowTimer->start(1000);  // Inicia autoShowTimer
    messageTimer->start(10000);  // Inicia messageTimer
    //**************************************************************************

}

void MainWindow::hideButtons()
{
    ui->startGameButton->hide();
    ui->highScoreButton->hide();
    ui->quitButton->hide();

    // Emitir la señal para iniciar el juego
    menu->iniciarJuegoSignal();
}


void MainWindow::switchToGameScene()
{
    ui->graphicsView->setScene(scene); // Cambiar a la escena del juego
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

    paredes.push_back(new pared(70, 350, 50, 10, QColor(0, 255, 0))); // Plataforma 3
    scene2->addItem(paredes.back());
*/
    paredes.push_back(new pared(310, 260, 160, 10)); // Pared horizontal centro inferior
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

    paredes.push_back(new pared(0, 195, 257, 10)); // Pared horizontal izquierda
    scene2->addItem(paredes.back());
    paredes.push_back(new pared(513, 195, 257, 10)); // Pared horizontal derecha
    scene2->addItem(paredes.back());

    setupExternalWalls(scene2);


    // Crear una instancia de Plataforma y agregarla a la escena2
    Plataforma *plataforma = new Plataforma();
    plataforma->setPos(100, 343); // Establece la posición de la plataforma
    scene2->addItem(plataforma);

    Plataforma *plataforma1 = new Plataforma();
    plataforma1->setPos(132, 343); // Establece la posición de la plataforma
    scene2->addItem(plataforma1);

    Plataforma *plataforma2a = new Plataforma();
    plataforma2a->setPos(163, 320); // Establece la posición de la plataforma
    scene2->addItem(plataforma2a);

    Plataforma *plataforma2b = new Plataforma();
    plataforma2b->setPos(190, 320); // Establece la posición de la plataforma
    scene2->addItem(plataforma2b);

    Plataforma *plataformasuelo = new Plataforma();
    plataformasuelo->setPos(0, 390); // Establece la posición de la plataforma
    scene2->addItem(plataformasuelo);

    Plataforma *plataformasuelo1 = new Plataforma();
    plataformasuelo1->setPos(31, 390); // Establece la posición de la plataforma
    scene2->addItem(plataformasuelo1);

    Plataforma *plataformasuelo4 = new Plataforma();
    plataformasuelo4->setPos(62, 390); // Establece la posición de la plataforma
    scene2->addItem(plataformasuelo4);

    Plataforma *plataformasuelo5 = new Plataforma();
    plataformasuelo5->setPos(93, 390); // Establece la posición de la plataforma
    scene2->addItem(plataformasuelo5);

    Plataforma *plataformasuelo6 = new Plataforma();
    plataformasuelo6->setPos(124, 390); // Establece la posición de la plataforma
    scene2->addItem(plataformasuelo6);

    Plataforma *plataformasuelo7 = new Plataforma();
    plataformasuelo7->setPos(155, 390); // Establece la posición de la plataforma
    scene2->addItem(plataformasuelo7);

    Plataforma *plataformasuelo8 = new Plataforma();
    plataformasuelo8->setPos(186, 390); // Establece la posición de la plataforma
    scene2->addItem(plataformasuelo8);

    Plataforma *plataformasuelo9 = new Plataforma();
    plataformasuelo9->setPos(217, 390); // Establece la posición de la plataforma
    scene2->addItem(plataformasuelo9);

    Plataforma *plataformasuelo10 = new Plataforma();
    plataformasuelo10->setPos(248, 390); // Establece la posición de la plataforma
    scene2->addItem(plataformasuelo10);

    Plataforma *plataformasuelo11 = new Plataforma();
    plataformasuelo11->setPos(279, 390); // Establece la posición de la plataforma
    scene2->addItem(plataformasuelo11);

    Plataforma *plataformasuelo12 = new Plataforma();
    plataformasuelo12->setPos(310, 390); // Establece la posición de la plataforma
    scene2->addItem(plataformasuelo12);

    Plataforma *plataformasuelo13 = new Plataforma();
    plataformasuelo13->setPos(341, 390); // Establece la posición de la plataforma
    scene2->addItem(plataformasuelo13);

    Plataforma *plataformasuelo14 = new Plataforma();
    plataformasuelo14->setPos(372, 390); // Establece la posición de la plataforma
    scene2->addItem(plataformasuelo14);

    Plataforma *plataformasuelo15 = new Plataforma();
    plataformasuelo15->setPos(403, 390); // Establece la posición de la plataforma
    scene2->addItem(plataformasuelo15);

    Plataforma *plataformasuelo16 = new Plataforma();
    plataformasuelo16->setPos(434, 390); // Establece la posición de la plataforma
    scene2->addItem(plataformasuelo16);

    Plataforma *plataformasuelo17 = new Plataforma();
    plataformasuelo17->setPos(465, 390); // Establece la posición de la plataforma
    scene2->addItem(plataformasuelo17);

    Plataforma *plataformasuelo18 = new Plataforma();
    plataformasuelo18->setPos(496, 390); // Establece la posición de la plataforma
    scene2->addItem(plataformasuelo18);

    Plataforma *plataformasuelo19 = new Plataforma();
    plataformasuelo19->setPos(527, 390); // Establece la posición de la plataforma
    scene2->addItem(plataformasuelo19);

    Plataforma *plataformasuelo20 = new Plataforma();
    plataformasuelo20->setPos(558, 390); // Establece la posición de la plataforma
    scene2->addItem(plataformasuelo20);

    Plataforma *plataformasuelo21 = new Plataforma();
    plataformasuelo21->setPos(589, 390); // Establece la posición de la plataforma
    scene2->addItem(plataformasuelo21);

    Plataforma *plataformasuelo22 = new Plataforma();
    plataformasuelo22->setPos(620, 390); // Establece la posición de la plataforma
    scene2->addItem(plataformasuelo22);

    Plataforma *plataformasuelo23 = new Plataforma();
    plataformasuelo23->setPos(651, 390); // Establece la posición de la plataforma
    scene2->addItem(plataformasuelo23);

    Plataforma *plataformasuelo24 = new Plataforma();
    plataformasuelo24->setPos(682, 390); // Establece la posición de la plataforma
    scene2->addItem(plataformasuelo24);

    Plataforma *plataformasuelo25 = new Plataforma();
    plataformasuelo25->setPos(713, 390); // Establece la posición de la plataforma
    scene2->addItem(plataformasuelo25);

    Plataforma *plataformasuelo26 = new Plataforma();
    plataformasuelo26->setPos(744, 390); // Establece la posición de la plataforma
    scene2->addItem(plataformasuelo26);



    Plataforma *plataformaup = new Plataforma();
    plataformaup->setPos(310, 260); // Establece la posición de la plataforma
    scene2->addItem(plataformaup);

    Plataforma *plataformaup2 = new Plataforma();
    plataformaup2->setPos(338, 260); // Establece la posición de la plataforma
    scene2->addItem(plataformaup2);

    Plataforma *plataformaup3 = new Plataforma();
    plataformaup3->setPos(366, 260); // Establece la posición de la plataforma
    scene2->addItem(plataformaup3);

    Plataforma *plataformaup4 = new Plataforma();
    plataformaup4->setPos(394, 260); // Establece la posición de la plataforma
    scene2->addItem(plataformaup4);

    Plataforma *plataformaup5 = new Plataforma();
    plataformaup5->setPos(422, 260); // Establece la posición de la plataforma
    scene2->addItem(plataformaup5);

    Plataforma *plataformaup6 = new Plataforma();
    plataformaup6->setPos(450, 260); // Establece la posición de la plataforma
    scene2->addItem(plataformaup6);

    Plataforma *plataformaizq = new Plataforma();
    plataformaizq->setPos(0, 195); // Establece la posición de la plataforma
    scene2->addItem(plataformaizq);

    Plataforma *plataformaizq2 = new Plataforma();
    plataformaizq2->setPos(28, 195); // Establece la posición de la plataforma
    scene2->addItem(plataformaizq2);

    Plataforma *plataformaizq3 = new Plataforma();
    plataformaizq3->setPos(56, 195); // Establece la posición de la plataforma
    scene2->addItem(plataformaizq3);

    // Agregar sprite animado
    sprite2 = new Sprite2();
    sprite2->setPos(25, 343); // Ajusta la posición según sea necesario
    scene2->addItem(sprite2);
    sprite2->startAnimation(); // Iniciar la animación

}

//**************************************************************************************

void MainWindow::checkCollisionForMessage() {
    // Verificar si Yuri está en colisión con NPC1 y el mensaje no se ha mostrado previamente
    if (Yuri->collidesWithItem(npc1Object) && !messageShown) {
        showMessage(); // Mostrar el mensaje
    }
}

void MainWindow::showMessage() {
    QFile file("../instrucciones.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "No se pudo abrir el archivo de instrucciones.";
        return;
    }

    // Leer el contenido del archivo de instrucciones
    QTextStream in(&file);
    QString message = in.readAll();

    // Cerrar el archivo
    file.close();

    // Crear un cuadro de diálogo personalizado
    QDialog dialog(this);
    QVBoxLayout layout(&dialog);
    QLabel label(&dialog);
    label.setText(message);
    layout.addWidget(&label);
    QPushButton nextButton("Siguiente", &dialog);
    layout.addWidget(&nextButton);
    dialog.setLayout(&layout);

    // Conectar el botón "Siguiente" para cerrar el cuadro de diálogo
    connect(&nextButton, &QPushButton::clicked, &dialog, &QDialog::accept);

    // Mostrar el cuadro de diálogo
    dialog.setWindowTitle("Instrucciones");
    dialog.exec();

    messageShown = true; // Establecer la bandera a true para indicar que el mensaje se ha mostrado
    // Iniciar el temporizador para esperar 10 segundos antes de restablecer la bandera
    messageTimer->start(10000); // 10 segundos
}

void MainWindow::autoShowMessage() {
    // Inicia el temporizador solo si el temporizador de autoShowTimer no está activo
    if (!autoShowTimer->isActive()) {
        autoShowTimer->start();
    }
}

void MainWindow::resetCollisionFlag() {
    // Reinicia la bandera de colisión a false cuando el temporizador de autoShowTimer termina
    collisionFlag = false;
}

void MainWindow::resetMessageTimer() {
    // Detener el temporizador si está activo
    if (messageTimer->isActive()) {
        messageTimer->stop();
    }
    messageShown = false; // Restablecer la bandera cuando el temporizador termina
}
//*******************************************************************************



MainWindow::~MainWindow()
{
    delete ui;
    delete pendulo;
}

// Función de manejo de eventos de teclado
void MainWindow::keyPressEvent(QKeyEvent *event)
{
    int newX = Yuri->x();
    int newY = Yuri->y();
    int newX_sprite2 = sprite2->x();
    int newY_sprite2 = sprite2->y();

    switch (event->key()) {
    case Qt::Key_W:
        if (ui->graphicsView->scene() == scene2) {
            newY_sprite2 -= 70;  // Mover sprite2 hacia arriba
            if (!checkPlatformCollision(sprite2, newX_sprite2, newY_sprite2)) {
                sprite2->setPos(newX_sprite2, newY_sprite2);
            }
        } else {
            newY -= 2;  // Mover hacia arriba
            Yuri->setDirection(sprite::Up);  // Cambiar dirección
        }
        break;
    case Qt::Key_S:
        newY += 2;  // Mover hacia abajo
        Yuri->setDirection(sprite::Down);  // Cambiar dirección
        break;
    case Qt::Key_A:
        if (ui->graphicsView->scene() == scene2) {
            sprite2->setFacingRight(false);  // Cambiar la dirección del sprite
            newX_sprite2 -= 2;  // Mover sprite2 hacia la izquierda
            if (!checkPlatformCollision(sprite2, newX_sprite2, newY_sprite2)) {
                sprite2->setPos(newX_sprite2, newY_sprite2);
            }
        } else {
            newX -= 2;  // Mover Yuri hacia la izquierda
            Yuri->setDirection(sprite::Left);  // Cambiar dirección
        }
        break;
    case Qt::Key_D:
        if (ui->graphicsView->scene() == scene2) {
            sprite2->setFacingRight(true);  // Cambiar la dirección del sprite
            newX_sprite2 += 2;  // Mover sprite2 hacia la derecha
            if (!checkPlatformCollision(sprite2, newX_sprite2, newY_sprite2)) {
                sprite2->setPos(newX_sprite2, newY_sprite2);
            }
        } else {
            newX += 2;  // Mover Yuri hacia la derecha
            Yuri->setDirection(sprite::Right);  // Cambiar dirección
        }
        break;
    case Qt::Key_Space:
        if (ui->graphicsView->scene() == scene2) {
            sprite2->startParabolicMovement(); // Iniciar el movimiento parabólico
        }
        break;
    default:
        QMainWindow::keyPressEvent(event);
        return;
    }

    // Verificar colisión con npc1 y mostrar el mensaje si es necesario
    checkCollisionForMessage();

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


    //**************************************************************
    // Verificar colisión con inventario1
    if (Yuri->collidesWithItem(inventario)) {
        // Incrementar el número de objetos en el inventario
        numeroDeObjetos++;
        // Actualizar el QLabel
        actualizarInventarioLabel();
        // Eliminar el inventario de la escena
        scene->removeItem(inventario);
        delete inventario;
        inventario = nullptr;  // Eliminar el puntero para evitar uso posterior
    }

    // Verificar colisión con inventario2
    if (Yuri->collidesWithItem(inventario2)) {
        // Incrementar el número de objetos en el inventario
        numeroDeObjetos++;
        // Actualizar el QLabel
        actualizarInventarioLabel();
        // Eliminar el inventario de la escena
        scene->removeItem(inventario2);
        delete inventario2;
        inventario2 = nullptr;  // Eliminar el puntero para evitar uso posterior
    }

    // Verificar colisión con inventario3
    if (Yuri->collidesWithItem(inventario3)) {
        // Incrementar el número de objetos en el inventario
        numeroDeObjetos++;
        // Actualizar el QLabel
        actualizarInventarioLabel();
        // Eliminar el inventario de la escena
        scene->removeItem(inventario3);
        delete inventario3;
        inventario3 = nullptr;  // Eliminar el puntero para evitar uso posterior
    }

    // Verificar colisión con inventario4
    if (Yuri->collidesWithItem(inventario4)) {
        // Incrementar el número de objetos en el inventario
        numeroDeObjetos++;
        // Actualizar el QLabel
        actualizarInventarioLabel();
        // Eliminar el inventario de la escena
        scene->removeItem(inventario4);
        delete inventario4;
        inventario4 = nullptr;  // Eliminar el puntero para evitar uso posterior
    }

    // Verificar colisión con inventario5
    if (Yuri->collidesWithItem(inventario5)) {
        // Incrementar el número de objetos en el inventario
        numeroDeObjetos++;
        // Actualizar el QLabel
        actualizarInventarioLabel();
        // Eliminar el inventario de la escena
        scene->removeItem(inventario5);
        delete inventario5;
        inventario5 = nullptr;  // Eliminar el puntero para evitar uso posterior
    }


    //***********************************************************************************

    // Verificar colisión con pasarnivel
    if (Yuri->collidesWithItem(nextLevelTrigger) && !nextLevelActivated) {
        if (numeroDeObjetos >= 5) {
            nextLevelLabel->setVisible(true);
            QTimer::singleShot(1000, this, &MainWindow::switchToNextScene); // Cambiar de escena después de 1 segundo
            nextLevelActivated = true;
        } else {
            // Mostrar un mensaje de advertencia si no tiene suficientes objetos
            QMessageBox::warning(this, "Advertencia", "¡Faltan objetos para pasar de nivel!");
        }
    }
}




void MainWindow::actualizarInventarioLabel() {
    inventarioLabel->setText(QString("Objetos: %1").arg(numeroDeObjetos));
}



void MainWindow::updateSprite2Position() {
    if (ui->graphicsView->scene() == scene2) {
        int newX_sprite2 = sprite2->x();
        int newY_sprite2 = sprite2->y() +2; // Ajustar la velocidad de caída

        bool onPlatform = false;
        QList<QGraphicsItem *> collidingItemsList = scene2->collidingItems(sprite2);
        foreach (QGraphicsItem *item, collidingItemsList) {
            if (Plataforma *plataforma = dynamic_cast<Plataforma*>(item)) {
                QRectF platformRect = plataforma->boundingRect().translated(plataforma->pos());
                QRectF spriteRect(newX_sprite2, newY_sprite2, sprite2->boundingRect().width(), sprite2->boundingRect().height());
                if (spriteRect.intersects(platformRect)) {
                    newY_sprite2 = plataforma->y() - sprite2->boundingRect().height();
                    onPlatform = true;
                    break;
                }
            }
        }

        sprite2->setPos(newX_sprite2, newY_sprite2);

        if (!onPlatform) {
            QTimer::singleShot(2, this, &MainWindow::updateSprite2Position); // Ajustar el intervalo para una actualización más suave
        }
    }
}


bool MainWindow::checkPlatformCollision(Sprite2 *sprite, int newX, int newY)
{
    QRectF newRect(newX, newY, sprite->boundingRect().width(), sprite->boundingRect().height());
    foreach (QGraphicsItem *item, sprite->collidingItems()) {
        if (Plataforma *plataforma = dynamic_cast<Plataforma *>(item)) {
            QRectF platformRect = plataforma->boundingRect().translated(plataforma->pos());
            if (newRect.intersects(platformRect)) {
                return true;
            }
        }
    }
    return false;
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
            Yuri->decreaseLife();  // Restar vida al colisionar con enemigo
            if (Yuri->vidas <= 0) {
                // Reiniciar el juego
                resetGame();
                break;
            }
        } else if (dynamic_cast<Reactor *>(item)) {
            Yuri->decreaseLife();  // Restar vida al colisionar con el reactor
            if (Yuri->vidas <= 0) {
                // Reiniciar el juego
                resetGame();
                break;
            }
        } else if (dynamic_cast<Radiacion *>(item)) {
            Yuri->decreaseLife();  // Restar vida al colisionar con la radiación
            if (Yuri->vidas <= 0) {
                // Reiniciar el juego
                resetGame();
                break;
            }
        } else if (dynamic_cast<VidaExtra *>(item)) {
            if (Yuri->vidas <= 50) {
                Yuri->vidas = 100;
            } else {
                Yuri->vidas += 50;  // Añadir 50 puntos de vida al colisionar con VidaExtra
            }
            emit Yuri->vidaCambiada(Yuri->vidas);  // Emitir la señal para actualizar la UI

            scene->removeItem(item);  // Eliminar VidaExtra de la escena
            delete item;  // Liberar memoria
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

void MainWindow::actualizarPendulo() {
    pendulo->actualizar(0.16); // Actualizar con un paso de tiempo de 0.016 segundos (16 ms)
}
