#ifndef MENU_H
#define MENU_H

#include <QObject>
#include <QMenu>
#include <QGraphicsScene>
#include <QGraphicsProxyWidget>


class Menu : public QObject
{
    Q_OBJECT
public:
    explicit Menu(QObject *parent = nullptr);
    QMenu *menu;
    QGraphicsScene *menuScene;


signals:
    void iniciarJuegoSignal();

public slots:
    void iniciarJuego();
    void mostrarPuntajeMaximo();
    void salir();


};

#endif // MENU_H
