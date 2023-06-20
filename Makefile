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

SRCS_CXX := $(wildcard src/*.cpp)
SRCS_S := $(wildcard src/*.S)
FONTS := $(wildcard fonts/*.psf)
OBJS := $(SRCS_CXX:.cpp=.o) $(SRCS_S:.S=.o) $(FONTS:.psf=.o)
DEPS := $(SRCS_CXX:.cpp=.d)

.PHONY: all kernel hdd-img clean

all: kernel hdd-img

kernel: $(KERNEL)

hdd-img:
	sh -v ./hdd-img.sh

clean:
	rm -rf $(KERNEL) $(OBJS) $(DEPS) $(KERNEL).img root/

$(KERNEL): $(OBJS)
	ld $(LDFLAGS) $^ -o $@

-include $(DEPS)

fonts/%.o: fonts/%.psf
	python3 compile_font.py $< $@

src/%.o: src/%.cpp
	g++ $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

src/%.o: src/%.S
	g++ $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@
