TEMPLATE = app

CONFIG += core gui c++14

QT += widgets

linux-g++ {
    message(********** Building with gcc 6 **********)
    QMAKE_CXX = g++-6
    QMAKE_CXXFLAGS *= -std=gnu++1y
}

SOURCES += \
    main.cpp \
    templates.cpp \
    entity/field.cpp \
    entity/components/componentsignatureparser.cpp \
    entity/components/icomponents.cpp \
    entity/components/componentsmaker.cpp \
    generator/abstractprojectgenerator.cpp \
    generator/basiccppprojectgenerator.cpp \
    generator/virtualdirectory.cpp \
    generator/virtualfile.cpp \
    generator/virtualfilesystemabstractitem.cpp \
    relationship/association.cpp \
    relationship/dependency.cpp \
    relationship/generalization.cpp \
    relationship/multiplyassociation.cpp \
    relationship/node.cpp \
    relationship/realization.cpp \
    translation/code.cpp \
    translation/projecttranslator.cpp \
    translation/signaturemaker.cpp \
    utility/helpfunctions.cpp \
    gui/newproject.cpp \
    gui/addscope.cpp \
    gui/scenefilter.cpp \
    gui/editmethoddialog.cpp \
    gui/chooseglobaldatabasedialog.cpp \
    gui/componentseditdelegate.cpp \
    gui/signatureeditdelegate.cpp \
    models/componentsmodel.cpp \
    helpers/entityhelpres.cpp \
    entity/components/token.cpp \
    entity/isectional.cpp \
    relationship/RelationFactory.cpp \
    entity/EntityFactory.cpp \
    db/ProjectDatabase.cpp \
    common/ElementsFactory.cpp \
    commands/CreateEntity.cpp \
    gui/graphics/Entity.cpp \
    models/ProjectTreeModel.cpp \
    project/Project.cpp \
    gui/Elements.cpp \
    entity/Type.cpp \
    entity/ExtendedType.cpp \
    entity/Enum.cpp \
    entity/Union.cpp \
    entity/Class.cpp \
    entity/TemplateClass.cpp \
    gui/View.cpp \
    commands/AddRelation.cpp \
    relationship/Relation.cpp \
    db/Database.cpp \
    gui/graphics/GraphicsRelation.cpp \
    common/BasicElement.cpp \
    common/ID.cpp \
    gui/graphics/Scene.cpp \
    application/Application.cpp \
    gui/MainWindow.cpp \
    entity/Scope.cpp \
    entity/GraphicEntityData.cpp \
    entity/Property.cpp \
    entity/Template.cpp \
    entity/ClassMethod.cpp \
    entity/TemplateClassMethod.cpp \
    models/ApplicationModel.cpp \
    models/BasicTreeItem.cpp \
    commands/RemoveProject.cpp \
    commands/BaseCommand.cpp \
    commands/MakeProjectCurrent.cpp \
    commands/CreateScope.cpp \
    helpers/GeneratorID.cpp \
    commands/MoveGraphicObject.cpp \
    commands/MoveTypeToAnotherScope.cpp \
    commands/RemoveComponentsCommands.cpp \
    commands/RenameEntity.cpp \
    gui/EditEntityDialog.cpp \
    gui/About.cpp \
    application/Settings.cpp \
    commands/AddComponentsCommands.cpp \
    commands/OpenProject.cpp

HEADERS += \
    enums.h \
    types.h \
    db/Database.h \
    db/ProjectDatabase.h \
    db/db_types.hpp \
    entity/field.h \
    entity/components/componentsignatureparser.h \
    entity/components/icomponents.h \
    entity/components/componentsmaker.h \
    entity/entity_types.hpp \
    generator/abstractprojectgenerator.h \
    generator/basiccppprojectgenerator.h \
    generator/virtualdirectory.h \
    generator/virtualfile.h \
    generator/virtualfilesystemabstractitem.h \
    generator/generator_types.hpp \
    relationship/association.h \
    relationship/dependency.h \
    relationship/generalization.h \
    relationship/multiplyassociation.h \
    relationship/node.h \
    relationship/realization.h \
    relationship/relationship_types.hpp \
    translation/code.h \
    translation/projecttranslator.h \
    translation/translator_types.hpp \
    translation/signaturemaker.h \
    utility/helpfunctions.h \
    gui/newproject.h \
    gui/addscope.h \
    gui/gui_types.hpp \
    gui/scenefilter.h \
    gui/editmethoddialog.h \
    gui/chooseglobaldatabasedialog.h \
    gui/componentseditdelegate.h \
    gui/signatureeditdelegate.h \
    models/models_types.hpp \
    models/componentsmodel.h \
    helpers/entityhelpres.h \
    entity/components/componentscommon.h \
    entity/components/components_types.h \
    entity/components/token.h \
    qthelpers.h \
    common/ID.h \
    entity/itypeuser.h \
    common/BasicElement.h \
    entity/isectional.h \
    common/common_types.h \
    common/meta.h \
    relationship/RelationFactory.h \
    entity/EntityFactory.h \
    common/ElementsFactory.h \
    commands/CreateEntity.h \
    gui/graphics/Entity.h \
    models/ProjectTreeModel.h \
    project/Project.h \
    gui/Elements.h \
    entity/Type.h \
    entity/ExtendedType.h \
    entity/Enum.h \
    entity/Union.h \
    entity/Class.h \
    entity/TemplateClass.h \
    gui/View.h \
    commands/AddRelation.h \
    relationship/Relation.h \
    gui/graphics/GraphicsRelation.h \
    gui/graphics/Scene.h \
    application/Application.h \
    gui/MainWindow.h \
    entity/Scope.h \
    entity/GraphicEntityData.h \
    common/SharedFromThis.h \
    entity/Property.h \
    db/IScopeSearcher.h \
    db/ITypeSearcher.h \
    entity/Template.h \
    entity/ClassMethod.h \
    entity/TemplateClassMethod.h \
    models/ApplicationModel.h \
    models/BasicTreeItem.h \
    commands/RemoveProject.h \
    commands/BaseCommand.h \
    commands/CommandsTypes.h \
    commands/MakeProjectCurrent.h \
    commands/CreateScope.h \
    helpers/GeneratorID.h \
    gui/graphics/GraphicsTypes.h \
    commands/MoveGraphicObject.h \
    project/ProjectTypes.hpp \
    commands/MoveTypeToAnotherScope.h \
    commands/RemoveComponentsCommands.h \
    commands/RenameEntity.h \
    gui/EditEntityDialog.h \
    gui/graphics/Common.h \
    gui/About.h \
    application/Settings.h \
    Constants.h \
    commands/AddComponentsCommands.h \
    commands/OpenProject.h

FORMS += \
    gui/newproject.ui \
    gui/addscope.ui \
    gui/editentitydialog.ui \
    gui/editmethoddialog.ui \
    gui/chooseglobaldatabasedialog.ui \
    gui/elements.ui \
    gui/MainWindow.ui \
    gui/About.ui

RESOURCES += \
    gui/main.qrc

DISTFILES += \
    CMakeLists.txt

DEFINES *= \
    QT_USE_QSTRINGBUILDER
