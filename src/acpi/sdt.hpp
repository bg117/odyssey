#pragma once

#include "../kernel/info.hpp"

#include <cstddef>
#include <cstdint>
#include <cstring>

extern kernel::info INFO;

namespace acpi
{
namespace sdt
{
template <typename SDTType> SDTType *search(const char signature[4])
{
  auto entries =
      (INFO.rsdt.header.length - sizeof(INFO.rsdt.header)) / sizeof(uint32_t);
  for (size_t i = 0; i < entries; i++)
  {
    auto hdr = reinterpret_cast<sdt_header *>(INFO.rsdt.pointers_to_sdts[i] +
                                              INFO.higher_half_direct_offset);
    if (memcmp(hdr->signature, signature, 4) == 0)
    {
      return reinterpret_cast<SDTType *>(hdr);
    }
  }
  return nullptr;
}
} // namespace sdt
} // namespace acpi