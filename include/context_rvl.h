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

typedef s64 OSTime;
typedef u32 OSTick;

void OSRegisterVersion(const char *version);

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

typedef struct OSAlarm OSAlarm;
typedef struct OSContext OSContext;

typedef void OSAlarmHandler(OSAlarm *alarm, OSContext *context);

// [SPQE7T]/ISpyD.elf:.debug_info::0x2950d7
struct OSAlarm
{
	OSAlarmHandler	*handler;	// size 0x04, offset 0x00
	u32				tag;		// size 0x04, offset 0x04
	OSTime			fire;		// size 0x08, offset 0x08
	OSAlarm			*prev;		// size 0x04, offset 0x10
	OSAlarm			*next;		// size 0x04, offset 0x14
	OSTime			period;		// size 0x08, offset 0x18
	OSTime			start;		// size 0x08, offset 0x20
	void			*userData;	// size 0x04, offset 0x28
	/* 4 bytes padding */
}; // size 0x30

void OSCreateAlarm(OSAlarm *alarm);
void OSSetAlarm(OSAlarm *alarm, OSTime tick, OSAlarmHandler handler);
void OSSetPeriodicAlarm(OSAlarm *alarm, OSTime tick, OSTime period,
                        OSAlarmHandler handler);
void OSCancelAlarm(OSAlarm *alarm);
void OSSetAlarmUserData(OSAlarm *alarm, void *userData);
void *OSGetAlarmUserData(const OSAlarm *alarm);

#define OSGetAlarmUserDataAny(type_, alarm_)	\
	((type_)(OSGetAlarmUserData(alarm_)))

#define OSSetAlarmUserDataAny(alarm_, data_)	\
	OSSetAlarmUserData(alarm_, (void *)(data_))

// [SPQE7T]/ISpyD.elf:.debug_info::0x3135
struct OSContext
{
	register_t	gpr[32];			// size 0x080, offset 0x000
	register_t	cr;					// size 0x004, offset 0x080
	register_t	lr;					// size 0x004, offset 0x084
	register_t	ctr;				// size 0x004, offset 0x088
	register_t	xer;				// size 0x004, offset 0x08c
	f64			fpr[32];			// size 0x100, offset 0x090
	u32			fpscr_pad;			// size 0x004, offset 0x190
	register_t	fpscr;				// size 0x004, offset 0x194
	register_t	srr0;				// size 0x004, offset 0x198
	register_t	srr1;				// size 0x004, offset 0x19c
	u16			mode;				// size 0x002, offset 0x1a0
	u16			state;				// size 0x002, offset 0x1a2
	register_t	gqr[8];				// size 0x020, offset 0x1a4
	u32			psf_pad;			// size 0x004, offset 0x1c4
	f64			psf[32];			// size 0x100, offset 0x1c8
}; // size 0x2c8

BOOL OSDisableInterrupts(void);
BOOL OSRestoreInterrupts(BOOL intrStatus);

#define OSSendMessageAny(msgQueue_, msg_, flags_)	\
	OSSendMessage(msgQueue_, (OSMessage)(msg_), flags_)

#define OSReceiveMessageAny(msgQueue_, msgOut_, flags_)	\
	OSReceiveMessage(msgQueue_, (OSMessage *)(msgOut_), flags_)

#define OSJamMessageAny(msgQueue_, msg_, flags_)	\
	OSJamMessage(msgQueue_, (OSMessage)(msg_), flags_)

typedef void *OSMessage;

typedef u32 OSMessageFlags;
enum OSMessageFlags_et
{
	OS_MESSAGE_NO_FLAGS			= 0,

	OS_MESSAGE_FLAG_PERSISTENT	= (1 << 0),
};

typedef struct OSMutex OSMutex;
typedef struct OSThread OSThread;

// [SPQE7T]/ISpyD.elf:.debug_info::0x333a
typedef struct OSMutexLink
{
	OSMutex	*next;	// size 0x04, offset 0x00
	OSMutex	*prev;	// size 0x04, offset 0x04
} OSMutexLink; // size 0x08

enum OSThreadState
{
	OS_THREAD_STATE_EXITED		= 0,
	OS_THREAD_STATE_READY		= (1 << 0),
	OS_THREAD_STATE_RUNNING		= (1 << 1),
	OS_THREAD_STATE_SLEEPING	= (1 << 2),
	OS_THREAD_STATE_MORIBUND	= (1 << 3),
} typedef OSThreadState;

