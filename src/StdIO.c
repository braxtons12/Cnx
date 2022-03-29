/// @file StdIO.c
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief StdIO brings human readable formatted I/O, similar to C++'s `std::format` and
/// `fmtlib`, and Rust's std::format, to C.
/// @version 0.1
/// @date 2021-08-15
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

#include <stdio.h>

#include "../include/C2nxt/StdPlatform.h"
// clang-format off
#if STD_PLATFORM_COMPILER_CLANG && STD_PLATFORM_APPLE
	_Pragma("GCC diagnostic push") _Pragma("GCC diagnostic ignored \"-Wnonportable-include-path\"")
#endif

#include "../include/C2nxt/StdIO.h"

#if STD_PLATFORM_COMPILER_CLANG && STD_PLATFORM_APPLE
	_Pragma("GCC diagnostic pop")
#endif

void print_(const_cstring restrict format_string, StdAllocator allocator, usize num_args, ...) {
	va_list list = {0};
	va_start(list, num_args);
	std_string_scoped string
		= std_vformat_with_allocator(format_string, allocator, num_args, list);
	puts(std_string_into_cstring(string));
	va_end(list);
}

// clang-format on

void eprint_(const_cstring restrict format_string, StdAllocator allocator, usize num_args, ...) {
	va_list list = {0};
	va_start(list, num_args);
	std_string_scoped string
		= std_vformat_with_allocator(format_string, allocator, num_args, list);
	ignore(fputs(std_string_into_cstring(string), stderr));
	va_end(list);
}

void fprint_(FILE* file,
			 const_cstring restrict format_string,
			 StdAllocator allocator,
			 usize num_args,
			 ...) {
	va_list list = {0};
	va_start(list, num_args);
	std_string_scoped string
		= std_vformat_with_allocator(format_string, allocator, num_args, list);
	ignore(fputs(std_string_into_cstring(string), file));
	va_end(list);
}

void println_(const_cstring restrict format_string, StdAllocator allocator, usize num_args, ...) {
	va_list list = {0};
	va_start(list, num_args);
	std_string_scoped string
		= std_vformat_with_allocator(format_string, allocator, num_args, list);
	puts(std_string_into_cstring(string));
	va_end(list);
}

void eprintln_(const_cstring restrict format_string, StdAllocator allocator, usize num_args, ...) {
	va_list list = {0};
	va_start(list, num_args);
	std_string_scoped string
		= std_vformat_with_allocator(format_string, allocator, num_args, list);
	ignore(fputs(std_string_into_cstring(string), stderr));
	ignore(putc('\n', stderr));
	va_end(list);
}

void fprintln_(FILE* file,
			   const_cstring restrict format_string,
			   StdAllocator allocator,
			   usize num_args,
			   ...) {
	va_list list = {0};
	va_start(list, num_args);
	std_string_scoped string
		= std_vformat_with_allocator(format_string, allocator, num_args, list);
	ignore(fputs(std_string_into_cstring(string), file));
	ignore(putc('\n', file));
#ifdef STD_FPRINTLN_FLUSHES
	fflush(file);
#endif // STD_FPRINTLN_FLUSHES
	va_end(list);
}
