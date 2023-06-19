#pragma once

#include "idt.hpp"

#include <cstdint>

namespace low_level
{
namespace isr
{
struct register_info
{
  uint64_t cr4;
  uint64_t cr3;
  uint64_t cr2;
  uint64_t cr0;
  uint64_t r15;
  uint64_t r14;
  uint64_t r13;
  uint64_t r12;
  uint64_t r11;
  uint64_t r10;
  uint64_t r9;
  uint64_t r8;
  uint64_t rdi;
  uint64_t rsi;
  uint64_t rbp;
  uint64_t rsp;
  uint64_t rdx;
  uint64_t rcx;
  uint64_t rbx;
  uint64_t rax;
};

struct interrupt_info
{
  register_info registers;
  uint8_t vector;
  uint8_t error_code;
};

using handler = void (*)(interrupt_info *);

void initialize(idt::entry idt[256]);
void set_handler(uint8_t vector, handler fn);
} // namespace isr
} // namespace low_level
