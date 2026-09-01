#!/bin/sh

cd ../../build
cmake --build .
cd ..

cp ./bin/kernel.elf ./build/fs/sys/kernel.elf
cp ./bin/app1.elf ./build/fs/usr/app1.elf
cp ./bin/app2.elf ./build/fs/usr/app2.elf
cp ./etc ./build/fs -r

riscv32-unknown-elf-strip ./build/fs/sys/kernel.elf

# Filesystem
mklittlefs -c ./build/fs ./bin/fs_img.bin -b 4096 -s $FS_SIZE >/dev/null
