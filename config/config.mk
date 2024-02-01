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
CFLAGS_COMMON+= -g -O0 -ffreestanding
CXXFLAGS_COMMON+= -g -O0 -ffreestanding
LDFLAGS_COMMON+= -nostdlib -lgcc -nostartfiles

# Setup include dirs
CFLAGS_COMMON+= -I $(ROOTSRCDIR)/include
CXXFLAGS_COMMON+= -I $(ROOTSRCDIR)/include

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
						-Wswitch-default -Wundef -Winline

# Architecture specific flags
CFLAGS32:=$(CFLAGS32)
CXXFLAGS32:=$(CXXFLAGS32)
LDFLAGS32:=$(LDFLAGS32)
CFLAGS64:=$(CFLAGS64) -mcmodel=kernel -no-pie
CXXFLAGS64:=$(CXXFLAGS64) -mcmodel=kernel -no-pie -z max-page-size=0x1000 -mno-red-zone -mno-mmx -mno-sse -mno-sse2
LDFLAGS64:=$(LDFLAGS64) -mcmodel=kernel -no-pie

# Final flags
CFLAGS64:=$(CFLAGS64) $(CWARNINGS) $(CFLAGS_COMMON)
CXXFLAGS64:=$(CXXFLAGS64) $(CXXWARNINGS) $(CXXFLAGS_COMMON)
LDFLAGS64:=$(LDFLAGS64) $(LDFLAGS_COMMON)
CFLAGS32:=$(CFLAGS32) $(CWARNINGS) $(CFLAGS_COMMON)
CXXFLAGS32:=$(CXXFLAGS32) $(CXXWARNINGS) $(CXXFLAGS_COMMON)
LDFLAGS32:=$(LDFLAGS32) $(LDFLAGS_COMMON)