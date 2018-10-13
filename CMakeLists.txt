cmake_minimum_required(VERSION 3.9.0 FATAL_ERROR)

# Should be set above project name definition
include(BuildToolset.cmake)

project(UmlTool)

include(BuildParameters.cmake)

set(ROOT ${CMAKE_SOURCE_DIR})
include(FilesList.cmake)

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
               ${COMMON_HEADERS} ${CONVERTERS_HEADERS})

target_link_libraries(uml-tool ${Boost_LIBRARIES})

include(CommonFunctions.cmake)
setCommonTargetProperties(uml-tool)

qt5_use_modules(uml-tool Widgets Core)

if(BUILD_TESTING)
    add_subdirectory(Tests)
endif(BUILD_TESTING)
