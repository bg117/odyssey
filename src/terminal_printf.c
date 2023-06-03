/**
 * @file terminal_printf.c
 * @author bg117 (bg117@sooosilly.xyz)
 * @brief terminal_printf implementation
 * @version 0.1
 * @date 2023-06-03
 *
 * This file contains the implementation for terminal_printf.
 */

#include "int.h"
#include "psf.h"
#include "terminal.h"

#include <ctype.h>
#include <stdarg.h>

char *fmtupr(char *str);
int strlen(const char *str);

#define PF_FMT_SPEC 1
#define PF_FMT_NORMAL 2

#define PF_LEN_SSHORT 1
#define PF_LEN_SHORT 2
#define PF_LEN_DEFAULT 3
#define PF_LEN_LONG 4
#define PF_LEN_LLONG 5

#define ZERO_PAD(buf)                                 \
  do                                                  \
  {                                                   \
    if (zero_pad)                                     \
    {                                                 \
      for (int i = 0; i < pad_len - strlen(buf); ++i) \
      {                                               \
        terminal_print_char('0');                     \
      }                                               \
    }                                                 \
  } while (0)

#define PF_FMT_RECEIVE_ARG_S(flen, ap, buf, base)      \
  switch (flen)                                        \
  {                                                    \
  case PF_LEN_SSHORT:                                  \
    int_to_string((char)va_arg(ap, int), buf, base);   \
    break;                                             \
  case PF_LEN_SHORT:                                   \
    int_to_string((short)va_arg(ap, int), buf, base);  \
    break;                                             \
  case PF_LEN_DEFAULT:                                 \
    int_to_string(va_arg(ap, int), buf, base);         \
    break;                                             \
  case PF_LEN_LONG:                                    \
    long_to_string(va_arg(ap, long), buf, base);       \
    break;                                             \
  case PF_LEN_LLONG:                                   \
    llong_to_string(va_arg(ap, long long), buf, base); \
    break;                                             \
  }                                                    \
  ZERO_PAD(buf)

#define PF_FMT_RECEIVE_ARG_U(flen, ap, buf, base)                        \
  switch (flen)                                                          \
  {                                                                      \
  case PF_LEN_SSHORT:                                                    \
    uint_to_string((unsigned char)va_arg(ap, unsigned int), buf, base);  \
    break;                                                               \
  case PF_LEN_SHORT:                                                     \
    uint_to_string((unsigned short)va_arg(ap, unsigned int), buf, base); \
    break;                                                               \
  case PF_LEN_DEFAULT:                                                   \
    uint_to_string(va_arg(ap, unsigned int), buf, base);                 \
    break;                                                               \
  case PF_LEN_LONG:                                                      \
    ulong_to_string(va_arg(ap, unsigned long), buf, base);               \
    break;                                                               \
  case PF_LEN_LLONG:                                                     \
    ullong_to_string(va_arg(ap, unsigned long long), buf, base);         \
    break;                                                               \
  }                                                                      \
  ZERO_PAD(buf)

void terminal_printf(const char *fmt, ...)
{
  va_list ap;
  va_start(ap, fmt);

  int i    = 0;
  int pfs  = PF_FMT_NORMAL;
  int flen = PF_LEN_DEFAULT;

  int space_pad = 0;
  int zero_pad  = 0;
  int pad_len   = 0;

  while (*fmt != '\0')
  {
    if (*fmt == '%')
    {
      pfs = PF_FMT_SPEC;
      ++fmt;
    }

    if (pfs == PF_FMT_SPEC)
    {

      switch (*fmt)
      {
      case '0':
        zero_pad = 1;
        ++fmt;
        while (*fmt >= '0' && *fmt <= '9')
        {
          pad_len *= 10;
          pad_len += *fmt - '0';
          ++fmt;
        }
        continue;
      case ' ':
        space_pad = 1;
        ++fmt;
        while (*fmt >= '0' && *fmt <= '9')
        {
          pad_len *= 10;
          pad_len += *fmt - '0';
          ++fmt;
        }
        continue;
      case 'h':
        if (flen == PF_LEN_DEFAULT)
          flen = PF_LEN_SHORT;
        else if (flen == PF_LEN_SHORT)
          flen = PF_LEN_SSHORT;
        ++fmt;
        continue;
      case 'l':
        if (flen == PF_LEN_DEFAULT)
          flen = PF_LEN_LONG;
        else if (flen == PF_LEN_SHORT)
          flen = PF_LEN_LLONG;
        ++fmt;
        continue;
      case '%':
        terminal_print_char('%');
        break;
      case 's': {
        char *str = va_arg(ap, char *);
        if (space_pad)
        {
          for (int i = 0; i < pad_len - strlen(str); ++i)
            terminal_print_char(' ');
        }
        terminal_print_string(str);
      }
      break;
      case 'i':
      case 'd': {
        char num[32];

        PF_FMT_RECEIVE_ARG_S(flen, ap, num, 10);
        terminal_print_string(num);
      }
      break;

      case 'u': {
        char num[32];

        PF_FMT_RECEIVE_ARG_U(flen, ap, num, 10);
        terminal_print_string(num);
      }
      break;

      case 'x': {
        char num[32];

        PF_FMT_RECEIVE_ARG_U(flen, ap, num, 16);
        terminal_print_string(num);
      }
      break;

      case 'X': {
        char num[32];

        PF_FMT_RECEIVE_ARG_U(flen, ap, num, 16);
        terminal_print_string(fmtupr(num));
      }
      break;

      case 'p': {
        char num[32];

        terminal_print_string("0x");
        PF_FMT_RECEIVE_ARG_U(flen, ap, num, 16);
        terminal_print_string(num);
      }
      break;

      case 'o': {
        char num[32];

        PF_FMT_RECEIVE_ARG_U(flen, ap, num, 16);
        terminal_print_string(num);
      }
      break;

      default:
        terminal_print_char('%');
        terminal_print_char(*fmt);
        break;
      }
    }
    else
    {
      terminal_print_char(*fmt);
    }

    pfs       = PF_FMT_NORMAL;
    flen      = PF_LEN_DEFAULT;
    space_pad = 0;
    zero_pad  = 0;
    pad_len   = 0;

    ++i;
    ++fmt;
  }

  va_end(ap);
}

char *fmtupr(char *str)
{
  char *ptr = str;
  while (*ptr != '\0')
  {
    if (*ptr >= 'a' && *ptr <= 'z')
      *ptr -= 32;
    ++ptr;
  }
  return str;
}

int strlen(const char *str)
{
  int len = 0;
  while (*str != '\0')
  {
    ++len;
    ++str;
  }
  return len;
}