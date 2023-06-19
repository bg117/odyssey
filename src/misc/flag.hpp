#pragma once

namespace flag
{
constexpr inline bool is_set(uint64_t n, uint64_t flag)
{
  return (n & flag) == flag;
}

inline uint64_t set(uint64_t &n, uint64_t flag)
{
  return n |= flag;
}

inline uint64_t unset(uint64_t &n, uint64_t flag)
{
  return n &= ~flag;
}
} // namespace flag
