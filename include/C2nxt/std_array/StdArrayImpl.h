/// @file StdArrayImpl.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief This module provides the function definitions for a template instantiation of
/// `StdArray(ARRAY_T, ARRAY_N)`
/// @version 0.2.0
/// @date 2022-03-21
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

	#define STD_TEMPLATE_SUPPRESS_INSTANTIATIONS TRUE

	#include <C2nxt/StdAllocators.h>
	#include <C2nxt/StdBasicTypes.h>
	#include <C2nxt/StdCollectionData.h>
	#include <C2nxt/StdIterator.h>
	#include <C2nxt/StdPlatform.h>
	#include <C2nxt/std_array/StdArrayDef.h>
	#include <C2nxt/std_option/StdOptionDef.h>

StdArrayIterator(ARRAY_T, ARRAY_N)
	StdArrayIdentifier(ARRAY_T, ARRAY_N, iterator_new)(const StdArray(ARRAY_T,
																	  ARRAY_N) * restrict self);
StdArrayConstIterator(ARRAY_T, ARRAY_N)
	StdArrayIdentifier(ARRAY_T,
					   ARRAY_N,
					   const_iterator_new)(const StdArray(ARRAY_T, ARRAY_N) * restrict self);

Ref(ARRAY_T)
	StdArrayIdentifier(ARRAY_T,
					   ARRAY_N,
					   iterator_next)(StdRandomAccessIterator(Ref(ARRAY_T)) * restrict self);
Ref(ARRAY_T)
	StdArrayIdentifier(ARRAY_T,
					   ARRAY_N,
					   iterator_previous)(StdRandomAccessIterator(Ref(ARRAY_T)) * restrict self);
Ref(ARRAY_T)
	StdArrayIdentifier(ARRAY_T,
					   ARRAY_N,
					   iterator_at)(const StdRandomAccessIterator(Ref(ARRAY_T)) * restrict self,
									usize index);
Ref(ARRAY_T)
	StdArrayIdentifier(ARRAY_T,
					   ARRAY_N,
					   iterator_rat)(const StdRandomAccessIterator(Ref(ARRAY_T)) * restrict self,
									 usize index);
Ref(ARRAY_T) StdArrayIdentifier(ARRAY_T, ARRAY_N, iterator_current)(
	const StdRandomAccessIterator(Ref(ARRAY_T)) * restrict self);
bool
	StdArrayIdentifier(ARRAY_T,
					   ARRAY_N,
					   iterator_equals)(const StdRandomAccessIterator(Ref(ARRAY_T)) * restrict self,
										const StdRandomAccessIterator(Ref(ARRAY_T)) * restrict rhs);
ConstRef(ARRAY_T)
	StdArrayIdentifier(ARRAY_T,
					   ARRAY_N,
					   iterator_cnext)(StdRandomAccessIterator(ConstRef(ARRAY_T)) * restrict self);
ConstRef(ARRAY_T) StdArrayIdentifier(ARRAY_T, ARRAY_N, iterator_cprevious)(
	StdRandomAccessIterator(ConstRef(ARRAY_T)) * restrict self);
ConstRef(ARRAY_T) StdArrayIdentifier(ARRAY_T, ARRAY_N, iterator_cat)(
	const StdRandomAccessIterator(ConstRef(ARRAY_T)) * restrict self,
	usize index);
ConstRef(ARRAY_T) StdArrayIdentifier(ARRAY_T, ARRAY_N, iterator_crat)(
	const StdRandomAccessIterator(ConstRef(ARRAY_T)) * restrict self,
	usize index);
ConstRef(ARRAY_T) StdArrayIdentifier(ARRAY_T, ARRAY_N, iterator_ccurrent)(
	const StdRandomAccessIterator(ConstRef(ARRAY_T)) * restrict self);
bool StdArrayIdentifier(ARRAY_T, ARRAY_N, iterator_cequals)(
	const StdRandomAccessIterator(ConstRef(ARRAY_T)) * restrict self,
	const StdRandomAccessIterator(ConstRef(ARRAY_T)) * restrict rhs);

ImplIntoStdRandomAccessIterator(StdArray(ARRAY_T, ARRAY_N),
								Ref(ARRAY_T),
								StdArrayIdentifier(ARRAY_T, ARRAY_N, into_iter),
								into,
								StdArrayIdentifier(ARRAY_T, ARRAY_N, iterator_new),
								StdArrayIdentifier(ARRAY_T, ARRAY_N, iterator_next),
								StdArrayIdentifier(ARRAY_T, ARRAY_N, iterator_previous),
								StdArrayIdentifier(ARRAY_T, ARRAY_N, iterator_at),
								StdArrayIdentifier(ARRAY_T, ARRAY_N, iterator_current),
								StdArrayIdentifier(ARRAY_T, ARRAY_N, iterator_equals));
