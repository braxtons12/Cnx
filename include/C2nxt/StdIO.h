/// @file StdIO.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief StdIO brings human readable formatted I/O, similar to C++'s `std::format` and
/// `fmtlib`, and Rust's std::format, to C.
/// @version 0.1.2
/// @date 2022-03-09
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

#include <C2nxt/StdAllocators.h>
#include <C2nxt/StdBasicTypes.h>
#include <C2nxt/StdFormat.h>

/// @defgroup io I/O
/// C2nxt's I/O API extends its formatting API to I/O
///
/// Standard I/O in C2nxt has the same requirements, functionality, and syntax as string formatting
/// via<br> `std_format(format_string, ...)` and the `StdFormat` Trait. Currently only formatted
/// output is feature complete, but formatted input is planned.
///
/// Usage examples:
/// @code {.c}
/// // use it to perform basic logging:
/// #define LOG(format_string, ...) println((format_string) __VA_OPT__(,) __VA_ARGS__)
/// #define LOG_WITH_ALLOCATOR(format_string, allocator, ...) \
/// 				println_with_allocator((format_string), allocator, __VA_OPT__(,) __VA_ARGS__)
///
/// void func(void) {
/// 	let f = 0;
/// 	let y = 24;
/// 	let j = 42;
/// 	// do things with them...
///		LOG("f: {}, y: {}, j: {}", f, y, j);
///		// continue doing things...
/// }
/// // use it directly:
/// void func2(void) {
///		let_mut vec = std_vector_new(u32);
/// 	ranged_for(i, 0, 10) {
///			std_vector_push_back(vec, i);
/// 	}
/// 	println("vec: {}", as_format_t(StdVector(u32), vec));
/// }
/// @endcode

#ifndef STD_IO
	/// @brief Declarations and definitions for Standard I/O
	#define STD_IO

	#define ___DISABLE_IF_NULL(format_string) \
		std_disable_if(!(format_string), "Can't print with a null format string")
/// @brief Formats the given arguments into the specified format string then prints it to `stdout`
///
/// @param format_string - The format string specifying how text and arguments should be formatted
/// @param allocator - The allocator to allocate the formatted string with
/// @param num_args - The number of args in the parameter pack
/// @param ... - The parameter pack of arguments to format
[[not_null(1)]] void
print_(restrict const_cstring format_string, StdAllocator allocator, usize num_args, ...)
	___DISABLE_IF_NULL(format_string);
/// @brief Formats the given arguments into the specified format string then prints it to `stderr`
///
/// @param format_string - The format string specifying how text and arguments should be formatted
/// @param allocator - The allocator to allocate the formatted string with
/// @param num_args - The number of args in the parameter pack
/// @param ... - The parameter pack of arguments to format
[[not_null(1)]] void
eprint_(restrict const_cstring format_string, StdAllocator allocator, usize num_args, ...)
	___DISABLE_IF_NULL(format_string);
/// @brief Formats the given arguments into the specified format string then prints it to the given
/// file
///
/// @param file - The file to print the formatted string to
/// @param format_string - The format string specifying how text and arguments should be formatted
/// @param allocator - The allocator to allocate the formatted string with
/// @param num_args - The number of args in the parameter pack
/// @param ... - The parameter pack of arguments to format
[[not_null(1, 2)]] void fprint_(FILE* file,
								restrict const_cstring format_string,
								StdAllocator allocator,
								usize num_args,
								...) ___DISABLE_IF_NULL(format_string)
	std_disable_if(!file, "Can't print to a file that is nullptr");
/// @brief Formats the given arguments into the specified format string then prints it to `stdout`,
/// followed by a newline
///
/// @param format_string - The format string specifying how text and arguments should be formatted
/// @param allocator - The allocator to allocate the formatted string with
/// @param num_args - The number of args in the parameter pack
/// @param ... - The parameter pack of arguments to format
[[not_null(1)]] void
println_(restrict const_cstring format_string, StdAllocator allocator, usize num_args, ...)
	___DISABLE_IF_NULL(format_string);
