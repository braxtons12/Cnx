/// @file ArrayImpl.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief This module provides the function definitions for a template instantiation of
/// `CnxArray(ARRAY_T, ARRAY_N)`
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

#if defined(ARRAY_T) && defined(ARRAY_N) && ARRAY_IMPL

	#define CNX_TEMPLATE_SUPPRESS_INSTANTIATIONS TRUE

	#include <Cnx/Allocators.h>
	#include <Cnx/BasicTypes.h>
	#include <Cnx/CollectionData.h>
	#include <Cnx/Iterator.h>
	#include <Cnx/Platform.h>
	#include <Cnx/array/ArrayDef.h>
	#include <Cnx/option/OptionDef.h>

ARRAY_STATIC ARRAY_INLINE CnxArrayIterator(ARRAY_T, ARRAY_N)
	CnxArrayIdentifier(ARRAY_T, ARRAY_N, iterator_new)(const CnxArray(ARRAY_T,
																	  ARRAY_N) * restrict self);
ARRAY_STATIC ARRAY_INLINE CnxArrayConstIterator(ARRAY_T, ARRAY_N)
	CnxArrayIdentifier(ARRAY_T,
					   ARRAY_N,
					   const_iterator_new)(const CnxArray(ARRAY_T, ARRAY_N) * restrict self);

ARRAY_STATIC ARRAY_INLINE Ref(ARRAY_T)
	CnxArrayIdentifier(ARRAY_T,
					   ARRAY_N,
					   iterator_next)(CnxRandomAccessIterator(Ref(ARRAY_T)) * restrict self);
ARRAY_STATIC ARRAY_INLINE Ref(ARRAY_T)
	CnxArrayIdentifier(ARRAY_T,
					   ARRAY_N,
					   iterator_previous)(CnxRandomAccessIterator(Ref(ARRAY_T)) * restrict self);
ARRAY_STATIC ARRAY_INLINE Ref(ARRAY_T)
	CnxArrayIdentifier(ARRAY_T,
					   ARRAY_N,
					   iterator_at)(const CnxRandomAccessIterator(Ref(ARRAY_T)) * restrict self,
									usize index);
ARRAY_STATIC ARRAY_INLINE Ref(ARRAY_T)
	CnxArrayIdentifier(ARRAY_T,
					   ARRAY_N,
					   iterator_rat)(const CnxRandomAccessIterator(Ref(ARRAY_T)) * restrict self,
									 usize index);
ARRAY_STATIC ARRAY_INLINE Ref(ARRAY_T) CnxArrayIdentifier(ARRAY_T, ARRAY_N, iterator_current)(
	const CnxRandomAccessIterator(Ref(ARRAY_T)) * restrict self);
ARRAY_STATIC ARRAY_INLINE bool
	CnxArrayIdentifier(ARRAY_T,
					   ARRAY_N,
					   iterator_equals)(const CnxRandomAccessIterator(Ref(ARRAY_T)) * restrict self,
										const CnxRandomAccessIterator(Ref(ARRAY_T)) * restrict rhs);
ARRAY_STATIC ARRAY_INLINE ConstRef(ARRAY_T)
	CnxArrayIdentifier(ARRAY_T,
					   ARRAY_N,
					   iterator_cnext)(CnxRandomAccessIterator(ConstRef(ARRAY_T)) * restrict self);
ARRAY_STATIC
	ARRAY_INLINE ConstRef(ARRAY_T) CnxArrayIdentifier(ARRAY_T, ARRAY_N, iterator_cprevious)(
		CnxRandomAccessIterator(ConstRef(ARRAY_T)) * restrict self);
ARRAY_STATIC ARRAY_INLINE ConstRef(ARRAY_T) CnxArrayIdentifier(ARRAY_T, ARRAY_N, iterator_cat)(
	const CnxRandomAccessIterator(ConstRef(ARRAY_T)) * restrict self,
	usize index);
ARRAY_STATIC ARRAY_INLINE ConstRef(ARRAY_T) CnxArrayIdentifier(ARRAY_T, ARRAY_N, iterator_crat)(
	const CnxRandomAccessIterator(ConstRef(ARRAY_T)) * restrict self,
	usize index);
ARRAY_STATIC ARRAY_INLINE ConstRef(ARRAY_T) CnxArrayIdentifier(ARRAY_T, ARRAY_N, iterator_ccurrent)(
	const CnxRandomAccessIterator(ConstRef(ARRAY_T)) * restrict self);
ARRAY_STATIC ARRAY_INLINE bool CnxArrayIdentifier(ARRAY_T, ARRAY_N, iterator_cequals)(
	const CnxRandomAccessIterator(ConstRef(ARRAY_T)) * restrict self,
	const CnxRandomAccessIterator(ConstRef(ARRAY_T)) * restrict rhs);

ImplIntoCnxRandomAccessIterator(CnxArray(ARRAY_T, ARRAY_N),
								Ref(ARRAY_T),
								CnxArrayIdentifier(ARRAY_T, ARRAY_N, into_iter),
								into,
								CnxArrayIdentifier(ARRAY_T, ARRAY_N, iterator_new),
								CnxArrayIdentifier(ARRAY_T, ARRAY_N, iterator_next),
								CnxArrayIdentifier(ARRAY_T, ARRAY_N, iterator_previous),
								CnxArrayIdentifier(ARRAY_T, ARRAY_N, iterator_at),
								CnxArrayIdentifier(ARRAY_T, ARRAY_N, iterator_current),
								CnxArrayIdentifier(ARRAY_T, ARRAY_N, iterator_equals));
