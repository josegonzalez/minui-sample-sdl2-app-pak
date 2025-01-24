ifeq (,$(CROSS_COMPILE))
$(error missing CROSS_COMPILE for this toolchain)
endif

PLATFORM ?= tg5040
LD_LIBRARY_PATH = /root/workspace/platform/$(PLATFORM)/lib/
PREFIX = /root/workspace/platform/$(PLATFORM)

-include minui/workspace/$(PLATFORM)/platform/makefile.env
SDL?=SDL

TARGET = main
PRODUCT = $(TARGET)

INCDIR = -I. -Iplatform/$(PLATFORM)/include/ -Iminui/workspace/all/common/ -Iminui/workspace/$(PLATFORM)/platform/
SOURCE = $(TARGET).c minui/workspace/all/common/scaler.c minui/workspace/all/common/utils.c minui/workspace/all/common/api.c minui/workspace/$(PLATFORM)/platform/platform.c

CC = $(CROSS_COMPILE)gcc
CFLAGS   = $(ARCH) -fomit-frame-pointer
CFLAGS  += $(INCDIR) -DPLATFORM=\"$(PLATFORM)\" -DUSE_$(SDL) -Ofast -std=gnu99
FLAGS = -L$(LD_LIBRARY_PATH) -ldl -lmsettings $(LIBS) -l$(SDL) -l$(SDL)_image -l$(SDL)_ttf -lpthread -lm -lz

all: minui $(PREFIX)/include/msettings.h
	LD_LIBRARY_PATH=$(LD_LIBRARY_PATH) $(CC) $(SOURCE) -o $(PRODUCT)-$(PLATFORM) $(CFLAGS) $(FLAGS)

clean:
	rm -rf $(PRODUCT)-$(PLATFORM)

minui:
	git clone https://github.com/shauninman/MinUI minui

platform/$(PLATFORM)/lib:
	mkdir -p platform/$(PLATFORM)/lib

platform/$(PLATFORM)/include:
	mkdir -p platform/$(PLATFORM)/include

# PREFIX is the path to the workspace
$(PREFIX)/include/msettings.h: platform/$(PLATFORM)/lib platform/$(PLATFORM)/include
	cd /root/workspace/minui/workspace/$(PLATFORM)/libmsettings && make
