TEMPLATE = app

CONFIG += console
CONFIG -= app_bundle
CONFIG += c++11

INCLUDEPATH += ../../

SOURCES += main.cpp \
           ../../type.cpp \
           ../../class.cpp \
           ../../enum.cpp \
           ../../field.cpp \
           ../../union.cpp \
           ../../classmethod.cpp \
           ../../templateclass.cpp \
           ../../template.cpp \
           ../../helpfunctions.cpp \
           ../../database.cpp \
           ../../scope.cpp

LIBS += -lgtest -lpthread

HEADERS += \
    TestDepthSearch.h
