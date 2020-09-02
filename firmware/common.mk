
ifeq ($(USE_CONSOLE),yes)
	OPT_DEFS += -DUSE_CONSOLE
endif

ifeq ($(USE_TYPEMATIC),yes)
	OPT_DEFS += -DUSE_TYPEMATIC
endif

SRC_DIR   := $(MODULES)
SRC       := $(foreach sdir,$(SRC_DIR),$(wildcard $(sdir)/*.c))
SRC       += $(BOARDS_DIR)/$(CURR_DIR)/$(BOARD_SRC)

OPT_DEFS  += -DCONFIG_H=$(CONFIG_H)

# Search Path
VPATH += $(VSEARCH_PATH)
