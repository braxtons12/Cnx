/// @file StdVectorImpl.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief This module provides the function definitions for a template instantiation of
/// `StdVector(VECTOR_T)`
/// @version 0.2.2
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

#if defined(VECTOR_T) && defined(VECTOR_SMALL_OPT_CAPACITY) && VECTOR_IMPL

	#define STD_TEMPLATE_SUPPRESS_INSTANTIATIONS TRUE

	#include <C2nxt/StdAllocators.h>
	#include <C2nxt/StdBasicTypes.h>
	#include <C2nxt/StdCollectionData.h>
	#include <C2nxt/StdIterator.h>
	#include <C2nxt/StdPlatform.h>
	#include <C2nxt/StdFormat.h>
	#include <C2nxt/std_vector/StdVectorDef.h>

StdVectorIterator(VECTOR_T)
	StdVectorIdentifier(VECTOR_T, iterator_new)(const StdVector(VECTOR_T) * restrict self);
StdVectorConstIterator(VECTOR_T)
	StdVectorIdentifier(VECTOR_T, const_iterator_new)(const StdVector(VECTOR_T) * restrict self);

Ref(VECTOR_T)
	StdVectorIdentifier(VECTOR_T,
						iterator_next)(StdRandomAccessIterator(Ref(VECTOR_T)) * restrict self);
Ref(VECTOR_T)
	StdVectorIdentifier(VECTOR_T,
						iterator_previous)(StdRandomAccessIterator(Ref(VECTOR_T)) * restrict self);
Ref(VECTOR_T)
	StdVectorIdentifier(VECTOR_T,
						iterator_at)(const StdRandomAccessIterator(Ref(VECTOR_T)) * restrict self,
									 usize index);
Ref(VECTOR_T)
	StdVectorIdentifier(VECTOR_T,
						iterator_rat)(const StdRandomAccessIterator(Ref(VECTOR_T)) * restrict self,
									  usize index);
Ref(VECTOR_T) StdVectorIdentifier(VECTOR_T, iterator_current)(
	const StdRandomAccessIterator(Ref(VECTOR_T)) * restrict self);
bool StdVectorIdentifier(VECTOR_T, iterator_equals)(
	const StdRandomAccessIterator(Ref(VECTOR_T)) * restrict self,
	const StdRandomAccessIterator(Ref(VECTOR_T)) * restrict rhs);

ConstRef(VECTOR_T) StdVectorIdentifier(VECTOR_T, iterator_cnext)(
	StdRandomAccessIterator(ConstRef(VECTOR_T)) * restrict self);
ConstRef(VECTOR_T) StdVectorIdentifier(VECTOR_T, iterator_cprevious)(
	StdRandomAccessIterator(ConstRef(VECTOR_T)) * restrict self);
ConstRef(VECTOR_T) StdVectorIdentifier(VECTOR_T, iterator_cat)(
	const StdRandomAccessIterator(ConstRef(VECTOR_T)) * restrict self,
	usize index);
ConstRef(VECTOR_T) StdVectorIdentifier(VECTOR_T, iterator_crat)(
	const StdRandomAccessIterator(ConstRef(VECTOR_T)) * restrict self,
	usize index);
ConstRef(VECTOR_T) StdVectorIdentifier(VECTOR_T, iterator_ccurrent)(
	const StdRandomAccessIterator(ConstRef(VECTOR_T)) * restrict self);
bool StdVectorIdentifier(VECTOR_T, iterator_cequals)(
	const StdRandomAccessIterator(ConstRef(VECTOR_T)) * restrict self,
	const StdRandomAccessIterator(ConstRef(VECTOR_T)) * restrict rhs);

ImplIntoStdRandomAccessIterator(StdVector(VECTOR_T),
								Ref(VECTOR_T),
								StdVectorIdentifier(VECTOR_T, into_iter),
								into,
								StdVectorIdentifier(VECTOR_T, iterator_new),
								StdVectorIdentifier(VECTOR_T, iterator_next),
								StdVectorIdentifier(VECTOR_T, iterator_previous),
								StdVectorIdentifier(VECTOR_T, iterator_at),
								StdVectorIdentifier(VECTOR_T, iterator_current),
								StdVectorIdentifier(VECTOR_T, iterator_equals));
ImplIntoStdRandomAccessIterator(StdVector(VECTOR_T),
								Ref(VECTOR_T),
								StdVectorIdentifier(VECTOR_T, into_reverse_iter),
								into_reverse,
								StdVectorIdentifier(VECTOR_T, iterator_new),
								StdVectorIdentifier(VECTOR_T, iterator_next),
								StdVectorIdentifier(VECTOR_T, iterator_previous),
								StdVectorIdentifier(VECTOR_T, iterator_rat),
								StdVectorIdentifier(VECTOR_T, iterator_current),
								StdVectorIdentifier(VECTOR_T, iterator_equals));

