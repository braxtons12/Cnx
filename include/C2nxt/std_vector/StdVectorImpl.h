/// @file StdVectorImpl.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief This module provides the function definitions for a template instantiation of
/// `StdVector(T)`
/// @version 0.2.1
/// @date 2022-02-24
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

#if defined(T) && defined(SMALL_OPT_CAPACITY) && STD_TEMPLATE_IMPL

	#define STD_TEMPLATE_SUPPRESS_INSTANTIATIONS TRUE

	#define min_value(x, y) ((x) < (y) ? (x) : (y))

	#include <C2nxt/StdAllocators.h>
	#include <C2nxt/StdBasicTypes.h>
	#include <C2nxt/StdCollectionData.h>
	#include <C2nxt/StdIterator.h>
	#include <C2nxt/StdPlatform.h>
	#include <C2nxt/std_vector/StdVectorDef.h>

StdVectorIterator(T) StdVectorIdentifier(T, iterator_new)(const StdVector(T) * restrict self);
StdVectorConstIterator(T)
	StdVectorIdentifier(T, const_iterator_new)(const StdVector(T) * restrict self);

Ref(T) StdVectorIdentifier(T, iterator_next)(StdRandomAccessIterator(Ref(T)) * restrict self);
Ref(T) StdVectorIdentifier(T, iterator_previous)(StdRandomAccessIterator(Ref(T)) * restrict self);
Ref(T) StdVectorIdentifier(T, iterator_at)(const StdRandomAccessIterator(Ref(T)) * restrict self,
										   usize index);
Ref(T) StdVectorIdentifier(T, iterator_rat)(const StdRandomAccessIterator(Ref(T)) * restrict self,
											usize index);
Ref(T)
	StdVectorIdentifier(T, iterator_current)(const StdRandomAccessIterator(Ref(T)) * restrict self);
bool StdVectorIdentifier(T, iterator_equals)(const StdRandomAccessIterator(Ref(T)) * restrict self,
											 const StdRandomAccessIterator(Ref(T)) * restrict rhs);

ConstRef(T)
	StdVectorIdentifier(T, iterator_cnext)(StdRandomAccessIterator(ConstRef(T)) * restrict self);
ConstRef(T)
	StdVectorIdentifier(T,
						iterator_cprevious)(StdRandomAccessIterator(ConstRef(T)) * restrict self);
ConstRef(T)
	StdVectorIdentifier(T, iterator_cat)(const StdRandomAccessIterator(ConstRef(T)) * restrict self,
										 usize index);
ConstRef(T)
	StdVectorIdentifier(T,
						iterator_crat)(const StdRandomAccessIterator(ConstRef(T)) * restrict self,
									   usize index);
ConstRef(T) StdVectorIdentifier(T, iterator_ccurrent)(
	const StdRandomAccessIterator(ConstRef(T)) * restrict self);
bool StdVectorIdentifier(T, iterator_cequals)(
	const StdRandomAccessIterator(ConstRef(T)) * restrict self,
	const StdRandomAccessIterator(ConstRef(T)) * restrict rhs);

ImplIntoStdRandomAccessIterator(StdVector(T),
								Ref(T),
								StdVectorIdentifier(T, into_iter),
								into,
								StdVectorIdentifier(T, iterator_new),
								StdVectorIdentifier(T, iterator_next),
								StdVectorIdentifier(T, iterator_previous),
								StdVectorIdentifier(T, iterator_at),
								StdVectorIdentifier(T, iterator_current),
								StdVectorIdentifier(T, iterator_equals));
ImplIntoStdRandomAccessIterator(StdVector(T),
								Ref(T),
								StdVectorIdentifier(T, into_reverse_iter),
								into_reverse,
								StdVectorIdentifier(T, iterator_new),
								StdVectorIdentifier(T, iterator_next),
								StdVectorIdentifier(T, iterator_previous),
								StdVectorIdentifier(T, iterator_rat),
								StdVectorIdentifier(T, iterator_current),
								StdVectorIdentifier(T, iterator_equals));