typedef u16 OSThreadFlags;
enum OSThreadFlags_et
{
	OS_THREAD_NO_FLAGS	= 0,

	OS_THREAD_DETACHED	= (1 << 0),
};

typedef void OSSwitchThreadCallback(OSThread *curThread, OSThread *newThread);
typedef void *OSThreadFunction(void *arg);

// [SPQE7T]/ISpyD.elf:.debug_info::0x2fb2
typedef struct OSThreadQueue
{
	OSThread	*head;	// size 0x04, offset 0x00
	OSThread	*tail;	// size 0x04, offset 0x04
} OSThreadQueue; // size 0x08

// [SPQE7T]/ISpyD.elf:.debug_info::0x328b
typedef struct OSThreadLink
{
	OSThread	*next;	// size 0x04, offset 0x00
	OSThread	*prev;	// size 0x04, offset 0x04
} OSThreadLink; // size 0x08

// [SPQE7T]/ISpyD.elf:.debug_info::0x32cf
typedef struct OSMutexQueue
{
	OSMutex	*head;	// size 0x04, offset 0x00
	OSMutex	*tail;	// size 0x04, offset 0x04
} OSMutexQueue; // size 0x08

// [SPQE7T]/ISpyD.elf:.debug_info::0x2feb
struct OSThread
{
	OSContext		context;		// size 0x2c8, offset 0x000
	u16				state;			// size 0x002, offset 0x2c8
	u16				attr;			// size 0x002, offset 0x2ca
	s32				suspend;		// size 0x004, offset 0x2cc
	s32				priority;		// size 0x004, offset 0x2d0
	s32				base;			// size 0x004, offset 0x2d4
	void			*val;			// size 0x004, offset 0x2d8
	OSThreadQueue	*queue;			// size 0x004, offset 0x2dc
	OSThreadLink	link;			// size 0x008, offset 0x2e0
	OSThreadQueue	queueJoin;		// size 0x008, offset 0x2e8
	OSMutex			*mutex;			// size 0x004, offset 0x2f0
	OSMutexQueue	queueMutex;		// size 0x008, offset 0x2f4
	OSThreadLink	linkActive;		// size 0x008, offset 0x2fc
	u8				*stackBase;		// size 0x004, offset 0x304
	u32				*stackEnd;		// size 0x004, offset 0x308
	s32				error;			// size 0x004, offset 0x30c
	void			*specific[2];	// size 0x008, offset 0x310
}; // size 0x318

BOOL OSCreateThread(OSThread *thread, OSThreadFunction *func, void *funcArg,
                   void *stackBegin, u32 stackSize, int prio,
                   OSThreadFlags flags);
BOOL OSJoinThread(OSThread *thread, void *val);
s32 OSResumeThread(OSThread *thread);

// [SPQE7T]/ISpyD.elf:.debug_info::0x23508c
typedef struct OSMessageQueue
{
	OSThreadQueue	queueSend;		// size 0x08, offset 0x00
	OSThreadQueue	queueReceive;	// size 0x08, offset 0x08
	OSMessage		*msgArray;		// size 0x04, offset 0x10
	s32				msgCount;		// size 0x04, offset 0x14
	s32				firstIndex;		// size 0x04, offset 0x18
	s32				usedCount;		// size 0x04, offset 0x1c
} OSMessageQueue; // size 0x20

// [SPQE7T]/ISpyD.elf:.debug_info::0x2f63
struct OSMutex
{
	OSThreadQueue	queue;		// size 0x08, offset 0x00
	OSThread		*thread;	// size 0x04, offset 0x08
	s32				count;		// size 0x04, offset 0x0c
	OSMutexLink		link;		// size 0x08, offset 0x10
}; // size 0x18

void OSInitMessageQueue(OSMessageQueue *msgQueue, OSMessage *buffer,
                        int capacity);
int OSSendMessage(OSMessageQueue *msgQueue, OSMessage msg,
                  OSMessageFlags flags);
int OSReceiveMessage(OSMessageQueue *msgQueue, OSMessage *msgOut,
                     OSMessageFlags flags);
int OSJamMessage(OSMessageQueue *msgQueue, OSMessage msg, OSMessageFlags flags);

extern u32 OS_BUS_CLOCK ATTR_ADDRESS(0x800000f8);

#define OS_TIMER_CLOCK	(OS_BUS_CLOCK / 4)

