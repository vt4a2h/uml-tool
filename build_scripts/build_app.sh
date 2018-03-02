#!/usr/bin/env bash

# Read args
while [[ $# -gt 0 ]]
do
   case "$1" in
      -p|--path)
      PARENT_DIR="$2"
      shift
      shift
      ;;

      -b|--build)
      BUILD=1
      shift
      ;;

      -t|--test)
      TEST=1
      shift
      ;;

      -c|--coverage)
      COV=1
      shift
      ;;
   esac
done

if [ -z ${PARENT_DIR} ] || [ ! -d ${PARENT_DIR} ]; then
   echo "No build directory specified"
   exit -1
fi

# Setup env
BUILD_DIR=${PARENT_DIR}/build
CMAKE_BIN=${PARENT_DIR}/uml-tool-dependencies/cmake/linux/bin/cmake
TESTS_BIN=${PARENT_DIR}/build/Tests/tests

CONFIG_PARAMS="-DCMAKE_BUILD_TYPE=Release"
if [ ! -z ${COV} ]; then
   CONFIG_PARAMS="${CONFIG_PARAMS} -DCOLLECT_COVERAGE=True"
fi

# Cleanup
rm -rf ${BUILD_DIR} && mkdir ${BUILD_DIR} && cd ${BUILD_DIR}

# Build
if [ ! -z ${BUILD} ]; then
   ${CMAKE_BIN} ${CONFIG_PARAMS} ..
   ${CMAKE_BIN} --build . --target uml-tool -- -j9
   ${CMAKE_BIN} --build . --target tests -- -j9
fi

# Test
if [ ! -z ${TEST} ]; then
   ${TESTS_BIN} --test_root ${ROOT}/Tests --db_path ${ROOT} --gtest_shuffle
fi

# Collect coverage
if [ ! -z ${COV} ]; then
   lcov --directory . --capture --output-file coverage.info
   lcov --remove coverage.info 'Tests/*' '/usr/*' '/opt/*' --output-file coverage.info
   lcov --list coverage.info
   coveralls-lcov --repo-token W5rWWqj02gOXKImGoV7l5jrWCWGe9jrtQ coverage.info
fi
