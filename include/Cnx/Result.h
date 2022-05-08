/// @file Result.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief This module provides a struct template for representing the value of a fallible operation
/// @version 0.2.2
/// @date 2022-05-08
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

#include <Cnx/Def.h>
#include <Cnx/result/ResultDef.h>

/// @ingroup error_handling
/// @{
/// @defgroup cnx_result CnxResult
/// `CnxResult(T)` is a "struct template" for representing the value of a fallible operation. It
/// enables a simple, type-safe way of returning, communicating, and handling the result of an
/// operation that can fail, without resorting to heap-allocation and pointers or out-parameters.
///
/// `CnxResult(T)` allows for arbitrary user-defined errors through `CnxError`, and shares some
/// functionality, such as `if_let(var, self)`, `with `CnxOption(T)` (see @ref monadics).
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
/// [[maybe_unused]] static const CnxErrorCategory example_error_category
/// 	= {.m_message_function = example_error_message_function};
///
/// CnxResult(u32) operation_that_can_fail(u32 important, u32 right) {
///		if(important != IMPORTANT_VALUE) {
///			return Err(u32, cnx_error_new(EXAMPLE_IMPORTANT_BAD_VALUE, example_error_category));
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
///			let err = cnx_result_unwrap_err(maybe_value);
/// 		eprintln("Error in example: {}", as_format_t(CnxError, err));
///			// or, even more succinctly:
///			eprintln("Error in example: {}", as_format_t(CnxResult(T), maybe_value));
/// 	}
/// }
/// @endcode
/// @}

#if !defined(RESULT_DECL) && (!defined(RESULT_IMPL) || !RESULT_IMPL) && defined(RESULT_T)
	#define RESULT_DECL 1
#endif // !defined(RESULT_DECL) && (!defined(RESULT_IMPL) || !RESULT_IMPL) &&
// defined(RESULT_T)

#if !defined(RESULT_T) && RESULT_DECL && !RESULT_INCLUDE_DEFAULT_INSTANTIATIONS
	#error CnxResult.h included with RESULT_DECL defined true but template parameter RESULT_T not defined
#endif // !defined(RESULT_T) && RESULT_DECL && !RESULT_INCLUDE_DEFAULT_INSTANTIATIONS

#if !defined(RESULT_T) && RESULT_IMPL && !RESULT_INCLUDE_DEFAULT_INSTANTIATIONS
	#error CnxResult.h included with RESULT_IMPL defined true but template parameter RESULT_T not defined
#endif // !defined(RESULT_T) && RESULT_IMPL && !RESULT_INCLUDE_DEFAULT_INSTANTIATIONS

#if RESULT_DECL && RESULT_IMPL
	#define RESULT_STATIC static
	#define RESULT_INLINE inline
#else
	#ifndef RESULT_STATIC
		#define RESULT_STATIC
	#endif // RESULT_STATIC
	#ifndef RESULT_INLINE
		#define RESULT_INLINE
	#endif // RESULT_INLINE
#endif	   // RESULT_DECL && RESULT_IMPL

#if defined(RESULT_T) && RESULT_DECL && !CNX_TEMPLATE_SUPPRESS_INSTANTIATIONS
	#include <Cnx/result/ResultDecl.h>
#endif // defined(RESULT_T) && RESULT_DECL && !CNX_TEMPLATE_SUPPRESS_INSTANTIATIONS

#if defined(RESULT_T) && RESULT_IMPL && !CNX_TEMPLATE_SUPPRESS_INSTANTIATIONS
	#include <Cnx/result/ResultImpl.h>
#endif // defined(RESULT_T) && RESULT_IMPL && !CNX_TEMPLATE_SUPPRESS_INSTANTIATIONS

#if RESULT_INCLUDE_DEFAULT_INSTANTIATIONS && !defined(RESULT_T) \
	&& !CNX_TEMPLATE_SUPPRESS_INSTANTIATIONS

	#ifndef CNX_RESULT
		/// @brief `CnxResult(T)` related declarations and definitions
		#define CNX_RESULT

		#define RESULT_DECL TRUE

		#define RESULT_T char
		#include <Cnx/result/ResultDecl.h>
		#undef RESULT_T

		#define RESULT_T u8
		#include <Cnx/result/ResultDecl.h>
		#undef RESULT_T

		#define RESULT_T u16
		#include <Cnx/result/ResultDecl.h>
		#undef RESULT_T

		#define RESULT_T u32
		#include <Cnx/result/ResultDecl.h>
		#undef RESULT_T

		#define RESULT_T u64
		#include <Cnx/result/ResultDecl.h>
		#undef RESULT_T

		#define RESULT_T usize
		#include <Cnx/result/ResultDecl.h>
		#undef RESULT_T

		#define RESULT_T i8
		#include <Cnx/result/ResultDecl.h>
		#undef RESULT_T

		#define RESULT_T i16
		#include <Cnx/result/ResultDecl.h>
		#undef RESULT_T

		#define RESULT_T i32
		#include <Cnx/result/ResultDecl.h>
		#undef RESULT_T

		#define RESULT_T i64
		#include <Cnx/result/ResultDecl.h>
		#undef RESULT_T

		#define RESULT_T isize
		#include <Cnx/result/ResultDecl.h>
		#undef RESULT_T

		#define RESULT_T f32
		#include <Cnx/result/ResultDecl.h>
		#undef RESULT_T

		#define RESULT_T f64
		#include <Cnx/result/ResultDecl.h>
		#undef RESULT_T

		#define RESULT_T cstring
		#include <Cnx/result/ResultDecl.h>
		#undef RESULT_T

		#undef RESULT_DECL

typedef CnxResult(i32) CnxResult;

	#endif // CNX_RESULT

#endif // !defined(T) && !defined(RESULT_DECL) && !defined(RESULT_IMPL) \
	   // && !defined(RESULT_UNDEF_PARAMS)

#if RESULT_UNDEF_PARAMS
	#undef T
	#undef RESULT_DECL
	#undef RESULT_IMPL
#endif // RESULT_UNDEF_PARAMS

#ifdef RESULT_STATIC
	#undef RESULT_STATIC
#endif
#ifdef RESULT_INLINE
	#undef RESULT_INLINE
#endif