ImplIntoCnxRandomAccessIterator(CnxArray(ARRAY_T, ARRAY_N),
								Ref(ARRAY_T),
								CnxArrayIdentifier(ARRAY_T, ARRAY_N, into_reverse_iter),
								into_reverse,
								CnxArrayIdentifier(ARRAY_T, ARRAY_N, iterator_new),
								CnxArrayIdentifier(ARRAY_T, ARRAY_N, iterator_next),
								CnxArrayIdentifier(ARRAY_T, ARRAY_N, iterator_previous),
								CnxArrayIdentifier(ARRAY_T, ARRAY_N, iterator_rat),
								CnxArrayIdentifier(ARRAY_T, ARRAY_N, iterator_current),
								CnxArrayIdentifier(ARRAY_T, ARRAY_N, iterator_equals));

ImplIntoCnxRandomAccessIterator(CnxArray(ARRAY_T, ARRAY_N),
								ConstRef(ARRAY_T),
								CnxArrayIdentifier(ARRAY_T, ARRAY_N, into_const_iter),
								into,
								CnxArrayIdentifier(ARRAY_T, ARRAY_N, const_iterator_new),
								CnxArrayIdentifier(ARRAY_T, ARRAY_N, iterator_cnext),
								CnxArrayIdentifier(ARRAY_T, ARRAY_N, iterator_cprevious),
								CnxArrayIdentifier(ARRAY_T, ARRAY_N, iterator_cat),
								CnxArrayIdentifier(ARRAY_T, ARRAY_N, iterator_ccurrent),
								CnxArrayIdentifier(ARRAY_T, ARRAY_N, iterator_cequals));
ImplIntoCnxRandomAccessIterator(CnxArray(ARRAY_T, ARRAY_N),
								ConstRef(ARRAY_T),
								CnxArrayIdentifier(ARRAY_T, ARRAY_N, into_reverse_const_iter),
								into_reverse,
								CnxArrayIdentifier(ARRAY_T, ARRAY_N, const_iterator_new),
								CnxArrayIdentifier(ARRAY_T, ARRAY_N, iterator_cnext),
								CnxArrayIdentifier(ARRAY_T, ARRAY_N, iterator_cprevious),
								CnxArrayIdentifier(ARRAY_T, ARRAY_N, iterator_crat),
								CnxArrayIdentifier(ARRAY_T, ARRAY_N, iterator_ccurrent),
								CnxArrayIdentifier(ARRAY_T, ARRAY_N, iterator_cequals));

__attr(always_inline) static inline ARRAY_T
	CnxArrayIdentifier(ARRAY_T, ARRAY_N, default_constructor)(__attr(maybe_unused)
																  CnxAllocator allocator) {
	return (ARRAY_T){0};
}

__attr(always_inline) static inline ARRAY_T
	CnxArrayIdentifier(ARRAY_T, ARRAY_N, default_copy_constructor)(const ARRAY_T* restrict element,
																   __attr(maybe_unused)
																	   CnxAllocator allocator) {
	return *element;
}

__attr(always_inline) static inline void CnxArrayIdentifier(ARRAY_T, ARRAY_N, default_destructor)(
	__attr(maybe_unused) ARRAY_T* restrict element, /** NOLINT(readability-non-const-parameter) **/
	__attr(maybe_unused) CnxAllocator allocator) {
}

static const struct CnxArrayIdentifier(ARRAY_T, ARRAY_N, vtable)
	CnxArrayIdentifier(ARRAY_T, ARRAY_N, vtable_impl)
	= {
		.clone = CnxArrayIdentifier(ARRAY_T, ARRAY_N, clone),
		.at_const = CnxArrayIdentifier(ARRAY_T, ARRAY_N, at_const),
		.at_mut = CnxArrayIdentifier(ARRAY_T, ARRAY_N, at_mut),
		.front_const = CnxArrayIdentifier(ARRAY_T, ARRAY_N, front_const),
		.front_mut = CnxArrayIdentifier(ARRAY_T, ARRAY_N, front_mut),
		.back_const = CnxArrayIdentifier(ARRAY_T, ARRAY_N, back_const),
		.back_mut = CnxArrayIdentifier(ARRAY_T, ARRAY_N, back_mut),
		.data_const = CnxArrayIdentifier(ARRAY_T, ARRAY_N, data_const),
		.data_mut = CnxArrayIdentifier(ARRAY_T, ARRAY_N, data_mut),
		.is_empty = CnxArrayIdentifier(ARRAY_T, ARRAY_N, is_empty),
		.is_full = CnxArrayIdentifier(ARRAY_T, ARRAY_N, is_full),
		.size = CnxArrayIdentifier(ARRAY_T, ARRAY_N, size),
		.capacity = CnxArrayIdentifier(ARRAY_T, ARRAY_N, capacity),
		.resize = CnxArrayIdentifier(ARRAY_T, ARRAY_N, resize),
		.clear = CnxArrayIdentifier(ARRAY_T, ARRAY_N, clear),
		.push_back = CnxArrayIdentifier(ARRAY_T, ARRAY_N, push_back),
		.pop_back = CnxArrayIdentifier(ARRAY_T, ARRAY_N, pop_back),
		.insert = CnxArrayIdentifier(ARRAY_T, ARRAY_N, insert),
		.erase = CnxArrayIdentifier(ARRAY_T, ARRAY_N, erase),
		.erase_n = CnxArrayIdentifier(ARRAY_T, ARRAY_N, erase_n),
		.free = CnxArrayIdentifier(ARRAY_T, ARRAY_N, free),
		.into_iter = CnxArrayIdentifier(ARRAY_T, ARRAY_N, into_iter),
		.into_reverse_iter = CnxArrayIdentifier(ARRAY_T, ARRAY_N, into_reverse_iter),
		.into_const_iter = CnxArrayIdentifier(ARRAY_T, ARRAY_N, into_const_iter),
		.into_reverse_const_iter = CnxArrayIdentifier(ARRAY_T, ARRAY_N, into_reverse_const_iter),
		.begin = CnxArrayIdentifier(ARRAY_T, ARRAY_N, begin),
		.end = CnxArrayIdentifier(ARRAY_T, ARRAY_N, end),
		.rbegin = CnxArrayIdentifier(ARRAY_T, ARRAY_N, rbegin),
		.rend = CnxArrayIdentifier(ARRAY_T, ARRAY_N, rend),
		.cbegin = CnxArrayIdentifier(ARRAY_T, ARRAY_N, cbegin),
		.cend = CnxArrayIdentifier(ARRAY_T, ARRAY_N, cend),
		.crbegin = CnxArrayIdentifier(ARRAY_T, ARRAY_N, crbegin),
		.crend = CnxArrayIdentifier(ARRAY_T, ARRAY_N, crend),
};

