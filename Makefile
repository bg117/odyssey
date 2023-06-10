KERNEL := odyssey

# use specifically limine v4.20230530.0
CFLAGS := \
	-Wall \
	-Wextra \
	-std=gnu99 \
	-ffreestanding \
	-fno-stack-protector \
	-fno-stack-check \
	-fno-PIE \
	-fno-PIC \
	-m64 \
	-march=x86-64 \
	-mabi=sysv \
	-mno-red-zone \
	-mcmodel=kernel \
	-g

CPPFLAGS := \
	-MMD

LDFLAGS := \
	-nostdlib \
	-static \
	-Wl,-melf_x86_64 \
	-Wl,-zmax-page-size=4096 \
	-Tlinker.lds \
	-nopie \
	-fuse-ld=lld

CC := clang
LD := clang

SRCS := $(wildcard src/*.c)
FONTS := $(wildcard fonts/*.psf)
OBJS := $(SRCS:.c=.o) $(FONTS:.psf=.o)
DEPS := $(SRCS:.c=.d)

.PHONY: all hdd-img kernel clean

all: kernel hdd-img

hdd-img:
	sh ./hdd-img.sh

kernel: $(KERNEL)

clean:
	rm -rf $(KERNEL) $(OBJS) $(DEPS) $(KERNEL).img

$(KERNEL): $(OBJS)
	$(LD) $^ $(LDFLAGS) -o $@

-include $(DEPS)

src/%.o: src/%.c
	$(CC) $< $(CFLAGS) $(CPPFLAGS) -c -o $@

fonts/%.o: fonts/%.psf
	objcopy -O elf64-x86-64 -I binary $< $@