ImplIntoStdRandomAccessIterator(StdVector(VECTOR_T),
								ConstRef(VECTOR_T),
								StdVectorIdentifier(VECTOR_T, into_const_iter),
								into,
								StdVectorIdentifier(VECTOR_T, const_iterator_new),
								StdVectorIdentifier(VECTOR_T, iterator_cnext),
								StdVectorIdentifier(VECTOR_T, iterator_cprevious),
								StdVectorIdentifier(VECTOR_T, iterator_cat),
								StdVectorIdentifier(VECTOR_T, iterator_ccurrent),
								StdVectorIdentifier(VECTOR_T, iterator_cequals));
ImplIntoStdRandomAccessIterator(StdVector(VECTOR_T),
								ConstRef(VECTOR_T),
								StdVectorIdentifier(VECTOR_T, into_reverse_const_iter),
								into_reverse,
								StdVectorIdentifier(VECTOR_T, const_iterator_new),
								StdVectorIdentifier(VECTOR_T, iterator_cnext),
								StdVectorIdentifier(VECTOR_T, iterator_cprevious),
								StdVectorIdentifier(VECTOR_T, iterator_crat),
								StdVectorIdentifier(VECTOR_T, iterator_ccurrent),
								StdVectorIdentifier(VECTOR_T, iterator_cequals));

[[always_inline]] static inline VECTOR_T
StdVectorIdentifier(VECTOR_T, default_constructor)([[maybe_unused]] StdAllocator allocator) {
	return (VECTOR_T){0};
}

[[always_inline]] static inline VECTOR_T
StdVectorIdentifier(VECTOR_T, default_copy_constructor)(const VECTOR_T* restrict elem,
														[[maybe_unused]] StdAllocator allocator) {
	return *elem;
}

[[always_inline]] static inline void StdVectorIdentifier(VECTOR_T, default_destructor)(
	[[maybe_unused]] VECTOR_T* restrict element, /** NOLINT(readability-non-const-parameter)**/
	[[maybe_unused]] StdAllocator allocator) {
}

static const struct StdVectorIdentifier(VECTOR_T, vtable) StdVectorIdentifier(VECTOR_T, vtable_impl)
	= {
		.clone = StdVectorIdentifier(VECTOR_T, clone),
		.at_const = StdVectorIdentifier(VECTOR_T, at_const),
		.at_mut = StdVectorIdentifier(VECTOR_T, at_mut),
		.front_const = StdVectorIdentifier(VECTOR_T, front_const),
		.front_mut = StdVectorIdentifier(VECTOR_T, front_mut),
		.back_const = StdVectorIdentifier(VECTOR_T, back_const),
		.back_mut = StdVectorIdentifier(VECTOR_T, back_mut),
		.data_const = StdVectorIdentifier(VECTOR_T, data_const),
		.data_mut = StdVectorIdentifier(VECTOR_T, data_mut),
		.is_empty = StdVectorIdentifier(VECTOR_T, is_empty),
		.is_full = StdVectorIdentifier(VECTOR_T, is_full),
		.size = StdVectorIdentifier(VECTOR_T, size),
		.capacity = StdVectorIdentifier(VECTOR_T, capacity),
		.reserve = StdVectorIdentifier(VECTOR_T, reserve),
		.resize = StdVectorIdentifier(VECTOR_T, resize),
		.shrink_to_fit = StdVectorIdentifier(VECTOR_T, shrink_to_fit),
		.clear = StdVectorIdentifier(VECTOR_T, clear),
		.push_back = StdVectorIdentifier(VECTOR_T, push_back),
		.pop_back = StdVectorIdentifier(VECTOR_T, pop_back),
		.insert = StdVectorIdentifier(VECTOR_T, insert),
		.erase = StdVectorIdentifier(VECTOR_T, erase),
		.erase_n = StdVectorIdentifier(VECTOR_T, erase_n),
		.free = StdVectorIdentifier(VECTOR_T, free),
		.into_iter = StdVectorIdentifier(VECTOR_T, into_iter),
		.into_reverse_iter = StdVectorIdentifier(VECTOR_T, into_reverse_iter),
		.into_const_iter = StdVectorIdentifier(VECTOR_T, into_const_iter),
		.into_reverse_const_iter = StdVectorIdentifier(VECTOR_T, into_reverse_const_iter),
		.begin = StdVectorIdentifier(VECTOR_T, begin),
		.end = StdVectorIdentifier(VECTOR_T, end),
		.rbegin = StdVectorIdentifier(VECTOR_T, rbegin),
		.rend = StdVectorIdentifier(VECTOR_T, rend),
		.cbegin = StdVectorIdentifier(VECTOR_T, cbegin),
		.cend = StdVectorIdentifier(VECTOR_T, cend),
		.crbegin = StdVectorIdentifier(VECTOR_T, crbegin),
		.crend = StdVectorIdentifier(VECTOR_T, crend),
};

static const struct StdCollectionData(StdVector(VECTOR_T))
	StdVectorIdentifier(VECTOR_T, default_collection_data)
	= {.m_constructor = StdVectorIdentifier(VECTOR_T, default_constructor),
	   .m_copy_constructor = StdVectorIdentifier(VECTOR_T, default_copy_constructor),
	   .m_destructor = StdVectorIdentifier(VECTOR_T, default_destructor)};

