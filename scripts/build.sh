#!/bin/sh

cd ..

# Bootloader
cd ./boot/build
cmake --build .
cd ../..

# Kernel
cd ./kernel/build
cmake --build .
cd ../..
cp ./kernel/bin/kernel.elf ./build/fs/kernel.elf

# Filesystem
mklittlefs -c ./build/fs ./bin/fs_img.bin -b 4096 -s $FS_SIZE >/dev/null

# Final image
cp ./boot/bin/bootloader.elf ./bin/bootloader.elf