static const struct CnxCollectionData(CnxArray(ARRAY_T, ARRAY_N))
	CnxArrayIdentifier(ARRAY_T, ARRAY_N, default_collection_data)
	= {.m_constructor = CnxArrayIdentifier(ARRAY_T, ARRAY_N, default_constructor),
	   .m_copy_constructor = CnxArrayIdentifier(ARRAY_T, ARRAY_N, default_copy_constructor),
	   .m_destructor = CnxArrayIdentifier(ARRAY_T, ARRAY_N, default_destructor)};

ARRAY_STATIC ARRAY_INLINE CnxArray(ARRAY_T, ARRAY_N)
	CnxArrayIdentifier(ARRAY_T, ARRAY_N, new)(void) {
	return cnx_array_new_with_allocator_and_collection_data(
		ARRAY_T,
		ARRAY_N,
		DEFAULT_ALLOCATOR,
		&CnxArrayIdentifier(ARRAY_T, ARRAY_N, default_collection_data));
}

ARRAY_STATIC ARRAY_INLINE CnxArray(ARRAY_T, ARRAY_N)
	CnxArrayIdentifier(ARRAY_T, ARRAY_N, new_with_allocator)(CnxAllocator allocator) {
	return cnx_array_new_with_allocator_and_collection_data(
		ARRAY_T,
		ARRAY_N,
		allocator,
		&CnxArrayIdentifier(ARRAY_T, ARRAY_N, default_collection_data));
}

ARRAY_STATIC ARRAY_INLINE CnxArray(ARRAY_T, ARRAY_N)
	CnxArrayIdentifier(ARRAY_T, ARRAY_N, new_with_collection_data)(
		const CnxCollectionData(CnxArray(ARRAY_T, ARRAY_N)) * restrict data) {
	return cnx_array_new_with_allocator_and_collection_data(ARRAY_T,
															ARRAY_N,
															DEFAULT_ALLOCATOR,
															data);
}

ARRAY_STATIC ARRAY_INLINE CnxArray(ARRAY_T, ARRAY_N)
	CnxArrayIdentifier(ARRAY_T, ARRAY_N, new_with_allocator_and_collection_data)(
		CnxAllocator allocator,
		const CnxCollectionData(CnxArray(ARRAY_T, ARRAY_N)) * restrict data) {
	return (CnxArray(ARRAY_T, ARRAY_N)){.m_array = {},
										.m_size = static_cast(usize)(0),
										.m_allocator = allocator,
										.m_data = data,
										.m_vtable
										= &CnxArrayIdentifier(ARRAY_T, ARRAY_N, vtable_impl)};
}

ARRAY_STATIC ARRAY_INLINE CnxArray(ARRAY_T, ARRAY_N)
	CnxArrayIdentifier(ARRAY_T, ARRAY_N, clone)(const CnxArray(ARRAY_T, ARRAY_N) * restrict self)
		cnx_disable_if(
			!(self->m_data->m_copy_constructor),
			"Can't clone a CnxArray(ARRAY_T, ARRAY_N) with elements that aren't copyable (no "
			"element copy constructor defined)") {
	cnx_assert(self->m_data->m_copy_constructor != nullptr,
			   "Can't clone a `CnxArray(ARRAY_T, ARRAY_N)` with elements that aren't copyable (no "
			   "element copy-constructor defined)");

	let_mut array = cnx_array_new_with_collection_data(ARRAY_T, ARRAY_N, self->m_data);

	foreach_ref(elem, *self) {
		cnx_array_push_back(array, self->m_data->m_copy_constructor(elem, self->m_allocator));
	}

	return array;
}

