/// @file StdString.c
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief This module provides string and stringview types comparable to C++ for C2nxt
/// @version 0.1.1
/// @date 2022-01-11
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

#include <C2nxt/StdString.h>
#include <memory.h>
//#include <uchar.h>
#include <C2nxt/StdAssert.h>
#include <C2nxt/StdMath.h>
#include <C2nxt/StdPlatform.h>
#include <C2nxt/StdRange.h>
#include <wchar.h>

ImplStdOption(StdString);
ImplStdOption(Ref(StdString));
ImplStdOption(ConstRef(StdString));
ImplStdOption(StdStringView);
ImplStdOption(Ref(StdStringView));
ImplStdOption(ConstRef(StdStringView));

static const std_stringview_vtable_t std_stringview_vtable = {
	.begin = std_stringview_begin,
	.end = std_stringview_end,
	.rbegin = std_stringview_rbegin,
	.rend = std_stringview_rend,
	.cbegin = std_stringview_begin,
	.cend = std_stringview_end,
	.crbegin = std_stringview_rbegin,
	.crend = std_stringview_rend,
};

static const std_string_vtable_t std_string_vtable = {
	.begin = std_string_begin,
	.end = std_string_end,
	.rbegin = std_string_rbegin,
	.rend = std_string_rend,
	.cbegin = std_string_cbegin,
	.cend = std_string_cend,
	.crbegin = std_string_crbegin,
	.crend = std_string_crend,
};

/// @brief Creates a default `StdStringIterator` (one pointing to the beginning of the string)
///
/// @param self - The `StdString` to get an iterator for
///
/// @return a `StdStringIterator` into `self`
StdStringIterator std_string_iterator_new(const StdString* restrict self);
/// @brief Creates a default `StdStringConstIterator` (one pointing to the beginning of the string)
///
/// @param self - The `StdString` to get an iterator for
///
/// @return a `StdStringConstIterator` into `self`
StdStringConstIterator std_string_const_iterator_new(const StdString* restrict self);
/// @brief Returns a reference to the current value the iterator points to, then increments the
/// iterator
///
/// @param self - The iterator to increment
///
/// @note if the iterator is at the end of the iteration, the result of calling this is undefined
char_ref std_string_iterator_next(StdRandomAccessIterator(char_ref) * restrict self);
/// @brief Returns a reference to the current value the iterator points to, then decrements the
/// iterator
///
/// @param self - The iterator to decrement
///
/// @note if the iterator is past the beginning of the iteration, the result of calling this is
/// undefined
char_ref std_string_iterator_previous(StdRandomAccessIterator(char_ref) * restrict self);
/// @brief Returns a reference to the value associated with the given index into the iteration
///
/// @param self - The iterator to get the value from
/// @param index - The index associated with the value
///
/// @note if the index is past the beginning or end of the iteration, the result of calling this is
/// undefined
char_ref
std_string_iterator_at(const StdRandomAccessIterator(char_ref) * restrict self, usize index);
/// @brief Returns a reference to the value associated with the given index into the iteration
///
/// @param self - The iterator to get the value from
/// @param index - The index associated with the value
///
/// @note if the index is past the beginning or end of the iteration, the result of calling this is
/// undefined
char_ref
std_string_iterator_rat(const StdRandomAccessIterator(char_ref) * restrict self, usize index);
/// @brief Returns a reference to the current value the iterator points to
///
/// @param self - The iterator to get the current value of
///
/// @return the current value the iterator points to
///
/// @note if this iterator is past the beginning or end of the iteration, the result of calling this
/// is undefined
char_ref std_string_iterator_current(const StdRandomAccessIterator(char_ref) * restrict self);
/// @brief Returns whether the two iterators are equal (they belong to the same container and are at
/// the same index in the iteration)
///
/// @param self - This iterator
/// @param rhs - The iterator to compare to
///
/// @return whether the iterators are equal
bool std_string_iterator_equals(const StdRandomAccessIterator(char_ref) * restrict self,
								const StdRandomAccessIterator(char_ref) * restrict rhs);

/// @brief Returns a reference to the current value the iterator points to, then increments the
/// iterator
///
/// @param self - The iterator to increment
///
/// @note if the iterator is at the end of the iteration, the result of calling this is undefined
const_char_ref std_string_iterator_cnext(StdRandomAccessIterator(const_char_ref) * restrict self);
/// @brief Returns a reference to the current value the iterator points to, then decrements the
/// iterator
///
/// @param self - The iterator to decrement
///
/// @note if the iterator is past the beginning of the iteration, the result of calling this is
/// undefined
const_char_ref
std_string_iterator_cprevious(StdRandomAccessIterator(const_char_ref) * restrict self);
/// @brief Returns a reference to the value associated with the given index into the iteration
///
/// @param self - The iterator to get the value from
/// @param index - The index associated with the value
///
/// @note if the index is past the beginning or end of the iteration, the result of calling this is
/// undefined
const_char_ref
std_string_iterator_cat(const StdRandomAccessIterator(const_char_ref) * restrict self, usize index);
/// @brief Returns a reference to the value associated with the given index into the iteration
///
/// @param self - The iterator to get the value from
/// @param index - The index associated with the value
///
/// @note if the index is past the beginning or end of the iteration, the result of calling this is
/// undefined
const_char_ref
std_string_iterator_crat(const StdRandomAccessIterator(const_char_ref) * restrict self,
						 usize index);
/// @brief Returns a reference to the current value the iterator points to
///
/// @param self - The iterator to get the current value of
///
/// @return the current value the iterator points to
///
/// @note if this iterator is past the beginning or end of the iteration, the result of calling this
/// is undefined
const_char_ref
std_string_iterator_ccurrent(const StdRandomAccessIterator(const_char_ref) * restrict self);
/// @brief Returns whether the two iterators are equal (they belong to the same container and are at
/// the same index in the iteration)
///
/// @param self - This iterator
/// @param rhs - The iterator to compare to
///
/// @return whether the iterators are equal
bool std_string_iterator_cequals(const StdRandomAccessIterator(const_char_ref) * restrict self,
								 const StdRandomAccessIterator(const_char_ref) * restrict rhs);

/// @brief Creates a default `StdStringViewIterator` (one pointing to the beginning of the view),
/// allocated by the given allocated
///
/// @param self - The `StdStringView` to get an iterator for
///
/// @return a `StdStringViewIterator` into the stringview
StdStringViewIterator std_stringview_iterator_new(const StdStringView* restrict self);
/// @brief Returns a reference to the current value the iterator points to, then increments the
/// iterator
///
/// @param self - The iterator to increment
///
/// @note if the iterator is at the end of the iteration, the result of calling this is undefined
const_char_ref
std_stringview_iterator_next(StdRandomAccessIterator(const_char_ref) * restrict self);
/// @brief Returns a reference to the current value the iterator points to, then decrements the
/// iterator
///
/// @param self - The iterator to decrement
///
/// @note if the iterator is past the beginning of the iteration, the result of calling this is
/// undefined
const_char_ref
std_stringview_iterator_previous(StdRandomAccessIterator(const_char_ref) * restrict self);
/// @brief Returns a reference to the value associated with the given index into the iteration
///
/// @param self - The iterator to get the value from
/// @param index - The index associated with the value
///
/// @note if the index is past the beginning or end of the iteration, the result of calling this is
/// undefined
const_char_ref
std_stringview_iterator_at(const StdRandomAccessIterator(const_char_ref) * restrict self,
						   usize index);
/// @brief Returns a reference to the value associated with the given index into the iteration
///
/// @param self - The iterator to get the value from
/// @param index - The index associated with the value
///
/// @note if the index is past the beginning or end of the iteration, the result of calling this is
/// undefined
const_char_ref
std_stringview_iterator_rat(const StdRandomAccessIterator(const_char_ref) * restrict self,
							usize index);