// clang-format off
#define OSTicksToSeconds(ticks)			((ticks)        / (OS_TIMER_CLOCK         )       )
#define OSTicksToMilliseconds(ticks)	((ticks)        / (OS_TIMER_CLOCK / 1000  )       )
#define OSTicksToMicroseconds(ticks)	((ticks) * 8    / (OS_TIMER_CLOCK / 125000)       )
#define OSTicksToNanoseconds(ticks)		((ticks) * 8000 / (OS_TIMER_CLOCK / 125000)       )

#define OSSecondsToTicks(sec)			((  sec)        * (OS_TIMER_CLOCK         )       )
#define OSMillisecondsToTicks(msec)		(( msec)        * (OS_TIMER_CLOCK / 1000  )       )
#define OSMicrosecondsToTicks(usec)		(( usec)        * (OS_TIMER_CLOCK / 125000) / 8   )
#define OSNanosecondsToTicks(nsec)		(( nsec)        * (OS_TIMER_CLOCK / 125000) / 8000)

#define OSDiffTick(tick1, tick0)		((OSTick)(tick1) - (OSTick)(tick0))
// clang-format on
OSTime OSGetTime(void);

BOOL AICheckInit(void);
void AIInit(void *stack);

// [SPQE7T]/ISpyD.elf:.debug_info::0x119a90
typedef struct ARCHandle
{
	void		*archiveStartAddr;	// size 0x04, offset 0x00
	void		*FSTStart;			// size 0x04, offset 0x04
	void		*fileStart;			// size 0x04, offset 0x08
	u32			entryNum;			// size 0x04, offset 0x0c // more accurately entryCount
	const char	*FSTStringStart;	// size 0x04, offset 0x10
	u32			FSTLength;			// size 0x04, offset 0x14
	u32			currDir;			// size 0x04, offset 0x18 // more accurately what ARCDir calls entryNum
} ARCHandle; // size 0x1c

// [SPQE7T]/ISpyD.elf:.debug_info::0x368e0b
typedef struct ARCFileInfo
{
	ARCHandle	*handle;		// size 0x04, offset 0x00
	u32			startOffset;	// size 0x04, offset 0x04
	u32			length;			// size 0x04, offset 0x08
} ARCFileInfo; // size 0x0c

BOOL ARCInitHandle(void *bin, ARCHandle *handle);
BOOL ARCOpen(ARCHandle *handle, const char *filename, ARCFileInfo *af);
BOOL ARCFastOpen(ARCHandle *handle, int entrynum, ARCFileInfo *af);
void *ARCGetStartAddrInMem(ARCFileInfo *af);
u32 ARCGetLength(ARCFileInfo *af);
BOOL ARCClose(ARCFileInfo *af);

// [SPQE7T]/ISpyD.elf:.debug_info::0x36a6f8
typedef void AXFrameCallback(void);

// [SPQE7T]/ISpyD.elf:.debug_info::0x36c40e
typedef void AXAuxCallback(void *data, void *context);

// TODO
typedef struct _AXVPB AXVPB;

void AXInit(void);
AXFrameCallback *AXRegisterCallback(AXFrameCallback *cb);

// [SPQE7T]/ISpyD.elf:.debug_info::0x36bdec
typedef void *AXFXAllocFunc(size_t /* explicitly unnamed */);

// [SPQE7T]/ISpyD.elf:.debug_info::0x36be0f
typedef void AXFXFreeFunc(void */* explicitly unnamed */);

// [SPQE7T]/ISpyD.elf:.debug_info::0x36c3ae
typedef struct AXFX_BUS
{
	s32	*left;		// size 0x04, offset 0x00
	s32	*right;		// size 0x04, offset 0x04
	s32	*surround;	// size 0x04, offset 0x08
} AXFX_BUS; // size 0x0c

