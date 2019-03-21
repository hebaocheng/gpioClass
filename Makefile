##############################
# Makefile
##############################
# !!!=== cross compile...
CROSS_COMPILE ?= arm-poky-linux-gnueabi-

#LD		:= $(CROSS_COMPILE)ld
#CC		:= $(CROSS_COMPILE)gcc
#CPP		:= $(CROSS_COMPILE)cpp
#AR		:= $(CROSS_COMPILE)ar
STRIP	:= $(CROSS_COMPILE)strip

TOP  := $(shell pwd)
mydir :=$(TOP)

CFLAGS := -O2 -Wall -Wfatal-errors -I$(mydir)
#CFLAGS += -mfpu=neon
LFLAGS := 

LIB:= -L $(mydir)/lib 

target=gpio

# ok for c/c++
#SRC = $(wildcard $(SRC_DIR)/*.c)
#OBJ = $(patsubst %.c,%.o, $(SRC))

OBJECTS:= $(mydir)/gpioclass.cpp

all: ${target} install

${target}:$(OBJECTS)
	$(CXX) $(CFLAGS) $(LFLAGS) $(LIB) -o $@ $^
#	$(STRIP) $@

install:
	-cp $(target) ~/tftpboot/

clean:
	rm -f *.o $(target)

