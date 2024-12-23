#ifndef RVL_SDK_HBM_NW4HBM_MATH_ARITHMETIC_HPP
#define RVL_SDK_HBM_NW4HBM_MATH_ARITHMETIC_HPP

/*******************************************************************************
 * headers
 */

#include <types.h>

#if 0
#include <revolution/OS/OSFastCast.h>
#endif

#include <context_rvl.h>

/*******************************************************************************
 * classes and functions
 */

namespace nw4hbm { namespace math
{
	inline f32 FSelect(f32 cond, f32 ifPos, f32 ifNeg)
	{
		f32 ret;

		asm ("fsel %0, %1, %2, %3"
			: "=f"(ret)
			: "f"(cond), "f"(ifPos), "f"(ifNeg)
		);

		return ret;
	}

	inline f32 FAbs(f32 x)
	{
		f32 ret;

		asm ("fabs %0, %1"
			: "=f"(ret)
			: "f"(x)
		);

		return ret;
	}

	inline u16 F32ToU16(f32 x)
	{
		u16 rval;

		OSf32tou16(&x, &rval);

		return rval;
	}

	inline f32 U16ToF32(u16 x)
	{
		f32 rval;

		OSu16tof32(&x, &rval);

		return rval;
	}
}} // namespace nw4hbm::math

#endif // RVL_SDK_HBM_NW4HBM_MATH_ARITHMETIC_HPP
