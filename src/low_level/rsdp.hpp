#pragma once

#include <cstdint>

namespace low_level
{
struct rsdp
{
  char signature[8];
  uint8_t checksum;
  char oem_id[6];
  uint8_t revision;
  uint32_t rsdt_address;
};
}
