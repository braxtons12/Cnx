/// @file RangeImpl.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief This module provides the function definitions for a template instantiation of
/// `CnxRange(T)`
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

#include <Cnx/range/RangeDef.h>

#if defined(RANGE_T) && RANGE_IMPL

	#define CNX_TEMPLATE_SUPPRESS_INSTANTIATIONS TRUE

	#include <Cnx/Iterator.h>
	#include <Cnx/BasicTypes.h>

[[maybe_unused]] static const CnxRangeIdentifier(RANGE_T, vtable)
	CnxRangeIdentifier(RANGE_T, vtable_impl)
	= {
		.begin = CnxRangeIdentifier(RANGE_T, begin),
		.end = CnxRangeIdentifier(RANGE_T, end),
		.cbegin = CnxRangeIdentifier(RANGE_T, begin),
		.cend = CnxRangeIdentifier(RANGE_T, end),
		.into_iter = CnxRangeIdentifier(RANGE_T, into_iter),
};

	#define ___DISABLE_IF_NULL(self) \
		cnx_disable_if(!(self), "Can't perform an operation on a null range")

[[nodiscard]] [[not_null(1)]] CnxRangeIdentifier(RANGE_T, Iterator)
	CnxRangeIdentifier(RANGE_T, iterator_new)(const CnxRange(RANGE_T) * restrict self)
		___DISABLE_IF_NULL(self);
[[nodiscard]] [[not_null(1)]] Ref(RANGE_T)
	CnxRangeIdentifier(RANGE_T, iterator_next)(CnxForwardIterator(Ref(RANGE_T)) * restrict self)
		___DISABLE_IF_NULL(self);
[[nodiscard]] [[not_null(1)]] Ref(RANGE_T)
	CnxRangeIdentifier(RANGE_T,
					   iterator_current)(const CnxForwardIterator(Ref(RANGE_T)) * restrict self)
		___DISABLE_IF_NULL(self);
[[nodiscard]] [[not_null(1, 2)]] bool
	CnxRangeIdentifier(RANGE_T,
					   iterator_equals)(const CnxForwardIterator(Ref(RANGE_T)) * restrict self,
										const CnxForwardIterator(Ref(RANGE_T)) * restrict rhs)
		___DISABLE_IF_NULL(self) cnx_disable_if(!rhs, "Can't compare a CnxRange to a nullptr");

ImplIntoCnxForwardIterator(CnxRange(RANGE_T),
						   Ref(RANGE_T),
						   CnxRangeIdentifier(RANGE_T, into_iter),
						   CnxRangeIdentifier(RANGE_T, iterator_new),
						   CnxRangeIdentifier(RANGE_T, iterator_next),
						   CnxRangeIdentifier(RANGE_T, iterator_current),
						   CnxRangeIdentifier(RANGE_T, iterator_equals));

CnxRangeIdentifier(RANGE_T, Iterator)
	CnxRangeIdentifier(RANGE_T, iterator_new)(const CnxRange(RANGE_T) * restrict self) {
	let_mut _self = const_cast(CnxRange(RANGE_T)*)(
		self); /** HACK relying on all Cnx compliant concrete iterator
				* types to have the same       \ layout **/
	let begin = static_cast(CnxRangeIdentifier(RANGE_T, Iterator)*)(self->m_begin.m_self);
	return (CnxRangeIdentifier(RANGE_T, Iterator)){
		.m_range = _self,
		.m_index = begin->m_index,
	};
}

Ref(RANGE_T)
	CnxRangeIdentifier(RANGE_T, iterator_next)(CnxForwardIterator(Ref(RANGE_T)) * restrict self) {
	let_mut _self = static_cast(CnxRangeIdentifier(RANGE_T, Iterator)*)(self->m_self);
	while(!cnx_iterator_equals(_self->m_range->m_current, _self->m_range->m_end)) {
		let current = &cnx_iterator_next(_self->m_range->m_current);
		_self->m_index++;
		if(_self->m_range->m_filter(static_cast(const RANGE_T*)(current))) {
			return current;
		}
	}

	return &cnx_iterator_current(_self->m_range->m_begin);
}

Ref(RANGE_T)
	CnxRangeIdentifier(RANGE_T,
					   iterator_current)(const CnxForwardIterator(Ref(RANGE_T)) * restrict self) {
	let_mut _self = static_cast(const CnxRangeIdentifier(RANGE_T, Iterator)*)(self->m_self);
	return &cnx_iterator_current(_self->m_range->m_current);
}

bool CnxRangeIdentifier(RANGE_T,
						iterator_equals)(const CnxForwardIterator(Ref(RANGE_T)) * restrict self,
										 const CnxForwardIterator(Ref(RANGE_T)) * restrict rhs) {
	let_mut _self = static_cast(const CnxRangeIdentifier(RANGE_T, Iterator)*)(self->m_self);
	let_mut _rhs = static_cast(const CnxRangeIdentifier(RANGE_T, Iterator)*)(rhs->m_self);

	return _self->m_range == _rhs->m_range && _self->m_index == _rhs->m_index;
}

CnxForwardIterator(Ref(RANGE_T))
	CnxRangeIdentifier(RANGE_T, begin)(CnxRange(RANGE_T) * restrict self) {
	let iter = cnx_range_into_iter(*self);
	let_mut inner = static_cast(CnxRangeIdentifier(RANGE_T, Iterator)*)(iter.m_self);
	self->m_current = self->m_begin;
	let_mut current = &cnx_iterator_current(self->m_current);
	do {
		if(self->m_filter(static_cast(const RANGE_T*)(current))) {
			return iter;
		}

		current = &cnx_iterator_next(self->m_current);
		inner->m_index++;
	} while(!cnx_iterator_equals(self->m_current, self->m_end));
	return iter;
}

CnxForwardIterator(Ref(RANGE_T))
	CnxRangeIdentifier(RANGE_T, end)(CnxRange(RANGE_T) * restrict self) {
	let_mut iter = cnx_range_into_iter(*self);
	let_mut inner = static_cast(CnxRangeIdentifier(RANGE_T, Iterator)*)(
		iter.m_self); /** HACK relying on all Cnx compliant concrete iterator types to have the
					   * same       \ layout **/
	let end = static_cast(CnxRangeIdentifier(RANGE_T, Iterator)*)(self->m_end.m_self);
	inner->m_index = end->m_index;
	return iter;
}

CnxRange(RANGE_T) CnxRangeIdentifier(RANGE_T, from)(CnxForwardIterator(Ref(RANGE_T)) begin,
													CnxForwardIterator(Ref(RANGE_T)) end,
													CnxRangeFilter(RANGE_T) filter) {
	return (CnxRange(RANGE_T)){.m_begin = begin,
							   .m_end = end,
							   .m_current = begin,
							   .m_filter = filter,
							   .m_vtable = &CnxRangeIdentifier(RANGE_T, vtable_impl)};
}

bool CnxRangeIdentifier(RANGE_T, default_filter)([[maybe_unused]] const RANGE_T* elem) {
	return true;
}

	#undef ___DISABLE_IF_NULL
	#undef CNX_TEMPLATE_SUPPRESS_INSTANTIATIONS
#endif // defined(RANGE_T) && RANGE_IMPL
