# SPDX-License-Identifier: Apache-2.0

set(SHIELD adafruit_2_8_tft_touch_v2)

cmake_minimum_required(VERSION 3.20.0)

# would be dope to define DBOARD_ROOT in here

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(snowboard_press)

target_sources(app PRIVATE src/main.c)

# Add custom linker section to relocate framebuffers to SRAM2
zephyr_linker_sources_ifdef(CONFIG_LV_Z_VBD_CUSTOM_SECTION
  SECTIONS dc_ram.ld)