ImplIntoStdRandomAccessIterator(StdArray(ARRAY_T, ARRAY_N),
								Ref(ARRAY_T),
								StdArrayIdentifier(ARRAY_T, ARRAY_N, into_reverse_iter),
								into_reverse,
								StdArrayIdentifier(ARRAY_T, ARRAY_N, iterator_new),
								StdArrayIdentifier(ARRAY_T, ARRAY_N, iterator_next),
								StdArrayIdentifier(ARRAY_T, ARRAY_N, iterator_previous),
								StdArrayIdentifier(ARRAY_T, ARRAY_N, iterator_rat),
								StdArrayIdentifier(ARRAY_T, ARRAY_N, iterator_current),
								StdArrayIdentifier(ARRAY_T, ARRAY_N, iterator_equals));

ImplIntoStdRandomAccessIterator(StdArray(ARRAY_T, ARRAY_N),
								ConstRef(ARRAY_T),
								StdArrayIdentifier(ARRAY_T, ARRAY_N, into_const_iter),
								into,
								StdArrayIdentifier(ARRAY_T, ARRAY_N, const_iterator_new),
								StdArrayIdentifier(ARRAY_T, ARRAY_N, iterator_cnext),
								StdArrayIdentifier(ARRAY_T, ARRAY_N, iterator_cprevious),
								StdArrayIdentifier(ARRAY_T, ARRAY_N, iterator_cat),
								StdArrayIdentifier(ARRAY_T, ARRAY_N, iterator_ccurrent),
								StdArrayIdentifier(ARRAY_T, ARRAY_N, iterator_cequals));
ImplIntoStdRandomAccessIterator(StdArray(ARRAY_T, ARRAY_N),
								ConstRef(ARRAY_T),
								StdArrayIdentifier(ARRAY_T, ARRAY_N, into_reverse_const_iter),
								into_reverse,
								StdArrayIdentifier(ARRAY_T, ARRAY_N, const_iterator_new),
								StdArrayIdentifier(ARRAY_T, ARRAY_N, iterator_cnext),
								StdArrayIdentifier(ARRAY_T, ARRAY_N, iterator_cprevious),
								StdArrayIdentifier(ARRAY_T, ARRAY_N, iterator_crat),
								StdArrayIdentifier(ARRAY_T, ARRAY_N, iterator_ccurrent),
								StdArrayIdentifier(ARRAY_T, ARRAY_N, iterator_cequals));

[[always_inline]] static inline ARRAY_T
StdArrayIdentifier(ARRAY_T, ARRAY_N, default_constructor)([[maybe_unused]] StdAllocator allocator) {
	return (ARRAY_T){0};
}

[[always_inline]] static inline ARRAY_T
StdArrayIdentifier(ARRAY_T,
				   ARRAY_N,
				   default_copy_constructor)(const ARRAY_T* restrict element,
											 [[maybe_unused]] StdAllocator allocator) {
	return *element;
}

[[always_inline]] static inline void StdArrayIdentifier(ARRAY_T, ARRAY_N, default_destructor)(
	[[maybe_unused]] ARRAY_T* restrict element, /** NOLINT(readability-non-const-parameter) **/
	[[maybe_unused]] StdAllocator allocator) {
}

