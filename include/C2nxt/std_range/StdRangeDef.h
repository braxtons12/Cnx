/// @file StdRangeDef.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief This module provides macro definitions for implementing and working with `StdRange(T)`
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

#ifndef STD_RANGE_DEF
#define STD_RANGE_DEF

#include <C2nxt/StdDef.h>

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
#define std_range_take_first_from_iterators(T, n, begin, end)                \
	({                                                                       \
		let_mut UNIQUE_VAR(range) = std_range_from_iterators(T, begin, end); \
		let_mut UNIQUE_VAR(current) = begin;                                 \
		for(let_mut i = 0; i < (n); ++i) {                                 \
			ignore(std_iterator_next(UNIQUE_VAR(current)));                  \
		}                                                                    \
		UNIQUE_VAR(range).m_end = UNIQUE_VAR(current);                       \
		UNIQUE_VAR(range);                                                   \
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
#define std_range_take_first(T, n, collection)                                   \
	({                                                                           \
		let_mut UNIQUE_VAR(begin) = (collection).m_vtable->begin(&(collection)); \
		let_mut UNIQUE_VAR(end) = (collection).m_vtable->end(&(collection));     \
		let_mut UNIQUE_VAR(begin_converted)                                      \
			= std_iterator_into_std_forward_iterator(UNIQUE_VAR(begin));         \
		let_mut UNIQUE_VAR(end_converted)                                        \
			= std_iterator_into_std_forward_iterator(UNIQUE_VAR(end));           \
		std_range_take_first_from_iterators(T,                                   \
											n,                                   \
											UNIQUE_VAR(begin_converted),         \
											UNIQUE_VAR(end_converted));          \
	})
/// @brief Collects the elements viewed by the range into a new `StdVector(T)`
///
/// Creates a new `StdVector(T)` and copies the elements viewed by `range` into it
///
/// # Requirements
/// - Requires that the `StdVector(T)` template has been instantiated for `T` and the instantiation
/// is in scope
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
#define std_transform_then_filter(T, collection, transform_function, filter_function)            \
	({                                                                                           \
		let_mut UNIQUE_VAR(range) = std_range_from(T, collection);                               \
		std_range_transform_then_filter(UNIQUE_VAR(range), transform_function, filter_functino); \
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

#endif // STD_RANGE_DEF
