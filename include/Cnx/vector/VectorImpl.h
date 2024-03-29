/// @file VectorImpl.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief This module provides the function definitions for a template instantiation of
/// `CnxVector(VECTOR_T)`
/// @version 0.2.5
/// @date 2022-12-08
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

#if defined(VECTOR_T) && defined(VECTOR_SMALL_OPT_CAPACITY) && VECTOR_IMPL

	#define CNX_TEMPLATE_SUPPRESS_INSTANTIATIONS TRUE

	#include <Cnx/Allocators.h>
	#include <Cnx/BasicTypes.h>
	#include <Cnx/CollectionData.h>
	#include <Cnx/Iterator.h>
	#include <Cnx/Platform.h>
	#include <Cnx/Format.h>
	#include <Cnx/vector/VectorDef.h>

VECTOR_STATIC VECTOR_INLINE CnxVectorIterator(VECTOR_T)
	CnxVectorIdentifier(VECTOR_T, iterator_new)(const CnxVector(VECTOR_T) * restrict self);
VECTOR_STATIC VECTOR_INLINE CnxVectorConstIterator(VECTOR_T)
	CnxVectorIdentifier(VECTOR_T, const_iterator_new)(const CnxVector(VECTOR_T) * restrict self);

VECTOR_STATIC VECTOR_INLINE Ref(VECTOR_T)
	CnxVectorIdentifier(VECTOR_T,
						iterator_next)(CnxRandomAccessIterator(Ref(VECTOR_T)) * restrict self);
VECTOR_STATIC VECTOR_INLINE Ref(VECTOR_T)
	CnxVectorIdentifier(VECTOR_T,
						iterator_previous)(CnxRandomAccessIterator(Ref(VECTOR_T)) * restrict self);
VECTOR_STATIC VECTOR_INLINE Ref(VECTOR_T)
	CnxVectorIdentifier(VECTOR_T,
						iterator_at)(const CnxRandomAccessIterator(Ref(VECTOR_T)) * restrict self,
									 usize index);
VECTOR_STATIC VECTOR_INLINE Ref(VECTOR_T)
	CnxVectorIdentifier(VECTOR_T,
						iterator_rat)(const CnxRandomAccessIterator(Ref(VECTOR_T)) * restrict self,
									  usize index);
VECTOR_STATIC VECTOR_INLINE Ref(VECTOR_T) CnxVectorIdentifier(VECTOR_T, iterator_current)(
	const CnxRandomAccessIterator(Ref(VECTOR_T)) * restrict self);
VECTOR_STATIC VECTOR_INLINE bool CnxVectorIdentifier(VECTOR_T, iterator_equals)(
	const CnxRandomAccessIterator(Ref(VECTOR_T)) * restrict self,
	const CnxRandomAccessIterator(Ref(VECTOR_T)) * restrict rhs);

VECTOR_STATIC VECTOR_INLINE ConstRef(VECTOR_T) CnxVectorIdentifier(VECTOR_T, iterator_cnext)(
	CnxRandomAccessIterator(ConstRef(VECTOR_T)) * restrict self);
VECTOR_STATIC VECTOR_INLINE ConstRef(VECTOR_T) CnxVectorIdentifier(VECTOR_T, iterator_cprevious)(
	CnxRandomAccessIterator(ConstRef(VECTOR_T)) * restrict self);
VECTOR_STATIC VECTOR_INLINE ConstRef(VECTOR_T) CnxVectorIdentifier(VECTOR_T, iterator_cat)(
	const CnxRandomAccessIterator(ConstRef(VECTOR_T)) * restrict self,
	usize index);
VECTOR_STATIC VECTOR_INLINE ConstRef(VECTOR_T) CnxVectorIdentifier(VECTOR_T, iterator_crat)(
	const CnxRandomAccessIterator(ConstRef(VECTOR_T)) * restrict self,
	usize index);
VECTOR_STATIC VECTOR_INLINE ConstRef(VECTOR_T) CnxVectorIdentifier(VECTOR_T, iterator_ccurrent)(
	const CnxRandomAccessIterator(ConstRef(VECTOR_T)) * restrict self);
VECTOR_STATIC VECTOR_INLINE bool CnxVectorIdentifier(VECTOR_T, iterator_cequals)(
	const CnxRandomAccessIterator(ConstRef(VECTOR_T)) * restrict self,
	const CnxRandomAccessIterator(ConstRef(VECTOR_T)) * restrict rhs);

ImplIntoCnxRandomAccessIterator(CnxVector(VECTOR_T),
								Ref(VECTOR_T),
								CnxVectorIdentifier(VECTOR_T, into_iter),
								into,
								CnxVectorIdentifier(VECTOR_T, iterator_new),
								CnxVectorIdentifier(VECTOR_T, iterator_next),
								CnxVectorIdentifier(VECTOR_T, iterator_previous),
								CnxVectorIdentifier(VECTOR_T, iterator_at),
								CnxVectorIdentifier(VECTOR_T, iterator_current),
								CnxVectorIdentifier(VECTOR_T, iterator_equals));
ImplIntoCnxRandomAccessIterator(CnxVector(VECTOR_T),
								Ref(VECTOR_T),
								CnxVectorIdentifier(VECTOR_T, into_reverse_iter),
								into_reverse,
								CnxVectorIdentifier(VECTOR_T, iterator_new),
								CnxVectorIdentifier(VECTOR_T, iterator_next),
								CnxVectorIdentifier(VECTOR_T, iterator_previous),
								CnxVectorIdentifier(VECTOR_T, iterator_rat),
								CnxVectorIdentifier(VECTOR_T, iterator_current),
								CnxVectorIdentifier(VECTOR_T, iterator_equals));

