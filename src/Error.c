/// @file Error.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief This module provides an extensible type for communicating errors via both error codes and
/// message strings.
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

#if(defined(_MSC_VER) || defined(_WIN32) || defined(_WIN64)) && !defined(_CRT_SECURE_NO_WARNINGS)
	// NOLINTNEXTLINE
	#define _CRT_SECURE_NO_WARNINGS
#endif

#include <Cnx/Error.h>
#include <string.h>

__attr(nodiscard) CnxError cnx_error_new(i64 error_code, CnxErrorCategory error_category) {
	return (CnxError){.m_error_code = error_code, .m_error_category = error_category};
}

__attr(nodiscard) __attr(not_null(1)) i64 cnx_error_code(const CnxError* restrict self) {
	return self->m_error_code;
}

__attr(nodiscard) __attr(not_null(1)) __attr(returns_nonnull) const_cstring
	cnx_error_message(const CnxError* restrict self) {
	return cnx_error_category_get_message(self->m_error_category, self->m_error_code);
}

__attr(nodiscard) __attr(returns_nonnull) const_cstring
	cnx_error_category_get_message(CnxErrorCategory self, i64 error_code) {
	return trait_call(message, self, error_code);
}

__attr(nodiscard) i64 cnx_error_category_get_last_error(CnxErrorCategory self) {
	return trait_call(get_last_error, self);
}

__attr(nodiscard) __attr(not_null(1)) __attr(returns_nonnull) const_cstring
	cnx_posix_category_get_message(__attr(maybe_unused) const CnxErrorCategory* restrict self,
								   i64 error_code) {
	return strerror(narrow_cast(int)(error_code));
}

__attr(nodiscard) __attr(not_null(1)) i64
	cnx_posix_category_get_last_error(__attr(maybe_unused) const CnxErrorCategory* restrict self) {
	return errno;
}

#if CNX_PLATFORM_WINDOWS
	#include <Cnx/Allocators.h>
	#include <Windows.h>

const_cstring
cnx_win32_category_get_message(__attr(maybe_unused) const CnxErrorCategory* restrict self,
							   i64 error_code) {
	let_mut alloc_size = static_cast(usize)(64); // NOLINT
	let_mut buffer = cnx_allocator_allocate_array_t(char, DEFAULT_ALLOCATOR, alloc_size);
	// get the win32 system error message for the error code
	DWORD error = 0;
	DWORD message_length = 0;
	do {
		message_length = FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM			 // NOLINT
											| FORMAT_MESSAGE_IGNORE_INSERTS, // NOLINT
										nullptr,
										static_cast(DWORD)(error_code),
										0,
										buffer, // NOLINT
										static_cast(DWORD)(alloc_size),
										nullptr);

		// bail if getting the message failed
		if(message_length == 0) {
			error = GetLastError();
			let old_size = alloc_size;
			alloc_size *= static_cast(usize)(2);
			buffer = cnx_allocator_reallocate_array_t(char,
													  DEFAULT_ALLOCATOR,
													  buffer,
													  old_size,
													  alloc_size);
		}
		else {
			return buffer;
		}
	} while(error == ERROR_INSUFFICIENT_BUFFER);

	return "Failed to get the error message from the system";
}

i64 cnx_win32_category_get_last_error(__attr(maybe_unused) const CnxErrorCategory* restrict self) {
	return GetLastError();
}
#endif

typedef struct ErrorContext {
	bool is_debug;
} ErrorContext;

__attr(nodiscard) __attr(not_null(1)) CnxFormatContext
	cnx_error_is_specifier_valid(__attr(maybe_unused) const CnxFormat* restrict self,
								 CnxStringView specifier) {
	let_mut context = (CnxFormatContext){.is_valid = CNX_FORMAT_SUCCESS};
	let length = cnx_stringview_length(specifier);
	let_mut state = (ErrorContext){.is_debug = false};

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

	*(static_cast(ErrorContext*)(context.state)) = state;
	return context;
}

__attr(nodiscard) __attr(not_null(1)) CnxString
	cnx_error_format(const CnxFormat* restrict self, CnxFormatContext context) {
	return cnx_error_format_with_allocator(self, context, cnx_allocator_new());
}

__attr(nodiscard) __attr(not_null(1)) CnxString
	cnx_error_format_with_allocator(const CnxFormat* restrict self,
									CnxFormatContext context,
									CnxAllocator allocator) {
	cnx_assert(context.is_valid == CNX_FORMAT_SUCCESS, "Invalid format specifier used to format a CnxError");

	let state = *(static_cast(const ErrorContext*)(context.state));
	let _self = static_cast(const CnxError*)(self->m_self);
	CnxScopedString message = cnx_string_from_with_allocator(
		trait_call(message, _self->m_error_category, _self->m_error_code),
		allocator);
	if(state.is_debug) {
		return cnx_format_with_allocator(
			AS_STRING(CnxError) ": [error_code: {x}, error_message: {}]",
			allocator,
			_self->m_error_code,
			message);
	}
	else {
		return cnx_format_with_allocator("Error {x}: {}", allocator, _self->m_error_code, message);
	}
}
