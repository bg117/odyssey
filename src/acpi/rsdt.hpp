#pragma once

#include "sdt_header.hpp"

namespace acpi
{
struct __attribute__((packed)) rsdt
{
  sdt_header header;
  uint32_t *pointers_to_sdts;

  rsdt() = default;

  rsdt(uint32_t address)
  {
    auto *header_ptr = reinterpret_cast<sdt_header *>(address);
    header = *header_ptr;
    pointers_to_sdts =
        reinterpret_cast<uint32_t *>(address + sizeof(sdt_header));
  }
};
} // namespace acpi