/// @brief Formats the given arguments into the specified format string then prints it to `stderr`,
/// followed by a newline
///
/// @param format_string - The format string specifying how text and arguments should be formatted
/// @param allocator - The allocator to allocate the formatted string with
/// @param num_args - The number of args in the parameter pack
/// @param ... - The parameter pack of arguments to format
[[not_null(1)]] void
eprintln_(restrict const_cstring format_string, StdAllocator allocator, usize num_args, ...)
	___DISABLE_IF_NULL(format_string);
/// @brief Formats the given arguments into the specified format string then prints it to the given
/// file, followed by a newline
///
/// @param file - The file to print the formatted string to
/// @param format_string - The format string specifying how text and arguments should be formatted
/// @param allocator - The allocator to allocate the formatted string with
/// @param num_args - The number of args in the parameter pack
/// @param ... - The parameter pack of arguments to format
[[not_null(1, 2)]] void fprintln_(FILE* file,
								  restrict const_cstring format_string,
								  StdAllocator allocator,
								  usize num_args,
								  ...) ___DISABLE_IF_NULL(format_string)
	std_disable_if(!file, "Can't print to a file that is nullptr");

	/// @brief Formats the given arguments into the specified format string then prints it to the
	/// given file
	///
	/// Formats the arguments according to the given format string, allocating the output string and
	/// any intermediaries with the given allocator. Requires that the number of specifiers in the
	/// format string matches the number of arguments in the parameter pack and that all arguments
	/// in the parameter pack have an accessible automatic conversion to `StdFormat` via
	/// @ref STD_AS_FORMAT_USER_SUPPLIED_TYPES or have been explicitly converted to `StdFormat`
	/// already.
	///
	/// @param file_ptr - The file to print the formatted string to
	/// @param format_string - The format string specifying how text and arguments should be
	/// formatted
	/// @param allocator - The allocator to allocate the formatted string with
	/// @param ... - The parameter pack of arguments to format
	/// @ingroup io
	#define fprint_with_allocator(file_ptr, format_string, allocator, ...) \
		fprint_(file_ptr,                                                  \
				format_string,                                             \
				allocator,                                                 \
				PP_NUM_ARGS(__VA_ARGS__) __VA_OPT__(, APPLY_TO_LIST(as_format, __VA_ARGS__)))
	/// @brief Formats the given arguments into the specified format string then prints it to the
	/// given file
	///
	/// Formats the arguments according to the given format string, allocating the output string and
	/// any intermediaries with the default system allocator. Requires that the number of specifiers
	/// in the format string matches the number of arguments in the parameter pack and that all
	/// arguments in the parameter pack have an accessible automatic conversion to `StdFormat` via
	/// @ref STD_AS_FORMAT_USER_SUPPLIED_TYPES or have been explicitly converted to `StdFormat`
	/// already.
	///
	/// @param file_ptr - The file to print the formatted string to
	/// @param format_string - The format string specifying how text and arguments should be
	/// formatted
	/// @param ... - The parameter pack of arguments to format
	/// @ingroup io
	#define fprint(file_ptr, format_string, ...) \
		fprint_with_allocator(file_ptr,          \
							  format_string,     \
							  std_allocator_new() __VA_OPT__(, ) __VA_ARGS__)
	/// @brief Formats the given arguments into the specified format string then prints it to
	/// `stdout`
	///
	/// Formats the arguments according to the given format string, allocating the output string and
	/// any intermediaries with the given allocator. Requires that the number of specifiers in the
	/// format string matches the number of arguments in the parameter pack and that all arguments
	/// in the parameter pack have an accessible automatic conversion to `StdFormat` via
	/// @ref STD_AS_FORMAT_USER_SUPPLIED_TYPES or have been explicitly converted to `StdFormat`
	/// already.
	///
	/// @param format_string - The format string specifying how text and arguments should be
	/// formatted
	/// @param allocator - The allocator to allocate the formatted string with
	/// @param ... - The parameter pack of arguments to format
	/// @ingroup io
	#define print_with_allocator(format_string, allocator, ...) \
		print_(format_string,                                   \
			   allocator,                                       \
			   PP_NUM_ARGS(__VA_ARGS__) __VA_OPT__(, APPLY_TO_LIST(as_format, __VA_ARGS__)))
	/// @brief Formats the given arguments into the specified format string then prints it to
	/// `stdout`
	///
	/// Formats the arguments according to the given format string, allocating the output string and
	/// any intermediaries with the default system allocator. Requires that the number of specifiers
	/// in the format string matches the number of arguments in the parameter pack and that all
	/// arguments in the parameter pack have an accessible automatic conversion to `StdFormat` via
	/// @ref STD_AS_FORMAT_USER_SUPPLIED_TYPES or have been explicitly converted to `StdFormat`
	/// already.
	///
	/// @param format_string - The format string specifying how text and arguments should be
	/// formatted
	/// @param ... - The parameter pack of arguments to format
	/// @ingroup io
	#define print(format_string, ...) \
		print_with_allocator(format_string, std_allocator_new() __VA_OPT__(, ) __VA_ARGS__)
	/// @brief Formats the given arguments into the specified format string then prints it to
	/// `stderr`
	///
	/// Formats the arguments according to the given format string, allocating the output string and
	/// any intermediaries with the given allocator. Requires that the number of specifiers in the
	/// format string matches the number of arguments in the parameter pack and that all arguments
	/// in the parameter pack have an accessible automatic conversion to `StdFormat` via
	/// @ref STD_AS_FORMAT_USER_SUPPLIED_TYPES or have been explicitly converted to `StdFormat`
	/// already.
	///
	/// @param format_string - The format string specifying how text and arguments should be
	/// formatted
	/// @param allocator - The allocator to allocate the formatted string with
	/// @param ... - The parameter pack of arguments to format
	/// @ingroup io
	#define eprint_with_allocator(format_string, allocator, ...) \
		eprint_(format_string,                                   \
				allocator,                                       \
				PP_NUM_ARGS(__VA_ARGS__) __VA_OPT__(, APPLY_TO_LIST(as_format, __VA_ARGS__)))
	/// @brief Formats the given arguments into the specified format string then prints it to
	/// `stderr`
	///
	/// Formats the arguments according to the given format string, allocating the output string and
	/// any intermediaries with the default system allocator. Requires that the number of specifiers
	/// in the format string matches the number of arguments in the parameter pack and that all
	/// arguments in the parameter pack have an accessible automatic conversion to `StdFormat` via
	/// @ref STD_AS_FORMAT_USER_SUPPLIED_TYPES or have been explicitly converted to `StdFormat`
	/// already.
	///
	/// @param format_string - The format string specifying how text and arguments should be
	/// formatted
	/// @param ... - The parameter pack of arguments to format
	/// @ingroup io
	#define eprint(format_string, ...) \
		eprint_with_allocator(format_string, std_allocator_new() __VA_OPT__(, ) __VA_ARGS__)

	/// @brief Formats the given arguments into the specified format string then prints it to the
	/// given file, followed by a newline
	///
	/// Formats the arguments according to the given format string, allocating the output string and
	/// any intermediaries with the given allocator. Requires that the number of specifiers in the
	/// format string matches the number of arguments in the parameter pack and that all arguments
	/// in the parameter pack have an accessible automatic conversion to `StdFormat` via
	/// @ref STD_AS_FORMAT_USER_SUPPLIED_TYPES or have been explicitly converted to `StdFormat`
	/// already.
	///
	/// @param file_ptr - The file to print the formatted string to
	/// @param format_string - The format string specifying how text and arguments should be
	/// formatted
	/// @param allocator - The allocator to allocate the formatted string with
	/// @param ... - The parameter pack of arguments to format
	#define fprintln_with_allocator(file_ptr, format_string, allocator, ...) \
		fprintln_(file_ptr,                                                  \
				  format_string,                                             \
				  allocator,                                                 \
				  PP_NUM_ARGS(__VA_ARGS__) __VA_OPT__(, APPLY_TO_LIST(as_format, __VA_ARGS__)))
	/// @brief Formats the given arguments into the specified format string then prints it to the
	/// given file, followed by a newline
	///
	/// Formats the arguments according to the given format string, allocating the output string and
	/// any intermediaries with the default system allocator. Requires that the number of specifiers
	/// in the format string matches the number of arguments in the parameter pack and that all
	/// arguments in the parameter pack have an accessible automatic conversion to `StdFormat` via
	/// @ref STD_AS_FORMAT_USER_SUPPLIED_TYPES or have been explicitly converted to `StdFormat`
	/// already.
	///
	/// @param file_ptr - The file to print the formatted string to
	/// @param format_string - The format string specifying how text and arguments should be
	/// formatted
	/// @param ... - The parameter pack of arguments to format
	/// @ingroup io
	#define fprintln(file_ptr, format_string, ...) \
		fprintln_with_allocator(file_ptr,          \
								format_string,     \
								std_allocator_new() __VA_OPT__(, ) __VA_ARGS__)
	/// @brief Formats the given arguments into the specified format string then prints it to
	/// `stdout`, followed by a newline
	///
	/// Formats the arguments according to the given format string, allocating the output string and
	/// any intermediaries with the given allocator. Requires that the number of specifiers in the
	/// format string matches the number of arguments in the parameter pack and that all arguments
	/// in the parameter pack have an accessible automatic conversion to `StdFormat` via
	/// @ref STD_AS_FORMAT_USER_SUPPLIED_TYPES or have been explicitly converted to `StdFormat`
	/// already.
	///
	/// @param format_string - The format string specifying how text and arguments should be
	/// formatted
	/// @param allocator - The allocator to allocate the formatted string with
	/// @param ... - The parameter pack of arguments to format
	/// @ingroup io
	#define println_with_allocator(format_string, allocator, ...) \
		println_(format_string,                                   \
				 allocator,                                       \
				 PP_NUM_ARGS(__VA_ARGS__) __VA_OPT__(, APPLY_TO_LIST(as_format, __VA_ARGS__)))
	/// @brief Formats the given arguments into the specified format string then prints it to
	/// `stdout`, followed by a newline
	///
	/// Formats the arguments according to the given format string, allocating the output string and
	/// any intermediaries with the default system allocator. Requires that the number of specifiers
	/// in the format string matches the number of arguments in the parameter pack and that all
	/// arguments in the parameter pack have an accessible automatic conversion to `StdFormat` via
	/// @ref STD_AS_FORMAT_USER_SUPPLIED_TYPES or have been explicitly converted to `StdFormat`
	/// already.
	///
	/// @param format_string - The format string specifying how text and arguments should be
	/// formatted
	/// @param ... - The parameter pack of arguments to format
	/// @ingroup io
	#define println(format_string, ...) \
		println_with_allocator(format_string, std_allocator_new() __VA_OPT__(, ) __VA_ARGS__)
	/// @brief Formats the given arguments into the specified format string then prints it to
	/// `stderr`, followed by a newline
	///
	/// Formats the arguments according to the given format string, allocating the output string and
	/// any intermediaries with the given allocator. Requires that the number of specifiers in the
	/// format string matches the number of arguments in the parameter pack and that all arguments
	/// in the parameter pack have an accessible automatic conversion to `StdFormat` via
	/// @ref STD_AS_FORMAT_USER_SUPPLIED_TYPES or have been explicitly converted to `StdFormat`
	/// already.
	///
	/// @param format_string - The format string specifying how text and arguments should be
	/// formatted
	/// @param allocator - The allocator to allocate the formatted string with
	/// @param ... - The parameter pack of arguments to format
	/// @ingroup io
	#define eprintln_with_allocator(format_string, allocator, ...) \
		eprintln_(format_string,                                   \
				  allocator,                                       \
				  PP_NUM_ARGS(__VA_ARGS__) __VA_OPT__(, APPLY_TO_LIST(as_format, __VA_ARGS__)))
	/// @brief Formats the given arguments into the specified format string then prints it to
	/// `stderr`, followed by a newline
	///
	/// Formats the arguments according to the given format string, allocating the output string and
	/// any intermediaries with the default system allocator. Requires that the number of specifiers
	/// in the format string matches the number of arguments in the parameter pack and that all
	/// arguments in the parameter pack have an accessible automatic conversion to `StdFormat` via
	/// @ref STD_AS_FORMAT_USER_SUPPLIED_TYPES or have been explicitly converted to `StdFormat`
	/// already.
	///
	/// @param format_string - The format string specifying how text and arguments should be
	/// formatted
	/// @param ... - The parameter pack of arguments to format
	/// @ingroup io
	#define eprintln(format_string, ...) \
		eprintln_with_allocator(format_string, std_allocator_new() __VA_OPT__(, ) __VA_ARGS__)
#endif // STD_IO
