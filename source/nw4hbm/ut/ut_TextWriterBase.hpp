#ifndef RVL_SDK_HBM_NW4HBM_UT_TEXT_WRITER_BASE_HPP
#define RVL_SDK_HBM_NW4HBM_UT_TEXT_WRITER_BASE_HPP

/*******************************************************************************
 * headers
 */

#include <cstdarg> // std::va_list
#include <cstdio> // std::vsnprintf
#include <cstring> // std::strlen
#include <cwchar>

#include <types.h>

#include "ut_CharWriter.hpp"
#include "ut_TagProcessorBase.hpp"

/*******************************************************************************
 * classes and functions
 */

namespace nw4hbm { namespace ut
{
	// forward declarations
	struct Rect;

	// [SGLEA4]/GormitiDebug.elf:.debug_info::0x49b65b, 0x4a55da...
	template <typename charT>
	class TextWriterBase : public CharWriter
	{
	// methods
	public:
#if 1
		// cdtors
		TextWriterBase();
		~TextWriterBase();

		// explicit instantiation function ordering (TODO)
		void SetLineHeight(f32 lineHeight);
		f32 GetLineHeight() const;
		void SetLineSpace(f32 lineSpace);
		void SetCharSpace(f32 charSpace);
		f32 GetLineSpace() const;
		f32 GetCharSpace() const;
		void SetTabWidth(int tabWidth);
		int GetTabWidth() const;
		void SetDrawFlag(u32 flags);
		u32 GetDrawFlag() const;
		void SetTagProcessor(TagProcessorBase<charT> *tagProcessor);
		void ResetTagProcessor();
		TagProcessorBase<charT> &GetTagProcessor() const;
		f32 CalcFormatStringWidth(const charT *format, ...) const;
		f32 CalcFormatStringHeight(const charT *format, ...) const;
		void CalcFormatStringRect(Rect *pRect, const charT *format, ...) const;
		void CalcVStringRect(Rect *pRect, const charT *format, std::va_list args) const;
		f32 CalcStringWidth(const charT *str, int length) const;
		f32 CalcStringWidth(const charT *str) const;
		f32 CalcStringHeight(const charT *str, int length) const;
		f32 CalcStringHeight(const charT *str) const;
		void CalcStringRect(Rect *pRect, const charT *str, int length) const;
		void CalcStringRect(Rect *pRect, const charT *str) const;
		f32 Printf(const charT *format, ...);
		f32 VPrintf(const charT *format, std::va_list args);
		f32 Print(const charT *str, int length);
		f32 Print(const charT *str);

		static charT *SetBuffer(charT *buf, u32 size);
		static charT *SetBuffer(u32 size);
		static u32 GetBufferSize();
		static charT *GetBuffer();
		static inline int VSNPrintf(charT *buffer, u32 count,
		                            const charT *format, std::va_list arg);
		static inline int StrLen(const charT *str);

		f32 CalcLineWidth(const charT *str, int length);
		int CalcLineRectImpl(Rect *pRect, const charT *str, int length);
		void CalcStringRectImpl(Rect *pRect, const charT *str, int length);
		f32 PrintImpl(const charT *str, int length);
		f32 AdjustCursor(float *xOrigin, float *yOrigin, const charT *str,
		                 int length);
		bool IsDrawFlagSet(u32 mask, u32 flag) const
		{
			return (mDrawFlag & mask) == flag;
		}
#else
		static charT *GetBuffer() { return mFormatBuffer; }
		static charT *SetBuffer(charT *buffer, u32 size)
		{
			charT *old = mFormatBuffer;
			mFormatBuffer = buffer;
			mFormatBufferSize = size;
			return old;
		}

		static u32 GetBufferSize() { return mFormatBufferSize; }

		TextWriterBase();
		~TextWriterBase();

		f32 GetCharSpace() const { return mCharSpace; }
		void SetCharSpace(f32 space) { mCharSpace = space; }

		f32 GetLineSpace() const { return mLineSpace; }
		void SetLineSpace(f32 space) { mLineSpace = space; }

		int GetTabWidth() const { return mTabWidth; }
		void SetTabWidth(int width) { mTabWidth = width; }

		void SetDrawFlag(u32 flag) { mDrawFlag = flag; }
		bool IsDrawFlagSet(u32 mask, u32 flag) const
		{
			return (mDrawFlag & mask) == flag;
		}

		TagProcessorBase<charT> *GetTagProcessor() const { return mTagProcessor; }
		void SetTagProcessor(TagProcessorBase<charT> *processor)
		{
			mTagProcessor = processor;
		}
		void ResetTagProcessor() { mTagProcessor = &mDefaultTagProcessor; }

		f32 GetLineHeight() const;

		f32 CalcLineWidth(const charT *str, int len);
		f32 CalcStringWidth(const charT *str, int len) const;
		void CalcStringRect(Rect *rect, const charT *str, int len) const;

		int StrLen(const charT *str);
		int VSNPrintf(charT *buffer, u32 count, const charT *fmt,
		              std::va_list args);
		f32 VPrintf(const charT *str, std::va_list args);
		f32 Print(const charT *str, int len);

		bool CalcLineRectImpl(Rect *rect, const charT *str, int len);
		void CalcStringRectImpl(Rect *rect, const charT *str, int len);

		f32 PrintImpl(const charT *str, int len);
		f32 AdjustCursor(f32 *x1, f32 *y1, const charT *str, int len);
#endif

	// members
	private:
		/* base CharWriter */					// offset 0x00, size 0x4c
		f32						mCharSpace;		// offset 0x4c, size 0x04
		f32						mLineSpace;		// offset 0x50, size 0x04
		int						mTabWidth;		// offset 0x54, size 0x04
		u32						mDrawFlag;		// offset 0x58, size 0x04
		TagProcessorBase<charT>	*mTagProcessor;	// offset 0x5c, size 0x04

	// static members
	private:
		static charT *mFormatBuffer;
		static u32 mFormatBufferSize;
		static TagProcessorBase<charT> mDefaultTagProcessor;
	}; // size 0x60

	// template specializations (TODO: how to instantiate on release?)
	template <>
	inline int TextWriterBase<char>::StrLen(const char *str)
	{
		return std::strlen(str);
	}

	template <>
	inline int TextWriterBase<wchar_t>::StrLen(const wchar_t *str)
	{
		return std::wcslen(str);
	}

	template <>
	inline int TextWriterBase<char>::VSNPrintf(char *buffer, u32 count,
	                                           const char *format,
	                                           std::va_list arg)
	{
		return std::vsnprintf(buffer, count, format, arg);
	}

	template <>
	inline int TextWriterBase<wchar_t>::VSNPrintf(wchar_t *buffer,
	                                              u32 count,
	                                              const wchar_t *format,
	                                              std::va_list arg)
	{
		return std::vswprintf(buffer, count, format, arg);
	}
}} // namespace nw4hbm::ut

#endif // RVL_SDK_HBM_NW4HBM_UT_TEXT_WRITER_BASE_HPP
