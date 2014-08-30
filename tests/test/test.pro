TEMPLATE = app

CONFIG += console
CONFIG -= app_bundle
CONFIG += qt
CONFIG += c++11

LIBS += -lgtest -lpthread

INCLUDEPATH += ../../

HEADERS += ../../enums.h \
    TestRelationMaker.h \
    TestDepthSearch.h \
    helpers.h \
    TestTypeMaker.h \
    TestCodeGenerator.h \
    TestFileMaker.h

SOURCES += main.cpp \
           ../../type.cpp \
           ../../class.cpp \
           ../../enum.cpp \
           ../../field.cpp \
           ../../union.cpp \
           ../../classmethod.cpp \
           ../../templateclass.cpp \
           ../../extendedtype.cpp\
           ../../template.cpp \
           ../../helpfunctions.cpp \
           ../../database.cpp \
           ../../projectdatabase.cpp \
           ../../relation.cpp \
           ../../node.cpp \
           ../../generalization.cpp \
           ../../dependency.cpp \
           ../../association.cpp \
           ../../realization.cpp \
           ../../multiplyassociation.cpp \
           ../../templateclassmethod.cpp \
           ../../projecttranslator.cpp \
           ../../constants.cpp \
           ../../templates.cpp \
           ../../scope.cpp \
           ../../virtualfile.cpp \
           ../../virtualdirectory.cpp \
           ../../virtualfilesystemabstractitem.cpp