[[always_inline]] static inline bool
StdVectorIdentifier(VECTOR_T, is_short)(const StdVector(VECTOR_T) * restrict self) {
	return self->m_capacity <= VECTOR_SMALL_OPT_CAPACITY;
}

StdVector(VECTOR_T) StdVectorIdentifier(VECTOR_T, new)(void) {
	return std_vector_new_with_allocator_and_collection_data(
		VECTOR_T,
		DEFAULT_ALLOCATOR,
		&StdVectorIdentifier(VECTOR_T, default_collection_data));
}

StdVector(VECTOR_T) StdVectorIdentifier(VECTOR_T, new_with_allocator)(StdAllocator allocator) {
	return std_vector_new_with_allocator_and_collection_data(
		VECTOR_T,
		allocator,
		&StdVectorIdentifier(VECTOR_T, default_collection_data));
}

StdVector(VECTOR_T) StdVectorIdentifier(VECTOR_T, new_with_collection_data)(
	const StdCollectionData(StdVector(VECTOR_T)) * restrict data) {

	return std_vector_new_with_allocator_and_collection_data(VECTOR_T, DEFAULT_ALLOCATOR, data);
}

StdVector(VECTOR_T) StdVectorIdentifier(VECTOR_T, new_with_allocator_and_collection_data)(
	StdAllocator allocator,
	const StdCollectionData(StdVector(VECTOR_T)) * restrict data) {
	let_mut vec = (StdVector(VECTOR_T)){.m_size = 0,
										.m_capacity = VECTOR_SMALL_OPT_CAPACITY,
										.m_allocator = allocator,
										.m_data = data,
										.m_vtable = &StdVectorIdentifier(VECTOR_T, vtable_impl)};
	std_assert(vec.m_data->m_constructor != nullptr, "Element default constructor cannot be null");
	std_assert(vec.m_data->m_destructor != nullptr, "Element destructor cannot be null");

	#if VECTOR_SMALL_OPT_CAPACITY == 0

	vec.m_long = static_cast(VECTOR_T*)(std_allocator_allocate_array_t(VECTOR_T,
																	   vec.m_data.m_allocator,
																	   VECTOR_DEFAULT_LONG_CAPACITY)
											.m_memory);
	vec.m_capacity = VECTOR_DEFAULT_LONG_CAPACITY;

	#else

	std_memset(VECTOR_T, vec.m_short, 0, VECTOR_SMALL_OPT_CAPACITY);

	#endif

	return vec;
}

StdVector(VECTOR_T) StdVectorIdentifier(VECTOR_T, new_with_capacity)(usize capacity) {
	return std_vector_new_with_capacity_and_allocator(VECTOR_T, capacity, DEFAULT_ALLOCATOR);
}

StdVector(VECTOR_T)
	StdVectorIdentifier(VECTOR_T, new_with_capacity_and_allocator)(usize capacity,
																   StdAllocator allocator) {
	let_mut vec = std_vector_new_with_allocator(VECTOR_T, allocator);
	std_vector_reserve(vec, capacity);
	return vec;
}

StdVector(VECTOR_T) StdVectorIdentifier(VECTOR_T, new_with_capacity_and_collection_data)(
	usize capacity,
	const StdCollectionData(StdVector(VECTOR_T)) * restrict data) {

	let_mut vec = std_vector_new_with_collection_data(VECTOR_T, data);
	std_vector_reserve(vec, capacity);
	return vec;
}

StdVector(VECTOR_T) StdVectorIdentifier(VECTOR_T, new_with_capacity_allocator_and_collection_data)(
	usize capacity,
	StdAllocator allocator,
	const StdCollectionData(StdVector(VECTOR_T)) * restrict data) {

	let_mut vec = std_vector_new_with_allocator_and_collection_data(VECTOR_T, allocator, data);
	std_vector_reserve(vec, capacity);
	return vec;
}

StdVector(VECTOR_T) StdVectorIdentifier(VECTOR_T, clone)(const StdVector(VECTOR_T) * restrict self)
	std_disable_if(!(self->m_data->m_copy_constructor),
				   "Can't clone a StdVector(VECTOR_T) with elements that aren't copyable (no "
				   "element copy constructor defined)") {
	std_assert(self->m_data->m_copy_constructor != nullptr,
			   "Can't clone StdVector(VECTOR_T) with elements that aren't copyable (no element "
			   "copy constructor defined)");

	let_mut vec
		= std_vector_new_with_capacity_allocator_and_collection_data(VECTOR_T,
																	 std_vector_capacity(*self),
																	 self->m_allocator,
																	 self->m_data);
	foreach_ref(elem, *self) {
		std_vector_push_back(vec, self->m_data->m_copy_constructor(elem, self->m_allocator));
	}
	return vec;
}

const VECTOR_T*
StdVectorIdentifier(VECTOR_T, at_const)(const StdVector(VECTOR_T) * restrict self, usize index) {
	std_assert(index <= self->m_size,
			   "std_vector_at called with index > size (index out of bounds");
	std_assert(index < self->m_capacity,
			   "std_vector_at called with index >= capacity (index out of bounds");

	return StdVectorIdentifier(VECTOR_T, is_short)(self) ? &(self->m_short[index]) :
														   &(self->m_long[index]);
}