/// @brief Returns a reference to the current value the iterator points to
///
/// @param self - The iterator to get the current value of
///
/// @return the current value the iterator points to
///
/// @note if this iterator is past the beginning or end of the iteration, the result of calling this
/// is undefined
const_char_ref
std_stringview_iterator_current(const StdRandomAccessIterator(const_char_ref) * restrict self);
/// @brief Returns whether the two iterators are equal (they belong to the same container and are at
/// the same index in the iteration)
///
/// @param self - This iterator
/// @param rhs - The iterator to compare to
///
/// @return whether the iterators are equal
bool std_stringview_iterator_equals(const StdRandomAccessIterator(const_char_ref) * restrict self,
									const StdRandomAccessIterator(const_char_ref) * restrict rhs);

ImplIntoStdRandomAccessIterator(StdString,
								char_ref,
								std_string_into_iter,
								into,
								std_string_iterator_new,
								std_string_iterator_next,
								std_string_iterator_previous,
								std_string_iterator_at,
								std_string_iterator_current,
								std_string_iterator_equals);
ImplIntoStdRandomAccessIterator(StdString,
								char_ref,
								std_string_into_reverse_iter,
								into_reverse,
								std_string_iterator_new,
								std_string_iterator_next,
								std_string_iterator_previous,
								std_string_iterator_rat,
								std_string_iterator_current,
								std_string_iterator_equals);

ImplIntoStdRandomAccessIterator(StdString,
								const_char_ref,
								std_string_into_const_iter,
								into,
								std_string_const_iterator_new,
								std_string_iterator_cnext,
								std_string_iterator_cprevious,
								std_string_iterator_cat,
								std_string_iterator_ccurrent,
								std_string_iterator_cequals);
ImplIntoStdRandomAccessIterator(StdString,
								const_char_ref,
								std_string_into_reverse_const_iter,
								into_reverse,
								std_string_const_iterator_new,
								std_string_iterator_cnext,
								std_string_iterator_cprevious,
								std_string_iterator_crat,
								std_string_iterator_ccurrent,
								std_string_iterator_cequals);

ImplIntoStdRandomAccessIterator(StdStringView,
								const_char_ref,
								std_stringview_into_iter,
								into,
								std_stringview_iterator_new,
								std_stringview_iterator_next,
								std_stringview_iterator_previous,
								std_stringview_iterator_at,
								std_stringview_iterator_current,
								std_stringview_iterator_equals);
ImplIntoStdRandomAccessIterator(StdStringView,
								const_char_ref,
								std_stringview_into_reverse_iter,
								into_reverse,
								std_stringview_iterator_new,
								std_stringview_iterator_next,
								std_stringview_iterator_previous,
								std_stringview_iterator_rat,
								std_stringview_iterator_current,
								std_stringview_iterator_equals);

static const bool std_string_is_little_endian
	= STD_PLATFORM_LITTLE_ENDIAN || !STD_PLATFORM_BIG_ENDIAN;
static const usize std_string_long_mask
	= std_string_is_little_endian ? static_cast(usize)(0x40U) : static_cast(usize)(0x01U);
static const usize std_string_capacity_mode_shift
	= std_string_is_little_endian ? (sizeof(usize) - 1) * 8U : 0U;
static const usize std_string_len_cap_shift = std_string_is_little_endian ? 0U : 1U;

always_inline static inline u8* std_string_length_remaining(const StdString* restrict self) {
	return static_cast(u8*)(&(self->m_short[STD_STRING_SHORT_OPTIMIZATION_CAPACITY]));
}

always_inline static inline bool std_string_is_short(const StdString* restrict self) {
	return !((self->m_capacity >> std_string_capacity_mode_shift) // NOLINT
			 & std_string_long_mask);
}

always_inline static inline void std_string_set_long(StdString* restrict self) {
	self->m_capacity |= (std_string_long_mask << std_string_capacity_mode_shift);
}

always_inline static inline void std_string_set_short(StdString* restrict self) {
	self->m_capacity
		&= ~(static_cast(usize)(std_string_long_mask << std_string_capacity_mode_shift));
}

always_inline static inline usize std_string_get_capacity(const StdString* restrict self) {
	return std_string_is_short(self) ?
			   STD_STRING_SHORT_OPTIMIZATION_CAPACITY :
				 (self->m_capacity
				& (~(std_string_long_mask << std_string_capacity_mode_shift))
					  >> std_string_len_cap_shift);
}

always_inline static inline void
std_string_set_capacity(StdString* restrict self, usize new_capacity) {
	std_assert(!std_string_is_short(self), "Can't set capacity on a short optimized string");
	self->m_capacity = (new_capacity << std_string_len_cap_shift)
					   | (std_string_long_mask << std_string_capacity_mode_shift);
}

always_inline static inline usize std_string_get_short_length(const StdString* restrict self) {
	std_assert(std_string_is_short(self), "Can't get short length of a long string");
	return STD_STRING_SHORT_OPTIMIZATION_CAPACITY
		   - (static_cast(usize)(*(std_string_length_remaining(self))) >> std_string_len_cap_shift);
}

always_inline static inline void std_string_set_length(StdString* restrict self, usize new_length) {
	std_assert(new_length <= std_string_capacity(*self),
			   "Can't set string length longer than capacity");
	if(std_string_is_short(self)) {
		*(std_string_length_remaining(self)) = static_cast(u8)(
			((STD_STRING_SHORT_OPTIMIZATION_CAPACITY - new_length) << std_string_len_cap_shift));
	}
	else {
		self->m_length = new_length;
	}
}

always_inline static inline void
std_string_increase_length(StdString* restrict self, usize amount_to_increase) {
	std_string_set_length(self, std_string_length(*self) + amount_to_increase);
}

always_inline static inline void
std_string_decrease_length(StdString* restrict self, usize amount_to_decrease) {
	std_assert(amount_to_decrease <= std_string_length(*self),
			   "Can't decrease string length by more than length");
	std_string_set_length(self, std_string_length(*self) - amount_to_decrease);
}

StdString std_string_new(void) {
	return std_string_new_with_allocator(DEFAULT_ALLOCATOR);
}

StdString std_string_new_with_allocator(StdAllocator allocator) {
	let_mut str = (StdString){
		.m_short = {[STD_STRING_SHORT_OPTIMIZATION_CAPACITY]
					= STD_STRING_SHORT_OPTIMIZATION_CAPACITY << std_string_len_cap_shift},
		.m_allocator = allocator,
		.m_vtable = &std_string_vtable};
	// std_string_set_length(&str, 0);
	return str;
}

StdString std_string_new_with_capacity(usize capacity) {
	return std_string_new_with_capacity_with_allocator(capacity, DEFAULT_ALLOCATOR);
}

StdString std_string_new_with_capacity_with_allocator(usize capacity, StdAllocator allocator) {
	let_mut string = std_string_new_with_allocator(allocator);
	if(capacity > STD_STRING_SHORT_OPTIMIZATION_CAPACITY) {
		std_string_set_long(&string);
		std_string_set_capacity(&string, capacity);
		std_string_set_length(&string, 0);
		string.m_long = static_cast(cstring)(
			std_allocator_allocate_array_t(char, string.m_allocator, capacity + 1).m_memory);
	}

	return string;
}

StdString std_string_from_cstring(restrict const_cstring string, usize length) {
	return std_string_from_cstring_with_allocator(string, length, DEFAULT_ALLOCATOR);
}

StdString std_string_from_cstring_with_allocator(restrict const_cstring string,
												 usize length,
												 StdAllocator allocator) {
	let_mut std_string = std_string_new_with_capacity_with_allocator(length, allocator);
	std_string_set_length(&std_string, length);
	std_memcpy(char, &std_string_at(std_string, 0), string, length);
	return std_string;
}

