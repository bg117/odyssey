cmake_minimum_required(VERSION 3.10)
project(Odyssey)
enable_language(C ASM)

set(FONT_OBJ_FILES "")
file(GLOB FONT_FILES ${CMAKE_CURRENT_SOURCE_DIR}/fonts/*.psf)
foreach(FILENAME ${FONT_FILES})
    get_filename_component(FILENAME_ONLY ${FILENAME} NAME)
    set(OBJECT ${CMAKE_CURRENT_BINARY_DIR}/${FILENAME_ONLY}.o)
    add_custom_command(
        OUTPUT ${OBJECT}
        COMMAND 
            python3
            ${CMAKE_CURRENT_SOURCE_DIR}/compile_font.py
            ${FILENAME}
            ${OBJECT}
        DEPENDS ${FILENAME}
    )
    list(APPEND FONT_OBJ_FILES ${OBJECT})
endforeach(FILENAME)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wno-missing-field-initializers -ffreestanding -fno-stack-protector -fno-stack-check -fno-strict-aliasing -fno-PIE -fno-PIC -m64 -march=x86-64 -mabi=sysv -mno-red-zone -mcmodel=kernel -MMD")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -Og -g -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -O3")
set(CMAKE_CXX_LINK_FLAGS "-nostdlib -static -melf_x86_64 -zmax-page-size=0x1000 -znoexecstack -T${CMAKE_CURRENT_SOURCE_DIR}/linker.lds")

set(CMAKE_CXX_LINK_EXECUTABLE
    "ld <CMAKE_CXX_LINK_FLAGS> <OBJECTS> -o <TARGET> <LINK_LIBRARIES>")

add_executable(odyssey
    src/_start.cpp
    src/convert.cpp
    src/cpu_exception.cpp
    src/framebuffer.cpp
    src/gdt.cpp
    src/heap.cpp
    src/icxxabi.cpp
    src/idt.cpp
    src/isr.cpp
    src/kmain.cpp
    src/mem_builtin.cpp
    src/operator_new.cpp
    src/pmm.cpp
    src/vmm.cpp
    src/isr_indirection.S
    src/misc/flag.hpp
    src/misc/config.hpp
    src/misc/convert.hpp
    src/misc/log.hpp
    src/misc/types.hpp
    src/misc/round.hpp
    src/graphics/framebuffer.hpp
    src/graphics/psf.hpp
    src/kernel/info.hpp
    src/kernel/cpu_exception.hpp
    src/memory/heap.hpp
    src/memory/paging_structure.hpp
    src/memory/vmm.hpp
    src/memory/pmm.hpp
    src/low_level/isr.hpp
    src/low_level/gdt.hpp
    src/low_level/idt.hpp

    ${FONT_OBJ_FILES}
)

add_custom_target(
    hdd-img
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/hdd-img.sh
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    DEPENDS odyssey
)