ImplIntoStdRandomAccessIterator(StdVector(T),
								ConstRef(T),
								StdVectorIdentifier(T, into_const_iter),
								into,
								StdVectorIdentifier(T, const_iterator_new),
								StdVectorIdentifier(T, iterator_cnext),
								StdVectorIdentifier(T, iterator_cprevious),
								StdVectorIdentifier(T, iterator_cat),
								StdVectorIdentifier(T, iterator_ccurrent),
								StdVectorIdentifier(T, iterator_cequals));
ImplIntoStdRandomAccessIterator(StdVector(T),
								ConstRef(T),
								StdVectorIdentifier(T, into_reverse_const_iter),
								into_reverse,
								StdVectorIdentifier(T, const_iterator_new),
								StdVectorIdentifier(T, iterator_cnext),
								StdVectorIdentifier(T, iterator_cprevious),
								StdVectorIdentifier(T, iterator_crat),
								StdVectorIdentifier(T, iterator_ccurrent),
								StdVectorIdentifier(T, iterator_cequals));

always_inline static inline T StdVectorIdentifier(T, default_constructor)(StdAllocator allocator) {
	ignore(allocator);
	return (T){0};
}

always_inline static inline T
StdVectorIdentifier(T, default_copy_constructor)(const T* restrict elem, StdAllocator allocator) {
	ignore(allocator);
	return *elem;
}

always_inline static inline void StdVectorIdentifier(T, default_destructor)(
	T* restrict element, /** NOLINT(readability-non-const-parameter)**/
	StdAllocator allocator) {
	ignore(allocator, element);
}

static const struct StdVectorIdentifier(T, vtable) StdVectorIdentifier(T, vtable_impl) = {
	.clone = StdVectorIdentifier(T, clone),
	.at_const = StdVectorIdentifier(T, at_const),
	.at_mut = StdVectorIdentifier(T, at_mut),
	.front_const = StdVectorIdentifier(T, front_const),
	.front_mut = StdVectorIdentifier(T, front_mut),
	.back_const = StdVectorIdentifier(T, back_const),
	.back_mut = StdVectorIdentifier(T, back_mut),
	.data_const = StdVectorIdentifier(T, data_const),
	.data_mut = StdVectorIdentifier(T, data_mut),
	.is_empty = StdVectorIdentifier(T, is_empty),
	.is_full = StdVectorIdentifier(T, is_full),
	.size = StdVectorIdentifier(T, size),
	.capacity = StdVectorIdentifier(T, capacity),
	.reserve = StdVectorIdentifier(T, reserve),
	.resize = StdVectorIdentifier(T, resize),
	.shrink_to_fit = StdVectorIdentifier(T, shrink_to_fit),
	.clear = StdVectorIdentifier(T, clear),
	.push_back = StdVectorIdentifier(T, push_back),
	.pop_back = StdVectorIdentifier(T, pop_back),
	.insert = StdVectorIdentifier(T, insert),
	.erase = StdVectorIdentifier(T, erase),
	.erase_n = StdVectorIdentifier(T, erase_n),
	.free = StdVectorIdentifier(T, free),
	.into_iter = StdVectorIdentifier(T, into_iter),
	.into_reverse_iter = StdVectorIdentifier(T, into_reverse_iter),
	.into_const_iter = StdVectorIdentifier(T, into_const_iter),
	.into_reverse_const_iter = StdVectorIdentifier(T, into_reverse_const_iter),
	.begin = StdVectorIdentifier(T, begin),
	.end = StdVectorIdentifier(T, end),
	.rbegin = StdVectorIdentifier(T, rbegin),
	.rend = StdVectorIdentifier(T, rend),
	.cbegin = StdVectorIdentifier(T, cbegin),
	.cend = StdVectorIdentifier(T, cend),
	.crbegin = StdVectorIdentifier(T, crbegin),
	.crend = StdVectorIdentifier(T, crend),
};

static const struct StdCollectionData(StdVector(T)) StdVectorIdentifier(T, default_collection_data)
	= {.m_constructor = StdVectorIdentifier(T, default_constructor),
	   .m_copy_constructor = StdVectorIdentifier(T, default_copy_constructor),
	   .m_destructor = StdVectorIdentifier(T, default_destructor)};

always_inline static inline bool
StdVectorIdentifier(T, is_short)(const StdVector(T) * restrict self) {
	return self->m_capacity <= SMALL_OPT_CAPACITY;
}