StdString std_string_from_wcstring(restrict const_wcstring string, usize length) {
	return std_string_from_wcstring_with_allocator(string, length, DEFAULT_ALLOCATOR);
}

StdString std_string_from_wcstring_with_allocator(restrict const_wcstring string,
												  usize length,
												  StdAllocator allocator) {
	let cstring_length = static_cast(usize)(snprintf(nullptr, length, "%ls", string));
	let_mut std_string = std_string_new_with_capacity_with_allocator(cstring_length, allocator);
	std_string_set_length(&std_string, cstring_length);
	let_mut str = &(std_string_at(std_string, 0));
	ignore(snprintf(str, length, "%ls", string));
	return std_string;
}

StdString std_string_from_stringview(const StdStringView* restrict view) {
	return std_string_from_cstring(view->m_view, view->m_length);
}

StdString std_string_from_stringview_with_allocator(const StdStringView* restrict view,
													StdAllocator allocator) {
	return std_string_from_cstring_with_allocator(view->m_view, view->m_length, allocator);
}

const_cstring(std_string_into_cstring)(const StdString* restrict self) {
	return std_string_is_short(self) ? &(self->m_short[0]) : self->m_long;
}

const_wcstring(std_string_into_wcstring)(const StdString* restrict self) {
	return std_string_into_wcstring_with_allocator(*self, self->m_allocator);
}

const_wcstring(std_string_into_wcstring_with_allocator)(const StdString* restrict self,
														StdAllocator allocator) {
	let_mut string = std_string_into_cstring(*self);
	let length = static_cast(usize)(swprintf(nullptr, std_string_length(*self), L"%s", string));
	let_mut wstring = static_cast(wcstring)(
		std_allocator_allocate_array_t(wchar_t, allocator, length).m_memory);
	ignore(swprintf(wstring, std_string_length(*self), L"%s", string));
	return wstring;
}

StdStringView(std_string_into_stringview)(const StdString* restrict self) {
	return (StdStringView){.m_view = std_string_into_cstring(*self),
						   .m_length = std_string_length(*self),
						   .m_vtable = &std_stringview_vtable};
}

StdString(std_string_clone)(const StdString* restrict self) {
	return std_string_clone_with_allocator(*self, self->m_allocator);
}

StdString(std_string_clone_with_allocator)(const StdString* restrict self, StdAllocator allocator) {
	let length = std_string_length(*self);
	let_mut string = std_string_new_with_capacity_with_allocator(length, allocator);
	std_string_set_length(&string, length);
	std_memcpy(char, &std_string_at(string, 0), &std_string_at(*self, 0), length);

	return string;
}

void(std_string_free)(void* restrict self) {
	let self_ptr = static_cast(StdString*)(self);
	if(!std_string_is_short(self_ptr)) {
		let ptr = self_ptr->m_long;
		let capacity = std_string_get_capacity(self_ptr);
		std_string_set_short(self_ptr);
		self_ptr->m_long = nullptr;
		std_allocator_deallocate_array_t(char, self_ptr->m_allocator, ptr, capacity);
	}
}

char_ptr std_string_at_mut(StdString* restrict self, usize index) {
	std_assert(index <= std_string_length(*self),
			   "std_string_at called with index > self->m_length (index out of bounds)");
	std_assert(index < std_string_capacity(*self),
			   "std_string_at called with index >= self->m_capacity (index out of bounds)");
	return std_string_is_short(self) ? &(self->m_short[index]) : &(self->m_long[index]);
}

const_char_ptr std_string_at_const(const StdString* restrict self, usize index) {
	std_assert(index <= std_string_length(*self),
			   "std_string_at called with index > self->m_length (index out of bounds)");
	std_assert(index < std_string_capacity(*self),
			   "std_string_at called with index >= self->m_capacity (index out of bounds)");
	return std_string_is_short(self) ? &(self->m_short[index]) : &(self->m_long[index]);
}

char_ptr std_string_front_mut(StdString* restrict self) {
	return &std_string_at(*self, 0);
}

const_char_ptr std_string_front_const(const StdString* restrict self) {
	return &std_string_at(*self, 0);
}

char_ptr std_string_back_mut(StdString* restrict self) {
	let length = std_string_length(*self);
	let back_index = length > 0 ? length - 1 : 0U;
	return &std_string_at(*self, back_index);
}

const_char_ptr std_string_back_const(const StdString* restrict self) {
	let length = std_string_length(*self);
	let back_index = length > 0 ? length - 1 : 0U;
	return &std_string_at(*self, back_index);
}

bool(std_string_is_empty)(const StdString* restrict self) {
	return std_string_size(*self) == 0;
}

bool(std_string_is_full)(const StdString* restrict self) {
	return std_string_size(*self) == std_string_capacity(*self);
}

usize(std_string_size)(const StdString* restrict self) {
	return std_string_is_short(self) ? std_string_get_short_length(self) : self->m_length;
}

usize(std_string_length)(const StdString* restrict self) {
	return std_string_size(*self);
}

usize std_string_max_size(void) {
	return std_max_value(usize) - 2;
}

usize(std_string_capacity)(const StdString* restrict self) {
	return std_string_get_capacity(self);
}

StdString(std_string_first)(const StdString* restrict self, usize num_chars) {
	let num_to_copy = std_min(num_chars, std_string_length(*self));
	let_mut string = std_string_new_with_capacity_with_allocator(num_chars, self->m_allocator);
	std_memcpy(char, &std_string_at(string, 0), &std_string_at(*self, 0), num_to_copy);
	std_string_set_length(&string, num_to_copy);

	return string;
}

cstring(std_string_first_cstring)(const StdString* restrict self, usize num_chars) {
	let num_to_copy = std_min(num_chars, std_string_length(*self));
	let_mut string = static_cast(cstring)(
		std_allocator_allocate_array_t(char, self->m_allocator, num_chars + 1).m_memory);

	std_memcpy(char, string, &std_string_at(*self, 0), num_to_copy);

	return string;
}

StdStringView(std_string_first_stringview)(const StdString* restrict self, usize num_chars) {
	let length = std_min(num_chars, std_string_length(*self));

	return (StdStringView){.m_view = &std_string_at(*self, 0), .m_length = length};
}

StdString(std_string_last)(const StdString* restrict self, usize num_chars) {
	let length = std_string_length(*self);
	let num_to_copy = std_min(num_chars, length);
	let start_index = num_chars >= length ? 0 : (length - num_chars);
	let_mut string = std_string_new_with_capacity_with_allocator(num_chars, self->m_allocator);
	std_memcpy(char, &std_string_at(string, 0), &std_string_at(*self, start_index), num_to_copy);
	std_string_set_length(&string, num_to_copy);

	return string;
}

cstring(std_string_last_cstring)(const StdString* restrict self, usize num_chars) {
	let length = std_string_length(*self);
	let num_to_copy = std_min(num_chars, length);
	let start_index = num_chars >= length ? 0 : (length - num_chars);
	let_mut string = static_cast(cstring)(
		std_allocator_allocate_array_t(char, self->m_allocator, num_chars + 1).m_memory);

	std_memcpy(char, string, &std_string_at(*self, start_index), num_to_copy);

	return string;
}

StdStringView(std_string_last_stringview)(const StdString* restrict self, usize num_chars) {
	let self_length = std_string_length(*self);
	let begin = num_chars >= self_length ? 0U : self_length - num_chars;
	let length = std_min(num_chars, self_length);

	return (StdStringView){.m_view = &std_string_at(*self, begin), .m_length = length};
}

bool(std_string_equal)(const StdString* restrict self, const StdString* restrict to_compare) {
	let length = std_string_length(*self);
	if(length != std_string_length(*to_compare)) {
		return false;
	}

	return 0 == memcmp(&std_string_at(*self, 0), &std_string_at(*to_compare, 0), length);
}

