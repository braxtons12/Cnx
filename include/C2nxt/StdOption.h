/// @file StdOption.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief This module provides a struct template for representing an optional value
/// @version 0.2.2
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
#include <C2nxt/StdEnum.h>
#include <C2nxt/std_option/StdOptionDef.h>

/// @ingroup error_handling
/// @{
///	@defgroup std_option StdOption
/// `StdOption(T)` is a "struct template" for an `Enum` representing an optional value.
/// It enables a simple, type-safe way of working with values that may or may not exist or be valid,
/// without resorting to heap-allocation and pointers or out-parameters.
///
///
/// Instantiating `StdOption(T)` for a given `T` is simple, and follows the same methodology as
/// other C2nxt templates:
///
/// Example:
/// @code {.c}
///	// have the template automatically undef macro template parameters
/// #define OPTION_UNDEF_PARAMS
///
///	// instantiate the declarations of the `StdOption(T)` template
///	#define T MyType
/// #define OPTION_DECL TRUE
/// #include <C2nxt/StdOption.h>
///
///	// instantiate the definitions of the `StdOption(T)` template
///	#define T MyType
/// #define OPTION_DECL TRUE
/// #include <C2nxt/StdOption.h>
/// @endcode
///
/// Once instantiated, `StdOption(T)` can be easily used:
/// Example:
/// @code {.c}
/// StdOption(u32) checked_add(u32 left, u32 right) {
/// 	return left <= std_max_value(u32) - right ?
/// 			Some(u32, left + right) :
/// 			None(u32);
/// }
///
/// void do_thing(void) {
///		let_mut left = 0;
///		let right = 10;
///		// do something with left and/or right ...
///		let_mut maybe_added = checked_add(left, right);
///
///		match(maybe_added) {
///			variant(Some, added) {
///				// do something with added
///			}
///			variant(None) {
///				maybe_added was the `None` variant, do something to recover
///			}
///		}
/// }
/// @endcode
/// @}

#if !defined(OPTION_DECL) && (!defined(OPTION_IMPL) || !OPTION_IMPL) \
	&& defined(OPTION_T)
	#define OPTION_DECL 1
#endif // !defined(OPTION_DECL) && (!defined(OPTION_IMPL) || !OPTION_IMPL) &&
	   // defined(OPTION_T)

#if !defined(OPTION_T) && OPTION_DECL && !OPTION_INCLUDE_DEFAULT_INSTANTIATIONS
	#error StdOption.h included with OPTION_DECL defined true but template parameter OPTION_T not defined
#endif // !defined(OPTION_T) && OPTION_DECL && !OPTION_INCLUDE_DEFAULT_INSTANTIATIONS

#if !defined(OPTION_T) && OPTION_IMPL && !OPTION_INCLUDE_DEFAULT_INSTANTIATIONS
	#error StdOption.h included with OPTION_IMPL defined true but template parameter OPTION_T not defined
#endif // !defined(OPTION_T) && OPTION_IMPL && !OPTION_INCLUDE_DEFAULT_INSTANTIATIONS

#if defined(OPTION_T) && OPTION_DECL && !STD_TEMPLATE_SUPPRESS_INSTANTIATIONS
	#include <C2nxt/std_option/StdOptionDecl.h>
#endif // defined(OPTION_T) && OPTION_DECL && !STD_TEMPLATE_SUPPRESS_INSTANTIATIONS

#if defined(OPTION_T) && OPTION_IMPL && !STD_TEMPLATE_SUPPRESS_INSTANTIATIONS
	#include <C2nxt/std_option/StdOptionImpl.h>
#endif // defined(OPTION_T) && OPTION_IMPL && !STD_TEMPLATE_SUPPRESS_INSTANTIATIONS

#if OPTION_INCLUDE_DEFAULT_INSTANTIATIONS && !defined(OPTION_T) \
	&& !STD_TEMPLATE_SUPPRESS_INSTANTIATIONS

	#ifndef STD_OPTION
		/// @brief `StdOption(T)` related declarations and definitions
		#define STD_OPTION

		#define OPTION_DECL TRUE

		#define OPTION_T char
		#include <C2nxt/std_option/StdOptionDecl.h>
		#undef OPTION_T

		#define OPTION_T u8
		#include <C2nxt/std_option/StdOptionDecl.h>
		#undef OPTION_T

		#define OPTION_T u16
		#include <C2nxt/std_option/StdOptionDecl.h>
		#undef OPTION_T

		#define OPTION_T u32
		#include <C2nxt/std_option/StdOptionDecl.h>
		#undef OPTION_T

		#define OPTION_T u64
		#include <C2nxt/std_option/StdOptionDecl.h>
		#undef OPTION_T

		#define OPTION_T usize
		#include <C2nxt/std_option/StdOptionDecl.h>
		#undef OPTION_T

		#define OPTION_T i8
		#include <C2nxt/std_option/StdOptionDecl.h>
		#undef OPTION_T

		#define OPTION_T i16
		#include <C2nxt/std_option/StdOptionDecl.h>
		#undef OPTION_T

		#define OPTION_T i32
		#include <C2nxt/std_option/StdOptionDecl.h>
		#undef OPTION_T

		#define OPTION_T i64
		#include <C2nxt/std_option/StdOptionDecl.h>
		#undef OPTION_T

		#define OPTION_T isize
		#include <C2nxt/std_option/StdOptionDecl.h>
		#undef OPTION_T

		#define OPTION_T f32
		#include <C2nxt/std_option/StdOptionDecl.h>
		#undef OPTION_T

		#define OPTION_T f64
		#include <C2nxt/std_option/StdOptionDecl.h>
		#undef OPTION_T

		#define OPTION_T cstring
		#include <C2nxt/std_option/StdOptionDecl.h>
		#undef OPTION_T

		#undef OPTION_DECL

	#endif // STD_OPTION

#endif // OPTION_INCLUDE_DEFAULT_INSTANTIATIONS && !defined(OPTION_T) \
	   // && !STD_TEMPLATE_SUPPRESS_INSTANTIATIONS

#if OPTION_UNDEF_PARAMS
	#undef OPTION_T
	#undef OPTION_DECL
	#undef OPTION_IMPL
#endif // OPTION_UNDEF_PARAMS
