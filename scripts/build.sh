#!/bin/sh

# Filesystem
rm ../build/fs.lfs
mklittlefs -c ../filesystem ../build/fs.lfs -b 4096 -s 3145728

# Bootloader
rm ../boot/bin/bootloader.elf
cd ../boot/build
cmake --build .