VECTOR_T* StdVectorIdentifier(VECTOR_T, at_mut)(StdVector(VECTOR_T) * restrict self, usize index) {
	std_assert(index <= self->m_size,
			   "std_vector_at called with index > size (index out of bounds");
	std_assert(index < self->m_capacity,
			   "std_vector_at called with index >= capacity (index out of bounds");

	return StdVectorIdentifier(VECTOR_T, is_short)(self) ? &(self->m_short[index]) :
														   &(self->m_long[index]);
}

const VECTOR_T*
StdVectorIdentifier(VECTOR_T, front_const)(const StdVector(VECTOR_T) * restrict self) {
	return &std_vector_at(*self, 0);
}

VECTOR_T* StdVectorIdentifier(VECTOR_T, front_mut)(StdVector(VECTOR_T) * restrict self) {
	return &std_vector_at_mut(*self, 0);
}

const VECTOR_T*
StdVectorIdentifier(VECTOR_T, back_const)(const StdVector(VECTOR_T) * restrict self) {
	let size = std_vector_size(*self);
	let back_index = size > 0 ? size - 1 : 0;
	return &std_vector_at(*self, back_index);
}

VECTOR_T* StdVectorIdentifier(VECTOR_T, back_mut)(StdVector(VECTOR_T) * restrict self) {
	let size = std_vector_size(*self);
	let back_index = size > 0 ? size - 1 : 0;
	return &std_vector_at_mut(*self, back_index);
}

const VECTOR_T*
StdVectorIdentifier(VECTOR_T, data_const)(const StdVector(VECTOR_T) * restrict self) {
	return &std_vector_front(*self);
}

VECTOR_T* StdVectorIdentifier(VECTOR_T, data_mut)(StdVector(VECTOR_T) * restrict self) {
	return &std_vector_front_mut(*self);
}

bool StdVectorIdentifier(VECTOR_T, is_empty)(const StdVector(VECTOR_T) * restrict self) {
	return std_vector_size(*self) == 0;
}

bool StdVectorIdentifier(VECTOR_T, is_full)(const StdVector(VECTOR_T) * restrict self) {
	return self->m_size == self->m_capacity;
}

usize StdVectorIdentifier(VECTOR_T, size)(const StdVector(VECTOR_T) * restrict self) {
	return self->m_size;
}

usize StdVectorIdentifier(VECTOR_T, max_size)(void) {
	return (std_max_value(usize) - 1) << 1U;
}

usize StdVectorIdentifier(VECTOR_T, capacity)(const StdVector(VECTOR_T) * restrict self) {
	return self->m_capacity;
}

void StdVectorIdentifier(VECTOR_T, resize_internal)(StdVector(VECTOR_T) * restrict self,
													usize new_size) {
	let size = std_vector_size(*self);
	if(new_size < size) {
		let num_to_destroy = size - new_size;
		for(let_mut i = new_size; i < new_size + num_to_destroy; ++i) {
			self->m_data->m_destructor(&std_vector_at_mut(*self, i), self->m_allocator);
		}
	}
	if(new_size > VECTOR_SMALL_OPT_CAPACITY) {
		let_mut array = static_cast(VECTOR_T*)(
			std_allocator_allocate_array_t(VECTOR_T, self->m_allocator, new_size).m_memory);
		let num_to_copy = size < new_size ? size : new_size;
		std_memcpy(VECTOR_T, array, &std_vector_at_mut(*self, 0), num_to_copy);
		if(!StdVectorIdentifier(VECTOR_T, is_short)(self)) {
			let_mut ptr = self->m_long;
			self->m_long = nullptr;
			std_allocator_deallocate_array_t(VECTOR_T, self->m_allocator, ptr, self->m_capacity);
		}
		self->m_capacity = new_size;
		self->m_size = num_to_copy;
		self->m_long = array;
	}
	else if(self->m_capacity != VECTOR_SMALL_OPT_CAPACITY && VECTOR_SMALL_OPT_CAPACITY != 0) {
		let capacity = VECTOR_SMALL_OPT_CAPACITY;
		let_mut array = static_cast(VECTOR_T*)(
			std_allocator_allocate_array_t(VECTOR_T, self->m_allocator, capacity).m_memory);
		std_memcpy(VECTOR_T, array, self->m_long, capacity);
		std_allocator_deallocate_array_t(VECTOR_T,
										 self->m_allocator,
										 self->m_long,
										 self->m_capacity);
		std_memcpy(VECTOR_T, self->m_short, array, capacity);
		std_allocator_deallocate_array_t(VECTOR_T, self->m_allocator, array, capacity);
		self->m_size = capacity;
		self->m_capacity = capacity;
	}
}

[[always_inline]] static inline usize
StdVectorIdentifier(VECTOR_T, get_expanded_capacity)(usize old_capacity, usize num_increments) {
	return num_increments * ((old_capacity * 3) / 2);
}

