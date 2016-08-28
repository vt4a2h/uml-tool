TEMPLATE = app

CONFIG += console core gui thread c++14
CONFIG -= app_bundle

QT += widgets

LIBS += -lgtest -lpthread

linux-g++ {
    message(********** Building with gcc 6 **********)
    QMAKE_CXX = g++-6
    QMAKE_CXXFLAGS *= -std=gnu++1y

    equals(WITH_COV, "TRUE") {
        message(********** Collecting test coverage **********)
        QMAKE_CXX = gcc-6
        QMAKE_LINK = gcc-6
        QMAKE_LFLAGS *= -lstdc++ --coverage
        QMAKE_CXXFLAGS *= -std=gnu++1y -g --coverage
    }
}

INCLUDEPATH += ../../

HEADERS += ../../enums.h \
    ../../application/settings.h \
    ../../project/project.h \
    ../../entity/property.h \
    ../../entity/class.h \
    ../../entity/scope.h \
    ../../entity/isectional.h \
    ../../entity/components/icomponents.h \
    ../../entity/GraphicEntityData.h \
    ../../entity/EntityFactory.h \
    ../../entity/Template.h \
    ../../constants.h \
    ../../entity/field.h \
    ../../qthelpers.h \
    ../../models/applicationmodel.h \
    ../../models/projecttreemodel.h \
    ../../models/basictreeitem.h \
    ../../helpers/entityhelpres.h\
    ../../helpers/generatorid.h\
    ../../gui/graphics/GraphicsRelation.h \
    ../../gui/graphics/Entity.h \
    ../../relationship/Relation.h \
    ../../relationship/RelationFactory.h \
    ../../common/ElementsFactory.h \
    ../../commands/movegraphicobject.h \
    ../../commands/basecommand.h \
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
    TestComponentSignatureParser.h \
    TestProjectBase.h \
    ../../db/ProjectDatabase.h \
    ../../db/Database.h \
    ../../common/BasicElement.h \
    ../../common/ID.h

SOURCES += main.cpp \
           ../../application/settings.cpp \
           ../../common/basicelement.cpp \
           ../../entity/type.cpp \
           ../../entity/components/icomponents.cpp \
           ../../entity/components/token.cpp \
           ../../entity/class.cpp \
           ../../entity/enum.cpp \
           ../../entity/field.cpp \
           ../../entity/union.cpp \
           ../../entity/classmethod.cpp \
           ../../entity/TemplateClass.cpp \
           ../../entity/extendedtype.cpp\
           ../../entity/Template.cpp \
           ../../entity/property.cpp \
           ../../entity/GraphicEntityData.cpp \
           ../../common/id.cpp \
           ../../entity/isectional.cpp \
           ../../utility/helpfunctions.cpp \
           ../../db/database.cpp \
           ../../db/ProjectDatabase.cpp \
           ../../relationship/Relation.cpp \
           ../../relationship/node.cpp \
           ../../relationship/generalization.cpp \
           ../../relationship/dependency.cpp \
           ../../relationship/association.cpp \
           ../../relationship/realization.cpp \
           ../../relationship/multiplyassociation.cpp \
           ../../relationship/RelationFactory.cpp \
           ../../gui/graphics/GraphicsRelation.cpp \
           ../../gui/graphics/Entity.cpp \
           ../../entity/templateclassmethod.cpp \
           ../../entity/scope.cpp \
           ../../entity/EntityFactory.cpp \
           ../../translation/projecttranslator.cpp \
           ../../templates.cpp \
           ../../generator/virtualfile.cpp \
           ../../generator/virtualdirectory.cpp \
           ../../generator/virtualfilesystemabstractitem.cpp \
           ../../generator/abstractprojectgenerator.cpp \
           ../../generator/basiccppprojectgenerator.cpp \
           ../../project/project.cpp \
           ../../translation/code.cpp \
           ../../entity/components/componentsmaker.cpp \
           ../../entity/components/componentsignatureparser.cpp \
           ../../translation/signaturemaker.cpp \
           ../../models/applicationmodel.cpp \
           ../../models/projecttreemodel.cpp \
           ../../models/basictreeitem.cpp \
           ../../helpers/entityhelpres.cpp \
           ../../helpers/generatorid.cpp \
           ../../common/ElementsFactory.cpp \
           ../../commands/movegraphicobject.cpp \
           ../../commands/basecommand.cpp
