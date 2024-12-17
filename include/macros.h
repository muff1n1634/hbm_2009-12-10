#ifndef MACROS_H
#define MACROS_H

// macro stuff

#define STR_(x)					#x
#define STR(x)					STR_(x)

// keywords

#ifndef decltype
# define decltype				__decltype__
#endif

// attributes

#define ATTR_ALIGN(x)			__attribute__((aligned(x)))
#define ATTR_NOINLINE			__attribute__((never_inline))
#define ATTR_UNUSED				__attribute__((unused))
#define ATTR_WEAK				__attribute__((weak))

#ifdef __MWERKS__
# define ATTR_ADDRESS(x)			: x
#else
# define ATTR_ADDRESS(x)
#endif

// helpers

#define MIN(x, y)				((x) < (y) ? (x) : (y))

#define ROUND_UP(x, align)		(((x) + ((align) - 1)) & -(align))

#define IS_ALIGNED(x, align)	(((unsigned long)(x) & ((align) - 1)) == 0)

#define ARRAY_LENGTH(x)			(sizeof (x) / sizeof ((x)[0]))

// Range-for loop macros

#if defined(__cplusplus)

// Sort of like C++11's range for, except now it's a macro for NW4R iterators
# define NW4R_RANGE_FOR(it_, list_)	\
	for (decltype((list_).GetBeginIter()) (it_) = (list_).GetBeginIter(); (it_) != (list_).GetEndIter(); ++(it_))

/* This macro specifically is for the for loops which declare an
 * Iterator currIt = it++; in the body, so that it does not get incremented
 * twice.
 */
# define NW4R_RANGE_FOR_NO_AUTO_INC(it_, list_)	\
	for (decltype((list_).GetBeginIter()) (it_) = (list_).GetBeginIter(); (it_) != (list_).GetEndIter();)

# define NW4HBM_RANGE_FOR				NW4R_RANGE_FOR
# define NW4HBM_RANGE_FOR_NO_AUTO_INC	NW4R_RANGE_FOR_NO_AUTO_INC

#endif // defined(__cplusplus)

#endif // MACROS_H
