/// @file RangeDef.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief This module provides macro definitions for implementing and working with `CnxRange(T)`
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

#ifndef CNX_RANGE_DEF
#define CNX_RANGE_DEF

#include <Cnx/Def.h>

/// @brief Macro alias for an instantiation of `CnxRange(T)` for the type `T`
///
/// Used for creating and referencing a typedef for a specific `CnxRange(T)` instantiation
///
/// @param T - The type held by the collection `CnxRange(T)` will abstract over
/// @ingroup ranges
#define CnxRange(T) CONCAT2(CnxRange, T)
/// @brief Used to create an identifier for a function, variable, macro, or struct associated
/// with a `CnxRange(T)` instantiation
#define CnxRangeIdentifier(T, Identifier) CONCAT4(cnx_range_, T, _, Identifier)
/// @brief Macro alias for filter function compatible with a `CnxRange(T)` instantiation
///
/// Used for creating and referencing a typedef for a filter function compatible with a specific
/// `CnxRange(T)` instantiation.
/// A `CnxRangeFilter(T)` must take the signature:
///
/// @code {.c}
/// bool (*filter)(const T* restrict elem);
/// @endcode
///
/// @param T - The type held by the collection `CnxRange(T)` will abstract over
/// @ingroup ranges
#define CnxRangeFilter(T) CnxRangeIdentifier(T, Filter)
/// @brief Macro alias for transform function compatible with a `CnxRange(T)` instantiation
///
/// Used for creating and referencing a typedef for a filter function compatible with a specific
/// `CnxRange(T)` instantiation.
/// A `CnxRangeTransform(T)` must take the signature:
///
/// @code
/// void (*transform)(T* restrict elem);
/// @endcode
///
/// @param T - The type held by the collection `CnxRange(T)` will abstract over
/// @ingroup ranges
#define CnxRangeTransform(T) CnxRangeIdentifier(T, Transform)
/// @brief Macro alias for accumulator function compatible with a `CnxRange(T)` instantiation
///
/// Used for creating and referencing a typedef for a filter function compatible with a specific
/// `CnxRange(T)` instantiation.
/// A `CnxRangeAccumulator(T)` must take the signature:
///
/// @code
/// T (*accumulator)(const T* current_value, const T* elem);
/// @endcode
///
/// @param T - The type held by the collection `CnxRange(T)` will abstract over
/// @ingroup ranges
#define CnxRangeAccumulator(T) CnxRangeIdentifier(T, Accumulator)

// clang-format off

