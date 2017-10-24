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
include(FilesList.cmake)

add_definitions(-DQT_USE_QSTRINGBUILDER)

set(RESOURCES ${GUI}/main.qrc)

include_directories(${Qt5Widgets_INCLUDES} ${Boost_INCLUDE_DIRS} ${APP} ${CMD} ${DB} ${ENTITY}
                    ${ENTITY_COMPONENTS} ${GEN} ${GUI} ${GUI_GRAPHICS} ${HELPERS} ${MODELS}
                    ${PROJECT} ${REL} ${TRANSLATION} ${UTIL} ${COMMON} ${BOOST_DI})

add_executable(uml-tool ${FREE_SRC} ${APP_SRC} ${CMD_SRC} ${DB_SRC} ${ENTITY_SRC}
               ${ENTITY_COMPONNTS_SRC} ${GEN_SRC} ${GUI_SRC} ${GUI_GRAPHICS_SRC} ${HELPERS_SRC}
               ${MODELS_SRC} ${PROJECT_SRC} ${REL_SRC} ${TRANSLATION_SRC} ${UTIL_SRC}
               ${COMMON_SRC} ${RESOURCES}
               ${FREE_HEADERS} ${APP_HEADERS} ${CMD_HEADERS} ${DB_HEADERS} ${ENTITY_HEADERS}
               ${ENTITY_COMPONNTS_HEADERS} ${GEN_HEADERS} ${GUI_HEADERS} ${GUI_GRAPHICS_HEADERS} ${HELPERS_HEADERS}
               ${MODELS_HEADERS} ${PROJECT_HEADERS} ${REL_HEADERS} ${TRANSLATION_HEADERS} ${UTIL_HEADERS}
               ${COMMON_HEADERS})

target_link_libraries(uml-tool ${Boost_LIBRARIES})

set_property(TARGET uml-tool PROPERTY CXX_STANDARD 17)
set_property(TARGET uml-tool PROPERTY CXX_EXTENSIONS OFF)

qt5_use_modules(uml-tool Widgets Core)
