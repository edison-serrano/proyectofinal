#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    scene = new QGraphicsScene();
    scene->setSceneRect(0,0,775,315);
    ui->setupUi(this);

    ui->graphicsView->setScene(scene);

    //paredes externas
    paredes.push_back(new pared(0,0,770,10));//pared arriba
    scene->addItem(paredes.back());
    paredes.push_back(new pared(0,390,770,10));// pared abajo
    scene->addItem(paredes.back());
    paredes.push_back(new pared(0,0,10,390));//pared izquierda
    scene->addItem(paredes.back());
    paredes.push_back(new pared(760,0,10,390));//pared derecha
    scene->addItem(paredes.back());


    //para personaje principal
    Yuri = new sprite();
    scene->addItem(Yuri);
    Yuri->setPos(50,50);
}

MainWindow::~MainWindow()
{
    delete ui;
}
