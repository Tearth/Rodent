#!/bin/sh

cd ..
openocd \
  -f interface/cmsis-dap.cfg \
  -f target/rp2350-riscv.cfg \
  -c "adapter speed 30000" \
  -c "program ./bin/bootloader.elf" \
  -c "program ./bin/fs_img.bin 0x10100000" \
  -c "reset halt" \
  -c "shutdown"