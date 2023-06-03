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

void *memcpy(void *dest, const void *src, size_t n)
{
    uint8_t *pdest = dest;
    const uint8_t *psrc = src;

    for (size_t i = 0; i < n; i++)
    {
        pdest[i] = psrc[i];
    }

    return dest;
}

void *memmove(void *dest, const void *src, size_t n)
{
    uint8_t *pdest = dest;
    const uint8_t *psrc = src;

    if (src > dest)
    {
        memcpy(dest, src, n);
    }
    else if (src < dest)
    {
        // copy backwards if dest is higher than src
        for (size_t i = n - 1; i >= 0; i--)
        {
            pdest[i] = psrc[i];
        }
    }

    return dest;
}

int memcmp(const void *s1, const void *s2, size_t n)
{
    const uint8_t *p1 = s1;
    const uint8_t *p2 = s2;

    for (size_t i = 0; i < n; i++)
    {
        if (p1[i] < p2[i])
        {
            return -1;
        }
        else if (p1[i] > p2[i])
        {
            return 1;
        }
    }

    return 0;
}