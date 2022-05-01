/// @file IO.c
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief Cnx I/O brings human readable formatted I/O, similar to C++'s `std::format` and
/// `fmtlib`, and Rust's std::format, to C.
/// @version 0.1.1
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

#include <Cnx/Platform.h>
#include <stdio.h>
// clang-format off
#if CNX_PLATFORM_COMPILER_CLANG && CNX_PLATFORM_APPLE
	_Pragma("GCC diagnostic push") _Pragma("GCC diagnostic ignored \"-Wnonportable-include-path\"")
#endif

#include <Cnx/IO.h>

#if CNX_PLATFORM_COMPILER_CLANG && CNX_PLATFORM_APPLE
	_Pragma("GCC diagnostic pop")
#endif

void print_(const_cstring restrict format_string, CnxAllocator allocator, usize num_args, ...) {
	va_list list = {0};
	va_start(list, num_args);
	CnxScopedString string
		= cnx_vformat_with_allocator(format_string, allocator, num_args, list);
	let cstr = cnx_string_into_cstring(string);
	let len = cnx_string_length(string);
	ignore(fwrite(cstr, sizeof(char), len, stdout));
	va_end(list);
}

// clang-format on

void eprint_(restrict const_cstring format_string, CnxAllocator allocator, usize num_args, ...) {
	va_list list = {0};
	va_start(list, num_args);
	CnxScopedString string = cnx_vformat_with_allocator(format_string, allocator, num_args, list);
	let cstr = cnx_string_into_cstring(string);
	let len = cnx_string_length(string);
	ignore(fwrite(cstr, sizeof(char), len, stderr));
	va_end(list);
}

void fprint_(FILE* file,
			 restrict const_cstring format_string,
			 CnxAllocator allocator,
			 usize num_args,
			 ...) {
	va_list list = {0};
	va_start(list, num_args);
	CnxScopedString string = cnx_vformat_with_allocator(format_string, allocator, num_args, list);
	let cstr = cnx_string_into_cstring(string);
	let len = cnx_string_length(string);
	ignore(fwrite(cstr, sizeof(char), len, file));
	va_end(list);
}

void println_(restrict const_cstring format_string, CnxAllocator allocator, usize num_args, ...) {
	va_list list = {0};
	va_start(list, num_args);
	CnxScopedString string = cnx_vformat_with_allocator(format_string, allocator, num_args, list);
	let cstr = cnx_string_into_cstring(string);
	let len = cnx_string_length(string);
	ignore(fwrite(cstr, sizeof(char), len, stdout));
	ignore(putc('\n', stdout));
	va_end(list);
}

void eprintln_(restrict const_cstring format_string, CnxAllocator allocator, usize num_args, ...) {
	va_list list = {0};
	va_start(list, num_args);
	CnxScopedString string = cnx_vformat_with_allocator(format_string, allocator, num_args, list);
	let cstr = cnx_string_into_cstring(string);
	let len = cnx_string_length(string);
	ignore(fwrite(cstr, sizeof(char), len, stderr));
	ignore(putc('\n', stderr));
	va_end(list);
}

void fprintln_(FILE* file,
			   restrict const_cstring format_string,
			   CnxAllocator allocator,
			   usize num_args,
			   ...) {
	va_list list = {0};
	va_start(list, num_args);
	CnxScopedString string = cnx_vformat_with_allocator(format_string, allocator, num_args, list);
	let cstr = cnx_string_into_cstring(string);
	let len = cnx_string_length(string);
	ignore(fwrite(cstr, sizeof(char), len, file));
	ignore(putc('\n', file));
#ifdef CNX_FPRINTLN_FLUSHES
	fflush(file);
#endif // CNX_FPRINTLN_FLUSHES
	va_end(list);
}
