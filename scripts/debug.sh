#!/bin/sh

openocd \
    -f interface/cmsis-dap.cfg \
    -f target/rp2350-riscv.cfg \
    -c "adapter speed ${ADAPTER_SPEED}"