ARRAY_STATIC ARRAY_INLINE const ARRAY_T*
CnxArrayIdentifier(ARRAY_T, ARRAY_N, at_const)(const CnxArray(ARRAY_T, ARRAY_N) * restrict self,
											   usize index) {
	cnx_assert(index <= self->m_size,
			   "cnx_array_at_mut called with index > size (index out of bounds)");
	cnx_assert(index < ARRAY_N, "cnx_array_at_mut called with index >= N (index out of bounds)");
	return &(self->m_array[index]);
}

ARRAY_STATIC ARRAY_INLINE ARRAY_T*
CnxArrayIdentifier(ARRAY_T, ARRAY_N, at_mut)(CnxArray(ARRAY_T, ARRAY_N) * restrict self,
											 usize index) {
	cnx_assert(index <= self->m_size,
			   "cnx_array_at_mut called with index > size (index out of bounds)");
	cnx_assert(index < ARRAY_N, "cnx_array_at_mut called with index >= N (index out of bounds)");
	return &(self->m_array[index]);
}

ARRAY_STATIC ARRAY_INLINE const ARRAY_T*
CnxArrayIdentifier(ARRAY_T, ARRAY_N, front_const)(const CnxArray(ARRAY_T,
																 ARRAY_N) * restrict self) {
	return &cnx_array_at(*self, 0);
}

ARRAY_STATIC ARRAY_INLINE ARRAY_T*
CnxArrayIdentifier(ARRAY_T, ARRAY_N, front_mut)(CnxArray(ARRAY_T, ARRAY_N) * restrict self) {
	return &cnx_array_at_mut(*self, 0);
}

ARRAY_STATIC ARRAY_INLINE const ARRAY_T*
CnxArrayIdentifier(ARRAY_T, ARRAY_N, back_const)(const CnxArray(ARRAY_T, ARRAY_N) * restrict self) {
	let back_index = self->m_size > 0 ? self->m_size - 1 : 0;
	return &cnx_array_at(*self, back_index);
}

ARRAY_STATIC ARRAY_INLINE ARRAY_T*
CnxArrayIdentifier(ARRAY_T, ARRAY_N, back_mut)(CnxArray(ARRAY_T, ARRAY_N) * restrict self) {
	let back_index = self->m_size > 0 ? self->m_size - 1 : 0;
	return &cnx_array_at_mut(*self, back_index);
}

ARRAY_STATIC ARRAY_INLINE const ARRAY_T*
CnxArrayIdentifier(ARRAY_T, ARRAY_N, data_const)(const CnxArray(ARRAY_T, ARRAY_N) * restrict self) {
	return &cnx_array_front(*self);
}

ARRAY_STATIC ARRAY_INLINE ARRAY_T*
CnxArrayIdentifier(ARRAY_T, ARRAY_N, data_mut)(CnxArray(ARRAY_T, ARRAY_N) * restrict self) {
	return &cnx_array_front_mut(*self);
}

ARRAY_STATIC ARRAY_INLINE bool
CnxArrayIdentifier(ARRAY_T, ARRAY_N, is_empty)(const CnxArray(ARRAY_T, ARRAY_N) * restrict self) {
	return self->m_size == 0;
}

ARRAY_STATIC ARRAY_INLINE bool
CnxArrayIdentifier(ARRAY_T, ARRAY_N, is_full)(const CnxArray(ARRAY_T, ARRAY_N) * restrict self) {
	return self->m_size == ARRAY_N;
}

ARRAY_STATIC ARRAY_INLINE usize
CnxArrayIdentifier(ARRAY_T, ARRAY_N, size)(const CnxArray(ARRAY_T, ARRAY_N) * restrict self) {
	return self->m_size;
}

ARRAY_STATIC ARRAY_INLINE usize
CnxArrayIdentifier(ARRAY_T, ARRAY_N, capacity)(const CnxArray(ARRAY_T, ARRAY_N) * restrict self) {
	ignore(self);
	return ARRAY_N;
}

ARRAY_STATIC ARRAY_INLINE void
CnxArrayIdentifier(ARRAY_T, ARRAY_N, resize_internal)(CnxArray(ARRAY_T, ARRAY_N) * restrict self,
													  usize new_size) {
	if(new_size < self->m_size) {
		for(let_mut i = new_size; i < self->m_size; ++i) {
			self->m_data->m_destructor(&cnx_array_at_mut(*self, i), self->m_allocator);
		}
		self->m_size = new_size;
	}
	else {
		let old_size = self->m_size;
		self->m_size = new_size;
		// cast away const for GCC compat
		for(let_mut i = const_cast(usize)(old_size); i < new_size; ++i) {
			cnx_array_at_mut(*self, i) = self->m_data->m_constructor(self->m_allocator);
		}
	}
}

ARRAY_STATIC ARRAY_INLINE void
CnxArrayIdentifier(ARRAY_T, ARRAY_N, resize)(CnxArray(ARRAY_T, ARRAY_N) * restrict self,
											 usize new_size) {
	cnx_assert(new_size <= ARRAY_N, "can't resize an array larger than it's capacity");
	CnxArrayIdentifier(ARRAY_T, ARRAY_N, resize_internal)(self, new_size);
}

ARRAY_STATIC ARRAY_INLINE void
CnxArrayIdentifier(ARRAY_T, ARRAY_N, clear)(CnxArray(ARRAY_T, ARRAY_N) * restrict self) {
	for(let_mut i = 0U; i < self->m_size; ++i) {
		self->m_data->m_destructor(&cnx_array_at_mut(*self, i), self->m_allocator);
	}
	self->m_size = 0;
}