ImplIntoCnxRandomAccessIterator(CnxVector(VECTOR_T),
								ConstRef(VECTOR_T),
								CnxVectorIdentifier(VECTOR_T, into_const_iter),
								into,
								CnxVectorIdentifier(VECTOR_T, const_iterator_new),
								CnxVectorIdentifier(VECTOR_T, iterator_cnext),
								CnxVectorIdentifier(VECTOR_T, iterator_cprevious),
								CnxVectorIdentifier(VECTOR_T, iterator_cat),
								CnxVectorIdentifier(VECTOR_T, iterator_ccurrent),
								CnxVectorIdentifier(VECTOR_T, iterator_cequals));
ImplIntoCnxRandomAccessIterator(CnxVector(VECTOR_T),
								ConstRef(VECTOR_T),
								CnxVectorIdentifier(VECTOR_T, into_reverse_const_iter),
								into_reverse,
								CnxVectorIdentifier(VECTOR_T, const_iterator_new),
								CnxVectorIdentifier(VECTOR_T, iterator_cnext),
								CnxVectorIdentifier(VECTOR_T, iterator_cprevious),
								CnxVectorIdentifier(VECTOR_T, iterator_crat),
								CnxVectorIdentifier(VECTOR_T, iterator_ccurrent),
								CnxVectorIdentifier(VECTOR_T, iterator_cequals));

__attr(always_inline) static inline VECTOR_T
	CnxVectorIdentifier(VECTOR_T,
						default_constructor)(__attr(maybe_unused) CnxAllocator allocator) {
	return (VECTOR_T){0};
}

__attr(always_inline) __attr(not_null(1)) static inline VECTOR_T
	CnxVectorIdentifier(VECTOR_T, default_copy_constructor)(const VECTOR_T* restrict elem,
															__attr(maybe_unused)
																CnxAllocator allocator) {
	return *elem;
}

__attr(always_inline)
	__attr(not_null(1)) static inline void CnxVectorIdentifier(VECTOR_T, default_destructor)(
		__attr(maybe_unused)
			VECTOR_T* restrict element, /** NOLINT(readability-non-const-parameter)**/
		__attr(maybe_unused) CnxAllocator allocator) {
}

static const struct CnxVectorIdentifier(VECTOR_T, vtable) CnxVectorIdentifier(VECTOR_T, vtable_impl)
	= {
		.clone = CnxVectorIdentifier(VECTOR_T, clone),
		.at_const = CnxVectorIdentifier(VECTOR_T, at_const),
		.at_mut = CnxVectorIdentifier(VECTOR_T, at_mut),
		.front_const = CnxVectorIdentifier(VECTOR_T, front_const),
		.front_mut = CnxVectorIdentifier(VECTOR_T, front_mut),
		.back_const = CnxVectorIdentifier(VECTOR_T, back_const),
		.back_mut = CnxVectorIdentifier(VECTOR_T, back_mut),
		.data_const = CnxVectorIdentifier(VECTOR_T, data_const),
		.data_mut = CnxVectorIdentifier(VECTOR_T, data_mut),
		.is_empty = CnxVectorIdentifier(VECTOR_T, is_empty),
		.is_full = CnxVectorIdentifier(VECTOR_T, is_full),
		.size = CnxVectorIdentifier(VECTOR_T, size),
		.capacity = CnxVectorIdentifier(VECTOR_T, capacity),
		.reserve = CnxVectorIdentifier(VECTOR_T, reserve),
		.resize = CnxVectorIdentifier(VECTOR_T, resize),
		.shrink_to_fit = CnxVectorIdentifier(VECTOR_T, shrink_to_fit),
		.clear = CnxVectorIdentifier(VECTOR_T, clear),
		.push_back = CnxVectorIdentifier(VECTOR_T, push_back),
		.pop_back = CnxVectorIdentifier(VECTOR_T, pop_back),
		.pop_front = CnxVectorIdentifier(VECTOR_T, pop_front),
		.insert = CnxVectorIdentifier(VECTOR_T, insert),
		.erase = CnxVectorIdentifier(VECTOR_T, erase),
		.erase_n = CnxVectorIdentifier(VECTOR_T, erase_n),
		.free = CnxVectorIdentifier(VECTOR_T, free),
		.into_iter = CnxVectorIdentifier(VECTOR_T, into_iter),
		.into_reverse_iter = CnxVectorIdentifier(VECTOR_T, into_reverse_iter),
		.into_const_iter = CnxVectorIdentifier(VECTOR_T, into_const_iter),
		.into_reverse_const_iter = CnxVectorIdentifier(VECTOR_T, into_reverse_const_iter),
		.begin = CnxVectorIdentifier(VECTOR_T, begin),
		.end = CnxVectorIdentifier(VECTOR_T, end),
		.rbegin = CnxVectorIdentifier(VECTOR_T, rbegin),
		.rend = CnxVectorIdentifier(VECTOR_T, rend),
		.cbegin = CnxVectorIdentifier(VECTOR_T, cbegin),
		.cend = CnxVectorIdentifier(VECTOR_T, cend),
		.crbegin = CnxVectorIdentifier(VECTOR_T, crbegin),
		.crend = CnxVectorIdentifier(VECTOR_T, crend),
};

static const struct CnxCollectionData(CnxVector(VECTOR_T))
	CnxVectorIdentifier(VECTOR_T, default_collection_data)
	= {.m_constructor = CnxVectorIdentifier(VECTOR_T, default_constructor),
	   .m_copy_constructor = CnxVectorIdentifier(VECTOR_T, default_copy_constructor),
	   .m_destructor = CnxVectorIdentifier(VECTOR_T, default_destructor)};

__attr(always_inline) static inline bool CnxVectorIdentifier(VECTOR_T, is_short)(
	const CnxVector(VECTOR_T) * restrict self) {
	return self->m_capacity <= VECTOR_SMALL_OPT_CAPACITY;
}

VECTOR_STATIC VECTOR_INLINE CnxVector(VECTOR_T) CnxVectorIdentifier(VECTOR_T, new)(void) {
	return cnx_vector_new_with_allocator_and_collection_data(
		VECTOR_T,
		DEFAULT_ALLOCATOR,
		&CnxVectorIdentifier(VECTOR_T, default_collection_data));
}

