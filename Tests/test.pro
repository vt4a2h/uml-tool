TEMPLATE = app

CONFIG += console core gui thread c++1z
CONFIG -= app_bundle

QMAKE_CXXFLAGS *= -pedantic -Wextra -Wall

QT += widgets

LIBS += -lgtest -lpthread

# TODO: set in a build script!
#linux-g++ {
#    equals(WITH_COV, "TRUE") {
#        message(********** Collecting test coverage **********)
#        QMAKE_CXX = gcc-6
#        QMAKE_LINK = gcc-6
#        QMAKE_LFLAGS *= -lstdc++ --coverage
#        QMAKE_CXXFLAGS *= -std=gnu++1y -g --coverage
#    }
#}

INCLUDEPATH += $$PWD/../

HEADERS += \
    ../enums.h \
    ../types.h \
    ../Application/settings.h \
    ../Project/project.h \
    ../Entity/property.h \
    ../Entity/class.h \
    ../Entity/scope.h \
    ../Entity/isectional.h \
    ../Entity/Components/icomponents.h \
    ../Entity/GraphicEntityData.h \
    ../Entity/EntityFactory.h \
    ../Entity/Template.h \
    ../Constants.h \
    ../Entity/field.h \
    ../QtHelpers.h \
    ../Models/ApplicationModel.h \
    ../Models/projecttreemodel.h \
    ../Models/basictreeitem.h \
    ../Models/SectionalTextConverter.hpp \
    ../Helpers/entityhelpres.h\
    ../Helpers/GeneratorID.h\
    ../GUI/graphics/GraphicsRelation.h \
    ../GUI/graphics/Entity.h \
    ../GUI/graphics/Scene.h \
    ../GUI/graphics/HeaderEditorEventFilter.h \
    ../Relationship/Relation.h \
    ../Relationship/RelationFactory.h \
    ../Common/ElementsFactory.h \
    ../Commands/basecommand.h \
    ../Commands/CreateEntity.h \
    ../Commands/CreateScope.h \
    ../Commands/MakeProjectCurrent.h \
    ../Commands/MoveGraphicObject.h \
    ../Commands/AddRelation.h \
    ../Commands/MoveTypeToAnotherScope.h \
    ../Commands/RemoveProject.h \
    ../Commands/RenameEntity.h \
    ../Commands/OpenProject.h \
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
    TestComponentSignatureParser.h \
    TestProjectBase.h \
    ../DB/ProjectDatabase.h \
    ../DB/Database.h \
    ../Common/BasicElement.h \
    ../Common/ID.h \
    ../DB/DBTypes.hpp \
    cases/TypeMakerTestCases.h \
    cases/SignatureParserCases.h \
    cases/SignatureMakerCases.h \
    cases/RelationMakerCases.h \
    cases/ProjectTranslatorCases.h \
    cases/ProjectCases.h \
    cases/IComponentsCases.h \
    cases/FileJsonCases.h \
    cases/EntitiesCases.h \
    cases/DepthSearchCases.h \
    cases/ComponentsMakerCases.h \
    cases/ClassComponentsCases.h \
    cases/FileMakerCases.h \
    TestCommands.h \
    cases/CommandsCases.h \
    cases/ProjectMakerCases.h \
    cases/ID.h \
    cases/HelpersCases.h \
    Arguments.hpp \
    TestSectionalTextConverter.hpp \
    cases/SectionalTextConverterCases.h

SOURCES += main.cpp \
           ../Application/settings.cpp \
           ../Common/basicelement.cpp \
           ../Entity/type.cpp \
           ../Entity/Components/icomponents.cpp \
           ../Entity/Components/token.cpp \
           ../Entity/class.cpp \
           ../Entity/enum.cpp \
           ../Entity/field.cpp \
           ../Entity/union.cpp \
           ../Entity/classmethod.cpp \
           ../Entity/TemplateClass.cpp \
           ../Entity/extendedtype.cpp\
           ../Entity/Template.cpp \
           ../Entity/property.cpp \
           ../Entity/GraphicEntityData.cpp \
           ../Common/id.cpp \
           ../Entity/isectional.cpp \
           ../Utility/helpfunctions.cpp \
           ../DB/database.cpp \
           ../DB/ProjectDatabase.cpp \
           ../Relationship/Relation.cpp \
           ../Relationship/node.cpp \
           ../Relationship/generalization.cpp \
           ../Relationship/dependency.cpp \
           ../Relationship/association.cpp \
           ../Relationship/realization.cpp \
           ../Relationship/multiplyassociation.cpp \
           ../Relationship/RelationFactory.cpp \
           ../GUI/graphics/GraphicsRelation.cpp \
           ../GUI/graphics/Entity.cpp \
           ../GUI/graphics/Scene.cpp \
           ../GUI/graphics/HeaderEditorEventFilter.cpp \
           ../Entity/templateclassmethod.cpp \
           ../Entity/scope.cpp \
           ../Entity/EntityFactory.cpp \
           ../Translation/projecttranslator.cpp \
           ../templates.cpp \
           ../Generator/virtualfile.cpp \
           ../Generator/virtualdirectory.cpp \
           ../Generator/virtualfilesystemabstractitem.cpp \
           ../Generator/abstractprojectgenerator.cpp \
           ../Generator/basiccppprojectgenerator.cpp \
           ../Project/project.cpp \
           ../Translation/code.cpp \
           ../Entity/Components/componentsmaker.cpp \
           ../Entity/Components/componentsignatureparser.cpp \
           ../Translation/signaturemaker.cpp \
           ../Models/ApplicationModel.cpp \
           ../Models/projecttreemodel.cpp \
           ../Models/basictreeitem.cpp \
           ../Models/SectionalTextConverter.cpp \
           ../Helpers/entityhelpres.cpp \
           ../Helpers/GeneratorID.cpp \
           ../Common/ElementsFactory.cpp \
           ../Commands/basecommand.cpp \
           ../Commands/CreateEntity.cpp \
           ../Commands/CreateScope.cpp \
           ../Commands/MakeProjectCurrent.cpp \
           ../Commands/MoveGraphicObject.cpp \
           ../Commands/AddRelation.cpp \
           ../Commands/MoveTypeToAnotherScope.cpp \
           ../Commands/RemoveProject.cpp \
           ../Commands/RenameEntity.cpp \
           ../Commands/OpenProject.cpp \
    Arguments.cpp
