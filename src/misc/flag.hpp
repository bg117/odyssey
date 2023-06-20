#pragma once

#include <cstdint>

namespace flag
{
template<typename T>
constexpr inline bool is_set(T n, uint64_t flag)
{
  return (n & flag) == flag;
}

template<typename T>
inline T set(T &n, uint64_t flag)
{
  return n |= flag;
}

template<typename T>
inline T unset(T &n, uint64_t flag)
{
  return n &= ~flag;
}
} // namespace flag
