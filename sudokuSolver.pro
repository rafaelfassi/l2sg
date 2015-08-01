#-------------------------------------------------
#
# Project created by QtCreator 2013-07-11T08:36:03
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = sudokuSolver
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    cell.cpp \
    grid.cpp \
    solverbruteforce.cpp \
    visualgrid.cpp \
    solverlogic.cpp \
    combination.cpp

HEADERS += \
    cell.h \
    grid.h \
    solverbruteforce.h \
    visualgrid.h \
    solverlogic.h \
    combination.h
