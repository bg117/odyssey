KERNEL := odyssey

# use specifically limine v4.20230530.0
CXXFLAGS = \
	-Wall \
	-Wextra \
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
	-mno-sse \
	-mno-sse2 \
	-mno-mmx \
	-mno-avx \
	-mno-avx2 \
	-mcmodel=kernel \
	-O0 \
	-g

CPPFLAGS := \
	-MMD

LDFLAGS := \
	-nostdlib \
	-static \
	-melf_x86_64 \
	-zmax-page-size=4096 \
	-Tlinker.lds \

CXX := g++
LD := ld

SRCS_CXX := $(wildcard src/*.cpp)
FONTS := $(wildcard fonts/*.psf)
OBJS := $(SRCS_CXX:.cpp=.o) $(FONTS:.psf=.o)
DEPS := $(SRCS_CXX:.cpp=.d)

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

src/%.o: src/%.cpp
	$(CXX) $< $(CXXFLAGS) $(CPPFLAGS) -c -o $@

fonts/%.o: fonts/%.psf
	objcopy -O elf64-x86-64 -I binary $< $@