VECTOR_STATIC VECTOR_INLINE CnxVector(VECTOR_T)
	CnxVectorIdentifier(VECTOR_T, new_with_allocator)(CnxAllocator allocator) {
	return cnx_vector_new_with_allocator_and_collection_data(
		VECTOR_T,
		allocator,
		&CnxVectorIdentifier(VECTOR_T, default_collection_data));
}

VECTOR_STATIC VECTOR_INLINE CnxVector(VECTOR_T)
	CnxVectorIdentifier(VECTOR_T, new_with_collection_data)(
		const CnxCollectionData(CnxVector(VECTOR_T)) * restrict data) {

	return cnx_vector_new_with_allocator_and_collection_data(VECTOR_T, DEFAULT_ALLOCATOR, data);
}

VECTOR_STATIC VECTOR_INLINE CnxVector(VECTOR_T)
	CnxVectorIdentifier(VECTOR_T, new_with_allocator_and_collection_data)(
		CnxAllocator allocator,
		const CnxCollectionData(CnxVector(VECTOR_T)) * restrict data) {
	let_mut vec = (CnxVector(VECTOR_T)){.m_size = 0,
										.m_capacity = VECTOR_SMALL_OPT_CAPACITY,
										.m_allocator = allocator,
										.m_data = data,
										.m_vtable = &CnxVectorIdentifier(VECTOR_T, vtable_impl)};
	cnx_assert(vec.m_data->m_constructor != nullptr, "Element default constructor cannot be null");
	cnx_assert(vec.m_data->m_destructor != nullptr, "Element destructor cannot be null");

	#if VECTOR_SMALL_OPT_CAPACITY == 0

	vec.m_long = cnx_allocator_allocate_array_t(VECTOR_T,
												vec.m_data.m_allocator,
												VECTOR_DEFAULT_LONG_CAPACITY);
	vec.m_capacity = VECTOR_DEFAULT_LONG_CAPACITY;

	#else

	cnx_memset(VECTOR_T, vec.m_short, 0, VECTOR_SMALL_OPT_CAPACITY);

	#endif

	return vec;
}

VECTOR_STATIC VECTOR_INLINE CnxVector(VECTOR_T)
	CnxVectorIdentifier(VECTOR_T, new_with_capacity)(usize capacity) {
	return cnx_vector_new_with_capacity_and_allocator(VECTOR_T, capacity, DEFAULT_ALLOCATOR);
}

VECTOR_STATIC VECTOR_INLINE CnxVector(VECTOR_T)
	CnxVectorIdentifier(VECTOR_T, new_with_capacity_and_allocator)(usize capacity,
																   CnxAllocator allocator) {
	let_mut vec = cnx_vector_new_with_allocator(VECTOR_T, allocator);
	cnx_vector_reserve(vec, capacity);
	return vec;
}

VECTOR_STATIC VECTOR_INLINE CnxVector(VECTOR_T)
	CnxVectorIdentifier(VECTOR_T, new_with_capacity_and_collection_data)(
		usize capacity,
		const CnxCollectionData(CnxVector(VECTOR_T)) * restrict data) {

	let_mut vec = cnx_vector_new_with_collection_data(VECTOR_T, data);
	cnx_vector_reserve(vec, capacity);
	return vec;
}

VECTOR_STATIC VECTOR_INLINE CnxVector(VECTOR_T)
	CnxVectorIdentifier(VECTOR_T, new_with_capacity_allocator_and_collection_data)(
		usize capacity,
		CnxAllocator allocator,
		const CnxCollectionData(CnxVector(VECTOR_T)) * restrict data) {

	let_mut vec = cnx_vector_new_with_allocator_and_collection_data(VECTOR_T, allocator, data);
	cnx_vector_reserve(vec, capacity);
	return vec;
}

VECTOR_STATIC VECTOR_INLINE CnxVector(VECTOR_T)
	CnxVectorIdentifier(VECTOR_T, clone)(const CnxVector(VECTOR_T) * restrict self)
		cnx_disable_if(!(self->m_data->m_copy_constructor),
					   "Can't clone a CnxVector(VECTOR_T) with elements that aren't copyable (no "
					   "element copy constructor defined)") {
	cnx_assert(self->m_data->m_copy_constructor != nullptr,
			   "Can't clone CnxVector(VECTOR_T) with elements that aren't copyable (no element "
			   "copy constructor defined)");

	let_mut vec
		= cnx_vector_new_with_capacity_allocator_and_collection_data(VECTOR_T,
																	 cnx_vector_capacity(*self),
																	 self->m_allocator,
																	 self->m_data);
	foreach_ref(elem, *self) {
		cnx_vector_push_back(vec, self->m_data->m_copy_constructor(elem, self->m_allocator));
	}
	return vec;
}

VECTOR_STATIC VECTOR_INLINE const VECTOR_T*
CnxVectorIdentifier(VECTOR_T, at_const)(const CnxVector(VECTOR_T) * restrict self, usize index) {
	cnx_assert(index <= self->m_size,
			   "cnx_vector_at called with index > size (index out of bounds");
	cnx_assert(index < self->m_capacity,
			   "cnx_vector_at called with index >= capacity (index out of bounds");

	return CnxVectorIdentifier(VECTOR_T, is_short)(self) ? &(self->m_short[index]) :
														   &(self->m_long[index]);
}

VECTOR_STATIC VECTOR_INLINE VECTOR_T*
CnxVectorIdentifier(VECTOR_T, at_mut)(CnxVector(VECTOR_T) * restrict self, usize index) {
	cnx_assert(index <= self->m_size,
			   "cnx_vector_at called with index > size (index out of bounds");
	cnx_assert(index < self->m_capacity,
			   "cnx_vector_at called with index >= capacity (index out of bounds");

	return CnxVectorIdentifier(VECTOR_T, is_short)(self) ? &(self->m_short[index]) :
														   &(self->m_long[index]);
}

