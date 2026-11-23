#!/bin/bash
# 用板载 CMSIS-DAP 烧录 zero_arm
cd "$(dirname "$0")" || exit 1

OPENOCD=~/tools/xpack-openocd-0.12.0-7/bin/openocd
SCRIPTS=~/tools/xpack-openocd-0.12.0-7/openocd/scripts
ELF=build/Debug/zero_arm.elf

# 清掉可能卡住调试器的残留进程
pkill -f xpack-openocd 2>/dev/null
sleep 0.3

"$OPENOCD" -s "$SCRIPTS" -f openocd.cfg \
  -c "program $ELF verify reset exit"