ARRAY_STATIC ARRAY_INLINE void
CnxArrayIdentifier(ARRAY_T,
				   ARRAY_N,
				   push_back)(CnxArray(ARRAY_T, ARRAY_N) * restrict self,
							  ARRAY_T element /** NOLINT(readability-non-const-parameter) **/) {
	cnx_assert(self->m_size < ARRAY_N,
			   "cnx_array_push_back called with self->m_size >= N (array bounds full)");
	cnx_array_at_mut(*self, self->m_size) = element;
	self->m_size++;
}

ARRAY_STATIC ARRAY_INLINE CnxOption(ARRAY_T)
	CnxArrayIdentifier(ARRAY_T, ARRAY_N, pop_back)(CnxArray(ARRAY_T, ARRAY_N) * restrict self) {
	if(self->m_size == 0) {
		return None(ARRAY_T);
	}

	let_mut ptr = &cnx_array_at_mut(*self, self->m_size - 1);
	let elem = Some(ARRAY_T, *ptr);
	self->m_data->m_destructor(ptr, self->m_allocator);
	*ptr = (ARRAY_T){0};
	self->m_size--;
	return elem;
}

ARRAY_STATIC ARRAY_INLINE void
CnxArrayIdentifier(ARRAY_T,
				   ARRAY_N,
				   insert)(CnxArray(ARRAY_T, ARRAY_N) * restrict self,
						   ARRAY_T element /** NOLINT(readability-non-const-parameter **/,
						   usize index) {
	cnx_assert(index < ARRAY_N, "cnx_array_insert called with index >= N (index out of bounds)");
	cnx_assert(self->m_size < ARRAY_N,
			   "cnx_array_insert called with self->m_size >= N (array bounds full)");

	if(index != self->m_size) {
		let num_to_move = self->m_size - index;
		cnx_memmove(ARRAY_T,
					&cnx_array_at_mut(*self, index + 1),
					&cnx_array_at_mut(*self, index),
					num_to_move);
	}
	cnx_array_at_mut(*self, index) = element;
	self->m_size++;
}

ARRAY_STATIC ARRAY_INLINE void
CnxArrayIdentifier(ARRAY_T, ARRAY_N, erase)(CnxArray(ARRAY_T, ARRAY_N) * restrict self,
											usize index) {
	cnx_assert(index < self->m_size,
			   "cnx_array_erase called with index >= self->m_size (index out of bounds)");
	self->m_data->m_destructor(&cnx_array_at_mut(*self, index), self->m_allocator);
	if(index != self->m_size - 1) {
		let num_to_move = self->m_size - (index + 1);
		cnx_memmove(ARRAY_T,
					&cnx_array_at_mut(*self, index),
					&cnx_array_at_mut(*self, index + 1),
					num_to_move);
	}
	self->m_size--;
}

ARRAY_STATIC ARRAY_INLINE void
CnxArrayIdentifier(ARRAY_T, ARRAY_N, erase_n)(CnxArray(ARRAY_T, ARRAY_N) * restrict self,
											  usize index,
											  usize length) {
	cnx_assert(index < self->m_size,
			   "cnx_array_erase_n called with index >= self->m_size (index out of bounds)");
	cnx_assert(index + length < self->m_size,
			   "cnx_array_erase_n called with index + length >= self->m_size (range out "
			   "of bounds)");

	let end = index + length;
	let num_to_move = self->m_size - end;
	for(let_mut i = index; i < end; ++i) {
		self->m_data->m_destructor(&cnx_array_at_mut(*self, i), self->m_allocator);
	}
	if(end != self->m_size) {
		cnx_memmove(ARRAY_T,
					&cnx_array_at_mut(*self, index),
					&cnx_array_at_mut(*self, end),
					num_to_move);
	}
	self->m_size -= length;
}

ARRAY_STATIC ARRAY_INLINE void CnxArrayIdentifier(ARRAY_T, ARRAY_N, free)(void* restrict self) {
	let self_ = static_cast(CnxArray(ARRAY_T, ARRAY_N)*)(self);
	for(let_mut i = 0U; i < self_->m_size; ++i) {
		self_->m_data->m_destructor(&cnx_array_at_mut(*self_, i), self_->m_allocator);
	}
	self_->m_size = 0;
}

ARRAY_STATIC ARRAY_INLINE CnxArrayIterator(ARRAY_T, ARRAY_N)
	CnxArrayIdentifier(ARRAY_T, ARRAY_N, iterator_new)(const CnxArray(ARRAY_T,
																	  ARRAY_N) * restrict self) {
	return (CnxArrayIterator(ARRAY_T, ARRAY_N)){.m_index = 0U,
												.m_array
												= const_cast(CnxArray(ARRAY_T, ARRAY_N)*)(self)};
}

ARRAY_STATIC ARRAY_INLINE CnxArrayConstIterator(ARRAY_T, ARRAY_N)
	CnxArrayIdentifier(ARRAY_T,
					   ARRAY_N,
					   const_iterator_new)(const CnxArray(ARRAY_T, ARRAY_N) * restrict self) {
	return (CnxArrayConstIterator(ARRAY_T, ARRAY_N)){
		.m_index = 0U,
		.m_array = const_cast(CnxArray(ARRAY_T, ARRAY_N)*)(self)};
}

