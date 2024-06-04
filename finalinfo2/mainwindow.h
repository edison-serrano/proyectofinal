// mainwindow.h
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QKeyEvent>
#include <QList>
#include <QTimer>
#include <QMap>
#include <QLabel>
#include <QPushButton>
#include "sprite.h"
#include "sprite2.h"
#include "pared.h"
#include "puerta.h"
#include "pasarnivel.h"
#include "enemigo.h"
#include "menu.h"
#include "inventario.h"
#include "npc1.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // Agrega la declaración del método hideButtons
    void hideButtons();
    bool checkPlatformCollision(Sprite2 *sprite, int newX, int newY);
    void updateSprite2Position();

protected:
    void keyPressEvent(QKeyEvent *event) override;

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    QGraphicsScene *scene2; // Nueva escena para el siguiente nivel
    sprite *Yuri;
    //sprite2 *Yuri2;
    QList<pared *> paredes;
    QList<puerta *> puertas;
    pasarnivel *nextLevelTrigger; // Objeto pasarnivel
    QLabel *nextLevelLabel; // QLabel para "Siguiente nivel"
    QLabel *lifeLabel; // QLabel para mostrar las vidas restantes

    QMap<puerta*, QPointF> puertaOriginalPositions; // Para guardar las posiciones originales de las puertas
    QMap<puerta*, QTimer*> puertaTimers; // Para manejar los temporizadores de las puertas

    void setupExternalWalls(QGraphicsScene *scene); // Método para configurar las paredes externas
    void setupScene2(); // Método para configurar la segunda escena
    Sprite2 *sprite2;
    bool scene2Initialized;
    bool nextLevelActivated;
    Inventario* inventario;
    npc1 *npc1Object;
    //npc1 npc1Object;


    int numeroDeObjetos;
    QLabel *inventarioLabel;
    void actualizarInventarioLabel();

    Menu *menu; // Agregar instancia de Menu como miembro privado

private slots:
    void closeDoor(puerta *p); // Slot para cerrar la puerta después de 3 segundos
    void switchToNextScene(); // Slot para cambiar a la siguiente escena
    void actualizarVidas(int vidas); // Slot para actualizar el QLabel de vidas
    void checkCollisions(); // Slot para verificar colisiones con enemigos
    void resetGame();
    void restartGame();
    void switchToGameScene(); // Slot para cambiar a la escena del juego
    void showMessage();
};

#endif // MAINWINDOW_H
