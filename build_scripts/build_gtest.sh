#!/bin/bash

git clone https://github.com/google/googletest.git

rm -rf build && mkdir build && cd build

CXX=g++-7
CXX_FLAGS="-std=c++1z -Ofast"
C=gcc-7
C_FLAGS="-std=c1x -Ofast"

cmake ../googletest/ -DCMAKE_CXX_COMPILER=$CXX -DCMAKE_C_COMPILER=$C -DCMAKE_C_FLAGS=$C_FLAGS -DCMAKE_CXX_FLAGS=$CXX_FLAGS &> /dev/null
make -j4 &> /dev/null
sudo make install &> /dev/null

cd ..