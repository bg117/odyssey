#pragma once

namespace convert
{
char *int_to_string(long long num, char *buf, int radix);
char *uint_to_string(unsigned long long num, char *buf, int radix);

char *string_to_upper(char *buf);
char *string_to_lower(char *buf);
} // namespace convert