/// @brief Creates a filtered `CnxRange(T)`
///
/// Creates a `CnxRange(T)` from the given iterators, filtered with the `CnxRangeFilter(T)`,
/// `filter`
///
/// @param T - The type held by the collection `CnxRange(T)` will abstract over
/// @param begin - The begin iterator for the collection
/// @param end - The end iterator for the collection
/// @param filter - The `CnxRangeFilter(T)` to filter the range with
///
/// @return a `CnxRange(T)` over the given iteration, filtered with `filter`
/// @ingroup ranges
#define cnx_range_from_iterators_filtered(T, begin, end, filter) _Generic((&(begin)),			   \
	CnxForwardIterator(Ref(T)) *		    : 	CnxRangeIdentifier(T, from)( 					   \
													*static_cast(CnxForwardIterator(Ref(T))*)      \
													(&(begin)),                                    \
													*static_cast(CnxForwardIterator(Ref(T))*)      \
													(&(end)),                                      \
													filter                                         \
												), 										           \
	CnxReverseIterator(Ref(T)) *		    : 	CnxRangeIdentifier(T, from)(					   \
													*static_cast(CnxForwardIterator(Ref(T))*)      \
													(&(begin)),                                    \
													*static_cast(CnxForwardIterator(Ref(T))*)      \
													(&(end)),                                      \
													filter                                         \
												), 										           \
	CnxBidirectionalIterator(Ref(T))* 	    :	CnxRangeIdentifier(T, from)(					   \
													cnx_iterator_into_forward_iterator(		   \
														*static_cast(							   \
															CnxBidirectionalIterator(Ref(T))*      \
														)                                          \
														(&(begin))                                 \
													),	                                           \
													cnx_iterator_into_forward_iterator(		   \
														*static_cast(							   \
															CnxBidirectionalIterator(Ref(T))*      \
														)                                          \
														(&(end))                                   \
													),	   	                                       \
													filter                                         \
												),										           \
	CnxRandomAccessIterator(Ref(T))* 	    : 	CnxRangeIdentifier(T, from)(					   \
													cnx_iterator_into_forward_iterator(		   \
														*static_cast(							   \
															CnxRandomAccessIterator(Ref(T))*       \
														)                                          \
														(&(begin))                                 \
													),	   	                                       \
													cnx_iterator_into_forward_iterator(		   \
														*static_cast(							   \
															CnxRandomAccessIterator(Ref(T))*       \
														)                                          \
														(&(end))                                   \
													),	   	                                       \
													filter                                         \
												),										           \
	const CnxForwardIterator(Ref(T)) *	    : 	CnxRangeIdentifier(T, from)(					   \
													*static_cast(                                  \
														CnxForwardIterator(Ref(T))*                \
													)                                              \
													(&(begin)),                                    \
													*static_cast(                                  \
														CnxForwardIterator(Ref(T))*                \
													)                                              \
													(&(end)),                                      \
													filter                                         \
												), 										           \
	const CnxReverseIterator(Ref(T)) *	    : 	CnxRangeIdentifier(T, from)(					   \
													*static_cast(                                  \
														CnxForwardIterator(Ref(T))*                \
													)                                              \
													(&(begin)),                                    \
													*static_cast(                                  \
														CnxForwardIterator(Ref(T))*                \
													)                                              \
													(&(end)),                                      \
													filter                                         \
												), 										           \
	const CnxBidirectionalIterator(Ref(T))* :	CnxRangeIdentifier(T, from)(				       \
													cnx_iterator_into_forward_iterator(		   \
														*static_cast(                              \
															CnxBidirectionalIterator(Ref(T))*      \
														)                                          \
														(&(begin))                                 \
													),	                                           \
													cnx_iterator_into_forward_iterator(		   \
														*static_cast(							   \
															CnxBidirectionalIterator(Ref(T))*      \
														)                                          \
														(&(end))                                   \
													),	                                           \
													filter                                         \
												),										           \
	const CnxRandomAccessIterator(Ref(T))* 	: 	CnxRangeIdentifier(T, from)(					   \
													cnx_iterator_into_forward_iterator(		   \
														*static_cast(							   \
															CnxRandomAccessIterator(Ref(T))*       \
														)                                          \
														(&(begin))                                 \
													),	                                           \
													cnx_iterator_into_forward_iterator(		   \
														*static_cast(							   \
															CnxRandomAccessIterator(Ref(T))*       \
														)                                          \
														(&(end))                                   \
													),	   	                                       \
													filter                                         \
												))
// clang-format on

/// @brief Creates a `CnxRange(T)`
///
/// Creates a `CnxRange(T)` from the given iterators
///
/// @param T - The type held by the collection `CnxRange(T)` will abstract over
/// @param begin - The begin iterator for the collection
/// @param end - The end iterator for the collection
///
/// @return a `CnxRange(T)` over the given iteration
/// @ingroup ranges
#define cnx_range_from_iterators(T, begin, end) \
	cnx_range_from_iterators_filtered(T, begin, end, CnxRangeIdentifier(T, default_filter))
