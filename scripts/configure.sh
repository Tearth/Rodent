#!/bin/sh

cd ..

mkdir ./bin
mkdir ./build
mkdir ./build/fs

# Bootloader
mkdir ./boot/build
cd ./boot/build
cmake ..