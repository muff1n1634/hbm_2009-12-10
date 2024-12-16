#ifndef CONTEXT_RVL_H
#define CONTEXT_RVL_H

#include <types.h>

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

// Matrix types

typedef f32 Mtx23[2][3];
typedef f32 Mtx[3][4];
typedef f32 Mtx43[4][3];
typedef f32 Mtx44[4][4];

typedef f32 (*Mtx23Ptr)[3];
typedef f32 (*MtxPtr)[4];
typedef f32 (*Mtx43Ptr)[3];
typedef f32 (*Mtx44Ptr)[4];

// not official names, just what i thought would make sense
typedef const f32 (*CMtx23Ptr)[3];
typedef const f32 (*CMtxPtr)[4];
typedef const f32 (*CMtx43Ptr)[3];
typedef const f32 (*CMtx44Ptr)[4];

// Vectors

// [SGLEA4]/GormitiDebug.elf:.debug_info::0x7a261
typedef struct Vec2
{
	f32	x;	// size 0x04, offset 0x00
	f32	y;	// size 0x04, offset 0x04
} Vec2; // size 0x08

// [SGLEA4]/GormitiDebug.elf:.debug_info::0x7a232
typedef struct Vec
{
	f32	x;	// size 0x04, offset 0x00
	f32	y;	// size 0x04, offset 0x04
	f32	z;	// size 0x04, offset 0x08
} Vec; // size 0x0c

typedef Vec2 *Vec2Ptr;
typedef Vec *VecPtr;

typedef const Vec2 *CVec2Ptr;
typedef const Vec *CVecPtr;

void PSMTXIdentity(Mtx m);
void PSMTXCopy(CMtxPtr src, MtxPtr dst);
void PSMTXConcat(CMtxPtr a, CMtxPtr b, MtxPtr ab);


// [SGLEA4]/GormitiDebug.elf:.debug_info::0xba2e6
typedef enum _GXTexFilter
{
	GX_NEAR,
	GX_LINEAR,

	GX_NEAR_MIP_NEAR,
	GX_LIN_MIP_NEAR,

	GX_NEAR_MIP_LIN,
	GX_LIN_MIP_LIN,
} GXTexFilter;

// [SGLEA4]/GormitiDebug.elf:.debug_info::0xba288
typedef enum _GXTexWrapMode
{
	GX_CLAMP,
	GX_REPEAT,
	GX_MIRROR,

	GX_MAX_TEXWRAPMODE
} GXTexWrapMode;

// [SGLEA4]/GormitiDebug.elf:.debug_info::0xba3e2
typedef enum _GXTlutFmt
{
	GX_TL_IA8,
	GX_TL_RGB565,
	GX_TL_RGB5A3,

	GX_MAX_TLUTFMT
} GXTlutFmt;

// [SGLEA4]/GormitiDebug.elf:.debug_info::0xba05c
typedef struct _GXColor
{
	u8	r;	// size 0x01, offset 0x00
	u8	g;	// size 0x01, offset 0x01
	u8	b;	// size 0x01, offset 0x02
	u8	a;	// size 0x01, offset 0x03
} GXColor; // size 0x04

// [SGLEA4]/GormitiDebug.elf:.debug_info::0x2a52c2
typedef struct _GXTexObj
{
	// i guess they didn't want to expose the internals
	byte4_t dummy[8];
} GXTexObj; // size 0x20

// [SGLEA4]/GormitiDebug.elf:.debug_info::0xba189
typedef struct TPLHeader
{
	u16				height;			// size 0x02, offset 0x00
	u16				width;			// size 0x02, offset 0x02
	u32				format;			// size 0x04, offset 0x04
	char			*data;			// size 0x04, offset 0x08
	GXTexWrapMode	wrapS;			// size 0x04, offset 0x0c
	GXTexWrapMode	wrapT;			// size 0x04, offset 0x10
	GXTexFilter		minFilter;		// size 0x04, offset 0x14
	GXTexFilter		magFilter;		// size 0x04, offset 0x18
	f32				LODBias;		// size 0x04, offset 0x1c
	u8				edgeLODEnable;	// size 0x01, offset 0x20
	u8				minLOD;			// size 0x01, offset 0x21
	u8				maxLOD;			// size 0x01, offset 0x22
	u8				unpacked;		// size 0x01, offset 0x23
} TPLHeader; // size 0x24

// [SGLEA4]/GormitiDebug.elf:.debug_info::0xba376
typedef struct TPLClutHeader
{
	u16			numEntries;	// size 0x02, offset 0x00
	u8			unpacked;	// size 0x01, offset 0x02
	byte1_t 	pad8;
	GXTlutFmt	format;		// size 0x04, offset 0x04
	char		*data;		// size 0x04, offset 0x08
} TPLClutHeader; // size 0x0c

// [SGLEA4]/GormitiDebug.elf:.debug_info::0xba141
typedef struct TPLDescriptor
{
	TPLHeader		*textureHeader;	// size 0x04, offset 0x00
	TPLClutHeader	*CLUTHeader;	// size 0x04, offset 0x04
} TPLDescriptor; // size 0x08

// [SGLEA4]/GormitiDebug.elf:.debug_info::0xba0de
typedef struct TPLPalette
{
	u32				versionNumber;		// size 0x04, offset 0x00
	u32				numDescriptors;		// size 0x04, offset 0x04
	TPLDescriptor	*descriptorArray;	// size 0x04, offset 0x08
} TPLPalette; // size 0x0c

#endif // CONTEXT_RVL_H
