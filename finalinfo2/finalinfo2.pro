QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    enemigo.cpp \
    inventario.cpp \
    main.cpp \
    mainwindow.cpp \
    menu.cpp \
    npc1.cpp \
    pared.cpp \
    pasarnivel.cpp \
    pendulo.cpp \
    plataforma.cpp \
    puerta.cpp \
    radiacion.cpp \
    reactor.cpp \
    sprite.cpp \
    sprite2.cpp \
    vidaextra.cpp

HEADERS += \
    enemigo.h \
    inventario.h \
    mainwindow.h \
    menu.h \
    npc1.h \
    pared.h \
    pasarnivel.h \
    pendulo.h \
    plataforma.h \
    puerta.h \
    radiacion.h \
    reactor.h \
    sprite.h \
    sprite2.h \
    vidaextra.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES +=

RESOURCES += \
    pictures.qrc