static const struct StdArrayIdentifier(ARRAY_T, ARRAY_N, vtable)
	StdArrayIdentifier(ARRAY_T, ARRAY_N, vtable_impl)
	= {
		.clone = StdArrayIdentifier(ARRAY_T, ARRAY_N, clone),
		.at_const = StdArrayIdentifier(ARRAY_T, ARRAY_N, at_const),
		.at_mut = StdArrayIdentifier(ARRAY_T, ARRAY_N, at_mut),
		.front_const = StdArrayIdentifier(ARRAY_T, ARRAY_N, front_const),
		.front_mut = StdArrayIdentifier(ARRAY_T, ARRAY_N, front_mut),
		.back_const = StdArrayIdentifier(ARRAY_T, ARRAY_N, back_const),
		.back_mut = StdArrayIdentifier(ARRAY_T, ARRAY_N, back_mut),
		.data_const = StdArrayIdentifier(ARRAY_T, ARRAY_N, data_const),
		.data_mut = StdArrayIdentifier(ARRAY_T, ARRAY_N, data_mut),
		.is_empty = StdArrayIdentifier(ARRAY_T, ARRAY_N, is_empty),
		.is_full = StdArrayIdentifier(ARRAY_T, ARRAY_N, is_full),
		.size = StdArrayIdentifier(ARRAY_T, ARRAY_N, size),
		.capacity = StdArrayIdentifier(ARRAY_T, ARRAY_N, capacity),
		.resize = StdArrayIdentifier(ARRAY_T, ARRAY_N, resize),
		.clear = StdArrayIdentifier(ARRAY_T, ARRAY_N, clear),
		.push_back = StdArrayIdentifier(ARRAY_T, ARRAY_N, push_back),
		.pop_back = StdArrayIdentifier(ARRAY_T, ARRAY_N, pop_back),
		.insert = StdArrayIdentifier(ARRAY_T, ARRAY_N, insert),
		.erase = StdArrayIdentifier(ARRAY_T, ARRAY_N, erase),
		.erase_n = StdArrayIdentifier(ARRAY_T, ARRAY_N, erase_n),
		.free = StdArrayIdentifier(ARRAY_T, ARRAY_N, free),
		.into_iter = StdArrayIdentifier(ARRAY_T, ARRAY_N, into_iter),
		.into_reverse_iter = StdArrayIdentifier(ARRAY_T, ARRAY_N, into_reverse_iter),
		.into_const_iter = StdArrayIdentifier(ARRAY_T, ARRAY_N, into_const_iter),
		.into_reverse_const_iter = StdArrayIdentifier(ARRAY_T, ARRAY_N, into_reverse_const_iter),
		.begin = StdArrayIdentifier(ARRAY_T, ARRAY_N, begin),
		.end = StdArrayIdentifier(ARRAY_T, ARRAY_N, end),
		.rbegin = StdArrayIdentifier(ARRAY_T, ARRAY_N, rbegin),
		.rend = StdArrayIdentifier(ARRAY_T, ARRAY_N, rend),
		.cbegin = StdArrayIdentifier(ARRAY_T, ARRAY_N, cbegin),
		.cend = StdArrayIdentifier(ARRAY_T, ARRAY_N, cend),
		.crbegin = StdArrayIdentifier(ARRAY_T, ARRAY_N, crbegin),
		.crend = StdArrayIdentifier(ARRAY_T, ARRAY_N, crend),
};

static const struct StdCollectionData(StdArray(ARRAY_T, ARRAY_N))
	StdArrayIdentifier(ARRAY_T, ARRAY_N, default_collection_data)
	= {.m_constructor = StdArrayIdentifier(ARRAY_T, ARRAY_N, default_constructor),
	   .m_copy_constructor = StdArrayIdentifier(ARRAY_T, ARRAY_N, default_copy_constructor),
	   .m_destructor = StdArrayIdentifier(ARRAY_T, ARRAY_N, default_destructor)};

StdArray(ARRAY_T, ARRAY_N) StdArrayIdentifier(ARRAY_T, ARRAY_N, new)(void) {
	return std_array_new_with_allocator_and_collection_data(
		ARRAY_T,
		ARRAY_N,
		DEFAULT_ALLOCATOR,
		&StdArrayIdentifier(ARRAY_T, ARRAY_N, default_collection_data));
}

StdArray(ARRAY_T, ARRAY_N)
	StdArrayIdentifier(ARRAY_T, ARRAY_N, new_with_allocator)(StdAllocator allocator) {
	return std_array_new_with_allocator_and_collection_data(
		ARRAY_T,
		ARRAY_N,
		allocator,
		&StdArrayIdentifier(ARRAY_T, ARRAY_N, default_collection_data));
}

StdArray(ARRAY_T, ARRAY_N) StdArrayIdentifier(ARRAY_T, ARRAY_N, new_with_collection_data)(
	const StdCollectionData(StdArray(ARRAY_T, ARRAY_N)) * restrict data) {
	return std_array_new_with_allocator_and_collection_data(ARRAY_T,
															ARRAY_N,
															DEFAULT_ALLOCATOR,
															data);
}

StdArray(ARRAY_T, ARRAY_N)
	StdArrayIdentifier(ARRAY_T, ARRAY_N, new_with_allocator_and_collection_data)(
		StdAllocator allocator,
		const StdCollectionData(StdArray(ARRAY_T, ARRAY_N)) * restrict data) {
	return (StdArray(ARRAY_T, ARRAY_N)){.m_array = {},
										.m_size = static_cast(usize)(0),
										.m_allocator = allocator,
										.m_data = data,
										.m_vtable
										= &StdArrayIdentifier(ARRAY_T, ARRAY_N, vtable_impl)};
}

StdArray(ARRAY_T, ARRAY_N)
	StdArrayIdentifier(ARRAY_T, ARRAY_N, clone)(const StdArray(ARRAY_T, ARRAY_N) * restrict self)
		std_disable_if(
			!(self->m_data->m_copy_constructor),
			"Can't clone a StdArray(ARRAY_T, ARRAY_N) with elements that aren't copyable (no "
			"element copy constructor defined)") {
	std_assert(self->m_data->m_copy_constructor != nullptr,
			   "Can't clone a `StdArray(ARRAY_T, ARRAY_N)` with elements that aren't copyable (no "
			   "element copy-constructor defined)");

	let_mut array = std_array_new_with_collection_data(ARRAY_T, ARRAY_N, self->m_data);

	foreach_ref(elem, *self) {
		std_array_push_back(array, self->m_data->m_copy_constructor(elem, self->m_allocator));
	}

	return array;
}

