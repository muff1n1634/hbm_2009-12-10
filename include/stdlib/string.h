#ifndef __STDCXX_STRING_H__
#define __STDCXX_STRING_H__

#include <__internal/size_t.h>

#ifdef __cplusplus
	extern "C" {
#endif

extern void *memset(void *s, int c, size_t n) __attribute__((nothrow));

#ifdef __cplusplus
	}
#endif

#endif // __STDCXX_STRING_H__
