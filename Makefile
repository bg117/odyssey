KERNEL := odyssey

# use specifically limine v4.20230530.0
COMMON_FLAGS = \
	-Wall \
	-Wextra \
	-std=gnu99 \
	-ffreestanding \
	-fno-stack-protector \
	-fno-stack-check \
	-fno-strict-aliasing \
	-fno-PIE \
	-fno-PIC \
	-m64 \
	-march=x86-64 \
	-mabi=sysv \
	-mno-red-zone \
	-mcmodel=kernel \
	-O3 \
	-g
CFLAGS := $(COMMON_FLAGS) -std=gnu17
CXXFLAGS := $(COMMON_FLAGS) -std=gnu++20

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
CXX := clang++
LD := clang

SRCS_C := $(wildcard src/*.c)
SRCS_CXX := $(wildcard src/*.cpp)
FONTS := $(wildcard fonts/*.psf)
OBJS := $(SRCS_CXX:.cpp=.cpp.o) $(FONTS:.psf=.o)
DEPS := $(SRCS_C:.c=.c.d) $(SRCS_CXX:.cpp=.cpp.d)

.PHONY: all hdd-img kernel clean

all: kernel hdd-img

hdd-img:
	bash -x ./hdd-img.sh

kernel: $(KERNEL)

clean:
	rm -rf $(KERNEL) $(OBJS) $(DEPS) $(KERNEL).img

$(KERNEL): $(OBJS)
	$(LD) $^ $(LDFLAGS) -o $@

-include $(DEPS)

src/%.c.o: src/%.c
	$(CC) $< $(CFLAGS) $(CPPFLAGS) -c -o $@

src/%.cpp.o: src/%.cpp
	$(CXX) $< $(CXXFLAGS) $(CPPFLAGS) -c -o $@

fonts/%.o: fonts/%.psf
	objcopy -O elf64-x86-64 -I binary $< $@