const ARRAY_T*
StdArrayIdentifier(ARRAY_T, ARRAY_N, at_const)(const StdArray(ARRAY_T, ARRAY_N) * restrict self,
											   usize index) {
	std_assert(index <= self->m_size,
			   "std_array_at_mut called with index > size (index out of bounds)");
	std_assert(index < ARRAY_N, "std_array_at_mut called with index >= N (index out of bounds)");
	return &(self->m_array[index]);
}

ARRAY_T* StdArrayIdentifier(ARRAY_T, ARRAY_N, at_mut)(StdArray(ARRAY_T, ARRAY_N) * restrict self,
													  usize index) {
	std_assert(index <= self->m_size,
			   "std_array_at_mut called with index > size (index out of bounds)");
	std_assert(index < ARRAY_N, "std_array_at_mut called with index >= N (index out of bounds)");
	return &(self->m_array[index]);
}

const ARRAY_T* StdArrayIdentifier(ARRAY_T,
								  ARRAY_N,
								  front_const)(const StdArray(ARRAY_T, ARRAY_N) * restrict self) {
	return &std_array_at(*self, 0);
}

ARRAY_T*
StdArrayIdentifier(ARRAY_T, ARRAY_N, front_mut)(StdArray(ARRAY_T, ARRAY_N) * restrict self) {
	return &std_array_at_mut(*self, 0);
}

const ARRAY_T*
StdArrayIdentifier(ARRAY_T, ARRAY_N, back_const)(const StdArray(ARRAY_T, ARRAY_N) * restrict self) {
	let back_index = self->m_size > 0 ? self->m_size - 1 : 0;
	return &std_array_at(*self, back_index);
}

ARRAY_T*
StdArrayIdentifier(ARRAY_T, ARRAY_N, back_mut)(StdArray(ARRAY_T, ARRAY_N) * restrict self) {
	let back_index = self->m_size > 0 ? self->m_size - 1 : 0;
	return &std_array_at_mut(*self, back_index);
}

const ARRAY_T*
StdArrayIdentifier(ARRAY_T, ARRAY_N, data_const)(const StdArray(ARRAY_T, ARRAY_N) * restrict self) {
	return &std_array_front(*self);
}

ARRAY_T*
StdArrayIdentifier(ARRAY_T, ARRAY_N, data_mut)(StdArray(ARRAY_T, ARRAY_N) * restrict self) {
	return &std_array_front_mut(*self);
}

bool StdArrayIdentifier(ARRAY_T, ARRAY_N, is_empty)(const StdArray(ARRAY_T,
																   ARRAY_N) * restrict self) {
	return self->m_size == 0;
}

bool StdArrayIdentifier(ARRAY_T, ARRAY_N, is_full)(const StdArray(ARRAY_T,
																  ARRAY_N) * restrict self) {
	return self->m_size == ARRAY_N;
}

usize StdArrayIdentifier(ARRAY_T, ARRAY_N, size)(const StdArray(ARRAY_T, ARRAY_N) * restrict self) {
	return self->m_size;
}

usize StdArrayIdentifier(ARRAY_T, ARRAY_N, capacity)(const StdArray(ARRAY_T,
																	ARRAY_N) * restrict self) {
	ignore(self);
	return ARRAY_N;
}

void StdArrayIdentifier(ARRAY_T, ARRAY_N, resize_internal)(StdArray(ARRAY_T,
																	ARRAY_N) * restrict self,
														   usize new_size) {
	if(new_size < self->m_size) {
		for(let_mut i = new_size; i < self->m_size; ++i) {
			self->m_data->m_destructor(&std_array_at_mut(*self, i), self->m_allocator);
		}
		self->m_size = new_size;
	}
	else {
		let old_size = self->m_size;
		self->m_size = new_size;
		// cast away const for GCC compat
		for(let_mut i = const_cast(usize)(old_size); i < new_size; ++i) {
			std_array_at_mut(*self, i) = self->m_data->m_constructor(self->m_allocator);
		}
	}
}

void StdArrayIdentifier(ARRAY_T, ARRAY_N, resize)(StdArray(ARRAY_T, ARRAY_N) * restrict self,
												  usize new_size) {
	std_assert(new_size <= ARRAY_N, "can't resize an array larger than it's capacity");
	StdArrayIdentifier(ARRAY_T, ARRAY_N, resize_internal)(self, new_size);
}

void StdArrayIdentifier(ARRAY_T, ARRAY_N, clear)(StdArray(ARRAY_T, ARRAY_N) * restrict self) {
	for(let_mut i = 0U; i < self->m_size; ++i) {
		self->m_data->m_destructor(&std_array_at_mut(*self, i), self->m_allocator);
	}
	self->m_size = 0;
}

