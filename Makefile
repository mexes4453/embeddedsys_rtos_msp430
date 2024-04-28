# DIRECTORIES

PATH_ROOT=/home/mexes/Documents/ES
PATH_DEV=$(PATH_ROOT)/dev/msp430/msp430-gcc-9.3.1.11_linux64
PATH_DEV_BIN=$(PATH_DEV)/bin
PATH_DEV_LIB=$(PATH_DEV)/lib
PATH_DEV_HEADER="$(PATH_DEV_LIB)/gcc/msp430-elf/9.3.1/plugin/include/msp430/"
DIR_INCLUDE=$(PATH_ROOT)/dev/msp430/msp430-gcc-support-files/include
DIR_PROJECT=$(PATH_ROOT)/projects/03_MSP430_rtos/01_uart
DIR_LINKER=$(DIR_INCLUDE)
DIR_BUILD = build
DIR_OBJ = $(DIR_BUILD)/obj
DIR_BIN = $(DIR_BUILD)/bin

# DIR DEBUGGER
DIR_TI_CCS=$(PATH_ROOT)/IDE/ccs1200/ccs
DIR_DEBUGGER=$(DIR_TI_CCS)/ccs_base/DebugServer
DIR_DEBUG_BIN=$(DIR_DEBUGGER)/bin
DIR_DEBUG_DRIVER=$(DIR_DEBUGGER)/drivers

# TOOLCHAIN
CC = $(PATH_DEV_BIN)/msp430-elf-gcc
DEBUG = LD_LIBRARY_PATH=$(DIR_DEBUG_DRIVER) $(DIR_DEBUG_BIN)/mspdebug

# FILES
TARGET = $(DIR_BIN)/blink

SRCS = 	main.c \
    led.c \
	bsp.c \
	bsp_uart.c \
	bsp_timer.c \
	ring_buffer.c \
	serial.c \


OBJ_NAMES = $(SRCS:.c=.o)
OBJS = $(patsubst %, $(DIR_OBJ)/%, $(OBJ_NAMES))

# FLAGS
MCU = msp430f5529
WFLAGS = -Wall -Wextra -Werror -Wshadow
CFLAGS = -mmcu=$(MCU) $(WFLAGS) $(addprefix -I,$(DIR_INCLUDE)) -Og -g -I$(DIR_PROJECT)
LDFLAGS = -mmcu=$(MCU) $(addprefix -L,$(DIR_LINKER))


# BUILD
# LINKING
$(TARGET): $(OBJS)
	@mkdir -p $(dir $@)
	$(CC) $(LDFLAGS) $^ -o $@

# COMPILING - OBJ FILES
$(DIR_OBJ)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC)  $(CFLAGS) -c -o $@ $^

# PHONY
.PHONY : all clean flash

all: $(TARGET)

clean:
	rm -r $(DIR_BUILD)

flash: $(TARGET)
	$(DEBUG) tilib "prog $(TARGET)"
	
	
