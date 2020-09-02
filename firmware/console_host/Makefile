WORKDIR = %cd%

TOOL_PATH = 
CC = gcc.exe
AR = ar.exe
LD = g++.exe

INC = ../common ../common/xt ../common/ps2 ../common/console ../common/util 
CFLAGS =  -Wall
LIB_DIR = 
LIB =  ftd2xx.lib
LDFLAGS = 

INC_DIRS =   $(addprefix -I,$(INC))
OBJ_DIR = obj
OUT = bin/xt2ps2_console_host.exe
COMMON_DIR = ../common

SRCS = main.c \
	   console_expansion.c \
	   xth_xcvr_con_exp.c \
	   con_exp_test.c 

all: out

OBJS = ${notdir ${SRCS:.c=.o}}


out: before $(OBJS)
	$(LD) $(LIB_DIR) -o $(OUT) $(OBJS)  $(LDFLAGS) $(LIB)

# the "common" object files
$(OBJ_DIR)/%.o : %.c 
	$(CC) $(CFLAGS) $(INC_DIRS) -c -o $@ $<

.PHONY: before 


