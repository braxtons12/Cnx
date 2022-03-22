/// @file StdRange.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief This module provides collection agnostic ways for passing, modifying, and working with
/// collections of elements as "Ranges"
/// @version 0.1
/// @date 2022-01-02
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

#include <C2nxt/StdIterator.h>
#include <stdio.h>

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
/// #include "C2nxt/StdVector.h"
/// #include "C2nxt/StdIO.h"
/// #include "C2nxt/StdRange.h"
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

#ifndef STD_RANGE
	/// @brief Declarations and definitions related to `StdRange(T)`
	#define STD_RANGE

	/// @brief Macro alias for an instantiation of `StdRange(T)` for the type `T`
	///
	/// Used for creating and referencing a typedef for a specific `StdRange(T)` instantiation
	///
	/// @param T - The type held by the collection `StdRange(T)` will abstract over
	/// @ingroup ranges
	#define StdRange(T) CONCAT2(StdRange, T)
	/// @brief Used to create an identifier for a function, variable, macro, or struct associated
	/// with a `StdRange(T)` instantiation
	#define StdRangeIdentifier(T, Identifier) CONCAT3(StdRange(T), _, Identifier)
	/// @brief Macro alias for filter function compatible with a `StdRange(T)` instantiation
	///
	/// Used for creating and referencing a typedef for a filter function compatible with a specific
	/// `StdRange(T)` instantiation.
	/// A `StdRangeFilter(T)` must take the signature:
	///
	/// @code {.c}
	/// bool (*filter)(const T* restrict elem);
	/// @endcode
	///
	/// @param T - The type held by the collection `StdRange(T)` will abstract over
	/// @ingroup ranges
	#define StdRangeFilter(T) StdRangeIdentifier(T, Filter)
	/// @brief Macro alias for transform function compatible with a `StdRange(T)` instantiation
	///
	/// Used for creating and referencing a typedef for a filter function compatible with a specific
	/// `StdRange(T)` instantiation.
	/// A `StdRangeTransform(T)` must take the signature:
	///
	/// @code
	/// void (*transform)(T* restrict elem);
	/// @endcode
	///
	/// @param T - The type held by the collection `StdRange(T)` will abstract over
	/// @ingroup ranges
	#define StdRangeTransform(T) StdRangeIdentifier(T, Transform)
	/// @brief Macro alias for accumulator function compatible with a `StdRange(T)` instantiation
	///
	/// Used for creating and referencing a typedef for a filter function compatible with a specific
	/// `StdRange(T)` instantiation.
	/// A `StdRangeAccumulator(T)` must take the signature:
	///
	/// @code
	/// T (*accumulator)(const T* current_value, const T* elem);
	/// @endcode
	///
	/// @param T - The type held by the collection `StdRange(T)` will abstract over
	/// @ingroup ranges
	#define StdRangeAccumulator(T) StdRangeIdentifier(T, Accumulator)

	/// @brief Instantiates `StdRange(T)` for the type `T`
	///
	/// Instantiates the necessary function and variable declarations, and struct definitions, for a
	/// `StdRange(T)` abstracting over a collection that holds `T`s.
	///
	/// @param T - The type held by the collection `StdRange(T)` will abstract over
	/// @ingroup ranges
	#define DeclStdRange(T)                                                                  \
		typedef bool (*StdRangeFilter(T))(const T* restrict element);                        \
		typedef void (*StdRangeTransform(T))(T* restrict element);                           \
		typedef T (*StdRangeAccumulator(T))(const T* restrict current_value,                 \
											const T* restrict element);                      \
		typedef struct StdRangeIdentifier(T, vtable) StdRangeIdentifier(T, vtable);          \
                                                                                             \
		typedef struct StdRange(T) {                                                         \
			StdForwardIterator(Ref(T)) m_begin;                                              \
			StdForwardIterator(Ref(T)) m_end;                                                \
			StdForwardIterator(Ref(T)) m_current;                                            \
			StdRangeFilter(T) m_filter;                                                      \
			const StdRangeIdentifier(T, vtable) * m_vtable;                                  \
		}                                                                                    \
		StdRange(T);                                                                         \
                                                                                             \
		StdRange(T) StdRangeIdentifier(T, from)(StdForwardIterator(Ref(T)) begin,            \
												StdForwardIterator(Ref(T)) end,              \
												StdRangeFilter(T) filter);                   \
		StdForwardIterator(Ref(T)) StdRangeIdentifier(T, begin)(StdRange(T)* restrict self); \
		StdForwardIterator(Ref(T)) StdRangeIdentifier(T, end)(StdRange(T)* restrict self);   \
                                                                                             \
		typedef struct StdRangeIdentifier(T, Iterator) {                                     \
			isize m_index;                                                                   \
			StdRange(T)* const m_range;                                                      \
		}                                                                                    \
		StdRangeIdentifier(T, Iterator);                                                     \
                                                                                             \
		DeclIntoStdForwardIterator(StdRange(T), Ref(T), StdRangeIdentifier(T, into_iter));   \
                                                                                             \
		typedef struct StdRangeIdentifier(T, vtable) {                                       \
			StdForwardIterator(Ref(T)) (*const begin)(StdRange(T)* restrict self);           \
			StdForwardIterator(Ref(T)) (*const end)(StdRange(T)* restrict self);             \
			StdForwardIterator(Ref(T)) (*const cbegin)(StdRange(T)* restrict self);          \
			StdForwardIterator(Ref(T)) (*const cend)(StdRange(T)* restrict self);            \
			StdForwardIterator(Ref(T)) (*const into_iter)(const StdRange(T)* restrict self); \
		}                                                                                    \
		StdRangeIdentifier(T, vtable);                                                       \
                                                                                             \
		bool StdRangeIdentifier(T, default_filter)(const T* elem);

	// clang-format off