void StdArrayIdentifier(ARRAY_T, ARRAY_N, push_back)(
	StdArray(ARRAY_T, ARRAY_N) * restrict self,
	ARRAY_T element /** NOLINT(readability-non-const-parameter) **/) {
	std_assert(self->m_size < ARRAY_N,
			   "std_array_push_back called with self->m_size >= N (array bounds full)");
	std_array_at_mut(*self, self->m_size) = element;
	self->m_size++;
}

StdOption(ARRAY_T)
	StdArrayIdentifier(ARRAY_T, ARRAY_N, pop_back)(StdArray(ARRAY_T, ARRAY_N) * restrict self) {
	if(self->m_size == 0) {
		return None(ARRAY_T);
	}

	let_mut ptr = &std_array_at_mut(*self, self->m_size - 1);
	let elem = Some(ARRAY_T, *ptr);
	self->m_data->m_destructor(ptr, self->m_allocator);
	*ptr = (ARRAY_T){0};
	self->m_size--;
	return elem;
}

void StdArrayIdentifier(ARRAY_T,
						ARRAY_N,
						insert)(StdArray(ARRAY_T, ARRAY_N) * restrict self,
								ARRAY_T element /** NOLINT(readability-non-const-parameter **/,
								usize index) {
	std_assert(index < ARRAY_N, "std_array_insert called with index >= N (index out of bounds)");
	std_assert(self->m_size < ARRAY_N,
			   "std_array_insert called with self->m_size >= N (array bounds full)");

	if(index != self->m_size) {
		let num_to_move = self->m_size - index;
		std_memmove(ARRAY_T,
					&std_array_at_mut(*self, index + 1),
					&std_array_at_mut(*self, index),
					num_to_move);
	}
	std_array_at_mut(*self, index) = element;
	self->m_size++;
}

void StdArrayIdentifier(ARRAY_T, ARRAY_N, erase)(StdArray(ARRAY_T, ARRAY_N) * restrict self,
												 usize index) {
	std_assert(index < self->m_size,
			   "std_array_erase called with index >= self->m_size (index out of bounds)");
	self->m_data->m_destructor(&std_array_at_mut(*self, index), self->m_allocator);
	if(index != self->m_size - 1) {
		let num_to_move = self->m_size - (index + 1);
		std_memmove(ARRAY_T,
					&std_array_at_mut(*self, index),
					&std_array_at_mut(*self, index + 1),
					num_to_move);
	}
	self->m_size--;
}

void StdArrayIdentifier(ARRAY_T, ARRAY_N, erase_n)(StdArray(ARRAY_T, ARRAY_N) * restrict self,
												   usize index,
												   usize length) {
	std_assert(index < self->m_size,
			   "std_array_erase_n called with index >= self->m_size (index out of bounds)");
	std_assert(index + length < self->m_size,
			   "std_array_erase_n called with index + length >= self->m_size (range out "
			   "of bounds)");

	let end = index + length;
	let num_to_move = self->m_size - end;
	for(let_mut i = index; i < end; ++i) {
		self->m_data->m_destructor(&std_array_at_mut(*self, i), self->m_allocator);
	}
	if(end != self->m_size) {
		std_memmove(ARRAY_T,
					&std_array_at_mut(*self, index),
					&std_array_at_mut(*self, end),
					num_to_move);
	}
	self->m_size -= length;
}

void StdArrayIdentifier(ARRAY_T, ARRAY_N, free)(void* restrict self) {
	let self_ = static_cast(StdArray(ARRAY_T, ARRAY_N)*)(self);
	for(let_mut i = 0U; i < self_->m_size; ++i) {
		self_->m_data->m_destructor(&std_array_at_mut(*self_, i), self_->m_allocator);
	}
	self_->m_size = 0;
}

StdArrayIterator(ARRAY_T, ARRAY_N)
	StdArrayIdentifier(ARRAY_T, ARRAY_N, iterator_new)(const StdArray(ARRAY_T,
																	  ARRAY_N) * restrict self) {
	return (StdArrayIterator(ARRAY_T, ARRAY_N)){.m_index = 0U,
												.m_array
												= const_cast(StdArray(ARRAY_T, ARRAY_N)*)(self)};
}

StdArrayConstIterator(ARRAY_T, ARRAY_N)
	StdArrayIdentifier(ARRAY_T,
					   ARRAY_N,
					   const_iterator_new)(const StdArray(ARRAY_T, ARRAY_N) * restrict self) {
	return (StdArrayConstIterator(ARRAY_T, ARRAY_N)){
		.m_index = 0U,
		.m_array = const_cast(StdArray(ARRAY_T, ARRAY_N)*)(self)};
}