/// @brief Creates a filtered `CnxRange(T)`
///
/// Creates a `CnxRange(T)` from the given collection, filtered with the `CnxRangeFilter(T)`,
/// `filter`
///
/// @param T - The type held by the collection `CnxRange(T)` will abstract over
/// @param collection - The collection to abstract into a range
/// @param filter - The `CnxRangeFilter(T)` to filter the range with
///
/// @return a `CnxRange(T)` over the given collection, filtered with `filter`
/// @ingroup ranges
#define cnx_range_from_filtered(T, collection, filter)                                      \
	({                                                                                      \
		let UNIQUE_VAR(begin) = (collection).m_vtable->begin(&(collection));                \
		let UNIQUE_VAR(end) = (collection).m_vtable->end(&(collection));                    \
		cnx_range_from_iterators_filtered(T, UNIQUE_VAR(begin), UNIQUE_VAR(end), (filter)); \
	})
/// @brief Creates a `CnxRange(T)`
///
/// Creates a `CnxRange(T)` from the given collection
///
/// @param T - The type held by the collection `CnxRange(T)` will abstract over
/// @param collection - The collection to abstract into a range
///
/// @return a `CnxRange(T)` over the given collection
/// @ingroup ranges
#define cnx_range_from(T, collection) \
	cnx_range_from_filtered(T, collection, CnxRangeIdentifier(T, default_filter))

/// @brief Returns an iterator associated with the given `CnxRange(T)`
///
/// @param self - The `CnxRange(T)` to get an iterator to
///
/// @return an iterator associated with `self`
#define cnx_range_into_iter(self) (self).m_vtable->into_iter(&(self))
/// @brief Returns at the beginning of the `CnxRange(T)`
///
/// Returns a `CnxForwardIterator(Ref(T))` into the given `CnxRange(T)`, at the beginning of the
/// iteration
/// @param self - The `CnxRange(T)` to get an iterator into
///
/// @return a `CnxForwardIterator(Ref(T))` at the beginning of the range
/// @ingroup ranges
#define cnx_range_begin(self) (self).m_vtable->begin(&(self))
/// @brief Returns at the end of the `CnxRange(T)`
///
/// Returns a `CnxForwardIterator(Ref(T))` into the given `CnxRange(T)`, at the end of the
/// iteration
/// @param self - The `CnxRange(T)` to get an iterator into
///
/// @return a `CnxForwardIterator(Ref(T))` at the end of the range
/// @ingroup ranges
#define cnx_range_end(self) (self).m_vtable->end(&(self))
/// @brief Creates a filtered `CnxRange(T)`
///
/// Creates a `CnxRange(T)` from the given iterators, filtered with the `CnxRangeFilter(T)`,
/// `filter`
///
/// @param T - The type held by the collection `CnxRange(T)` will abstract over
/// @param begin - The begin iterator for the collection
/// @param end - The end iterator for the collection
/// @param filter - The `CnxRangeFilter(T)` to filter the range with
///
/// @return a `CnxRange(T)` over the given iteration, filtered with `filter`
/// @ingroup ranges
#define cnx_range_filter_iterators(T, begin, end, filter) \
	cnx_range_from_iterators_filtered(T, begin, end, filter)
/// @brief Creates a filtered `CnxRange(T)`
///
/// Creates a `CnxRange(T)` from the given collection, filtered with the `CnxRangeFilter(T)`,
/// `filter`
///
/// @param T - The type held by the collection `CnxRange(T)` will abstract over
/// @param collection - The collection to abstract into a range
/// @param filter - The `CnxRangeFilter(T)` to filter the range with
///
/// @return a `CnxRange(T)` over the given collection, filtered with `filter`
/// @ingroup ranges
#define cnx_range_filter(T, collection, filter) cnx_range_from_filtered(T, collection, filter)
/// @brief Creates a `CnxRange(T)` over the first `n` elements in the iteration
///
/// Creates a `CnxRange(T)` beginning at begin and ending at begin + n
///
/// @param T - The type held by the collection `CnxRange(T)` will abstract over
/// @param n - The number of elements in the iteration to view as the range
/// @param begin - The begin iterator for the collection
/// @param end - The end iterator for the collection
///
/// @return a `CnxRange(T)` over the first n elements of the iteration
/// @ingroup ranges
#define cnx_range_take_first_from_iterators(T, n, begin, end)                \
	({                                                                       \
		let_mut UNIQUE_VAR(range) = cnx_range_from_iterators(T, begin, end); \
		let_mut UNIQUE_VAR(current) = begin;                                 \
		for(let_mut i = 0; i < (n); ++i) {                                   \
			ignore(cnx_iterator_next(UNIQUE_VAR(current)));                  \
		}                                                                    \
		UNIQUE_VAR(range).m_end = UNIQUE_VAR(current);                       \
		UNIQUE_VAR(range);                                                   \
	})
