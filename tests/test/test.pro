TEMPLATE = app

CONFIG += console
CONFIG -= app_bundle
CONFIG += c++11

SOURCES +=  main.cpp

INCLUDEPATH += ../../

LIBS += -lgtest -lpthread
