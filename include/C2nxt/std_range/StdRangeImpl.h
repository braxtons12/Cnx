/// @file StdRangeImpl.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief This module provides the function definitions for a template instantiation of
/// `StdRange(T)`
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

#if defined(RANGE_T) && RANGE_IMPL

	#define STD_TEMPLATE_SUPPRESS_INSTANTIATIONS TRUE

	#include <C2nxt/StdIterator.h>
	#include <C2nxt/StdBasicTypes.h>

[[maybe_unused]] static const StdRangeIdentifier(RANGE_T, vtable)
	StdRangeIdentifier(RANGE_T, vtable_impl)
	= {
		.begin = StdRangeIdentifier(RANGE_T, begin),
		.end = StdRangeIdentifier(RANGE_T, end),
		.cbegin = StdRangeIdentifier(RANGE_T, begin),
		.cend = StdRangeIdentifier(RANGE_T, end),
		.into_iter = StdRangeIdentifier(RANGE_T, into_iter),
};

	#define ___DISABLE_IF_NULL(self) \
		std_disable_if(!(self), "Can't perform an operation on a null range")

[[nodiscard]] [[not_null(1)]] StdRangeIdentifier(RANGE_T, Iterator)
	StdRangeIdentifier(RANGE_T, iterator_new)(const StdRange(RANGE_T) * restrict self)
		___DISABLE_IF_NULL(self);
[[nodiscard]] [[not_null(1)]] Ref(RANGE_T)
	StdRangeIdentifier(RANGE_T, iterator_next)(StdForwardIterator(Ref(RANGE_T)) * restrict self)
		___DISABLE_IF_NULL(self);
[[nodiscard]] [[not_null(1)]] Ref(RANGE_T)
	StdRangeIdentifier(RANGE_T,
					   iterator_current)(const StdForwardIterator(Ref(RANGE_T)) * restrict self)
		___DISABLE_IF_NULL(self);
[[nodiscard]] [[not_null(1, 2)]] bool
	StdRangeIdentifier(RANGE_T,
					   iterator_equals)(const StdForwardIterator(Ref(RANGE_T)) * restrict self,
										const StdForwardIterator(Ref(RANGE_T)) * restrict rhs)
		___DISABLE_IF_NULL(self) std_disable_if(!rhs, "Can't compare a StdRange to a nullptr");

ImplIntoStdForwardIterator(StdRange(RANGE_T),
						   Ref(RANGE_T),
						   StdRangeIdentifier(RANGE_T, into_iter),
						   StdRangeIdentifier(RANGE_T, iterator_new),
						   StdRangeIdentifier(RANGE_T, iterator_next),
						   StdRangeIdentifier(RANGE_T, iterator_current),
						   StdRangeIdentifier(RANGE_T, iterator_equals));

StdRangeIdentifier(RANGE_T, Iterator)
	StdRangeIdentifier(RANGE_T, iterator_new)(const StdRange(RANGE_T) * restrict self) {
	let_mut _self = const_cast(StdRange(RANGE_T)*)(
		self); /** HACK relying on all C2nxt compliant concrete iterator
				* types to have the same       \ layout **/
	let begin = static_cast(StdRangeIdentifier(RANGE_T, Iterator)*)(self->m_begin.m_self);
	return (StdRangeIdentifier(RANGE_T, Iterator)){
		.m_range = _self,
		.m_index = begin->m_index,
	};
}

Ref(RANGE_T)
	StdRangeIdentifier(RANGE_T, iterator_next)(StdForwardIterator(Ref(RANGE_T)) * restrict self) {
	let_mut _self = static_cast(StdRangeIdentifier(RANGE_T, Iterator)*)(self->m_self);
	while(!std_iterator_equals(_self->m_range->m_current, _self->m_range->m_end)) {
		let current = &std_iterator_next(_self->m_range->m_current);
		_self->m_index++;
		if(_self->m_range->m_filter(static_cast(const RANGE_T*)(current))) {
			return current;
		}
	}

	return &std_iterator_current(_self->m_range->m_begin);
}

Ref(RANGE_T)
	StdRangeIdentifier(RANGE_T,
					   iterator_current)(const StdForwardIterator(Ref(RANGE_T)) * restrict self) {
	let_mut _self = static_cast(const StdRangeIdentifier(RANGE_T, Iterator)*)(self->m_self);
	return &std_iterator_current(_self->m_range->m_current);
}

bool StdRangeIdentifier(RANGE_T,
						iterator_equals)(const StdForwardIterator(Ref(RANGE_T)) * restrict self,
										 const StdForwardIterator(Ref(RANGE_T)) * restrict rhs) {
	let_mut _self = static_cast(const StdRangeIdentifier(RANGE_T, Iterator)*)(self->m_self);
	let_mut _rhs = static_cast(const StdRangeIdentifier(RANGE_T, Iterator)*)(rhs->m_self);

	return _self->m_range == _rhs->m_range && _self->m_index == _rhs->m_index;
}

StdForwardIterator(Ref(RANGE_T))
	StdRangeIdentifier(RANGE_T, begin)(StdRange(RANGE_T) * restrict self) {
	let iter = std_range_into_iter(*self);
	let_mut inner = static_cast(StdRangeIdentifier(RANGE_T, Iterator)*)(iter.m_self);
	self->m_current = self->m_begin;
	let_mut current = &std_iterator_current(self->m_current);
	do {
		if(self->m_filter(static_cast(const RANGE_T*)(current))) {
			return iter;
		}

		current = &std_iterator_next(self->m_current);
		inner->m_index++;
	} while(!std_iterator_equals(self->m_current, self->m_end));
	return iter;
}

StdForwardIterator(Ref(RANGE_T))
	StdRangeIdentifier(RANGE_T, end)(StdRange(RANGE_T) * restrict self) {
	let_mut iter = std_range_into_iter(*self);
	let_mut inner = static_cast(StdRangeIdentifier(RANGE_T, Iterator)*)(
		iter.m_self); /** HACK relying on all C2nxt compliant concrete iterator types to have the
					   * same       \ layout **/
	let end = static_cast(StdRangeIdentifier(RANGE_T, Iterator)*)(self->m_end.m_self);
	inner->m_index = end->m_index;
	return iter;
}

StdRange(RANGE_T) StdRangeIdentifier(RANGE_T, from)(StdForwardIterator(Ref(RANGE_T)) begin,
													StdForwardIterator(Ref(RANGE_T)) end,
													StdRangeFilter(RANGE_T) filter) {
	return (StdRange(RANGE_T)){.m_begin = begin,
							   .m_end = end,
							   .m_current = begin,
							   .m_filter = filter,
							   .m_vtable = &StdRangeIdentifier(RANGE_T, vtable_impl)};
}

bool StdRangeIdentifier(RANGE_T, default_filter)([[maybe_unused]] const RANGE_T* elem) {
	return true;
}

	#undef ___DISABLE_IF_NULL
	#undef STD_TEMPLATE_SUPPRESS_INSTANTIATIONS
#endif // defined(RANGE_T) && RANGE_IMPL
