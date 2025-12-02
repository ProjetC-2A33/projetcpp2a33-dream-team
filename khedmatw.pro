QT += core gui sql printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

TEMPLATE = app
TARGET = khedmatw

SOURCES += \
	main.cpp \
	connection.cpp \
	projet.cpp \
	mainwindow.cpp

HEADERS += \
	connection.h \
	projet.h \
	mainwindow.h

FORMS += \
        projet.ui

DEFINES += QT_DEPRECATED_WARNINGS