Ref(ARRAY_T)
	StdArrayIdentifier(ARRAY_T,
					   ARRAY_N,
					   iterator_next)(StdRandomAccessIterator(Ref(ARRAY_T)) * restrict self) {
	let _self = static_cast(StdArrayIterator(ARRAY_T, ARRAY_N)*)(self->m_self);
	std_assert(_self->m_index > -1,
			   "Iterator value accessed when iterator is positioned before the beginning of "
			   "the iteration (iterator out of bounds)");
	std_assert(static_cast(usize)(_self->m_index) < std_array_size(*(_self->m_array)),
			   "Iterator value accessed when iterator is positioned after the end of "
			   "the iteration (iterator out of bounds)");
	_self->m_index++;
	if(static_cast(usize)(_self->m_index) >= std_array_size(*(_self->m_array))) {
		return &std_array_at_mut(*(_self->m_array), std_array_size(*(_self->m_array)) - 1);
	}

	return &std_array_at_mut(*(_self->m_array), static_cast(usize)(_self->m_index));
}

Ref(ARRAY_T)
	StdArrayIdentifier(ARRAY_T,
					   ARRAY_N,
					   iterator_previous)(StdRandomAccessIterator(Ref(ARRAY_T)) * restrict self) {
	let _self = static_cast(StdArrayIterator(ARRAY_T, ARRAY_N)*)(self->m_self);
	std_assert(_self->m_index > -1,
			   "Iterator value accessed when iterator is positioned before the beginning of "
			   "the iteration (iterator out of bounds)");
	std_assert(static_cast(usize)(_self->m_index) < std_array_size(*(_self->m_array)),
			   "Iterator value accessed when iterator is positioned after the end of the "
			   "iteration (iterator out of bounds)");
	_self->m_index--;
	if(_self->m_index < 0) {
		return &std_array_at_mut(*(_self->m_array), 0);
	}

	return &std_array_at_mut(*(_self->m_array), static_cast(usize)(_self->m_index));
}

Ref(ARRAY_T)
	StdArrayIdentifier(ARRAY_T,
					   ARRAY_N,
					   iterator_at)(const StdRandomAccessIterator(Ref(ARRAY_T)) * restrict self,
									usize index) {
	let _self = static_cast(StdArrayIterator(ARRAY_T, ARRAY_N)*)(self->m_self);
	std_assert(index < std_array_size(*(_self->m_array)),
			   "Iterator value accessed when iterator is positioned after the end of "
			   "the iteration (iterator out of bounds)");
	return &std_array_at_mut(*(_self->m_array), index);
}

Ref(ARRAY_T)
	StdArrayIdentifier(ARRAY_T,
					   ARRAY_N,
					   iterator_rat)(const StdRandomAccessIterator(Ref(ARRAY_T)) * restrict self,
									 usize index) {
	let _self = static_cast(StdArrayIterator(ARRAY_T, ARRAY_N)*)(self->m_self);
	std_assert(index < std_array_size(*(_self->m_array)),
			   "Iterator value accessed when iterator is positioned "
			   "after the end of the iteration (iterator out of bounds)");
	return &std_array_at_mut(*(_self->m_array), (std_array_size(*(_self->m_array)) - 1) - index);
}

Ref(ARRAY_T) StdArrayIdentifier(ARRAY_T, ARRAY_N, iterator_current)(
	const StdRandomAccessIterator(Ref(ARRAY_T)) * restrict self) {
	let _self = static_cast(StdArrayIterator(ARRAY_T, ARRAY_N)*)(self->m_self);
	std_assert(_self->m_index > -1,
			   "Iterator value accessed when iterator is positioned before the "
			   "beginning of the iteration (iterator out of bounds)");
	std_assert(static_cast(usize)(_self->m_index) < std_array_size(*(_self->m_array)),
			   "Iterator value accessed when iterator is positioned after the "
			   "end of the iteration (iterator out of bounds)");
	return &std_array_at_mut(*(_self->m_array), static_cast(usize)(_self->m_index));
}

bool StdArrayIdentifier(ARRAY_T, ARRAY_N, iterator_equals)(
	const StdRandomAccessIterator(Ref(ARRAY_T)) * restrict self,
	const StdRandomAccessIterator(Ref(ARRAY_T)) * restrict rhs) {
	let _self = static_cast(const StdArrayIterator(ARRAY_T, ARRAY_N)*)(self->m_self);
	let _rhs = static_cast(const StdArrayIterator(ARRAY_T, ARRAY_N)*)(rhs->m_self);
	return _self->m_index == _rhs->m_index && _self->m_array == _rhs->m_array;
}