VECTOR_STATIC VECTOR_INLINE const VECTOR_T*
CnxVectorIdentifier(VECTOR_T, front_const)(const CnxVector(VECTOR_T) * restrict self) {
	return &cnx_vector_at(*self, 0);
}

VECTOR_STATIC VECTOR_INLINE VECTOR_T*
CnxVectorIdentifier(VECTOR_T, front_mut)(CnxVector(VECTOR_T) * restrict self) {
	return &cnx_vector_at_mut(*self, 0);
}

VECTOR_STATIC VECTOR_INLINE const VECTOR_T*
CnxVectorIdentifier(VECTOR_T, back_const)(const CnxVector(VECTOR_T) * restrict self) {
	let size = cnx_vector_size(*self);
	let back_index = size > 0 ? size - 1 : 0;
	return &cnx_vector_at(*self, back_index);
}

VECTOR_STATIC VECTOR_INLINE VECTOR_T*
CnxVectorIdentifier(VECTOR_T, back_mut)(CnxVector(VECTOR_T) * restrict self) {
	let size = cnx_vector_size(*self);
	let back_index = size > 0 ? size - 1 : 0;
	return &cnx_vector_at_mut(*self, back_index);
}

VECTOR_STATIC VECTOR_INLINE const VECTOR_T*
CnxVectorIdentifier(VECTOR_T, data_const)(const CnxVector(VECTOR_T) * restrict self) {
	return &cnx_vector_front(*self);
}

VECTOR_STATIC VECTOR_INLINE VECTOR_T*
CnxVectorIdentifier(VECTOR_T, data_mut)(CnxVector(VECTOR_T) * restrict self) {
	return &cnx_vector_front_mut(*self);
}

VECTOR_STATIC VECTOR_INLINE bool
CnxVectorIdentifier(VECTOR_T, is_empty)(const CnxVector(VECTOR_T) * restrict self) {
	return cnx_vector_size(*self) == 0;
}

VECTOR_STATIC VECTOR_INLINE bool
CnxVectorIdentifier(VECTOR_T, is_full)(const CnxVector(VECTOR_T) * restrict self) {
	return self->m_size == self->m_capacity;
}

VECTOR_STATIC VECTOR_INLINE usize
CnxVectorIdentifier(VECTOR_T, size)(const CnxVector(VECTOR_T) * restrict self) {
	return self->m_size;
}

VECTOR_STATIC VECTOR_INLINE usize CnxVectorIdentifier(VECTOR_T, max_size)(void) {
	return (cnx_max_value(usize) - 1) << 1U;
}

VECTOR_STATIC VECTOR_INLINE usize
CnxVectorIdentifier(VECTOR_T, capacity)(const CnxVector(VECTOR_T) * restrict self) {
	return self->m_capacity;
}

VECTOR_STATIC VECTOR_INLINE void
CnxVectorIdentifier(VECTOR_T, resize_internal)(CnxVector(VECTOR_T) * restrict self,
											   usize new_size) {
	let size = cnx_vector_size(*self);
	if(self->m_data->m_destructor != CnxVectorIdentifier(VECTOR_T, default_destructor)
	   && new_size < size)
	{
		let num_to_destroy = size - new_size;
		for(let_mut i = new_size; i < new_size + num_to_destroy; ++i) {
			self->m_data->m_destructor(&cnx_vector_at_mut(*self, i), self->m_allocator);
		}
	}

	if(new_size > VECTOR_SMALL_OPT_CAPACITY) {
		if(!CnxVectorIdentifier(VECTOR_T, is_short)(self)) {
			let_mut array = cnx_allocator_reallocate_array_t(VECTOR_T,
															 self->m_allocator,
															 &cnx_vector_at_mut(*self, 0),
															 size,
															 new_size);
			let resized_size = size < new_size ? size : new_size;
			self->m_capacity = new_size;
			self->m_size = resized_size;
			self->m_long = array;
		}
		else {
			let_mut array = cnx_allocator_allocate_array_t(VECTOR_T, self->m_allocator, new_size);
			let num_to_copy = size < new_size ? size : new_size;
			cnx_memcpy(VECTOR_T, array, self->m_short, num_to_copy);
			self->m_capacity = new_size;
			self->m_size = num_to_copy;
			self->m_long = array;
		}
	}
	else if(self->m_capacity != VECTOR_SMALL_OPT_CAPACITY && VECTOR_SMALL_OPT_CAPACITY != 0) {
		let capacity = VECTOR_SMALL_OPT_CAPACITY;
		// we may end up going back to this method of shuffling things around,
		// but memmove seems to be __just__ faster here
		// let_mut array = cnx_allocator_allocate_array_t(VECTOR_T, self->m_allocator, capacity);
		// cnx_memcpy(VECTOR_T, array, self->m_long, capacity);
		// cnx_allocator_deallocate(self->m_allocator, self->m_long);
		// cnx_memcpy(VECTOR_T, self->m_short, array, capacity);
		// cnx_allocator_deallocate(self->m_allocator, array);
		let old = self->m_long;
		cnx_memmove(VECTOR_T, self->m_short, self->m_long, capacity);
		cnx_allocator_deallocate(self->m_allocator, old);
		self->m_size = capacity;
		self->m_capacity = capacity;
	}
}

__attr(always_inline) static inline usize
	CnxVectorIdentifier(VECTOR_T, get_expanded_capacity)(usize old_capacity, usize num_increments) {
	return num_increments * ((old_capacity * 3) / 2);
}

VECTOR_STATIC VECTOR_INLINE void
CnxVectorIdentifier(VECTOR_T, reserve)(CnxVector(VECTOR_T) * restrict self, usize new_capacity) {
	if(new_capacity > self->m_capacity) {
		let num_increments = 1 + (new_capacity / ((self->m_capacity * 3) / 2));
		let actual_new_capacity
			= CnxVectorIdentifier(VECTOR_T, get_expanded_capacity)(self->m_capacity,
																   num_increments);
		CnxVectorIdentifier(VECTOR_T, resize_internal)(self, actual_new_capacity);
	}
}

