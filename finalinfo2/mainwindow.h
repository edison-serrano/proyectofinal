#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QGraphicsItem>
#include <QRectF>
#include <QGraphicsScene>
#include "sprite.h"
#include <pared.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    QList<pared *> paredes;

    sprite *Yuri;

    float x,y,ancho,alto;
};

#endif // MAINWINDOW_H

