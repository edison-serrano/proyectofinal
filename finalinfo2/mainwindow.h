#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QGraphicsItem>
#include <QRectF>
#include <QGraphicsScene>
#include "sprite.h"
#include <pared.h>
#include <QKeyEvent>  // Añadir esta línea

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
    void keyPressEvent(QKeyEvent *event);  // Añadir esta línea

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    QList<pared *> paredes;

    sprite *Yuri;

    float x,y,ancho,alto;
};

#endif // MAINWINDOW_H