// [SPQE7T]/ISpyD.elf:.debug_info::0x36bec8
typedef struct AXFX_REVERBHI_EXP
{
	f32			*earlyLine[3];				// size 0x00c, offset 0x000
	u32			earlyPos[3];				// size 0x00c, offset 0x00c
	u32			earlyLength;				// size 0x004, offset 0x018
	u32			earlyMaxLength;				// size 0x004, offset 0x01c
	f32			earlyCoef[3];				// size 0x00c, offset 0x020
	f32			*preDelayLine[3];			// size 0x00c, offset 0x02c
	u32			preDelayPos;				// size 0x004, offset 0x038
	u32			preDelayLength;				// size 0x004, offset 0x03c
	u32			preDelayMaxLength;			// size 0x004, offset 0x040
	f32			*combLine[3][3];			// size 0x024, offset 0x044
	u32			combPos[3];					// size 0x00c, offset 0x068
	u32			combLength[3];				// size 0x00c, offset 0x074
	u32			combMaxLength[3];			// size 0x00c, offset 0x080
	f32			combCoef[3];				// size 0x00c, offset 0x08c
	f32			*allpassLine[3][2];			// size 0x018, offset 0x098
	u32			allpassPos[2];				// size 0x008, offset 0x0b0
	u32			allpassLength[2];			// size 0x008, offset 0x0b8
	u32			allpassMaxLength[2];		// size 0x008, offset 0x0c0
	f32			*lastAllpassLine[3];		// size 0x00c, offset 0x0c8
	u32			lastAllpassPos[3];			// size 0x00c, offset 0x0d4
	u32			lastAllpassLength[3];		// size 0x00c, offset 0x0e0
	u32			lastAllpassMaxLength[3];	// size 0x00c, offset 0x0ec
	f32			allpassCoef;				// size 0x004, offset 0x0f8
	f32			lastLpfOut[3];				// size 0x00c, offset 0x0fc
	f32			lpfCoef;					// size 0x004, offset 0x108
	u32			active;						// size 0x004, offset 0x10c
	u32			earlyMode;					// size 0x004, offset 0x110
	f32			preDelayTimeMax;			// size 0x004, offset 0x114
	f32			preDelayTime;				// size 0x004, offset 0x118
	u32			fusedMode;					// size 0x004, offset 0x11c
	f32			fusedTime;					// size 0x004, offset 0x120
	f32			coloration;					// size 0x004, offset 0x124
	f32			damping;					// size 0x004, offset 0x128
	f32			crosstalk;					// size 0x004, offset 0x12c
	f32			earlyGain;					// size 0x004, offset 0x130
	f32			fusedGain;					// size 0x004, offset 0x134
	AXFX_BUS	*busIn;						// size 0x004, offset 0x138
	AXFX_BUS	*busOut;					// size 0x004, offset 0x13c
	f32			outGain;					// size 0x004, offset 0x140
	f32			sendGain;					// size 0x004, offset 0x144
} AXFX_REVERBHI_EXP; // size 0x148

// [SPQE7T]/ISpyD.elf:.debug_info::0x36be2c
typedef struct AXFX_REVERBHI
{
	AXFX_REVERBHI_EXP	reverbInner;	// size 0x148, offset 0x000
	f32					coloration;		// size 0x004, offset 0x148
	f32					mix;			// size 0x004, offset 0x14c
	f32					time;			// size 0x004, offset 0x150
	f32					damping;		// size 0x004, offset 0x154
	f32					preDelay;		// size 0x004, offset 0x158
	f32					crosstalk;		// size 0x004, offset 0x15c
} AXFX_REVERBHI; // size 0x160

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

// [SPQE7T]/ISpyD.elf:.debug_info::0x28a5e5
enum _GXTexFmt
{
	GX_TF_I4		=  0,
	GX_TF_I8		=  1,
	GX_TF_IA4		=  2,
	GX_TF_IA8		=  3,

	GX_TF_RGB565	=  4,
	GX_TF_RGB5A3	=  5,
	GX_TF_RGBA8		=  6,

	GX_TF_CMPR		= 14,

	GX_CTF_R4		= 32,
	GX_CTF_RA4		= 34,
	GX_CTF_RA8		= 35,
	GX_CTF_YUVA8	= 38,

	GX_CTF_A8		= 39,
	GX_CTF_R8		= 40,
	GX_CTF_G8		= 41,
	GX_CTF_B8		= 42,
	GX_CTF_RG8		= 43,
	GX_CTF_GB8		= 44,

	GX_TF_Z8		= 17,
	GX_TF_Z16		= 19,
	GX_TF_Z24X8		= 22,

	GX_CTF_Z4		= 48,
	GX_CTF_Z8M		= 57,
	GX_CTF_Z8L		= 58,
	GX_CTF_Z16L		= 60,

	GX_TF_A8		= GX_CTF_A8
} typedef GXTexFmt;

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

	__GXinBegin = FALSE;
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

typedef struct MEMiHeapHead MEMiHeapHead;
typedef struct MEMAllocator MEMAllocator;

// [SPQE7T]/ISpyD.elf:.debug_info::0x113386
typedef void *MEMAllocatorAllocFunc(MEMAllocator *pAllocator, u32 size);

