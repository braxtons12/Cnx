/// @file Range.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief This module provides collection agnostic ways for passing, modifying, and working with
/// collections of elements as "Ranges"
/// @version 0.2.0
/// @date 2022-03-22
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

/// @defgroup ranges Ranges
/// Cnx Ranges provides a "struct template" enabling a lazy, collection-agnostic way to view,
/// filter, modify, and reference a collection's elements.
///
/// Ranges is one of the Cnx features that relies most heavily on GNU C extensions (currently), in
/// particular, `__auto_type` and statement expressions. It would benefit the most from C2x
/// standardization of `auto` and lambdas.
///
/// Requires:
/// 1. A Cnx compatible collection.
/// 2. `CnxRange(T)` has been fully instantiated for the type contained in your collection
/// 	(`<C2nxt/Range.h>` has been included and in scope with `RANGE_T` defined to the type and
/// 	and `RANGE_DECL` defined to `TRUE`, and it has been included in exactly one translation unit
/// 	in your binary with `RANGE_T` defined to the type and `RANGE_IMPL` defined to true).
///
/// A Cnx compatible collection provides:
/// 1. A vtable pointer member variable `m_vtable` which contains function pointers to the
/// 	collection's associated functions.
/// 2. Some category of Cnx compatible iterators (eg: `CnxForwardIterator(Ref(T))`).
/// 3. Functions to return begin and end iterators, named `begin` and `end`, respectively, in
/// 	`m_vtable`. They should take the signature
///
///			YourIteratorCategory(Ref(T)) (*const begin)(YourCollectionType* restrict self);
/// 		YourIteratorCategory(Ref(T)) (*const end)(YourCollectionType* restrict self);
///
/// If these are met, your collection can be used with `CnxRange(T)` and its abstractions.
///
/// Example usage of `CnxRange(T)`:
///
/// @code {.c}
/// #include <Cnx/Def.h>
///
/// #define VECTOR_INCLUDE_DEFAULT_INSTANTIATIONS TRUE
/// #include <Cnx/Vector.h>
/// #undef VECTOR_INCLUDE_DEFAULT_INSTANTIATIONS
///
/// #define RANGE_INCLUDE_DEFAULT_INSTANTIATIONS TRUE
/// #include <Cnx/Range.h>
/// #undef RANGE_INCLUDE_DEFAULT_INSTANTIATIONS
///
/// #include <Cnx/IO.h>
///
/// // example function to pass to cnx_transform, multiplies all elements by 3
/// void transform(u32* restrict elem) {
/// 	(*elem) *= 3;
/// }
///
/// // example function to pass to cnx_filter, hides odd elements from view
/// bool filter(const u32* restrict elem) {
///		return *elem % 2 == 0;
/// }
///
/// // example function to pass to cnx_accumulate, sums all elements
/// u32 accumulate(const u32* restrict current, const u32* restrict elem) {
///		return (*current) + (*elem);
/// }
///
/// void example(void) {
///		let_mut cnx_vector_scoped(u32) vec = cnx_vector_new(u32);
/// 	ranged_for(i, 0U, 10U) {
///			cnx_vector_push_back(vec, i);
/// 	}
///
///		let_mut range = cnx_range_from(u32, vec);
///
///		// prints the elements of `range` (thus the elements of `vec`),
/// 	// with a newline after each element
///		// you'll see 0 thru 9
/// 	foreach(elem, range) {
/// 		println("{}", elem);
/// 	}
///
///		// transform the elements with `transform`
///		// then filter them out of the view of the created range `range2` `with `filter`
///		let_mut range2 = cnx_range_transform_then_filter(range, transform, filter);
///		// print the elements in `range2`
/// 	//(thus the elements of `vec` `range2` still sees after `filter`),
/// 	// followed by a newline
///		// you'll see multiples of 6 from 0 thru 24
///		foreach(elem, range2) {
///			println("{}", elem);
/// 	}
///
///		// copy the elements viewed by range (thus the elements in `vec`)
///		// into a new vector `vec2`
///		let_mut cnx_vector_scoped(u32) vec2 = cnx_range_collect(u32, range);
///		// print the elements of `vec`
///		// you'll see multiples of 3 from 0 thru 27
///		foreach(elem, vec) {
///			println("{}", elem);
/// 	}
///		// print the elements of `vec2`
///		// you'll see multiples of 3 from 0 thru 27,
/// 	//the same output as for `vec`
///		foreach(elem, vec2) {
///			println("{}", elem);
/// 	}
///
///		// "accumulates" (in this case, sums), the elements in `vec2`
///		let acc = cnx_accumulate(u32, vec2, accumulate)
///		// prints the accumulation followed by a newline, you'll see 135
///		println("{}", acc);
/// }
///
///	// The total output you'll see from above is:
/// // 0
/// // 1
/// // 2
/// // 3
/// // 4
/// // 5
/// // 6
/// // 7
/// // 8
/// // 9
/// // 0
/// // 6
/// // 12
/// // 18
/// // 24
/// // 0
/// // 3
/// // 6
/// // 9
/// // 12
/// // 15
/// // 18
/// // 21
/// // 24
/// // 27
/// // 0
/// // 3
/// // 6
/// // 9
/// // 12
/// // 15
/// // 18
/// // 21
/// // 24
/// // 27
/// // 135
/// @endcode

