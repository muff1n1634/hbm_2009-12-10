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
#define ATTR_FALLTHROUGH		/* no known attribute, but mwcc doesn't seem to care */
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

#define CLAMP(x, low, high)		((x) < (low) ? (low) : (x) > (high) ? (high) : (x))

#define ROUND_UP(x, align)		(((x) + ((align) - 1)) & -(align))

#define IS_ALIGNED(x, align)	(((unsigned long)(x) & ((align) - 1)) == 0)

#define ARRAY_LENGTH(x)			(sizeof (x) / sizeof ((x)[0]))

#define BOOLIFY_TRUE_TERNARY_TYPE(type_, exp_)	((exp_) ? (type_)1 : (type_)0)
#define BOOLIFY_TRUE_TERNARY(exp_)				BOOLIFY_TRUE_TERNARY_TYPE(int, exp_)

#define BOOLIFY_FALSE_TERNARY_TYPE(type_, exp_)	((exp_) ? (type_)0 : (type_)1)
#define BOOLIFY_FALSE_TERNARY(exp_)				BOOLIFY_FALSE_TERNARY_TYPE(int, exp_)

#define BOOLIFY_TERNARY_TYPE					BOOLIFY_TRUE_TERNARY_TYPE
#define BOOLIFY_TERNARY							BOOLIFY_TRUE_TERNARY

// math

#define CONST_FLOAT_PI				3.1415926f

#define DEG_TO_RAD_MULT_CONSTANT	(CONST_FLOAT_PI / 180.0f)
#define RAD_TO_DEG_MULT_CONSTANT	(180.0f / CONST_FLOAT_PI)

#define DEG_TO_RAD(x)			((x) * DEG_TO_RAD_MULT_CONSTANT)
#define RAD_TO_DEG(x)			((x) * RAD_TO_DEG_MULT_CONSTANT)

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
