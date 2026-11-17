#!/bin/bash
# 用板载 CMSIS-DAP 烧录（CLion 的 ST-LINK 方案不认这块板）
set -e
cd "$(dirname "$0")"
ELF="build/Debug/zero_robot_arm.elf"
[ -f "$ELF" ] || { echo "找不到 $ELF，先在 CLion 里 Build 一次"; exit 1; }
python3 -m pyocd flash -t stm32f407ve "$ELF"
echo "烧录完成，已复位运行"
