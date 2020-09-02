USE_CONSOLE = yes
USE_TYPEMATIC = yes

# Target device
MCU ?= atmega32u4

# Processor frequency.
F_CPU ?= 16000000
#
# Target architecture
ARCH ?= AVR8

# Target file name (without extension).
TARGET ?= xt2ps2_pro_micro

CONFIG_H ?= config_pro_micro.h

BOARD_SRC := board_pro_micro.c

include $(ROOT_DIR)/common.mk
include $(ROOT_DIR)/avr.mk