bool std_string_equal_cstring(const StdString* restrict self,
							  restrict const_cstring to_compare,
							  usize length) {
	let self_length = std_string_length(*self);
	if(self_length != length) {
		return false;
	}

	return 0 == memcmp(&std_string_at(*self, 0), to_compare, self_length);
}

bool std_string_equal_stringview(const StdString* restrict self,
								 const StdStringView* restrict to_compare) {
	return std_string_equal_cstring(self, to_compare->m_view, to_compare->m_length);
}

/// @brief Determines if the substring of `self` beginning at `index` is the same as the given
/// `substring`
static inline always_inline
StdOption(usize) std_string_contains_from_left(const StdString* restrict self,
											   const StdString* restrict substring,
											   usize index) {
	let sublength = std_string_length(*substring);
	std_assert(index + sublength < std_string_length(*self),
			   "std_string_contains_from_left called with index + substring->m_length >= "
			   "self->m_length (indices are out of bounds)");

	if(0 == memcmp(&std_string_at(*self, index), &std_string_at(*substring, 0), sublength)) {
		return Some(usize, index);
	}
	else {
		return None(usize);
	}
}

/// @brief Determines if the substring of `self` ending with `index` (inclusive)is the same as
/// the given `substring`
static inline always_inline
StdOption(usize) std_string_contains_from_right(const StdString* restrict self,
												const StdString* restrict substring,
												usize index) {

	let sublength = std_string_length(*substring);
	std_assert(index >= sublength,
			   "std_string_contains_from_right called with index < substring->m_length (index out "
			   "of bounds)");

	let start_index = index - (sublength - 1);
	if(0 == memcmp(&std_string_at(*self, start_index), &std_string_at(*substring, 0), sublength)) {
		return Some(usize, start_index);
	}
	else {
		return None(usize);
	}
}

bool(std_string_contains)(const StdString* restrict self, const StdString* restrict substring) {
	let length = std_string_length(*self);
	let sublength = std_string_length(*substring);
	if(length < sublength) {
		return false;
	}

	for(usize left = static_cast(usize)(0), right = length - 1;
		right >= sublength && left + sublength < length;
		++left, --right)
	{

		let contains_left = std_string_contains_from_left(self, substring, left);
		if(std_option_is_some(contains_left)) {
			return true;
		}

		let contains_right = std_string_contains_from_right(self, substring, right);
		if(std_option_is_some(contains_right)) {
			return true;
		}
	}

	return false;
}

/// @brief Determines if the substring of `self` beginning at `index` is the same as the given
/// `substring`
static inline always_inline
StdOption(usize) std_string_contains_cstring_from_left(const StdString* restrict self,
													   restrict const_cstring substring,
													   usize substring_length,
													   usize index) {
	std_assert(index + substring_length < std_string_length(*self),
			   "std_string_contains_cstring_from_left called with index + substring->m_length >= "
			   "self->m_length (indices are out of bounds)");

	if(0 == memcmp(&std_string_at(*self, index), substring, substring_length)) {
		return Some(usize, index);
	}
	else {
		return None(usize);
	}
}

/// @brief Determines if the substring of `self` ending with `index` (inclusive)is the same as
/// the given `substring`
static inline always_inline
StdOption(usize) std_string_contains_cstring_from_right(const StdString* restrict self,
														restrict const_cstring substring,
														usize substring_length,
														usize index) {
	std_assert(
		index >= substring_length,
		"std_string_contains_cstring_from_right called with index < substring->m_length (index out "
		"of bounds)");

	let start_index = index - (substring_length - 1);
	if(0 == memcmp(&std_string_at(*self, start_index), substring, substring_length)) {
		return Some(usize, start_index);
	}
	else {
		return None(usize);
	}
}

bool std_string_contains_cstring(const StdString* restrict self,
								 restrict const_cstring substring,
								 usize substring_length) {
	let length = std_string_length(*self);
	if(length < substring_length) {
		return false;
	}
	for(usize left = static_cast(usize)(0), right = length - 1;
		right >= substring_length && left + substring_length < length;
		++left, --right)
	{

		let contains_left
			= std_string_contains_cstring_from_left(self, substring, substring_length, left);
		if(std_option_is_some(contains_left)) {
			return true;
		}

		let contains_right
			= std_string_contains_cstring_from_right(self, substring, substring_length, right);
		if(std_option_is_some(contains_right)) {
			return true;
		}
	}

	return false;
}

bool std_string_contains_stringview(const StdString* restrict self,
									const StdStringView* restrict substring) {
	return std_string_contains_cstring(self, substring->m_view, substring->m_length);
}

bool(std_string_starts_with)(const StdString* restrict self, const StdString* restrict substring) {
	let starts_with = std_string_contains_from_left(self, substring, 0);
	return std_option_is_some(starts_with);
}

bool std_string_starts_with_cstring(const StdString* restrict self,
									restrict const_cstring substring,
									usize substring_length) {
	let starts_with = std_string_contains_cstring_from_left(self, substring, substring_length, 0);
	return std_option_is_some(starts_with);
}

bool std_string_starts_with_stringview(const StdString* restrict self,
									   const StdStringView* restrict substring) {
	return std_string_starts_with_cstring(self, substring->m_view, substring->m_length);
}

bool(std_string_ends_with)(const StdString* restrict self, const StdString* restrict substring) {
	let ends_with = std_string_contains_from_right(self, substring, std_string_length(*self) - 1);
	return std_option_is_some(ends_with);
}

bool std_string_ends_with_cstring(const StdString* restrict self,
								  restrict const_cstring substring,
								  usize substring_length) {
	let ends_with = std_string_contains_cstring_from_right(self,
														   substring,
														   substring_length,
														   std_string_length(*self) - 1);
	return std_option_is_some(ends_with);
}

bool std_string_ends_with_stringview(const StdString* restrict self,
									 const StdStringView* restrict substring) {
	return std_string_ends_with_cstring(self, substring->m_view, substring->m_length);
}

StdOption(usize)(std_string_find_first)(const StdString* restrict self,
										const StdString* restrict substring) {
	let length = std_string_length(*self);
	let sublength = std_string_length(*substring);
	std_assert(length > sublength,
			   "std_string_find_first called with "
			   "substring->m_length > self->m_length (substring length out of bounds");
	let end = length - sublength;
	for(let_mut i = 0U; i < end; ++i) {
		let maybe_contains = std_string_contains_from_left(self, substring, i);
		if(std_option_is_some(maybe_contains)) {
			return maybe_contains;
		}
	}

	return None(usize);
}

StdOption(usize) std_string_find_first_cstring(const StdString* restrict self,
											   restrict const_cstring substring,
											   usize substring_length) {
	let length = std_string_length(*self);
	std_assert(length > substring_length,
			   "std_string_find_first_cstring called with "
			   "substring_length > self->m_length (substring length out of bounds");
	let end = length - substring_length;
	for(let_mut i = 0U; i < end; ++i) {
		let maybe_contains
			= std_string_contains_cstring_from_left(self, substring, substring_length, i);
		if(std_option_is_some(maybe_contains)) {
			return maybe_contains;
		}
	}

	return None(usize);
}

StdOption(usize) std_string_find_first_stringview(const StdString* restrict self,
												  const StdStringView* restrict substring) {
	return std_string_find_first_cstring(self, substring->m_view, substring->m_length);
}

StdOption(usize)(std_string_find_last)(const StdString* restrict self,
									   const StdString* restrict substring) {
	let length = std_string_length(*self);
	let sublength = std_string_length(*substring);
	std_assert(length > sublength,
			   "std_string_find_first_cstring called with "
			   "substring->m_length > self->m_length (substring length out of bounds");
	std_assert(sublength > 0, "std_string_find_last called with a substring of 0 length");

	let end = sublength;
	for(let_mut i = length - 1; i >= end; --i) {
		let maybe_contains = std_string_contains_from_right(self, substring, i);
		if(std_option_is_some(maybe_contains)) {
			return maybe_contains;
		}
	}
	return None(usize);
}

