# written by lu yongjiang  on 08/27/11
# modified by liu jirang on 20140228

NAME    = Uart
CC      = csky-abiv2-elf-gcc 
AS      = csky-abiv2-elf-as
AR      = csky-abiv2-elf-ar
LD      = csky-abiv2-elf-ld
DUMP    = csky-abiv2-elf-objdump
OBJCOPY = csky-abiv2-elf-objcopy
ROOTDIR  = $(shell pwd)

INCLUDEDIRS = -I$(ROOTDIR)/include  -I$(ROOTDIR)/drivers -I./

CFLAGS += -mcpu=ck810 -g3 -c -O0 -mlittle-endian -mno-multiple-stld 
CFLAGS += -Wall
CFLAGS += -fno-strict-aliasing -fno-builtin -fomit-frame-pointer  
CFLAGS += -frename-registers 
CFLAGS += $(INCLUDEDIRS)

LDFLAGS = -EL 
ASFLAGS = $(CFLAGS) -Wa,--gstabs

#CFLAGS += -D CONFIG_CKCPU_MMU=1
#CFLAGS += -D CONFIG_QEMU=1

INCFILE = ck810.h

export CC AS AR LD DUMP OBJCOPY CFLAGS LDFLAGS ASFLAGS INCLUDEDIRS ROOTDIR

LDLIBS	=  -lm -lc -lgcc
DEPEND	= .dep

.PHONY : all

all    : childdir $(NAME).elf 

childdir:	 
	@for dir in $(SUB_DIRS); do\
		$(MAKE) -C $$dir || exit 1;\
	done

SUB_DIRS = $(shell ls  -F | grep /$ | grep -v Debug | grep -v Release | \
		  grep -v CVS | grep -v arch | grep -v include | grep -v obj \
		| sed "s/\///g")

SUBDIR_FILES = $(foreach dir, $(SUB_DIRS), $(dir)/$(dir).o)

SSRC = $(wildcard *.S) arch/entry.S
CSRC = $(wildcard *.c)

OBJECTS =  $(SSRC:%.S=%.o) $(CSRC:%.c=%.o) 


arch/crt0.o: arch/crt0.S
	$(CC)  $(ASFLAGS) -o $@ $<
%.o:%.c  
	$(CC)  $(CFLAGS)  -o $@ $<
%.o:%.S 
	$(CC)  $(ASFLAGS) -o $@ $<

$(NAME).elf: arch/crt0.o $(OBJECTS)  $(SUBDIR_FILES) ckcpu.ld 
	$(CC)  -mcpu=ck810 -mno-multiple-stld -mlittle-endian -nostartfiles -nostdlib \
	       "`csky-abiv2-elf-gcc -mcpu=ck810 -mlittle-endian -print-file-name=crti.o`" \
	       "`csky-abiv2-elf-gcc -mcpu=ck810 -mlittle-endian -print-file-name=crtbegin.o`" \
	       -T./ckcpu.ld -o $(NAME).elf arch/crt0.o  $(OBJECTS) $(SUBDIR_FILES) $(LDLIBS) -Wl,-Map=Uart.map \
	       "`csky-abiv2-elf-gcc -mcpu=ck810 -mlittle-endian -print-file-name=crtend.o`" \
	       "`csky-abiv2-elf-gcc -mcpu=ck810 -mlittle-endian -print-file-name=crtn.o`"	
	$(DUMP) -xd $(NAME).elf > $(NAME).asm

ifneq ($(wildcard .dep),)
include $(DEPEND)
endif

dep: 
	$(SHELL) -ec '$(CC) -MM $(CFLAGS) $(INCLUDEDIRS) $(CSRC) $(SSRC) \
        | sed '\''s@\(.*\)\.o[ :]@\1.o:@g'\'' > $(DEPEND)'
	@for dir in $(SUB_DIRS); do\
                $(MAKE) -C $$dir dep;\
        done


clean:
	rm -f arch/crt0.o
	rm -f $(OBJECTS)
	rm -f $(NAME).elf
	rm -f $(NAME).asm
	@for dir in $(SUB_DIRS); do\
		$(MAKE) -C $$dir clean;\
	done

.PHONY : debug-make

DEBUG_VARS = 	SUB_DIRS \
		SUBDIR_FILES\
		INCLUDEDIRS \
		CSRC \
		SSRC \
		OBJECTS \
		ASFLAGS \
		CFLAGS
#:
#: debug-make -- Print a list of Makefile variables

debug-make:
	@$(foreach var, $(DEBUG_VARS), echo $(var)=$($(var)) ; )

