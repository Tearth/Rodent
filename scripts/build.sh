#!/bin/sh

# Bootloader
cd ../boot
rm ./bin/bootloader.elf
cd ./build
cmake --build .
cd ../..

# Filesystem
rm ./build/fs/fs_img.bin
mklittlefs -c ./build/fs ./bin/fs_img.bin -b 4096 -s 0x20000

# Final image
cp ./boot/bin/bootloader.elf ./bin/bootloader.elf