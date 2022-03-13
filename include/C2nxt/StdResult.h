/// @file StdResult.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief This module provides a struct template for representing the value of a fallible operation
/// @version 0.2.0
/// @date 2022-03-12
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

#include <C2nxt/StdDef.h>
#include <C2nxt/std_result/StdResultDef.h>

/// @ingroup error_handling
/// @{
/// @defgroup std_result StdResult
/// `StdResult(T)` is a "struct template" for representing the value of a fallible operation. It
/// enables a simple, type-safe way of returning, communicating, and handling the result of an
/// operation that can fail, without resorting to heap-allocation and pointers or out-parameters.
///
/// `StdResult(T)` allows for arbitrary user-defined errors through `StdError`, and shares some
/// functionality, such as `if_let(var, self)`, `with `StdOption(T)` (see @ref monadics).
///
/// Example:
///
/// @code {.c}
/// #define IMPORTANT_VALUE 3U
///
///	typedef enum ExampleErrorTypes {
///		EXAMPLE_SUCCESS = 0,
/// 	EXAMPLE_IMPORTANT_BAD_VALUE
/// } ExampleErrorTypes;
///
/// const_cstring example_error_message_function(i64 error_code) {
///		if(error_code == EXAMPLE_IMPORTANT_BAD_VALUE) {
///			return "Error: bad value for `important`";
/// 	}
/// 	else {
///			return "No error: Example successful";
/// 	}
/// }
///
/// [[maybe_unused]] static const StdErrorCategory example_error_category
/// 	= {.m_message_function = example_error_message_function};
///
/// StdResult(u32) operation_that_can_fail(u32 important, u32 right) {
///		if(important != IMPORTANT_VALUE) {
///			return Err(u32, std_error_new(EXAMPLE_IMPORTANT_BAD_VALUE, example_error_category));
/// 	}
/// 	else {
///			let res = // result of some operation with `important` and `right`...
///			return Ok(u32, res);
/// 	}
/// }
///
/// void example(void) {
///		let_mut val1 = IMPORTANT_VALUE;
/// 	let_mut val2 = 10U;
///		// do something with val1 and/or val2...
///		let_mut maybe_value = operation_that_can_fail(val1, val2);
///		if_let(value, maybe_value) {
///			// do something with value...
/// 	}
/// 	else {
/// 		// maybe_value was `Err(u32)`, do something to recover
/// 		// or report the error, eg:...
///			let err = std_result_unwrap_err(maybe_value);
/// 		eprintln("Error in example: {}", as_format_t(StdError, err));
///			// or, even more succinctly:
///			eprintln("Error in example: {}", as_format_t(StdResult(T), maybe_value));
/// 	}
/// }
/// @endcode
/// @}

#if !defined(STD_TEMPLATE_DECL) && (!defined(STD_TEMPLATE_IMPL) || !STD_TEMPLATE_IMPL) && defined(T)
	#define STD_TEMPLATE_DECL 1
#endif // !defined(STD_TEMPLATE_DECL) && (!defined(STD_TEMPLATE_IMPL) || !STD_TEMPLATE_IMPL) &&
// defined(T)

#if !defined(T) && STD_TEMPLATE_DECL
	#error StdResult.h included with STD_TEMPLATE_DECL defined true but template parameter T not defined
#endif // !defined(T) && STD_TEMPLATE_DECL

#if !defined(T) && STD_TEMPLATE_IMPL
	#error StdResult.h included with STD_TEMPLATE_IMPL defined true but template parameter T not defined
#endif // !defined(T) && STD_TEMPLATE_IMPL

#if defined(T) && STD_TEMPLATE_DECL && !STD_TEMPLATE_SUPPRESS_INSTANTIATIONS
	#include <C2nxt/std_result/StdResultDecl.h>
#endif // defined(T) && STD_TEMPLATE_DECL && !STD_TEMPLATE_SUPPRESS_INSTANTIATIONS

#if defined(T) && STD_TEMPLATE_IMPL && !STD_TEMPLATE_SUPPRESS_INSTANTIATIONS
	#include <C2nxt/std_result/StdResultImpl.h>
#endif // defined(T) && STD_TEMPLATE_IMPL && !STD_TEMPLATE_SUPPRESS_INSTANTIATIONS

#if STD_TEMPLATE_INCLUDE_DEFAULT_INSTANTIATIONS && !defined(T) && !defined(STD_TEMPLATE_DECL) \
	&& !defined(STD_TEMPLATE_IMPL) && !defined(STD_TEMPLATE_UNDEF_PARAMS)                     \
	&& !STD_TEMPLATE_SUPPRESS_INSTANTIATIONS

	#ifndef STD_RESULT
		/// @brief `StdResult(T)` related declarations and definitions
		#define STD_RESULT

		#define STD_TEMPLATE_DECL TRUE

		#define T char
		#include <C2nxt/std_result/StdResultDecl.h>
		#undef T

		#define T u8
		#include <C2nxt/std_result/StdResultDecl.h>
		#undef T

		#define T u16
		#include <C2nxt/std_result/StdResultDecl.h>
		#undef T

		#define T u32
		#include <C2nxt/std_result/StdResultDecl.h>
		#undef T

		#define T u64
		#include <C2nxt/std_result/StdResultDecl.h>
		#undef T

		#define T usize
		#include <C2nxt/std_result/StdResultDecl.h>
		#undef T

		#define T i8
		#include <C2nxt/std_result/StdResultDecl.h>
		#undef T

		#define T i16
		#include <C2nxt/std_result/StdResultDecl.h>
		#undef T

		#define T i32
		#include <C2nxt/std_result/StdResultDecl.h>
		#undef T

		#define T i64
		#include <C2nxt/std_result/StdResultDecl.h>
		#undef T

		#define T isize
		#include <C2nxt/std_result/StdResultDecl.h>
		#undef T

		#define T f32
		#include <C2nxt/std_result/StdResultDecl.h>
		#undef T

		#define T f64
		#include <C2nxt/std_result/StdResultDecl.h>
		#undef T

		#define T cstring
		#include <C2nxt/std_result/StdResultDecl.h>
		#undef T

		#undef STD_TEMPLATE_DECL

	#endif // STD_RESULT

#endif // !defined(T) && !defined(STD_TEMPLATE_DECL) && !defined(STD_TEMPLATE_IMPL) \
	   // && !defined(STD_TEMPLATE_UNDEF_PARAMS)

#if STD_TEMPLATE_UNDEF_PARAMS
	#undef T
	#undef STD_TEMPLATE_DECL
	#undef STD_TEMPLATE_IMPL
#endif // STD_TEMPLATE_UNDEF_PARAMS
