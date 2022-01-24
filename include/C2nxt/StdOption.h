/// @file StdOption.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief This module provides a struct template for representing an optional value
/// @version 0.2
/// @date 2022-01-23
///
/// MIT License
/// @copyright Copyright (c) 2021 Braxton Salyer <braxtonsalyer@gmail.com>
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
/// #define STD_TEMPLATE_UNDEF_PARAMS
///
///	// instantiate the declarations of the `StdOption(T)` template
///	#define T MyType
/// #define STD_TEMPLATE_DECL TRUE
/// #include <C2nxt/StdOption.h>
///
///	// instantiate the definitions of the `StdOption(T)` template
///	#define T MyType
/// #define STD_TEMPLATE_DECL TRUE
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

#if !defined(STD_TEMPLATE_DECL) && (!defined(STD_TEMPLATE_IMPL) || !STD_TEMPLATE_IMPL) && defined(T)
	#define STD_TEMPLATE_DECL 1
#endif // !defined(STD_TEMPLATE_DECL) && (!defined(STD_TEMPLATE_IMPL) || !STD_TEMPLATE_IMPL) &&
	   // defined(T)

#if !defined(T) && STD_TEMPLATE_DECL
	#error StdOption.h included with STD_TEMPLATE_DECL defined true but template parameter T not defined
#endif // !defined(T) && STD_TEMPLATE_DECL

#if !defined(T) && STD_TEMPLATE_IMPL
	#error StdOption.h included with STD_TEMPLATE_IMPL defined true but template parameter T not defined
#endif // !defined(T) && STD_TEMPLATE_IMPL

#if defined(T) && STD_TEMPLATE_DECL && !STD_TEMPLATE_SUPPRESS_INSTANTIATIONS
	#include <C2nxt/std_option/StdOptionDecl.h>
#endif // defined(T) && STD_TEMPLATE_DECL && !STD_TEMPLATE_SUPPRESS_INSTANTIATIONS

#if defined(T) && STD_TEMPLATE_IMPL && !STD_TEMPLATE_SUPPRESS_INSTANTIATIONS
	#include <C2nxt/std_option/StdOptionImpl.h>
#endif // defined(T) && STD_TEMPLATE_IMPL && !STD_TEMPLATE_SUPPRESS_INSTANTIATIONS

