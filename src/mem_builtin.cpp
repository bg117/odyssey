/**
 * @file mem_builtin.cpp
 * @author bg117 (bg117@sooosilly.xyz)
 * @brief string.h memory functions implementation
 * @version 0.1
 * @date 2023-06-03
 *
 * This file contains implementations for memcpy, memmove, and memset.
 * We cannot use the default compiler implementations as we are
 * compiling in freestanding mode.
 *
 * These definitions allow us to use special syntax that make use of
 * these functions.
 */
#include <cstdint>
#include <cstddef>

extern "C"
{
void *memset(void *dest, const int32_t value, const size_t count)
{
  auto *p = static_cast<uint8_t *>(dest);

  for (size_t i = 0; i < count; i++)
  {
    p[i] = static_cast<uint8_t>(value);
  }

  return dest;
}

void *memcpy(void *dest, const void *src, const size_t count)
{
  auto *pdest      = static_cast<uint8_t *>(dest);
  const auto *psrc = static_cast<const uint8_t *>(src);

  for (size_t i = 0; i < count; i++)
  {
    pdest[i] = psrc[i];
  }

  return dest;
}

void *memmove(void *dest, const void *src, const size_t count)
{
  auto *pdest      = static_cast<uint8_t *>(dest);
  const auto *psrc = static_cast<const uint8_t *>(src);

  if (src > dest)
  {
    for (size_t i = 0; i < count; i++)
    {
      pdest[i] = psrc[i];
    }
  }
  else if (src < dest)
  {
    for (size_t i = count; i > 0; i--)
    {
      pdest[i - 1] = psrc[i - 1];
    }
  }

  return dest;
}

int memcmp(const void *ptr1, const void *ptr2, const size_t count)
{
  const auto *p1 = static_cast<const uint8_t *>(ptr1);
  const auto *p2 = static_cast<const uint8_t *>(ptr2);

  for (size_t i = 0; i < count; i++)
  {
    if (p1[i] != p2[i])
    {
      return p1[i] < p2[i] ? -1 : 1;
    }
  }

  return 0;
}

size_t strlen(const char *s)
{
  const char *p = s;
  while (*p)
  {
    ++p;
  }

  return static_cast<size_t>(p - s);
}
}
