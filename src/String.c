/// @file String.c
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief This module provides string and stringview types comparable to C++ for Cnx
/// @version 0.1.5
/// @date 2022-04-30
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

#include <Cnx/String.h>
#include <memory.h>
//#include <uchar.h>
#include <Cnx/Assert.h>
#include <Cnx/Math.h>
#include <Cnx/Platform.h>
#include <wchar.h>

#define OPTION_UNDEF_PARAMS TRUE

/// @brief Declares `CnxOption(T)` for `CnxString`
#define OPTION_T	CnxString
#define OPTION_IMPL TRUE
#include <Cnx/Option.h> // NOLINT(readability-duplicate-include)

/// @brief Declares `CnxOption(T)` for `Ref(CnxString)`
#define OPTION_T	Ref(CnxString)
#define OPTION_IMPL TRUE
#include <Cnx/Option.h> // NOLINT(readability-duplicate-include)

/// @brief Declares `CnxOption(T)` for `ConstRef(CnxString)`
#define OPTION_T	ConstRef(CnxString)
#define OPTION_IMPL TRUE
#include <Cnx/Option.h> // NOLINT(readability-duplicate-include)

/// @brief Declares `CnxOption(T)` for `CnxStringView`
#define OPTION_T	CnxStringView
#define OPTION_IMPL TRUE
#include <Cnx/Option.h> // NOLINT(readability-duplicate-include)

/// @brief Declares `CnxOption(T)` for `Ref(CnxStringView)`
#define OPTION_T	Ref(CnxStringView)
#define OPTION_IMPL TRUE
#include <Cnx/Option.h> // NOLINT(readability-duplicate-include)

/// @brief Declares `CnxOption(T)` for `ConstRef(CnxStringView)`
#define OPTION_T	ConstRef(CnxStringView)
#define OPTION_IMPL TRUE
#include <Cnx/Option.h> // NOLINT(readability-duplicate-include)

#undef OPTION_UNDEF_PARAMS

static const cnx_stringview_vtable_t cnx_stringview_vtable = {
	.begin = cnx_stringview_begin,
	.end = cnx_stringview_end,
	.rbegin = cnx_stringview_rbegin,
	.rend = cnx_stringview_rend,
	.cbegin = cnx_stringview_begin,
	.cend = cnx_stringview_end,
	.crbegin = cnx_stringview_rbegin,
	.crend = cnx_stringview_rend,
};

static const cnx_string_vtable_t cnx_string_vtable = {
	.begin = cnx_string_begin,
	.end = cnx_string_end,
	.rbegin = cnx_string_rbegin,
	.rend = cnx_string_rend,
	.cbegin = cnx_string_cbegin,
	.cend = cnx_string_cend,
	.crbegin = cnx_string_crbegin,
	.crend = cnx_string_crend,
};

/// @brief Creates a default `CnxStringIterator` (one pointing to the beginning of the string)
///
/// @param self - The `CnxString` to get an iterator for
///
/// @return a `CnxStringIterator` into `self`
CnxStringIterator cnx_string_iterator_new(const CnxString* restrict self);
/// @brief Creates a default `CnxStringConstIterator` (one pointing to the beginning of the string)
///
/// @param self - The `CnxString` to get an iterator for
///
/// @return a `CnxStringConstIterator` into `self`
CnxStringConstIterator cnx_string_const_iterator_new(const CnxString* restrict self);
/// @brief Returns a reference to the current value the iterator points to, then increments the
/// iterator
///
/// @param self - The iterator to increment
///
/// @note if the iterator is at the end of the iteration, the result of calling this is undefined
char_ref cnx_string_iterator_next(CnxRandomAccessIterator(char_ref) * restrict self);
/// @brief Returns a reference to the current value the iterator points to, then decrements the
/// iterator
///
/// @param self - The iterator to decrement
///
/// @note if the iterator is past the beginning of the iteration, the result of calling this is
/// undefined
char_ref cnx_string_iterator_previous(CnxRandomAccessIterator(char_ref) * restrict self);
/// @brief Returns a reference to the value associated with the given index into the iteration
///
/// @param self - The iterator to get the value from
/// @param index - The index associated with the value
///
/// @note if the index is past the beginning or end of the iteration, the result of calling this is
/// undefined
char_ref
cnx_string_iterator_at(const CnxRandomAccessIterator(char_ref) * restrict self, usize index);
/// @brief Returns a reference to the value associated with the given index into the iteration
///
/// @param self - The iterator to get the value from
/// @param index - The index associated with the value
///
/// @note if the index is past the beginning or end of the iteration, the result of calling this is
/// undefined
char_ref
cnx_string_iterator_rat(const CnxRandomAccessIterator(char_ref) * restrict self, usize index);
/// @brief Returns a reference to the current value the iterator points to
///
/// @param self - The iterator to get the current value of
///
/// @return the current value the iterator points to
///
/// @note if this iterator is past the beginning or end of the iteration, the result of calling this
/// is undefined
char_ref cnx_string_iterator_current(const CnxRandomAccessIterator(char_ref) * restrict self);
/// @brief Returns whether the two iterators are equal (they belong to the same container and are at
/// the same index in the iteration)
///
/// @param self - This iterator
/// @param rhs - The iterator to compare to
///
/// @return whether the iterators are equal
bool cnx_string_iterator_equals(const CnxRandomAccessIterator(char_ref) * restrict self,
								const CnxRandomAccessIterator(char_ref) * restrict rhs);

