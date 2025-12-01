#-------------------------------------------------
#
# Project created by QtCreator 2018-10-26T21:45:23
#
#-------------------------------------------------

QT       += core gui sql charts
QT += core gui widgets sql printsupport charts

# Pas besoin de "using namespace QtCharts" en Qt 6

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
include(C:\Users\MSI\Desktop\projetc++\Qt-QrCodeGenerator-main\Qt-QrCodeGenerator.pri)
TARGET = Atelier_Connexion
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++17

SOURCES += \
        main.cpp \
    connection.cpp \
    smartstudio.cpp \
    materielle.cpp \


HEADERS += \
    connection.h \
    smartstudio.h \
    materielle.h \

# Fichiers d'interface utilisateur
FORMS += \
    smartstudio.ui

# Options de compilation pour Windows
win32: LIBS += -luser32

# Options de compilation pour Linux
unix:!macx: LIBS += -lX11

# Options de compilation pour Mac OS X
macx: LIBS += -framework ApplicationServices

# Options de compilation pour le débogage
debug {
    CONFIG += debug
    CONFIG += console
    DEFINES += QT_DEBUG
}

# Options de compilation pour la version release
release {
    CONFIG += release
    CONFIG -= debug
    DEFINES += QT_NO_DEBUG_OUTPUT
}

# Désactive les avertissements pour les conversions implicites
QMAKE_CXXFLAGS += -Wno-conversion

# Désactive les avertissements pour les variables non utilisées
QMAKE_CXXFLAGS += -Wno-unused-parameter

# Active les optimisations de vitesse
QMAKE_CXXFLAGS_RELEASE += -O2

# Active les optimisations pour la taille du code
# QMAKE_CXXFLAGS_RELEASE += -Os

# Active les optimisations spécifiques au processeur
# QMAKE_CXXFLAGS_RELEASE += -march=native

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
INCLUDEPATH += Qt-QrCodeGenerator-main/src
