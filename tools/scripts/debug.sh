#!/bin/sh

openocd \
    -f interface/cmsis-dap.cfg \
    -f target/${ADAPTER_TARGET}.cfg \
    -c "adapter speed ${ADAPTER_SPEED}"