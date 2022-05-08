/// @file RangeDecl.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief This module provides the function declarations and type definitions for a template
/// instantiation of `CnxRange(T)`
/// @version 0.2.1
/// @date 2022-05-08
///
/// MIT License
/// @copyright Copyright (c) 2022 Braxton Salyer <braxtonsalyer@gmail.com>
///
/// Permission is hereby granted, free of charge, to any person obtaining a copy
/// of this software and associated documentation files (the "Software"), to deal
/// in the Software without restriction, including without limitation the rights
/// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
/// copies of the Software, and to permit persons to whom the Software is
/// furnished to do so, subject to the following conditions:
///
/// The above copyright notice and this permission notice shall be included in all
/// copies or substantial portions of the Software.
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
/// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
/// SOFTWARE.

#include <Cnx/range/RangeDef.h>

#if defined(RANGE_T) && RANGE_DECL

	#define CNX_TEMPLATE_SUPPRESS_INSTANTIATIONS TRUE

	#include <Cnx/Iterator.h>
	#include <Cnx/BasicTypes.h>

typedef bool (*CnxRangeFilter(RANGE_T))(const RANGE_T* restrict element);
typedef void (*CnxRangeTransform(RANGE_T))(RANGE_T* restrict element);
typedef RANGE_T (*CnxRangeAccumulator(RANGE_T))(const RANGE_T* restrict current_value,
												const RANGE_T* restrict element);
typedef struct CnxRangeIdentifier(RANGE_T, vtable) CnxRangeIdentifier(RANGE_T, vtable);

typedef struct CnxRange(RANGE_T) {
	CnxForwardIterator(Ref(RANGE_T)) m_begin;
	CnxForwardIterator(Ref(RANGE_T)) m_end;
	CnxForwardIterator(Ref(RANGE_T)) m_current;
	CnxRangeFilter(RANGE_T) m_filter;
	const CnxRangeIdentifier(RANGE_T, vtable) * m_vtable;
}
CnxRange(RANGE_T);

__attr(nodiscard) RANGE_STATIC RANGE_INLINE CnxRange(RANGE_T)
	CnxRangeIdentifier(RANGE_T, from)(CnxForwardIterator(Ref(RANGE_T)) begin,
									  CnxForwardIterator(Ref(RANGE_T)) end,
									  CnxRangeFilter(RANGE_T) filter);

	#define ___DISABLE_IF_NULL(self) \
		cnx_disable_if(!(self), "Can't perform an operation on a null range")

__attr(nodiscard) __attr(not_null(1)) RANGE_STATIC RANGE_INLINE CnxForwardIterator(Ref(RANGE_T))
	CnxRangeIdentifier(RANGE_T, begin)(CnxRange(RANGE_T) * restrict self) ___DISABLE_IF_NULL(self);
__attr(nodiscard) __attr(not_null(1)) RANGE_STATIC RANGE_INLINE CnxForwardIterator(Ref(RANGE_T))
	CnxRangeIdentifier(RANGE_T, end)(CnxRange(RANGE_T) * restrict self) ___DISABLE_IF_NULL(self);

typedef struct CnxRangeIdentifier(RANGE_T, Iterator) {
	isize m_index;
	CnxRange(RANGE_T)* const m_range;
}
CnxRangeIdentifier(RANGE_T, Iterator);

DeclIntoCnxForwardIterator(CnxRange(RANGE_T), Ref(RANGE_T), CnxRangeIdentifier(RANGE_T, into_iter));

typedef struct CnxRangeIdentifier(RANGE_T, vtable) {
	CnxForwardIterator(Ref(RANGE_T)) (*const begin)(CnxRange(RANGE_T)* restrict self);
	CnxForwardIterator(Ref(RANGE_T)) (*const end)(CnxRange(RANGE_T)* restrict self);
	CnxForwardIterator(Ref(RANGE_T)) (*const cbegin)(CnxRange(RANGE_T)* restrict self);
	CnxForwardIterator(Ref(RANGE_T)) (*const cend)(CnxRange(RANGE_T)* restrict self);
	CnxForwardIterator(Ref(RANGE_T)) (*const into_iter)(const CnxRange(RANGE_T)* restrict self);
}
CnxRangeIdentifier(RANGE_T, vtable);

__attr(nodiscard) RANGE_STATIC RANGE_INLINE
	bool CnxRangeIdentifier(RANGE_T, default_filter)(const RANGE_T* elem);

	#undef ___DISABLE_IF_NULL
	#undef CNX_TEMPLATE_SUPPRESS_INSTANTIATIONS
#endif // defined(RANGE_T) && RANGE_DECL