/// @brief Creates a filtered `StdRange(T)`
///
/// Creates a `StdRange(T)` from the given iterators, filtered with the `StdRangeFilter(T)`,
/// `filter`
///
/// @param T - The type held by the collection `StdRange(T)` will abstract over
/// @param begin - The begin iterator for the collection
/// @param end - The end iterator for the collection
/// @param filter - The `StdRangeFilter(T)` to filter the range with
///
/// @return a `StdRange(T)` over the given iteration, filtered with `filter`
/// @ingroup ranges
#define std_range_from_iterators_filtered(T, begin, end, filter) _Generic((&(begin)),			   \
	StdForwardIterator(Ref(T)) *		    : 	StdRangeIdentifier(T, from)( 					   \
													*static_cast(StdForwardIterator(Ref(T))*)      \
													(&(begin)),                                    \
													*static_cast(StdForwardIterator(Ref(T))*)      \
													(&(end)),                                      \
													filter                                         \
												), 										           \
	StdReverseIterator(Ref(T)) *		    : 	StdRangeIdentifier(T, from)(					   \
													*static_cast(StdForwardIterator(Ref(T))*)      \
													(&(begin)),                                    \
													*static_cast(StdForwardIterator(Ref(T))*)      \
													(&(end)),                                      \
													filter                                         \
												), 										           \
	StdBidirectionalIterator(Ref(T))* 	    :	StdRangeIdentifier(T, from)(					   \
													std_iterator_into_std_forward_iterator(		   \
														*static_cast(							   \
															StdBidirectionalIterator(Ref(T))*      \
														)                                          \
														(&(begin))                                 \
													),	                                           \
													std_iterator_into_std_forward_iterator(		   \
														*static_cast(							   \
															StdBidirectionalIterator(Ref(T))*      \
														)                                          \
														(&(end))                                   \
													),	   	                                       \
													filter                                         \
												),										           \
	StdRandomAccessIterator(Ref(T))* 	    : 	StdRangeIdentifier(T, from)(					   \
													std_iterator_into_std_forward_iterator(		   \
														*static_cast(							   \
															StdRandomAccessIterator(Ref(T))*       \
														)                                          \
														(&(begin))                                 \
													),	   	                                       \
													std_iterator_into_std_forward_iterator(		   \
														*static_cast(							   \
															StdRandomAccessIterator(Ref(T))*       \
														)                                          \
														(&(end))                                   \
													),	   	                                       \
													filter                                         \
												),										           \
	const StdForwardIterator(Ref(T)) *	    : 	StdRangeIdentifier(T, from)(					   \
													*static_cast(                                  \
														StdForwardIterator(Ref(T))*                \
													)                                              \
													(&(begin)),                                    \
													*static_cast(                                  \
														StdForwardIterator(Ref(T))*                \
													)                                              \
													(&(end)),                                      \
													filter                                         \
												), 										           \
	const StdReverseIterator(Ref(T)) *	    : 	StdRangeIdentifier(T, from)(					   \
													*static_cast(                                  \
														StdForwardIterator(Ref(T))*                \
													)                                              \
													(&(begin)),                                    \
													*static_cast(                                  \
														StdForwardIterator(Ref(T))*                \
													)                                              \
													(&(end)),                                      \
													filter                                         \
												), 										           \
	const StdBidirectionalIterator(Ref(T))* :	StdRangeIdentifier(T, from)(				       \
													std_iterator_into_std_forward_iterator(		   \
														*static_cast(                              \
															StdBidirectionalIterator(Ref(T))*      \
														)                                          \
														(&(begin))                                 \
													),	                                           \
													std_iterator_into_std_forward_iterator(		   \
														*static_cast(							   \
															StdBidirectionalIterator(Ref(T))*      \
														)                                          \
														(&(end))                                   \
													),	                                           \
													filter                                         \
												),										           \
	const StdRandomAccessIterator(Ref(T))* 	: 	StdRangeIdentifier(T, from)(					   \
													std_iterator_into_std_forward_iterator(		   \
														*static_cast(							   \
															StdRandomAccessIterator(Ref(T))*       \
														)                                          \
														(&(begin))                                 \
													),	                                           \
													std_iterator_into_std_forward_iterator(		   \
														*static_cast(							   \
															StdRandomAccessIterator(Ref(T))*       \
														)                                          \
														(&(end))                                   \
													),	   	                                       \
													filter                                         \
												))
