/// @file StdRange.h
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
/// C2nxt Ranges provides a "struct template" enabling a lazy, collection-agnostic way to view,
/// filter, modify, and reference a collection's elements.
///
/// Ranges is one of the C2nxt features that relies most heavily on GNU C extensions (currently), in
/// particular, `__auto_type` and statement expressions. It would benefit the most from C2x
/// standardization of `auto` and lambdas.
///
/// Requires:
/// 1. A C2nxt compatible collection.
/// 2. `StdRange(T)` has been fully instantiated for the type contained in your collection
/// 	(`DeclStdRange(T)` has been used and is in scope, and `ImplStdRange(T)` has been used in a
/// 	translation unit that will be linked into the final binary).
///
/// A C2nxt compatible collection provides:
/// 1. A vtable pointer member variable `m_vtable` which contains function pointers to the
/// 	collection's associated functions.
/// 2. Some category of C2nxt compatible iterators (eg: `StdForwardIterator(Ref(T))`).
/// 3. Functions to return begin and end iterators, named `begin` and `end`, respectively, in
/// 	`m_vtable`. They should take the signature
///
///			YourIteratorCategory(Ref(T)) (*const begin)(YourCollectionType* restrict self);
/// 		YourIteratorCategory(Ref(T)) (*const end)(YourCollectionType* restrict self);
///
/// If these are met, your collection can be used with `StdRange(T)` and its abstractions.
///
/// Example usage of `StdRange(T)`:
///
/// @code {.c}
/// #include <C2nxt/StdDef.h>
///
/// #define VECTOR_INCLUDE_DEFAULT_INSTANTIATIONS TRUE
/// #include <C2nxt/StdVector.h>
/// #undef VECTOR_INCLUDE_DEFAULT_INSTANTIATIONS
///
/// #define RANGE_INCLUDE_DEFAULT_INSTANTIATIONS TRUE
/// #include <C2nxt/StdRange.h>
/// #undef RANGE_INCLUDE_DEFAULT_INSTANTIATIONS
///
/// #include <C2nxt/StdIO.h>
///
/// // example function to pass to std_transform, multiplies all elements by 3
/// void transform(u32* restrict elem) {
/// 	(*elem) *= 3;
/// }
///
/// // example function to pass to std_filter, hides odd elements from view
/// bool filter(const u32* restrict elem) {
///		return *elem % 2 == 0;
/// }
///
/// // example function to pass to std_accumulate, sums all elements
/// u32 accumulate(const u32* restrict current, const u32* restrict elem) {
///		return (*current) + (*elem);
/// }
///
/// void example(void) {
///		let_mut std_vector_scoped(u32) vec = std_vector_new(u32);
/// 	ranged_for(i, 0U, 10U) {
///			std_vector_push_back(vec, i);
/// 	}
///
///		let_mut range = std_range_from(u32, vec);
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
///		let_mut range2 = std_range_transform_then_filter(range, transform, filter);
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
///		let_mut std_vector_scoped(u32) vec2 = std_range_collect(u32, range);
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
///		let acc = std_accumulate(u32, vec2, accumulate)
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

#include <C2nxt/std_range/StdRangeDef.h>

#if !defined(RANGE_DECL) && (!defined(RANGE_IMPL) || !RANGE_IMPL) && defined(RANGE_T)
	#define RANGE_DECL 1
#endif // !defined(RANGE_DECL) && (!defined(RANGE_IMPL) || !RANGE_IMPL) &&
	   // defined(RANGE_T)

#if !defined(RANGE_T) && RANGE_DECL && !RANGE_INCLUDE_DEFAULT_INSTANTIATIONS
	#error StdRange.h included with RANGE_DECL defined true but template parameter RANGE_T not defined
#endif // !defined(RANGE_T) && RANGE_DECL && !RANGE_INCLUDE_DEFAULT_INSTANTIATIONS

#if !defined(RANGE_T) && RANGE_IMPL && !RANGE_INCLUDE_DEFAULT_INSTANTIATIONS
	#error StdRange.h included with RANGE_IMPL defined true but template parameter RANGE_T not defined
#endif // !defined(RANGE_T) && RANGE_IMPL && !RANGE_INCLUDE_DEFAULT_INSTANTIATIONS

#if defined(RANGE_T) && RANGE_DECL && !STD_TEMPLATE_SUPPRESS_INSTANTIATIONS
	#include <C2nxt/std_range/StdRangeDecl.h>
#endif // defined(RANGE_T) && RANGE_DECL && !STD_TEMPLATE_SUPPRESS_INSTANTIATIONS

#if defined(RANGE_T) && RANGE_IMPL && !STD_TEMPLATE_SUPPRESS_INSTANTIATIONS
	#include <C2nxt/std_range/StdRangeImpl.h>
#endif // defined(RANGE_T) && RANGE_IMPL && !STD_TEMPLATE_SUPPRESS_INSTANTIATIONS

#if RANGE_INCLUDE_DEFAULT_INSTANTIATIONS && !defined(RANGE_T) \
	&& !STD_TEMPLATE_SUPPRESS_INSTANTIATIONS

	#ifndef STD_RANGE
		#define STD_RANGE

		#define RANGE_DECL TRUE

		#define RANGE_T char
		#include <C2nxt/std_range/StdRangeDecl.h>
		#undef RANGE_T

		#define RANGE_T u8
		#include <C2nxt/std_range/StdRangeDecl.h>
		#undef RANGE_T

		#define RANGE_T u16
		#include <C2nxt/std_range/StdRangeDecl.h>
		#undef RANGE_T

		#define RANGE_T u32
		#include <C2nxt/std_range/StdRangeDecl.h>
		#undef RANGE_T

		#define RANGE_T u64
		#include <C2nxt/std_range/StdRangeDecl.h>
		#undef RANGE_T

		#define RANGE_T usize
		#include <C2nxt/std_range/StdRangeDecl.h>
		#undef RANGE_T

		#define RANGE_T i8
		#include <C2nxt/std_range/StdRangeDecl.h>
		#undef RANGE_T

		#define RANGE_T i16
		#include <C2nxt/std_range/StdRangeDecl.h>
		#undef RANGE_T

		#define RANGE_T i32
		#include <C2nxt/std_range/StdRangeDecl.h>
		#undef RANGE_T

		#define RANGE_T i64
		#include <C2nxt/std_range/StdRangeDecl.h>
		#undef RANGE_T

		#define RANGE_T isize
		#include <C2nxt/std_range/StdRangeDecl.h>
		#undef RANGE_T

		#define RANGE_T f32
		#include <C2nxt/std_range/StdRangeDecl.h>
		#undef RANGE_T

		#define RANGE_T f64
		#include <C2nxt/std_range/StdRangeDecl.h>
		#undef RANGE_T

		#undef RANGE_DECL

	#endif // STD_RANGE

#endif // RANGE_INCLUDE_DEFAULT_INSTANTIATIONS && !defined(RANGE_T) \
	   // && !STD_TEMPLATE_SUPPRESS_INSTANTIATIONS