StdVector(T) StdVectorIdentifier(T, new)(void) {
	return std_vector_new_with_allocator_and_collection_data(
		T,
		DEFAULT_ALLOCATOR,
		&StdVectorIdentifier(T, default_collection_data));
}

StdVector(T) StdVectorIdentifier(T, new_with_allocator)(StdAllocator allocator) {
	return std_vector_new_with_allocator_and_collection_data(
		T,
		allocator,
		&StdVectorIdentifier(T, default_collection_data));
}

StdVector(T) StdVectorIdentifier(T, new_with_collection_data)(
	const StdCollectionData(StdVector(T)) * restrict data) {

	return std_vector_new_with_allocator_and_collection_data(T, DEFAULT_ALLOCATOR, data);
}

StdVector(T) StdVectorIdentifier(T, new_with_allocator_and_collection_data)(
	StdAllocator allocator,
	const StdCollectionData(StdVector(T)) * restrict data) {
	let_mut vec = (StdVector(T)){.m_size = 0,
								 .m_capacity = SMALL_OPT_CAPACITY,
								 .m_allocator = allocator,
								 .m_data = data,
								 .m_vtable = &StdVectorIdentifier(T, vtable_impl)};
	std_assert(vec.m_data->m_constructor != nullptr, "Element default constructor cannot be null");
	std_assert(vec.m_data->m_destructor != nullptr, "Element destructor cannot be null");

	#if SMALL_OPT_CAPACITY == 0

	vec.m_long = static_cast(T*)(
		std_allocator_allocate_array_t(T, vec.m_data.m_allocator, DEFAULT_LONG_CAPACITY).m_memory);
	vec.m_capacity = DEFAULT_LONG_CAPACITY;

	#else

	std_memset(T, vec.m_short, 0, SMALL_OPT_CAPACITY);

	#endif

	return vec;
}

StdVector(T) StdVectorIdentifier(T, new_with_capacity)(usize capacity) {
	return std_vector_new_with_capacity_and_allocator(T, capacity, DEFAULT_ALLOCATOR);
}

StdVector(T) StdVectorIdentifier(T, new_with_capacity_and_allocator)(usize capacity,
																	 StdAllocator allocator) {
	let_mut vec = std_vector_new_with_allocator(T, allocator);
	std_vector_reserve(vec, capacity);
	return vec;
}

StdVector(T) StdVectorIdentifier(T, new_with_capacity_and_collection_data)(
	usize capacity,
	const StdCollectionData(StdVector(T)) * restrict data) {

	let_mut vec = std_vector_new_with_collection_data(T, data);
	std_vector_reserve(vec, capacity);
	return vec;
}

StdVector(T) StdVectorIdentifier(T, new_with_capacity_allocator_and_collection_data)(
	usize capacity,
	StdAllocator allocator,
	const StdCollectionData(StdVector(T)) * restrict data) {

	let_mut vec = std_vector_new_with_allocator_and_collection_data(T, allocator, data);
	std_vector_reserve(vec, capacity);
	return vec;
}

StdVector(T) StdVectorIdentifier(T, clone)(const StdVector(T) * restrict self)
	std_disable_if(!(self->m_data->m_copy_constructor),
				   "Can't clone a StdVector(T) with elements that aren't copyable (no "
				   "element copy constructor defined)") {
	std_assert(self->m_data->m_copy_constructor != nullptr,
			   "Can't clone StdVector(T) with elements that aren't copyable (no element "
			   "copy constructor defined)");

	let_mut vec
		= std_vector_new_with_capacity_allocator_and_collection_data(T,
																	 std_vector_capacity(*self),
																	 self->m_allocator,
																	 self->m_data);
	foreach_ref(elem, *self) {
		std_vector_push_back(vec, self->m_data->m_copy_constructor(elem, self->m_allocator));
	}
	return vec;
}

const T* StdVectorIdentifier(T, at_const)(const StdVector(T) * restrict self, usize index) {
	std_assert(index <= self->m_size,
			   "std_vector_at called with index > size (index out of bounds");
	std_assert(index < self->m_capacity,
			   "std_vector_at called with index >= capacity (index out of bounds");

	return StdVectorIdentifier(T, is_short)(self) ? &(self->m_short[index]) :
													&(self->m_long[index]);
}