StdOption(usize) std_string_find_last_cstring(const StdString* restrict self,
											  restrict const_cstring substring,
											  usize substring_length) {
	let length = std_string_length(*self);
	std_assert(length > substring_length,
			   "std_string_find_first_cstring called with "
			   "substring_length > self->m_length (substring length out of bounds");
	std_assert(substring_length > 0, "std_string_find_last called with a substring of 0 length");

	let end = substring_length;
	for(let_mut i = length - 1; i >= end; --i) {
		let maybe_contains
			= std_string_contains_cstring_from_right(self, substring, substring_length, i);
		if(std_option_is_some(maybe_contains)) {
			return maybe_contains;
		}
	}
	return None(usize);
}

StdOption(usize) std_string_find_last_stringview(const StdString* restrict self,
												 const StdStringView* restrict substring) {
	return std_string_find_last_cstring(self, substring->m_view, substring->m_length);
}

StdString(std_string_substring)(const StdString* restrict self, usize index, usize length) {
	return std_string_substring_with_allocator(*self, index, length, self->m_allocator);
}

StdString(std_string_substring_with_allocator)(const StdString* restrict self,
											   usize index,
											   usize length,
											   StdAllocator allocator) {
	let maybe_unused self_length = std_string_length(*self);
	std_assert(index < self_length,
			   "std_string_substring called with index >= self->m_length (index out of bounds)");
	std_assert(index + length < self_length,
			   "std_string_substring called with index + length >= self->m_length (substring range "
			   "out of bounds)");

	let_mut string = std_string_new_with_capacity_with_allocator(length, allocator);

	std_memcpy(char, &std_string_at(string, 0), &std_string_at(*self, index), length);
	std_string_set_length(&string, length);

	return string;
}

StdStringView(std_string_stringview_of)(const StdString* restrict self, usize index, usize length) {
	let maybe_unused self_length = std_string_length(*self);
	std_assert(index < self_length,
			   "std_stringview_of called with index >= self->m_length (index out of bounds)");
	std_assert(
		index + length < self_length,
		"std_string_stringview_of called with index + length >= self->m_length (substring range "
		"out of bounds)");

	return (StdStringView){.m_view = &std_string_at(*self, index),
						   .m_length = length,
						   .m_vtable = &std_stringview_vtable};
}

StdString(std_string_concatenate)(const StdString* restrict left, const StdString* restrict right) {
	return std_string_concatenate_with_allocator(left, right, left->m_allocator);
}

StdString std_string_concatenate_cstring(const StdString* restrict left,
										 restrict const_cstring right,
										 usize right_length) {
	return std_string_concatenate_cstring_with_allocator(left,
														 right,
														 right_length,
														 left->m_allocator);
}

StdString std_string_concatenate_cstrings(restrict const_cstring left,
										  usize left_length,
										  restrict const_cstring right,
										  usize right_length) {
	return std_string_concatenate_cstrings_with_allocator(left,
														  left_length,
														  right,
														  right_length,
														  STD_DEFAULT_ALLOCATOR);
}

StdString std_string_concatenate_stringview(const StdString* restrict left,
											const StdStringView* restrict right) {
	return std_string_concatenate_cstring(left, right->m_view, right->m_length);
}

// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
StdString std_string_concatenate_stringviews(const StdStringView* restrict left,
											 const StdStringView* restrict right) {
	return std_string_concatenate_cstrings(left->m_view,
										   left->m_length,
										   right->m_view,
										   right->m_length);
}

// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
StdString(std_string_concatenate_with_allocator)(const StdString* restrict left,
												 const StdString* restrict right,
												 StdAllocator allocator) {
	let left_length = std_string_length(*left);
	let right_length = std_string_length(*right);
	let_mut string
		= std_string_new_with_capacity_with_allocator(left_length + right_length, allocator);

	std_string_set_length(&string, left_length + right_length);
	std_memcpy(char, &std_string_at(string, 0), &std_string_at(*left, 0), left_length);
	if(right_length > 0) {
		std_memcpy(char,
				   &std_string_at(string, left_length),
				   &std_string_at(*right, 0),
				   right_length);
	}

	return string;
}

StdString std_string_concatenate_cstring_with_allocator(const StdString* restrict left,
														restrict const_cstring right,
														usize right_length,
														StdAllocator allocator) {
	let left_length = std_string_length(*left);
	let_mut string
		= std_string_new_with_capacity_with_allocator(left_length + right_length, allocator);

	std_string_set_length(&string, left_length + right_length);
	std_memcpy(char, &std_string_at(string, 0), &std_string_at(*left, 0), left_length);
	if(right_length > 0) {
		std_memcpy(char, &std_string_at(string, left_length), right, right_length);
	}

	return string;
}

StdString std_string_concatenate_cstrings_with_allocator(restrict const_cstring left,
														 usize left_length,
														 restrict const_cstring right,
														 usize right_length,
														 StdAllocator allocator) {
	let_mut string
		= std_string_new_with_capacity_with_allocator(left_length + right_length, allocator);

	std_string_set_length(&string, left_length + right_length);
	std_memcpy(char, &std_string_at(string, 0), left, left_length);
	if(right_length > 0) {
		std_memcpy(char, &std_string_at(string, left_length), right, right_length);
	}

	return string;
}

StdString std_string_concatenate_stringview_with_allocator(const StdString* restrict left,
														   const StdStringView* restrict right,
														   StdAllocator allocator) {
	return std_string_concatenate_cstring_with_allocator(left,
														 right->m_view,
														 right->m_length,
														 allocator);
}

StdString std_string_concatenate_stringviews_with_allocator(const StdStringView* restrict left,
															const StdStringView* restrict right,
															StdAllocator allocator) {
	return std_string_concatenate_cstrings_with_allocator(left->m_view,
														  left->m_length,
														  right->m_view,
														  right->m_length,
														  allocator);
}

void(std_string_fill)(StdString* restrict self, char character) {
	std_string_set_length(self, std_string_capacity(*self));
	std_memset(char, &std_string_at(*self, 0), character, std_string_length(*self));
}

void(std_string_clear)(StdString* restrict self) {
	std_string_fill(*self, 0);
	std_string_set_length(self, 0);
}

void(std_string_shrink_to_fit)(StdString* restrict self) {
	let_mut old = *self;
	*self = std_string_clone(old);
	std_string_free(old);
}

void(std_string_insert)(StdString* restrict self,
						const StdString* restrict to_insert,
						usize index) {
	let length = std_string_length(*self);
	std_assert(index <= length,
			   "std_string_insert called with index > self->m_length (index out of bounds)");

	if(index != 0) {
		let_mut std_string_scoped first = std_string_first(*self, index);

		let_mut left = std_string_concatenate(&first, to_insert);
		let_mut old = *self;
		if(index < length) {
			let_mut std_string_scoped second = std_string_last(*self, length - index);
			*self = std_string_concatenate(&left, &second);
			std_string_free(left);
		}
		else {
			*self = left;
		}
		std_string_free(old);
	}
	else {
		let new = std_string_concatenate(to_insert, self);
		let_mut old = *self;
		*self = new;
		std_string_free(old);
	}
}

