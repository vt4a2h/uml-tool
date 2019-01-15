TEMPLATE = app

CONFIG += core gui c++1z

QT += widgets

QMAKE_CXXFLAGS *= -pedantic -Wextra -Wall

SOURCES += \
    Application/Application.cpp \
    Application/Settings.cpp \
    Commands/AddComponentsCommands.cpp \
    Commands/AddRelation.cpp \
    Commands/BaseCommand.cpp \
    Commands/CommandFactory.cpp \
    Commands/CreateEntity.cpp \
    Commands/CreateScope.cpp \
    Commands/MakeProjectCurrent.cpp \
    Commands/MementoCmd.cpp \
    Commands/MoveGraphicObject.cpp \
    Commands/OpenProject.cpp \
    Commands/RemoveComponentsCommands.cpp \
    Commands/RemoveProject.cpp \
    Commands/RenameEntity.cpp \
    Common/BasicElement.cpp \
    Common/ElementsFactory.cpp \
    Common/ID.cpp \
    Common/IOriginator.cpp \
    Common/Memento.cpp \
    DB/Database.cpp \
    DB/ProjectDatabase.cpp \
    Entity/Class.cpp \
    Entity/ClassMethod.cpp \
    Entity/Components/componentsignatureparser.cpp \
    Entity/Components/icomponents.cpp \
    Entity/Components/token.cpp \
    Entity/Converters/BaseTextConversionStrategy.cpp \
    Entity/Converters/EnumTextConversionStrategy.cpp \
    Entity/EntityFactory.cpp \
    Entity/Enum.cpp \
    Entity/ExtendedType.cpp \
    Entity/GraphicEntityData.cpp \
    Entity/ITextRepresentable.cpp \
    Entity/Property.cpp \
    Entity/Scope.cpp \
    Entity/Template.cpp \
    Entity/TemplateClass.cpp \
    Entity/TemplateClassMethod.cpp \
    Entity/Type.cpp \
    Entity/Union.cpp \
    Entity/field.cpp \
    Entity/isectional.cpp \
    GUI/About.cpp \
    GUI/Elements.cpp \
    GUI/EntityProperties.cpp \
    GUI/HtmlDelegate.cpp \
    GUI/MainWindow.cpp \
    GUI/NewProject.cpp \
    GUI/Preferences.cpp \
    GUI/PropertiesHandlerBase.cpp \
    GUI/Section.cpp \
    GUI/View.cpp \
    GUI/addscope.cpp \
    GUI/chooseglobaldatabasedialog.cpp \
    GUI/componentseditdelegate.cpp \
    GUI/graphics/Entity.cpp \
    GUI/graphics/GraphicsRelation.cpp \
    GUI/graphics/HeaderEditorEventFilter.cpp \
    GUI/graphics/Scene.cpp \
    Generator/abstractprojectgenerator.cpp \
    Generator/basiccppprojectgenerator.cpp \
    Generator/virtualdirectory.cpp \
    Generator/virtualfile.cpp \
    Generator/virtualfilesystemabstractitem.cpp \
    Helpers/GeneratorID.cpp \
    Helpers/entityhelpres.cpp \
    Models/ApplicationModel.cpp \
    Models/BasicTreeItem.cpp \
    Models/ComponentsModel.cpp \
    Models/MessagesModel.cpp \
    Models/ProjectTreeModel.cpp \
    Project/Project.cpp \
    Project/ProjectFactory.cpp \
    Relationship/Relation.cpp \
    Relationship/RelationFactory.cpp \
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
    main.cpp \
    templates.cpp \
    Project/ProjectDB.cpp

