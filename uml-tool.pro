TEMPLATE = app

CONFIG += core gui c++1z

QT += widgets

QMAKE_CXXFLAGS *= -pedantic -Wextra -Wall

SOURCES += \
    main.cpp \
    templates.cpp \
    Entity/field.cpp \
    Entity/Components/componentsignatureparser.cpp \
    Entity/Components/icomponents.cpp \
    Entity/Components/componentsmaker.cpp \
    Generator/abstractprojectgenerator.cpp \
    Generator/basiccppprojectgenerator.cpp \
    Generator/virtualdirectory.cpp \
    Generator/virtualfile.cpp \
    Generator/virtualfilesystemabstractitem.cpp \
    Relationship/association.cpp \
    Relationship/dependency.cpp \
    Relationship/generalization.cpp \
    Relationship/multiplyassociation.cpp \
    Relationship/node.cpp \
    Relationship/realization.cpp \
    Translation/code.cpp \
    Translation/projecttranslator.cpp \
    Translation/signaturemaker.cpp \
    Utility/helpfunctions.cpp \
    GUI/addscope.cpp \
    GUI/chooseglobaldatabasedialog.cpp \
    GUI/componentseditdelegate.cpp \
    Helpers/entityhelpres.cpp \
    Entity/Components/token.cpp \
    Entity/isectional.cpp \
    Relationship/RelationFactory.cpp \
    Entity/EntityFactory.cpp \
    DB/ProjectDatabase.cpp \
    Common/ElementsFactory.cpp \
    Commands/CreateEntity.cpp \
    GUI/graphics/Entity.cpp \
    Models/ProjectTreeModel.cpp \
    Project/Project.cpp \
    GUI/Elements.cpp \
    Entity/Type.cpp \
    Entity/ExtendedType.cpp \
    Entity/Enum.cpp \
    Entity/Union.cpp \
    Entity/Class.cpp \
    Entity/TemplateClass.cpp \
    GUI/View.cpp \
    Commands/AddRelation.cpp \
    Relationship/Relation.cpp \
    DB/Database.cpp \
    GUI/graphics/GraphicsRelation.cpp \
    Common/BasicElement.cpp \
    Common/ID.cpp \
    GUI/graphics/Scene.cpp \
    Application/Application.cpp \
    GUI/MainWindow.cpp \
    Entity/Scope.cpp \
    Entity/GraphicEntityData.cpp \
    Entity/Property.cpp \
    Entity/Template.cpp \
    Entity/ClassMethod.cpp \
    Entity/TemplateClassMethod.cpp \
    Models/ApplicationModel.cpp \
    Models/BasicTreeItem.cpp \
    Commands/RemoveProject.cpp \
    Commands/BaseCommand.cpp \
    Commands/MakeProjectCurrent.cpp \
    Commands/CreateScope.cpp \
    Helpers/GeneratorID.cpp \
    Commands/MoveGraphicObject.cpp \
    Commands/MoveTypeToAnotherScope.cpp \
    Commands/RemoveComponentsCommands.cpp \
    Commands/RenameEntity.cpp \
    GUI/About.cpp \
    Application/Settings.cpp \
    Commands/AddComponentsCommands.cpp \
    Commands/OpenProject.cpp \
    GUI/NewProject.cpp \
    Models/ComponentsModel.cpp \
    Models/MessagesModel.cpp \
    GUI/HtmlDelegate.cpp \
    GUI/Preferences.cpp \
    GUI/graphics/HeaderEditorEventFilter.cpp \
    GUI/EntityProperties.cpp \
    GUI/Section.cpp \
    Models/SectionalTextConverter.cpp \
    GUI/PropertiesHandlerBase.cpp \
    Common/Memento.cpp \
    Commands/CommandFactory.cpp \
    Commands/MementoCmd.cpp \
    Common/IOriginator.cpp \
    Entity/ITextRepresentable.cpp

