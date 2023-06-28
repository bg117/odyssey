#pragma once

#include "sdt_header.hpp"

namespace acpi
{
struct __attribute__((packed)) madt
{
  sdt_header header;
  uint32_t local_controller_address;
  uint32_t flags;
  uint8_t entries_begin;
};
} // namespace acpi