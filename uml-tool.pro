TEMPLATE = app

CONFIG += core gui

QT += widgets

QMAKE_CXX = gcc-5
QMAKE_LINK = gcc-5
QMAKE_LFLAGS *= -lstdc++
QMAKE_CXXFLAGS *= -std=c++1y

QMAKE_CXXFLAGS_DEBUG *= --coverage
QMAKE_LFLAGS_DEBUG *= --coverage

SOURCES += \
    main.cpp \
    templates.cpp \
    db/database.cpp \
    db/projectdatabase.cpp \
    entity/class.cpp \
    entity/classmethod.cpp \
    entity/enum.cpp \
    entity/extendedtype.cpp \
    entity/field.cpp \
    entity/scope.cpp \
    entity/template.cpp \
    entity/templateclass.cpp \
    entity/templateclassmethod.cpp \
    entity/type.cpp \
    entity/union.cpp \
    entity/components/componentsignatureparser.cpp \
    entity/components/icomponents.cpp \
    entity/basicentity.cpp \
    entity/entitiesfactory.cpp \
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
    translator/code.cpp \
    translator/projecttranslator.cpp \
    utility/helpfunctions.cpp \
    application/application.cpp \
    project/project.cpp \
    gui/mainwindow.cpp \
    gui/about.cpp \
    gui/newproject.cpp \
    gui/addscope.cpp \
    gui/graphics/entity.cpp \
    gui/editentitydialog.cpp \
    gui/scenefilter.cpp \
    gui/signaturemaker.cpp \
    gui/editmethoddialog.cpp \
    gui/chooseglobaldatabasedialog.cpp \
    gui/componentseditdelegate.cpp \
    gui/signatureeditdelegate.cpp \
    gui/componentsmaker.cpp \
    models/basictreeitem.cpp \
    models/projecttreemodel.cpp \
    models/applicationmodel.cpp \
    models/componentsmodel.cpp \
    commands/createscope.cpp \
    commands/basecommand.cpp \
    commands/movegraphicobject.cpp \
    commands/renameentity.cpp \
    commands/movetypetootherscope.cpp \
    commands/removecomponentscommands.cpp \
    commands/addcomponentscommands.cpp \
    helpers/entityhelpres.cpp

HEADERS += \
    enums.h \
    types.h \
    db/database.h \
    db/projectdatabase.h \
    db/db_types.hpp \
    entity/class.h \
    entity/classmethod.h \
    entity/enum.h \
    entity/extendedtype.h \
    entity/field.h \
    entity/scope.h \
    entity/template.h \
    entity/templateclass.h \
    entity/templateclassmethod.h \
    entity/type.h \
    entity/union.h \
    entity/components/componentsignatureparser.h \
    entity/components/icomponents.h \
    entity/basicentity.h \
    entity/entitiesfactory.h \
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
    translator/code.h \
    translator/projecttranslator.h \
    translator/translator_types.hpp \
    utility/helpfunctions.h \
    application/application.h \
    project/project.h \
    gui/mainwindow.h \
    gui/about.h \
    gui/newproject.h \
    gui/addscope.h \
    gui/graphics/entity.h \
    gui/editentitydialog.h \
    gui/gui_types.hpp \
    gui/scenefilter.h \
    gui/signaturemaker.h \
    gui/editmethoddialog.h \
    gui/chooseglobaldatabasedialog.h \
    gui/componentseditdelegate.h \
    gui/signatureeditdelegate.h \
    gui/componentsmaker.h \
    models/basictreeitem.h \
    models/projecttreemodel.h \
    models/applicationmodel.h \
    models/models_types.hpp \
    models/componentsmodel.h \
    commands/createscope.h \
    commands/basecommand.h \
    commands/movegraphicobject.h \
    commands/createentity.h \
    commands/renameentity.h \
    commands/movetypetootherscope.h \
    commands/commands_types.hpp \
    commands/removecomponentscommands.h \
    commands/addcomponentscommands.h \
    project/project_types.hpp \
    application/settings.h \
    constants.h \
    helpers/entityhelpres.h

FORMS += \
    gui/mainwindow.ui \
    gui/about.ui \
    gui/newproject.ui \
    gui/addscope.ui \
    gui/editentitydialog.ui \
    gui/editmethoddialog.ui \
    gui/chooseglobaldatabasedialog.ui

RESOURCES += \
    gui/main.qrc
