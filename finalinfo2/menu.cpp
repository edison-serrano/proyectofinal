#include "menu.h"
#include <QAction>
#include <QMessageBox>
#include <QApplication>
#include <QGraphicsPixmapItem> // Para agregar una imagen de fondo
#include <QInputDialog>
#include <QTextStream>
#include <QFile>

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

}

void Menu::iniciarJuego()
{
    emit iniciarJuegoSignal(); // Emitir la señal para indicar que se debe iniciar el juego
}

void Menu::mostrarPuntajeMaximo()
{
    // Pedir al usuario que ingrese su nombre
    QString nombre = QInputDialog::getText(nullptr, "Ingrese su nombre", "Nombre:");

    // Obtener el puntaje máximo (reemplazar 'X' con el valor realR(FALTA CONFIGURA)
    int puntajeMaximo = obtenerPuntajeMaximo();

    // Mostrar el puntaje máximo
    QMessageBox::information(nullptr, "Puntaje máximo", "Tu puntaje máximo es: " + QString::number(puntajeMaximo));

    // Guardar el nombre del jugador y su puntaje máximo en el archivo
    guardarJugador(nombre, puntajeMaximo);
}

int Menu::obtenerPuntajeMaximo()
{
    // Aquí implementa la lógica para obtener el puntaje máximo
    return 100; // Cambiar por lógica real para obtener el puntaje máximo(FALTA)
}

void Menu::guardarJugador(const QString &nombre, int puntaje)
{
    // Abrir el archivo en modo de escritura (si no existe, se creará)
    QFile file("../jugadores.txt");
    if (file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        // Crear un stream de texto para escribir en el archivo
        QTextStream stream(&file);
        // Escribir el nombre del jugador y su puntaje en una línea del archivo
        stream << nombre << " - Puntaje máximo: " << puntaje << Qt::endl;
        // Cerrar el archivo
        file.close();
    } else {
        // Manejar el caso en el que no se pudo abrir el archivo
        QMessageBox::critical(nullptr, "Error", "No se pudo abrir el archivo para guardar el puntaje.");
    }
}


void Menu::salir()
{
    // Aquí puedes agregar la lógica para salir de la aplicación.
    QApplication::quit();
}
