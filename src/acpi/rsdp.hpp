#pragma once

#include <cstdint>

namespace acpi
{
struct rsdp
{
  char signature[8];
  uint8_t checksum;
  char oem_id[6];
  uint8_t revision;
  uint32_t rsdt_address;

  rsdp() = default;

  rsdp(void *ptr)
  {
    auto *rsdp_ptr = reinterpret_cast<rsdp *>(ptr);
    *this = *rsdp_ptr;
  }

  rsdp(uintptr_t address) : rsdp(reinterpret_cast<void *>(address))
  {
  }
};
}
