#include "misc/convert.hpp"

namespace
{
constexpr char ALNUM_RANGE[] =
    "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz";
}

namespace convert
{
char *int_to_string(long long num, char *buf, int radix)
{
  char *rc;
  char *ptr;
  char *low;
  // Check for supported base.
  if (radix < 2 || radix > 36)
  {
    *buf = '\0';
    return buf;
  }
  rc = ptr = buf;
  // Set '-' for negative decimals.
  if (num < 0 && radix == 10)
  {
    *ptr++ = '-';
  }
  // Remember where the numbers start.
  low = ptr;
  // The actual conversion.
  do
  {
    // Modulo is negative for negative value. This trick makes abs()
    // unnecessary.
    *ptr++ = ALNUM_RANGE[35 + num % radix];
    num /= radix;
  } while (num);
  // Terminating the string.
  *ptr-- = '\0';
  // Invert the numbers.
  while (low < ptr)
  {
    char tmp = *low;
    *low++   = *ptr;
    *ptr--   = tmp;
  }
  return rc;
}

char *uint_to_string(unsigned long long num, char *buf, int radix)
{
  char *rc;
  char *ptr;
  char *low;
  // Check for supported base.
  if (radix < 2 || radix > 36)
  {
    *buf = '\0';
    return buf;
  }
  rc = ptr = buf;

  // Remember where the numbers start.
  low = ptr;
  // The actual conversion.
  do
  {
    // Modulo is negative for negative value. This trick makes abs()
    // unnecessary.
    *ptr++ = ALNUM_RANGE[35 + num % radix];
    num /= radix;
  } while (num);
  // Terminating the string.
  *ptr-- = '\0';
  // Invert the numbers.
  while (low < ptr)
  {
    char tmp = *low;
    *low++   = *ptr;
    *ptr--   = tmp;
  }
  return rc;
}

char *string_to_upper(char *buf)
{
  char *p = buf;
  while (*p)
  {
    if (*p >= 'a' && *p <= 'z')
    {
      *p -= 32;
    }
    ++p;
  }

  return buf;
}

char *string_to_lower(char *buf)
{
  char *p = buf;
  while (*p)
  {
    if (*p >= 'A' && *p <= 'Z')
    {
      *p += 32;
    }
    ++p;
  }

  return buf;
}
} // namespace convert