void StdVectorIdentifier(VECTOR_T, reserve)(StdVector(VECTOR_T) * restrict self,
											usize new_capacity) {
	if(new_capacity > self->m_capacity) {
		let num_increments = 1 + (new_capacity / ((self->m_capacity * 3) / 2));
		let actual_new_capacity
			= StdVectorIdentifier(VECTOR_T, get_expanded_capacity)(self->m_capacity,
																   num_increments);
		StdVectorIdentifier(VECTOR_T, resize_internal)(self, actual_new_capacity);
	}
}

void StdVectorIdentifier(VECTOR_T, resize)(StdVector(VECTOR_T) * restrict self, usize new_size) {
	StdVectorIdentifier(VECTOR_T, resize_internal)(self, new_size);
	if(new_size > self->m_size) {
		for(let_mut i = self->m_size - 1; i < new_size; ++i) {
			std_vector_at_mut(*self, i) = self->m_data->m_constructor(self->m_allocator);
		}
	}
	self->m_size = new_size;
}

void StdVectorIdentifier(VECTOR_T, shrink_to_fit)(StdVector(VECTOR_T) * restrict self) {
	StdVectorIdentifier(VECTOR_T, resize_internal)(self, std_vector_size(*self));
}

void StdVectorIdentifier(VECTOR_T, clear)(StdVector(VECTOR_T) * restrict self) {
	for(let_mut i = 0U; i < std_vector_size(*self); ++i) {
		self->m_data->m_destructor(&std_vector_at_mut(*self, i), self->m_allocator);
	}
	self->m_size = 0U;
}

void StdVectorIdentifier(VECTOR_T, push_back)(
	StdVector(VECTOR_T) * restrict self,
	VECTOR_T element /** NOLINT(readability-non-const-parameter) **/) {
	if(self->m_size + 1 > self->m_capacity) {
		let new_capacity
			= StdVectorIdentifier(VECTOR_T, get_expanded_capacity)(self->m_capacity, 1);
		StdVectorIdentifier(VECTOR_T, resize_internal)(self, new_capacity);
	}

	std_vector_at_mut(*self, self->m_size) = element;
	self->m_size++;
}

StdOption(VECTOR_T) StdVectorIdentifier(VECTOR_T, pop_back)(StdVector(VECTOR_T) * restrict self) {
	if(self->m_size == 0) {
		return None(VECTOR_T);
	}

	let_mut ptr = &std_vector_at_mut(*self, self->m_size - 1);
	let elem = Some(VECTOR_T, *ptr);
	*ptr = (VECTOR_T){0};
	self->m_size--;

	return elem;
}

void StdVectorIdentifier(VECTOR_T,
						 insert)(StdVector(VECTOR_T) * restrict self,
								 VECTOR_T element /** NOLINT(readability-non-const-parameter **/,
								 usize index) {
	std_assert(index <= self->m_size,
			   "std_vector_insert called with index > size (index out of bounds)");

	if(self->m_size + 1 > self->m_capacity) {
		let new_capacity
			= StdVectorIdentifier(VECTOR_T, get_expanded_capacity)(self->m_capacity, 1);
		StdVectorIdentifier(VECTOR_T, resize_internal)(self, new_capacity);
	}

	if(index != self->m_size) {
		let num_to_move = self->m_size - index;
		std_memmove(VECTOR_T,
					&std_vector_at_mut(*self, index + 1),
					&std_vector_at_mut(*self, index),
					num_to_move);
	}
	std_vector_at_mut(*self, index) = element;
	self->m_size++;
}

void StdVectorIdentifier(VECTOR_T, erase)(StdVector(VECTOR_T) * restrict self, usize index) {
	std_assert(index < self->m_size,
			   "std_vector_erase called with index >= self->m_size (index out of bounds)");

	self->m_data->m_destructor(&std_vector_at_mut(*self, index), self->m_allocator);

	if(index != self->m_size - 1) {
		let num_to_move = self->m_size - (index + 1);
		std_memmove(VECTOR_T,
					&std_vector_at_mut(*self, index),
					&std_vector_at_mut(*self, index + 1),
					num_to_move);
	}
	self->m_size--;
}

void StdVectorIdentifier(VECTOR_T, erase_n)(StdVector(VECTOR_T) * restrict self,
											usize index,
											usize num_elements) {
	std_assert(index < self->m_size,
			   "std_vector_erase_n called with index >= size (index out of bounds)");
	std_assert(index + num_elements < self->m_size,
			   "std_vector_erase_n called with index + size >= size (range "
			   "out of bounds)");

	let end = index + num_elements;
	let num_to_move = self->m_size - end;

	for(let_mut i = index; i < end; ++i) {
		self->m_data->m_destructor(&std_vector_at_mut(*self, i), self->m_allocator);
	}

	if(end != self->m_size) {
		std_memmove(VECTOR_T,
					&std_vector_at_mut(*self, index),
					&std_vector_at_mut(*self, end),
					num_to_move);
	}
	self->m_size -= num_elements;
}

