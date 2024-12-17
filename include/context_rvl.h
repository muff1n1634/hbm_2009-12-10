#ifndef CONTEXT_RVL_H
#define CONTEXT_RVL_H

#include <macros.h>
#include <types.h>

// IWYU pragma: begin_exports

#ifdef __cplusplus
	extern "C" {
#endif

/* Contains the context of the other Revolution SDK libraries that the WPAD
 * library needs to compile.
 *
 * Ideally, this file's usages should be replaced with the headers in your
 * project that declare these symbols.
 */

ATTR_WEAK void OSReport(const char *msg, ...);
ATTR_WEAK void OSPanic(const char *file, int line, const char *msg, ...);

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

static inline f32 __OSu16tof32(const u16 *arg)
{
	f32 ret;
	__asm__ ("psq_l %0, 0(%1), 1, 3" : "=f"(ret) : "r"(arg));

	return ret;
}

static inline u16 __OSf32tou16(f32 arg)
{
	f32 a;
	f32 *ptr = &a;

	u16 r;
	__asm__ ("psq_st %0, 0(%1), 1, 3" :: "f"(arg), "r"(ptr));
	r = *(u16 *)ptr;

	return r;
}

static inline void OSf32tou16(const f32 *in, u16 *out)
{
	*out = __OSf32tou16(*in);
}

static inline void OSu16tof32(const u16 *in, f32 *out)
{
	*out = __OSu16tof32(in);
}

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

#if defined(NDEBUG)
# define MTXInverse	PSMTXInverse
# define MTXTrans	PSMTXTrans

# define MTXMultVec	PSMTXMultVec
#else
# define MTXInverse	C_MTXInverse
# define MTXTrans	C_MTXTrans

# define MTXMultVec	C_MTXMultVec
#endif

void PSMTXIdentity(Mtx m);
void PSMTXCopy(CMtxPtr src, MtxPtr dst);
void PSMTXConcat(CMtxPtr a, CMtxPtr b, MtxPtr ab);

void PSMTXInverse(CMtxPtr src, MtxPtr inv);
void C_MTXInverse(CMtxPtr src, MtxPtr inv);

void PSMTXTrans(Mtx m, f32, f32, f32);
void C_MTXTrans(Mtx m, f32, f32, f32);

void PSMTXMultVec(CMtxPtr m, CVecPtr src, VecPtr dst);
void C_MTXMultVec(CMtxPtr m, CVecPtr src, VecPtr dst);

// some are gormiti dwarf, some are i spy dwarf, some are the dwarf 1 megadump

// TODO: get a specific source (name, game id)
// TODO: check library version of game that has this
// dwarfv1_megadump.txt:1220088-1220092
typedef enum _GXAttnFn
{
	GX_AF_SPEC,
	GX_AF_SPOT,

	GX_AF_NONE
} GXAttnFn;

// [SGLEA4]/GormitiDebug.elf:.debug_info::0x438266
typedef enum _GXAttr
{
	GX_VA_PNMTXIDX,

	GX_VA_TEX0MTXIDX,
	GX_VA_TEX1MTXIDX,
	GX_VA_TEX2MTXIDX,
	GX_VA_TEX3MTXIDX,
	GX_VA_TEX4MTXIDX,
	GX_VA_TEX5MTXIDX,
	GX_VA_TEX6MTXIDX,
	GX_VA_TEX7MTXIDX,

	GX_VA_POS,
	GX_VA_NRM,

	GX_VA_CLR0,
	GX_VA_CLR1,

	GX_VA_TEX0,
	GX_VA_TEX1,
	GX_VA_TEX2,
	GX_VA_TEX3,
	GX_VA_TEX4,
	GX_VA_TEX5,
	GX_VA_TEX6,
	GX_VA_TEX7,

	GX_POS_MTX_ARRAY,
	GX_NRM_MTX_ARRAY,
	GX_TEX_MTX_ARRAY,
	GX_LIGHT_ARRAY,

	GX_VA_NBT,

	GX_VA_MAX_ATTR,
	GX_VA_NULL = 0xff
} GXAttr;

// [SPQE7T]/ISpyD.elf:.debug_info::0x268dc0
typedef enum _GXAttrType
{
	GX_NONE,

	GX_DIRECT,

	GX_INDEX8,
	GX_INDEX16,
} GXAttrType;

// [SGLEA4]/GormitiDebug.elf:.debug_info::0x45d222
typedef enum _GXBlendFactor
{
	GX_BL_ZERO,
	GX_BL_ONE,

	GX_BL_SRCCLR,
	GX_BL_INVSRCCLR,

	GX_BL_SRCALPHA,
	GX_BL_INVSRCALPHA,

	GX_BL_DSTALPHA,
	GX_BL_INVDSTALPHA,

	GX_BL_DSTCLR		= GX_BL_SRCCLR,
	GX_BL_INVDSTCLR		= GX_BL_INVSRCCLR
} GXBlendFactor;

// [SGLEA4]/GormitiDebug.elf:.debug_info::0x48f667
typedef enum _GXBlendMode
{
	GX_BM_NONE,

	GX_BM_BLEND,
	GX_BM_LOGIC,
	GX_BM_SUBTRACT,

	GX_MAX_BLENDMODE
} GXBlendMode;

// [SGLEA4]/GormitiDebug.elf:.debug_info::0x4070a5
typedef enum _GXChannelID
{
	GX_COLOR0,
	GX_COLOR1,

	GX_ALPHA0,
	GX_ALPHA1,

	GX_COLOR0A0,
	GX_COLOR1A1,

	GX_COLOR_ZERO,

	GX_ALPHA_BUMP,
	GX_ALPHA_BUMPN,

	GX_COLOR_NULL = 0xff
} GXChannelID;

// [SGLEA4]/GormitiDebug.elf:.debug_info::0x401562
typedef enum _GXColorSrc
{
	GX_SRC_REG,
	GX_SRC_VTX,
} GXColorSrc;

// [SPQE7T]/ISpyD.elf:.debug_info::0x268cce
typedef enum _GXCompCnt
{
	GX_POS_XY = 0,
	GX_POS_XYZ,

	GX_NRM_XYZ = 0,
	GX_NRM_NBT,
	GX_NRM_NBT3,

	GX_CLR_RGB = 0,
	GX_CLR_RGBA,

	GX_TEX_S = 0,
	GX_TEX_ST,
} GXCompCnt;

// [SPQE7T]/ISpyD.elf:.debug_info::0x25e1b3
typedef enum _GXCompType
{
	GX_U8,
	GX_S8,
	GX_U16,
	GX_S16,
	GX_F32,

	GX_RGB565 = 0,
	GX_RGB8,
	GX_RGBX8,
	GX_RGBA4,
	GX_RGBA6,
	GX_RGBA8,
} GXCompType;

// [SPQE7T]/ISpyD.elf:.debug_info::0x286e6d
typedef enum _GXCullMode
{
	GX_CULL_NONE,
	GX_CULL_FRONT,
	GX_CULL_BACK,
	GX_CULL_ALL
} GXCullMode;

// TODO: get a specific source (name, game id)
// TODO: check library version of game that has this
// dwarfv1_megadump.txt:1220081-1220085
typedef enum _GXDiffuseFn
{
	GX_DF_NONE,

	GX_DF_SIGN,
	GX_DF_CLAMP,
} GXDiffuseFn;

// [SPQE7T]/ISpyD.elf:.debug_info::0x1eeb49
typedef enum _GXLightID
{
	GX_LIGHT0		= (1 << 0),
	GX_LIGHT1		= (1 << 1),
	GX_LIGHT2		= (1 << 2),
	GX_LIGHT3		= (1 << 3),
	GX_LIGHT4		= (1 << 4),
	GX_LIGHT5		= (1 << 5),
	GX_LIGHT6		= (1 << 6),
	GX_LIGHT7		= (1 << 7),

	GX_MAX_LIGHT	= (1 << 8),
	GX_LIGHT_NULL	= 0
} GXLightID;

// [SGLEA4]/GormitiDebug.elf:.debug_info::0x48f6dc
typedef enum _GXLogicOp
{
	GX_LO_CLEAR,

	GX_LO_AND,
	GX_LO_REVAND,
	GX_LO_COPY,
	GX_LO_INVAND,
	GX_LO_NOOP,
	GX_LO_XOR,
	GX_LO_OR,
	GX_LO_NOR,
	GX_LO_EQUIV,
	GX_LO_INV,
	GX_LO_REVOR,
	GX_LO_INVCOPY,
	GX_LO_INVOR,
	GX_LO_NAND,

	GX_LO_SET
} GXLogicOp;

// [SGLEA4]/GormitiDebug.elf:.debug_info::0x455fb4
typedef enum _GXPrimitive
{
	GX_POINTS			= 0xb8, // 0b1011'1000

	GX_LINES			= 0xa8, // 0b1010'1000
	GX_LINESTRIP		= 0xb0, // 0b1011'0000

	GX_TRIANGLES		= 0x90, // 0x1001'0000
	GX_TRIANGLESTRIP	= 0x98, // 0x1001'1000
	GX_TRIANGLEFAN		= 0xa0, // 0x1010'0000

	GX_QUADS			= 0x80, // 0x1000'0000
} GXPrimitive;

// [SPQE7T]/ISpyD.elf:.debug_info::0x1f105e
typedef enum _GXTevMode
{
	GX_MODULATE,
	GX_DECAL,
	GX_BLEND,
	GX_REPLACE,
	GX_PASSCLR,
} GXTevMode;

// [SGLEA4]/GormitiDebug.elf:.debug_info::0x163019
typedef enum _GXTevStageID
{
	GX_TEVSTAGE0,
	GX_TEVSTAGE1,
	GX_TEVSTAGE2,
	GX_TEVSTAGE3,
	GX_TEVSTAGE4,
	GX_TEVSTAGE5,
	GX_TEVSTAGE6,
	GX_TEVSTAGE7,
	GX_TEVSTAGE8,
	GX_TEVSTAGE9,
	GX_TEVSTAGE10,
	GX_TEVSTAGE11,
	GX_TEVSTAGE12,
	GX_TEVSTAGE13,
	GX_TEVSTAGE14,
	GX_TEVSTAGE15,

	GX_MAX_TEVSTAGE
} GXTevStageID;

// [SGLEA4]/GormitiDebug.elf:.debug_info::0x14a955
typedef enum _GXTexCoordID
{
	GX_TEXCOORD0,
	GX_TEXCOORD1,
	GX_TEXCOORD2,
	GX_TEXCOORD3,
	GX_TEXCOORD4,
	GX_TEXCOORD5,
	GX_TEXCOORD6,
	GX_TEXCOORD7,

	GX_MAX_TEXCOORD,
	GX_TEXCOORD_NULL = 0xff
} GXTexCoordID;

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

// [SGLEA4]/GormitiDebug.elf:.debug_info::0x2b93e8
typedef enum _GXTexMapID
{
	GX_TEXMAP0,
	GX_TEXMAP1,
	GX_TEXMAP2,
	GX_TEXMAP3,
	GX_TEXMAP4,
	GX_TEXMAP5,
	GX_TEXMAP6,
	GX_TEXMAP7,

	GX_MAX_TEXMAP,
	GX_TEXMAP_NULL	= 0xff,
	GX_TEX_DISABLE	= 256
} GXTexMapID;

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

// [SPQE7T]/ISpyD.elf:.debug_info::0x2689ea
typedef enum _GXVtxFmt
{
	GX_VTXFMT0,
	GX_VTXFMT1,
	GX_VTXFMT2,
	GX_VTXFMT3,
	GX_VTXFMT4,
	GX_VTXFMT5,
	GX_VTXFMT6,
	GX_VTXFMT7,

	GX_MAX_VTXFMT
} GXVtxFmt;

/* Name:
 * Patent US 6,937,245 B1
 * https://patents.google.com/patent/US6937245B1
 * section 13, line 4
 */
typedef u8 GXBool;

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

void GXClearVtxDesc(void);
void GXLoadPosMtxImm(Mtx, int);
void GXSetBlendMode(GXBlendMode, GXBlendFactor, GXBlendFactor, GXLogicOp);
void GXSetChanCtrl(GXChannelID, GXBool, GXColorSrc, GXColorSrc, GXLightID,
                   GXDiffuseFn, GXAttnFn);
void GXSetCullMode(GXCullMode);
void GXSetLineWidth(u8, int);
void GXSetNumChans(u8);
void GXSetNumTevStages(u8);
void GXSetNumTexGens(u8);
void GXSetTevOp(GXTevStageID, GXTevMode);
void GXSetTevOrder(GXTevStageID, GXTexCoordID, GXTexMapID, GXChannelID);
void GXSetVtxAttrFmt(GXVtxFmt, GXAttr, GXCompCnt, GXCompType, u8);
void GXSetVtxDesc(GXAttr, GXAttrType);

void GXBegin(GXPrimitive, GXVtxFmt, u16);

static inline void GXEnd(void)
{
#if !defined(NDEBUG)
	extern GXBool __GXinBegin;

	if (!__GXinBegin)
	{
		OSError_FileLine("GXGeometry.h", 127,
		                 "GXEnd: called without a GXBegin");
	}

	__GXinBegin = false;
#endif // !defined(NDEBUG)
}

union WGPipe
{
	u32 u32;
	f32 f32;
};

extern volatile union WGPipe __WGPipe ATTR_ADDRESS(0xcc008000);

inline void GXPosition3f32(f32 x, f32 y, f32 z)
{
	__WGPipe.f32 = x;
	__WGPipe.f32 = y;
	__WGPipe.f32 = z;
}

inline void GXColor1u32(u32 color)
{
	__WGPipe.u32 = color;
}

// TODO
typedef struct MEMAllocator MEMAllocator;

void *MEMAllocFromAllocator(MEMAllocator *allocator, u32 size);
void MEMFreeToAllocator(MEMAllocator *allocator, void *block);

// TODO
struct KPADStatus;

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

#ifdef __cplusplus
	}
#endif

// IWYU pragma: end_exports

#endif // CONTEXT_RVL_H