VECTOR_STATIC VECTOR_INLINE void
CnxVectorIdentifier(VECTOR_T, resize)(CnxVector(VECTOR_T) * restrict self, usize new_size) {
	CnxVectorIdentifier(VECTOR_T, resize_internal)(self, new_size);
	if(new_size > self->m_size) {
		for(let_mut i = self->m_size - 1; i < new_size; ++i) {
			cnx_vector_at_mut(*self, i) = self->m_data->m_constructor(self->m_allocator);
		}
	}
	self->m_size = new_size;
}

VECTOR_STATIC VECTOR_INLINE void
CnxVectorIdentifier(VECTOR_T, shrink_to_fit)(CnxVector(VECTOR_T) * restrict self) {
	CnxVectorIdentifier(VECTOR_T, resize_internal)(self, cnx_vector_size(*self));
}

VECTOR_STATIC VECTOR_INLINE void
CnxVectorIdentifier(VECTOR_T, clear)(CnxVector(VECTOR_T) * restrict self) {
	for(let_mut i = 0U; i < cnx_vector_size(*self); ++i) {
		self->m_data->m_destructor(&cnx_vector_at_mut(*self, i), self->m_allocator);
	}
	self->m_size = 0U;
}

VECTOR_STATIC VECTOR_INLINE void
CnxVectorIdentifier(VECTOR_T,
					push_back)(CnxVector(VECTOR_T) * restrict self,
							   VECTOR_T element /** NOLINT(readability-non-const-parameter) **/) {
	if(self->m_size == self->m_capacity) {
		let new_capacity
			= CnxVectorIdentifier(VECTOR_T, get_expanded_capacity)(self->m_capacity, 1);
		CnxVectorIdentifier(VECTOR_T, resize_internal)(self, new_capacity);
	}

	cnx_vector_at_mut(*self, self->m_size) = element;
	self->m_size++;
}

VECTOR_STATIC VECTOR_INLINE CnxOption(VECTOR_T)
	CnxVectorIdentifier(VECTOR_T, pop_back)(CnxVector(VECTOR_T) * restrict self) {
	if(self->m_size == 0) {
		return None(VECTOR_T);
	}

	let_mut ptr = &cnx_vector_at_mut(*self, self->m_size - 1);
	let elem = Some(VECTOR_T, *ptr);
	*ptr = (VECTOR_T){0};
	self->m_size--;

	return elem;
}

VECTOR_STATIC VECTOR_INLINE CnxOption(VECTOR_T)
	CnxVectorIdentifier(VECTOR_T, pop_front)(CnxVector(VECTOR_T) * restrict self) {
	if(self->m_size == 0) {
		return None(VECTOR_T);
	}

	let elem = Some(VECTOR_T, cnx_vector_front_mut(*self));
	cnx_vector_erase(*self, 0);

	return elem;
}

VECTOR_STATIC VECTOR_INLINE void
CnxVectorIdentifier(VECTOR_T,
					insert)(CnxVector(VECTOR_T) * restrict self,
							VECTOR_T element /** NOLINT(readability-non-const-parameter **/,
							usize index) {
	cnx_assert(index <= self->m_size,
			   "cnx_vector_insert called with index > size (index out of bounds)");

	if(self->m_size + 1 > self->m_capacity) {
		let new_capacity
			= CnxVectorIdentifier(VECTOR_T, get_expanded_capacity)(self->m_capacity, 1);
		CnxVectorIdentifier(VECTOR_T, resize_internal)(self, new_capacity);
	}

	if(index != self->m_size) {
		let num_to_move = self->m_size - index;
		cnx_memmove(VECTOR_T,
					&cnx_vector_at_mut(*self, index + 1),
					&cnx_vector_at_mut(*self, index),
					num_to_move);
	}
	cnx_vector_at_mut(*self, index) = element;
	self->m_size++;
}

VECTOR_STATIC VECTOR_INLINE void
CnxVectorIdentifier(VECTOR_T, erase)(CnxVector(VECTOR_T) * restrict self, usize index) {
	cnx_assert(index < self->m_size,
			   "cnx_vector_erase called with index >= self->m_size (index out of bounds)");

	self->m_data->m_destructor(&cnx_vector_at_mut(*self, index), self->m_allocator);

	if(index != self->m_size - 1) {
		let num_to_move = self->m_size - (index + 1);
		cnx_memmove(VECTOR_T,
					&cnx_vector_at_mut(*self, index),
					&cnx_vector_at_mut(*self, index + 1),
					num_to_move);
	}
	self->m_size--;
}

VECTOR_STATIC VECTOR_INLINE void
CnxVectorIdentifier(VECTOR_T,
					erase_n)(CnxVector(VECTOR_T) * restrict self, usize index, usize num_elements) {
	cnx_assert(index < self->m_size,
			   "cnx_vector_erase_n called with index >= size (index out of bounds)");
	cnx_assert(index + num_elements < self->m_size,
			   "cnx_vector_erase_n called with index + size >= size (range "
			   "out of bounds)");

	let end = index + num_elements;
	let num_to_move = self->m_size - end;

	for(let_mut i = index; i < end; ++i) {
		self->m_data->m_destructor(&cnx_vector_at_mut(*self, i), self->m_allocator);
	}

	if(end != self->m_size) {
		cnx_memmove(VECTOR_T,
					&cnx_vector_at_mut(*self, index),
					&cnx_vector_at_mut(*self, end),
					num_to_move);
	}
	self->m_size -= num_elements;
}

VECTOR_STATIC VECTOR_INLINE void CnxVectorIdentifier(VECTOR_T, free)(void* restrict self) {
	let self_ = static_cast(CnxVector(VECTOR_T)*)(self);
	for(let_mut i = 0U; i < self_->m_size; ++i) {
		self_->m_data->m_destructor(&cnx_vector_at_mut(*self_, i), self_->m_allocator);
	}

	if(!CnxVectorIdentifier(VECTOR_T, is_short)(self_)) {
		cnx_allocator_deallocate(self_->m_allocator, self_->m_long);
		self_->m_capacity = VECTOR_SMALL_OPT_CAPACITY;
	}
	self_->m_size = 0U;
}