/// @brief Creates a `CnxRange(T)` over the first `n` elements in the collection
///
/// Creates a `CnxRange(T)` beginning at the beginning of the collection and ending at begin + n
///
/// @param T - The type held by the collection `CnxRange(T)` will abstract over
/// @param n - The number of elements in the iteration to view as the range
/// @param collection - The collection to abstract into a range
///
/// @return a `CnxRange(T)` over the first n elements of the iteration
/// @ingroup ranges
#define cnx_range_take_first(T, n, collection)                                                   \
	({                                                                                           \
		let_mut UNIQUE_VAR(begin) = (collection).m_vtable->begin(&(collection));                 \
		let_mut UNIQUE_VAR(end) = (collection).m_vtable->end(&(collection));                     \
		let_mut UNIQUE_VAR(begin_converted)                                                      \
			= cnx_iterator_into_forward_iterator(UNIQUE_VAR(begin));                             \
		let_mut UNIQUE_VAR(end_converted) = cnx_iterator_into_forward_iterator(UNIQUE_VAR(end)); \
		cnx_range_take_first_from_iterators(T,                                                   \
											n,                                                   \
											UNIQUE_VAR(begin_converted),                         \
											UNIQUE_VAR(end_converted));                          \
	})
/// @brief Collects the elements viewed by the range into a new `CnxVector(T)`
///
/// Creates a new `CnxVector(T)` and copies the elements viewed by `range` into it
///
/// # Requirements
/// - Requires that the `CnxVector(T)` template has been instantiated for `T` and the instantiation
/// is in scope
///
/// @param T - The type held by the collection `CnxRange(T)` will abstract over
/// @param range - The `CnxRange(T)` to collect
///
/// @return a `CnxVector(T)` containing the elements viewed by `range`
/// @ingroup ranges
#define cnx_range_collect(T, range)                      \
	({                                                   \
		let_mut UNIQUE_VAR(vec) = cnx_vector_new(T);     \
		foreach(elem, range) {                           \
			cnx_vector_push_back(UNIQUE_VAR(vec), elem); \
		}                                                \
		UNIQUE_VAR(vec);                                 \
	})
/// @brief Transforms the elements viewed by the `CnxRange(T)`
///
/// Transforms the elements viewed by the range in place with the `CnxRangeTransform(T)`
/// function, `transform_function`
///
/// @param range - The `CnxRange(T)` to transform
/// @param transform_function - The `CnxRangeTransform(T)` to transform elements with
///
/// @return The given range, after applying the transformation
/// @ingroup ranges
#define cnx_range_transform(range, transform_function) \
	({                                                 \
		foreach_ref_mut(elem, range) {                 \
			(transform_function)(elem);                \
		}                                              \
                                                       \
		(range);                                       \
	})
/// @brief Transforms the elements in the collection
///
/// Transforms the elements in the collection in place with the `CnxRangeTransform(T)`
/// function, `transform_function`, and returns a `CnxRange(T)` viewing the collection
///
/// @param T - The type held by the collection `CnxRange(T)` will abstract over
/// @param collection - The collection to abstract into a range
/// @param transform_function - The `CnxRangeTransform(T)` to transform elements with
///
/// @return The given range, after applying the transformation
/// @ingroup ranges
#define cnx_transform(T, collection, transform_function)            \
	({                                                              \
		let_mut UNIQUE_VAR(range) = cnx_range_from(T, collection);  \
		cnx_range_transform(UNIQUE_VAR(range), transform_function); \
	})
