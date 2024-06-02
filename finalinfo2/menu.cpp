#include "menu.h"
#include <QAction>
#include <QMessageBox>
#include <QApplication>
#include <QGraphicsPixmapItem> // Para agregar una imagen de fondo

Menu::Menu(QObject *parent) : QObject(parent)
{
    // Crear la escena del menú
    menuScene = new QGraphicsScene();
    menuScene->setSceneRect(0, 0, 400, 300); // Establecer el tamaño de la escena

    // Crear el menú
    menu = new QMenu("Menú");

    // Crear las acciones para los botones del menú
    QAction *iniciarJuegoAction = new QAction("Iniciar juego");
    QAction *puntajeMaximoAction = new QAction("Puntaje máximo");
    QAction *salirAction = new QAction("Salir");

    // Agregar las acciones al menú
    menu->addAction(iniciarJuegoAction);
    menu->addAction(puntajeMaximoAction);
    menu->addAction(salirAction);

    // Conectar las acciones a las funciones correspondientes
    connect(iniciarJuegoAction, &QAction::triggered, this, &Menu::iniciarJuego);
    connect(puntajeMaximoAction, &QAction::triggered, this, &Menu::mostrarPuntajeMaximo);
    connect(salirAction, &QAction::triggered, this, &Menu::salir);

    // Agregar el menú a la escena
    QGraphicsProxyWidget *proxyWidget = menuScene->addWidget(menu);
    proxyWidget->setPos(100, 100); // Posición del menú en la escena

    // Agregar un fondo a la escena
    QPixmap background(":/path/to/your/background/image.jpg"); // Cambia la ruta a la imagen de fondo
    QGraphicsPixmapItem *backgroundItem = new QGraphicsPixmapItem(background);
    menuScene->addItem(backgroundItem);
}

void Menu::iniciarJuego()
{
    emit iniciarJuegoSignal(); // Emitir la señal para indicar que se debe iniciar el juego
}

void Menu::mostrarPuntajeMaximo()
{
    // Aquí puedes agregar la lógica para mostrar el puntaje máximo.
    // Por ejemplo:
    QMessageBox::information(nullptr, "Puntaje máximo", "Tu puntaje máximo es: X");
}

void Menu::salir()
{
    // Aquí puedes agregar la lógica para salir de la aplicación.
    QApplication::quit();
}