VECTOR_STATIC VECTOR_INLINE CnxVectorIterator(VECTOR_T)
	CnxVectorIdentifier(VECTOR_T, iterator_new)(const CnxVector(VECTOR_T) * restrict self) {
	return (CnxVectorIterator(VECTOR_T)){.m_index = 0U,
										 .m_vector = const_cast(CnxVector(VECTOR_T)*)(self)};
}

VECTOR_STATIC VECTOR_INLINE CnxVectorConstIterator(VECTOR_T)
	CnxVectorIdentifier(VECTOR_T, const_iterator_new)(const CnxVector(VECTOR_T) * restrict self) {
	return (CnxVectorConstIterator(VECTOR_T)){.m_index = 0U,
											  .m_vector = const_cast(CnxVector(VECTOR_T)*)(self)};
}

VECTOR_STATIC VECTOR_INLINE Ref(VECTOR_T)
	CnxVectorIdentifier(VECTOR_T,
						iterator_next)(CnxRandomAccessIterator(Ref(VECTOR_T)) * restrict self) {
	let _self = static_cast(CnxVectorIterator(VECTOR_T)*)(self->m_self);

	cnx_assert(_self->m_index > -1,
			   "Iterator value accessed when iterator is positioned before the "
			   "beginning of the "
			   "iteration (iterator out of bounds)");
	cnx_assert(static_cast(usize)(_self->m_index) < _self->m_vector->m_size,
			   "Iterator value accessed when iterator is positioned after the end of the "
			   "iteration (iterator out of bounds)");
	cnx_assert(static_cast(usize)(_self->m_index) < _self->m_vector->m_capacity,
			   "Iterator value accessed when iterator is positioned after the end of the "
			   "iteration (iterator out of bounds)");
	_self->m_index++;
	if(static_cast(usize)(_self->m_index) >= _self->m_vector->m_size) {
		return &cnx_vector_at_mut(*(_self->m_vector), _self->m_vector->m_size - 1);
	}

	return &cnx_vector_at_mut(*(_self->m_vector), static_cast(usize)(_self->m_index));
}

VECTOR_STATIC VECTOR_INLINE Ref(VECTOR_T)
	CnxVectorIdentifier(VECTOR_T,
						iterator_previous)(CnxRandomAccessIterator(Ref(VECTOR_T)) * restrict self) {
	let _self = static_cast(CnxVectorIterator(VECTOR_T)*)(self->m_self);

	cnx_assert(_self->m_index > -1,
			   "Iterator value accessed when iterator is positioned before the "
			   "beginning of the "
			   "iteration (iterator out of bounds)");
	cnx_assert(static_cast(usize)(_self->m_index) < _self->m_vector->m_size,
			   "Iterator value accessed when iterator is positioned after the end of the "
			   "iteration (iterator out of bounds)");
	_self->m_index--;
	if(_self->m_index < 0) {
		return &cnx_vector_at_mut(*(_self->m_vector), 0);
	}

	return &cnx_vector_at_mut(*(_self->m_vector), static_cast(usize)(_self->m_index));
}

VECTOR_STATIC VECTOR_INLINE Ref(VECTOR_T)
	CnxVectorIdentifier(VECTOR_T,
						iterator_at)(const CnxRandomAccessIterator(Ref(VECTOR_T)) * restrict self,
									 usize index) {
	let _self = static_cast(CnxVectorIterator(VECTOR_T)*)(self->m_self);

	cnx_assert(index < _self->m_vector->m_size,
			   "cnx_vector_iterator_at called with index pst the end of the iteration "
			   "(iterator index out of bounds)");
	return &cnx_vector_at_mut(*(_self->m_vector), index);
}

VECTOR_STATIC VECTOR_INLINE Ref(VECTOR_T)
	CnxVectorIdentifier(VECTOR_T,
						iterator_rat)(const CnxRandomAccessIterator(Ref(VECTOR_T)) * restrict self,
									  usize index) {
	let _self = static_cast(CnxVectorIterator(VECTOR_T)*)(self->m_self);

	cnx_assert(index < _self->m_vector->m_size,
			   "cnx_vector_iterator_at called with index pst the end of the iteration "
			   "(iterator index out of bounds)");
	return &cnx_vector_at_mut(*(_self->m_vector), (_self->m_vector->m_size - 1) - index);
}

VECTOR_STATIC VECTOR_INLINE Ref(VECTOR_T) CnxVectorIdentifier(VECTOR_T, iterator_current)(
	const CnxRandomAccessIterator(Ref(VECTOR_T)) * restrict self) {
	let _self = static_cast(const CnxVectorIterator(VECTOR_T)*)(self->m_self);

	cnx_assert(_self->m_index > -1,
			   "Iterator value accessed when iterator is positioned before the "
			   "beginning of the "
			   "iteration (iterator out of bounds)");
	cnx_assert(static_cast(usize)(_self->m_index) < _self->m_vector->m_size,
			   "Iterator value accessed when iterator is positioned after the end of the "
			   "iteration (iterator out of bounds)");
	cnx_assert(static_cast(usize)(_self->m_index) < _self->m_vector->m_capacity,
			   "Iterator value accessed when iterator is positioned after the end of the "
			   "iteration (iterator out of bounds)");
	return &cnx_vector_at_mut(*(_self->m_vector), static_cast(usize)(_self->m_index));
}

VECTOR_STATIC VECTOR_INLINE bool
CnxVectorIdentifier(VECTOR_T,
					iterator_equals)(const CnxRandomAccessIterator(Ref(VECTOR_T)) * restrict self,
									 const CnxRandomAccessIterator(Ref(VECTOR_T)) * restrict rhs) {
	let _self = static_cast(const CnxVectorIterator(VECTOR_T)*)(self->m_self);
	let _rhs = static_cast(const CnxVectorIterator(VECTOR_T)*)(rhs->m_self);

	return _self->m_index == _rhs->m_index && _self->m_vector == _rhs->m_vector;
}