// clang-format on

	/// @brief Creates a `StdRange(T)`
	///
	/// Creates a `StdRange(T)` from the given iterators
	///
	/// @param T - The type held by the collection `StdRange(T)` will abstract over
	/// @param begin - The begin iterator for the collection
	/// @param end - The end iterator for the collection
	///
	/// @return a `StdRange(T)` over the given iteration
	/// @ingroup ranges
	#define std_range_from_iterators(T, begin, end) \
		std_range_from_iterators_filtered(T, begin, end, StdRangeIdentifier(T, default_filter))
	/// @brief Creates a filtered `StdRange(T)`
	///
	/// Creates a `StdRange(T)` from the given collection, filtered with the `StdRangeFilter(T)`,
	/// `filter`
	///
	/// @param T - The type held by the collection `StdRange(T)` will abstract over
	/// @param collection - The collection to abstract into a range
	/// @param filter - The `StdRangeFilter(T)` to filter the range with
	///
	/// @return a `StdRange(T)` over the given collection, filtered with `filter`
	/// @ingroup ranges
	#define std_range_from_filtered(T, collection, filter)                                      \
		({                                                                                      \
			let UNIQUE_VAR(begin) = (collection).m_vtable->begin(&(collection));                \
			let UNIQUE_VAR(end) = (collection).m_vtable->end(&(collection));                    \
			std_range_from_iterators_filtered(T, UNIQUE_VAR(begin), UNIQUE_VAR(end), (filter)); \
		})
	/// @brief Creates a `StdRange(T)`
	///
	/// Creates a `StdRange(T)` from the given collection
	///
	/// @param T - The type held by the collection `StdRange(T)` will abstract over
	/// @param collection - The collection to abstract into a range
	///
	/// @return a `StdRange(T)` over the given collection
	/// @ingroup ranges
	#define std_range_from(T, collection) \
		std_range_from_filtered(T, collection, StdRangeIdentifier(T, default_filter))

	/// @brief Returns an iterator associated with the given `StdRange(T)`
	///
	/// @param self - The `StdRange(T)` to get an iterator to
	///
	/// @return an iterator associated with `self`
	#define std_range_into_iter(self) (self).m_vtable->into_iter(&(self))
	/// @brief Returns at the beginning of the `StdRange(T)`
	///
	/// Returns a `StdForwardIterator(Ref(T))` into the given `StdRange(T)`, at the beginning of the
	/// iteration
	/// @param self - The `StdRange(T)` to get an iterator into
	///
	/// @return a `StdForwardIterator(Ref(T))` at the beginning of the range
	/// @ingroup ranges
	#define std_range_begin(self) (self).m_vtable->begin(&(self))
	/// @brief Returns at the end of the `StdRange(T)`
	///
	/// Returns a `StdForwardIterator(Ref(T))` into the given `StdRange(T)`, at the end of the
	/// iteration
	/// @param self - The `StdRange(T)` to get an iterator into
	///
	/// @return a `StdForwardIterator(Ref(T))` at the end of the range
	/// @ingroup ranges
	#define std_range_end(self) (self).m_vtable->end(&(self))
	/// @brief Creates a filtered `StdRange(T)`
	///
	/// Creates a `StdRange(T)` from the given iterators, filtered with the `StdRangeFilter(T)`,
	/// `filter`
	///
	/// @param T - The type held by the collection `StdRange(T)` will abstract over
	/// @param begin - The begin iterator for the collection
	/// @param end - The end iterator for the collection
	/// @param filter - The `StdRangeFilter(T)` to filter the range with
	///
	/// @return a `StdRange(T)` over the given iteration, filtered with `filter`
	/// @ingroup ranges
	#define std_range_filter_iterators(T, begin, end, filter) \
		std_range_from_iterators_filtered(T, begin, end, filter)
	/// @brief Creates a filtered `StdRange(T)`
	///
	/// Creates a `StdRange(T)` from the given collection, filtered with the `StdRangeFilter(T)`,
	/// `filter`
	///
	/// @param T - The type held by the collection `StdRange(T)` will abstract over
	/// @param collection - The collection to abstract into a range
	/// @param filter - The `StdRangeFilter(T)` to filter the range with
	///
	/// @return a `StdRange(T)` over the given collection, filtered with `filter`
	/// @ingroup ranges
	#define std_range_filter(T, collection, filter) std_range_from_filtered(T, collection, filter)
	/// @brief Creates a `StdRange(T)` over the first `n` elements in the iteration
	///
	/// Creates a `StdRange(T)` beginning at begin and ending at begin + n
	///
	/// @param T - The type held by the collection `StdRange(T)` will abstract over
	/// @param n - The number of elements in the iteration to view as the range
	/// @param begin - The begin iterator for the collection
	/// @param end - The end iterator for the collection
	///
	/// @return a `StdRange(T)` over the first n elements of the iteration
	/// @ingroup ranges
	#define std_range_take_first_from_iterators(T, n, begin, end)      \
		({                                                             \
			let_mut UNIQUE_VAR(range) = std_range_from(T, begin, end); \
			let_mut UNIQUE_VAR(current) = begin;                       \
			for(let_mut i = 0; i < (n); ++i) {                         \
				ignore(std_iterator_next(UNIQUE_VAR(current)));        \
			}                                                          \
			UNIQUE_VAR(range).m_end = UNIQUE_VAR(current);             \
			UNIQUE_VAR(range);                                         \
		})
	/// @brief Creates a `StdRange(T)` over the first `n` elements in the collection
	///
	/// Creates a `StdRange(T)` beginning at the beginning of the collection and ending at begin + n
	///
	/// @param T - The type held by the collection `StdRange(T)` will abstract over
	/// @param n - The number of elements in the iteration to view as the range
	/// @param collection - The collection to abstract into a range
	///
	/// @return a `StdRange(T)` over the first n elements of the iteration
	/// @ingroup ranges
	#define std_range_take_first(T, n, collection)                                         \
		({                                                                                 \
			let_mut UNIQUE_VAR(begin) = (collection).m_vtable->begin(&(collection));       \
			let_mut UNIQUE_VAR(end) = (collection).m_vtable->end(&(collection));           \
			std_range_take_first_from_iterators(T, n, UNIQUE_VAR(begin), UNIQUE_VAR(end)); \
		})
	/// @brief Collects the elements viewed by the range into a new `StdVector(T)`
	///
	/// Creates a new `StdVector(T)` and copies the elements viewed by `range` into it
	///
	/// @param T - The type held by the collection `StdRange(T)` will abstract over
	/// @param range - The `StdRange(T)` to collect
	///
	/// @return a `StdVector(T)` containing the elements viewed by `range`
	/// @ingroup ranges
	#define std_range_collect(T, range)                      \
		({                                                   \
			let_mut UNIQUE_VAR(vec) = std_vector_new(T);     \
			foreach(elem, range) {                           \
				std_vector_push_back(UNIQUE_VAR(vec), elem); \
			}                                                \
			UNIQUE_VAR(vec);                                 \
		})
	/// @brief Transforms the elements viewed by the `StdRange(T)`
	///
	/// Transforms the elements viewed by the range in place with the `StdRangeTransform(T)`
	/// function, `transform_function`
	///
	/// @param range - The `StdRange(T)` to transform
	/// @param transform_function - The `StdRangeTransform(T)` to transform elements with
	///
	/// @return The given range, after applying the transformation
	/// @ingroup ranges
	#define std_range_transform(range, transform_function)               \
		({                                                               \
			foreach_ref_mut(elem, range) { (transform_function)(elem); } \
                                                                         \
			(range);                                                     \
		})
	/// @brief Transforms the elements in the collection
	///
	/// Transforms the elements in the collection in place with the `StdRangeTransform(T)`
	/// function, `transform_function`, and returns a `StdRange(T)` viewing the collection
	///
	/// @param T - The type held by the collection `StdRange(T)` will abstract over
	/// @param collection - The collection to abstract into a range
	/// @param transform_function - The `StdRangeTransform(T)` to transform elements with
	///
	/// @return The given range, after applying the transformation
	/// @ingroup ranges
	#define std_transform(T, collection, transform_function)            \
		({                                                              \
			let_mut UNIQUE_VAR(range) = std_range_from(T, collection);  \
			std_range_transform(UNIQUE_VAR(range), transform_function); \
		})
	/// @brief Transforms the elements viewed by the `StdRange(T)`, then filters the range
	///
	/// Transforms the elements viewed by the range in place with the `StdRangeTransform(T)`
	/// function, `transform_function`, then filters the range with the `StdRangeFilter(T)`,
	/// `filter_function`
	///
	/// @param range - The `StdRange(T)` to transform and filter
	/// @param transform_function - The `StdRangeTransform(T)` to transform elements with
	/// @param filter_function - The `StdRangeFilter(T)` to filter the range with
	///
	/// @return The given range, after applying the transformation, filtered
	/// @ingroup ranges
	#define std_range_transform_then_filter(range, transform_function, filter_function)       \
		({                                                                                    \
			let_mut UNIQUE_VAR(transformed) = std_range_transform(range, transform_function); \
			UNIQUE_VAR(transformed).m_filter = (filter_function);                             \
			UNIQUE_VAR(transformed);                                                          \
		})
	/// @brief Transforms the elements in the collection, then returns a filtered `StdRange(T)` over
	/// the collection
	///
	/// Transforms the elements viewed in the collection in place with the `StdRangeTransform(T)`
	/// function, `transform_function`, then returns a range over the collection filtered with the
	/// `StdRangeFilter(T)`, `filter_function`
	///
	/// @param T - The type held by the collection `StdRange(T)` will abstract over
	/// @param collection - The collection to abstract into a range
	/// @param transform_function - The `StdRangeTransform(T)` to transform elements with
	/// @param filter_function - The `StdRangeFilter(T)` to filter the range with
	///
	/// @return The given range, after applying the transformation, filtered
	/// @ingroup ranges
	#define std_transform_then_filter(T, collection, transform_function, filter_function) \
		({                                                                                \
			let_mut UNIQUE_VAR(range) = std_range_from(T, collection);                    \
			std_range_transform_then_filter(UNIQUE_VAR(range),                            \
											transform_function,                           \
											filter_functino);                             \
		})
	/// @brief "Accumulates" the elements viewed by the `StdRange(T)`
	///
	/// "Accumulates" the elements viewed by the given `StdRange(T)` with the
	/// `StdRangeAccumulator(T)`, `accumulation_function`, returning the result
	///
	/// @param range - The `StdRange(T)` to accumulate the elements of
	/// @param accumulation_function - The `StdRangeAccumulator(T)` to accumulate elements with
	///
	/// @return The resulting accumulation
	/// @ingroup ranges
	#define std_range_accumulate(range, accumulation_function)                                     \
		({                                                                                         \
			let UNIQUE_VAR(_begin) = std_range_begin(range);                                       \
			let_mut UNIQUE_VAR(accumulation) = std_iterator_current(UNIQUE_VAR(_begin));           \
			foreach_ref(elem, range) {                                                             \
				UNIQUE_VAR(accumulation) = accumulation_function(&UNIQUE_VAR(accumulation), elem); \
			}                                                                                      \
			UNIQUE_VAR(accumulation);                                                              \
		})
	/// @brief "Accumulates" the elements in the collection
	///
	/// "Accumulates" the elements in the given collection with the
	/// `StdRangeAccumulator(T)`, `accumulation_function`, returning the result
	///
	/// @param T - The type held by the collection `StdRange(T)` will abstract over
	/// @param collection - The collection to accumulate
	/// @param accumulation_function - The `StdRangeAccumulator(T)` to accumulate elements with
	///
	/// @return The resulting accumulation
	/// @ingroup ranges
	#define std_accumulate(T, collection, accumulation_function)             \
		({                                                                   \
			let_mut UNIQUE_VAR(_range) = std_range_from(T, collection);      \
			std_range_accumulate(UNIQUE_VAR(_range), accumulation_function); \
		})

	/// @brief Instantiates `StdRange(T)` of the type `T`
	///
	/// Instantiates the necessary function and variable definitions for `StdRange(T)` of the type
	/// `T`. Requires that `DeclStdRange(T)` has been used and is in scope.
	///
	/// @param T - The type held by the collection `StdRange(T)` will abstract over
	/// @ingroup ranges
	#define ImplStdRange(T)                                                                        \
		[[maybe_unused]] static const StdRangeIdentifier(T, vtable)                                \
			StdRangeIdentifier(T, vtable_impl)                                                     \
			= {                                                                                    \
				.begin = StdRangeIdentifier(T, begin),                                             \
				.end = StdRangeIdentifier(T, end),                                                 \
				.cbegin = StdRangeIdentifier(T, begin),                                            \
				.cend = StdRangeIdentifier(T, end),                                                \
				.into_iter = StdRangeIdentifier(T, into_iter),                                     \
		};                                                                                         \
                                                                                                   \
		StdRangeIdentifier(T, Iterator)                                                            \
			StdRangeIdentifier(T, iterator_new)(const StdRange(T)* restrict self);                 \
		Ref(T) StdRangeIdentifier(T, iterator_next)(StdForwardIterator(Ref(T))* restrict self);    \
		Ref(T) StdRangeIdentifier(T, iterator_current)(                                            \
			const StdForwardIterator(Ref(T))* restrict self);                                      \
		bool StdRangeIdentifier(T,                                                                 \
								iterator_equals)(const StdForwardIterator(Ref(T))* restrict self,  \
												 const StdForwardIterator(Ref(T))* restrict rhs);  \
                                                                                                   \
		ImplIntoStdForwardIterator(StdRange(T),                                                    \
								   Ref(T),                                                         \
								   StdRangeIdentifier(T, into_iter),                               \
								   StdRangeIdentifier(T, iterator_new),                            \
								   StdRangeIdentifier(T, iterator_next),                           \
								   StdRangeIdentifier(T, iterator_current),                        \
								   StdRangeIdentifier(T, iterator_equals));                        \
                                                                                                   \
		StdRangeIdentifier(T, Iterator)                                                            \
			StdRangeIdentifier(T, iterator_new)(const StdRange(T)* restrict self) {                \
			let_mut _self = const_cast(StdRange(T)*)(self);                                        \
			/** HACK relying on all C2nxt compliant concrete iterator types to have the same       \
			 * layout **/                                                                          \
			let begin = static_cast(StdRangeIdentifier(T, Iterator)*)(self->m_begin.m_self);       \
			return (StdRangeIdentifier(T, Iterator)){                                              \
				.m_range = _self,                                                                  \
				.m_index = begin->m_index,                                                         \
			};                                                                                     \
		}                                                                                          \
                                                                                                   \
		Ref(T) StdRangeIdentifier(T, iterator_next)(StdForwardIterator(Ref(T))* restrict self) {   \
			let_mut _self = static_cast(StdRangeIdentifier(T, Iterator)*)(self->m_self);           \
			while(!std_iterator_equals(_self->m_range->m_current, _self->m_range->m_end)) {        \
				let current = &std_iterator_next(_self->m_range->m_current);                       \
				_self->m_index++;                                                                  \
				if(_self->m_range->m_filter(static_cast(const T*)(current))) {                     \
					return current;                                                                \
				}                                                                                  \
			}                                                                                      \
                                                                                                   \
			return &std_iterator_current(_self->m_range->m_begin);                                 \
		}                                                                                          \
                                                                                                   \
		Ref(T) StdRangeIdentifier(T, iterator_current)(                                            \
			const StdForwardIterator(Ref(T))* restrict self) {                                     \
			let_mut _self = static_cast(const StdRangeIdentifier(T, Iterator)*)(self->m_self);     \
			return &std_iterator_current(_self->m_range->m_current);                               \
		}                                                                                          \
                                                                                                   \
		bool StdRangeIdentifier(T,                                                                 \
								iterator_equals)(const StdForwardIterator(Ref(T))* restrict self,  \
												 const StdForwardIterator(Ref(T))* restrict rhs) { \
			let_mut _self = static_cast(const StdRangeIdentifier(T, Iterator)*)(self->m_self);     \
			let_mut _rhs = static_cast(const StdRangeIdentifier(T, Iterator)*)(rhs->m_self);       \
                                                                                                   \
			return _self->m_range == _rhs->m_range && _self->m_index == _rhs->m_index;             \
		}                                                                                          \
                                                                                                   \
		StdForwardIterator(Ref(T)) StdRangeIdentifier(T, begin)(StdRange(T)* restrict self) {      \
			let iter = std_range_into_iter(*self);                                                 \
			let_mut inner = static_cast(StdRangeIdentifier(T, Iterator)*)(iter.m_self);            \
			self->m_current = self->m_begin;                                                       \
			let_mut current = &std_iterator_current(self->m_current);                              \
			do {                                                                                   \
				if(self->m_filter(static_cast(const T*)(current))) {                               \
					return iter;                                                                   \
				}                                                                                  \
                                                                                                   \
				current = &std_iterator_next(self->m_current);                                     \
				inner->m_index++;                                                                  \
			} while(!std_iterator_equals(self->m_current, self->m_end));                           \
			return iter;                                                                           \
		}                                                                                          \
                                                                                                   \
		StdForwardIterator(Ref(T)) StdRangeIdentifier(T, end)(StdRange(T)* restrict self) {        \
			let_mut iter = std_range_into_iter(*self);                                             \
			let_mut inner = static_cast(StdRangeIdentifier(T, Iterator)*)(iter.m_self);            \
			/** HACK relying on all C2nxt compliant concrete iterator types to have the same       \
			 * layout **/                                                                          \
			let end = static_cast(StdRangeIdentifier(T, Iterator)*)(self->m_end.m_self);           \
			inner->m_index = end->m_index;                                                         \
			return iter;                                                                           \
		}                                                                                          \
                                                                                                   \
		StdRange(T) StdRangeIdentifier(T, from)(StdForwardIterator(Ref(T)) begin,                  \
												StdForwardIterator(Ref(T)) end,                    \
												StdRangeFilter(T) filter) {                        \
			return (StdRange(T)){.m_begin = begin,                                                 \
								 .m_end = end,                                                     \
								 .m_current = begin,                                               \
								 .m_filter = filter,                                               \
								 .m_vtable = &StdRangeIdentifier(T, vtable_impl)};                 \
		}                                                                                          \
                                                                                                   \
		bool StdRangeIdentifier(T, default_filter)(const T* elem) {                                \
			ignore(elem);                                                                          \
			return true;                                                                           \
		}

