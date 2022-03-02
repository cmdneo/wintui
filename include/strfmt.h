#ifndef INCLUDE_STRFMT_H
#define INCLUDE_STRFMT_H

#include <wchar.h>

void sf_pad(wchar_t *s, wchar_t *buf, wchar_t pad, int size, int mode);


enum sf_fmt {
	SF_LEFT = 0,
	SF_RIGHT,
	SF_CENTER,
};

#endif