// [SPQE7T]/ISpyD.elf:.debug_info::0x1133bb
typedef void MEMAllocatorFreeFunc(MEMAllocator *pAllocator, void *memBlock);

// [SPQE7T]/ISpyD.elf:.debug_info::0x113345
typedef struct MEMAllocatorFunc
{
	MEMAllocatorAllocFunc	*pfAlloc;	// size 0x04, offset 0x00
	MEMAllocatorFreeFunc	*pfFree;	// size 0x04, offset 0x04
} MEMAllocatorFunc; // size 0x08

// [SPQE7T]/ISpyD.elf:.debug_info::0x1132e1
struct MEMAllocator
{
	const MEMAllocatorFunc	*pFunc;		// size 0x04, offset 0x00
	MEMiHeapHead			*pHeap;		// size 0x04, offset 0x04
	u32						heapParam1;	// size 0x04, offset 0x08
	u32						heapParam2;	// size 0x04, offset 0x0c
}; // size 0x10

void *MEMAllocFromAllocator(MEMAllocator *allocator, u32 size);
void MEMFreeToAllocator(MEMAllocator *allocator, void *block);
void MEMInitAllocatorForExpHeap(MEMAllocator *allocator, MEMiHeapHead *heap,
                                s32 align);

MEMiHeapHead *MEMCreateExpHeapEx(void *start, u32 size, u16 opt);
MEMiHeapHead *MEMDestroyExpHeap(MEMiHeapHead *heap);
static inline MEMiHeapHead *MEMCreateExpHeap(void *start, u32 size)
{
	return MEMCreateExpHeapEx(start, size, 0);
}

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

typedef s32 WPADResult;
enum WPADResult_et
{
	WPAD_ERR_OK						= 0,

	WPAD_ERR_NO_CONTROLLER			= -1,	/* name known from asserts */
	WPAD_ERR_COMMUNICATION_ERROR	= -2,	// [RT3P54] has this as WPAD_ERR_BUSY
	WPAD_ERR_3						= -3,	// [RT3P54] has this as WPAD_ERR_TRANSFER
	WPAD_ERR_INVALID				= -4,	/* name comes from [R89JEL] */
//	WPAD_ERR_5						= -5,	/* unknown */
//	WPAD_ERR_6						= -6,	/* unknown */
	WPAD_ERR_CORRUPTED				= -7,	/* name comes from [R89JEL] */

#define WPAD_ESUCCESS	WPAD_ERR_OK
#define WPAD_ENODEV		WPAD_ERR_NO_CONTROLLER
#define WPAD_ECOMM		WPAD_ERR_COMMUNICATION_ERROR
// #define WPAD_E3
#define WPAD_EINVAL		WPAD_ERR_INVALID
#define WPAD_EBADE		WPAD_ERR_CORRUPTED

#define WPAD_CESUCCESS	(WPAD_ESUCCESS + 0)
#define WPAD_CENODEV	(WPAD_ENODEV + 0)
#define WPAD_CECOMM		(WPAD_ECOMM + 0)
#define WPAD_CERR_3		(WPAD_ERR_3 + 0)
#define WPAD_CEINVAL	(WPAD_EINVAL + 0)
#define WPAD_CEBADE		(WPAD_EBADE + 0)
};

typedef s32 WPADChannel;
enum WPADChannel_et
{
	WPAD_CHAN0,
	WPAD_CHAN1,
	WPAD_CHAN2,
	WPAD_CHAN3,				/* name known from asserts */

	WPAD_MAX_CONTROLLERS,	/* name known from asserts */
	WPAD_CHAN_INVALID		= -1
};

typedef s32 WPADDeviceType;
enum WPADDeviceType_et
{
	WPAD_DEV_CORE				=  0,
	WPAD_DEV_FS					=  1,
	WPAD_DEV_CLASSIC			=  2,	/* name known from asserts, but not value */
	WPAD_DEV_BALANCE_CHECKER	=  3,
	WPAD_DEV_VSM				=  4,

	WPAD_DEV_MOTION_PLUS		=  5,
	WPAD_DEV_MPLS_PT_FS			=  6,
	WPAD_DEV_MPLS_PT_CLASSIC	=  7,

	WPAD_DEV_TRAIN				= 16,
	WPAD_DEV_GUITAR				= 17,
	WPAD_DEV_DRUM				= 18,
	WPAD_DEV_TAIKO				= 19,
	WPAD_DEV_TURNTABLE			= 20,

