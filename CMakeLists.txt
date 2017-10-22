cmake_minimum_required(VERSION 3.9.0 FATAL_ERROR)

if(UNIX AND NOT APPLE)
    set(CMAKE_C_COMPILER gcc-7)
    set(CMAKE_CXX_COMPILER g++-7)
else()
    message(FATAL_ERROR "Unsupported system")
endif(UNIX AND NOT APPLE)

project(UmlTool)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS} -Wall -Wextra -std=c++1z")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Ofast -DNDEBUG")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5Widgets REQUIRED)
find_package(Boost 1.59.0 REQUIRED)

set(ROOT ${CMAKE_SOURCE_DIR})

set(APP ${ROOT}/Application)
set(APP_HEADERS
    ${APP}/Application.h
    ${APP}/Settings.h)
set(APP_SRC
    ${APP}/Application.cpp
    ${APP}/Settings.cpp)

set(CMD ${ROOT}/Commands)
set(CMD_HEADERS
    ${CMD}/CreateEntity.h
    ${CMD}/AddRelation.h
    ${CMD}/RemoveProject.h
    ${CMD}/BaseCommand.h
    ${CMD}/CommandsTypes.h
    ${CMD}/MakeProjectCurrent.h
    ${CMD}/CreateScope.h
    ${CMD}/MoveGraphicObject.h
    ${CMD}/MoveTypeToAnotherScope.h
    ${CMD}/RemoveComponentsCommands.h
    ${CMD}/RenameEntity.h
    ${CMD}/AddComponentsCommands.h
    ${CMD}/OpenProject.h)
set(CMD_SRC
    ${CMD}/CreateEntity.cpp
    ${CMD}/AddRelation.cpp
    ${CMD}/RemoveProject.cpp
    ${CMD}/BaseCommand.cpp
    ${CMD}/MakeProjectCurrent.cpp
    ${CMD}/CreateScope.cpp
    ${CMD}/MoveGraphicObject.cpp
    ${CMD}/MoveTypeToAnotherScope.cpp
    ${CMD}/RemoveComponentsCommands.cpp
    ${CMD}/RenameEntity.cpp
    ${CMD}/AddComponentsCommands.cpp
    ${CMD}/OpenProject.cpp)

set(DB ${ROOT}/DB)
set(DB_HEADERS
    ${DB}/Database.h
    ${DB}/ProjectDatabase.h
    ${DB}/IScopeSearcher.h
    ${DB}/ITypeSearcher.h
    ${DB}/DBTypes.hpp)
set(DB_SRC
    ${DB}/ProjectDatabase.cpp
    ${DB}/Database.cpp)

set(ENTITY ${ROOT}/Entity)
set(ENTITY_HEADERS
    ${ENTITY}/field.h
    ${ENTITY}/entity_types.hpp
    ${ENTITY}/itypeuser.h
    ${ENTITY}/isectional.h
    ${ENTITY}/EntityFactory.h
    ${ENTITY}/Type.h
    ${ENTITY}/ExtendedType.h
    ${ENTITY}/Enum.h
    ${ENTITY}/Union.h
    ${ENTITY}/Class.h
    ${ENTITY}/TemplateClass.h
    ${ENTITY}/Scope.h
    ${ENTITY}/GraphicEntityData.h
    ${ENTITY}/Property.h
    ${ENTITY}/Template.h
    ${ENTITY}/ClassMethod.h
    ${ENTITY}/TemplateClassMethod.h)
set(ENTITY_SRC
    ${ENTITY}/field.cpp
    ${ENTITY}/isectional.cpp
    ${ENTITY}/EntityFactory.cpp
    ${ENTITY}/Type.cpp
    ${ENTITY}/ExtendedType.cpp
    ${ENTITY}/Enum.cpp
    ${ENTITY}/Union.cpp
    ${ENTITY}/Class.cpp
    ${ENTITY}/TemplateClass.cpp
    ${ENTITY}/Scope.cpp
    ${ENTITY}/GraphicEntityData.cpp
    ${ENTITY}/Property.cpp
    ${ENTITY}/Template.cpp
    ${ENTITY}/ClassMethod.cpp
    ${ENTITY}/TemplateClassMethod.cpp)

set(ENTITY_COMPONENTS ${ENTITY}/Components)
set(ENTITY_COMPONNTS_HEADERS
    ${ENTITY_COMPONENTS}/componentsignatureparser.h
    ${ENTITY_COMPONENTS}/icomponents.h
    ${ENTITY_COMPONENTS}/componentsmaker.h
    ${ENTITY_COMPONENTS}/componentscommon.h
    ${ENTITY_COMPONENTS}/components_types.h
    ${ENTITY_COMPONENTS}/token.h)