VECTOR_STATIC VECTOR_INLINE ConstRef(VECTOR_T) CnxVectorIdentifier(VECTOR_T, iterator_cnext)(
	CnxRandomAccessIterator(ConstRef(VECTOR_T)) * restrict self) {
	let _self = static_cast(CnxVectorConstIterator(VECTOR_T)*)(self->m_self);

	cnx_assert(_self->m_index > -1,
			   "Iterator value accessed when iterator is positioned before the "
			   "beginning of the "
			   "iteration (iterator out of bounds)");
	cnx_assert(static_cast(usize)(_self->m_index) < _self->m_vector->m_size,
			   "Iterator value accessed when iterator is positioned after the end of the "
			   "iteration (iterator out of bounds)");
	cnx_assert(static_cast(usize)(_self->m_index) < _self->m_vector->m_capacity,
			   "Iterator value accessed when iterator is positioned after the end of the "
			   "iteration (iterator out of bounds)");
	_self->m_index++;
	if(static_cast(usize)(_self->m_index) >= _self->m_vector->m_size) {
		return &cnx_vector_at(*(_self->m_vector), _self->m_vector->m_size - 1);
	}

	return &cnx_vector_at(*(_self->m_vector), static_cast(usize)(_self->m_index));
}

VECTOR_STATIC VECTOR_INLINE ConstRef(VECTOR_T) CnxVectorIdentifier(VECTOR_T, iterator_cprevious)(
	CnxRandomAccessIterator(ConstRef(VECTOR_T)) * restrict self) {
	let _self = static_cast(CnxVectorConstIterator(VECTOR_T)*)(self->m_self);

	cnx_assert(_self->m_index > -1,
			   "Iterator value accessed when iterator is positioned before the "
			   "beginning of the "
			   "iteration (iterator out of bounds)");
	cnx_assert(static_cast(usize)(_self->m_index) < _self->m_vector->m_size,
			   "Iterator value accessed when iterator is positioned after the end of the "
			   "iteration (iterator out of bounds)");
	_self->m_index--;
	if(_self->m_index < 0) {
		return &cnx_vector_at(*(_self->m_vector), 0);
	}

	return &cnx_vector_at(*(_self->m_vector), static_cast(usize)(_self->m_index));
}

VECTOR_STATIC VECTOR_INLINE ConstRef(VECTOR_T) CnxVectorIdentifier(VECTOR_T, iterator_cat)(
	const CnxRandomAccessIterator(ConstRef(VECTOR_T)) * restrict self,
	usize index) {
	let _self = static_cast(CnxVectorConstIterator(VECTOR_T)*)(self->m_self);

	cnx_assert(index < _self->m_vector->m_size,
			   "cnx_vector_iterator_at called with index pst the end of the iteration "
			   "(iterator index out of bounds)");
	return &cnx_vector_at(*(_self->m_vector), index);
}

VECTOR_STATIC VECTOR_INLINE ConstRef(VECTOR_T) CnxVectorIdentifier(VECTOR_T, iterator_crat)(
	const CnxRandomAccessIterator(ConstRef(VECTOR_T)) * restrict self,
	usize index) {
	let _self = static_cast(CnxVectorConstIterator(VECTOR_T)*)(self->m_self);

	cnx_assert(index < _self->m_vector->m_size,
			   "cnx_vector_iterator_at called with index pst the end of the iteration "
			   "(iterator index out of bounds)");
	return &cnx_vector_at(*(_self->m_vector), (_self->m_vector->m_size - 1) - index);
}

VECTOR_STATIC VECTOR_INLINE ConstRef(VECTOR_T) CnxVectorIdentifier(VECTOR_T, iterator_ccurrent)(
	const CnxRandomAccessIterator(ConstRef(VECTOR_T)) * restrict self) {
	let _self = static_cast(const CnxVectorConstIterator(VECTOR_T)*)(self->m_self);

	cnx_assert(_self->m_index > -1,
			   "Iterator value accessed when iterator is positioned before the "
			   "beginning of the "
			   "iteration (iterator out of bounds)");
	cnx_assert(static_cast(usize)(_self->m_index) < _self->m_vector->m_size,
			   "Iterator value accessed when iterator is positioned after the end of the "
			   "iteration (iterator out of bounds)");
	cnx_assert(static_cast(usize)(_self->m_index) < _self->m_vector->m_capacity,
			   "Iterator value accessed when iterator is positioned after the end of the "
			   "iteration (iterator out of bounds)");
	return &cnx_vector_at(*(_self->m_vector), static_cast(usize)(_self->m_index));
}

VECTOR_STATIC VECTOR_INLINE bool CnxVectorIdentifier(VECTOR_T, iterator_cequals)(
	const CnxRandomAccessIterator(ConstRef(VECTOR_T)) * restrict self,
	const CnxRandomAccessIterator(ConstRef(VECTOR_T)) * restrict rhs) {
	let _self = static_cast(const CnxVectorConstIterator(VECTOR_T)*)(self->m_self);
	let _rhs = static_cast(const CnxVectorConstIterator(VECTOR_T)*)(rhs->m_self);

	return _self->m_index == _rhs->m_index && _self->m_vector == _rhs->m_vector;
}

VECTOR_STATIC VECTOR_INLINE CnxRandomAccessIterator(Ref(VECTOR_T))
	CnxVectorIdentifier(VECTOR_T, begin)(CnxVector(VECTOR_T) * restrict self) {
	let_mut iter = cnx_vector_into_iter(*self);
	let_mut inner = static_cast(CnxVectorIterator(VECTOR_T)*)(iter.m_self);
	inner->m_index = 0;
	return iter;
}

