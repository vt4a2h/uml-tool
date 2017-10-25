cmake_minimum_required(VERSION 3.9.0 FATAL_ERROR)

# Get top-level dir
get_filename_component(ROOT ${CMAKE_SOURCE_DIR} DIRECTORY)

include(${ROOT}/BuildToolset.cmake)

project(UmlToolTests)

include(${ROOT}/BuildParameters.cmake)

include(${ROOT}/FilesList.cmake)

set(TEST_HEADERS
    ${CMAKE_SOURCE_DIR}/TestRelationMaker.h
    ${CMAKE_SOURCE_DIR}/TestDepthSearch.h
    ${CMAKE_SOURCE_DIR}/helpers.h
    ${CMAKE_SOURCE_DIR}/TestTypeMaker.h
    ${CMAKE_SOURCE_DIR}/TestFileMaker.h
    ${CMAKE_SOURCE_DIR}/TestJson.h
    ${CMAKE_SOURCE_DIR}/TestProjectTranslator.h
    ${CMAKE_SOURCE_DIR}/TestProjectMaker.h
    ${CMAKE_SOURCE_DIR}/TestProject.h
    ${CMAKE_SOURCE_DIR}/TestSignatureMaker.hpp
    ${CMAKE_SOURCE_DIR}/TestIComponents.h
    ${CMAKE_SOURCE_DIR}/TestEntities.h
    ${CMAKE_SOURCE_DIR}/TestComponentsMaker.h
    ${CMAKE_SOURCE_DIR}/TestComponentsBase.h
    ${CMAKE_SOURCE_DIR}/TestClassComponents.h
    ${CMAKE_SOURCE_DIR}/TestComponentSignatureParser.h
    ${CMAKE_SOURCE_DIR}/TestProjectBase.h
    ${CMAKE_SOURCE_DIR}/TestCommands.h)

set(CASES_DIR ${CMAKE_SOURCE_DIR}/cases)
set(CASES_HEADERS
    ${CASES_DIR}/TypeMakerTestCases.h
    ${CASES_DIR}/SignatureParserCases.h
    ${CASES_DIR}/SignatureMakerCases.h
    ${CASES_DIR}/RelationMakerCases.h
    ${CASES_DIR}/ProjectTranslatorCases.h
    ${CASES_DIR}/ProjectCases.h
    ${CASES_DIR}/IComponentsCases.h
    ${CASES_DIR}/FileJsonCases.h
    ${CASES_DIR}/EntitiesCases.h
    ${CASES_DIR}/DepthSearchCases.h
    ${CASES_DIR}/ComponentsMakerCases.h
    ${CASES_DIR}/ClassComponentsCases.h
    ${CASES_DIR}/FileMakerCases.h
    ${CASES_DIR}/CommandsCases.h
    ${CASES_DIR}/ProjectMakerCases.h
    ${CASES_DIR}/ID.h
    ${CASES_DIR}/HelpersCases.h)

include_directories(${Qt5Widgets_INCLUDES} ${Boost_INCLUDE_DIRS} ${CMAKE_SOURCE_DIR} ${CASES_DIR})

#add_executable(tests main.cpp ${TEST_HEADERS} ${CASES_HEADERS} ${FREE_SRC} ${APP_SRC} ${CMD_SRC} ${DB_SRC} ${ENTITY_SRC}
#               ${ENTITY_COMPONNTS_SRC} ${GEN_SRC} ${GUI_SRC} ${GUI_GRAPHICS_SRC} ${HELPERS_SRC}
#               ${MODELS_SRC} ${PROJECT_SRC} ${REL_SRC} ${TRANSLATION_SRC} ${UTIL_SRC}
#               ${COMMON_SRC}
#               ${FREE_HEADERS} ${APP_HEADERS} ${CMD_HEADERS} ${DB_HEADERS} ${ENTITY_HEADERS}
#               ${ENTITY_COMPONNTS_HEADERS} ${GEN_HEADERS} ${GUI_HEADERS} ${GUI_GRAPHICS_HEADERS} ${HELPERS_HEADERS}
#               ${MODELS_HEADERS} ${PROJECT_HEADERS} ${REL_HEADERS} ${TRANSLATION_HEADERS} ${UTIL_HEADERS}
#               ${COMMON_HEADERS})
#
#target_link_libraries(tests ${Boost_LIBRARIES})
#
#include(${ROOT}/CommonFunctions.cmake)
#setCommonTargetProperties(tests)
#
#qt5_use_modules(tests Widgets Core)