/// @brief Returns a reference to the current value the iterator points to, then increments the
/// iterator
///
/// @param self - The iterator to increment
///
/// @note if the iterator is at the end of the iteration, the result of calling this is undefined
const_char_ref cnx_string_iterator_cnext(CnxRandomAccessIterator(const_char_ref) * restrict self);
/// @brief Returns a reference to the current value the iterator points to, then decrements the
/// iterator
///
/// @param self - The iterator to decrement
///
/// @note if the iterator is past the beginning of the iteration, the result of calling this is
/// undefined
const_char_ref
cnx_string_iterator_cprevious(CnxRandomAccessIterator(const_char_ref) * restrict self);
/// @brief Returns a reference to the value associated with the given index into the iteration
///
/// @param self - The iterator to get the value from
/// @param index - The index associated with the value
///
/// @note if the index is past the beginning or end of the iteration, the result of calling this is
/// undefined
const_char_ref
cnx_string_iterator_cat(const CnxRandomAccessIterator(const_char_ref) * restrict self, usize index);
/// @brief Returns a reference to the value associated with the given index into the iteration
///
/// @param self - The iterator to get the value from
/// @param index - The index associated with the value
///
/// @note if the index is past the beginning or end of the iteration, the result of calling this is
/// undefined
const_char_ref
cnx_string_iterator_crat(const CnxRandomAccessIterator(const_char_ref) * restrict self,
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
cnx_string_iterator_ccurrent(const CnxRandomAccessIterator(const_char_ref) * restrict self);
/// @brief Returns whether the two iterators are equal (they belong to the same container and are at
/// the same index in the iteration)
///
/// @param self - This iterator
/// @param rhs - The iterator to compare to
///
/// @return whether the iterators are equal
bool cnx_string_iterator_cequals(const CnxRandomAccessIterator(const_char_ref) * restrict self,
								 const CnxRandomAccessIterator(const_char_ref) * restrict rhs);

/// @brief Creates a default `CnxStringViewIterator` (one pointing to the beginning of the view),
/// allocated by the given allocated
///
/// @param self - The `CnxStringView` to get an iterator for
///
/// @return a `CnxStringViewIterator` into the stringview
CnxStringViewIterator cnx_stringview_iterator_new(const CnxStringView* restrict self);
/// @brief Returns a reference to the current value the iterator points to, then increments the
/// iterator
///
/// @param self - The iterator to increment
///
/// @note if the iterator is at the end of the iteration, the result of calling this is undefined
const_char_ref
cnx_stringview_iterator_next(CnxRandomAccessIterator(const_char_ref) * restrict self);
/// @brief Returns a reference to the current value the iterator points to, then decrements the
/// iterator
///
/// @param self - The iterator to decrement
///
/// @note if the iterator is past the beginning of the iteration, the result of calling this is
/// undefined
const_char_ref
cnx_stringview_iterator_previous(CnxRandomAccessIterator(const_char_ref) * restrict self);
/// @brief Returns a reference to the value associated with the given index into the iteration
///
/// @param self - The iterator to get the value from
/// @param index - The index associated with the value
///
/// @note if the index is past the beginning or end of the iteration, the result of calling this is
/// undefined
const_char_ref
cnx_stringview_iterator_at(const CnxRandomAccessIterator(const_char_ref) * restrict self,
						   usize index);
/// @brief Returns a reference to the value associated with the given index into the iteration
///
/// @param self - The iterator to get the value from
/// @param index - The index associated with the value
///
/// @note if the index is past the beginning or end of the iteration, the result of calling this is
/// undefined
const_char_ref
cnx_stringview_iterator_rat(const CnxRandomAccessIterator(const_char_ref) * restrict self,
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
cnx_stringview_iterator_current(const CnxRandomAccessIterator(const_char_ref) * restrict self);
/// @brief Returns whether the two iterators are equal (they belong to the same container and are at
/// the same index in the iteration)
///
/// @param self - This iterator
/// @param rhs - The iterator to compare to
///
/// @return whether the iterators are equal
bool cnx_stringview_iterator_equals(const CnxRandomAccessIterator(const_char_ref) * restrict self,
									const CnxRandomAccessIterator(const_char_ref) * restrict rhs);

ImplIntoCnxRandomAccessIterator(CnxString,
								char_ref,
								cnx_string_into_iter,
								into,
								cnx_string_iterator_new,
								cnx_string_iterator_next,
								cnx_string_iterator_previous,
								cnx_string_iterator_at,
								cnx_string_iterator_current,
								cnx_string_iterator_equals);
ImplIntoCnxRandomAccessIterator(CnxString,
								char_ref,
								cnx_string_into_reverse_iter,
								into_reverse,
								cnx_string_iterator_new,
								cnx_string_iterator_next,
								cnx_string_iterator_previous,
								cnx_string_iterator_rat,
								cnx_string_iterator_current,
								cnx_string_iterator_equals);

ImplIntoCnxRandomAccessIterator(CnxString,
								const_char_ref,
								cnx_string_into_const_iter,
								into,
								cnx_string_const_iterator_new,
								cnx_string_iterator_cnext,
								cnx_string_iterator_cprevious,
								cnx_string_iterator_cat,
								cnx_string_iterator_ccurrent,
								cnx_string_iterator_cequals);
ImplIntoCnxRandomAccessIterator(CnxString,
								const_char_ref,
								cnx_string_into_reverse_const_iter,
								into_reverse,
								cnx_string_const_iterator_new,
								cnx_string_iterator_cnext,
								cnx_string_iterator_cprevious,
								cnx_string_iterator_crat,
								cnx_string_iterator_ccurrent,
								cnx_string_iterator_cequals);

ImplIntoCnxRandomAccessIterator(CnxStringView,
								const_char_ref,
								cnx_stringview_into_iter,
								into,
								cnx_stringview_iterator_new,
								cnx_stringview_iterator_next,
								cnx_stringview_iterator_previous,
								cnx_stringview_iterator_at,
								cnx_stringview_iterator_current,
								cnx_stringview_iterator_equals);
ImplIntoCnxRandomAccessIterator(CnxStringView,
								const_char_ref,
								cnx_stringview_into_reverse_iter,
								into_reverse,
								cnx_stringview_iterator_new,
								cnx_stringview_iterator_next,
								cnx_stringview_iterator_previous,
								cnx_stringview_iterator_rat,
								cnx_stringview_iterator_current,
								cnx_stringview_iterator_equals);

static const bool cnx_string_is_little_endian
	= CNX_PLATFORM_LITTLE_ENDIAN || !CNX_PLATFORM_BIG_ENDIAN;
static const usize cnx_string_long_mask
	= cnx_string_is_little_endian ? static_cast(usize)(0x40U) : static_cast(usize)(0x01U);
static const usize cnx_string_capacity_mode_shift
	= cnx_string_is_little_endian ? (sizeof(usize) - 1) * 8U : 0U;
static const usize cnx_string_len_cap_shift = cnx_string_is_little_endian ? 0U : 1U;

__attr(always_inline) __attr(nodiscard) __attr(not_null(1))
	__attr(returns_not_null) static inline u8* cnx_string_length_remaining(
		const CnxString* restrict self) {
	return static_cast(u8*)(&(self->m_short[CNX_STRING_SHORT_OPTIMIZATION_CAPACITY]));
}

__attr(always_inline) __attr(nodiscard)
	__attr(not_null(1)) static inline bool cnx_string_is_short(const CnxString* restrict self) {
	return !((self->m_capacity >> cnx_string_capacity_mode_shift) // NOLINT
			 & cnx_string_long_mask);
}

__attr(always_inline)
	__attr(not_null(1)) static inline void cnx_string_set_long(CnxString* restrict self) {
	self->m_capacity |= (cnx_string_long_mask << cnx_string_capacity_mode_shift);
}

__attr(always_inline)
	__attr(not_null(1)) static inline void cnx_string_set_short(CnxString* restrict self) {
	self->m_capacity
		&= ~(static_cast(usize)(cnx_string_long_mask << cnx_string_capacity_mode_shift));
}

__attr(always_inline) __attr(nodiscard) __attr(not_null(1)) static inline usize
	cnx_string_get_capacity(const CnxString* restrict self) {
	return cnx_string_is_short(self) ?
			   CNX_STRING_SHORT_OPTIMIZATION_CAPACITY :
			   (self->m_capacity
				& (~(cnx_string_long_mask << cnx_string_capacity_mode_shift))
					  >> cnx_string_len_cap_shift);
}

__attr(always_inline)
	__attr(not_null(1)) static inline void cnx_string_set_capacity(CnxString* restrict self,
																   usize new_capacity) {
	cnx_assert(!cnx_string_is_short(self), "Can't set capacity on a short optimized string");
	self->m_capacity = (new_capacity << cnx_string_len_cap_shift)
					   | (cnx_string_long_mask << cnx_string_capacity_mode_shift);
}

__attr(always_inline) __attr(nodiscard) __attr(not_null(1)) static inline usize
	cnx_string_get_short_length(const CnxString* restrict self) {
	cnx_assert(cnx_string_is_short(self), "Can't get short length of a long string");
	return CNX_STRING_SHORT_OPTIMIZATION_CAPACITY
		   - (static_cast(usize)(*(cnx_string_length_remaining(self))) >> cnx_string_len_cap_shift);
}

__attr(always_inline)
	__attr(not_null(1)) static inline void cnx_string_set_length(CnxString* restrict self,
																 usize new_length) {
	cnx_assert(new_length <= cnx_string_capacity(*self),
			   "Can't set string length longer than capacity");
	if(cnx_string_is_short(self)) {
		*(cnx_string_length_remaining(self)) = static_cast(u8)(
			((CNX_STRING_SHORT_OPTIMIZATION_CAPACITY - new_length) << cnx_string_len_cap_shift));
	}
	else {
		self->m_length = new_length;
	}
}

__attr(always_inline)
	__attr(not_null(1)) static inline void cnx_string_increase_length(CnxString* restrict self,
																	  usize amount_to_increase) {
	cnx_string_set_length(self, cnx_string_length(*self) + amount_to_increase);
}

__attr(always_inline)
	__attr(not_null(1)) static inline void cnx_string_decrease_length(CnxString* restrict self,
																	  usize amount_to_decrease) {
	cnx_assert(amount_to_decrease <= cnx_string_length(*self),
			   "Can't decrease string length by more than length");
	cnx_string_set_length(self, cnx_string_length(*self) - amount_to_decrease);
}

CnxString cnx_string_new(void) {
	return cnx_string_new_with_allocator(DEFAULT_ALLOCATOR);
}

CnxString cnx_string_new_with_allocator(CnxAllocator allocator) {
	let_mut str = (CnxString){
		.m_short = {[CNX_STRING_SHORT_OPTIMIZATION_CAPACITY]
					= CNX_STRING_SHORT_OPTIMIZATION_CAPACITY << cnx_string_len_cap_shift},
		.m_allocator = allocator,
		.m_vtable = &cnx_string_vtable};
	// cnx_string_set_length(&str, 0);
	return str;
}

CnxString cnx_string_new_with_capacity(usize capacity) {
	return cnx_string_new_with_capacity_with_allocator(capacity, DEFAULT_ALLOCATOR);
}

CnxString cnx_string_new_with_capacity_with_allocator(usize capacity, CnxAllocator allocator) {
	let_mut string = cnx_string_new_with_allocator(allocator);
	if(capacity > CNX_STRING_SHORT_OPTIMIZATION_CAPACITY) {
		cnx_string_set_long(&string);
		cnx_string_set_capacity(&string, capacity);
		cnx_string_set_length(&string, 0);
		string.m_long = cnx_allocator_allocate_array_t(char, string.m_allocator, capacity + 1);
	}

	return string;
}

CnxString cnx_string_from_cstring(restrict const_cstring string, usize length) {
	return cnx_string_from_cstring_with_allocator(string, length, DEFAULT_ALLOCATOR);
}

CnxString cnx_string_from_cstring_with_allocator(restrict const_cstring string,
												 usize length,
												 CnxAllocator allocator) {
	let_mut cnx_string = cnx_string_new_with_capacity_with_allocator(length, allocator);
	cnx_string_set_length(&cnx_string, length);
	cnx_memcpy(char, &cnx_string_at(cnx_string, 0), string, length);
	return cnx_string;
}

CnxString cnx_string_from_wcstring(restrict const_wcstring string, usize length) {
	return cnx_string_from_wcstring_with_allocator(string, length, DEFAULT_ALLOCATOR);
}

CnxString cnx_string_from_wcstring_with_allocator(restrict const_wcstring string,
												  usize length,
												  CnxAllocator allocator) {
#if CNX_PLATFORM_COMPILER_GCC
	_Pragma("GCC diagnostic push") _Pragma("GCC diagnostic ignored \"-Wformat-truncation\"")
#endif // CNX_PLATFORM_COMPILER_GCC
		let cstring_length
		= static_cast(usize)(snprintf(nullptr, length, "%ls", string));
#if CNX_PLATFORM_COMPILER_GCC
	_Pragma("GCC diagnostic pop")
#endif // CNX_PLATFORM_COMPILER_GCC

		let_mut cnx_string
		= cnx_string_new_with_capacity_with_allocator(cstring_length, allocator);
	cnx_string_set_length(&cnx_string, cstring_length);
	let_mut str = &(cnx_string_at(cnx_string, 0));
	ignore(snprintf(str, length, "%ls", string));
	return cnx_string;
}

CnxString cnx_string_from_stringview(const CnxStringView* restrict view) {
	return cnx_string_from_stringview_with_allocator(view, DEFAULT_ALLOCATOR);
}

CnxString cnx_string_from_stringview_with_allocator(const CnxStringView* restrict view,
													CnxAllocator allocator) {
	return cnx_string_from_cstring_with_allocator(view->m_view, view->m_length, allocator);
}

const_cstring(cnx_string_into_cstring)(const CnxString* restrict self) {
	return cnx_string_is_short(self) ? &(self->m_short[0]) : self->m_long;
}

const_wcstring(cnx_string_into_wcstring)(const CnxString* restrict self) {
	return cnx_string_into_wcstring_with_allocator(*self, self->m_allocator);
}

const_wcstring(cnx_string_into_wcstring_with_allocator)(const CnxString* restrict self,
														CnxAllocator allocator) {
	let_mut string = cnx_string_into_cstring(*self);
	let length = static_cast(usize)(swprintf(nullptr, cnx_string_length(*self), L"%s", string));
	let_mut wstring = cnx_allocator_allocate_array_t(wchar_t, allocator, length);
	ignore(swprintf(wstring, cnx_string_length(*self), L"%s", string));
	return wstring;
}

CnxStringView(cnx_string_into_stringview)(const CnxString* restrict self) {
	return (CnxStringView){.m_view = cnx_string_into_cstring(*self),
						   .m_length = cnx_string_length(*self),
						   .m_vtable = &cnx_stringview_vtable};
}

CnxString(cnx_string_clone)(const CnxString* restrict self) {
	return cnx_string_clone_with_allocator(*self, self->m_allocator);
}

CnxString(cnx_string_clone_with_allocator)(const CnxString* restrict self, CnxAllocator allocator) {
	let length = cnx_string_length(*self);
	let_mut string = cnx_string_new_with_capacity_with_allocator(length, allocator);
	cnx_string_set_length(&string, length);
	cnx_memcpy(char, &cnx_string_at(string, 0), &cnx_string_at(*self, 0), length);

	return string;
}

void(cnx_string_free)(void* restrict self) {
	let self_ptr = static_cast(CnxString*)(self);
	if(!cnx_string_is_short(self_ptr)) {
		let ptr = self_ptr->m_long;
		cnx_string_set_short(self_ptr);
		self_ptr->m_long = nullptr;
		cnx_allocator_deallocate(self_ptr->m_allocator, ptr);
	}
}

char_ptr cnx_string_at_mut(CnxString* restrict self, usize index) {
	cnx_assert(index <= cnx_string_length(*self),
			   "cnx_string_at called with index > self->m_length (index out of bounds)");
	cnx_assert(index < cnx_string_capacity(*self),
			   "cnx_string_at called with index >= self->m_capacity (index out of bounds)");
	return cnx_string_is_short(self) ? &(self->m_short[index]) : &(self->m_long[index]);
}

const_char_ptr cnx_string_at_const(const CnxString* restrict self, usize index) {
	cnx_assert(index <= cnx_string_length(*self),
			   "cnx_string_at called with index > self->m_length (index out of bounds)");
	cnx_assert(index < cnx_string_capacity(*self),
			   "cnx_string_at called with index >= self->m_capacity (index out of bounds)");
	return cnx_string_is_short(self) ? &(self->m_short[index]) : &(self->m_long[index]);
}

char_ptr cnx_string_front_mut(CnxString* restrict self) {
	return &cnx_string_at(*self, 0);
}

const_char_ptr cnx_string_front_const(const CnxString* restrict self) {
	return &cnx_string_at(*self, 0);
}

char_ptr cnx_string_back_mut(CnxString* restrict self) {
	let length = cnx_string_length(*self);
	let back_index = length > 0 ? length - 1 : 0U;
	return &cnx_string_at(*self, back_index);
}

const_char_ptr cnx_string_back_const(const CnxString* restrict self) {
	let length = cnx_string_length(*self);
	let back_index = length > 0 ? length - 1 : 0U;
	return &cnx_string_at(*self, back_index);
}

bool(cnx_string_is_empty)(const CnxString* restrict self) {
	return cnx_string_size(*self) == 0;
}

bool(cnx_string_is_full)(const CnxString* restrict self) {
	return cnx_string_size(*self) == cnx_string_capacity(*self);
}

usize(cnx_string_size)(const CnxString* restrict self) {
	return cnx_string_is_short(self) ? cnx_string_get_short_length(self) : self->m_length;
}

usize(cnx_string_length)(const CnxString* restrict self) {
	return cnx_string_size(*self);
}

usize cnx_string_max_size(void) {
	return cnx_max_value(usize) - 2;
}

usize(cnx_string_capacity)(const CnxString* restrict self) {
	return cnx_string_get_capacity(self);
}

CnxString(cnx_string_first)(const CnxString* restrict self, usize num_chars) {
	let num_to_copy = cnx_min(num_chars, cnx_string_length(*self));
	let_mut string = cnx_string_new_with_capacity_with_allocator(num_chars, self->m_allocator);
	cnx_memcpy(char, &cnx_string_at(string, 0), &cnx_string_at(*self, 0), num_to_copy);
	cnx_string_set_length(&string, num_to_copy);

	return string;
}

cstring(cnx_string_first_cstring)(const CnxString* restrict self, usize num_chars) {
	let num_to_copy = cnx_min(num_chars, cnx_string_length(*self));
	let_mut string = cnx_allocator_allocate_array_t(char, self->m_allocator, num_chars + 1);

	cnx_memcpy(char, string, &cnx_string_at(*self, 0), num_to_copy);

	return string;
}

CnxStringView(cnx_string_first_stringview)(const CnxString* restrict self, usize num_chars) {
	let length = cnx_min(num_chars, cnx_string_length(*self));

	return (CnxStringView){.m_view = &cnx_string_at(*self, 0), .m_length = length};
}

CnxString(cnx_string_last)(const CnxString* restrict self, usize num_chars) {
	let length = cnx_string_length(*self);
	let num_to_copy = cnx_min(num_chars, length);
	let start_index = num_chars >= length ? 0 : (length - num_chars);
	let_mut string = cnx_string_new_with_capacity_with_allocator(num_chars, self->m_allocator);
	cnx_memcpy(char, &cnx_string_at(string, 0), &cnx_string_at(*self, start_index), num_to_copy);
	cnx_string_set_length(&string, num_to_copy);

	return string;
}

cstring(cnx_string_last_cstring)(const CnxString* restrict self, usize num_chars) {
	let length = cnx_string_length(*self);
	let num_to_copy = cnx_min(num_chars, length);
	let start_index = num_chars >= length ? 0 : (length - num_chars);
	let_mut string = cnx_allocator_allocate_array_t(char, self->m_allocator, num_chars + 1);

	cnx_memcpy(char, string, &cnx_string_at(*self, start_index), num_to_copy);

	return string;
}

CnxStringView(cnx_string_last_stringview)(const CnxString* restrict self, usize num_chars) {
	let self_length = cnx_string_length(*self);
	let begin = num_chars >= self_length ? 0U : self_length - num_chars;
	let length = cnx_min(num_chars, self_length);

	return (CnxStringView){.m_view = &cnx_string_at(*self, begin), .m_length = length};
}

bool(cnx_string_equal)(const CnxString* restrict self, const CnxString* restrict to_compare) {
	let length = cnx_string_length(*self);
	if(length != cnx_string_length(*to_compare)) {
		return false;
	}

	return 0 == memcmp(&cnx_string_at(*self, 0), &cnx_string_at(*to_compare, 0), length);
}

bool cnx_string_equal_cstring(const CnxString* restrict self,
							  restrict const_cstring to_compare,
							  usize length) {
	let self_length = cnx_string_length(*self);
	if(self_length != length) {
		return false;
	}

	return 0 == memcmp(&cnx_string_at(*self, 0), to_compare, self_length);
}

bool cnx_string_equal_stringview(const CnxString* restrict self,
								 const CnxStringView* restrict to_compare) {
	return cnx_string_equal_cstring(self, to_compare->m_view, to_compare->m_length);
}

/// @brief Determines if the substring of `self` beginning at `index` is the same as the given
/// `substring`
__attr(always_inline) __attr(nodiscard) __attr(not_null(1, 2)) static inline CnxOption(usize)
	cnx_string_contains_from_left(const CnxString* restrict self,
								  const CnxString* restrict substring,
								  usize index) {
	let sublength = cnx_string_length(*substring);
	cnx_assert(index + sublength < cnx_string_length(*self),
			   "cnx_string_contains_from_left called with index + substring->m_length >= "
			   "self->m_length (indices are out of bounds)");

	if(0 == memcmp(&cnx_string_at(*self, index), &cnx_string_at(*substring, 0), sublength)) {
		return Some(usize, index);
	}
	else {
		return None(usize);
	}
}

/// @brief Determines if the substring of `self` ending with `index` (inclusive)is the same as
/// the given `substring`
__attr(always_inline) __attr(nodiscard) __attr(not_null(1, 2)) static inline CnxOption(usize)
	cnx_string_contains_from_right(const CnxString* restrict self,
								   const CnxString* restrict substring,
								   usize index) {

	let sublength = cnx_string_length(*substring);
	cnx_assert(index >= sublength,
			   "cnx_string_contains_from_right called with index < substring->m_length (index out "
			   "of bounds)");

	let start_index = index - (sublength - 1);
	if(0 == memcmp(&cnx_string_at(*self, start_index), &cnx_string_at(*substring, 0), sublength)) {
		return Some(usize, start_index);
	}
	else {
		return None(usize);
	}
}

bool(cnx_string_contains)(const CnxString* restrict self, const CnxString* restrict substring) {
	let length = cnx_string_length(*self);
	let sublength = cnx_string_length(*substring);
	if(length < sublength) {
		return false;
	}

	for(usize left = static_cast(usize)(0), right = length - 1;
		right >= sublength && left + sublength < length;
		++left, --right)
	{

		let contains_left = cnx_string_contains_from_left(self, substring, left);
		if(cnx_option_is_some(contains_left)) {
			return true;
		}

		let contains_right = cnx_string_contains_from_right(self, substring, right);
		if(cnx_option_is_some(contains_right)) {
			return true;
		}
	}

	return false;
}

/// @brief Determines if the substring of `self` beginning at `index` is the same as the given
/// `substring`
__attr(always_inline) __attr(nodiscard) __attr(not_null(1, 2)) static inline CnxOption(usize)
	cnx_string_contains_cstring_from_left(const CnxString* restrict self,
										  restrict const_cstring substring,
										  usize substring_length,
										  usize index) {
	cnx_assert(index + substring_length < cnx_string_length(*self),
			   "cnx_string_contains_cstring_from_left called with index + substring->m_length >= "
			   "self->m_length (indices are out of bounds)");

	if(0 == memcmp(&cnx_string_at(*self, index), substring, substring_length)) {
		return Some(usize, index);
	}
	else {
		return None(usize);
	}
}

/// @brief Determines if the substring of `self` ending with `index` (inclusive)is the same as
/// the given `substring`
__attr(always_inline) __attr(nodiscard) __attr(not_null(1, 2)) static inline CnxOption(usize)
	cnx_string_contains_cstring_from_right(const CnxString* restrict self,
										   restrict const_cstring substring,
										   usize substring_length,
										   usize index) {
	cnx_assert(
		index >= substring_length,
		"cnx_string_contains_cstring_from_right called with index < substring->m_length (index out "
		"of bounds)");

	let start_index = index - (substring_length - 1);
	if(0 == memcmp(&cnx_string_at(*self, start_index), substring, substring_length)) {
		return Some(usize, start_index);
	}
	else {
		return None(usize);
	}
}

bool cnx_string_contains_cstring(const CnxString* restrict self,
								 restrict const_cstring substring,
								 usize substring_length) {
	let length = cnx_string_length(*self);
	if(length < substring_length) {
		return false;
	}
	for(usize left = static_cast(usize)(0), right = length - 1;
		right >= substring_length && left + substring_length < length;
		++left, --right)
	{

		let contains_left
			= cnx_string_contains_cstring_from_left(self, substring, substring_length, left);
		if(cnx_option_is_some(contains_left)) {
			return true;
		}

		let contains_right
			= cnx_string_contains_cstring_from_right(self, substring, substring_length, right);
		if(cnx_option_is_some(contains_right)) {
			return true;
		}
	}

	return false;
}

bool cnx_string_contains_stringview(const CnxString* restrict self,
									const CnxStringView* restrict substring) {
	return cnx_string_contains_cstring(self, substring->m_view, substring->m_length);
}

bool(cnx_string_starts_with)(const CnxString* restrict self, const CnxString* restrict substring) {
	let starts_with = cnx_string_contains_from_left(self, substring, 0);
	return cnx_option_is_some(starts_with);
}

bool cnx_string_starts_with_cstring(const CnxString* restrict self,
									restrict const_cstring substring,
									usize substring_length) {
	let starts_with = cnx_string_contains_cstring_from_left(self, substring, substring_length, 0);
	return cnx_option_is_some(starts_with);
}

bool cnx_string_starts_with_stringview(const CnxString* restrict self,
									   const CnxStringView* restrict substring) {
	return cnx_string_starts_with_cstring(self, substring->m_view, substring->m_length);
}

bool(cnx_string_ends_with)(const CnxString* restrict self, const CnxString* restrict substring) {
	let ends_with = cnx_string_contains_from_right(self, substring, cnx_string_length(*self) - 1);
	return cnx_option_is_some(ends_with);
}

bool cnx_string_ends_with_cstring(const CnxString* restrict self,
								  restrict const_cstring substring,
								  usize substring_length) {
	let ends_with = cnx_string_contains_cstring_from_right(self,
														   substring,
														   substring_length,
														   cnx_string_length(*self) - 1);
	return cnx_option_is_some(ends_with);
}

bool cnx_string_ends_with_stringview(const CnxString* restrict self,
									 const CnxStringView* restrict substring) {
	return cnx_string_ends_with_cstring(self, substring->m_view, substring->m_length);
}

CnxOption(usize)(cnx_string_find_first)(const CnxString* restrict self,
										const CnxString* restrict substring) {
	let length = cnx_string_length(*self);
	let sublength = cnx_string_length(*substring);
	cnx_assert(length > sublength,
			   "cnx_string_find_first called with "
			   "substring->m_length > self->m_length (substring length out of bounds");
	let end = length - sublength;
	for(let_mut i = 0U; i < end; ++i) {
		let maybe_contains = cnx_string_contains_from_left(self, substring, i);
		if(cnx_option_is_some(maybe_contains)) {
			return maybe_contains;
		}
	}

	return None(usize);
}

CnxOption(usize) cnx_string_find_first_cstring(const CnxString* restrict self,
											   restrict const_cstring substring,
											   usize substring_length) {
	let length = cnx_string_length(*self);
	cnx_assert(length > substring_length,
			   "cnx_string_find_first_cstring called with "
			   "substring_length > self->m_length (substring length out of bounds");
	let end = length - substring_length;
	for(let_mut i = 0U; i < end; ++i) {
		let maybe_contains
			= cnx_string_contains_cstring_from_left(self, substring, substring_length, i);
		if(cnx_option_is_some(maybe_contains)) {
			return maybe_contains;
		}
	}

	return None(usize);
}

CnxOption(usize) cnx_string_find_first_stringview(const CnxString* restrict self,
												  const CnxStringView* restrict substring) {
	return cnx_string_find_first_cstring(self, substring->m_view, substring->m_length);
}

CnxOption(usize)(cnx_string_find_last)(const CnxString* restrict self,
									   const CnxString* restrict substring) {
	let length = cnx_string_length(*self);
	let sublength = cnx_string_length(*substring);
	cnx_assert(length > sublength,
			   "cnx_string_find_first_cstring called with "
			   "substring->m_length > self->m_length (substring length out of bounds");
	cnx_assert(sublength > 0, "cnx_string_find_last called with a substring of 0 length");

	let end = sublength;
	for(let_mut i = length - 1; i >= end; --i) {
		let maybe_contains = cnx_string_contains_from_right(self, substring, i);
		if(cnx_option_is_some(maybe_contains)) {
			return maybe_contains;
		}
	}
	return None(usize);
}

CnxOption(usize) cnx_string_find_last_cstring(const CnxString* restrict self,
											  restrict const_cstring substring,
											  usize substring_length) {
	let length = cnx_string_length(*self);
	cnx_assert(length > substring_length,
			   "cnx_string_find_first_cstring called with "
			   "substring_length > self->m_length (substring length out of bounds");
	cnx_assert(substring_length > 0, "cnx_string_find_last called with a substring of 0 length");

	let end = substring_length;
	for(let_mut i = length - 1; i >= end; --i) {
		let maybe_contains
			= cnx_string_contains_cstring_from_right(self, substring, substring_length, i);
		if(cnx_option_is_some(maybe_contains)) {
			return maybe_contains;
		}
	}
	return None(usize);
}

CnxOption(usize) cnx_string_find_last_stringview(const CnxString* restrict self,
												 const CnxStringView* restrict substring) {
	return cnx_string_find_last_cstring(self, substring->m_view, substring->m_length);
}

CnxString(cnx_string_substring)(const CnxString* restrict self, usize index, usize length) {
	return cnx_string_substring_with_allocator(*self, index, length, self->m_allocator);
}

CnxString(cnx_string_substring_with_allocator)(const CnxString* restrict self,
											   usize index,
											   usize length,
											   CnxAllocator allocator) {
	__attr(maybe_unused) let self_length = cnx_string_length(*self);
	cnx_assert(index < self_length,
			   "cnx_string_substring called with index >= self->m_length (index out of bounds)");
	cnx_assert(index + length <= self_length,
			   "cnx_string_substring called with index + length > self->m_length (substring range "
			   "out of bounds)");

	let_mut string = cnx_string_new_with_capacity_with_allocator(length, allocator);

	cnx_memcpy(char, &cnx_string_at(string, 0), &cnx_string_at(*self, index), length);
	cnx_string_set_length(&string, length);

	return string;
}

CnxStringView(cnx_string_stringview_of)(const CnxString* restrict self, usize index, usize length) {
	__attr(maybe_unused) let self_length = cnx_string_length(*self);
	cnx_assert(index < self_length,
			   "cnx_stringview_of called with index >= self->m_length (index out of bounds)");
	cnx_assert(
		index + length <= self_length,
		"cnx_string_stringview_of called with index + length > self->m_length (substring range "
		"out of bounds)");

	return (CnxStringView){.m_view = &cnx_string_at(*self, index),
						   .m_length = length,
						   .m_vtable = &cnx_stringview_vtable};
}

CnxString(cnx_string_concatenate)(const CnxString* restrict left, const CnxString* restrict right) {
	return cnx_string_concatenate_with_allocator(left, right, left->m_allocator);
}

CnxString cnx_string_concatenate_cstring(const CnxString* restrict left,
										 restrict const_cstring right,
										 usize right_length) {
	return cnx_string_concatenate_cstring_with_allocator(left,
														 right,
														 right_length,
														 left->m_allocator);
}

CnxString cnx_string_concatenate_cstrings(restrict const_cstring left,
										  usize left_length,
										  restrict const_cstring right,
										  usize right_length) {
	return cnx_string_concatenate_cstrings_with_allocator(left,
														  left_length,
														  right,
														  right_length,
														  CNX_DEFAULT_ALLOCATOR);
}

CnxString cnx_string_concatenate_stringview(const CnxString* restrict left,
											const CnxStringView* restrict right) {
	return cnx_string_concatenate_cstring(left, right->m_view, right->m_length);
}

// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
CnxString cnx_string_concatenate_stringviews(const CnxStringView* restrict left,
											 const CnxStringView* restrict right) {
	return cnx_string_concatenate_cstrings(left->m_view,
										   left->m_length,
										   right->m_view,
										   right->m_length);
}

// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
CnxString(cnx_string_concatenate_with_allocator)(const CnxString* restrict left,
												 const CnxString* restrict right,
												 CnxAllocator allocator) {
	let left_length = cnx_string_length(*left);
	let right_length = cnx_string_length(*right);
	let_mut string
		= cnx_string_new_with_capacity_with_allocator(left_length + right_length, allocator);

	cnx_string_set_length(&string, left_length + right_length);
	cnx_memcpy(char, &cnx_string_at(string, 0), &cnx_string_at(*left, 0), left_length);
	if(right_length > 0) {
		cnx_memcpy(char,
				   &cnx_string_at(string, left_length),
				   &cnx_string_at(*right, 0),
				   right_length);
	}

	return string;
}

CnxString cnx_string_concatenate_cstring_with_allocator(const CnxString* restrict left,
														restrict const_cstring right,
														usize right_length,
														CnxAllocator allocator) {
	let left_length = cnx_string_length(*left);
	let_mut string
		= cnx_string_new_with_capacity_with_allocator(left_length + right_length, allocator);

	cnx_string_set_length(&string, left_length + right_length);
	cnx_memcpy(char, &cnx_string_at(string, 0), &cnx_string_at(*left, 0), left_length);
	if(right_length > 0) {
		cnx_memcpy(char, &cnx_string_at(string, left_length), right, right_length);
	}

	return string;
}

CnxString cnx_string_concatenate_cstrings_with_allocator(restrict const_cstring left,
														 usize left_length,
														 restrict const_cstring right,
														 usize right_length,
														 CnxAllocator allocator) {
	let_mut string
		= cnx_string_new_with_capacity_with_allocator(left_length + right_length, allocator);

	cnx_string_set_length(&string, left_length + right_length);
	cnx_memcpy(char, &cnx_string_at(string, 0), left, left_length);
	if(right_length > 0) {
		cnx_memcpy(char, &cnx_string_at(string, left_length), right, right_length);
	}

	return string;
}

CnxString cnx_string_concatenate_stringview_with_allocator(const CnxString* restrict left,
														   const CnxStringView* restrict right,
														   CnxAllocator allocator) {
	return cnx_string_concatenate_cstring_with_allocator(left,
														 right->m_view,
														 right->m_length,
														 allocator);
}

CnxString cnx_string_concatenate_stringviews_with_allocator(const CnxStringView* restrict left,
															const CnxStringView* restrict right,
															CnxAllocator allocator) {
	return cnx_string_concatenate_cstrings_with_allocator(left->m_view,
														  left->m_length,
														  right->m_view,
														  right->m_length,
														  allocator);
}

void(cnx_string_fill)(CnxString* restrict self, char character) {
	cnx_string_set_length(self, cnx_string_capacity(*self));
	cnx_memset(char, &cnx_string_at(*self, 0), character, cnx_string_length(*self));
}

void(cnx_string_clear)(CnxString* restrict self) {
	cnx_string_fill(*self, 0);
	cnx_string_set_length(self, 0);
}

void(cnx_string_shrink_to_fit)(CnxString* restrict self) {
	let_mut old = *self;
	*self = cnx_string_clone(old);
	cnx_string_free(old);
}

void(cnx_string_insert)(CnxString* restrict self,
						const CnxString* restrict to_insert,
						usize index) {
	let length = cnx_string_length(*self);
	cnx_assert(index <= length,
			   "cnx_string_insert called with index > self->m_length (index out of bounds)");

	if(index != 0) {
		CnxScopedString first = cnx_string_first(*self, index);

		let_mut left = cnx_string_concatenate(&first, to_insert);
		let_mut old = *self;
		if(index < length) {
			CnxScopedString second = cnx_string_last(*self, length - index);
			*self = cnx_string_concatenate(&left, &second);
			cnx_string_free(left);
		}
		else {
			*self = left;
		}
		cnx_string_free(old);
	}
	else {
		let new = cnx_string_concatenate(to_insert, self);
		let_mut old = *self;
		*self = new;
		cnx_string_free(old);
	}
}

void cnx_string_insert_cstring(CnxString* restrict self,
							   restrict const_cstring to_insert,
							   // NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
							   usize to_insert_length,
							   usize index) {
	let length = cnx_string_length(*self);
	cnx_assert(index <= length,
			   "cnx_string_insert called with index > self->m_length (index out of bounds)");

	if(index != 0) {
		CnxScopedString first = cnx_string_first(*self, index);

		let_mut left = cnx_string_concatenate_cstring(&first, to_insert, to_insert_length);
		let_mut old = *self;
		if(index < length) {
			CnxScopedString second = cnx_string_last(*self, length - index);
			*self = cnx_string_concatenate(&left, &second);
			cnx_string_free(left);
		}
		else {
			*self = left;
		}
		cnx_string_free(old);
	}
	else {
		CnxScopedString left = cnx_string_from_cstring_with_allocator(to_insert,
																	  to_insert_length,
																	  self->m_allocator);
		let new = cnx_string_concatenate(&left, self);
		let_mut old = *self;
		*self = new;
		cnx_string_free(old);
	}
}

void cnx_string_insert_stringview(CnxString* restrict self,
								  const CnxStringView* restrict to_insert,
								  usize index) {
	cnx_string_insert_cstring(self, to_insert->m_view, to_insert->m_length, index);
}

void(cnx_string_erase)(CnxString* restrict self, usize index) {
	let length = cnx_string_length(*self);
	cnx_assert(index < length,
			   "cnx_string_erase called with index >= self->m_length (index out of bounds)");

	if(index != length - 1) {
		let num_to_move = length - (index + 1);
		cnx_memmove(char,
					&cnx_string_at(*self, index),
					&cnx_string_at(*self, index + 1),
					num_to_move);
	}
	cnx_string_decrease_length(self, 1);
}

void(cnx_string_erase_n)(CnxString* restrict self, usize index, usize num_characters) {
	let length = cnx_string_length(*self);
	cnx_assert(index < length,
			   "cnx_string_erase_n called with index >= self->m_length (index out of bounds)");
	cnx_assert(index + num_characters < length,
			   "cnx_string_erase_n called with"
			   " index + num_characters >= self->m_length (range out of bounds)");

	let end = index + num_characters;
	let num_to_move = length - end;

	if(end != length) {
		cnx_memmove(char, &cnx_string_at(*self, index), &cnx_string_at(*self, end), num_to_move);
	}
	else {
		cnx_memset(char, &cnx_string_at(*self, index), 0, num_characters);
	}
	cnx_string_decrease_length(self, num_characters);
}

void cnx_string_resize_internal(CnxString* restrict self, usize new_size) {
	let length = cnx_string_length(*self);
	new_size += 1;
	if(new_size < length) {
		let num_to_erase = length - new_size;
		cnx_memset(char, &cnx_string_at(*self, new_size), 0, num_to_erase);
	}
	if(new_size > CNX_STRING_SHORT_OPTIMIZATION_CAPACITY) {
		let_mut string = cnx_allocator_allocate_array_t(char, self->m_allocator, new_size);
		let num_to_copy = cnx_min(length, new_size);
		cnx_memcpy(char, string, &cnx_string_at(*self, 0), num_to_copy);
		if(!cnx_string_is_short(self)) {
			let_mut ptr = self->m_long;
			self->m_long = nullptr;
			cnx_allocator_deallocate(self->m_allocator, ptr);
		}
		cnx_string_set_long(self);
		cnx_string_set_capacity(self, new_size - 1);
		cnx_string_set_length(self, num_to_copy);
		self->m_long = string;
	}
	else if(cnx_string_capacity(*self) != CNX_STRING_SHORT_OPTIMIZATION_CAPACITY) {
		let capacity = CNX_STRING_SHORT_OPTIMIZATION_CAPACITY + 1;
		let_mut string = cnx_allocator_allocate_array_t(char, self->m_allocator, capacity);
		cnx_memcpy(char, string, self->m_long, capacity);
		cnx_allocator_deallocate(self->m_allocator, self->m_long);
		cnx_memcpy(char, self->m_short, string, capacity);
		cnx_allocator_deallocate(self->m_allocator, string);
		cnx_string_set_short(self);
		cnx_string_set_length(self, capacity - 1);
		// cnx_string_set_capacity(self, capacity - 1);
	}
}

__attr(always_inline) __attr(nodiscard) static inline usize
	cnx_string_get_expanded_capacity(usize old_capacity, usize num_increments) {
	return num_increments * ((old_capacity * 3) / 2);
}

void(cnx_string_resize)(CnxString* restrict self, usize new_size) {
	let length = cnx_string_length(*self);
	cnx_string_resize_internal(self, new_size);
	cnx_string_set_length(self, cnx_min(new_size, length));
}

void(cnx_string_reserve)(CnxString* restrict self, usize new_capacity) {
	let capacity = cnx_string_capacity(*self);
	if(new_capacity > capacity) {
		let num_increments = 1 + (new_capacity / ((capacity * 3) / 2));
		let actual_new_capacity = cnx_string_get_expanded_capacity(capacity, num_increments);
		cnx_string_resize_internal(self, actual_new_capacity);
	}
}

void(cnx_string_push_back)(CnxString* restrict self, char character) {
	let length = cnx_string_length(*self);
	let capacity = cnx_string_capacity(*self);
	if(length + 1 >= capacity) {
		let new_capacity = cnx_string_get_expanded_capacity(capacity, 1);
		cnx_string_resize_internal(self, new_capacity);
	}

	cnx_string_at(*self, length) = character;
	cnx_string_increase_length(self, 1);
}

void(cnx_string_push_front)(CnxString* restrict self, char character) {
	let length = cnx_string_length(*self);
	let capacity = cnx_string_capacity(*self);
	if(length + 1 >= capacity) {
		let new_capacity = cnx_string_get_expanded_capacity(capacity, 1);
		cnx_string_resize_internal(self, new_capacity);
	}

	cnx_memmove(char, &cnx_string_at(*self, 1), &cnx_string_at(*self, 0), length);

	cnx_string_at(*self, 0U) = character;
	cnx_string_increase_length(self, 1);
}

CnxOption(char)(cnx_string_pop_back)(CnxString* restrict self) {
	let length = cnx_string_length(*self);
	if(length == 0) {
		return None(char);
	}

	let_mut character_ptr = &cnx_string_at(*self, length - 1);
	let character = Some(char, *character_ptr);
	*character_ptr = 0;
	cnx_string_decrease_length(self, 1);

	return character;
}

CnxOption(char)(cnx_string_pop_front)(CnxString* restrict self) {
	let length = cnx_string_length(*self);
	if(length == 0) {
		return None(char);
	}

	let character = Some(char, cnx_string_at(*self, 0U));
	cnx_memmove(char, &cnx_string_at(*self, 0), &cnx_string_at(*self, 1), length - 1);
	cnx_string_decrease_length(self, 1);

	return character;
}

void(cnx_string_append)(CnxString* restrict self, const CnxString* restrict to_append) {
	cnx_string_insert(*self, to_append, cnx_string_length(*self));
}

void cnx_string_append_cstring(CnxString* restrict self,
							   restrict const_cstring to_append,
							   usize to_append_length) {
	cnx_string_insert_cstring(self, to_append, to_append_length, cnx_string_length(*self));
}

void cnx_string_append_stringview(CnxString* restrict self,
								  const CnxStringView* restrict to_append) {
	cnx_string_append_cstring(self, to_append->m_view, to_append->m_length);
}

void(cnx_string_prepend)(CnxString* restrict self, const CnxString* restrict to_prepend) {
	cnx_string_insert(*self, to_prepend, 0U);
}

void cnx_string_prepend_cstring(CnxString* restrict self,
								restrict const_cstring to_prepend,
								usize to_prepend_length) {
	cnx_string_insert_cstring(self, to_prepend, to_prepend_length, 0U);
}

void cnx_string_prepend_stringview(CnxString* restrict self,
								   const CnxStringView* restrict to_prepend) {
	cnx_string_prepend_cstring(self, to_prepend->m_view, to_prepend->m_length);
}

void(cnx_string_replace)(CnxString* restrict self,
						 const CnxString* restrict to_replace_with,
						 usize index) {
	let length = cnx_string_length(*self);
	let replace_length = cnx_string_length(*to_replace_with);
	let stop = cnx_min(replace_length + index, length);

	cnx_memcpy(char,
			   &cnx_string_at(*self, index),
			   &cnx_string_at(*to_replace_with, 0),
			   stop - index);

	if(stop == length) {
		let begin = length - index;
		let num_remaining = replace_length - begin;
		let new_capacity = length + num_remaining;
		cnx_string_reserve(*self, new_capacity);
		cnx_memcpy(char,
				   &cnx_string_at(*self, length),
				   &cnx_string_at(*to_replace_with, begin),
				   num_remaining);
	}
}

void cnx_string_replace_cstring(CnxString* restrict self,
								restrict const_cstring to_replace_with,
								usize to_replace_with_length,
								usize index) {
	let length = cnx_string_length(*self);
	let stop = cnx_min(to_replace_with_length + index, length);

	cnx_memcpy(char, &cnx_string_at(*self, index), to_replace_with, stop - index);

	if(stop == length) {
		let begin = length - index;
		let num_remaining = to_replace_with_length - begin;
		let new_capacity = length + num_remaining;
		cnx_string_reserve(*self, new_capacity);
		cnx_memcpy(char, &cnx_string_at(*self, length), &(to_replace_with[begin]), num_remaining);
	}
}

void cnx_string_replace_stringview(CnxString* restrict self,
								   const CnxStringView* restrict to_replace_with,
								   usize index) {
	cnx_string_replace_cstring(self, to_replace_with->m_view, to_replace_with->m_length, index);
}

CnxStringIterator cnx_string_iterator_new(const CnxString* restrict self) {
	return (CnxStringIterator){.m_index = 0U, .m_string = const_cast(CnxString*)(self)};
}

CnxStringConstIterator cnx_string_const_iterator_new(const CnxString* restrict self) {
	return (CnxStringConstIterator){.m_index = 0U, .m_string = const_cast(CnxString*)(self)};
}

char_ref cnx_string_iterator_next(CnxRandomAccessIterator(char_ref) * restrict self) {
	let _self = static_cast(CnxStringIterator*)(self->m_self);

	cnx_assert(_self->m_index > -1,
			   "Iterator value accessed when iterator is positioned before beginning of iteration "
			   "(iterator out of bounds)");
	cnx_assert(static_cast(usize)(_self->m_index) < cnx_string_length(*(_self->m_string)),
			   "Iterator value accessed when iterator is positioned after the end of the iteration "
			   "(iterator out of bounds");
	_self->m_index++;
	if(static_cast(usize)(_self->m_index) >= cnx_string_length(*(_self->m_string))) {
		return &cnx_string_at(*(_self->m_string), cnx_string_length(*(_self->m_string)) - 1);
	}
	return &cnx_string_at(*(_self->m_string), static_cast(usize)(_self->m_index));
}

char_ref cnx_string_iterator_previous(CnxRandomAccessIterator(char_ref) * restrict self) {
	let _self = static_cast(CnxStringIterator*)(self->m_self);

	cnx_assert(_self->m_index > -1,
			   "Iterator value accessed when iterator is positioned before beginning of iteration "
			   "(iterator out of bounds)");
	cnx_assert(static_cast(usize)(_self->m_index) < cnx_string_length(*(_self->m_string)),
			   "Iterator value accessed when iterator is positioned after the end of the iteration "
			   "(iterator out of bounds");
	_self->m_index--;
	if(_self->m_index < 0) {
		return &cnx_string_at(*(_self->m_string), 0);
	}
	return &cnx_string_at(*(_self->m_string), static_cast(usize)(_self->m_index));
}

char_ref
cnx_string_iterator_at(const CnxRandomAccessIterator(char_ref) * restrict self, usize index) {
	let _self = static_cast(const CnxStringIterator*)(self->m_self);

	cnx_assert(index < cnx_string_length(*(_self->m_string)),
			   "cnx_string_iterator_at called with index past the end of the iteration "
			   "(iterator index out of bounds");
	return &cnx_string_at(*(_self->m_string), index);
}

char_ref
cnx_string_iterator_rat(const CnxRandomAccessIterator(char_ref) * restrict self, usize index) {
	let _self = static_cast(const CnxStringIterator*)(self->m_self);

	cnx_assert(index < cnx_string_length(*(_self->m_string)),
			   "cnx_string_iterator_rat called with index past the end of the iteration "
			   "(iterator index out of bounds");
	return &cnx_string_at(*(_self->m_string), (cnx_string_length(*(_self->m_string)) - 1) - index);
}

char_ref cnx_string_iterator_current(const CnxRandomAccessIterator(char_ref) * restrict self) {
	let _self = static_cast(const CnxStringIterator*)(self->m_self);

	cnx_assert(_self->m_index > -1,
			   "Iterator value accessed when iterator is positioned before beginning of iteration "
			   "(iterator out of bounds)");
	cnx_assert(static_cast(usize)(_self->m_index) < cnx_string_length(*(_self->m_string)),
			   "Iterator value accessed when iterator is positioned after the end of the iteration "
			   "(iterator out of bounds");
	return &cnx_string_at(*(_self->m_string), static_cast(usize)(_self->m_index));
}

bool cnx_string_iterator_equals(const CnxRandomAccessIterator(char_ref) * restrict self,
								const CnxRandomAccessIterator(char_ref) * restrict rhs) {
	let _self = static_cast(const CnxStringIterator*)(self->m_self);
	let _rhs = static_cast(const CnxStringIterator*)(rhs->m_self);

	return _self->m_index == _rhs->m_index && _self->m_string == _rhs->m_string;
}

const_char_ref cnx_string_iterator_cnext(CnxRandomAccessIterator(const_char_ref) * restrict self) {
	let _self = static_cast(CnxStringConstIterator*)(self->m_self);

	cnx_assert(_self->m_index > -1,
			   "Iterator value accessed when iterator is positioned before beginning of iteration "
			   "(iterator out of bounds)");
	cnx_assert(static_cast(usize)(_self->m_index) < cnx_string_length(*(_self->m_string)),
			   "Iterator value accessed when iterator is positioned after the end of the iteration "
			   "(iterator out of bounds");
	_self->m_index++;
	if(static_cast(usize)(_self->m_index) >= cnx_string_length(*(_self->m_string))) {
		return &cnx_string_at(*(_self->m_string), cnx_string_length(*(_self->m_string)) - 1);
	}
	return &cnx_string_at(*(_self->m_string), static_cast(usize)(_self->m_index));
}

const_char_ref
cnx_string_iterator_cprevious(CnxRandomAccessIterator(const_char_ref) * restrict self) {
	let _self = static_cast(CnxStringConstIterator*)(self->m_self);

	cnx_assert(_self->m_index > -1,
			   "Iterator value accessed when iterator is positioned before beginning of iteration "
			   "(iterator out of bounds)");
	cnx_assert(static_cast(usize)(_self->m_index) < cnx_string_length(*(_self->m_string)),
			   "Iterator value accessed when iterator is positioned after the end of the iteration "
			   "(iterator out of bounds");
	_self->m_index--;
	if(_self->m_index < 0) {
		return &cnx_string_at(*(_self->m_string), 0);
	}
	return &cnx_string_at(*(_self->m_string), static_cast(usize)(_self->m_index));
}

const_char_ref
cnx_string_iterator_cat(const CnxRandomAccessIterator(const_char_ref) * restrict self,
						usize index) {
	let _self = static_cast(const CnxStringConstIterator*)(self->m_self);

	cnx_assert(index < cnx_string_length(*(_self->m_string)),
			   "cnx_string_iterator_cat called with index past the end of the iteration "
			   "(iterator index out of bounds");
	return &cnx_string_at(*(_self->m_string), index);
}

const_char_ref
cnx_string_iterator_crat(const CnxRandomAccessIterator(const_char_ref) * restrict self,
						 usize index) {
	let _self = static_cast(const CnxStringConstIterator*)(self->m_self);

	cnx_assert(index < cnx_string_length(*(_self->m_string)),
			   "cnx_string_iterator_crat called with index past the end of the iteration "
			   "(iterator index out of bounds");
	return &cnx_string_at(*(_self->m_string), (cnx_string_length(*(_self->m_string)) - 1) - index);
}

const_char_ref
cnx_string_iterator_ccurrent(const CnxRandomAccessIterator(const_char_ref) * restrict self) {
	let _self = static_cast(const CnxStringConstIterator*)(self->m_self);

	cnx_assert(_self->m_index > -1,
			   "Iterator value accessed when iterator is positioned before beginning of iteration "
			   "(iterator out of bounds)");
	cnx_assert(static_cast(usize)(_self->m_index) < cnx_string_length(*(_self->m_string)),
			   "Iterator value accessed when iterator is positioned after the end of the iteration "
			   "(iterator out of bounds");
	return &cnx_string_at(*(_self->m_string), static_cast(usize)(_self->m_index));
}

bool cnx_string_iterator_cequals(const CnxRandomAccessIterator(const_char_ref) * restrict self,
								 const CnxRandomAccessIterator(const_char_ref) * restrict rhs) {
	let _self = static_cast(const CnxStringConstIterator*)(self->m_self);
	let _rhs = static_cast(const CnxStringConstIterator*)(rhs->m_self);

	return _self->m_index == _rhs->m_index && _self->m_string == _rhs->m_string;
}

CnxRandomAccessIterator(char_ref)(cnx_string_begin)(CnxString* restrict self) {
	let_mut iter = cnx_string_into_iter(self);
	let_mut inner = static_cast(CnxStringIterator*)(iter.m_self);
	inner->m_index = 0U;
	return iter;
}

CnxRandomAccessIterator(char_ref)(cnx_string_end)(CnxString* restrict self) {
	let_mut iter = cnx_string_into_iter(self);
	let_mut inner = static_cast(CnxStringIterator*)(iter.m_self);
	inner->m_index = static_cast(isize)(cnx_string_length(*self));
	return iter;
}

CnxRandomAccessIterator(char_ref)(cnx_string_rbegin)(CnxString* restrict self) {
	let_mut iter = cnx_string_into_reverse_iter(self);
	let_mut inner = static_cast(CnxStringIterator*)(iter.m_self);
	inner->m_index = static_cast(isize)(cnx_string_length(*self) - 1);
	return iter;
}

CnxRandomAccessIterator(char_ref)(cnx_string_rend)(CnxString* restrict self) {
	let_mut iter = cnx_string_into_reverse_iter(self);
	let_mut inner = static_cast(CnxStringIterator*)(iter.m_self);
	inner->m_index = -1;
	return iter;
}

CnxRandomAccessIterator(const_char_ref)(cnx_string_cbegin)(const CnxString* restrict self) {
	let_mut iter = cnx_string_into_const_iter(self);
	let_mut inner = static_cast(CnxStringConstIterator*)(iter.m_self);
	inner->m_index = 0U;
	return iter;
}

CnxRandomAccessIterator(const_char_ref)(cnx_string_cend)(const CnxString* restrict self) {
	let_mut iter = cnx_string_into_const_iter(self);
	let_mut inner = static_cast(CnxStringConstIterator*)(iter.m_self);
	inner->m_index = static_cast(isize)(cnx_string_length(*self));
	return iter;
}

CnxRandomAccessIterator(const_char_ref)(cnx_string_crbegin)(const CnxString* restrict self) {
	let_mut iter = cnx_string_into_reverse_const_iter(self);
	let_mut inner = static_cast(CnxStringConstIterator*)(iter.m_self);
	inner->m_index = static_cast(isize)(cnx_string_length(*self) - 1);
	return iter;
}

CnxRandomAccessIterator(const_char_ref)(cnx_string_crend)(const CnxString* restrict self) {
	let_mut iter = cnx_string_into_reverse_const_iter(self);
	let_mut inner = static_cast(CnxStringConstIterator*)(iter.m_self);
	inner->m_index = -1;
	return iter;
}

CnxStringView cnx_stringview_new(const CnxString* restrict string) {
	return (CnxStringView){.m_view = &cnx_string_at(*string, 0U),
						   .m_length = cnx_string_length(*string),
						   .m_vtable = &cnx_stringview_vtable};
}

CnxStringView cnx_stringview_from(restrict const_cstring string, usize index, usize length) {
	cnx_assert(strlen(string) >= index + length,
			   "Can't create stringview with bounds outside the given cstring");
	return (CnxStringView){.m_view = &(string[index]),
						   .m_length = length,
						   .m_vtable = &cnx_stringview_vtable};
}

const_char_ptr(cnx_stringview_at)(const CnxStringView* restrict self, usize index) {
	cnx_assert(index < self->m_length,
			   "cnx_stringview_at called with index > length (index out of bounds)");
	return &(self->m_view[index]);
}

usize(cnx_stringview_length)(const CnxStringView* restrict self) {
	return self->m_length;
}

usize(cnx_stringview_size)(const CnxStringView* restrict self) {
	return self->m_length;
}

// NOLINTNEXTLINE(bugprone-easily-swappable-parameters
bool(cnx_stringview_equal)(const CnxStringView* restrict self,
						   const CnxStringView* restrict to_compare) {
	if(self->m_length != to_compare->m_length) {
		return false;
	}

	return memcmp(self->m_view, to_compare->m_view, self->m_length) == 0;
}

bool cnx_stringview_equal_string(const CnxStringView* restrict self,
								 const CnxString* restrict to_compare) {
	return cnx_string_equal_stringview(to_compare, self);
}

// NOLINTNEXTLINE(bugprone-easily-swappable-parameters
bool cnx_stringview_equal_cstring(const CnxStringView* restrict self,
								  restrict const_cstring to_compare,
								  usize to_compare_length) {
	let view = cnx_stringview_from(to_compare, 0, to_compare_length);
	return (cnx_stringview_equal)(self, &view);
}

const_cstring(cnx_stringview_into_cstring)(const CnxStringView* restrict self) {
	return self->m_view;
}

CnxStringViewIterator cnx_stringview_iterator_new(const CnxStringView* restrict self) {
	return (CnxStringViewIterator){.m_index = 0U, .m_view = const_cast(CnxStringView*)(self)};
}

const_char_ref
cnx_stringview_iterator_next(CnxRandomAccessIterator(const_char_ref) * restrict self) {
	let _self = static_cast(CnxStringViewIterator*)(self->m_self);

	let ret = cnx_stringview_at(*(_self->m_view), static_cast(usize)(_self->m_index));
	_self->m_index++;
	return ret;
}

const_char_ref
cnx_stringview_iterator_previous(CnxRandomAccessIterator(const_char_ref) * restrict self) {
	let _self = static_cast(CnxStringViewIterator*)(self->m_self);

	let ret = cnx_stringview_at(*(_self->m_view), static_cast(usize)(_self->m_index));
	_self->m_index--;
	return ret;
}

const_char_ref
cnx_stringview_iterator_at(const CnxRandomAccessIterator(const_char_ref) * restrict self,
						   usize index) {
	let _self = static_cast(const CnxStringViewIterator*)(self->m_self);

	return cnx_stringview_at(*(_self->m_view), index);
}

const_char_ref
cnx_stringview_iterator_rat(const CnxRandomAccessIterator(const_char_ref) * restrict self,
							usize index) {
	let _self = static_cast(const CnxStringViewIterator*)(self->m_self);

	return cnx_stringview_at(*(_self->m_view),
							 (cnx_stringview_length(*(_self->m_view)) - 1) - index);
}

const_char_ref
cnx_stringview_iterator_current(const CnxRandomAccessIterator(const_char_ref) * restrict self) {
	let _self = static_cast(const CnxStringViewIterator*)(self->m_self);

	return cnx_stringview_at(*(_self->m_view), static_cast(usize)(_self->m_index));
}

bool cnx_stringview_iterator_equals(const CnxRandomAccessIterator(const_char_ref) * restrict self,
									const CnxRandomAccessIterator(const_char_ref) * restrict rhs) {
	let _self = static_cast(const CnxStringViewIterator*)(self->m_self);
	let _rhs = static_cast(const CnxStringViewIterator*)(rhs->m_self);

	return _self->m_view == _rhs->m_view && _self->m_index == _rhs->m_index;
}

CnxRandomAccessIterator(const_char_ref)(cnx_stringview_begin)(const CnxStringView* restrict self) {
	let_mut iter = cnx_stringview_into_iter(self);
	let_mut inner = static_cast(CnxStringViewIterator*)(iter.m_self);
	inner->m_index = 0U;
	return iter;
}

CnxRandomAccessIterator(const_char_ref)(cnx_stringview_end)(const CnxStringView* restrict self) {
	let_mut iter = cnx_stringview_into_iter(self);
	let_mut inner = static_cast(CnxStringViewIterator*)(iter.m_self);
	inner->m_index = static_cast(isize)(self->m_length);
	return iter;
}

CnxRandomAccessIterator(const_char_ref)(cnx_stringview_rbegin)(const CnxStringView* restrict self) {
	let_mut iter = cnx_stringview_into_reverse_iter(self);
	let_mut inner = static_cast(CnxStringViewIterator*)(iter.m_self);
	inner->m_index = static_cast(isize)(self->m_length - 1);
	return iter;
}

CnxRandomAccessIterator(const_char_ref)(cnx_stringview_rend)(const CnxStringView* restrict self) {
	let_mut iter = cnx_stringview_into_reverse_iter(self);
	let_mut inner = static_cast(CnxStringViewIterator*)(iter.m_self);
	inner->m_index = -1;
	return iter;
}
