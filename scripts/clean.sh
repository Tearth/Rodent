#!/bin/sh

cd ..

# Bootloader
cd ./boot/build
cmake --build . --target clean
cd ../..