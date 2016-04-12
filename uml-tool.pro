TEMPLATE = app

CONFIG += core gui c++14

QT += widgets

linux-g++ {
    message(********** Building with gcc 5 **********)
    QMAKE_CXX = g++-5
    QMAKE_CXXFLAGS *= -std=c++1y
}

SOURCES += \
    main.cpp \
    templates.cpp \
    db/database.cpp \
    entity/classmethod.cpp \
    entity/field.cpp \
    entity/scope.cpp \
    entity/template.cpp \
    entity/templateclassmethod.cpp \
    entity/components/componentsignatureparser.cpp \
    entity/components/icomponents.cpp \
    entity/components/componentsmaker.cpp \
    entity/property.cpp \
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
    relationship/relation.cpp \
    translation/code.cpp \
    translation/projecttranslator.cpp \
    translation/signaturemaker.cpp \
    utility/helpfunctions.cpp \
    application/application.cpp \
    gui/mainwindow.cpp \
    gui/about.cpp \
    gui/newproject.cpp \
    gui/addscope.cpp \
    gui/editentitydialog.cpp \
    gui/scenefilter.cpp \
    gui/editmethoddialog.cpp \
    gui/chooseglobaldatabasedialog.cpp \
    gui/componentseditdelegate.cpp \
    gui/signatureeditdelegate.cpp \
    models/basictreeitem.cpp \
    models/applicationmodel.cpp \
    models/componentsmodel.cpp \
    commands/createscope.cpp \
    commands/basecommand.cpp \
    commands/movegraphicobject.cpp \
    commands/renameentity.cpp \
    commands/movetypetootherscope.cpp \
    commands/removecomponentscommands.cpp \
    commands/addcomponentscommands.cpp \
    helpers/entityhelpres.cpp \
    entity/components/token.cpp \
    application/settings.cpp \
    gui/graphics/scene.cpp \
    gui/graphics/graphicsrelation.cpp \
    commands/addrelation.cpp \
    helpers/generatorid.cpp \
    common/id.cpp \
    common/basicelement.cpp \
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
    gui/View.cpp

HEADERS += \
    enums.h \
    types.h \
    db/database.h \
    db/ProjectDatabase.h \
    db/db_types.hpp \
    entity/classmethod.h \
    entity/field.h \
    entity/scope.h \
    entity/template.h \
    entity/templateclassmethod.h \
    entity/components/componentsignatureparser.h \
    entity/components/icomponents.h \
    entity/components/componentsmaker.h \
    entity/entity_types.hpp \
    entity/property.h \
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
    relationship/relation.h \
    relationship/relationship_types.hpp \
    translation/code.h \
    translation/projecttranslator.h \
    translation/translator_types.hpp \
    translation/signaturemaker.h \
    utility/helpfunctions.h \
    application/application.h \
    gui/mainwindow.h \
    gui/about.h \
    gui/newproject.h \
    gui/addscope.h \
    gui/editentitydialog.h \
    gui/gui_types.hpp \
    gui/scenefilter.h \
    gui/editmethoddialog.h \
    gui/chooseglobaldatabasedialog.h \
    gui/componentseditdelegate.h \
    gui/signatureeditdelegate.h \
    models/basictreeitem.h \
    models/applicationmodel.h \
    models/models_types.hpp \
    models/componentsmodel.h \
    commands/createscope.h \
    commands/basecommand.h \
    commands/movegraphicobject.h \
    commands/renameentity.h \
    commands/movetypetootherscope.h \
    commands/commands_types.hpp \
    commands/removecomponentscommands.h \
    commands/addcomponentscommands.h \
    project/project_types.hpp \
    application/settings.h \
    constants.h \
    helpers/entityhelpres.h \
    entity/components/componentscommon.h \
    entity/components/components_types.h \
    entity/components/token.h \
    qthelpers.h \
    gui/graphics/scene.h \
    gui/graphics/common.h \
    gui/graphics/graphicsrelation.h \
    commands/addrelation.h \
    gui/graphics/graphics_types.h \
    helpers/generatorid.h \
    common/id.h \
    db/itypesearcher.h \
    entity/itypeuser.h \
    common/basicelement.h \
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
    gui/View.h

FORMS += \
    gui/mainwindow.ui \
    gui/about.ui \
    gui/newproject.ui \
    gui/addscope.ui \
    gui/editentitydialog.ui \
    gui/editmethoddialog.ui \
    gui/chooseglobaldatabasedialog.ui \
    gui/elements.ui

RESOURCES += \
    gui/main.qrc

DISTFILES += \
    CMakeLists.txt
