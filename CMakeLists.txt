cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

project(UmlTool)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS} -std=c++1y")
set(CMAKE_CXX_COMPILER g++-5)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5Widgets REQUIRED)
find_package(Boost 1.59.0 REQUIRED)

set(ROOT ${CMAKE_SOURCE_DIR})
set(APP ${ROOT}/application)
set(CMD ${ROOT}/commands)
set(DB ${ROOT}/db)
set(ENTITY ${ROOT}/entity)
set(ENTITY_COMPONENTS ${ENTITY}/components)
set(GEN ${ROOT}/generator)
set(GUI ${ROOT}/gui)
set(GUI_GRAPHICS ${GUI}/graphics)
set(HELPERS ${ROOT}/helpers)
set(MODELS ${ROOT}/models)
set(PROJECT ${ROOT}/project)
set(REL ${ROOT}/relationship)
set(TRANSLATION ${ROOT}/translation)
set(UTIL ${ROOT}/utility)
set(COMMON ${ROOT}/common)

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
                    ${PROJECT} ${REL} ${TRANSLATION} ${UTIL} ${COMMON})

add_executable(uml-tool main.cpp ${APP_SRC} ${CMD_SRC} ${DB_SRC} ${ENTITY_SRC}
               ${ENTITY_COMPONENTS_SRC} ${GEN_SRC} ${GUI_SRC} ${GUI_GRAPHICS_SRC} ${HELPERS_SRC}
               ${MODELS_SRC} ${PROJECT_SRC} ${REL_SRC} ${TRANSLATION_SRC} ${UTIL_SRC}
               ${COMMON_SRC} ${RESOURCES})
target_link_libraries(uml-tool ${Boost_LIBRARIES})
qt5_use_modules(uml-tool Widgets Core)
