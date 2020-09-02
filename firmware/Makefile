
ROOT_DIR = ${CURDIR}
BUILD_DIR = ${CURDIR}/build
COMMON_DIR = ${CURDIR}/common
BOARDS_DIR = ${CURDIR}/boards

MODULES := $(COMMON_DIR) $(COMMON_DIR)/xt $(COMMON_DIR)/util $(COMMON_DIR)/ps2 $(COMMON_DIR)/keyevent $(COMMON_DIR)/keymap $(COMMON_DIR)/xt2ps2 $(COMMON_DIR)/console

VSEARCH_PATH = $(COMMON_DIR):$(BOARDS_DIR):$(COMMON_DIR)/util:$(COMMON_DIR)/xt:$(COMMON_DIR)/keyevent:$(COMMON_DIR)/ps2:$(COMMON_DIR)/keyevent:$(COMMON_DIR)/keymap:$(COMMON_DIR)/xt2ps2:$(COMMON_DIR)/console
VSEARCH_PATH += :$(BOARDS_DIR)/$(CURR_DIR)

export

ifeq ($(words $(MAKECMDGOALS)), 2)
	MK_TARGET = $(lastword $(MAKECMDGOALS))
endif 

.PHONY: all 
all: nano_mega328p pro_micro tiny85_revb

.PHONY: nano_mega328p pro_micro tiny85_revb

nano_mega328p pro_micro tiny85_revb: CURR_DIR = $@
nano_mega328p pro_micro tiny85_revb:
	make -f $(BOARDS_DIR)/$@/$@.mk 

clean:
	rm -r build