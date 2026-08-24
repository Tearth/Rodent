#!/bin/sh

cd ../..
openocd \
  -f interface/cmsis-dap.cfg \
  -f target/rp2350-riscv.cfg \
  -c "adapter speed ${ADAPTER_SPEED}" \
  -c "program ./bin/bootloader.elf" \
  -c "program ./bin/fs_img.bin ${FS_BASE_ADDR}" \
  -c "reset halt" \
  -c "shutdown"