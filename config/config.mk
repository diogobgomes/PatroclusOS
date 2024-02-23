# CONFIGURE FILE FOR MAKEFILES - v0.1
#
# Not usefull on it's own, to be used by configure script to generate config.mk.
#
# In principle, you should only interact with the configure script, mess with
# this file at your own risk
#
#
# 2023 Diogo Gomes

# Setup basic flags
CFLAGS_COMMON+= -g -O0 -ffreestanding -std=c2x
CXXFLAGS_COMMON+= -g -O0 -ffreestanding -fno-exceptions -std=gnu++17
LDFLAGS_COMMON+= -nostdlib -lgcc -nostartfiles

# Command line variables
# Leave TRACEMAX off by default
ifeq ($(SET_TRACEMAX),1)
	TRACEMAX:=-DTRACEMAX
endif

# Setup include dirs
CFLAGS_COMMON+= -I $(ROOTSRCDIR)/include $(TRACEMAX)
CXXFLAGS_COMMON+= -I $(ROOTSRCDIR)/include $(TRACEMAX)

# Warnings
CWARNINGS := -Wall -Wextra -pedantic -Wshadow -Wpointer-arith -Wcast-align \
            -Wwrite-strings -Wmissing-prototypes  \
            -Wredundant-decls -Wnested-externs -Winline -Wno-long-long \
            -Wuninitialized -Wconversion -Wstrict-prototypes
CXXWARNINGS:=-pedantic -Wall -Wextra -Wcast-align -Wcast-qual \
	 					-Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 \
						-Winit-self -Wlogical-op  \
						-Wmissing-include-dirs -Wnoexcept -Wold-style-cast \
						-Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-conversion \
						-Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=5 \
						-Wswitch-default -Wundef -Winline -Wno-packed-bitfield-compat

# Architecture specific flags
CFLAGS32:=$(CFLAGS32) -D__is32__
CXXFLAGS32:=$(CXXFLAGS32) -D__is32__
LDFLAGS32:=$(LDFLAGS32) -D__is32__
CFLAGS64:=$(CFLAGS64) -D__is64__ -mcmodel=kernel -no-pie
CXXFLAGS64:=$(CXXFLAGS64) -D__is64__ -mcmodel=kernel -z max-page-size=0x1000 -mno-red-zone -mno-mmx -mno-sse -mno-sse2 -no-pie
LDFLAGS64:=$(LDFLAGS64) -D__is64__ -mcmodel=kernel -z max-page-size=0x1000 -no-pie
CFLAGS16:=$(CFLAGS16) -D__is16__ -m16 -march=i386
CXXFLAGS16:=$(CXXFLAGS16) -D__is16__ -m16 -march=i386
#LDFLAGS16:=$(LDFLAGS16) -m16 -march=i386

# Final flags
CFLAGS64:=$(CFLAGS64) $(CWARNINGS) $(CFLAGS_COMMON)
CXXFLAGS64:=$(CXXFLAGS64) $(CXXWARNINGS) $(CXXFLAGS_COMMON)
LDFLAGS64:=$(LDFLAGS64) $(LDFLAGS_COMMON)
CFLAGS32:=$(CFLAGS32) $(CWARNINGS) $(CFLAGS_COMMON)
CXXFLAGS32:=$(CXXFLAGS32) $(CXXWARNINGS) $(CXXFLAGS_COMMON)
LDFLAGS32:=$(LDFLAGS32) $(LDFLAGS_COMMON)
CFLAGS16:=$(CFLAGS16) $(CWARNINGS) $(CFLAGS_COMMON)
CXXFLAGS16:=$(CXXFLAGS16) $(CXXWARNINGS) $(CXXFLAGS_COMMON)
#LDFLAGS16:=$(LDFLAGS16) $(LDFLAGS_COMMON)