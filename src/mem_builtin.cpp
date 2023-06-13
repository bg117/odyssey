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

__attribute__((optnone)) extern "C" void *memset(void *dest, int32_t value,
                                                 size_t count)
{
  uint8_t *dst = (uint8_t *)dest;
  uint64_t val = (uint8_t)value;
  size_t i;

  // Set bytes until the destination is aligned to 8 bytes
  while ((uintptr_t)dst & 7 && count > 0)
  {
    *dst++ = val;
    --count;
  }

  uint64_t *dst_aligned = (uint64_t *)dst;
  uint64_t value64      = val | (val << 8) | (val << 16) | (val << 24) |
                     (val << 32) | (val << 40) | (val << 48) | (val << 56);

  // Fill memory with 8-byte chunks using movsq
  for (i = 0; i < count / 8; ++i)
  {
    *dst_aligned++ = value64;
  }

  // Fill any remaining bytes
  dst = (uint8_t *)dst_aligned;
  for (i = 0; i < count % 8; ++i)
  {
    *dst++ = val;
  }

  return dest;
}

__attribute__((optnone)) extern "C" void *memcpy(void *dest, const void *src,
                                                 size_t count)
{
  uint8_t *dst             = (uint8_t *)dest;
  const uint8_t *src_bytes = (const uint8_t *)src;

  if (((uintptr_t)dst & 7) == 0 && ((uintptr_t)src & 7) == 0 && count >= 8)
  {
    uint64_t *dst_aligned       = (uint64_t *)dst;
    const uint64_t *src_aligned = (const uint64_t *)src_bytes;

    // Copy 8-byte chunks using movsq
    for (size_t i = 0; i < count / 8; ++i)
    {
      *dst_aligned++ = *src_aligned++;
    }

    dst       = (uint8_t *)dst_aligned;
    src_bytes = (const uint8_t *)src_aligned;
  }

  // Copy any remaining bytes
  for (size_t i = 0; i < count; ++i)
  {
    *dst++ = *src_bytes++;
  }

  return dest;
}

__attribute__((optnone)) extern "C" void *memmove(void *dest, const void *src,
                                                  size_t count)
{
  uint8_t *dst             = (uint8_t *)dest;
  const uint8_t *src_bytes = (const uint8_t *)src;

  if (dst < src_bytes && ((uintptr_t)dst & 7) == 0 &&
      ((uintptr_t)src_bytes & 7) == 0 && count >= 8)
  {
    uint64_t *dst_aligned       = (uint64_t *)dst;
    const uint64_t *src_aligned = (const uint64_t *)src_bytes;

    // Forward copy 8-byte chunks using movsq
    for (size_t i = 0; i < count / 8; ++i)
    {
      *dst_aligned++ = *src_aligned++;
    }

    dst       = (uint8_t *)dst_aligned;
    src_bytes = (const uint8_t *)src_aligned;
  }
  else if (dst > src_bytes && ((uintptr_t)dst & 7) == 0 &&
           ((uintptr_t)src_bytes & 7) == 0 && count >= 8)
  {
    uint64_t *dst_aligned       = (uint64_t *)(dst + count - 8);
    const uint64_t *src_aligned = (const uint64_t *)(src_bytes + count - 8);

    // Backward copy 8-byte chunks using movsq
    for (size_t i = 0; i < count / 8; ++i)
    {
      *(dst_aligned - i) = *(src_aligned - i);
    }

    dst       = (uint8_t *)dst_aligned;
    src_bytes = (const uint8_t *)src_aligned;
  }

  // Copy any remaining bytes
  if (dst < src_bytes)
  {
    for (size_t i = 0; i < count; ++i)
    {
      *dst++ = *src_bytes++;
    }
  }
  else if (dst > src_bytes)
  {
    for (size_t i = 0; i < count; ++i)
    {
      *(dst + count - i - 1) = *(src_bytes + count - i - 1);
    }
  }

  return dest;
}

__attribute__((optnone)) extern "C" int memcmp(const void *ptr1,
                                               const void *ptr2, size_t count)
{
  const uint8_t *src1 = (const uint8_t *)ptr1;
  const uint8_t *src2 = (const uint8_t *)ptr2;

  // Compare 8-byte chunks as long as memory is aligned
  while (((uintptr_t)src1 & 7) == 0 && count >= 8)
  {
    uint64_t *aligned_src1 = (uint64_t *)src1;
    uint64_t *aligned_src2 = (uint64_t *)src2;

    if (*aligned_src1 != *aligned_src2)
    {
      // Mismatch found, determine which byte differs
      for (int i = 0; i < 8; ++i)
      {
        if (src1[i] != src2[i])
        {
          return (int)src1[i] - (int)src2[i];
        }
      }
    }

    src1 += 8;
    src2 += 8;
    count -= 8;
  }

  // Compare remaining bytes
  for (size_t i = 0; i < count; ++i)
  {
    if (*src1 != *src2)
    {
      return (int)*src1 - (int)*src2;
    }

    ++src1;
    ++src2;
  }

  return 0;
}

__attribute__((optnone)) extern "C" size_t strlen(const char *s)
{
  const char *p = s;
  while (*p)
  {
    ++p;
  }

  return static_cast<size_t>(p - s);
}