T* StdVectorIdentifier(T, at_mut)(StdVector(T) * restrict self, usize index) {
	std_assert(index <= self->m_size,
			   "std_vector_at called with index > size (index out of bounds");
	std_assert(index < self->m_capacity,
			   "std_vector_at called with index >= capacity (index out of bounds");

	return StdVectorIdentifier(T, is_short)(self) ? &(self->m_short[index]) :
													&(self->m_long[index]);
}

const T* StdVectorIdentifier(T, front_const)(const StdVector(T) * restrict self) {
	return &std_vector_at(*self, 0);
}

T* StdVectorIdentifier(T, front_mut)(StdVector(T) * restrict self) {
	return &std_vector_at_mut(*self, 0);
}

const T* StdVectorIdentifier(T, back_const)(const StdVector(T) * restrict self) {
	let size = std_vector_size(*self);
	let back_index = size > 0 ? size - 1 : 0;
	return &std_vector_at(*self, back_index);
}

T* StdVectorIdentifier(T, back_mut)(StdVector(T) * restrict self) {
	let size = std_vector_size(*self);
	let back_index = size > 0 ? size - 1 : 0;
	return &std_vector_at_mut(*self, back_index);
}

const T* StdVectorIdentifier(T, data_const)(const StdVector(T) * restrict self) {
	return &std_vector_front(*self);
}

T* StdVectorIdentifier(T, data_mut)(StdVector(T) * restrict self) {
	return &std_vector_front_mut(*self);
}

bool StdVectorIdentifier(T, is_empty)(const StdVector(T) * restrict self) {
	return std_vector_size(*self) == 0;
}

bool StdVectorIdentifier(T, is_full)(const StdVector(T) * restrict self) {
	return self->m_size == self->m_capacity;
}

usize StdVectorIdentifier(T, size)(const StdVector(T) * restrict self) {
	return self->m_size;
}

usize StdVectorIdentifier(T, max_size)(void) {
	return (std_max_value(usize) - 1) << 1U;
}

usize StdVectorIdentifier(T, capacity)(const StdVector(T) * restrict self) {
	return self->m_capacity;
}

void StdVectorIdentifier(T, resize_internal)(StdVector(T) * restrict self, usize new_size) {
	let size = std_vector_size(*self);
	if(new_size < size) {
		let num_to_destroy = size - new_size;
		for(let_mut i = new_size; i < new_size + num_to_destroy; ++i) {
			self->m_data->m_destructor(&std_vector_at_mut(*self, i), self->m_allocator);
		}
	}
	if(new_size > SMALL_OPT_CAPACITY) {
		let_mut array = static_cast(T*)(
			std_allocator_allocate_array_t(T, self->m_allocator, new_size).m_memory);
		let num_to_copy = min_value(size, new_size);
		std_memcpy(T, array, &std_vector_at_mut(*self, 0), num_to_copy);
		if(!StdVectorIdentifier(T, is_short)(self)) {
			let_mut ptr = self->m_long;
			self->m_long = nullptr;
			std_allocator_deallocate_array_t(T, self->m_allocator, ptr, self->m_capacity);
		}
		self->m_capacity = new_size;
		self->m_size = num_to_copy;
		self->m_long = array;
	}
	else if(self->m_capacity != SMALL_OPT_CAPACITY && SMALL_OPT_CAPACITY != 0) {
		let capacity = SMALL_OPT_CAPACITY;
		let_mut array = static_cast(T*)(
			std_allocator_allocate_array_t(T, self->m_allocator, capacity).m_memory);
		std_memcpy(T, array, self->m_long, capacity);
		std_allocator_deallocate_array_t(T, self->m_allocator, self->m_long, self->m_capacity);
		std_memcpy(T, self->m_short, array, capacity);
		std_allocator_deallocate_array_t(T, self->m_allocator, array, capacity);
		self->m_size = capacity;
		self->m_capacity = capacity;
	}
}

static inline always_inline usize StdVectorIdentifier(T,
													  get_expanded_capacity)(usize old_capacity,
																			 usize num_increments) {
	return num_increments * ((old_capacity * 3) / 2);
}

