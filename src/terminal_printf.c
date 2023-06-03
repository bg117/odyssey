/**
 * @file terminal_printf.c
 * @author bg117 (bg117@sooosilly.xyz)
 * @brief terminal_printf implementation
 * @version 0.1
 * @date 2023-06-03
 *
 * This file contains the implementation for terminal_printf.
 */

#include "terminal.h"
#include "psf.h"
#include "int.h"

#include <stdarg.h>
#include <ctype.h>

char *fmtupr(char *str);
int strlen(const char *str);

#define PF_FMT_SPEC 1
#define PF_FMT_NORMAL 2

#define PF_LEN_SSHORT 1
#define PF_LEN_SHORT 2
#define PF_LEN_DEFAULT 3
#define PF_LEN_LONG 4
#define PF_LEN_LLONG 5

#define PF_FMT_RECEIVE_ARG(flen, ap, s, buf, base)            \
    switch (flen)                                             \
    {                                                         \
    case PF_LEN_SSHORT:                                       \
        int_to_string((s char)va_arg(ap, s int), buf, base);  \
        break;                                                \
    case PF_LEN_SHORT:                                        \
        int_to_string((s short)va_arg(ap, s int), buf, base); \
        break;                                                \
    case PF_LEN_DEFAULT:                                      \
        int_to_string(va_arg(ap, s int), buf, base);          \
        break;                                                \
    case PF_LEN_LONG:                                         \
        int_to_string(va_arg(ap, s long), buf, base);         \
        break;                                                \
    case PF_LEN_LLONG:                                        \
        int_to_string(va_arg(ap, s long long), buf, base);    \
        break;                                                \
    }                                                         \
    if (zero_pad)                                             \
    {                                                         \
        for (int i = 0; i < pad_len - strlen(buf); ++i)       \
        {                                                     \
            terminal_print_char('0');                         \
        }                                                     \
    }

void terminal_printf(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);

    int i = 0;
    int pfs = PF_FMT_NORMAL;
    int flen = PF_LEN_DEFAULT;

    int space_pad = 0;
    int zero_pad = 0;
    int pad_len = 0;

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
            case 's':
            {
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
            case 'd':
            {
                char num[32];

                PF_FMT_RECEIVE_ARG(flen, ap, signed, num, 10);
                terminal_print_string(num);
            }
            break;

            case 'u':
            {
                char num[32];

                PF_FMT_RECEIVE_ARG(flen, ap, unsigned, num, 10);
                terminal_print_string(num);
            }
            break;

            case 'x':
            {
                char num[32];

                PF_FMT_RECEIVE_ARG(flen, ap, unsigned, num, 16);
                terminal_print_string(num);
            }
            break;

            case 'X':
            {
                char num[32];

                PF_FMT_RECEIVE_ARG(flen, ap, unsigned, num, 16);
                terminal_print_string(fmtupr(num));
            }
            break;

            case 'p':
            {
                char num[32];

                terminal_print_string("0x");
                PF_FMT_RECEIVE_ARG(flen, ap, unsigned, num, 16);
                terminal_print_string(num);
            }

            case 'o':
            {
                char num[32];

                PF_FMT_RECEIVE_ARG(flen, ap, unsigned, num, 16);
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

        pfs = PF_FMT_NORMAL;
        flen = PF_LEN_DEFAULT;
        space_pad = 0;
        zero_pad = 0;
        pad_len = 0;

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