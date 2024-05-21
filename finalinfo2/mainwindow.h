#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QKeyEvent>
#include <QList>
#include <QTimer>
#include <QMap>
#include <QLabel>
#include "sprite.h"
#include "pared.h"
#include "puerta.h"
#include "pasarnivel.h"
#include "enemigo.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void keyPressEvent(QKeyEvent *event) override;

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    QGraphicsScene *scene2; // Nueva escena para el siguiente nivel
    sprite *Yuri;
    QList<pared *> paredes;
    QList<puerta *> puertas;
    pasarnivel *nextLevelTrigger; // Objeto pasarnivel
    QLabel *nextLevelLabel; // QLabel para "Siguiente nivel"

    QMap<puerta*, QPointF> puertaOriginalPositions; // Para guardar las posiciones originales de las puertas
    QMap<puerta*, QTimer*> puertaTimers; // Para manejar los temporizadores de las puertas

    void setupExternalWalls(QGraphicsScene *scene); // Método para configurar las paredes externas

private slots:
    void closeDoor(puerta *p); // Slot para cerrar la puerta después de 3 segundos
    void switchToNextScene(); // Slot para cambiar a la siguiente escena
};

#endif // MAINWINDOW_H
