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

INCLUDEPATH *= $$PWD/../ $$PWD/../boost-di/include $$PWD/../range-v3/include

# Suppress warnings
unix: QMAKE_CXXFLAGS *= -isystem $$PWD/../range-v3/include

HEADERS += \
    $$PWD/../enums.h \
    $$PWD/../types.h \
    $$PWD/../Application/settings.h \
    $$PWD/../Project/project.h \
    $$PWD/../Entity/property.h \
    $$PWD/../Entity/class.h \
    $$PWD/../Entity/scope.h \
    $$PWD/../Entity/isectional.h \
    $$PWD/../Entity/Components/icomponents.h \
    $$PWD/../Entity/GraphicEntityData.h \
    $$PWD/../Entity/EntityFactory.h \
    $$PWD/../Entity/Template.h \
    $$PWD/../Constants.h \
    $$PWD/../Entity/field.h \
    $$PWD/../QtHelpers.h \
    $$PWD/../Models/ApplicationModel.h \
    $$PWD/../Models/projecttreemodel.h \
    $$PWD/../Models/basictreeitem.h \
    $$PWD/../Models/SectionalTextConverter.hpp \
    $$PWD/../Helpers/entityhelpres.h\
    $$PWD/../Helpers/GeneratorID.h\
    $$PWD/../GUI/graphics/GraphicsRelation.h \
    $$PWD/../GUI/graphics/Entity.h \
    $$PWD/../GUI/graphics/Scene.h \
    $$PWD/../GUI/graphics/HeaderEditorEventFilter.h \
    $$PWD/../Relationship/Relation.h \
    $$PWD/../Relationship/RelationFactory.h \
    $$PWD/../Common/ElementsFactory.h \
    $$PWD/../Common/IOriginator.hpp \
    $$PWD/../Commands/basecommand.h \
    $$PWD/../Commands/CreateEntity.h \
    $$PWD/../Commands/CreateScope.h \
    $$PWD/../Commands/MakeProjectCurrent.h \
    $$PWD/../Commands/MoveGraphicObject.h \
    $$PWD/../Commands/AddRelation.h \
    $$PWD/../Commands/MoveTypeToAnotherScope.h \
    $$PWD/../Commands/RemoveProject.h \
    $$PWD/../Commands/RenameEntity.h \
    $$PWD/../Commands/OpenProject.h \
    $$PWD/../Commands/CommandFactory.hpp \
    $$PWD/../Entity/ITextRepresentable.hpp \
    $$PWD/../Entity/Converters/ITextConversionStrategy.hpp \
    $$PWD/../Entity/Converters/BaseTextConversionStrategy.hpp \
    $$PWD/../Entity/Converters/ConvertersTypes.hpp \
    $$PWD/../Entity/Converters/EnumTextConversionStrategy.hpp \
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
    $$PWD/../DB/ProjectDatabase.h \
    $$PWD/../DB/Database.h \
    $$PWD/../Common/BasicElement.h \
    $$PWD/../Common/ID.h \
    $$PWD/../Common/Memento.hpp \
    $$PWD/../DB/DBTypes.hpp \
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
    cases/Memento.h \
    TestSectionalTextConvertion.hpp \
    cases/SectionalTextConvertionCases.h

SOURCES += main.cpp \
           $$PWD/../Application/settings.cpp \
           $$PWD/../Common/basicelement.cpp \
           $$PWD/../Entity/type.cpp \
           $$PWD/../Entity/Components/icomponents.cpp \
           $$PWD/../Entity/Components/token.cpp \
           $$PWD/../Entity/class.cpp \
           $$PWD/../Entity/enum.cpp \
           $$PWD/../Entity/field.cpp \
           $$PWD/../Entity/union.cpp \
           $$PWD/../Entity/classmethod.cpp \
           $$PWD/../Entity/TemplateClass.cpp \
           $$PWD/../Entity/extendedtype.cpp\
           $$PWD/../Entity/Template.cpp \
           $$PWD/../Entity/property.cpp \
           $$PWD/../Entity/GraphicEntityData.cpp \
           $$PWD/../Entity/ITextRepresentable.cpp \
           $$PWD/../Common/IOriginator.cpp \
           $$PWD/../Common/id.cpp \
           $$PWD/../Common/Memento.cpp \
           $$PWD/../Entity/isectional.cpp \
           $$PWD/../Utility/helpfunctions.cpp \
           $$PWD/../DB/database.cpp \
           $$PWD/../DB/ProjectDatabase.cpp \
           $$PWD/../Relationship/Relation.cpp \
           $$PWD/../Relationship/node.cpp \
           $$PWD/../Relationship/generalization.cpp \
           $$PWD/../Relationship/dependency.cpp \
           $$PWD/../Relationship/association.cpp \
           $$PWD/../Relationship/realization.cpp \
           $$PWD/../Relationship/multiplyassociation.cpp \
           $$PWD/../Relationship/RelationFactory.cpp \
           $$PWD/../GUI/graphics/GraphicsRelation.cpp \
           $$PWD/../GUI/graphics/Entity.cpp \
           $$PWD/../GUI/graphics/Scene.cpp \
           $$PWD/../GUI/graphics/HeaderEditorEventFilter.cpp \
           $$PWD/../Entity/templateclassmethod.cpp \
           $$PWD/../Entity/scope.cpp \
           $$PWD/../Entity/EntityFactory.cpp \
           $$PWD/../Translation/projecttranslator.cpp \
           $$PWD/../templates.cpp \
           $$PWD/../Generator/virtualfile.cpp \
           $$PWD/../Generator/virtualdirectory.cpp \
           $$PWD/../Generator/virtualfilesystemabstractitem.cpp \
           $$PWD/../Generator/abstractprojectgenerator.cpp \
           $$PWD/../Generator/basiccppprojectgenerator.cpp \
           $$PWD/../Project/project.cpp \
           $$PWD/../Translation/code.cpp \
           $$PWD/../Entity/Components/componentsmaker.cpp \
           $$PWD/../Entity/Components/componentsignatureparser.cpp \
           $$PWD/../Translation/signaturemaker.cpp \
           $$PWD/../Models/ApplicationModel.cpp \
           $$PWD/../Models/projecttreemodel.cpp \
           $$PWD/../Models/basictreeitem.cpp \
           $$PWD/../Models/SectionalTextConverter.cpp \
           $$PWD/../Helpers/entityhelpres.cpp \
           $$PWD/../Helpers/GeneratorID.cpp \
           $$PWD/../Common/ElementsFactory.cpp \
           $$PWD/../Commands/basecommand.cpp \
           $$PWD/../Commands/CreateEntity.cpp \
           $$PWD/../Commands/CreateScope.cpp \
           $$PWD/../Commands/MakeProjectCurrent.cpp \
           $$PWD/../Commands/MoveGraphicObject.cpp \
           $$PWD/../Commands/AddRelation.cpp \
           $$PWD/../Commands/MoveTypeToAnotherScope.cpp \
           $$PWD/../Commands/RemoveProject.cpp \
           $$PWD/../Commands/RenameEntity.cpp \
           $$PWD/../Commands/OpenProject.cpp \
           $$PWD/../Commands/CommandFactory.cpp \
           $$PWD/../Entity/Converters/BaseTextConversionStrategy.cpp \
           $$PWD/../Entity/Converters/EnumTextConversionStrategy.cpp \
           Arguments.cpp
