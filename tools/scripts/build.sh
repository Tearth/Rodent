#!/bin/sh

cd ../../build
cmake --build .
cd ..

cp ./bin/kernel.elf ./build/fs/sys/kernel.elf
cp ./etc ./build/fs -r

riscv32-unknown-elf-strip ./build/fs/sys/kernel.elf

# Filesystem
mklittlefs -c ./build/fs ./bin/fs_img.bin -b 4096 -s $FS_SIZE >/dev/null