HEADERS += \
    Application/Application.h \
    Application/Settings.h \
    Commands/AddComponentsCommands.h \
    Commands/AddRelation.h \
    Commands/BaseCommand.h \
    Commands/CommandFactory.hpp \
    Commands/CommandsTypes.h \
    Commands/CreateEntity.h \
    Commands/CreateScope.h \
    Commands/MakeProjectCurrent.h \
    Commands/MementoCmd.hpp \
    Commands/MoveGraphicObject.h \
    Commands/OpenProject.h \
    Commands/RemoveComponentsCommands.h \
    Commands/RemoveProject.h \
    Commands/RenameEntity.h \
    Common/BasicElement.h \
    Common/CommonTypes.hpp \
    Common/ElementsFactory.h \
    Common/ID.h \
    Common/IOriginator.hpp \
    Common/Memento.hpp \
    Common/SharedFromThis.h \
    Common/meta.h \
    Constants.h \
    DB/DBTypes.hpp \
    DB/Database.h \
    DB/IScopeSearcher.h \
    DB/ITypeSearcher.h \
    DB/ProjectDatabase.h \
    Entity/Class.h \
    Entity/ClassMethod.h \
    Entity/Components/components_types.h \
    Entity/Components/componentscommon.h \
    Entity/Components/componentsignatureparser.h \
    Entity/Components/icomponents.h \
    Entity/Components/token.h \
    Entity/Converters/BaseTextConversionStrategy.hpp \
    Entity/Converters/ConvertersTypes.hpp \
    Entity/Converters/EnumTextConversionStrategy.hpp \
    Entity/Converters/ITextConversionStrategy.hpp \
    Entity/EntityFactory.h \
    Entity/EntityTypes.hpp \
    Entity/Enum.h \
    Entity/ExtendedType.h \
    Entity/GraphicEntityData.h \
    Entity/ITextRepresentable.hpp \
    Entity/Property.h \
    Entity/Scope.h \
    Entity/Template.h \
    Entity/TemplateClass.h \
    Entity/TemplateClassMethod.h \
    Entity/Type.h \
    Entity/Union.h \
    Entity/field.h \
    Entity/isectional.h \
    Entity/itypeuser.h \
    GUI/About.h \
    GUI/Elements.h \
    GUI/EntityProperties.h \
    GUI/GuiTypes.hpp \
    GUI/HtmlDelegate.h \
    GUI/IPropertiesHandler.hpp \
    GUI/MainWindow.h \
    GUI/NewProject.h \
    GUI/Preferences.h \
    GUI/PropertiesHandlerBase.hpp \
    GUI/Section.h \
    GUI/View.h \
    GUI/addscope.h \
    GUI/chooseglobaldatabasedialog.h \
    GUI/componentseditdelegate.h \
    GUI/graphics/Common.h \
    GUI/graphics/Entity.h \
    GUI/graphics/GraphicsRelation.h \
    GUI/graphics/GraphicsTypes.h \
    GUI/graphics/HeaderEditorEventFilter.h \
    GUI/graphics/Scene.h \
    Generator/abstractprojectgenerator.h \
    Generator/basiccppprojectgenerator.h \
    Generator/generator_types.hpp \
    Generator/virtualdirectory.h \
    Generator/virtualfile.h \
    Generator/virtualfilesystemabstractitem.h \
    Helpers/GeneratorID.h \
    Helpers/entityhelpres.h \
    Models/ApplicationModel.h \
    Models/BasicTreeItem.h \
    Models/ComponentsModel.h \
    Models/IMessenger.h \
    Models/MessagesModel.h \
    Models/ModelsTypes.hpp \
    Models/ProjectTreeModel.h \
    Project/Project.h \
    Project/ProjectTypes.hpp \
    Project/ProjectFactory.hpp \
    QtHelpers.h \
    Relationship/Relation.h \
    Relationship/RelationFactory.h \
    Relationship/association.h \
    Relationship/dependency.h \
    Relationship/generalization.h \
    Relationship/multiplyassociation.h \
    Relationship/node.h \
    Relationship/realization.h \
    Relationship/relationship_types.hpp \
    Translation/code.h \
    Translation/projecttranslator.h \
    Translation/signaturemaker.h \
    Translation/translator_types.hpp \
    Utility/helpfunctions.h \
    enums.h \
    types.h \
    Project/ProjectDB.hpp

INCLUDEPATH *= $$PWD/boost-di/include $$PWD/range-v3/include

# Suppress warnings
unix: QMAKE_CXXFLAGS *= -isystem $$PWD/range-v3/include

FORMS += \
    GUI/About.ui \
    GUI/EntityProperties.ui \
    GUI/MainWindow.ui \
    GUI/NewProject.ui \
    GUI/Preferences.ui \
    GUI/Section.ui \
    GUI/addscope.ui \
    GUI/chooseglobaldatabasedialog.ui \
    GUI/elements.ui

RESOURCES += \
    GUI/main.qrc

DEFINES *= \
    QT_USE_QSTRINGBUILDER

DISTFILES += \
    BuildParameters.cmake \
    BuildToolset.cmake \
    CMakeLists.txt \
    FilesList.cmake \
    Tests/TestFiles.cmake

# TODO: generate qt.conf wit correct path to plugins:
# [Paths]
# Plugins = path-to-plugins
# $$[QT_INSTALL_PLUGINS]
