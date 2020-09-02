USE_CONSOLE = yes
USE_TYPEMATIC = yes

# Target device
MCU ?= atmega328p

# Processor frequency.
F_CPU ?= 16000000
#
# Target architecture
ARCH ?= AVR8

# Target file name (without extension).
TARGET ?= xt2ps2_nano_mega328p

CONFIG_H ?= config_nano_mega328p.h

BOARD_SRC := board_nano_mega328p.c

include $(ROOT_DIR)/common.mk
include $(ROOT_DIR)/avr.mk