void std_string_insert_cstring(StdString* restrict self,
							   restrict const_cstring to_insert,
							   // NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
							   usize to_insert_length,
							   usize index) {
	let length = std_string_length(*self);
	std_assert(index <= length,
			   "std_string_insert called with index > self->m_length (index out of bounds)");

	if(index != 0) {
		let_mut std_string_scoped first = std_string_first(*self, index);

		let_mut left = std_string_concatenate_cstring(&first, to_insert, to_insert_length);
		let_mut old = *self;
		if(index < length) {
			let_mut std_string_scoped second = std_string_last(*self, length - index);
			*self = std_string_concatenate(&left, &second);
			std_string_free(left);
		}
		else {
			*self = left;
		}
		std_string_free(old);
	}
	else {
		let_mut std_string_scoped left = std_string_from_cstring_with_allocator(to_insert,
																				to_insert_length,
																				self->m_allocator);
		let new = std_string_concatenate(&left, self);
		let_mut old = *self;
		*self = new;
		std_string_free(old);
	}
}

void std_string_insert_stringview(StdString* restrict self,
								  const StdStringView* restrict to_insert,
								  usize index) {
	std_string_insert_cstring(self, to_insert->m_view, to_insert->m_length, index);
}

void(std_string_erase)(StdString* restrict self, usize index) {
	let length = std_string_length(*self);
	std_assert(index < length,
			   "std_string_erase called with index >= self->m_length (index out of bounds)");

	if(index != length - 1) {
		let num_to_move = length - (index + 1);
		std_memmove(char,
					&std_string_at(*self, index),
					&std_string_at(*self, index + 1),
					num_to_move);
	}
	std_string_decrease_length(self, 1);
}

void(std_string_erase_n)(StdString* restrict self, usize index, usize num_characters) {
	let length = std_string_length(*self);
	std_assert(index < length,
			   "std_string_erase_n called with index >= self->m_length (index out of bounds)");
	std_assert(index + num_characters < length,
			   "std_string_erase_n called with"
			   " index + num_characters >= self->m_length (range out of bounds)");

	let end = index + num_characters;
	let num_to_move = length - end;

	if(end != length) {
		std_memmove(char, &std_string_at(*self, index), &std_string_at(*self, end), num_to_move);
	}
	else {
		std_memset(char, &std_string_at(*self, index), 0, num_characters);
	}
	std_string_decrease_length(self, num_characters);
}

void std_string_resize_internal(StdString* restrict self, usize new_size) {
	let length = std_string_length(*self);
	new_size += 1;
	if(new_size < length) {
		let num_to_erase = length - new_size;
		std_memset(char, &std_string_at(*self, new_size), 0, num_to_erase);
	}
	if(new_size > STD_STRING_SHORT_OPTIMIZATION_CAPACITY) {
		let_mut string = static_cast(cstring)(
			std_allocator_allocate_array_t(char, self->m_allocator, new_size).m_memory);
		let num_to_copy = std_min(length, new_size);
		std_memcpy(char, string, &std_string_at(*self, 0), num_to_copy);
		if(!std_string_is_short(self)) {
			let_mut ptr = self->m_long;
			self->m_long = nullptr;
			std_allocator_deallocate_array_t(char,
											 self->m_allocator,
											 ptr,
											 std_string_capacity(*self));
		}
		std_string_set_long(self);
		std_string_set_capacity(self, new_size - 1);
		std_string_set_length(self, num_to_copy);
		self->m_long = string;
	}
	else if(std_string_capacity(*self) != STD_STRING_SHORT_OPTIMIZATION_CAPACITY) {
		let capacity = STD_STRING_SHORT_OPTIMIZATION_CAPACITY + 1;
		let_mut string = static_cast(cstring)(
			std_allocator_allocate_array_t(char, self->m_allocator, capacity).m_memory);
		std_memcpy(char, string, self->m_long, capacity);
		std_allocator_deallocate(
			self->m_allocator,
			(StdMemory){.m_memory = self->m_long, .m_size_bytes = self->m_capacity});
		std_memcpy(char, self->m_short, string, capacity);
		std_allocator_deallocate(self->m_allocator,
								 (StdMemory){.m_memory = string, .m_size_bytes = capacity});
		std_string_set_short(self);
		std_string_set_length(self, capacity - 1);
		// std_string_set_capacity(self, capacity - 1);
	}
}

static inline always_inline usize std_string_get_expanded_capacity(usize old_capacity,
																   usize num_increments) {
	return num_increments * ((old_capacity * 3) / 2);
}

void(std_string_resize)(StdString* restrict self, usize new_size) {
	let length = std_string_length(*self);
	std_string_resize_internal(self, new_size);
	std_string_set_length(self, std_min(new_size, length));
}

void(std_string_reserve)(StdString* restrict self, usize new_capacity) {
	let capacity = std_string_capacity(*self);
	if(new_capacity > capacity) {
		let num_increments = 1 + (new_capacity / ((capacity * 3) / 2));
		let actual_new_capacity = std_string_get_expanded_capacity(capacity, num_increments);
		std_string_resize_internal(self, actual_new_capacity);
	}
}

void(std_string_push_back)(StdString* restrict self, char character) {
	let length = std_string_length(*self);
	let capacity = std_string_capacity(*self);
	if(length + 1 >= capacity) {
		let new_capacity = std_string_get_expanded_capacity(capacity, 1);
		std_string_resize_internal(self, new_capacity);
	}

	std_string_at(*self, length) = character;
	std_string_increase_length(self, 1);
}

void(std_string_push_front)(StdString* restrict self, char character) {
	let length = std_string_length(*self);
	let capacity = std_string_capacity(*self);
	if(length + 1 >= capacity) {
		let new_capacity = std_string_get_expanded_capacity(capacity, 1);
		std_string_resize_internal(self, new_capacity);
	}

	std_memmove(char, &std_string_at(*self, 1), &std_string_at(*self, 0), length);

	std_string_at(*self, 0U) = character;
	std_string_increase_length(self, 1);
}

StdOption(char)(std_string_pop_back)(StdString* restrict self) {
	let length = std_string_length(*self);
	if(length == 0) {
		return None(char);
	}

	let_mut character_ptr = &std_string_at(*self, length - 1);
	let character = Some(char, *character_ptr);
	*character_ptr = 0;
	std_string_decrease_length(self, 1);

	return character;
}

StdOption(char)(std_string_pop_front)(StdString* restrict self) {
	let length = std_string_length(*self);
	if(length == 0) {
		return None(char);
	}

	let character = Some(char, std_string_at(*self, 0U));
	std_memmove(char, &std_string_at(*self, 0), &std_string_at(*self, 1), length - 1);
	std_string_decrease_length(self, 1);

	return character;
}

void(std_string_append)(StdString* restrict self, const StdString* restrict to_append) {
	std_string_insert(*self, to_append, std_string_length(*self));
}

void std_string_append_cstring(StdString* restrict self,
							   restrict const_cstring to_append,
							   usize to_append_length) {
	std_string_insert_cstring(self, to_append, to_append_length, std_string_length(*self));
}

void std_string_append_stringview(StdString* restrict self,
								  const StdStringView* restrict to_append) {
	std_string_append_cstring(self, to_append->m_view, to_append->m_length);
}

void(std_string_prepend)(StdString* restrict self, const StdString* restrict to_prepend) {
	std_string_insert(*self, to_prepend, 0U);
}

void std_string_prepend_cstring(StdString* restrict self,
								restrict const_cstring to_prepend,
								usize to_prepend_length) {
	std_string_insert_cstring(self, to_prepend, to_prepend_length, 0U);
}

void std_string_prepend_stringview(StdString* restrict self,
								   const StdStringView* restrict to_prepend) {
	std_string_prepend_cstring(self, to_prepend->m_view, to_prepend->m_length);
}

void(std_string_replace)(StdString* restrict self,
						 const StdString* restrict to_replace_with,
						 usize index) {
	let length = std_string_length(*self);
	let replace_length = std_string_length(*to_replace_with);
	let stop = std_min(replace_length + index, length);

	std_memcpy(char,
			   &std_string_at(*self, index),
			   &std_string_at(*to_replace_with, 0),
			   stop - index);

	if(stop == length) {
		let begin = length - index;
		let num_remaining = replace_length - begin;
		let new_capacity = length + num_remaining;
		std_string_reserve(*self, new_capacity);
		std_memcpy(char,
				   &std_string_at(*self, length),
				   &std_string_at(*to_replace_with, begin),
				   num_remaining);
	}
}

