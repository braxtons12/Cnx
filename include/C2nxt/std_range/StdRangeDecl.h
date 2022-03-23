/// @file StdRangeDecl.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief This module provides the function declarations and type definitions for a template
/// instantiation of `StdRange(T)`
/// @version 0.2.0
/// @date 2022-03-22
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

#include <C2nxt/std_range/StdRangeDef.h>

#if defined(RANGE_T) && RANGE_DECL

	#define STD_TEMPLATE_SUPPRESS_INSTANTIATIONS TRUE

	#include <C2nxt/StdIterator.h>
	#include <C2nxt/StdBasicTypes.h>

typedef bool (*StdRangeFilter(RANGE_T))(const RANGE_T* restrict element);
typedef void (*StdRangeTransform(RANGE_T))(RANGE_T* restrict element);
typedef RANGE_T (*StdRangeAccumulator(RANGE_T))(const RANGE_T* restrict current_value,
												const RANGE_T* restrict element);
typedef struct StdRangeIdentifier(RANGE_T, vtable) StdRangeIdentifier(RANGE_T, vtable);

typedef struct StdRange(RANGE_T) {
	StdForwardIterator(Ref(RANGE_T)) m_begin;
	StdForwardIterator(Ref(RANGE_T)) m_end;
	StdForwardIterator(Ref(RANGE_T)) m_current;
	StdRangeFilter(RANGE_T) m_filter;
	const StdRangeIdentifier(RANGE_T, vtable) * m_vtable;
}
StdRange(RANGE_T);

[[nodiscard]] StdRange(RANGE_T)
	StdRangeIdentifier(RANGE_T, from)(StdForwardIterator(Ref(RANGE_T)) begin,
									  StdForwardIterator(Ref(RANGE_T)) end,
									  StdRangeFilter(RANGE_T) filter);

	#define ___DISABLE_IF_NULL(self) \
		std_disable_if(!(self), "Can't perform an operation on a null range")

[[nodiscard]] [[not_null(1)]] StdForwardIterator(Ref(RANGE_T))
	StdRangeIdentifier(RANGE_T, begin)(StdRange(RANGE_T) * restrict self) ___DISABLE_IF_NULL(self);
[[nodiscard]] [[not_null(1)]] StdForwardIterator(Ref(RANGE_T))
	StdRangeIdentifier(RANGE_T, end)(StdRange(RANGE_T) * restrict self) ___DISABLE_IF_NULL(self);

typedef struct StdRangeIdentifier(RANGE_T, Iterator) {
	isize m_index;
	StdRange(RANGE_T)* const m_range;
}
StdRangeIdentifier(RANGE_T, Iterator);

DeclIntoStdForwardIterator(StdRange(RANGE_T), Ref(RANGE_T), StdRangeIdentifier(RANGE_T, into_iter));

typedef struct StdRangeIdentifier(RANGE_T, vtable) {
	StdForwardIterator(Ref(RANGE_T)) (*const begin)(StdRange(RANGE_T)* restrict self);
	StdForwardIterator(Ref(RANGE_T)) (*const end)(StdRange(RANGE_T)* restrict self);
	StdForwardIterator(Ref(RANGE_T)) (*const cbegin)(StdRange(RANGE_T)* restrict self);
	StdForwardIterator(Ref(RANGE_T)) (*const cend)(StdRange(RANGE_T)* restrict self);
	StdForwardIterator(Ref(RANGE_T)) (*const into_iter)(const StdRange(RANGE_T)* restrict self);
}
StdRangeIdentifier(RANGE_T, vtable);

[[nodiscard]] bool StdRangeIdentifier(RANGE_T, default_filter)(const RANGE_T* elem);

	#undef ___DISABLE_IF_NULL
	#undef STD_TEMPLATE_SUPPRESS_INSTANTIATIONS
#endif // defined(RANGE_T) && RANGE_DECL
