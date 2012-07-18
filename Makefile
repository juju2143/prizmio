ifeq ($(strip $(FXCGSDK)),)
export FXCGSDK := $(abspath ../../)
endif

include $(FXCGSDK)/common/prizm_rules

AR = sh3eb-elf-ar
GCC = sh3eb-elf-gcc
GCCFLAGS = $(MACHDEP) -Os -I$(FXCGSDK)/include -Wl,-static -Wl,-gc-sections -lc -lfxcg -lgcc
LD = sh3eb-elf-ld
LDFLAGS = $(MACHDEP) -T$(FXCGSDK)/common/prizm.ld -Wl,-static -Wl,-gc-sections
OBJCOPY = sh3eb-elf-objcopy
LIB = libprizmio.a
DISTDIR = $(FXCGSDK)/lib
vpath %.a $(DISTDIR)
OBJS = console.o screen.o registry.o uart.o

all: $(LIB)

%.o: %.c
	$(GCC) $(GCCFLAGS) -c $<

%.elf: %.o
	$(LD) $(LDFLAGS) $^ -o $@

$(LIB): $(OBJS)
	$(AR) rcs "$(DISTDIR)/$(LIB)" $^
	cp -u prizmio.h "$(FXCGSDK)/include"

clean:
	rm -rf *.o *.elf *.a
	rm -f "$(DISTDIR)/$(LIB)"
	rm -f "$(FXCGSDK)/include/prizmio.h"