void std_string_replace_cstring(StdString* restrict self,
								restrict const_cstring to_replace_with,
								usize to_replace_with_length,
								usize index) {
	let length = std_string_length(*self);
	let stop = std_min(to_replace_with_length + index, length);

	std_memcpy(char, &std_string_at(*self, index), to_replace_with, stop - index);

	if(stop == length) {
		let begin = length - index;
		let num_remaining = to_replace_with_length - begin;
		let new_capacity = length + num_remaining;
		std_string_reserve(*self, new_capacity);
		std_memcpy(char, &std_string_at(*self, length), &(to_replace_with[begin]), num_remaining);
	}
}

void std_string_replace_stringview(StdString* restrict self,
								   const StdStringView* restrict to_replace_with,
								   usize index) {
	std_string_replace_cstring(self, to_replace_with->m_view, to_replace_with->m_length, index);
}

StdStringIterator std_string_iterator_new(const StdString* restrict self) {
	return (StdStringIterator){.m_index = 0U, .m_string = const_cast(StdString*)(self)};
}

StdStringConstIterator std_string_const_iterator_new(const StdString* restrict self) {
	return (StdStringConstIterator){.m_index = 0U, .m_string = const_cast(StdString*)(self)};
}

char_ref std_string_iterator_next(StdRandomAccessIterator(char_ref) * restrict self) {
	let _self = static_cast(StdStringIterator*)(self->m_self);

	std_assert(_self->m_index > -1,
			   "Iterator value accessed when iterator is positioned before beginning of iteration "
			   "(iterator out of bounds)");
	std_assert(static_cast(usize)(_self->m_index) < std_string_length(*(_self->m_string)),
			   "Iterator value accessed when iterator is positioned after the end of the iteration "
			   "(iterator out of bounds");
	_self->m_index++;
	if(static_cast(usize)(_self->m_index) >= std_string_length(*(_self->m_string))) {
		return &std_string_at(*(_self->m_string), std_string_length(*(_self->m_string)) - 1);
	}
	return &std_string_at(*(_self->m_string), static_cast(usize)(_self->m_index));
}

char_ref std_string_iterator_previous(StdRandomAccessIterator(char_ref) * restrict self) {
	let _self = static_cast(StdStringIterator*)(self->m_self);

	std_assert(_self->m_index > -1,
			   "Iterator value accessed when iterator is positioned before beginning of iteration "
			   "(iterator out of bounds)");
	std_assert(static_cast(usize)(_self->m_index) < std_string_length(*(_self->m_string)),
			   "Iterator value accessed when iterator is positioned after the end of the iteration "
			   "(iterator out of bounds");
	_self->m_index--;
	if(_self->m_index < 0) {
		return &std_string_at(*(_self->m_string), 0);
	}
	return &std_string_at(*(_self->m_string), static_cast(usize)(_self->m_index));
}

char_ref
std_string_iterator_at(const StdRandomAccessIterator(char_ref) * restrict self, usize index) {
	let _self = static_cast(const StdStringIterator*)(self->m_self);

	std_assert(index < std_string_length(*(_self->m_string)),
			   "std_string_iterator_at called with index past the end of the iteration "
			   "(iterator index out of bounds");
	return &std_string_at(*(_self->m_string), index);
}

char_ref
std_string_iterator_rat(const StdRandomAccessIterator(char_ref) * restrict self, usize index) {
	let _self = static_cast(const StdStringIterator*)(self->m_self);

	std_assert(index < std_string_length(*(_self->m_string)),
			   "std_string_iterator_rat called with index past the end of the iteration "
			   "(iterator index out of bounds");
	return &std_string_at(*(_self->m_string), (std_string_length(*(_self->m_string)) - 1) - index);
}

char_ref std_string_iterator_current(const StdRandomAccessIterator(char_ref) * restrict self) {
	let _self = static_cast(const StdStringIterator*)(self->m_self);

	std_assert(_self->m_index > -1,
			   "Iterator value accessed when iterator is positioned before beginning of iteration "
			   "(iterator out of bounds)");
	std_assert(static_cast(usize)(_self->m_index) < std_string_length(*(_self->m_string)),
			   "Iterator value accessed when iterator is positioned after the end of the iteration "
			   "(iterator out of bounds");
	return &std_string_at(*(_self->m_string), static_cast(usize)(_self->m_index));
}

bool std_string_iterator_equals(const StdRandomAccessIterator(char_ref) * restrict self,
								const StdRandomAccessIterator(char_ref) * restrict rhs) {
	let _self = static_cast(const StdStringIterator*)(self->m_self);
	let _rhs = static_cast(const StdStringIterator*)(rhs->m_self);

	return _self->m_index == _rhs->m_index && _self->m_string == _rhs->m_string;
}

const_char_ref std_string_iterator_cnext(StdRandomAccessIterator(const_char_ref) * restrict self) {
	let _self = static_cast(StdStringConstIterator*)(self->m_self);

	std_assert(_self->m_index > -1,
			   "Iterator value accessed when iterator is positioned before beginning of iteration "
			   "(iterator out of bounds)");
	std_assert(static_cast(usize)(_self->m_index) < std_string_length(*(_self->m_string)),
			   "Iterator value accessed when iterator is positioned after the end of the iteration "
			   "(iterator out of bounds");
	_self->m_index++;
	if(static_cast(usize)(_self->m_index) >= std_string_length(*(_self->m_string))) {
		return &std_string_at(*(_self->m_string), std_string_length(*(_self->m_string)) - 1);
	}
	return &std_string_at(*(_self->m_string), static_cast(usize)(_self->m_index));
}

const_char_ref
std_string_iterator_cprevious(StdRandomAccessIterator(const_char_ref) * restrict self) {
	let _self = static_cast(StdStringConstIterator*)(self->m_self);

	std_assert(_self->m_index > -1,
			   "Iterator value accessed when iterator is positioned before beginning of iteration "
			   "(iterator out of bounds)");
	std_assert(static_cast(usize)(_self->m_index) < std_string_length(*(_self->m_string)),
			   "Iterator value accessed when iterator is positioned after the end of the iteration "
			   "(iterator out of bounds");
	_self->m_index--;
	if(_self->m_index < 0) {
		return &std_string_at(*(_self->m_string), 0);
	}
	return &std_string_at(*(_self->m_string), static_cast(usize)(_self->m_index));
}

const_char_ref
std_string_iterator_cat(const StdRandomAccessIterator(const_char_ref) * restrict self,
						usize index) {
	let _self = static_cast(const StdStringConstIterator*)(self->m_self);

	std_assert(index < std_string_length(*(_self->m_string)),
			   "std_string_iterator_cat called with index past the end of the iteration "
			   "(iterator index out of bounds");
	return &std_string_at(*(_self->m_string), index);
}

const_char_ref
std_string_iterator_crat(const StdRandomAccessIterator(const_char_ref) * restrict self,
						 usize index) {
	let _self = static_cast(const StdStringConstIterator*)(self->m_self);

	std_assert(index < std_string_length(*(_self->m_string)),
			   "std_string_iterator_crat called with index past the end of the iteration "
			   "(iterator index out of bounds");
	return &std_string_at(*(_self->m_string), (std_string_length(*(_self->m_string)) - 1) - index);
}