HEADERS += \
    enums.h \
    types.h \
    DB/Database.h \
    DB/ProjectDatabase.h \
    Entity/field.h \
    Entity/Components/componentsignatureparser.h \
    Entity/Components/icomponents.h \
    Entity/Components/componentsmaker.h \
    Generator/abstractprojectgenerator.h \
    Generator/basiccppprojectgenerator.h \
    Generator/virtualdirectory.h \
    Generator/virtualfile.h \
    Generator/virtualfilesystemabstractitem.h \
    Generator/generator_types.hpp \
    Relationship/association.h \
    Relationship/dependency.h \
    Relationship/generalization.h \
    Relationship/multiplyassociation.h \
    Relationship/node.h \
    Relationship/realization.h \
    Relationship/relationship_types.hpp \
    Translation/code.h \
    Translation/projecttranslator.h \
    Translation/translator_types.hpp \
    Translation/signaturemaker.h \
    Utility/helpfunctions.h \
    GUI/addscope.h \
    GUI/chooseglobaldatabasedialog.h \
    GUI/componentseditdelegate.h \
    Helpers/entityhelpres.h \
    Entity/Components/componentscommon.h \
    Entity/Components/components_types.h \
    Entity/Components/token.h \
    Common/ID.h \
    Entity/itypeuser.h \
    Common/BasicElement.h \
    Entity/isectional.h \
    Common/meta.h \
    Relationship/RelationFactory.h \
    Entity/EntityFactory.h \
    Common/ElementsFactory.h \
    Commands/CreateEntity.h \
    GUI/graphics/Entity.h \
    Models/ProjectTreeModel.h \
    Project/Project.h \
    GUI/Elements.h \
    Entity/Type.h \
    Entity/ExtendedType.h \
    Entity/Enum.h \
    Entity/Union.h \
    Entity/Class.h \
    Entity/TemplateClass.h \
    GUI/View.h \
    Commands/AddRelation.h \
    Relationship/Relation.h \
    GUI/graphics/GraphicsRelation.h \
    GUI/graphics/Scene.h \
    Application/Application.h \
    GUI/MainWindow.h \
    Entity/Scope.h \
    Entity/GraphicEntityData.h \
    Common/SharedFromThis.h \
    Entity/Property.h \
    DB/IScopeSearcher.h \
    DB/ITypeSearcher.h \
    Entity/Template.h \
    Entity/ClassMethod.h \
    Entity/TemplateClassMethod.h \
    Models/ApplicationModel.h \
    Models/BasicTreeItem.h \
    Commands/RemoveProject.h \
    Commands/BaseCommand.h \
    Commands/CommandsTypes.h \
    Commands/MakeProjectCurrent.h \
    Commands/CreateScope.h \
    Helpers/GeneratorID.h \
    GUI/graphics/GraphicsTypes.h \
    Commands/MoveGraphicObject.h \
    Project/ProjectTypes.hpp \
    Commands/MoveTypeToAnotherScope.h \
    Commands/RemoveComponentsCommands.h \
    Commands/RenameEntity.h \
    GUI/graphics/Common.h \
    GUI/About.h \
    Application/Settings.h \
    Constants.h \
    Commands/AddComponentsCommands.h \
    Commands/OpenProject.h \
    QtHelpers.h \
    GUI/NewProject.h \
    Models/ModelsTypes.hpp \
    Models/ComponentsModel.h \
    Models/MessagesModel.h \
    Models/IMessenger.h \
    GUI/HtmlDelegate.h \
    GUI/Preferences.h \
    DB/DBTypes.hpp \
    GUI/graphics/HeaderEditorEventFilter.h \
    GUI/EntityProperties.h \
    GUI/Section.h \
    GUI/IPropertiesHandler.hpp \
    Entity/EntityTypes.hpp \
    GUI/GuiTypes.hpp \
    Models/SectionalTextConverter.hpp \
    Common/CommonTypes.hpp \
    GUI/PropertiesHandlerBase.hpp \
    Common/Memento.hpp \
    Commands/CommandFactory.hpp \
    Common/IOriginator.hpp \
    Commands/MementoCmd.hpp \
    Entity/ITextRepresentable.hpp \
    Entity/Converters/ITextConversionStrategy.hpp

INCLUDEPATH *= $$PWD/boost-di/include $$PWD/range-v3/include

# Suppress warnings
unix: QMAKE_CXXFLAGS *= -isystem $$PWD/range-v3/include

FORMS += \
    GUI/addscope.ui \
    GUI/chooseglobaldatabasedialog.ui \
    GUI/elements.ui \
    GUI/MainWindow.ui \
    GUI/About.ui \
    GUI/NewProject.ui \
    GUI/Preferences.ui \
    GUI/EntityProperties.ui \
    GUI/Section.ui

RESOURCES += \
    GUI/main.qrc

DEFINES *= \
    QT_USE_QSTRINGBUILDER

DISTFILES += \
    BuildParameters.cmake \
    BuildToolset.cmake \
    FilesList.cmake \
    Tests/CMakeLists.txt \
    CMakeLists.txt
