/// @file Error.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief This module provides an extensible type for communicating errors via both error codes and
/// message strings.
/// @version 0.2.0
/// @date 2022-04-15
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

CnxError cnx_error_new(i64 error_code, CnxErrorCategory error_category) {
	return (CnxError){.m_error_code = error_code, .m_error_category = error_category};
}

i64 cnx_error_code(const CnxError* restrict self) {
	return self->m_error_code;
}

const_cstring cnx_error_message(const CnxError* restrict self) {
	return cnx_error_category_get_message(self->m_error_category, self->m_error_code);
}

const_cstring cnx_error_category_get_message(CnxErrorCategory self, i64 error_code) {
	return trait_call(message, self, error_code);
}

i64 cnx_error_category_get_last_error(CnxErrorCategory self) {
	return trait_call(get_last_error, self);
}

const_cstring cnx_posix_category_get_message([[maybe_unused]] const CnxErrorCategory* restrict self,
											 i64 error_code) {
	return strerror(narrow_cast(int)(error_code));
}

i64 cnx_posix_category_get_last_error([[maybe_unused]] const CnxErrorCategory* restrict self) {
	return errno;
}

#if CNX_PLATFORM_WINDOWS
	#include <Cnx/Allocators.h>
	#include <Windows.h>

const_cstring cnx_win32_category_get_message([[maybe_unused]] const CnxErrorCategory* restrict self,
											 i64 error_code) {
	wchar_t buffer[1024]; // NOLINT
	// get the win32 system error message for the error code
	DWORD wide_length
		= FormatMessageW(0x00001000 /**FORMAT_MESSAGE_FROM_SYSTEM**/		   // NOLINT
							 | 0x00000200 /**FORMAT_MESSAGE_IGNORE_INSERTS**/, // NOLINT
						 nullptr,
						 code,
						 0,
						 buffer, // NOLINT
						 1024,	 // NOLINT
						 nullptr);

	// bail if getting the message failed
	if(wide_length == 0) {
		return "failed to get message from system";
	}

	let_mut alloc_size = wide_length * 2_usize;
	DWORD error = 0;
	// try to convert the "wide" error message string (UTF16, or UCS-2) to "narrow/normal"
	// UTF8
	do {
		let_mut p = cnx_allocator_allocate_array_t(char, DEFAULT_ALLOCATOR, alloc_size);
		// if memory allocation fails, bail
		if(p == nullptr) {
			return "failed to get message from system";
		}

		// attempt to convert the system error message from "wide" string (UTF16 or UCS-2)
		// to "narrow/normal" UTF8
		let_mut bytes = static_cast(DWORD)(WideCharToMultiByte(65001 /**CP_UTF8**/, // NOLINT
															   0,
															   buffer, // NOLINT
															   static_cast(int)(wide_length + 1),
															   p,
															   static_cast(int)(alloc_size),
															   nullptr,
															   nullptr));
		// if conversion succeeded, trim the string and return it
		if(bytes != 0) {
			char* end = strchr(p, 0); // NOLINT
			// We just want the primary error message, so trim all but the first line
			while(end[-1] == '\n' || end[-1] == '\r') { // NOLINT
				--end;									// NOLINT
			}
			*end = '\0';
			let view = cnx_stringview_from(p, 0, (end - p));
			let_mut str = cnx_string_from(&view);
			free(p); // NOLINT
			return cnx_string_into_cstring(str);
		}

		free(p); // NOLINT

		// increase the allocation size and try again on the next iteration
		alloc_size += alloc_size >> 2U;
		// or bail if an error other than insufficient buffer size occurred
		error = GetLastError();
	} while(error == 0x7a /**ERROR_INSUFFICIENT_BUFFER**/); // NOLINT

	return "failed to get message from system";
}

i64 cnx_win32_category_get_last_error([[maybe_unused]] const CnxErrorCategory* restrict self) {
	return GetLastError();
}
#endif

CnxString cnx_error_format(const CnxFormat* restrict self, CnxFormatSpecifier specifier) {
	return cnx_error_format_with_allocator(self, specifier, cnx_allocator_new());
}

CnxString cnx_error_format_with_allocator(const CnxFormat* restrict self,
										  [[maybe_unused]] CnxFormatSpecifier specifier,
										  CnxAllocator allocator) {
	cnx_assert(specifier.m_type == CNX_FORMAT_TYPE_DEFAULT
				   || specifier.m_type == CNX_FORMAT_TYPE_DEBUG,
			   "Can only format CnxError with default or debug format specifier");

	let _self = static_cast(const CnxError*)(self->m_self);
	cnx_string_scoped message = cnx_string_from_with_allocator(
		trait_call(message, _self->m_error_category, _self->m_error_code),
		allocator);
	if(specifier.m_type == CNX_FORMAT_TYPE_DEBUG) {
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