void StdVectorIdentifier(VECTOR_T, free)(void* restrict self) {
	let self_ = static_cast(StdVector(VECTOR_T)*)(self);
	for(let_mut i = 0U; i < self_->m_size; ++i) {
		self_->m_data->m_destructor(&std_vector_at_mut(*self_, i), self_->m_allocator);
	}

	if(!StdVectorIdentifier(VECTOR_T, is_short)(self_)) {
		std_allocator_deallocate_array_t(VECTOR_T,
										 self_->m_allocator,
										 self_->m_long,
										 self_->m_capacity);
		self_->m_capacity = VECTOR_SMALL_OPT_CAPACITY;
	}
	self_->m_size = 0U;
}

StdVectorIterator(VECTOR_T)
	StdVectorIdentifier(VECTOR_T, iterator_new)(const StdVector(VECTOR_T) * restrict self) {
	return (StdVectorIterator(VECTOR_T)){.m_index = 0U,
										 .m_vector = const_cast(StdVector(VECTOR_T)*)(self)};
}

StdVectorConstIterator(VECTOR_T)
	StdVectorIdentifier(VECTOR_T, const_iterator_new)(const StdVector(VECTOR_T) * restrict self) {
	return (StdVectorConstIterator(VECTOR_T)){.m_index = 0U,
											  .m_vector = const_cast(StdVector(VECTOR_T)*)(self)};
}

Ref(VECTOR_T)
	StdVectorIdentifier(VECTOR_T,
						iterator_next)(StdRandomAccessIterator(Ref(VECTOR_T)) * restrict self) {
	let _self = static_cast(StdVectorIterator(VECTOR_T)*)(self->m_self);

	std_assert(_self->m_index > -1,
			   "Iterator value accessed when iterator is positioned before the "
			   "beginning of the "
			   "iteration (iterator out of bounds)");
	std_assert(static_cast(usize)(_self->m_index) < _self->m_vector->m_size,
			   "Iterator value accessed when iterator is positioned after the end of the "
			   "iteration (iterator out of bounds)");
	std_assert(static_cast(usize)(_self->m_index) < _self->m_vector->m_capacity,
			   "Iterator value accessed when iterator is positioned after the end of the "
			   "iteration (iterator out of bounds)");
	_self->m_index++;
	if(static_cast(usize)(_self->m_index) >= _self->m_vector->m_size) {
		return &std_vector_at_mut(*(_self->m_vector), _self->m_vector->m_size - 1);
	}

	return &std_vector_at_mut(*(_self->m_vector), static_cast(usize)(_self->m_index));
}

Ref(VECTOR_T)
	StdVectorIdentifier(VECTOR_T,
						iterator_previous)(StdRandomAccessIterator(Ref(VECTOR_T)) * restrict self) {
	let _self = static_cast(StdVectorIterator(VECTOR_T)*)(self->m_self);

	std_assert(_self->m_index > -1,
			   "Iterator value accessed when iterator is positioned before the "
			   "beginning of the "
			   "iteration (iterator out of bounds)");
	std_assert(static_cast(usize)(_self->m_index) < _self->m_vector->m_size,
			   "Iterator value accessed when iterator is positioned after the end of the "
			   "iteration (iterator out of bounds)");
	_self->m_index--;
	if(_self->m_index < 0) {
		return &std_vector_at_mut(*(_self->m_vector), 0);
	}

	return &std_vector_at_mut(*(_self->m_vector), static_cast(usize)(_self->m_index));
}

Ref(VECTOR_T)
	StdVectorIdentifier(VECTOR_T,
						iterator_at)(const StdRandomAccessIterator(Ref(VECTOR_T)) * restrict self,
									 usize index) {
	let _self = static_cast(StdVectorIterator(VECTOR_T)*)(self->m_self);

	std_assert(index < _self->m_vector->m_size,
			   "std_vector_iterator_at called with index pst the end of the iteration "
			   "(iterator index out of bounds)");
	return &std_vector_at_mut(*(_self->m_vector), index);
}

Ref(VECTOR_T)
	StdVectorIdentifier(VECTOR_T,
						iterator_rat)(const StdRandomAccessIterator(Ref(VECTOR_T)) * restrict self,
									  usize index) {
	let _self = static_cast(StdVectorIterator(VECTOR_T)*)(self->m_self);

	std_assert(index < _self->m_vector->m_size,
			   "std_vector_iterator_at called with index pst the end of the iteration "
			   "(iterator index out of bounds)");
	return &std_vector_at_mut(*(_self->m_vector), (_self->m_vector->m_size - 1) - index);
}

Ref(VECTOR_T) StdVectorIdentifier(VECTOR_T, iterator_current)(
	const StdRandomAccessIterator(Ref(VECTOR_T)) * restrict self) {
	let _self = static_cast(const StdVectorIterator(VECTOR_T)*)(self->m_self);

	std_assert(_self->m_index > -1,
			   "Iterator value accessed when iterator is positioned before the "
			   "beginning of the "
			   "iteration (iterator out of bounds)");
	std_assert(static_cast(usize)(_self->m_index) < _self->m_vector->m_size,
			   "Iterator value accessed when iterator is positioned after the end of the "
			   "iteration (iterator out of bounds)");
	std_assert(static_cast(usize)(_self->m_index) < _self->m_vector->m_capacity,
			   "Iterator value accessed when iterator is positioned after the end of the "
			   "iteration (iterator out of bounds)");
	return &std_vector_at_mut(*(_self->m_vector), static_cast(usize)(_self->m_index));
}

