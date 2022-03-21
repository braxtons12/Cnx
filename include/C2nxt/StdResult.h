/// @file StdResult.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief This module provides a struct template for representing the value of a fallible operation
/// @version 0.2.1
/// @date 2022-03-20
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

#if !defined(RESULT_DECL) && (!defined(RESULT_IMPL) || !RESULT_IMPL) && defined(RESULT_T)
	#define RESULT_DECL 1
#endif // !defined(RESULT_DECL) && (!defined(RESULT_IMPL) || !RESULT_IMPL) &&
// defined(RESULT_T)

#if !defined(RESULT_T) && RESULT_DECL && !RESULT_INCLUDE_DEFAULT_INSTANTIATIONS
	#error StdResult.h included with RESULT_DECL defined true but template parameter RESULT_T not defined
#endif // !defined(RESULT_T) && RESULT_DECL && !RESULT_INCLUDE_DEFAULT_INSTANTIATIONS

#if !defined(RESULT_T) && RESULT_IMPL && !RESULT_INCLUDE_DEFAULT_INSTANTIATIONS
	#error StdResult.h included with RESULT_IMPL defined true but template parameter RESULT_T not defined
#endif // !defined(RESULT_T) && RESULT_IMPL && !RESULT_INCLUDE_DEFAULT_INSTANTIATIONS

#if defined(RESULT_T) && RESULT_DECL && !STD_TEMPLATE_SUPPRESS_INSTANTIATIONS
	#include <C2nxt/std_result/StdResultDecl.h>
#endif // defined(RESULT_T) && RESULT_DECL && !STD_TEMPLATE_SUPPRESS_INSTANTIATIONS

#if defined(RESULT_T) && RESULT_IMPL && !STD_TEMPLATE_SUPPRESS_INSTANTIATIONS
	#include <C2nxt/std_result/StdResultImpl.h>
#endif // defined(RESULT_T) && RESULT_IMPL && !STD_TEMPLATE_SUPPRESS_INSTANTIATIONS

#if RESULT_INCLUDE_DEFAULT_INSTANTIATIONS && !defined(RESULT_T) \
	&& !STD_TEMPLATE_SUPPRESS_INSTANTIATIONS

	#ifndef STD_RESULT
		/// @brief `StdResult(T)` related declarations and definitions
		#define STD_RESULT

		#define RESULT_DECL TRUE

		#define RESULT_T char
		#include <C2nxt/std_result/StdResultDecl.h>
		#undef RESULT_T

		#define RESULT_T u8
		#include <C2nxt/std_result/StdResultDecl.h>
		#undef RESULT_T

		#define RESULT_T u16
		#include <C2nxt/std_result/StdResultDecl.h>
		#undef RESULT_T

		#define RESULT_T u32
		#include <C2nxt/std_result/StdResultDecl.h>
		#undef RESULT_T

		#define RESULT_T u64
		#include <C2nxt/std_result/StdResultDecl.h>
		#undef RESULT_T

		#define RESULT_T usize
		#include <C2nxt/std_result/StdResultDecl.h>
		#undef RESULT_T

		#define RESULT_T i8
		#include <C2nxt/std_result/StdResultDecl.h>
		#undef RESULT_T

		#define RESULT_T i16
		#include <C2nxt/std_result/StdResultDecl.h>
		#undef RESULT_T

		#define RESULT_T i32
		#include <C2nxt/std_result/StdResultDecl.h>
		#undef RESULT_T

		#define RESULT_T i64
		#include <C2nxt/std_result/StdResultDecl.h>
		#undef RESULT_T

		#define RESULT_T isize
		#include <C2nxt/std_result/StdResultDecl.h>
		#undef RESULT_T

		#define RESULT_T f32
		#include <C2nxt/std_result/StdResultDecl.h>
		#undef RESULT_T

		#define RESULT_T f64
		#include <C2nxt/std_result/StdResultDecl.h>
		#undef RESULT_T

		#define RESULT_T cstring
		#include <C2nxt/std_result/StdResultDecl.h>
		#undef RESULT_T

		#undef RESULT_DECL

	#endif // STD_RESULT

#endif // !defined(T) && !defined(RESULT_DECL) && !defined(RESULT_IMPL) \
	   // && !defined(RESULT_UNDEF_PARAMS)

#if RESULT_UNDEF_PARAMS
	#undef T
	#undef RESULT_DECL
	#undef RESULT_IMPL
#endif // RESULT_UNDEF_PARAMS