ConstRef(ARRAY_T)
	StdArrayIdentifier(ARRAY_T,
					   ARRAY_N,
					   iterator_cnext)(StdRandomAccessIterator(ConstRef(ARRAY_T)) * restrict self) {
	let _self = static_cast(StdArrayConstIterator(ARRAY_T, ARRAY_N)*)(self->m_self);

	std_assert(_self->m_index > -1,
			   "Iterator value accessed when iterator is positioned before the "
			   "beginning of the iteration (iterator out of bounds)");
	std_assert(static_cast(usize)(_self->m_index) < std_array_size(*(_self->m_array)),
			   "Iterator value accessed when iterator is positioned after the "
			   "end of the iteration (iterator out of bounds)");
	_self->m_index++;
	if(static_cast(usize)(_self->m_index) >= std_array_size(*(_self->m_array))) {
		return &std_array_at(*(_self->m_array), std_array_size(*(_self->m_array)) - 1);
	}

	return &std_array_at(*(_self->m_array), static_cast(usize)(_self->m_index));
}

ConstRef(ARRAY_T) StdArrayIdentifier(ARRAY_T, ARRAY_N, iterator_cprevious)(
	StdRandomAccessIterator(ConstRef(ARRAY_T)) * restrict self) {
	let _self = static_cast(StdArrayConstIterator(ARRAY_T, ARRAY_N)*)(self->m_self);
	std_assert(_self->m_index > -1,
			   "Iterator value accessed when iterator is positioned before "
			   "the beginning of the iteration (iterator out of bounds)");
	std_assert(static_cast(usize)(_self->m_index) < std_array_size(*(_self->m_array)),
			   "Iterator value accessed when iterator is positioned after "
			   "the end of the iteration (iterator out of bounds)");
	_self->m_index--;
	if(_self->m_index < 0) {
		return &std_array_at(*(_self->m_array), 0);
	}

	return &std_array_at(*(_self->m_array), static_cast(usize)(_self->m_index));
}

ConstRef(ARRAY_T) StdArrayIdentifier(ARRAY_T, ARRAY_N, iterator_cat)(
	const StdRandomAccessIterator(ConstRef(ARRAY_T)) * restrict self,
	usize index) {
	let _self = static_cast(StdArrayConstIterator(ARRAY_T, ARRAY_N)*)(self->m_self);
	std_assert(index < std_array_size(*(_self->m_array)),
			   "Iterator value accessed when iterator is positioned "
			   "after the end of the iteration (iterator out of bounds)");

	return &std_array_at(*(_self->m_array), index);
}

ConstRef(ARRAY_T) StdArrayIdentifier(ARRAY_T, ARRAY_N, iterator_crat)(
	const StdRandomAccessIterator(ConstRef(ARRAY_T)) * restrict self,
	usize index) {
	let _self = static_cast(StdArrayConstIterator(ARRAY_T, ARRAY_N)*)(self->m_self);
	std_assert(index < std_array_size(*(_self->m_array)),
			   "Iterator value accessed when iterator is positioned "
			   "after the end of the iteration (iterator out of bounds)");

	return &std_array_at(*(_self->m_array), (std_array_size(*(_self->m_array)) - 1) - index);
}

ConstRef(ARRAY_T) StdArrayIdentifier(ARRAY_T, ARRAY_N, iterator_ccurrent)(
	const StdRandomAccessIterator(ConstRef(ARRAY_T)) * restrict self) {
	let _self = static_cast(StdArrayConstIterator(ARRAY_T, ARRAY_N)*)(self->m_self);
	std_assert(_self->m_index > -1,
			   "Iterator value accessed when iterator is positioned before "
			   "the beginning of the iteration (iterator out of bounds)");
	std_assert(static_cast(usize)(_self->m_index) < std_array_size(*(_self->m_array)),
			   "Iterator value accessed when iterator is positioned after "
			   "the end of the iteration (iterator out of bounds)");

	return &std_array_at(*(_self->m_array), static_cast(usize)(_self->m_index));
}

bool StdArrayIdentifier(ARRAY_T, ARRAY_N, iterator_cequals)(
	const StdRandomAccessIterator(ConstRef(ARRAY_T)) * restrict self,
	const StdRandomAccessIterator(ConstRef(ARRAY_T)) * restrict rhs) {
	let _self = static_cast(const StdArrayConstIterator(ARRAY_T, ARRAY_N)*)(self->m_self);
	let _rhs = static_cast(const StdArrayConstIterator(ARRAY_T, ARRAY_N)*)(rhs->m_self);
	return _self->m_index == _rhs->m_index && _self->m_array == _rhs->m_array;
}

StdRandomAccessIterator(Ref(ARRAY_T))
	StdArrayIdentifier(ARRAY_T, ARRAY_N, begin)(StdArray(ARRAY_T, ARRAY_N) * restrict self) {
	let_mut iter = std_array_into_iter(*self);
	let_mut inner = static_cast(StdArrayIterator(ARRAY_T, ARRAY_N)*)(iter.m_self);
	inner->m_index = 0;
	return iter;
}

