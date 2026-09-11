#!/bin/sh

screen -d -m openocd \
    -f interface/cmsis-dap.cfg \
    -f target/${ADAPTER_TARGET}.cfg \
    -c "adapter speed ${ADAPTER_SPEED}" &
sleep 0.25