/// @brief Instantiates `StdRange(T)` for the builtin `char`
DeclStdRange(char);
/// @brief Instantiates `StdRange(T)` for the builtin `u8`
DeclStdRange(u8);
/// @brief Instantiates `StdRange(T)` for the builtin `u16`
DeclStdRange(u16);
/// @brief Instantiates `StdRange(T)` for the builtin `u32`
DeclStdRange(u32);
/// @brief Instantiates `StdRange(T)` for the builtin `u64`
DeclStdRange(u64);
/// @brief Instantiates `StdRange(T)` for the builtin `usize`
DeclStdRange(usize);
/// @brief Instantiates `StdRange(T)` for the builtin `i8`
DeclStdRange(i8);
/// @brief Instantiates `StdRange(T)` for the builtin `i16`
DeclStdRange(i16);
/// @brief Instantiates `StdRange(T)` for the builtin `i32`
DeclStdRange(i32);
/// @brief Instantiates `StdRange(T)` for the builtin `i64`
DeclStdRange(i64);
/// @brief Instantiates `StdRange(T)` for the builtin `isize`
DeclStdRange(isize);
/// @brief Instantiates `StdRange(T)` for the builtin `f32`
DeclStdRange(f32);
/// @brief Instantiates `StdRange(T)` for the builtin `f64`
DeclStdRange(f64);

#endif // STD_RANGE
