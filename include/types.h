#ifndef TYPES_H
#define TYPES_H

#include <stddef.h>
#include <stdint.h>

// Null pointers

#undef NULL
#define NULL			0

#define nullptr			NULL

// Fixed-width types

typedef uint8_t			u8;
typedef uint16_t		u16;
typedef uint32_t		u32;
typedef uint64_t		u64;

typedef  int8_t			s8;
typedef  int16_t		s16;
typedef  int32_t		s32;
typedef  int64_t		s64;

// Floating-point types

typedef float			f32;
typedef double			f64;

// Booleans

typedef int				BOOL;
#define TRUE			1
#define FALSE			0

// Character types

#if !defined(__cplusplus) || __cplusplus < 201103L
typedef unsigned char	char8_t;
typedef uint_least16_t	char16_t;
typedef uint_least32_t	char32_t;
#else
// they are keywords
#endif

// Other types

typedef unsigned long int	byte4_t;
typedef unsigned short int	byte2_t;
typedef unsigned char		byte1_t;

typedef byte1_t				byte_t;

typedef unsigned int		register_t;

#endif // TYPES_H
