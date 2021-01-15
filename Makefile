CC         = gcc
LD         = ld
KERNEL_HDD = disk.hdd
OS_NAME    = testos

TARGET := $(OS_NAME).elf

CFLAGS = -O2 -pipe -Wall -Wextra

#----------------NO TOUCHY---------------------

CHARDFLAGS := $(CFLAGS)               \
	-std=gnu99                     \
	-masm=intel                    \
	-fno-pic                       \
	-mno-sse                       \
	-mno-sse2                      \
	-mno-mmx                       \
	-mno-80387                     \
	-mno-red-zone                  \
	-mcmodel=kernel                \
	-ffreestanding                 \
	-fno-stack-protector           \
	-Isrc/                         \

LDHARDFLAGS := $(LDFLAGS)        \
	-static                   \
	-nostdlib                 \
	-no-pie                   \
	-z max-page-size=0x1000   \
	-T src/linker.ld

LDINTERNALFLAGS :=  \
	-Tsrc/linker.ld \
	-static     \
	-nostdlib   \
	-no-pie
 
INTERNALCFLAGS  :=           \
	-I.                  \
	-ffreestanding       \
	-fno-stack-protector \
	-fno-pic             \
	-mno-80387           \
	-mno-mmx             \
	-mno-3dnow           \
	-mno-sse             \
	-mno-sse2            \
	-mcmodel=kernel      \
	-mno-red-zone		 \
	-masm=intel

#-------------------------

CFILES := $(shell find ./ -type f -name '*.c')
OBJ    := $(CFILES:.c=.o)
 

.PHONY: clean all run

all: clean $(TARGET) $(KERNEL_HDD) run

run: $(KERNEL_HDD)
	qemu-system-x86_64 -m 2G -hda $(KERNEL_HDD)

$(TARGET): $(OBJ)
	$(LD) $(LDINTERNALFLAGS) $(OBJ) -o $@
 
%.o: %.c
	$(CC) $(CFLAGS) $(INTERNALCFLAGS) -c $< -o $@

$(KERNEL_HDD): boot/limine-install 
	rm -f $(KERNEL_HDD)
	dd if=/dev/zero bs=1M count=0 seek=64 of=$(KERNEL_HDD)
	parted -s $(KERNEL_HDD) mklabel gpt
	parted -s $(KERNEL_HDD) mkpart primary 2048s 100%
	echfs-utils -g -p0 $(KERNEL_HDD) quick-format 512
	echfs-utils -g -p0 $(KERNEL_HDD) import $(OS_NAME).elf $(OS_NAME).elf
	echfs-utils -g -p0 $(KERNEL_HDD) import boot/limine.cfg  limine.cfg
	boot/limine-install $(KERNEL_HDD)

clean:
	rm -rf $(TARGET) $(OBJ) $(KERNEL_HDD)