StdRandomAccessIterator(Ref(ARRAY_T))
	StdArrayIdentifier(ARRAY_T, ARRAY_N, end)(StdArray(ARRAY_T, ARRAY_N) * restrict self) {
	let_mut iter = std_array_into_iter(*self);
	let_mut inner = static_cast(StdArrayIterator(ARRAY_T, ARRAY_N)*)(iter.m_self);
	inner->m_index = static_cast(isize)(std_array_size(*self));
	return iter;
}

StdRandomAccessIterator(Ref(ARRAY_T))
	StdArrayIdentifier(ARRAY_T, ARRAY_N, rbegin)(StdArray(ARRAY_T, ARRAY_N) * restrict self) {
	let_mut iter = std_array_into_reverse_iter(*self);
	let_mut inner = static_cast(StdArrayIterator(ARRAY_T, ARRAY_N)*)(iter.m_self);
	inner->m_index = static_cast(isize)(std_array_size(*self) - 1);
	return iter;
}

StdRandomAccessIterator(Ref(ARRAY_T))
	StdArrayIdentifier(ARRAY_T, ARRAY_N, rend)(StdArray(ARRAY_T, ARRAY_N) * restrict self) {
	let_mut iter = std_array_into_reverse_iter(*self);
	let_mut inner = static_cast(StdArrayIterator(ARRAY_T, ARRAY_N)*)(iter.m_self);
	inner->m_index = -1;
	return iter;
}

StdRandomAccessIterator(ConstRef(ARRAY_T))
	StdArrayIdentifier(ARRAY_T, ARRAY_N, cbegin)(const StdArray(ARRAY_T, ARRAY_N) * restrict self) {
	let_mut iter = std_array_into_const_iter(*self);
	let_mut inner = static_cast(StdArrayConstIterator(ARRAY_T, ARRAY_N)*)(iter.m_self);
	inner->m_index = 0;
	return iter;
}

StdRandomAccessIterator(ConstRef(ARRAY_T))
	StdArrayIdentifier(ARRAY_T, ARRAY_N, cend)(const StdArray(ARRAY_T, ARRAY_N) * restrict self) {
	let_mut iter = std_array_into_const_iter(*self);
	let_mut inner = static_cast(StdArrayConstIterator(ARRAY_T, ARRAY_N)*)(iter.m_self);
	inner->m_index = static_cast(isize)(std_array_size(*self));
	return iter;
}

StdRandomAccessIterator(ConstRef(ARRAY_T))
	StdArrayIdentifier(ARRAY_T, ARRAY_N, crbegin)(const StdArray(ARRAY_T,
																 ARRAY_N) * restrict self) {
	let_mut iter = std_array_into_reverse_const_iter(*self);
	let_mut inner = static_cast(StdArrayConstIterator(ARRAY_T, ARRAY_N)*)(iter.m_self);
	inner->m_index = static_cast(isize)(std_array_size(*self) - 1);
	return iter;
}

StdRandomAccessIterator(ConstRef(ARRAY_T))
	StdArrayIdentifier(ARRAY_T, ARRAY_N, crend)(const StdArray(ARRAY_T, ARRAY_N) * restrict self) {
	let_mut iter = std_array_into_reverse_const_iter(*self);
	let_mut inner = static_cast(StdArrayConstIterator(ARRAY_T, ARRAY_N)*)(iter.m_self);
	inner->m_index = -1;
	return iter;
}

StdString StdArrayIdentifier(ARRAY_T, ARRAY_N, format)(const StdFormat* restrict self,
													   StdFormatSpecifier specifier) {
	return StdArrayIdentifier(ARRAY_T, ARRAY_N, format_with_allocator)(self,
																	   specifier,
																	   DEFAULT_ALLOCATOR);
}

StdString StdArrayIdentifier(ARRAY_T,
							 ARRAY_N,
							 format_with_allocator)(const StdFormat* restrict self,
													[[maybe_unused]] StdFormatSpecifier specifier,
													StdAllocator allocator) {
	std_assert(specifier.m_type == STD_FORMAT_TYPE_DEFAULT
				   || specifier.m_type == STD_FORMAT_TYPE_DEBUG,
			   "Can only format StdArray with default or debug format specifier");

	let _self = *static_cast(const StdArray(ARRAY_T, ARRAY_N)*)(self->m_self);
	let size = std_array_size(_self);
	let capacity = std_array_capacity(_self);
	let data = std_array_data(_self);
	return std_format_with_allocator(
		AS_STRING(StdArray(ARRAY_T, ARRAY_N)) ": [size: {d}, capacity: {d}, data pointer: {x}]",
		allocator,
		size,
		capacity,
		as_format_t(nullptr_t, data));
}

	#undef STD_TEMPLATE_SUPPRESS_INSTANTIATIONS
#endif // defined(ARRAY_T) && defined(ARRAY_N) && ARRAY_IMPL
