#!/bin/sh

cd ..
mkdir ./bin
mkdir ./build
mkdir ./build/fs
cd ./build

cmake .. -DCMAKE_BUILD_TYPE=${BUILD_PROFILE} -DCMAKE_TOOLCHAIN_FILE=./toolchain/${BUILD_TOOLCHAIN}.toolchain.cmake