USE_CONSOLE = no
USE_TYPEMATIC = yes

# Target device
MCU ?= attiny85

# Processor frequency.
F_CPU ?= 8000000
#
# Target architecture
ARCH ?= AVR8

# Target file name (without extension).
TARGET ?= xt2ps2_tiny85

CONFIG_H ?= config_tiny85_revb.h

BOARD_SRC := board_tiny85_revb.c

include $(ROOT_DIR)/common.mk
include $(ROOT_DIR)/avr.mk
