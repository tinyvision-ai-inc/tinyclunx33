# Copyright (c) 2024 tinyVision.ai Inc.
# SPDX-License-Identifier: Apache-2.0

zephyr_sources(soc.c)
zephyr_sources(${ZEPHYR_BASE}/soc/common/riscv-privileged/soc_irq.S)
zephyr_sources(${ZEPHYR_BASE}/soc/common/riscv-privileged/vector.S)

zephyr_include_directories(.)

# Use native RISC-V linker script
set(SOC_LINKER_SCRIPT ${ZEPHYR_BASE}/include/zephyr/arch/riscv/common/linker.ld CACHE INTERNAL "")

# Append extra snippets to it
zephyr_linker_sources(RAM_SECTIONS ${CMAKE_CURRENT_SOURCE_DIR}/linker.ld)
zephyr_code_relocate_ifdef(CONFIG_FLASH_TINYVISION FILES ../../../drivers/flash/flash_tinyvision.c LOCATION RAM NOKEEP)
