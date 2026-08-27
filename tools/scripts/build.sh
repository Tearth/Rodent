#!/bin/sh

cd ../../build
cmake --build .
cd ..

cp ./bin/kernel.elf ./build/fs/sys/kernel.elf
cp ./etc ./build/fs -r

# Filesystem
mklittlefs -c ./build/fs ./bin/fs_img.bin -b 4096 -s $FS_SIZE >/dev/null
