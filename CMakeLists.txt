cmake_minimum_required(VERSION 3.13.0 FATAL_ERROR)

# Should be set above project name definition
include(BuildToolset.cmake)

project(UmlTool)

include(BuildParameters.cmake)

find_package(Qt5Widgets REQUIRED)
if(BUILD_TESTING)
    find_package(GTest REQUIRED)
    enable_testing()
endif(BUILD_TESTING)

include_directories(SYSTEM ${CMAKE_SOURCE_DIR}/range-v3/include
                           ${CMAKE_SOURCE_DIR}/boost-di/include)

set(ROOT ${CMAKE_SOURCE_DIR})
include(FilesList.cmake)

set(RESOURCES ${GUI}/main.qrc)

include_directories(${Qt5Widgets_INCLUDES} ${APP} ${CMD} ${DB} ${ENTITY}
                    ${ENTITY_COMPONENTS} ${GEN} ${GUI} ${GUI_GRAPHICS} ${HELPERS} ${MODELS}
                    ${PROJECT} ${REL} ${TRANSLATION} ${UTIL} ${COMMON} ${BOOST_DI})

add_executable(uml-tool ${FREE_SRC} ${APP_SRC} ${CMD_SRC} ${DB_SRC} ${ENTITY_SRC}
               ${ENTITY_COMPONNTS_SRC} ${GEN_SRC} ${GUI_SRC} ${GUI_GRAPHICS_SRC} ${HELPERS_SRC}
               ${MODELS_SRC} ${PROJECT_SRC} ${REL_SRC} ${TRANSLATION_SRC} ${UTIL_SRC}
               ${COMMON_SRC} ${RESOURCES} ${CONVERSION_SRC}
               ${FREE_HEADERS} ${APP_HEADERS} ${CMD_HEADERS} ${DB_HEADERS} ${ENTITY_HEADERS}
               ${ENTITY_COMPONNTS_HEADERS} ${GEN_HEADERS} ${GUI_HEADERS} ${GUI_GRAPHICS_HEADERS} ${HELPERS_HEADERS}
               ${MODELS_HEADERS} ${PROJECT_HEADERS} ${REL_HEADERS} ${TRANSLATION_HEADERS} ${UTIL_HEADERS}
               ${COMMON_HEADERS} ${CONVERTERS_HEADERS})

include(CommonFunctions.cmake)
setCommonTargetProperties(uml-tool)

if(BUILD_TESTING)
    include(Tests/TestFiles.cmake)

    include_directories(${CASES_DIR} ${GTEST_INCLUDE_DIRS})

    add_executable(tests ${TESTS_DIR} ${TEST_SOURCES} ${APP_SRC} ${CMD_SRC} ${DB_SRC} ${ENTITY_SRC}
                   ${ENTITY_COMPONNTS_SRC} ${GEN_SRC} ${GUI_SRC} ${GUI_GRAPHICS_SRC} ${HELPERS_SRC}
                   ${MODELS_SRC} ${PROJECT_SRC} ${REL_SRC} ${TRANSLATION_SRC} ${UTIL_SRC}
                   ${COMMON_SRC} ${CONVERSION_SRC}
                   ${TEST_HEADERS} ${CASES_HEADERS} ${FREE_HEADERS} ${APP_HEADERS} ${CMD_HEADERS} ${DB_HEADERS}
                   ${ENTITY_HEADERS} ${ENTITY_COMPONNTS_HEADERS} ${GEN_HEADERS} ${GUI_HEADERS} ${GUI_GRAPHICS_HEADERS}
                   ${HELPERS_HEADERS} ${MODELS_HEADERS} ${PROJECT_HEADERS} ${REL_HEADERS} ${TRANSLATION_HEADERS}
                   ${UTIL_HEADERS} ${COMMON_HEADERS} ${CONVERTERS_HEADERS})

    target_link_libraries(tests ${GTEST_LIBRARIES} pthread)

    setCommonTargetProperties(tests)
    qt5_use_modules(tests Widgets Core)

    gtest_discover_tests(tests)
endif(BUILD_TESTING)

qt5_use_modules(uml-tool Widgets Core)
