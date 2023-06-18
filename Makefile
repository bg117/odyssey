KERNEL := odyssey

# use specifically limine v4.20230530.0
CXXFLAGS = \
	-Wall \
	-Wextra \
	-Wno-missing-field-initializers \
	-std=gnu++20 \
	-ffreestanding \
	-fno-stack-protector \
	-fno-stack-check \
	-fno-strict-aliasing \
	-fno-tree-loop-distribute-patterns \
	-fno-PIE \
	-fno-PIC \
	-m64 \
	-march=x86-64 \
	-mabi=sysv \
	-mno-red-zone \
	-mcmodel=kernel \
	-O3 \
	-g

CPPFLAGS := \
	-MMD

LDFLAGS := \
	-nostdlib \
	-static \
	-melf_x86_64 \
	-zmax-page-size=4096 \
	-znoexecstack \
	-Tlinker.lds

CXX := g++
LD := ld

SRCS := $(wildcard src/*.cpp)
FONTS := $(wildcard fonts/*.psf)
OBJS := $(SRCS:.cpp=.o) $(FONTS:.psf=.o)
DEPS := $(SRCS:.cpp=.d)

.PHONY: all hdd-img kernel clean

all: kernel hdd-img

kernel: $(KERNEL)

hdd-img:
	bash -x ./hdd-img.sh

clean:
	rm -rf $(KERNEL) $(OBJS) $(DEPS) $(KERNEL).img

$(KERNEL): $(OBJS)
	ld $(LDFLAGS) $^ -o $@

-include $(DEPS)

fonts/%.o: fonts/%.psf
	objcopy -O elf64-x86-64 -I binary $< $@

src/%.o: src/%.cpp
	g++  $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@