ARRAY_STATIC ARRAY_INLINE Ref(ARRAY_T)
	CnxArrayIdentifier(ARRAY_T,
					   ARRAY_N,
					   iterator_next)(CnxRandomAccessIterator(Ref(ARRAY_T)) * restrict self) {
	let _self = static_cast(CnxArrayIterator(ARRAY_T, ARRAY_N)*)(self->m_self);
	cnx_assert(_self->m_index > -1,
			   "Iterator value accessed when iterator is positioned before the beginning of "
			   "the iteration (iterator out of bounds)");
	cnx_assert(static_cast(usize)(_self->m_index) < cnx_array_size(*(_self->m_array)),
			   "Iterator value accessed when iterator is positioned after the end of "
			   "the iteration (iterator out of bounds)");
	_self->m_index++;
	if(static_cast(usize)(_self->m_index) >= cnx_array_size(*(_self->m_array))) {
		return &cnx_array_at_mut(*(_self->m_array), cnx_array_size(*(_self->m_array)) - 1);
	}

	return &cnx_array_at_mut(*(_self->m_array), static_cast(usize)(_self->m_index));
}

ARRAY_STATIC ARRAY_INLINE Ref(ARRAY_T)
	CnxArrayIdentifier(ARRAY_T,
					   ARRAY_N,
					   iterator_previous)(CnxRandomAccessIterator(Ref(ARRAY_T)) * restrict self) {
	let _self = static_cast(CnxArrayIterator(ARRAY_T, ARRAY_N)*)(self->m_self);
	cnx_assert(_self->m_index > -1,
			   "Iterator value accessed when iterator is positioned before the beginning of "
			   "the iteration (iterator out of bounds)");
	cnx_assert(static_cast(usize)(_self->m_index) < cnx_array_size(*(_self->m_array)),
			   "Iterator value accessed when iterator is positioned after the end of the "
			   "iteration (iterator out of bounds)");
	_self->m_index--;
	if(_self->m_index < 0) {
		return &cnx_array_at_mut(*(_self->m_array), 0);
	}

	return &cnx_array_at_mut(*(_self->m_array), static_cast(usize)(_self->m_index));
}

ARRAY_STATIC ARRAY_INLINE Ref(ARRAY_T)
	CnxArrayIdentifier(ARRAY_T,
					   ARRAY_N,
					   iterator_at)(const CnxRandomAccessIterator(Ref(ARRAY_T)) * restrict self,
									usize index) {
	let _self = static_cast(CnxArrayIterator(ARRAY_T, ARRAY_N)*)(self->m_self);
	cnx_assert(index < cnx_array_size(*(_self->m_array)),
			   "Iterator value accessed when iterator is positioned after the end of "
			   "the iteration (iterator out of bounds)");
	return &cnx_array_at_mut(*(_self->m_array), index);
}

ARRAY_STATIC ARRAY_INLINE Ref(ARRAY_T)
	CnxArrayIdentifier(ARRAY_T,
					   ARRAY_N,
					   iterator_rat)(const CnxRandomAccessIterator(Ref(ARRAY_T)) * restrict self,
									 usize index) {
	let _self = static_cast(CnxArrayIterator(ARRAY_T, ARRAY_N)*)(self->m_self);
	cnx_assert(index < cnx_array_size(*(_self->m_array)),
			   "Iterator value accessed when iterator is positioned "
			   "after the end of the iteration (iterator out of bounds)");
	return &cnx_array_at_mut(*(_self->m_array), (cnx_array_size(*(_self->m_array)) - 1) - index);
}

ARRAY_STATIC ARRAY_INLINE Ref(ARRAY_T) CnxArrayIdentifier(ARRAY_T, ARRAY_N, iterator_current)(
	const CnxRandomAccessIterator(Ref(ARRAY_T)) * restrict self) {
	let _self = static_cast(CnxArrayIterator(ARRAY_T, ARRAY_N)*)(self->m_self);
	cnx_assert(_self->m_index > -1,
			   "Iterator value accessed when iterator is positioned before the "
			   "beginning of the iteration (iterator out of bounds)");
	cnx_assert(static_cast(usize)(_self->m_index) < cnx_array_size(*(_self->m_array)),
			   "Iterator value accessed when iterator is positioned after the "
			   "end of the iteration (iterator out of bounds)");
	return &cnx_array_at_mut(*(_self->m_array), static_cast(usize)(_self->m_index));
}

ARRAY_STATIC ARRAY_INLINE bool
CnxArrayIdentifier(ARRAY_T,
				   ARRAY_N,
				   iterator_equals)(const CnxRandomAccessIterator(Ref(ARRAY_T)) * restrict self,
									const CnxRandomAccessIterator(Ref(ARRAY_T)) * restrict rhs) {
	let _self = static_cast(const CnxArrayIterator(ARRAY_T, ARRAY_N)*)(self->m_self);
	let _rhs = static_cast(const CnxArrayIterator(ARRAY_T, ARRAY_N)*)(rhs->m_self);
	return _self->m_index == _rhs->m_index && _self->m_array == _rhs->m_array;
}