	// seems to be like maybe general purpose non-specific device types
	// maybe this was for testing or something? idk
	WPAD_DEV_BULK_1				= 21,
	WPAD_DEV_BULK_2				= 22,
	WPAD_DEV_BULK_3				= 23,
	WPAD_DEV_BULK_4				= 24,
	WPAD_DEV_BULK_5				= 25,
	WPAD_DEV_BULK_6				= 26,
	WPAD_DEV_BULK_7				= 27,
	WPAD_DEV_BULK_8				= 28,

	WPAD_DEV_MPLS_PT_UNKNOWN	= 250,

	WPAD_DEV_251				= 251,
	WPAD_DEV_252				= 252,	// invalid device mode?
	WPAD_DEV_NONE				= 253,	// sort of like WPAD_ENODEV (see __wpadAbortInitExtension in WPADHIDParser.c)
//	WPAD_DEV_254				= 254,	/* unknown, doesn't seem to be used anywhere */
	WPAD_DEV_INITIALIZING		= 255,	// see __a1_20_status_report
};

// names from wiiuse/wpad.h
typedef u16 WPADButton;
enum WPADButton_et
{
//  H..-AB12 ...+^v><
	/* NOTE: the bytes from the report are swapped when being placed into this
	 * format (see MAKE_BUTTON in WPADHIDParser.c)
	 */
	WPAD_BUTTON_LEFT	= (1 <<  0),
	WPAD_BUTTON_RIGHT	= (1 <<  1),
	WPAD_BUTTON_DOWN	= (1 <<  2),
	WPAD_BUTTON_UP		= (1 <<  3),
	WPAD_BUTTON_PLUS	= (1 <<  4),
	//					  (1 <<  5),
	//					  (1 <<  6),
	//					  (1 <<  7),
	WPAD_BUTTON_2		= (1 <<  8),
	WPAD_BUTTON_1		= (1 <<  9),
	WPAD_BUTTON_B		= (1 << 10),
	WPAD_BUTTON_A		= (1 << 11),
	WPAD_BUTTON_MINUS	= (1 << 12),
	//					  (1 << 13),	// WPAD_BUTTON_FS_Z
	//					  (1 << 14),	// WPAD_BUTTON_FS_C
	WPAD_BUTTON_HOME	= (1 << 15),

	WPAD_BUTTON_ALL		= 0x9f1f
};

// names from wiiuse/wpad.h
typedef u16 WPADExtButton;
enum WPADExtButton_et
{
	// Nunchuk (fight stick?)
//  .CZ..... ........
	/* NOTE: these bits are actually in the normal button variable, but are
	 * enumerated here because they are buttons for an extension
	 */
	WPAD_BUTTON_FS_Z		= (1 << 13),
	WPAD_BUTTON_FS_C		= (1 << 14),

	WPAD_BUTTON_FS_ALL		= 0x6000,

	// Classic Controller
//  >vL-H+R. lBYAXr<^
	WPAD_BUTTON_CL_UP		= (1 <<  0),
	WPAD_BUTTON_CL_LEFT		= (1 <<  1),
	WPAD_BUTTON_CL_ZR		= (1 <<  2),
	WPAD_BUTTON_CL_X		= (1 <<  3),
	WPAD_BUTTON_CL_A		= (1 <<  4),
	WPAD_BUTTON_CL_Y		= (1 <<  5),
	WPAD_BUTTON_CL_B		= (1 <<  6),
	WPAD_BUTTON_CL_ZL		= (1 <<  7),
	//						  (1 <<  8),
	WPAD_BUTTON_CL_FULL_R	= (1 <<  9),
	WPAD_BUTTON_CL_PLUS		= (1 << 10),
	WPAD_BUTTON_CL_HOME		= (1 << 11),
	WPAD_BUTTON_CL_MINUS	= (1 << 12),
	WPAD_BUTTON_CL_FULL_L	= (1 << 13),
	WPAD_BUTTON_CL_DOWN		= (1 << 14),
	WPAD_BUTTON_CL_RIGHT	= (1 << 15),

	WPAD_BUTTON_CL_ALL		= 0xfeff,