/// @brief Transforms the elements viewed by the `CnxRange(T)`, then filters the range
///
/// Transforms the elements viewed by the range in place with the `CnxRangeTransform(T)`
/// function, `transform_function`, then filters the range with the `CnxRangeFilter(T)`,
/// `filter_function`
///
/// @param range - The `CnxRange(T)` to transform and filter
/// @param transform_function - The `CnxRangeTransform(T)` to transform elements with
/// @param filter_function - The `CnxRangeFilter(T)` to filter the range with
///
/// @return The given range, after applying the transformation, filtered
/// @ingroup ranges
#define cnx_range_transform_then_filter(range, transform_function, filter_function)       \
	({                                                                                    \
		let_mut UNIQUE_VAR(transformed) = cnx_range_transform(range, transform_function); \
		UNIQUE_VAR(transformed).m_filter = (filter_function);                             \
		UNIQUE_VAR(transformed);                                                          \
	})
/// @brief Transforms the elements in the collection, then returns a filtered `CnxRange(T)` over
/// the collection
///
/// Transforms the elements viewed in the collection in place with the `CnxRangeTransform(T)`
/// function, `transform_function`, then returns a range over the collection filtered with the
/// `CnxRangeFilter(T)`, `filter_function`
///
/// @param T - The type held by the collection `CnxRange(T)` will abstract over
/// @param collection - The collection to abstract into a range
/// @param transform_function - The `CnxRangeTransform(T)` to transform elements with
/// @param filter_function - The `CnxRangeFilter(T)` to filter the range with
///
/// @return The given range, after applying the transformation, filtered
/// @ingroup ranges
#define cnx_transform_then_filter(T, collection, transform_function, filter_function)            \
	({                                                                                           \
		let_mut UNIQUE_VAR(range) = cnx_range_from(T, collection);                               \
		cnx_range_transform_then_filter(UNIQUE_VAR(range), transform_function, filter_functino); \
	})
/// @brief "Accumulates" the elements viewed by the `CnxRange(T)`
///
/// "Accumulates" the elements viewed by the given `CnxRange(T)` with the
/// `CnxRangeAccumulator(T)`, `accumulation_function`, returning the result
///
/// @param range - The `CnxRange(T)` to accumulate the elements of
/// @param accumulation_function - The `CnxRangeAccumulator(T)` to accumulate elements with
///
/// @return The resulting accumulation
/// @ingroup ranges
#define cnx_range_accumulate(range, accumulation_function)                                     \
	({                                                                                         \
		let UNIQUE_VAR(_begin) = cnx_range_begin(range);                                       \
		let_mut UNIQUE_VAR(accumulation) = cnx_iterator_current(UNIQUE_VAR(_begin));           \
		foreach_ref(elem, range) {                                                             \
			UNIQUE_VAR(accumulation) = accumulation_function(&UNIQUE_VAR(accumulation), elem); \
		}                                                                                      \
		UNIQUE_VAR(accumulation);                                                              \
	})
/// @brief "Accumulates" the elements in the collection
///
/// "Accumulates" the elements in the given collection with the
/// `CnxRangeAccumulator(T)`, `accumulation_function`, returning the result
///
/// @param T - The type held by the collection `CnxRange(T)` will abstract over
/// @param collection - The collection to accumulate
/// @param accumulation_function - The `CnxRangeAccumulator(T)` to accumulate elements with
///
/// @return The resulting accumulation
/// @ingroup ranges
#define cnx_accumulate(T, collection, accumulation_function)             \
	({                                                                   \
		let_mut UNIQUE_VAR(_range) = cnx_range_from(T, collection);      \
		cnx_range_accumulate(UNIQUE_VAR(_range), accumulation_function); \
	})

#endif // CNX_RANGE_DEF