ARRAY_STATIC ARRAY_INLINE ConstRef(ARRAY_T)
	CnxArrayIdentifier(ARRAY_T,
					   ARRAY_N,
					   iterator_cnext)(CnxRandomAccessIterator(ConstRef(ARRAY_T)) * restrict self) {
	let _self = static_cast(CnxArrayConstIterator(ARRAY_T, ARRAY_N)*)(self->m_self);

	cnx_assert(_self->m_index > -1,
			   "Iterator value accessed when iterator is positioned before the "
			   "beginning of the iteration (iterator out of bounds)");
	cnx_assert(static_cast(usize)(_self->m_index) < cnx_array_size(*(_self->m_array)),
			   "Iterator value accessed when iterator is positioned after the "
			   "end of the iteration (iterator out of bounds)");
	_self->m_index++;
	if(static_cast(usize)(_self->m_index) >= cnx_array_size(*(_self->m_array))) {
		return &cnx_array_at(*(_self->m_array), cnx_array_size(*(_self->m_array)) - 1);
	}

	return &cnx_array_at(*(_self->m_array), static_cast(usize)(_self->m_index));
}

ARRAY_STATIC ARRAY_INLINE ConstRef(ARRAY_T)
	CnxArrayIdentifier(ARRAY_T, ARRAY_N, iterator_cprevious)(
		CnxRandomAccessIterator(ConstRef(ARRAY_T)) * restrict self) {
	let _self = static_cast(CnxArrayConstIterator(ARRAY_T, ARRAY_N)*)(self->m_self);
	cnx_assert(_self->m_index > -1,
			   "Iterator value accessed when iterator is positioned before "
			   "the beginning of the iteration (iterator out of bounds)");
	cnx_assert(static_cast(usize)(_self->m_index) < cnx_array_size(*(_self->m_array)),
			   "Iterator value accessed when iterator is positioned after "
			   "the end of the iteration (iterator out of bounds)");
	_self->m_index--;
	if(_self->m_index < 0) {
		return &cnx_array_at(*(_self->m_array), 0);
	}

	return &cnx_array_at(*(_self->m_array), static_cast(usize)(_self->m_index));
}

ARRAY_STATIC ARRAY_INLINE ConstRef(ARRAY_T) CnxArrayIdentifier(ARRAY_T, ARRAY_N, iterator_cat)(
	const CnxRandomAccessIterator(ConstRef(ARRAY_T)) * restrict self,
	usize index) {
	let _self = static_cast(CnxArrayConstIterator(ARRAY_T, ARRAY_N)*)(self->m_self);
	cnx_assert(index < cnx_array_size(*(_self->m_array)),
			   "Iterator value accessed when iterator is positioned "
			   "after the end of the iteration (iterator out of bounds)");

	return &cnx_array_at(*(_self->m_array), index);
}

ARRAY_STATIC ARRAY_INLINE ConstRef(ARRAY_T) CnxArrayIdentifier(ARRAY_T, ARRAY_N, iterator_crat)(
	const CnxRandomAccessIterator(ConstRef(ARRAY_T)) * restrict self,
	usize index) {
	let _self = static_cast(CnxArrayConstIterator(ARRAY_T, ARRAY_N)*)(self->m_self);
	cnx_assert(index < cnx_array_size(*(_self->m_array)),
			   "Iterator value accessed when iterator is positioned "
			   "after the end of the iteration (iterator out of bounds)");

	return &cnx_array_at(*(_self->m_array), (cnx_array_size(*(_self->m_array)) - 1) - index);
}

ARRAY_STATIC ARRAY_INLINE ConstRef(ARRAY_T) CnxArrayIdentifier(ARRAY_T, ARRAY_N, iterator_ccurrent)(
	const CnxRandomAccessIterator(ConstRef(ARRAY_T)) * restrict self) {
	let _self = static_cast(CnxArrayConstIterator(ARRAY_T, ARRAY_N)*)(self->m_self);
	cnx_assert(_self->m_index > -1,
			   "Iterator value accessed when iterator is positioned before "
			   "the beginning of the iteration (iterator out of bounds)");
	cnx_assert(static_cast(usize)(_self->m_index) < cnx_array_size(*(_self->m_array)),
			   "Iterator value accessed when iterator is positioned after "
			   "the end of the iteration (iterator out of bounds)");

	return &cnx_array_at(*(_self->m_array), static_cast(usize)(_self->m_index));
}

ARRAY_STATIC ARRAY_INLINE bool CnxArrayIdentifier(ARRAY_T, ARRAY_N, iterator_cequals)(
	const CnxRandomAccessIterator(ConstRef(ARRAY_T)) * restrict self,
	const CnxRandomAccessIterator(ConstRef(ARRAY_T)) * restrict rhs) {
	let _self = static_cast(const CnxArrayConstIterator(ARRAY_T, ARRAY_N)*)(self->m_self);
	let _rhs = static_cast(const CnxArrayConstIterator(ARRAY_T, ARRAY_N)*)(rhs->m_self);
	return _self->m_index == _rhs->m_index && _self->m_array == _rhs->m_array;
}

ARRAY_STATIC ARRAY_INLINE CnxRandomAccessIterator(Ref(ARRAY_T))
	CnxArrayIdentifier(ARRAY_T, ARRAY_N, begin)(CnxArray(ARRAY_T, ARRAY_N) * restrict self) {
	let_mut iter = cnx_array_into_iter(*self);
	let_mut inner = static_cast(CnxArrayIterator(ARRAY_T, ARRAY_N)*)(iter.m_self);
	inner->m_index = 0;
	return iter;
}