void StdVectorIdentifier(T, reserve)(StdVector(T) * restrict self, usize new_capacity) {
	if(new_capacity > self->m_capacity) {
		let num_increments = 1 + (new_capacity / ((self->m_capacity * 3) / 2));
		let actual_new_capacity
			= StdVectorIdentifier(T, get_expanded_capacity)(self->m_capacity, num_increments);
		StdVectorIdentifier(T, resize_internal)(self, actual_new_capacity);
	}
}

void StdVectorIdentifier(T, resize)(StdVector(T) * restrict self, usize new_size) {
	StdVectorIdentifier(T, resize_internal)(self, new_size);
	if(new_size > self->m_size) {
		for(let_mut i = self->m_size - 1; i < new_size; ++i) {
			std_vector_at_mut(*self, i) = self->m_data->m_constructor(self->m_allocator);
		}
	}
	self->m_size = new_size;
}

void StdVectorIdentifier(T, shrink_to_fit)(StdVector(T) * restrict self) {
	StdVectorIdentifier(T, resize_internal)(self, std_vector_size(*self));
}

void StdVectorIdentifier(T, clear)(StdVector(T) * restrict self) {
	for(let_mut i = 0U; i < std_vector_size(*self); ++i) {
		self->m_data->m_destructor(&std_vector_at_mut(*self, i), self->m_allocator);
	}
	self->m_size = 0U;
}

void StdVectorIdentifier(T, push_back)(StdVector(T) * restrict self,
									   T element /** NOLINT(readability-non-const-parameter) **/) {
	if(self->m_size + 1 > self->m_capacity) {
		let new_capacity = StdVectorIdentifier(T, get_expanded_capacity)(self->m_capacity, 1);
		StdVectorIdentifier(T, resize_internal)(self, new_capacity);
	}

	std_vector_at_mut(*self, self->m_size) = element;
	self->m_size++;
}

StdOption(T) StdVectorIdentifier(T, pop_back)(StdVector(T) * restrict self) {
	if(self->m_size == 0) {
		return None(T);
	}

	let_mut ptr = &std_vector_at_mut(*self, self->m_size - 1);
	let elem = Some(T, *ptr);
	*ptr = (T){0};
	self->m_size--;

	return elem;
}

void StdVectorIdentifier(T, insert)(StdVector(T) * restrict self,
									T element /** NOLINT(readability-non-const-parameter **/,
									usize index) {
	std_assert(index <= self->m_size,
			   "std_vector_insert called with index > size (index out of bounds)");

	if(self->m_size + 1 > self->m_capacity) {
		let new_capacity = StdVectorIdentifier(T, get_expanded_capacity)(self->m_capacity, 1);
		StdVectorIdentifier(T, resize_internal)(self, new_capacity);
	}

	if(index != self->m_size) {
		let num_to_move = self->m_size - index;
		std_memmove(T,
					&std_vector_at_mut(*self, index + 1),
					&std_vector_at_mut(*self, index),
					num_to_move);
	}
	std_vector_at_mut(*self, index) = element;
	self->m_size++;
}

void StdVectorIdentifier(T, erase)(StdVector(T) * restrict self, usize index) {
	std_assert(index < self->m_size,
			   "std_vector_erase called with index >= self->m_size (index out of bounds)");

	self->m_data->m_destructor(&std_vector_at_mut(*self, index), self->m_allocator);

	if(index != self->m_size - 1) {
		let num_to_move = self->m_size - (index + 1);
		std_memmove(T,
					&std_vector_at_mut(*self, index),
					&std_vector_at_mut(*self, index + 1),
					num_to_move);
	}
	self->m_size--;
}

void StdVectorIdentifier(T,
						 erase_n)(StdVector(T) * restrict self, usize index, usize num_elements) {
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
		std_memmove(T,
					&std_vector_at_mut(*self, index),
					&std_vector_at_mut(*self, end),
					num_to_move);
	}
	self->m_size -= num_elements;
}

