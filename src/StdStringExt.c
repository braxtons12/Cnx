/// @file StdStringExt.c
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief This module provides additional functionality for working with `StdString`s
/// @version 0.2.0
/// @date 2022-03-31
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

#include <C2nxt/StdAssert.h>
#include <C2nxt/StdMath.h>
#include <C2nxt/StdStringExt.h>

StdVector(StdString)(std_string_split_on)(const StdString* restrict self, char delimiter) {
	return std_string_split_on_with_allocator(*self, delimiter, self->m_allocator);
}

StdVector(StdString)(std_string_split_on_with_allocator)(const StdString* restrict self,
														 char delimiter,
														 StdAllocator allocator) {
	let_mut vec = std_vector_new_with_allocator(StdString, allocator);

	let size = std_string_size(*self);
	let_mut start = static_cast(usize)(0);
	for(let_mut index = static_cast(usize)(0); index < size; ++index) {
		if(std_string_at(*self, index) == delimiter) {
			if(index != 0) {
				let length = index - start;
				if(length > 0) {
					let view = std_string_stringview_of(*self, start, length);
					std_vector_push_back(vec, std_string_from_with_allocator(&view, allocator));
					start = std_min(index + 1, size);
				}
			}
			else {
				start = 1;
			}
		}
	}

	// if we haven't found at least one instance of `delimiter`, forward a copy of `self`
	// OR if the string doesn't end with 'delimiter', make sure we push the last substring
	if(start != size) {
		let length = size - start;
		let view = std_string_stringview_of(*self, start, length);
		std_vector_push_back(vec, std_string_from_with_allocator(&view, allocator));
	}

	return vec;
}

StdVector(StdStringView)(std_string_view_split_on)(const StdString* restrict self, char delimiter) {
	return std_string_view_split_on_with_allocator(*self, delimiter, self->m_allocator);
}

StdVector(StdStringView)(std_string_view_split_on_with_allocator)(const StdString* restrict self,
																  char delimiter,
																  StdAllocator allocator) {
	let_mut vec = std_vector_new_with_allocator(StdStringView, allocator);

	let size = std_string_size(*self);
	let_mut start = static_cast(usize)(0);
	for(let_mut index = static_cast(usize)(0); index < size; ++index) {
		if(std_string_at(*self, index) == delimiter) {
			if(index != 0) {
				let length = index - start;
				if(length > 0) {
					let view = std_string_stringview_of(*self, start, length);
					std_vector_push_back(vec, view);
					start = std_min(index + 1, size);
				}
			}
			else {
				start = 1;
			}
		}
	}

	// if we haven't found at least one instance of `delimiter`, forward a copy of `self`
	// OR if the string doesn't end with 'delimiter', make sure we push the last substring
	if(start != size) {
		let length = size - start;
		let view = std_string_stringview_of(*self, start, length);
		std_vector_push_back(vec, view);
	}

	return vec;
}

usize(std_string_occurrences_of_char)(const StdString* restrict self, char to_find) {

	let_mut count = static_cast(usize)(0);
	foreach(elem, *self) {
		if(elem == to_find) {
			count++;
		}
	}

	return count;
}

// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
usize(std_string_occurrences_of)(const StdString* restrict self,
								 const StdString* restrict to_find) {

	let_mut count = static_cast(usize)(0);
	let size = std_string_size(*self);
	let find_size = std_string_size(*to_find);
	for(let_mut index = static_cast(usize)(0); index < size; ++index) {
		if(index + find_size <= size) {
			let view = std_string_stringview_of(*self, index, find_size);
			if(std_string_equal(*to_find, &view)) {
				count++;
			}
		}
	}

	return count;
}

usize std_string_occurrences_of_stringview(const StdString* restrict self,
										   const StdStringView* restrict to_find) {

	let_mut count = static_cast(usize)(0);
	let size = std_string_size(*self);
	let find_size = std_stringview_size(*to_find);
	for(let_mut index = static_cast(usize)(0); index < size; ++index) {
		if(index + find_size <= size) {
			let view = std_string_stringview_of(*self, index, find_size);
			if(std_stringview_equal(*to_find, &view)) {
				count++;
			}
		}
	}

	return count;
}

usize std_string_occurrences_of_cstring(const StdString* restrict self,
										restrict const_cstring to_find,
										usize to_find_length) {
	let to_find_view = std_stringview_from(to_find, 0, to_find_length);
	return std_string_occurrences_of_stringview(self, &to_find_view);
}

StdVector(usize)(std_string_find_occurrences_of_char)(const StdString* restrict self,
													  char to_find) {

	return std_string_find_occurrences_of_char_with_allocator(*self, to_find, self->m_allocator);
}

StdVector(usize)(std_string_find_occurrences_of)(const StdString* restrict self,
												 const StdString* restrict to_find) {

	return std_string_find_occurrences_of_with_allocator(*self, to_find, self->m_allocator);
}

StdVector(usize) std_string_find_occurrences_of_stringview(const StdString* restrict self,
														   const StdStringView* restrict to_find) {
	return std_string_find_occurrences_of_with_allocator(*self, to_find, self->m_allocator);
}

StdVector(usize) std_string_find_occurrences_of_cstring(const StdString* restrict self,
														restrict const_cstring to_find,
														usize to_find_length) {
	return std_string_find_occurrences_of_cstring_with_allocator(self,
																 to_find,
																 to_find_length,
																 self->m_allocator);
}

StdVector(usize)(std_string_find_occurrences_of_char_with_allocator)(const StdString* restrict self,
																	 char to_find,
																	 StdAllocator allocator) {
	let_mut vec = std_vector_new_with_allocator(usize, allocator);
	let size = std_string_size(*self);
	for(let_mut index = static_cast(usize)(0); index < size; ++index) {
		if(std_string_at(*self, index) == to_find) {
			std_vector_push_back(vec, index);
		}
	}

	return vec;
}

// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
StdVector(usize)(std_string_find_occurrences_of_with_allocator)(const StdString* restrict self,
																const StdString* restrict to_find,
																StdAllocator allocator) {
	let_mut vec = std_vector_new_with_allocator(usize, allocator);
	let size = std_string_size(*self);
	let find_size = std_string_size(*to_find);
	for(let_mut index = static_cast(usize)(0); index < size; ++index) {
		if(index + find_size <= size) {
			let view = std_string_stringview_of(*self, index, find_size);
			if(std_string_equal(*to_find, &view)) {
				std_vector_push_back(vec, index);
			}
		}
	}

	return vec;
}

StdVector(usize)
	std_string_find_occurrences_of_stringview_with_allocator(const StdString* restrict self,
															 const StdStringView* restrict to_find,
															 StdAllocator allocator) {

	let_mut vec = std_vector_new_with_allocator(usize, allocator);
	let size = std_string_size(*self);
	let find_size = std_stringview_size(*to_find);
	for(let_mut index = static_cast(usize)(0); index < size; ++index) {
		if(index + find_size <= size) {
			let view = std_string_stringview_of(*self, index, find_size);
			if(std_stringview_equal(*to_find, &view)) {
				std_vector_push_back(vec, index);
			}
		}
	}

	return vec;
}

StdVector(usize)
	std_string_find_occurrences_of_cstring_with_allocator(const StdString* restrict self,
														  restrict const_cstring to_find,
														  usize to_find_length,
														  StdAllocator allocator) {
	let to_find_view = std_stringview_from(to_find, 0, to_find_length);
	return std_string_find_occurrences_of_stringview_with_allocator(self, &to_find_view, allocator);
}
