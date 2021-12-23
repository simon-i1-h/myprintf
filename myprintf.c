#include <stdarg.h>

/* for fputc */
#include <stdio.h>

/* for INT_MAX */
#include <limits.h>

#define countof(arr) (sizeof(arr) / sizeof((arr)[0]))

inline static void
saturating_inc(int *i)
{
	if (*i < INT_MAX)
		(*i)++;
}

int
myvfprintf(FILE *f, const char *fmt, va_list ap)
{
	enum state {
		STATE_NONE,
		STATE_ESCAPE
	} st = STATE_NONE;

	int retlen = 0;

	for (int i = 0; fmt[i] != '\0'; i++) {
		char c = fmt[i];

		switch (st) {
		case STATE_NONE: {
			if (c == '%') {
				st = STATE_ESCAPE;
				break;
			}

			int r = fputc((unsigned char)c, f);
			if (r == EOF)
				return retlen;
			saturating_inc(&retlen);
			if (retlen == INT_MAX)
				return retlen;
			break;
		}
		case STATE_ESCAPE: {
			if (c == '%') {
				int r = fputc((unsigned char)c, f);
				if (r == EOF)
					return retlen;
				saturating_inc(&retlen);
				if (retlen == INT_MAX)
					return retlen;

				st = STATE_NONE;
				break;
			} else if (c == 'd') {
				int d = va_arg(ap, int);

				/* int to string */
				char s[CHAR_BIT * sizeof(int)];
				int i = countof(s);
				s[--i] = '\0';
				if (d == 0) {
					s[--i] = '0';
				} else {
					while (d != 0) {
						s[--i] = d % 10 + '0';
						d /= 10;
					}
				}

				for (; s[i] != '\0'; i++) {
					int r = fputc((unsigned char)s[i], f);
					if (r == EOF)
						return retlen;
					saturating_inc(&retlen);
					if (retlen == INT_MAX)
						return retlen;
				}

				st = STATE_NONE;
				break;
			} else if (c == 'c') {
				/*
				 * va_argは完全に昇格した型のみを受け
				 * 取るらしい。va_argの仕様というより
				 * はC言語の可変長引数の仕様かもしれな
				 * い。
				 */
				char c = (unsigned char)va_arg(ap, int);

				int r = fputc((unsigned char)c, f);
				if (r == EOF)
					return retlen;
				saturating_inc(&retlen);
				if (retlen == INT_MAX)
					return retlen;

				st = STATE_NONE;
				break;
			}

			/* unexpected escape sequence */
			break;
		}
		}
	}

	return retlen;
}

int
myfprintf(FILE *f, const char *fmt, ...)
{
	int ret;
	va_list ap;
	va_start(ap, fmt);
	ret = myvfprintf(f, fmt, ap);
	va_end(ap);
	return ret;
}

int
myprintf(const char *fmt, ...)
{
	int ret;
	va_list ap;
	va_start(ap, fmt);
	ret = myvfprintf(stdout, fmt, ap);
	va_end(ap);
	return ret;
}
