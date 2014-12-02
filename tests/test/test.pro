TEMPLATE = app

CONFIG += console qt c++14
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
    TestProjectMaker.h \
    TestProject.h

SOURCES += main.cpp \
           ../../entity/type.cpp \
           ../../entity/class.cpp \
           ../../entity/enum.cpp \
           ../../entity/field.cpp \
           ../../entity/union.cpp \
           ../../entity/classmethod.cpp \
           ../../entity/templateclass.cpp \
           ../../entity/extendedtype.cpp\
           ../../entity/template.cpp \
           ../../utility/helpfunctions.cpp \
           ../../db/database.cpp \
           ../../db/projectdatabase.cpp \
           ../../relationship/relation.cpp \
           ../../relationship/node.cpp \
           ../../relationship/generalization.cpp \
           ../../relationship/dependency.cpp \
           ../../relationship/association.cpp \
           ../../relationship/realization.cpp \
           ../../relationship/multiplyassociation.cpp \
           ../../entity/templateclassmethod.cpp \
           ../../translator/projecttranslator.cpp \
           ../../constants.cpp \
           ../../templates.cpp \
           ../../entity/scope.cpp \
           ../../generator/virtualfile.cpp \
           ../../generator/virtualdirectory.cpp \
           ../../generator/virtualfilesystemabstractitem.cpp \
           ../../generator/abstractprojectgenerator.cpp \
           ../../generator/basiccppprojectgenerator.cpp \
           ../../project/project.cpp \
           ../../translator/code.cpp \
