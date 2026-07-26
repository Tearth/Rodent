#!/bin/sh

cd ../boot/bin
openocd -f interface/cmsis-dap.cfg -f target/rp2350-riscv.cfg -c "adapter speed 5000" -c "program ./bootloader.elf verify reset exit"