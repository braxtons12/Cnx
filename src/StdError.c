/// @file StdError.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief This module provides an extensible type for communicating errors via both error codes and
/// message strings.
/// @version 0.1.1
/// @date 2022-01-07
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

#if(defined(_MSC_VER) || defined(_WIN32) || defined(_WIN64)) && !defined(_CRT_SECURE_NO_WARNINGS)
	// NOLINTNEXTLINE
	#define _CRT_SECURE_NO_WARNINGS
#endif

#include "../include/C2nxt/StdError.h"

#include <errno.h>

StdError std_error_new(i64 error_code, StdErrorCategory error_category) {
	return (StdError){.m_error_code = error_code, .m_error_category = error_category};
}

i64 std_error_code(const StdError* restrict self) {
	return self->m_error_code;
}

StdString std_error_message(const StdError* restrict self) {
	return std_string_from(
		std_error_category_get_message(self->m_error_category, self->m_error_code));
}

StdString std_error_message_with_allocator(const StdError* restrict self, StdAllocator allocator) {
	return std_string_from_with_allocator(
		std_error_category_get_message(self->m_error_category, self->m_error_code),
		allocator);
}

const_cstring std_error_message_as_cstring(const StdError* restrict self) {
	return std_error_category_get_message(self->m_error_category, self->m_error_code);
}

const_cstring std_error_category_get_message(StdErrorCategory self, i64 error_code) {
	return self.m_message_function(error_code);
}

const_cstring std_error_category_get_posix_message(i64 error_code) {
	return strerror(narrow_cast(int)(error_code));
}

StdString std_error_format(const StdFormat* restrict self, StdFormatSpecifier specifier) {
	return std_error_format_with_allocator(self, specifier, std_allocator_new());
}

StdString std_error_format_with_allocator(const StdFormat* restrict self,
										  StdFormatSpecifier maybe_unused specifier,
										  StdAllocator allocator) {
	std_assert(specifier.m_type == STD_FORMAT_TYPE_DEFAULT
				   || specifier.m_type == STD_FORMAT_TYPE_DEBUG,
			   "Can only format StdError with default or debug format specifier");

	let _self = static_cast(const StdError*)(self->m_self);
	let message = std_string_from_with_allocator(
		_self->m_error_category.m_message_function(_self->m_error_code),
		allocator);
	if(specifier.m_type == STD_FORMAT_TYPE_DEBUG) {
		return std_format_with_allocator(
			AS_STRING(StdError) ": [error_code: {x}, error_message: {}]",
			allocator,
			_self->m_error_code,
			message);
	}
	else {
		return std_format_with_allocator("Error {x}: {}", allocator, _self->m_error_code, message);
	}
}
