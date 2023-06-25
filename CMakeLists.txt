cmake_minimum_required(VERSION 3.13)
project(Odyssey)
enable_language(C CXX ASM)

set(FONT_OBJ_FILES "")
file(GLOB FONT_FILES ${CMAKE_CURRENT_SOURCE_DIR}/fonts/*.psf)
foreach(FILENAME ${FONT_FILES})
    get_filename_component(FILENAME_ONLY ${FILENAME} NAME)
    set(OBJECT ${CMAKE_CURRENT_BINARY_DIR}/${FILENAME_ONLY}.o)
    add_custom_command(
        OUTPUT ${OBJECT}
        COMMAND 
            python3
            ${CMAKE_CURRENT_SOURCE_DIR}/scripts/compile-font.py
            ${FILENAME}
            ${OBJECT}
        DEPENDS ${FILENAME}
    )
    list(APPEND FONT_OBJ_FILES ${OBJECT})
endforeach(FILENAME)

set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 20)

set(C_CXX_COMMON_FLAGS -Wall -Wextra -Wno-missing-field-initializers -ffreestanding -fno-stack-protector -fno-stack-check -fno-strict-aliasing -fno-PIE -fno-PIC -m64 -march=x86-64 -mabi=sysv -mno-red-zone -mcmodel=kernel -masm=intel)
set(C_CXX_COMMON_LINK_FLAGS -nostdlib -static -Wl,-melf_x86_64 -zmax-page-size=0x1000 -znoexecstack -T${CMAKE_CURRENT_SOURCE_DIR}/linker.lds)

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

target_compile_options(odyssey PUBLIC
    ${C_CXX_COMMON_FLAGS}
    $<$<CONFIG:DEBUG>: ${C_CXX_COMMON_FLAGS_DEBUG}>
    $<$<CONFIG:RELEASE>: ${C_CXX_COMMON_FLAGS_RELEASE}>
)

target_link_options(odyssey PUBLIC
    ${C_CXX_COMMON_LINK_FLAGS}
)

add_custom_target(
    hdd-img
    COMMAND python3 ${CMAKE_CURRENT_SOURCE_DIR}/scripts/make-efi-img.py ${CMAKE_CURRENT_BINARY_DIR}/odyssey.img ${CMAKE_CURRENT_BINARY_DIR}/odyssey ${CMAKE_CURRENT_SOURCE_DIR}/limine.cfg
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    DEPENDS odyssey
)