	// Shinkansen train controller
	// shares the same format as the Classic Controller
	// (see WPADiExcludeButton)
	WPAD_BUTTON_TR_UP		= WPAD_BUTTON_CL_UP,
	WPAD_BUTTON_TR_LEFT		= WPAD_BUTTON_CL_LEFT,
	WPAD_BUTTON_TR_ZR		= WPAD_BUTTON_CL_ZR,
	WPAD_BUTTON_TR_X		= WPAD_BUTTON_CL_X,
	WPAD_BUTTON_TR_A		= WPAD_BUTTON_CL_A,
	WPAD_BUTTON_TR_Y		= WPAD_BUTTON_CL_Y,
	WPAD_BUTTON_TR_B		= WPAD_BUTTON_CL_B,
	WPAD_BUTTON_TR_ZL		= WPAD_BUTTON_CL_ZL,
	//						= WPAD_BUTTON_CL_8,
	WPAD_BUTTON_TR_FULL_R	= WPAD_BUTTON_CL_FULL_R,
	WPAD_BUTTON_TR_PLUS		= WPAD_BUTTON_CL_PLUS,
	WPAD_BUTTON_TR_HOME		= WPAD_BUTTON_CL_HOME,
	WPAD_BUTTON_TR_MINUS	= WPAD_BUTTON_CL_MINUS,
	WPAD_BUTTON_TR_FULL_L	= WPAD_BUTTON_CL_FULL_L,
	WPAD_BUTTON_TR_DOWN		= WPAD_BUTTON_CL_DOWN,
	WPAD_BUTTON_TR_RIGHT	= WPAD_BUTTON_CL_RIGHT,

	WPAD_BUTTON_TR_ALL		= WPAD_BUTTON_CL_ALL,
};

// WPADControlMotor
typedef u32 WPADMotorCommand;
enum WPADMotorCommand_et
{
	WPAD_MOTOR_STOP		= 0,	/* name known from asserts */
	WPAD_MOTOR_RUMBLE	= 1,	/* name known from asserts */
};

// WPADControlSpeaker
typedef u32 WPADSpeakerCommand;
enum WPADSpeakerCommand_et
{
	WPAD_SPEAKER_DISABLE	= 0,
	WPAD_SPEAKER_ENABLE		= 1,	// might be ON? see HBMRemoteSpk.cpp
	WPAD_SPEAKER_MUTE		= 2,
	WPAD_SPEAKER_UNMUTE		= 3,
	WPAD_SPEAKER_PLAY		= 4,	// figured out from HBM usage

	// does the same thing as ENABLE? unless i'm missing something
	// not used so i don't know the context
	WPAD_SPEAKER_CMD_05		= 5,
};

typedef void WPADCallback(WPADChannel chan, WPADResult result);
typedef void WPADConnectCallback(WPADChannel chan, WPADResult result);
typedef void WPADExtensionCallback(WPADChannel chan, WPADDeviceType devType);

typedef void WUDSyncDeviceCallback(s32 result, s32 num);
typedef WUDSyncDeviceCallback WPADSimpleSyncCallback;

// [SPQE7T]/ISpyD.elf:.debug_info::0xd675b
typedef struct WPADInfo
{
	BOOL	dpd;		// size 0x04, offset 0x00
	BOOL	speaker;	// size 0x04, offset 0x04
	BOOL	attach;		// size 0x04, offset 0x08
	BOOL	lowBat;		// size 0x04, offset 0x0c
	BOOL	nearempty;	// size 0x04, offset 0x10
	u8		battery;	// size 0x01, offset 0x14
	u8		led;		// size 0x01, offset 0x15
	u8		protocol;	// size 0x01, offset 0x16
	u8		firmware;	// size 0x01, offset 0x17
} WPADInfo; // size 0x18

u8 WPADGetRadioSensitivity(WPADChannel chan);

WPADResult WPADProbe(WPADChannel chan, WPADDeviceType *devTypeOut);
WPADConnectCallback *WPADSetConnectCallback(WPADChannel chan,
                                            WPADConnectCallback *cb);
WPADExtensionCallback *WPADSetExtensionCallback(WPADChannel chan,
                                                WPADExtensionCallback *cb);
WPADResult WPADGetInfoAsync(WPADChannel chan, WPADInfo *info, WPADCallback *cb);
void WPADControlMotor(WPADChannel chan, WPADMotorCommand command);
BOOL WPADIsSpeakerEnabled(WPADChannel chan);
WPADResult WPADControlSpeaker(WPADChannel chan, WPADSpeakerCommand command,
                              WPADCallback *cb);
BOOL WPADCanSendStreamData(WPADChannel chan);
WPADResult WPADSendStreamData(WPADChannel chan, void *p_buf, u16 len);
BOOL WPADIsUsedCallbackByKPAD(void);
void WPADSetCallbackByKPAD(BOOL isKPAD);

