cmake_minimum_required(VERSION 3.9.0 FATAL_ERROR)

project(UmlToolTests)

include(${ROOT}/BuildToolset.cmake)
include(${ROOT}/BuildParameters.cmake)

if (COLLECT_COVERAGE)
    SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lstdc++ --coverage")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g --coverage")
endif(COLLECT_COVERAGE)

enable_testing()
find_package(GTest REQUIRED)

include(${ROOT}/FilesList.cmake)

set(TEST_HEADERS
    ${ROOT}/Tests/TestRelationMaker.h
    ${ROOT}/Tests/TestDepthSearch.h
    ${ROOT}/Tests/helpers.h
    ${ROOT}/Tests/TestTypeMaker.h
    ${ROOT}/Tests/TestFileMaker.h
    ${ROOT}/Tests/TestJson.h
    ${ROOT}/Tests/TestProjectTranslator.h
    ${ROOT}/Tests/TestProjectMaker.h
    ${ROOT}/Tests/TestProject.h
    ${ROOT}/Tests/TestSignatureMaker.hpp
    ${ROOT}/Tests/TestIComponents.h
    ${ROOT}/Tests/TestEntities.h
    ${ROOT}/Tests/TestComponentsMaker.h
    ${ROOT}/Tests/TestComponentsBase.h
    ${ROOT}/Tests/TestClassComponents.h
    ${ROOT}/Tests/TestComponentSignatureParser.h
    ${ROOT}/Tests/TestProjectBase.h
    ${ROOT}/Tests/Arguments.hpp
    ${ROOT}/Tests/TestCommands.h)
    
set(TEST_SOURCES
    ${ROOT}/Tests/main.cpp
    ${ROOT}/Tests/Arguments.cpp)

set(CASES_DIR ${ROOT}/Tests/cases)
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

include_directories(${Qt5Widgets_INCLUDES} ${Boost_INCLUDE_DIRS} ${CMAKE_SOURCE_DIR} ${CASES_DIR} ${GTEST_INCLUDE_DIRS})

add_executable(tests ${TEST_SOURCES} ${APP_SRC} ${CMD_SRC} ${DB_SRC} ${ENTITY_SRC}
               ${ENTITY_COMPONNTS_SRC} ${GEN_SRC} ${GUI_SRC} ${GUI_GRAPHICS_SRC} ${HELPERS_SRC}
               ${MODELS_SRC} ${PROJECT_SRC} ${REL_SRC} ${TRANSLATION_SRC} ${UTIL_SRC}
               ${COMMON_SRC}
               ${TEST_HEADERS} ${CASES_HEADERS} ${FREE_HEADERS} ${APP_HEADERS} ${CMD_HEADERS} ${DB_HEADERS}
               ${ENTITY_HEADERS} ${ENTITY_COMPONNTS_HEADERS} ${GEN_HEADERS} ${GUI_HEADERS} ${GUI_GRAPHICS_HEADERS}
               ${HELPERS_HEADERS} ${MODELS_HEADERS} ${PROJECT_HEADERS} ${REL_HEADERS} ${TRANSLATION_HEADERS}
               ${UTIL_HEADERS} ${COMMON_HEADERS})

target_link_libraries(tests ${Boost_LIBRARIES} ${GTEST_LIBRARIES} pthread)

include(${ROOT}/CommonFunctions.cmake)
setCommonTargetProperties(tests)
qt5_use_modules(tests Widgets Core)
