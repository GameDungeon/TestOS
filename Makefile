CC         = gcc
LD         = ld
KERNEL_HDD = disk.hdd
OS_NAME    = testos

CFLAGS = -O2 -pipe -Wall -Wextra

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

.PHONY: clean all run

all: clean kernel/$(OS_NAME).elf $(KERNEL_HDD) run

run: $(KERNEL_HDD)
	qemu-system-x86_64 -m 2G -hda $(KERNEL_HDD)

kernel/$(OS_NAME).elf:
	$(MAKE) -C $(OS_NAME)

$(KERNEL_HDD): boot/limine-install 
	rm -f $(KERNEL_HDD)
	dd if=/dev/zero bs=1M count=0 seek=64 of=$(KERNEL_HDD)
	parted -s $(KERNEL_HDD) mklabel gpt
	parted -s $(KERNEL_HDD) mkpart primary 2048s 100%
	echfs-utils -g -p0 $(KERNEL_HDD) quick-format 512
	echfs-utils -g -p0 $(KERNEL_HDD) import kernel/$(OS_NAME).elf $(OS_NAME).elf
	echfs-utils -g -p0 $(KERNEL_HDD) import boot/limine.cfg  limine.cfg
	boot/limine-install $(KERNEL_HDD)

kernel/$(OS_NAME).elf:
	cd kernel && make 
	cd ../

clean:
	rm -f $(KERNEL_HDD)
	cd kernel && make clean
	cd ../