#include <Cnx/range/RangeDef.h>

#if !defined(RANGE_DECL) && (!defined(RANGE_IMPL) || !RANGE_IMPL) && defined(RANGE_T)
	#define RANGE_DECL 1
#endif // !defined(RANGE_DECL) && (!defined(RANGE_IMPL) || !RANGE_IMPL) &&
	   // defined(RANGE_T)

#if !defined(RANGE_T) && RANGE_DECL && !RANGE_INCLUDE_DEFAULT_INSTANTIATIONS
	#error Range.h included with RANGE_DECL defined true but template parameter RANGE_T not defined
#endif // !defined(RANGE_T) && RANGE_DECL && !RANGE_INCLUDE_DEFAULT_INSTANTIATIONS

#if !defined(RANGE_T) && RANGE_IMPL && !RANGE_INCLUDE_DEFAULT_INSTANTIATIONS
	#error Range.h included with RANGE_IMPL defined true but template parameter RANGE_T not defined
#endif // !defined(RANGE_T) && RANGE_IMPL && !RANGE_INCLUDE_DEFAULT_INSTANTIATIONS

#if defined(RANGE_T) && RANGE_DECL && !CNX_TEMPLATE_SUPPRESS_INSTANTIATIONS
	#include <Cnx/range/RangeDecl.h>
#endif // defined(RANGE_T) && RANGE_DECL && !CNX_TEMPLATE_SUPPRESS_INSTANTIATIONS

#if defined(RANGE_T) && RANGE_IMPL && !CNX_TEMPLATE_SUPPRESS_INSTANTIATIONS
	#include <Cnx/range/RangeImpl.h>
#endif // defined(RANGE_T) && RANGE_IMPL && !CNX_TEMPLATE_SUPPRESS_INSTANTIATIONS

#if RANGE_INCLUDE_DEFAULT_INSTANTIATIONS && !defined(RANGE_T) \
	&& !CNX_TEMPLATE_SUPPRESS_INSTANTIATIONS

	#ifndef CNX_RANGE
		#define CNX_RANGE

		#define RANGE_DECL TRUE

		#define RANGE_T char
		#include <Cnx/range/RangeDecl.h>
		#undef RANGE_T

		#define RANGE_T u8
		#include <Cnx/range/RangeDecl.h>
		#undef RANGE_T

		#define RANGE_T u16
		#include <Cnx/range/RangeDecl.h>
		#undef RANGE_T

		#define RANGE_T u32
		#include <Cnx/range/RangeDecl.h>
		#undef RANGE_T

		#define RANGE_T u64
		#include <Cnx/range/RangeDecl.h>
		#undef RANGE_T

		#define RANGE_T usize
		#include <Cnx/range/RangeDecl.h>
		#undef RANGE_T

		#define RANGE_T i8
		#include <Cnx/range/RangeDecl.h>
		#undef RANGE_T

		#define RANGE_T i16
		#include <Cnx/range/RangeDecl.h>
		#undef RANGE_T

		#define RANGE_T i32
		#include <Cnx/range/RangeDecl.h>
		#undef RANGE_T

		#define RANGE_T i64
		#include <Cnx/range/RangeDecl.h>
		#undef RANGE_T

		#define RANGE_T isize
		#include <Cnx/range/RangeDecl.h>
		#undef RANGE_T

		#define RANGE_T f32
		#include <Cnx/range/RangeDecl.h>
		#undef RANGE_T

		#define RANGE_T f64
		#include <Cnx/range/RangeDecl.h>
		#undef RANGE_T

		#undef RANGE_DECL

	#endif // CNX_RANGE

#endif // RANGE_INCLUDE_DEFAULT_INSTANTIATIONS && !defined(RANGE_T) \
	   // && !CNX_TEMPLATE_SUPPRESS_INSTANTIATIONS