bool StdVectorIdentifier(VECTOR_T, iterator_equals)(
	const StdRandomAccessIterator(Ref(VECTOR_T)) * restrict self,
	const StdRandomAccessIterator(Ref(VECTOR_T)) * restrict rhs) {
	let _self = static_cast(const StdVectorIterator(VECTOR_T)*)(self->m_self);
	let _rhs = static_cast(const StdVectorIterator(VECTOR_T)*)(rhs->m_self);

	return _self->m_index == _rhs->m_index && _self->m_vector == _rhs->m_vector;
}

ConstRef(VECTOR_T) StdVectorIdentifier(VECTOR_T, iterator_cnext)(
	StdRandomAccessIterator(ConstRef(VECTOR_T)) * restrict self) {
	let _self = static_cast(StdVectorConstIterator(VECTOR_T)*)(self->m_self);

	std_assert(_self->m_index > -1,
			   "Iterator value accessed when iterator is positioned before the "
			   "beginning of the "
			   "iteration (iterator out of bounds)");
	std_assert(static_cast(usize)(_self->m_index) < _self->m_vector->m_size,
			   "Iterator value accessed when iterator is positioned after the end of the "
			   "iteration (iterator out of bounds)");
	std_assert(static_cast(usize)(_self->m_index) < _self->m_vector->m_capacity,
			   "Iterator value accessed when iterator is positioned after the end of the "
			   "iteration (iterator out of bounds)");
	_self->m_index++;
	if(static_cast(usize)(_self->m_index) >= _self->m_vector->m_size) {
		return &std_vector_at(*(_self->m_vector), _self->m_vector->m_size - 1);
	}

	return &std_vector_at(*(_self->m_vector), static_cast(usize)(_self->m_index));
}

ConstRef(VECTOR_T) StdVectorIdentifier(VECTOR_T, iterator_cprevious)(
	StdRandomAccessIterator(ConstRef(VECTOR_T)) * restrict self) {
	let _self = static_cast(StdVectorConstIterator(VECTOR_T)*)(self->m_self);

	std_assert(_self->m_index > -1,
			   "Iterator value accessed when iterator is positioned before the "
			   "beginning of the "
			   "iteration (iterator out of bounds)");
	std_assert(static_cast(usize)(_self->m_index) < _self->m_vector->m_size,
			   "Iterator value accessed when iterator is positioned after the end of the "
			   "iteration (iterator out of bounds)");
	_self->m_index--;
	if(_self->m_index < 0) {
		return &std_vector_at(*(_self->m_vector), 0);
	}

	return &std_vector_at(*(_self->m_vector), static_cast(usize)(_self->m_index));
}

ConstRef(VECTOR_T) StdVectorIdentifier(VECTOR_T, iterator_cat)(
	const StdRandomAccessIterator(ConstRef(VECTOR_T)) * restrict self,
	usize index) {
	let _self = static_cast(StdVectorConstIterator(VECTOR_T)*)(self->m_self);

	std_assert(index < _self->m_vector->m_size,
			   "std_vector_iterator_at called with index pst the end of the iteration "
			   "(iterator index out of bounds)");
	return &std_vector_at(*(_self->m_vector), index);
}

ConstRef(VECTOR_T) StdVectorIdentifier(VECTOR_T, iterator_crat)(
	const StdRandomAccessIterator(ConstRef(VECTOR_T)) * restrict self,
	usize index) {
	let _self = static_cast(StdVectorConstIterator(VECTOR_T)*)(self->m_self);

	std_assert(index < _self->m_vector->m_size,
			   "std_vector_iterator_at called with index pst the end of the iteration "
			   "(iterator index out of bounds)");
	return &std_vector_at(*(_self->m_vector), (_self->m_vector->m_size - 1) - index);
}

ConstRef(VECTOR_T) StdVectorIdentifier(VECTOR_T, iterator_ccurrent)(
	const StdRandomAccessIterator(ConstRef(VECTOR_T)) * restrict self) {
	let _self = static_cast(const StdVectorConstIterator(VECTOR_T)*)(self->m_self);

	std_assert(_self->m_index > -1,
			   "Iterator value accessed when iterator is positioned before the "
			   "beginning of the "
			   "iteration (iterator out of bounds)");
	std_assert(static_cast(usize)(_self->m_index) < _self->m_vector->m_size,
			   "Iterator value accessed when iterator is positioned after the end of the "
			   "iteration (iterator out of bounds)");
	std_assert(static_cast(usize)(_self->m_index) < _self->m_vector->m_capacity,
			   "Iterator value accessed when iterator is positioned after the end of the "
			   "iteration (iterator out of bounds)");
	return &std_vector_at(*(_self->m_vector), static_cast(usize)(_self->m_index));
}

