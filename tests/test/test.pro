TEMPLATE = app

CONFIG += console qt
CONFIG -= app_bundle

QT += widgets

LIBS += -lgtest -lpthread

QMAKE_CXX = gcc-5
QMAKE_LINK = gcc-5
QMAKE_LFLAGS *= -lstdc++
QMAKE_CXXFLAGS *= -std=c++1y

QMAKE_CXXFLAGS_DEBUG += --coverage
QMAKE_LFLAGS_DEBUG += --coverage

INCLUDEPATH += ../../

HEADERS += ../../enums.h \
    ../../project/project.h \
    ../../entity/basicentity.h \
    ../../constants.h \
    ../../models/applicationmodel.h \
    ../../models/projecttreemodel.h \
    ../../models/basictreeitem.h \
    ../../helpers/entityhelpres.h\
    TestRelationMaker.h \
    TestDepthSearch.h \
    helpers.h \
    TestTypeMaker.h \
    TestFileMaker.h \
    TestJson.h \
    TestProjectTranslator.h \
    TestProjectMaker.h \
    TestProject.h \
    TestSignatureMaker.hpp \
    TestIComponents.h \
    TestEntities.h \
    TestComponentsMaker.h \
    TestComponentsBase.h \
    TestClassComponents.h \
    cases/projecttranslatortestcases.h \
    cases/relationmakertestcases.h \
    cases/depthsearchtestcases.h \
    cases/filemakertestcases.h \
    cases/filejsontestcases.h \
    cases/projectmakertestcases.h \
    cases/projecttestcases.h \
    cases/typemakertestcases.h \
    cases/icomponentstestcases.h \
    cases/entitiestestcases.h \
    cases/componentsmakercases.h \
    cases/signaturemakercases.h \
    cases/classcomponentscases.h \
    cases/signatureparsercases.h \
    TestComponentSignatureParser.h

SOURCES += main.cpp \
           ../../entity/type.cpp \
           ../../entity/components/icomponents.cpp \
           ../../entity/components/token.cpp \
           ../../entity/basicentity.cpp \
           ../../entity/class.cpp \
           ../../entity/enum.cpp \
           ../../entity/field.cpp \
           ../../entity/union.cpp \
           ../../entity/classmethod.cpp \
           ../../entity/templateclass.cpp \
           ../../entity/extendedtype.cpp\
           ../../entity/template.cpp \
           ../../entity/property.cpp \
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
           ../../translation/projecttranslator.cpp \
           ../../templates.cpp \
           ../../entity/scope.cpp \
           ../../generator/virtualfile.cpp \
           ../../generator/virtualdirectory.cpp \
           ../../generator/virtualfilesystemabstractitem.cpp \
           ../../generator/abstractprojectgenerator.cpp \
           ../../generator/basiccppprojectgenerator.cpp \
           ../../project/project.cpp \
           ../../translation/code.cpp \
           ../../entity/components/componentsmaker.cpp \
           ../../entity/components/componentsignatureparser.cpp \
           ../../gui/signaturemaker.cpp \
           ../../models/applicationmodel.cpp \
           ../../models/projecttreemodel.cpp \
           ../../models/basictreeitem.cpp \
           ../../helpers/entityhelpres.cpp
