TEMPLATE = app

CONFIG += console qt c++11
CONFIG -= app_bundle

LIBS += -lgtest -lpthread

INCLUDEPATH += ../../

HEADERS += ../../enums.h \
    TestRelationMaker.h \
    TestDepthSearch.h \
    helpers.h \
    TestTypeMaker.h \
    TestFileMaker.h \
    TestJson.h \
    TestProjectTranslator.h \
    TestProjectMaker.h

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
           ../../virtualfilesystemabstractitem.cpp \
           ../../abstractprojectgenerator.cpp \
           ../../basiccppprojectgenerator.cpp \
           ../../code.cpp \