const_char_ref
std_string_iterator_ccurrent(const StdRandomAccessIterator(const_char_ref) * restrict self) {
	let _self = static_cast(const StdStringConstIterator*)(self->m_self);

	std_assert(_self->m_index > -1,
			   "Iterator value accessed when iterator is positioned before beginning of iteration "
			   "(iterator out of bounds)");
	std_assert(static_cast(usize)(_self->m_index) < std_string_length(*(_self->m_string)),
			   "Iterator value accessed when iterator is positioned after the end of the iteration "
			   "(iterator out of bounds");
	return &std_string_at(*(_self->m_string), static_cast(usize)(_self->m_index));
}

bool std_string_iterator_cequals(const StdRandomAccessIterator(const_char_ref) * restrict self,
								 const StdRandomAccessIterator(const_char_ref) * restrict rhs) {
	let _self = static_cast(const StdStringConstIterator*)(self->m_self);
	let _rhs = static_cast(const StdStringConstIterator*)(rhs->m_self);

	return _self->m_index == _rhs->m_index && _self->m_string == _rhs->m_string;
}

StdRandomAccessIterator(char_ref)(std_string_begin)(StdString* restrict self) {
	let_mut iter = std_string_into_iter(self);
	let_mut inner = static_cast(StdStringIterator*)(iter.m_self);
	inner->m_index = 0U;
	return iter;
}

StdRandomAccessIterator(char_ref)(std_string_end)(StdString* restrict self) {
	let_mut iter = std_string_into_iter(self);
	let_mut inner = static_cast(StdStringIterator*)(iter.m_self);
	inner->m_index = static_cast(isize)(std_string_length(*self));
	return iter;
}

StdRandomAccessIterator(char_ref)(std_string_rbegin)(StdString* restrict self) {
	let_mut iter = std_string_into_reverse_iter(self);
	let_mut inner = static_cast(StdStringIterator*)(iter.m_self);
	inner->m_index = static_cast(isize)(std_string_length(*self) - 1);
	return iter;
}

StdRandomAccessIterator(char_ref)(std_string_rend)(StdString* restrict self) {
	let_mut iter = std_string_into_reverse_iter(self);
	let_mut inner = static_cast(StdStringIterator*)(iter.m_self);
	inner->m_index = -1;
	return iter;
}

StdRandomAccessIterator(const_char_ref)(std_string_cbegin)(const StdString* restrict self) {
	let_mut iter = std_string_into_const_iter(self);
	let_mut inner = static_cast(StdStringConstIterator*)(iter.m_self);
	inner->m_index = 0U;
	return iter;
}

StdRandomAccessIterator(const_char_ref)(std_string_cend)(const StdString* restrict self) {
	let_mut iter = std_string_into_const_iter(self);
	let_mut inner = static_cast(StdStringConstIterator*)(iter.m_self);
	inner->m_index = static_cast(isize)(std_string_length(*self));
	return iter;
}

StdRandomAccessIterator(const_char_ref)(std_string_crbegin)(const StdString* restrict self) {
	let_mut iter = std_string_into_reverse_const_iter(self);
	let_mut inner = static_cast(StdStringConstIterator*)(iter.m_self);
	inner->m_index = static_cast(isize)(std_string_length(*self) - 1);
	return iter;
}

StdRandomAccessIterator(const_char_ref)(std_string_crend)(const StdString* restrict self) {
	let_mut iter = std_string_into_reverse_const_iter(self);
	let_mut inner = static_cast(StdStringConstIterator*)(iter.m_self);
	inner->m_index = -1;
	return iter;
}

StdStringView std_stringview_new(const StdString* restrict string) {
	return (StdStringView){.m_view = &std_string_at(*string, 0U),
						   .m_length = std_string_length(*string),
						   .m_vtable = &std_stringview_vtable};
}

StdStringView std_stringview_from(restrict const_cstring string, usize index, usize length) {
	std_assert(strlen(string) >= index + length,
			   "Can't create stringview with bounds outside the given cstring");
	return (StdStringView){.m_view = &(string[index]),
						   .m_length = length,
						   .m_vtable = &std_stringview_vtable};
}

const_char_ptr(std_stringview_at)(const StdStringView* restrict self, usize index) {
	std_assert(index < self->m_length,
			   "std_stringview_at called with index > length (index out of bounds)");
	return &(self->m_view[index]);
}

usize(std_stringview_length)(const StdStringView* restrict self) {
	return self->m_length;
}

usize(std_stringview_size)(const StdStringView* restrict self) {
	return self->m_length;
}

const_cstring(std_stringview_into_cstring)(const StdStringView* restrict self) {
	return self->m_view;
}

StdStringViewIterator std_stringview_iterator_new(const StdStringView* restrict self) {
	return (StdStringViewIterator){.m_index = 0U, .m_view = const_cast(StdStringView*)(self)};
}

const_char_ref
std_stringview_iterator_next(StdRandomAccessIterator(const_char_ref) * restrict self) {
	let _self = static_cast(StdStringViewIterator*)(self->m_self);

	let ret = std_stringview_at(*(_self->m_view), static_cast(usize)(_self->m_index));
	_self->m_index++;
	return ret;
}

const_char_ref
std_stringview_iterator_previous(StdRandomAccessIterator(const_char_ref) * restrict self) {
	let _self = static_cast(StdStringViewIterator*)(self->m_self);

	let ret = std_stringview_at(*(_self->m_view), static_cast(usize)(_self->m_index));
	_self->m_index--;
	return ret;
}

const_char_ref
std_stringview_iterator_at(const StdRandomAccessIterator(const_char_ref) * restrict self,
						   usize index) {
	let _self = static_cast(const StdStringViewIterator*)(self->m_self);

	return std_stringview_at(*(_self->m_view), index);
}

const_char_ref
std_stringview_iterator_rat(const StdRandomAccessIterator(const_char_ref) * restrict self,
							usize index) {
	let _self = static_cast(const StdStringViewIterator*)(self->m_self);

	return std_stringview_at(*(_self->m_view),
							 (std_stringview_length(*(_self->m_view)) - 1) - index);
}

const_char_ref
std_stringview_iterator_current(const StdRandomAccessIterator(const_char_ref) * restrict self) {
	let _self = static_cast(const StdStringViewIterator*)(self->m_self);

	return std_stringview_at(*(_self->m_view), static_cast(usize)(_self->m_index));
}

bool std_stringview_iterator_equals(const StdRandomAccessIterator(const_char_ref) * restrict self,
									const StdRandomAccessIterator(const_char_ref) * restrict rhs) {
	let _self = static_cast(const StdStringViewIterator*)(self->m_self);
	let _rhs = static_cast(const StdStringViewIterator*)(rhs->m_self);

	return _self->m_view == _rhs->m_view && _self->m_index == _rhs->m_index;
}

StdRandomAccessIterator(const_char_ref)(std_stringview_begin)(const StdStringView* restrict self) {
	let_mut iter = std_stringview_into_iter(self);
	let_mut inner = static_cast(StdStringViewIterator*)(iter.m_self);
	inner->m_index = 0U;
	return iter;
}

StdRandomAccessIterator(const_char_ref)(std_stringview_end)(const StdStringView* restrict self) {
	let_mut iter = std_stringview_into_iter(self);
	let_mut inner = static_cast(StdStringViewIterator*)(iter.m_self);
	inner->m_index = static_cast(isize)(self->m_length);
	return iter;
}

StdRandomAccessIterator(const_char_ref)(std_stringview_rbegin)(const StdStringView* restrict self) {
	let_mut iter = std_stringview_into_reverse_iter(self);
	let_mut inner = static_cast(StdStringViewIterator*)(iter.m_self);
	inner->m_index = static_cast(isize)(self->m_length - 1);
	return iter;
}

StdRandomAccessIterator(const_char_ref)(std_stringview_rend)(const StdStringView* restrict self) {
	let_mut iter = std_stringview_into_reverse_iter(self);
	let_mut inner = static_cast(StdStringViewIterator*)(iter.m_self);
	inner->m_index = -1;
	return iter;
}
