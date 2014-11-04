CONFIG += core gui c++11

QT += qml quick widgets

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
    project/project.cpp

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
    project/project.h

RESOURCES += \
    gui/qml.qrc
