set(TESTS_DIR ${ROOT}/Tests)
set(TEST_HEADERS
    ${TESTS_DIR}/TestRelationMaker.h
    ${TESTS_DIR}/TestDepthSearch.h
    ${TESTS_DIR}/helpers.h
    ${TESTS_DIR}/TestTypeMaker.h
    ${TESTS_DIR}/TestFileMaker.h
    ${TESTS_DIR}/TestJson.h
    ${TESTS_DIR}/TestProjectTranslator.h
    ${TESTS_DIR}/TestProjectMaker.h
    ${TESTS_DIR}/TestProject.h
    ${TESTS_DIR}/TestSignatureMaker.hpp
    ${TESTS_DIR}/TestSectionalTextConvertion.hpp
    ${TESTS_DIR}/TestIComponents.h
    ${TESTS_DIR}/TestEntities.h
    ${TESTS_DIR}/TestComponentsMaker.h
    ${TESTS_DIR}/TestComponentsBase.h
    ${TESTS_DIR}/TestClassComponents.h
    ${TESTS_DIR}/TestComponentSignatureParser.h
    ${TESTS_DIR}/TestProjectBase.h
    ${TESTS_DIR}/Arguments.hpp
    ${TESTS_DIR}/TestCommands.h)

set(TEST_SOURCES
    ${TESTS_DIR}/main.cpp
    ${TESTS_DIR}/Arguments.cpp)

set(CASES_DIR ${TESTS_DIR}/cases)
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
    ${CASES_DIR}/Memento.h
    ${CASES_DIR}/SectionalTextConvertionCases.h
    ${CASES_DIR}/HelpersCases.h)
