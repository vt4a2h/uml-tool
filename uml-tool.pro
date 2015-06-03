CONFIG += core gui c++14

QT += widgets

SOURCES += \
    main.cpp \
    constants.cpp \
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
    models/basictreeitem.cpp \
    models/projecttreemodel.cpp \
    models/applicationmodel.cpp \
    entity/icomponents.cpp \
    entity/basicentity.cpp \
    gui/addscope.cpp \
    gui/graphics/entity.cpp \
    entity/entitiesfactory.cpp \
    gui/editentitydialog.cpp \
    commands/createscope.cpp \
    commands/basecommand.cpp \
    commands/movegraphicobject.cpp \
    commands/createentity.cpp \
    commands/renameentity.cpp \
    commands/movetypetootherscope.cpp \
    gui/scenefilter.cpp \
    models/classcomponentsmodel.cpp \
    gui/classcomponentseditdelegate.cpp \
    gui/signaturemaker.cpp

HEADERS += \
    enums.h \
    types.h \
    db/database.h \
    db/projectdatabase.h \
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
    generator/abstractprojectgenerator.h \
    generator/basiccppprojectgenerator.h \
    generator/virtualdirectory.h \
    generator/virtualfile.h \
    generator/virtualfilesystemabstractitem.h \
    relationship/association.h \
    relationship/dependency.h \
    relationship/generalization.h \
    relationship/multiplyassociation.h \
    relationship/node.h \
    relationship/realization.h \
    relationship/relation.h \
    translator/code.h \
    translator/projecttranslator.h \
    utility/helpfunctions.h \
    application/application.h \
    project/project.h \
    gui/mainwindow.h \
    gui/about.h \
    gui/newproject.h \
    models/basictreeitem.h \
    models/projecttreemodel.h \
    models/applicationmodel.h \
    entity/icomponents.h \
    entity/basicentity.h \
    gui/addscope.h \
    gui/graphics/entity.h \
    entity/entitiesfactory.h \
    gui/editentitydialog.h \
    commands/createscope.h \
    commands/basecommand.h \
    commands/movegraphicobject.h \
    commands/createentity.h \
    commands/renameentity.h \
    commands/movetypetootherscope.h \
    entity/entity_types.hpp \
    relationship/relationship_types.hpp \
    db/db_types.hpp \
    translator/translator_types.hpp \
    generator/generator_types.hpp \
    project/project_types.hpp \
    gui/gui_types.hpp \
    models/models_types.hpp \
    commands/commands_types.hpp \
    gui/scenefilter.h \
    models/classcomponentsmodel.h \
    gui/classcomponentseditdelegate.h \
    gui/signaturemaker.h

FORMS += \
    gui/mainwindow.ui \
    gui/about.ui \
    gui/newproject.ui \
    gui/addscope.ui \
    gui/editentitydialog.ui

RESOURCES += \
    gui/main.qrc

debug:MOC_DIR = debug/moc
