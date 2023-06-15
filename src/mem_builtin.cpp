/**
 * @file mem_builtin.c
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
#include <stddef.h>
#include <stdint.h>

extern "C" void *memset(void *dest, int32_t value, size_t count)
{
  uint8_t *p = (uint8_t *)dest;

  for (size_t i = 0; i < count; i++)
  {
    p[i] = (uint8_t)value;
  }

  return dest;
}

extern "C" void *memcpy(void *dest, const void *src, size_t count)
{
  uint8_t *pdest      = (uint8_t *)dest;
  const uint8_t *psrc = (const uint8_t *)src;

  for (size_t i = 0; i < count; i++)
  {
    pdest[i] = psrc[i];
  }

  return dest;
}

extern "C" void *memmove(void *dest, const void *src, size_t count)
{
  uint8_t *pdest      = (uint8_t *)dest;
  const uint8_t *psrc = (const uint8_t *)src;

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

extern "C" int memcmp(const void *ptr1, const void *ptr2, size_t count)
{
  const uint8_t *p1 = (const uint8_t *)ptr1;
  const uint8_t *p2 = (const uint8_t *)ptr2;

  for (size_t i = 0; i < count; i++)
  {
    if (p1[i] != p2[i])
    {
      return p1[i] < p2[i] ? -1 : 1;
    }
  }

  return 0;
}

extern "C" size_t strlen(const char *s)
{
  const char *p = s;
  while (*p)
  {
    ++p;
  }

  return static_cast<size_t>(p - s);
}