VECTOR_STATIC VECTOR_INLINE CnxRandomAccessIterator(Ref(VECTOR_T))
	CnxVectorIdentifier(VECTOR_T, end)(CnxVector(VECTOR_T) * restrict self) {
	let_mut iter = cnx_vector_into_iter(*self);
	let_mut inner = static_cast(CnxVectorIterator(VECTOR_T)*)(iter.m_self);
	inner->m_index = static_cast(isize)(self->m_size);
	return iter;
}

VECTOR_STATIC VECTOR_INLINE CnxRandomAccessIterator(Ref(VECTOR_T))
	CnxVectorIdentifier(VECTOR_T, rbegin)(CnxVector(VECTOR_T) * restrict self) {
	let_mut iter = cnx_vector_into_reverse_iter(*self);
	let_mut inner = static_cast(CnxVectorIterator(VECTOR_T)*)(iter.m_self);
	inner->m_index = static_cast(isize)(self->m_size - 1);
	return iter;
}

VECTOR_STATIC VECTOR_INLINE CnxRandomAccessIterator(Ref(VECTOR_T))
	CnxVectorIdentifier(VECTOR_T, rend)(CnxVector(VECTOR_T) * restrict self) {
	let_mut iter = cnx_vector_into_reverse_iter(*self);
	let_mut inner = static_cast(CnxVectorIterator(VECTOR_T)*)(iter.m_self);
	inner->m_index = -1;
	return iter;
}

VECTOR_STATIC VECTOR_INLINE CnxRandomAccessIterator(ConstRef(VECTOR_T))
	CnxVectorIdentifier(VECTOR_T, cbegin)(const CnxVector(VECTOR_T) * restrict self) {
	let_mut iter = cnx_vector_into_const_iter(*self);
	let_mut inner = static_cast(CnxVectorConstIterator(VECTOR_T)*)(iter.m_self);
	inner->m_index = 0;
	return iter;
}

VECTOR_STATIC VECTOR_INLINE CnxRandomAccessIterator(ConstRef(VECTOR_T))
	CnxVectorIdentifier(VECTOR_T, cend)(const CnxVector(VECTOR_T) * restrict self) {
	let_mut iter = cnx_vector_into_const_iter(*self);
	let_mut inner = static_cast(CnxVectorConstIterator(VECTOR_T)*)(iter.m_self);
	inner->m_index = static_cast(isize)(self->m_size);
	return iter;
}

VECTOR_STATIC VECTOR_INLINE CnxRandomAccessIterator(ConstRef(VECTOR_T))
	CnxVectorIdentifier(VECTOR_T, crbegin)(const CnxVector(VECTOR_T) * restrict self) {
	let_mut iter = cnx_vector_into_reverse_const_iter(*self);
	let_mut inner = static_cast(CnxVectorConstIterator(VECTOR_T)*)(iter.m_self);
	inner->m_index = static_cast(isize)(self->m_size - 1);
	return iter;
}

VECTOR_STATIC VECTOR_INLINE CnxRandomAccessIterator(ConstRef(VECTOR_T))
	CnxVectorIdentifier(VECTOR_T, crend)(const CnxVector(VECTOR_T) * restrict self) {
	let_mut iter = cnx_vector_into_reverse_const_iter(*self);
	let_mut inner = static_cast(CnxVectorConstIterator(VECTOR_T)*)(iter.m_self);
	inner->m_index = -1;
	return iter;
}

typedef struct CnxVectorIdentifier(VECTOR_T, FormatContext) {
	bool is_debug;
}
CnxVectorIdentifier(VECTOR_T, FormatContext);

VECTOR_STATIC VECTOR_INLINE CnxFormatContext CnxVectorIdentifier(VECTOR_T, is_specifier_valid)(
	__attr(maybe_unused) const CnxFormat* restrict self,
	CnxStringView specifier) {

	let_mut context = (CnxFormatContext){.is_valid = CNX_FORMAT_SUCCESS};
	let length = cnx_stringview_length(specifier);
	let_mut state = (CnxVectorIdentifier(VECTOR_T, FormatContext)){.is_debug = false};

	if(length > 1) {
		context.is_valid = CNX_FORMAT_BAD_SPECIFIER_INVALID_CHAR_IN_SPECIFIER;
		return context;
	}

	if(length == 1) {
		if(cnx_stringview_at(specifier, 0) != 'D') {
			context.is_valid = CNX_FORMAT_BAD_SPECIFIER_INVALID_CHAR_IN_SPECIFIER;
			return context;
		}

		state.is_debug = true;
	}

	*(static_cast(CnxVectorIdentifier(VECTOR_T, FormatContext)*)(context.state)) = state;
	return context;
}

VECTOR_STATIC VECTOR_INLINE CnxString CnxVectorIdentifier(VECTOR_T,
														  format)(const CnxFormat* restrict self,
																  CnxFormatContext context) {
	return CnxVectorIdentifier(VECTOR_T, format_with_allocator)(self, context, DEFAULT_ALLOCATOR);
}

VECTOR_STATIC VECTOR_INLINE CnxString
CnxVectorIdentifier(VECTOR_T, format_with_allocator)(const CnxFormat* restrict self,
													 __attr(maybe_unused) CnxFormatContext context,
													 CnxAllocator allocator) {
	cnx_assert(context.is_valid == CNX_FORMAT_SUCCESS,
			   "Invalid format specifier used to format a " AS_STRING(CnxVector(VECTOR_T)));

	let _self = *static_cast(const CnxVector(VECTOR_T)*)(self->m_self);
	let size = cnx_vector_size(_self);
	let capacity = cnx_vector_capacity(_self);
	let data = cnx_vector_data(_self);
	return cnx_format_with_allocator(
		AS_STRING(CnxVector(VECTOR_T)) ": [size: {d}, capacity: {d}, data pointer: {x}]",
		allocator,
		size,
		capacity,
		as_format_t(nullptr_t, data));
}

	#undef CNX_TEMPLATE_SUPPRESS_INSTANTIATIONS
#endif // defined(VECTOR_T) && defined(VECTOR_SMALL_OPT_CAPACITY) && VECTOR_IMPL
