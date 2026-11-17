#!/bin/bash
# 用 OpenOCD + 板载 CMSIS-DAP 烧录
set -e
cd "$(dirname "$0")"

# 先清掉可能占着 DAP 的残留进程，否则会报 unable to open CMSIS-DAP device
pkill -f "xpack-openocd" 2>/dev/null || true
pkill -f "pyocd" 2>/dev/null || true
sleep 0.5

OOCD="$HOME/tools/xpack-openocd-0.12.0-7/bin/openocd"
ELF="build/Debug/zero_robot_arm.elf"
[ -x "$OOCD" ] || { echo "找不到 OpenOCD: $OOCD"; exit 1; }
[ -f "$ELF" ]  || { echo "找不到 $ELF，先在 CLion 里 Build 一次"; exit 1; }

"$OOCD" -s "$HOME/tools/xpack-openocd-0.12.0-7/openocd/scripts" \
  -f openocd.cfg \
  -c "program $ELF verify reset exit"
