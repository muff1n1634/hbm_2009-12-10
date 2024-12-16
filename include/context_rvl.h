#ifndef CONTEXT_RVL_H
#define CONTEXT_RVL_H

/* Contains the context of the other Revolution SDK libraries that the WPAD
 * library needs to compile.
 *
 * Ideally, this file's usages should be replaced with the headers in your
 * project that declare these symbols.
 */

__attribute__((weak)) void OSReport(const char *msg, ...);
__attribute__((weak)) void OSPanic(const char *file, int line,
                                   const char *msg, ...);

#if !defined(NDEBUG)
# define OSAssertMessage_FileLine(file_, line_, exp_, ...)	\
	(void)((exp_) || (OSPanic(file_, line_, __VA_ARGS__), 0))
#else
# define OSAssertMessage_FileLine(file_, line_, exp_, ...)	\
	((void)0)
#endif

// defined in terms of OSAssertMessage_FileLine
#define OSAssertMessage_Line(line_, exp_, ...)	\
	OSAssertMessage_FileLine(__FILE__, line_, exp_, __VA_ARGS__)
#define OSAssert_Line(line_, exp_)	\
	OSAssertMessage_FileLine(__FILE__, line_, exp_, "Failed assertion " #exp_)

// OSError
#define OSError_FileLine(file_, line_, ...)	\
	OSPanic(file_, line_, __VA_ARGS__)

// defined in terms of OSError_FileLine
#define OSError_Line(line_, ...)	\
	OSError_FileLine(__FILE__, line_, __VA_ARGS__)

#endif // CONTEXT_RVL_H
