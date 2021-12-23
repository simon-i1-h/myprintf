#ifndef MYPRINTF_H
#define MYPRINTF_H

#define PRINTF(fmt, first) __attribute__((format(printf, fmt, first)))

PRINTF(1, 2)
int myprintf(const char *fmt, ...);

#endif /* MYPRINTF_H */