#if !defined(T) && !defined(STD_TEMPLATE_DECL) && !defined(STD_TEMPLATE_IMPL) \
	&& !defined(STD_TEMPLATE_UNDEF_PARAMS)

	#ifndef STD_OPTION
		/// @brief `StdVector(T)` related declarations and definitions
		#define STD_OPTION

		#define T				  char
		#define STD_TEMPLATE_DECL 1
		#include <C2nxt/std_option/StdOptionDecl.h>
		#undef T
		#undef STD_TEMPLATE_DECL

		#define T				  u8
		#define STD_TEMPLATE_DECL 1
		#include <C2nxt/std_option/StdOptionDecl.h>
		#undef T
		#undef STD_TEMPLATE_DECL

		#define T				  u16
		#define STD_TEMPLATE_DECL 1
		#include <C2nxt/std_option/StdOptionDecl.h>
		#undef T
		#undef STD_TEMPLATE_DECL

		#define T				  u32
		#define STD_TEMPLATE_DECL 1
		#include <C2nxt/std_option/StdOptionDecl.h>
		#undef T
		#undef STD_TEMPLATE_DECL

		#define T				  u64
		#define STD_TEMPLATE_DECL 1
		#include <C2nxt/std_option/StdOptionDecl.h>
		#undef T
		#undef STD_TEMPLATE_DECL

		#define T				  usize
		#define STD_TEMPLATE_DECL 1
		#include <C2nxt/std_option/StdOptionDecl.h>
		#undef T
		#undef STD_TEMPLATE_DECL

		#define T				  i8
		#define STD_TEMPLATE_DECL 1
		#include <C2nxt/std_option/StdOptionDecl.h>
		#undef T
		#undef STD_TEMPLATE_DECL

		#define T				  i16
		#define STD_TEMPLATE_DECL 1
		#include <C2nxt/std_option/StdOptionDecl.h>
		#undef T
		#undef STD_TEMPLATE_DECL

		#define T				  i32
		#define STD_TEMPLATE_DECL 1
		#include <C2nxt/std_option/StdOptionDecl.h>
		#undef T
		#undef STD_TEMPLATE_DECL

		#define T				  i64
		#define STD_TEMPLATE_DECL 1
		#include <C2nxt/std_option/StdOptionDecl.h>
		#undef T
		#undef STD_TEMPLATE_DECL

		#define T				  isize
		#define STD_TEMPLATE_DECL 1
		#include <C2nxt/std_option/StdOptionDecl.h>
		#undef T
		#undef STD_TEMPLATE_DECL

		#define T				  f32
		#define STD_TEMPLATE_DECL 1
		#include <C2nxt/std_option/StdOptionDecl.h>
		#undef T
		#undef STD_TEMPLATE_DECL

		#define T				  f64
		#define STD_TEMPLATE_DECL 1
		#include <C2nxt/std_option/StdOptionDecl.h>
		#undef T
		#undef STD_TEMPLATE_DECL

		#define T				  u8_ptr
		#define STD_TEMPLATE_DECL 1
		#include <C2nxt/std_option/StdOptionDecl.h>
		#undef T
		#undef STD_TEMPLATE_DECL

		#define T				  u16_ptr
		#define STD_TEMPLATE_DECL 1
		#include <C2nxt/std_option/StdOptionDecl.h>
		#undef T
		#undef STD_TEMPLATE_DECL

		#define T				  u32_ptr
		#define STD_TEMPLATE_DECL 1
		#include <C2nxt/std_option/StdOptionDecl.h>
		#undef T
		#undef STD_TEMPLATE_DECL

		#define T				  u64_ptr
		#define STD_TEMPLATE_DECL 1
		#include <C2nxt/std_option/StdOptionDecl.h>
		#undef T
		#undef STD_TEMPLATE_DECL

		#define T				  usize_ptr
		#define STD_TEMPLATE_DECL 1
		#include <C2nxt/std_option/StdOptionDecl.h>
		#undef T
		#undef STD_TEMPLATE_DECL

		#define T				  i8_ptr
		#define STD_TEMPLATE_DECL 1
		#include <C2nxt/std_option/StdOptionDecl.h>
		#undef T
		#undef STD_TEMPLATE_DECL

		#define T				  i16_ptr
		#define STD_TEMPLATE_DECL 1
		#include <C2nxt/std_option/StdOptionDecl.h>
		#undef T
		#undef STD_TEMPLATE_DECL

		#define T				  i32_ptr
		#define STD_TEMPLATE_DECL 1
		#include <C2nxt/std_option/StdOptionDecl.h>
		#undef T
		#undef STD_TEMPLATE_DECL

		#define T				  i64_ptr
		#define STD_TEMPLATE_DECL 1
		#include <C2nxt/std_option/StdOptionDecl.h>
		#undef T
		#undef STD_TEMPLATE_DECL

		#define T				  isize_ptr
		#define STD_TEMPLATE_DECL 1
		#include <C2nxt/std_option/StdOptionDecl.h>
		#undef T
		#undef STD_TEMPLATE_DECL

		#define T				  f32_ptr
		#define STD_TEMPLATE_DECL 1
		#include <C2nxt/std_option/StdOptionDecl.h>
		#undef T
		#undef STD_TEMPLATE_DECL

		#define T				  f64_ptr
		#define STD_TEMPLATE_DECL 1
		#include <C2nxt/std_option/StdOptionDecl.h>
		#undef T
		#undef STD_TEMPLATE_DECL

		#define T				  cstring
		#define STD_TEMPLATE_DECL 1
		#include <C2nxt/std_option/StdOptionDecl.h>
		#undef T
		#undef STD_TEMPLATE_DECL

		#define T				  char_ptr
		#define STD_TEMPLATE_DECL 1
		#include <C2nxt/std_option/StdOptionDecl.h>
		#undef T
		#undef STD_TEMPLATE_DECL
	#endif // STD_OPTION

#endif // !defined(T) && !defined(STD_TEMPLATE_DECL) && !defined(STD_TEMPLATE_IMPL) \
	   // && !defined(STD_TEMPLATE_UNDEF_PARAMS)

#if STD_TEMPLATE_UNDEF_PARAMS
	#undef T
	#undef STD_TEMPLATE_DECL
	#undef STD_TEMPLATE_IMPL
#endif // STD_TEMPLATE_UNDEF_PARAMS
