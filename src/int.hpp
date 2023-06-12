#pragma once

namespace convert
{
char *int_to_string(long long num, char *buf, int radix);
char *uint_to_string(unsigned long long num, char *buf, int radix);
} // namespace convert