set(ENTITY_COMPONNTS_SRC
    ${ENTITY_COMPONENTS}/componentsignatureparser.cpp
    ${ENTITY_COMPONENTS}/icomponents.cpp
    ${ENTITY_COMPONENTS}/componentsmaker.cpp
    ${ENTITY_COMPONENTS}/token.cpp)

set(GEN ${ROOT}/Generator)
set(GEN_HEADERS
    ${GEN}/abstractprojectgenerator.h
    ${GEN}/basiccppprojectgenerator.h
    ${GEN}/virtualdirectory.h
    ${GEN}/virtualfile.h
    ${GEN}/virtualfilesystemabstractitem.h
    ${GEN}/generator_types.hpp)
set(GEN_SOURCES
    ${GEN}/abstractprojectgenerator.cpp
    ${GEN}/basiccppprojectgenerator.cpp
    ${GEN}/virtualdirectory.cpp
    ${GEN}/virtualfile.cpp
    ${GEN}/virtualfilesystemabstractitem.cpp)

set(GUI ${ROOT}/GUI)
set(GUI_HEADERS
    ${GUI}/addscope.h
    ${GUI}/gui_types.hpp
    ${GUI}/scenefilter.h
    ${GUI}/editmethoddialog.h
    ${GUI}/chooseglobaldatabasedialog.h
    ${GUI}/componentseditdelegate.h
    ${GUI}/signatureeditdelegate.h
    ${GUI}/View.h
    ${GUI}/Elements.h
    ${GUI}/MainWindow.h
    ${GUI}/EditEntityDialog.h
    ${GUI}/About.h
    ${GUI}/NewProject.h
    ${GUI}/HtmlDelegate.h
    ${GUI}/Preferences.h)
set(GUI_SRC
    ${GUI}/addscope.cpp
    ${GUI}/scenefilter.cpp
    ${GUI}/editmethoddialog.cpp
    ${GUI}/chooseglobaldatabasedialog.cpp
    ${GUI}/componentseditdelegate.cpp
    ${GUI}/signatureeditdelegate.cpp
    ${GUI}/Elements.cpp
    ${GUI}/View.cpp
    ${GUI}/MainWindow.cpp
    ${GUI}/EditEntityDialog.cpp
    ${GUI}/About.cpp
    ${GUI}/NewProject.cpp
    ${GUI}/HtmlDelegate.cpp
    ${GUI}/Preferences.cpp)

set(GUI_GRAPHICS ${GUI}/graphics)
set(GUI_GRAPHICS_HEADERS
    ${GUI_GRAPHICS}/Entity.h
    ${GUI_GRAPHICS}/GraphicsRelation.h
    ${GUI_GRAPHICS}/Scene.h
    ${GUI_GRAPHICS}/GraphicsTypes.h
    ${GUI_GRAPHICS}/Common.h)
set(GUI_GRAPHICS_SRC
    ${GUI_GRAPHICS}/Entity.cpp
    ${GUI_GRAPHICS}/GraphicsRelation.cpp
    ${GUI_GRAPHICS}/Scene.cpp)

set(HELPERS ${ROOT}/Helpers)
set(HELPERS_HEADERS
    ${HELPERS}/entityhelpres.h
    ${HELPERS}/GeneratorID.h)
set(HELPERS_SRC
    ${HELPERS}/entityhelpres.cpp
    ${HELPERS}/GeneratorID.cpp)

set(MODELS ${ROOT}/Models)
set(MODELS_HEADERS
    ${MODELS}/ProjectTreeModel.h
    ${MODELS}/ApplicationModel.h
    ${MODELS}/BasicTreeItem.h
    ${MODELS}/ModelsTypes.hpp
    ${MODELS}/ComponentsModel.h
    ${MODELS}/MessagesModel.h
    ${MODELS}/IMessenger.h)
set(MODELS_SRC
    ${MODELS}/ProjectTreeModel.cpp
    ${MODELS}/ApplicationModel.cpp
    ${MODELS}/BasicTreeItem.cpp
    ${MODELS}/ComponentsModel.cpp
    ${MODELS}/MessagesModel.cpp)

set(PROJECT ${ROOT}/Project)
set(PROJECT_HEADERS
    ${PROJECT}/Project.h
    ${PROJECT}/ProjectTypes.hpp)
set(PROJECT_SRC
    ${PROJECT}/Project.cpp)

