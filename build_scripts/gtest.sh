#!/bin/bash

git clone https://github.com/google/googletest.git

rm -rf build && mkdir build && cd build

CXX=g++-6
CXX_FLAGS="-std=c++1z -Ofast"
C=gcc-6
C_FLAGS="-std=c1x -Ofast"

cmake ../googletest/ -DCMAKE_CXX_COMPILER=$CXX -DCMAKE_C_COMPILER=$C -DCMAKE_C_FLAGS=$C_FLAGS -DCMAKE_CXX_FLAGS=$CXX_FLAGS
make -j4
sudo make install

cd ..