#ifndef ODYSSEY_INT_H
#define ODYSSEY_INT_H

char *int_to_string(int num, char *buf, int radix);
char *uint_to_string(unsigned int num, char *buf, int radix);
char *long_to_string(long num, char *buf, int radix);
char *llong_to_string(long long num, char *buf, int radix);
char *ulong_to_string(unsigned long num, char *buf, int radix);
char *ullong_to_string(unsigned long long num, char *buf, int radix);

#endif