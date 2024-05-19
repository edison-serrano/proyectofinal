#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QKeyEvent>
#include <QList>
#include <QTimer>
#include <QMap>
#include "sprite.h"
#include "pared.h"
#include "puerta.h"

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
    sprite *Yuri;
    QList<pared *> paredes;
    QList<puerta *> puertas;

    QMap<puerta*, QPointF> puertaOriginalPositions; // Para guardar las posiciones originales de las puertas
    QMap<puerta*, QTimer*> puertaTimers; // Para manejar los temporizadores de las puertas

private slots:
    void closeDoor(puerta *p); // Slot para cerrar la puerta después de 3 segundos
};

#endif // MAINWINDOW_H