ARRAY_STATIC ARRAY_INLINE CnxRandomAccessIterator(Ref(ARRAY_T))
	CnxArrayIdentifier(ARRAY_T, ARRAY_N, end)(CnxArray(ARRAY_T, ARRAY_N) * restrict self) {
	let_mut iter = cnx_array_into_iter(*self);
	let_mut inner = static_cast(CnxArrayIterator(ARRAY_T, ARRAY_N)*)(iter.m_self);
	inner->m_index = static_cast(isize)(cnx_array_size(*self));
	return iter;
}

ARRAY_STATIC ARRAY_INLINE CnxRandomAccessIterator(Ref(ARRAY_T))
	CnxArrayIdentifier(ARRAY_T, ARRAY_N, rbegin)(CnxArray(ARRAY_T, ARRAY_N) * restrict self) {
	let_mut iter = cnx_array_into_reverse_iter(*self);
	let_mut inner = static_cast(CnxArrayIterator(ARRAY_T, ARRAY_N)*)(iter.m_self);
	inner->m_index = static_cast(isize)(cnx_array_size(*self) - 1);
	return iter;
}

ARRAY_STATIC ARRAY_INLINE CnxRandomAccessIterator(Ref(ARRAY_T))
	CnxArrayIdentifier(ARRAY_T, ARRAY_N, rend)(CnxArray(ARRAY_T, ARRAY_N) * restrict self) {
	let_mut iter = cnx_array_into_reverse_iter(*self);
	let_mut inner = static_cast(CnxArrayIterator(ARRAY_T, ARRAY_N)*)(iter.m_self);
	inner->m_index = -1;
	return iter;
}

ARRAY_STATIC ARRAY_INLINE CnxRandomAccessIterator(ConstRef(ARRAY_T))
	CnxArrayIdentifier(ARRAY_T, ARRAY_N, cbegin)(const CnxArray(ARRAY_T, ARRAY_N) * restrict self) {
	let_mut iter = cnx_array_into_const_iter(*self);
	let_mut inner = static_cast(CnxArrayConstIterator(ARRAY_T, ARRAY_N)*)(iter.m_self);
	inner->m_index = 0;
	return iter;
}

ARRAY_STATIC ARRAY_INLINE CnxRandomAccessIterator(ConstRef(ARRAY_T))
	CnxArrayIdentifier(ARRAY_T, ARRAY_N, cend)(const CnxArray(ARRAY_T, ARRAY_N) * restrict self) {
	let_mut iter = cnx_array_into_const_iter(*self);
	let_mut inner = static_cast(CnxArrayConstIterator(ARRAY_T, ARRAY_N)*)(iter.m_self);
	inner->m_index = static_cast(isize)(cnx_array_size(*self));
	return iter;
}

ARRAY_STATIC ARRAY_INLINE CnxRandomAccessIterator(ConstRef(ARRAY_T))
	CnxArrayIdentifier(ARRAY_T, ARRAY_N, crbegin)(const CnxArray(ARRAY_T,
																 ARRAY_N) * restrict self) {
	let_mut iter = cnx_array_into_reverse_const_iter(*self);
	let_mut inner = static_cast(CnxArrayConstIterator(ARRAY_T, ARRAY_N)*)(iter.m_self);
	inner->m_index = static_cast(isize)(cnx_array_size(*self) - 1);
	return iter;
}

ARRAY_STATIC ARRAY_INLINE CnxRandomAccessIterator(ConstRef(ARRAY_T))
	CnxArrayIdentifier(ARRAY_T, ARRAY_N, crend)(const CnxArray(ARRAY_T, ARRAY_N) * restrict self) {
	let_mut iter = cnx_array_into_reverse_const_iter(*self);
	let_mut inner = static_cast(CnxArrayConstIterator(ARRAY_T, ARRAY_N)*)(iter.m_self);
	inner->m_index = -1;
	return iter;
}

ARRAY_STATIC ARRAY_INLINE CnxString CnxArrayIdentifier(ARRAY_T,
													   ARRAY_N,
													   format)(const CnxFormat* restrict self,
															   CnxFormatSpecifier specifier) {
	return CnxArrayIdentifier(ARRAY_T, ARRAY_N, format_with_allocator)(self,
																	   specifier,
																	   DEFAULT_ALLOCATOR);
}

ARRAY_STATIC ARRAY_INLINE CnxString CnxArrayIdentifier(ARRAY_T, ARRAY_N, format_with_allocator)(
	const CnxFormat* restrict self,
	__attr(maybe_unused) CnxFormatSpecifier specifier,
	CnxAllocator allocator) {
	cnx_assert(specifier.m_type == CNX_FORMAT_TYPE_DEFAULT
				   || specifier.m_type == CNX_FORMAT_TYPE_DEBUG,
			   "Can only format CnxArray with default or debug format specifier");

	let _self = *static_cast(const CnxArray(ARRAY_T, ARRAY_N)*)(self->m_self);
	let size = cnx_array_size(_self);
	let capacity = cnx_array_capacity(_self);
	let data = cnx_array_data(_self);
	return cnx_format_with_allocator(
		AS_STRING(CnxArray(ARRAY_T, ARRAY_N)) ": [size: {d}, capacity: {d}, data pointer: {x}]",
		allocator,
		size,
		capacity,
		as_format_t(nullptr_t, data));
}

	#undef CNX_TEMPLATE_SUPPRESS_INSTANTIATIONS
#endif // defined(ARRAY_T) && defined(ARRAY_N) && ARRAY_IMPL