set(REL ${ROOT}/Relationship)
set(REL_HEADERS
    ${REL}/association.h
    ${REL}/dependency.h
    ${REL}/generalization.h
    ${REL}/multiplyassociation.h
    ${REL}/node.h
    ${REL}/realization.h
    ${REL}/relationship_types.hpp
    ${REL}/RelationFactory.h
    ${REL}/Relation.h)
set(REL_SRC
    ${REL}/association.cpp
    ${REL}/dependency.cpp
    ${REL}/generalization.cpp
    ${REL}/multiplyassociation.cpp
    ${REL}/node.cpp
    ${REL}/realization.cpp
    ${REL}/RelationFactory.cpp
    ${REL}/Relation.cpp)

set(TRANSLATION ${ROOT}/Translation)
set(TRANSLATION_HEADERS
    ${TRANSLATION}/code.h
    ${TRANSLATION}/projecttranslator.h
    ${TRANSLATION}/translator_types.hpp
    ${TRANSLATION}/signaturemaker.h)
set(TRANSLATION_SRC
    ${TRANSLATION}/code.cpp
    ${TRANSLATION}/projecttranslator.cpp
    ${TRANSLATION}/signaturemaker.cpp)

set(UTIL ${ROOT}/Utility)
set(UTIL_HEADERS
    ${UTIL}/helpfunctions.h)
set(UTIL_SRC
    ${UTIL}/helpfunctions.cpp)

set(COMMON ${ROOT}/Common)
set(COMMON_HEADERS
    ${COMMON}/ID.h
    ${COMMON}/BasicElement.h
    ${COMMON}/common_types.h
    ${COMMON}/meta.h
    ${COMMON}/ElementsFactory.h
    ${COMMON}/SharedFromThis.h)
set(COMMON_SRC
    ${COMMON}/ElementsFactory.cpp
    ${COMMON}/BasicElement.cpp
    ${COMMON}/ID.cpp)

set(FREE_HEADERS
    ${ROOT}/enums.h
    ${ROOT}/types.h)
set(FREE_SRC
    ${ROOT}/main.cpp
    ${ROOT}/templates.cpp)

set(BOOST_DI ${ROOT}/boost-di/include)

add_definitions(-DQT_USE_QSTRINGBUILDER)

file(GLOB APP_SRC ${APP}/*.cpp)
file(GLOB CMD_SRC ${CMD}/*.cpp)
file(GLOB DB_SRC ${DB}/*.cpp)
file(GLOB ENTITY_SRC ${ENTITY}/*.cpp)
file(GLOB ENTITY_COMPONENTS_SRC ${ENTITY_COMPONENTS}/*.cpp)
file(GLOB GEN_SRC ${GEN}/*.cpp)
file(GLOB GUI_SRC ${GUI}/*.cpp)
file(GLOB GUI_GRAPHICS_SRC ${GUI_GRAPHICS}/*.cpp)
file(GLOB HELPERS_SRC ${HELPERS}/*.cpp)
file(GLOB MODELS_SRC ${MODELS}/*.cpp)
file(GLOB PROJECT_SRC ${PROJECT}/*.cpp)
file(GLOB REL_SRC ${REL}/*.cpp)
file(GLOB TRANSLATION_SRC ${TRANSLATION}/*.cpp)
file(GLOB UTIL_SRC ${UTIL}/*.cpp)
file(GLOB COMMON_SRC ${COMMON}/*.cpp)

set(RESOURCES ${GUI}/main.qrc)

include_directories(${Qt5Widgets_INCLUDES} ${Boost_INCLUDE_DIRS} ${ROOT} ${APP} ${CMD} ${DB} ${ENTITY}
                    ${ENTITY_COMPONENTS} ${GEN} ${GUI} ${GUI_GRAPHICS} ${HELPERS} ${MODELS}
                    ${PROJECT} ${REL} ${TRANSLATION} ${UTIL} ${COMMON} ${BOOST_DI})

add_executable(uml-tool main.cpp ${APP_SRC} ${CMD_SRC} ${DB_SRC} ${ENTITY_SRC}
               ${ENTITY_COMPONENTS_SRC} ${GEN_SRC} ${GUI_SRC} ${GUI_GRAPHICS_SRC} ${HELPERS_SRC}
               ${MODELS_SRC} ${PROJECT_SRC} ${REL_SRC} ${TRANSLATION_SRC} ${UTIL_SRC}
               ${COMMON_SRC} ${RESOURCES})

target_link_libraries(uml-tool ${Boost_LIBRARIES})

set_property(TARGET uml-tool PROPERTY CXX_STANDARD 17)
set_property(TARGET uml-tool PROPERTY CXX_EXTENSIONS OFF)

qt5_use_modules(uml-tool Widgets Core)
