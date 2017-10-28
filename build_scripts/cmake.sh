#!/bin/bash

rm -rf build && mkdir build && cd build

wget https://cmake.org/files/v3.9/cmake-3.9.4.tar.gz
tar -xzf cmake-3.9.4.tar.gz

cd cmake-3.9.4/
CXX=g++-7 ./bootstrap --parallel=4 &> /dev/null
make -j4 &> /dev/null

sudo make install &> /dev/null

cmake --version

cd ../../