void StdVectorIdentifier(T, free)(void* restrict self) {
	let self_ = static_cast(StdVector(T)*)(self);
	for(let_mut i = 0U; i < self_->m_size; ++i) {
		self_->m_data->m_destructor(&std_vector_at_mut(*self_, i), self_->m_allocator);
	}

	if(!StdVectorIdentifier(T, is_short)(self_)) {
		std_allocator_deallocate_array_t(T, self_->m_allocator, self_->m_long, self_->m_capacity);
		self_->m_capacity = SMALL_OPT_CAPACITY;
	}
	self_->m_size = 0U;
}

StdVectorIterator(T) StdVectorIdentifier(T, iterator_new)(const StdVector(T) * restrict self) {
	return (StdVectorIterator(T)){.m_index = 0U, .m_vector = const_cast(StdVector(T)*)(self)};
}

StdVectorConstIterator(T)
	StdVectorIdentifier(T, const_iterator_new)(const StdVector(T) * restrict self) {
	return (StdVectorConstIterator(T)){.m_index = 0U, .m_vector = const_cast(StdVector(T)*)(self)};
}

Ref(T) StdVectorIdentifier(T, iterator_next)(StdRandomAccessIterator(Ref(T)) * restrict self) {
	let _self = static_cast(StdVectorIterator(T)*)(self->m_self);

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

Ref(T) StdVectorIdentifier(T, iterator_previous)(StdRandomAccessIterator(Ref(T)) * restrict self) {
	let _self = static_cast(StdVectorIterator(T)*)(self->m_self);

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

Ref(T) StdVectorIdentifier(T, iterator_at)(const StdRandomAccessIterator(Ref(T)) * restrict self,
										   usize index) {
	let _self = static_cast(StdVectorIterator(T)*)(self->m_self);

	std_assert(index < _self->m_vector->m_size,
			   "std_vector_iterator_at called with index pst the end of the iteration "
			   "(iterator index out of bounds)");
	return &std_vector_at_mut(*(_self->m_vector), index);
}

Ref(T) StdVectorIdentifier(T, iterator_rat)(const StdRandomAccessIterator(Ref(T)) * restrict self,
											usize index) {
	let _self = static_cast(StdVectorIterator(T)*)(self->m_self);

	std_assert(index < _self->m_vector->m_size,
			   "std_vector_iterator_at called with index pst the end of the iteration "
			   "(iterator index out of bounds)");
	return &std_vector_at_mut(*(_self->m_vector), (_self->m_vector->m_size - 1) - index);
}

Ref(T)
	StdVectorIdentifier(T,
						iterator_current)(const StdRandomAccessIterator(Ref(T)) * restrict self) {
	let _self = static_cast(const StdVectorIterator(T)*)(self->m_self);

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

bool StdVectorIdentifier(T, iterator_equals)(const StdRandomAccessIterator(Ref(T)) * restrict self,
											 const StdRandomAccessIterator(Ref(T)) * restrict rhs) {
	let _self = static_cast(const StdVectorIterator(T)*)(self->m_self);
	let _rhs = static_cast(const StdVectorIterator(T)*)(rhs->m_self);

	return _self->m_index == _rhs->m_index && _self->m_vector == _rhs->m_vector;
}

ConstRef(T)
	StdVectorIdentifier(T, iterator_cnext)(StdRandomAccessIterator(ConstRef(T)) * restrict self) {
	let _self = static_cast(StdVectorConstIterator(T)*)(self->m_self);

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

ConstRef(T)
	StdVectorIdentifier(T,
						iterator_cprevious)(StdRandomAccessIterator(ConstRef(T)) * restrict self) {
	let _self = static_cast(StdVectorConstIterator(T)*)(self->m_self);

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

ConstRef(T)
	StdVectorIdentifier(T, iterator_cat)(const StdRandomAccessIterator(ConstRef(T)) * restrict self,
										 usize index) {
	let _self = static_cast(StdVectorConstIterator(T)*)(self->m_self);

	std_assert(index < _self->m_vector->m_size,
			   "std_vector_iterator_at called with index pst the end of the iteration "
			   "(iterator index out of bounds)");
	return &std_vector_at(*(_self->m_vector), index);
}

ConstRef(T)
	StdVectorIdentifier(T,
						iterator_crat)(const StdRandomAccessIterator(ConstRef(T)) * restrict self,
									   usize index) {
	let _self = static_cast(StdVectorConstIterator(T)*)(self->m_self);

	std_assert(index < _self->m_vector->m_size,
			   "std_vector_iterator_at called with index pst the end of the iteration "
			   "(iterator index out of bounds)");
	return &std_vector_at(*(_self->m_vector), (_self->m_vector->m_size - 1) - index);
}

ConstRef(T) StdVectorIdentifier(T, iterator_ccurrent)(
	const StdRandomAccessIterator(ConstRef(T)) * restrict self) {
	let _self = static_cast(const StdVectorConstIterator(T)*)(self->m_self);

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

bool StdVectorIdentifier(T, iterator_cequals)(
	const StdRandomAccessIterator(ConstRef(T)) * restrict self,
	const StdRandomAccessIterator(ConstRef(T)) * restrict rhs) {
	let _self = static_cast(const StdVectorConstIterator(T)*)(self->m_self);
	let _rhs = static_cast(const StdVectorConstIterator(T)*)(rhs->m_self);

	return _self->m_index == _rhs->m_index && _self->m_vector == _rhs->m_vector;
}

StdRandomAccessIterator(Ref(T)) StdVectorIdentifier(T, begin)(StdVector(T) * restrict self) {
	let_mut iter = std_vector_into_iter(*self);
	let_mut inner = static_cast(StdVectorIterator(T)*)(iter.m_self);
	inner->m_index = 0;
	return iter;
}

StdRandomAccessIterator(Ref(T)) StdVectorIdentifier(T, end)(StdVector(T) * restrict self) {
	let_mut iter = std_vector_into_iter(*self);
	let_mut inner = static_cast(StdVectorIterator(T)*)(iter.m_self);
	inner->m_index = static_cast(isize)(self->m_size);
	return iter;
}

StdRandomAccessIterator(Ref(T)) StdVectorIdentifier(T, rbegin)(StdVector(T) * restrict self) {
	let_mut iter = std_vector_into_reverse_iter(*self);
	let_mut inner = static_cast(StdVectorIterator(T)*)(iter.m_self);
	inner->m_index = static_cast(isize)(self->m_size - 1);
	return iter;
}

StdRandomAccessIterator(Ref(T)) StdVectorIdentifier(T, rend)(StdVector(T) * restrict self) {
	let_mut iter = std_vector_into_reverse_iter(*self);
	let_mut inner = static_cast(StdVectorIterator(T)*)(iter.m_self);
	inner->m_index = -1;
	return iter;
}

StdRandomAccessIterator(ConstRef(T))
	StdVectorIdentifier(T, cbegin)(const StdVector(T) * restrict self) {
	let_mut iter = std_vector_into_const_iter(*self);
	let_mut inner = static_cast(StdVectorConstIterator(T)*)(iter.m_self);
	inner->m_index = 0;
	return iter;
}

StdRandomAccessIterator(ConstRef(T))
	StdVectorIdentifier(T, cend)(const StdVector(T) * restrict self) {
	let_mut iter = std_vector_into_const_iter(*self);
	let_mut inner = static_cast(StdVectorConstIterator(T)*)(iter.m_self);
	inner->m_index = static_cast(isize)(self->m_size);
	return iter;
}

StdRandomAccessIterator(ConstRef(T))
	StdVectorIdentifier(T, crbegin)(const StdVector(T) * restrict self) {
	let_mut iter = std_vector_into_reverse_const_iter(*self);
	let_mut inner = static_cast(StdVectorConstIterator(T)*)(iter.m_self);
	inner->m_index = static_cast(isize)(self->m_size - 1);
	return iter;
}

StdRandomAccessIterator(ConstRef(T))
	StdVectorIdentifier(T, crend)(const StdVector(T) * restrict self) {
	let_mut iter = std_vector_into_reverse_const_iter(*self);
	let_mut inner = static_cast(StdVectorConstIterator(T)*)(iter.m_self);
	inner->m_index = -1;
	return iter;
}

	#undef STD_TEMPLATE_SUPPRESS_INSTANTIATIONS
#endif // defined(T) && defined(SMALL_OPT_CAPACITY) && STD_TEMPLATE_IMPL