// [SPQE7T]/ISpyD.elf:.debug_info::0x29f653
typedef struct WENCInfo
{
	byte_t	data[32];
} WENCInfo; // size 0x20

s32 WENCGetEncodeData(WENCInfo *info, u32 flag, const s16 *pbyPcmData,
                       s32 nSampleNum, byte_t *pbyAdpcmData);

// [SPQE7T]/ISpyD.elf:.debug_info::0xd66f9
typedef struct KPADMPDir
{
	Vec	X;	// size 0x0c, offset 0x00
	Vec	Y;	// size 0x0c, offset 0x0c
	Vec	Z;	// size 0x0c, offset 0x18
} KPADMPDir; // size 0x24

// [SPQE7T]/ISpyD.elf:.debug_info::0xd66b8
typedef struct KPADMPStatus
{
	Vec			mpls;	// size 0x0c, offset 0x00
	Vec			angle;	// size 0x0c, offset 0x0c
	KPADMPDir	dir;	// size 0x24, offset 0x18
} KPADMPStatus; // size 0x3c

// [SPQE7T]/ISpyD.elf:.debug_info::0xd650e
typedef union KPADEXStatus
{
	// [SPQE7T]/ISpyD.elf:.debug_info::0xd6549
	struct /* explicitly untagged */
	{
		Vec2	stick;		// size 0x08, offset 0x00
		Vec		acc;		// size 0x0c, offset 0x08
		f32		acc_value;	// size 0x04, offset 0x14
		f32		acc_speed;	// size 0x04, offset 0x18
	} fs; // size 0x1c

	// [SPQE7T]/ISpyD.elf:.debug_info::0xd659d
	struct /* explicitly untagged */
	{
		u32		hold;		// size 0x04, offset 0x00
		u32		trig;		// size 0x04, offset 0x04
		u32		release;	// size 0x04, offset 0x08
		Vec2	lstick;		// size 0x08, offset 0x0c
		Vec2	rstick;		// size 0x08, offset 0x14
		f32		ltrigger;	// size 0x04, offset 0x1c
		f32		rtrigger;	// size 0x04, offset 0x20
	} cl; // size 0x24

	// [SPQE7T]/ISpyD.elf:.debug_info::0xd6623
	struct /* explicitly untagged */
	{
		f64	tgc_weight;		// size 0x08, offset 0x00
		f64	weight[4];		// size 0x20, offset 0x08
		f64	weight_ave[4];	// size 0x20, offset 0x28
		s32	weight_err;		// size 0x04, offset 0x48
		s32	tgc_weight_err;	// size 0x04, offset 0x4c
	} bl; // size 0x50
} KPADEXStatus;

// [SPQE7T]/ISpyD.elf:.debug_info::0xd6300
typedef struct KPADStatus
{
	u32				hold;			// size 0x04, offset 0x00
	u32				trig;			// size 0x04, offset 0x04
	u32				release;		// size 0x04, offset 0x08
	Vec				acc;			// size 0x0c, offset 0x0c
	f32				acc_value;		// size 0x04, offset 0x18
	f32				acc_speed;		// size 0x04, offset 0x1c
	Vec2			pos;			// size 0x08, offset 0x20
	Vec2			vec;			// size 0x08, offset 0x28
	f32				speed;			// size 0x04, offset 0x30
	Vec2			horizon;		// size 0x08, offset 0x34
	Vec2			hori_vec;		// size 0x08, offset 0x3c
	f32				hori_speed;		// size 0x04, offset 0x44
	f32				dist;			// size 0x04, offset 0x48
	f32				dist_vec;		// size 0x04, offset 0x4c
	f32				dist_speed;		// size 0x04, offset 0x50
	Vec2			acc_vertical;	// size 0x08, offset 0x54
	u8				dev_type;		// size 0x01, offset 0x5c
	s8				wpad_err;		// size 0x01, offset 0x5d
	s8				dpd_valid_fg;	// size 0x01, offset 0x5e
	u8				data_format;	// size 0x01, offset 0x5f
	KPADEXStatus	ex_status;		// size 0x50, offset 0x60
	KPADMPStatus	mpls;			// size 0x3c, offset 0xb0
	byte1_t			__paddings__[4]; // ??? is this the compiler?
} KPADStatus; // size 0xf0

#ifdef __cplusplus
	}
#endif

// IWYU pragma: end_exports

#endif // CONTEXT_RVL_H