bool StdVectorIdentifier(VECTOR_T, iterator_cequals)(
	const StdRandomAccessIterator(ConstRef(VECTOR_T)) * restrict self,
	const StdRandomAccessIterator(ConstRef(VECTOR_T)) * restrict rhs) {
	let _self = static_cast(const StdVectorConstIterator(VECTOR_T)*)(self->m_self);
	let _rhs = static_cast(const StdVectorConstIterator(VECTOR_T)*)(rhs->m_self);

	return _self->m_index == _rhs->m_index && _self->m_vector == _rhs->m_vector;
}

StdRandomAccessIterator(Ref(VECTOR_T))
	StdVectorIdentifier(VECTOR_T, begin)(StdVector(VECTOR_T) * restrict self) {
	let_mut iter = std_vector_into_iter(*self);
	let_mut inner = static_cast(StdVectorIterator(VECTOR_T)*)(iter.m_self);
	inner->m_index = 0;
	return iter;
}

StdRandomAccessIterator(Ref(VECTOR_T))
	StdVectorIdentifier(VECTOR_T, end)(StdVector(VECTOR_T) * restrict self) {
	let_mut iter = std_vector_into_iter(*self);
	let_mut inner = static_cast(StdVectorIterator(VECTOR_T)*)(iter.m_self);
	inner->m_index = static_cast(isize)(self->m_size);
	return iter;
}

StdRandomAccessIterator(Ref(VECTOR_T))
	StdVectorIdentifier(VECTOR_T, rbegin)(StdVector(VECTOR_T) * restrict self) {
	let_mut iter = std_vector_into_reverse_iter(*self);
	let_mut inner = static_cast(StdVectorIterator(VECTOR_T)*)(iter.m_self);
	inner->m_index = static_cast(isize)(self->m_size - 1);
	return iter;
}

StdRandomAccessIterator(Ref(VECTOR_T))
	StdVectorIdentifier(VECTOR_T, rend)(StdVector(VECTOR_T) * restrict self) {
	let_mut iter = std_vector_into_reverse_iter(*self);
	let_mut inner = static_cast(StdVectorIterator(VECTOR_T)*)(iter.m_self);
	inner->m_index = -1;
	return iter;
}

StdRandomAccessIterator(ConstRef(VECTOR_T))
	StdVectorIdentifier(VECTOR_T, cbegin)(const StdVector(VECTOR_T) * restrict self) {
	let_mut iter = std_vector_into_const_iter(*self);
	let_mut inner = static_cast(StdVectorConstIterator(VECTOR_T)*)(iter.m_self);
	inner->m_index = 0;
	return iter;
}

StdRandomAccessIterator(ConstRef(VECTOR_T))
	StdVectorIdentifier(VECTOR_T, cend)(const StdVector(VECTOR_T) * restrict self) {
	let_mut iter = std_vector_into_const_iter(*self);
	let_mut inner = static_cast(StdVectorConstIterator(VECTOR_T)*)(iter.m_self);
	inner->m_index = static_cast(isize)(self->m_size);
	return iter;
}

StdRandomAccessIterator(ConstRef(VECTOR_T))
	StdVectorIdentifier(VECTOR_T, crbegin)(const StdVector(VECTOR_T) * restrict self) {
	let_mut iter = std_vector_into_reverse_const_iter(*self);
	let_mut inner = static_cast(StdVectorConstIterator(VECTOR_T)*)(iter.m_self);
	inner->m_index = static_cast(isize)(self->m_size - 1);
	return iter;
}

StdRandomAccessIterator(ConstRef(VECTOR_T))
	StdVectorIdentifier(VECTOR_T, crend)(const StdVector(VECTOR_T) * restrict self) {
	let_mut iter = std_vector_into_reverse_const_iter(*self);
	let_mut inner = static_cast(StdVectorConstIterator(VECTOR_T)*)(iter.m_self);
	inner->m_index = -1;
	return iter;
}

StdString StdVectorIdentifier(VECTOR_T, format)(const StdFormat* restrict self,
												StdFormatSpecifier specifier) {
	return StdVectorIdentifier(VECTOR_T, format_with_allocator)(self, specifier, DEFAULT_ALLOCATOR);
}

StdString
StdVectorIdentifier(VECTOR_T, format_with_allocator)(const StdFormat* restrict self,
													 [[maybe_unused]] StdFormatSpecifier specifier,
													 StdAllocator allocator) {
	std_assert(specifier.m_type == STD_FORMAT_TYPE_DEFAULT
				   || specifier.m_type == STD_FORMAT_TYPE_DEBUG,
			   "Can only format StdVector with default or debug format specifier");

	let _self = *static_cast(const StdVector(VECTOR_T)*)(self->m_self);
	let size = std_vector_size(_self);
	let capacity = std_vector_capacity(_self);
	let data = std_vector_data(_self);
	return std_format_with_allocator(
		AS_STRING(StdVector(VECTOR_T)) ": [size: {d}, capacity: {d}, data pointer: {x}]",
		allocator,
		size,
		capacity,
		as_format_t(nullptr_t, data));
}

	#undef STD_TEMPLATE_SUPPRESS_INSTANTIATIONS
#endif // defined(VECTOR_T) && defined(VECTOR_SMALL_OPT_CAPACITY) && VECTOR_IMPL
