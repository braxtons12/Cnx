/// @file StdStringExt.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief This module provides additional functionality for working with `StdString`s
/// @version 0.2.0
/// @date 2022-03-31
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

#ifndef STD_STRING_EXT
#define STD_STRING_EXT

#include <C2nxt/StdDef.h>
#include <C2nxt/StdString.h>

#define VECTOR_INCLUDE_DEFAULT_INSTANTIATIONS TRUE
#include <C2nxt/StdVector.h>
#undef VECTOR_INCLUDE_DEFAULT_INSTANTIATIONS

#define ___DISABLE_IF_NULL(self) \
	std_disable_if(!(self), "Can't perform a StdString operation on a nullptr")

/// @brief Splits the given string at each instance of the given `delimiter` character, returning a
/// `StdVector` of the resulting substrings
///
/// Splits the string by taking the substring within each pair of `delimiter`s (or the beginning
/// and/or end of the string). Only substrings of at least one character are considered; zero-length
/// substrings are discarded. The returned vector and its contained strings will use the allocator
/// associated with `self` for necessary memory allocations (to specify the allocator to use, use
/// `std_string_split_on_with_allocator`).
///
/// @param self - The string to split
/// @param delimiter - The character to split the string on
///
/// @return a `StdVector(String)` containing the substrings delimited by `delimiter` in `self`
/// @ingroup std_string
/// @headerfile "C2nxt/StdStringExt.h"
[[nodiscard]] [[not_null(1)]] StdVector(StdString)
	std_string_split_on(const StdString* restrict self, char delimiter) ___DISABLE_IF_NULL(self);

/// @brief Splits the given string at each instance of the given `delimiter` character, returning a
/// `StdVector` of the resulting substrings
///
/// Splits the string by taking the substring within each pair of `delimiter`s (or the beginning
/// and/or end of the string). Only substrings of at least one character are considered; zero-length
/// substrings are discarded. The returned vector and its contained strings will use the given
/// allocator for necessary memory allocations.
///
/// @param self - The string to split
/// @param delimiter - The character to split the string on
/// @param allocator - The allocator for the returned vector and its contained strings to use for
/// memory allocation
///
/// @return a `StdVector(String)` containing the substrings delimited by `delimiter` in `self`
/// @ingroup std_string
/// @headerfile "C2nxt/StdStringExt.h"
[[nodiscard]] [[not_null(1)]] StdVector(StdString)
	std_string_split_on_with_allocator(const StdString* restrict self,
									   char delimiter,
									   StdAllocator allocator) ___DISABLE_IF_NULL(self);

/// @brief Splits the given string into a set of stringviews at each instance of the given
/// `delimiter` character, returning them in a `StdVector`
///
/// Splits the string by taking the substring within each pair of `delimiter`s (or the beginning
/// and/or end of the string). Only substrings of at least one character are considered; zero-length
/// substrings are discarded. The returned vector will use the allocator associated with `self` for
/// necessary memory allocations (to specify the allocator to use, use
/// `std_string_view_split_on_with_allocator`).
///
/// @param self - The string to split
/// @param delimiter - The character to split the string on
///
/// @return a `StdVector(StdStringView)` containing views of the substrings delimited by `delimiter`
/// in `self`
/// @ingroup std_string
/// @headerfile "C2nxt/StdStringExt.h"
[[nodiscard]] [[not_null(1)]] StdVector(StdStringView)
	std_string_view_split_on(const StdString* restrict self, char delimiter)
		___DISABLE_IF_NULL(self);

/// @brief Splits the given string into a set of stringviews at each instance of the given
/// `delimiter` character, returning them in a `StdVector`
///
/// Splits the string by taking the substring within each pair of `delimiter`s (or the beginning
/// and/or end of the string). Only substrings of at least one character are considered; zero-length
/// substrings are discarded. The returned vector will use the given allocator for necessary memory
/// allocations.
///
/// @param self - The string to split
/// @param delimiter - The character to split the string on
/// @param allocator - The allocator for the returned vector to use for memory allocations
///
/// @return a `StdVector(StdStringView)` containing views of the substrings delimited by `delimiter`
/// in `self`
/// @ingroup std_string
/// @headerfile "C2nxt/StdStringExt.h"
[[nodiscard]] [[not_null(1)]] StdVector(StdStringView)
	std_string_view_split_on_with_allocator(const StdString* restrict self,
											char delimiter,
											StdAllocator allocator) ___DISABLE_IF_NULL(self);

/// @brief Determines the total number of times the character `to_find` occurs in `self`
///
/// @param self - The string to find the occurrences of `to_find` in
/// @param to_find - The character to find in `self`
///
/// @return The number of times `to_find` occurs in `self`
/// @ingroup std_string
/// @headerfile "C2nxt/StdStringExt.h"
[[nodiscard]] [[not_null(1)]] usize
std_string_occurrences_of_char(const StdString* restrict self, char to_find)
	___DISABLE_IF_NULL(self);

/// @brief Determines the total number of times the string `to_find` occurs in `self`
///
/// @param self - The string to find the occurrences of `to_find` in
/// @param to_find - The string to find in `self`
///
/// @return The number of times `to_find` occurs in `self`
/// @ingroup std_string
/// @headerfile "C2nxt/StdStringExt.h"
[[nodiscard]] [[not_null(1)]] usize
std_string_occurrences_of(const StdString* restrict self, const StdString* restrict to_find)
	___DISABLE_IF_NULL(self);

/// @brief Determines the total number of times the stringview `to_find` occurs in `self`
///
/// @param self - The string to find the occurrences of `to_find` in
/// @param to_find - The stringview to find in `self`
///
/// @return The number of times `to_find` occurs in `self`
/// @ingroup std_string
/// @headerfile "C2nxt/StdStringExt.h"
[[nodiscard]] [[not_null(1)]] usize
std_string_occurrences_of_stringview(const StdString* restrict self,
									 const StdStringView* restrict to_find)
	___DISABLE_IF_NULL(self);

/// @brief Determines the total number of times the `cstring` `to_find` occurs in `self`
///
/// @param self - The string to find the occurrences of `to_find` in
/// @param to_find - The `cstring` to find in `self`
///
/// @return The number of times `to_find` occurs in `self`
/// @ingroup std_string
/// @headerfile "C2nxt/StdStringExt.h"
[[nodiscard]] [[not_null(1)]] usize
std_string_occurrences_of_cstring(const StdString* restrict self,
								  restrict const_cstring to_find,
								  usize to_find_length) ___DISABLE_IF_NULL(self);

/// @brief Finds the indices at which the character `to_find` occurs in `self`, and returns them in
/// a `StdVector`
///
/// The returned vector will use the allocator accociated with `self` for necessary memory
/// allocations (to specify the allocator to use, use
/// `std_string_find_occurrences_of_char_with_allocator`)
///
/// @param self - The string to find where `to_find` occurs in
/// @param to_find - The character to find the occurrences of in `self`
///
/// @return a `StdVector(usize)` containing the indices at which `to_find` occurs in `self`
/// @ingroup std_string
/// @headerfile "C2nxt/StdStringExt.h"
[[nodiscard]] [[not_null(1)]] StdVector(usize)
	std_string_find_occurrences_of_char(const StdString* restrict self, char to_find)
		___DISABLE_IF_NULL(self);

/// @brief Finds the indices at which the string `to_find` occurs in `self`, and returns them in
/// a `StdVector`
///
/// The returned vector will use the allocator accociated with `self` for necessary memory
/// allocations (to specify the allocator to use, use
/// `std_string_find_occurrences_of_with_allocator`)
///
/// @param self - The string to find where `to_find` occurs in
/// @param to_find - The string to find the occurrences of in `self`
///
/// @return a `StdVector(usize)` containing the indices at which `to_find` occurs in `self`
/// @ingroup std_string
/// @headerfile "C2nxt/StdStringExt.h"
[[nodiscard]] [[not_null(1)]] StdVector(usize)
	std_string_find_occurrences_of(const StdString* restrict self,
								   const StdString* restrict to_find) ___DISABLE_IF_NULL(self);

/// @brief Finds the indices at which the stringview `to_find` occurs in `self`, and returns them in
/// a `StdVector`
///
/// The returned vector will use the allocator accociated with `self` for necessary memory
/// allocations (to specify the allocator to use, use
/// `std_string_find_occurrences_of_with_allocator`)
///
/// @param self - The string to find where `to_find` occurs in
/// @param to_find - The stringview to find the occurrences of in `self`
///
/// @return a `StdVector(usize)` containing the indices at which `to_find` occurs in `self`
/// @ingroup std_string
/// @headerfile "C2nxt/StdStringExt.h"
[[nodiscard]] [[not_null(1)]] StdVector(usize)
	std_string_find_occurrences_of_stringview(const StdString* restrict self,
											  const StdStringView* restrict to_find)
		___DISABLE_IF_NULL(self);

/// @brief Finds the indices at which the `cstring` `to_find` occurs in `self`, and returns them in
/// a `StdVector`
///
/// The returned vector will use the allocator accociated with `self` for necessary memory
/// allocations (to specify the allocator to use, use
/// `std_string_find_occurrences_of_with_allocator`)
///
/// @param self - The string to find where `to_find` occurs in
/// @param to_find - The `cstring` to find the occurrences of in `self`
///
/// @return a `StdVector(usize)` containing the indices at which `to_find` occurs in `self`
/// @ingroup std_string
/// @headerfile "C2nxt/StdStringExt.h"
[[nodiscard]] [[not_null(1)]] StdVector(usize)
	std_string_find_occurrences_of_cstring(const StdString* restrict self,
										   restrict const_cstring to_find,
										   usize to_find_length) ___DISABLE_IF_NULL(self);

/// @brief Finds the indices at which the character `to_find` occurs in `self`, and returns them in
/// a `StdVector`
///
/// The returned vector will use the given allocator for necessary memory allocations
///
/// @param self - The string to find where `to_find` occurs in
/// @param to_find - The character to find the occurrences of in `self`
/// @param allocator - The allocator the returned vector will use for necessary memory allocations
///
/// @return a `StdVector(usize)` containing the indices at which `to_find` occurs in `self`
/// @ingroup std_string
/// @headerfile "C2nxt/StdStringExt.h"
[[nodiscard]] [[not_null(1)]] StdVector(usize)
	std_string_find_occurrences_of_char_with_allocator(const StdString* restrict self,
													   char to_find,
													   StdAllocator allocator)
		___DISABLE_IF_NULL(self);

/// @brief Finds the indices at which the string `to_find` occurs in `self`, and returns them in
/// a `StdVector`
///
/// The returned vector will use the given allocator for necessary memory allocations
///
/// @param self - The string to find where `to_find` occurs in
/// @param to_find - The string to find the occurrences of in `self`
/// @param allocator - The allocator the returned vector will use for necessary memory allocations
///
/// @return a `StdVector(usize)` containing the indices at which `to_find` occurs in `self`
/// @ingroup std_string
/// @headerfile "C2nxt/StdStringExt.h"
[[nodiscard]] [[not_null(1)]] StdVector(usize)
	std_string_find_occurrences_of_with_allocator(const StdString* restrict self,
												  const StdString* restrict to_find,
												  StdAllocator allocator) ___DISABLE_IF_NULL(self);

/// @brief Finds the indices at which the stringview `to_find` occurs in `self`, and returns them in
/// a `StdVector`
///
/// The returned vector will use the given allocator for necessary memory allocations
///
/// @param self - The string to find where `to_find` occurs in
/// @param to_find - The stringview to find the occurrences of in `self`
/// @param allocator - The allocator the returned vector will use for necessary memory allocations
///
/// @return a `StdVector(usize)` containing the indices at which `to_find` occurs in `self`
/// @ingroup std_string
/// @headerfile "C2nxt/StdStringExt.h"
[[nodiscard]] [[not_null(1)]] StdVector(usize)
	std_string_find_occurrences_of_stringview_with_allocator(const StdString* restrict self,
															 const StdStringView* restrict to_find,
															 StdAllocator allocator)
		___DISABLE_IF_NULL(self);

/// @brief Finds the indices at which the `cstring` `to_find` occurs in `self`, and returns them in
/// a `StdVector`
///
/// The returned vector will use the given allocator for necessary memory allocations
///
/// @param self - The string to find where `to_find` occurs in
/// @param to_find - The `cstring` to find the occurrences of in `self`
/// @param allocator - The allocator the returned vector will use for necessary memory allocations
///
/// @return a `StdVector(usize)` containing the indices at which `to_find` occurs in `self`
/// @ingroup std_string
/// @headerfile "C2nxt/StdStringExt.h"
[[nodiscard]] [[not_null(1)]] StdVector(usize)
	std_string_find_occurrences_of_cstring_with_allocator(const StdString* restrict self,
														  restrict const_cstring to_find,
														  usize to_find_length,
														  StdAllocator allocator)
		___DISABLE_IF_NULL(self);

/// @brief Splits the given string at each instance of the given `delimiter` character, returning a
/// `StdVector` of the resulting substrings
///
/// Splits the string by taking the substring within each pair of `delimiter`s (or the beginning
/// and/or end of the string). Only substrings of at least one character are considered; zero-length
/// substrings are discarded. The returned vector and its contained strings will use the allocator
/// associated with `self` for necessary memory allocations (to specify the allocator to use, use
/// `std_string_split_on_with_allocator`).
///
/// @param self - The string to split
/// @param delimiter - The character to split the string on
///
/// @return a `StdVector(String)` containing the substrings delimited by `delimiter` in `self`
/// @ingroup std_string
/// @headerfile "C2nxt/StdStringExt.h"
#define std_string_split_on(self, delimiter) ((std_string_split_on)(&(self), delimiter))

/// @brief Splits the given string at each instance of the given `delimiter` character, returning a
/// `StdVector` of the resulting substrings
///
/// Splits the string by taking the substring within each pair of `delimiter`s (or the beginning
/// and/or end of the string). Only substrings of at least one character are considered; zero-length
/// substrings are discarded. The returned vector and its contained strings will use the given
/// allocator for necessary memory allocations.
///
/// @param self - The string to split
/// @param delimiter - The character to split the string on
/// @param allocator - The allocator for the returned vector and its contained strings to use for
/// memory allocation
///
/// @return a `StdVector(String)` containing the substrings delimited by `delimiter` in `self`
/// @ingroup std_string
/// @headerfile "C2nxt/StdStringExt.h"
#define std_string_split_on_with_allocator(self, delimiter, allocator) \
	((std_string_split_on_with_allocator)(&(self), delimiter, allocator))

/// @brief Splits the given string into a set of stringviews at each instance of the given
/// `delimiter` character, returning them in a `StdVector`
///
/// Splits the string by taking the substring within each pair of `delimiter`s (or the beginning
/// and/or end of the string). Only substrings of at least one character are considered; zero-length
/// substrings are discarded. The returned vector will use the allocator associated with `self` for
/// necessary memory allocations (to specify the allocator to use, use
/// `std_string_view_split_on_with_allocator`).
///
/// @param self - The string to split
/// @param delimiter - The character to split the string on
///
/// @return a `StdVector(StdStringView)` containing views of the substrings delimited by `delimiter`
/// in `self`
/// @ingroup std_string
/// @headerfile "C2nxt/StdStringExt.h"
#define std_string_view_split_on(self, delimiter) ((std_string_view_split_on)(&(self), delimiter))

/// @brief Splits the given string into a set of stringviews at each instance of the given
/// `delimiter` character, returning them in a `StdVector`
///
/// Splits the string by taking the substring within each pair of `delimiter`s (or the beginning
/// and/or end of the string). Only substrings of at least one character are considered; zero-length
/// substrings are discarded. The returned vector will use the given allocator for necessary memory
/// allocations.
///
/// @param self - The string to split
/// @param delimiter - The character to split the string on
/// @param allocator - The allocator for the returned vector to use for memory allocations
///
/// @return a `StdVector(StdStringView)` containing views of the substrings delimited by `delimiter`
/// in `self`
/// @ingroup std_string
/// @headerfile "C2nxt/StdStringExt.h"
#define std_string_view_split_on_with_allocator(self, delimiter, allocator) \
	((std_string_view_split_on_with_allocator)(&(self), delimiter, allocator))

/// @brief Determines the total number of times the character `to_find` occurs in `self`
///
/// @param self - The string to find the occurrences of `to_find` in
/// @param to_find - The character to find in `self`
///
/// @return The number of times `to_find` occurs in `self`
/// @ingroup std_string
/// @headerfile "C2nxt/StdStringExt.h"
#define std_string_occurrences_of_char(self, to_find) \
	std_string_occurrences_of_char(&(self), to_find)

// clang-format off

/// @brief Determines the total number of times the string or string-like `to_find` occurs in `self`
///
/// `to_find` my be ` StdString`, `StdStringView` or `cstring`
///
/// @param self - The string to find the occurrences of `to_find` in
/// @param to_find - The string to find in `self`
///
/// @return The number of times `to_find` occurs in `self`
/// @ingroup std_string
/// @headerfile "C2nxt/StdStringExt.h"
#define std_string_occurrences_of(self, to_find) 												   \
	_Generic((to_find), 																		   \
	const_cstring 				: 	std_string_occurrences_of_cstring(&(self), 					   \
										static_cast(const_cstring)(to_find), 				       \
										strlen(static_cast(const_cstring)(to_find))),      	   	   \
	cstring 					: 	std_string_occurrences_of_cstring(&(self), 				   	   \
										static_cast(cstring)(to_find), 				    	   	   \
										strlen(static_cast(cstring)(to_find))),      		   	   \
	const char[sizeof(to_find)] 	: 	std_string_occurrences_of_cstring(&(self), 	/** NOLINT **/ \
										static_cast(const_cstring)(to_find), 				   	   \
										sizeof(to_find)), 							/** NOLINT **/ \
	char[sizeof(to_find)] 		: 	std_string_occurrences_of_cstring(&(self), 		/** NOLINT **/ \
										static_cast(cstring)(to_find),  						   \
										sizeof(to_find)), 							/** NOLINT **/ \
	StdStringView* 				: 	std_string_occurrences_of_stringview(&(self), 			       \
										static_cast(StdStringView*)(to_find)), 		   		   	   \
	const StdStringView* 		: 	std_string_occurrences_of_stringview(&(self), 			   	   \
										static_cast(const StdStringView*)(to_find)), 		   	   \
	StdString* 					: 	std_string_occurrences_of(&(self), 							   \
										static_cast(StdString*)(to_find)),   				   	   \
	const StdString* 			: 	std_string_occurrences_of(&(self), 							   \
										static_cast(const StdString*)(to_find)))
// clang-format on

/// @brief Finds the indices at which the character `to_find` occurs in `self`, and returns them in
/// a `StdVector`
///
/// The returned vector will use the allocator accociated with `self` for necessary memory
/// allocations (to specify the allocator to use, use
/// `std_string_find_occurrences_of_char_with_allocator`)
///
/// @param self - The string to find where `to_find` occurs in
/// @param to_find - The character to find the occurrences of in `self`
///
/// @return a `StdVector(usize)` containing the indices at which `to_find` occurs in `self`
/// @ingroup std_string
/// @headerfile "C2nxt/StdStringExt.h"
#define std_string_find_occurrences_of_char(self, to_find) \
	std_string_find_occurrences_of_char(&(self), to_find)

/// @brief Finds the indices at which the character `to_find` occurs in `self`, and returns them in
/// a `StdVector`
///
/// The returned vector will use the given allocator for necessary memory allocations
///
/// @param self - The string to find where `to_find` occurs in
/// @param to_find - The character to find the occurrences of in `self`
/// @param allocator - The allocator the returned vector will use for necessary memory allocations
///
/// @return a `StdVector(usize)` containing the indices at which `to_find` occurs in `self`
/// @ingroup std_string
/// @headerfile "C2nxt/StdStringExt.h"
#define std_string_find_occurrences_of_char_with_allocator(self, to_find, allocator) \
	std_string_find_occurrences_of_char_with_allocator(&(self), to_find, allocator)

// clang-format off

/// @brief Finds the indices at which the string or string-like `to_find` occurs in `self`, and
/// returns them in a `StdVector`
///
/// The returned vector will use the allocator accociated with `self` for necessary memory
/// allocations (to specify the allocator to use, use
/// `std_string_find_occurrences_of_with_allocator`)
///
/// `to_find` may be a `StdString`, `StdStringView`, or `cstring`
///
/// @param self - The string to find where `to_find` occurs in
/// @param to_find - The string to find the occurrences of in `self`
///
/// @return a `StdVector(usize)` containing the indices at which `to_find` occurs in `self`
/// @ingroup std_string
/// @headerfile "C2nxt/StdStringExt.h"
#define std_string_find_occurrences_of(self, to_find) 											   \
	_Generic((to_find), 																		   \
	const_cstring 				: 	std_string_find_occurrences_of_cstring(&(self), 			   \
										static_cast(const_cstring)(to_find), 				   	   \
										strlen(static_cast(const_cstring)(to_find))),      	   	   \
	cstring 					: 	std_string_find_occurrences_of_cstring(&(self), 			   \
										static_cast(cstring)(to_find), 				    	   	   \
										strlen(static_cast(cstring)(to_find))),      		   	   \
	const char[sizeof(to_find)] : 	std_string_find_occurrences_of_cstring(&(self), /** NOLINT **/ \
										static_cast(const_cstring)(to_find), 				   	   \
										sizeof(to_find)), 							/** NOLINT **/ \
	char[sizeof(to_find)] 		: 	std_string_find_occurrences_of_cstring(&(self), /** NOLINT **/ \
										static_cast(cstring)(to_find),  						   \
										sizeof(to_find)), 							/** NOLINT **/ \
	StdStringView* 				: 	std_string_find_occurrences_of_stringview(&(self), 			   \
										static_cast(StdStringView*)(to_find)), 		   		   	   \
	const StdStringView* 		: 	std_string_find_occurrences_of_stringview(&(self), 			   \
										static_cast(const StdStringView*)(to_find)), 		   	   \
	StdString* 					: 	std_string_find_occurrences_of(&(self), 					   \
										static_cast(StdString*)(to_find)),   				   	   \
	const StdString* 			: 	std_string_find_occurrences_of(&(self), 					   \
										static_cast(const StdString*)(to_find)))

/// @brief Finds the indices at which the string or string-like `to_find` occurs in `self`, and
/// returns them in a `StdVector`
///
/// The returned vector will use the given allocator for necessary memory allocations
///
/// `to_find` may be a `StdString`, `StdStringView`, or `cstring`
///
/// @param self - The string to find where `to_find` occurs in
/// @param to_find - The string to find the occurrences of in `self`
///
/// @return a `StdVector(usize)` containing the indices at which `to_find` occurs in `self`
/// @ingroup std_string
/// @headerfile "C2nxt/StdStringExt.h"
#define std_string_find_occurrences_of_with_allocator(self, to_find, allocator) 				   \
	_Generic((to_find), 																		   \
	const_cstring 				: 	std_string_find_occurrences_of_cstring_with_allocator(&(self), \
										static_cast(const_cstring)(to_find), 				   	   \
										strlen(static_cast(const_cstring)(to_find)), allocator),   \
	cstring 					: 	std_string_find_occurrences_of_cstring_with_allocator(&(self), \
										static_cast(cstring)(to_find), 				    	   	   \
										strlen(static_cast(cstring)(to_find)), allocator),         \
	const char[sizeof(to_find)] : 	/** NOLINT **/ 												   \
									std_string_find_occurrences_of_cstring_with_allocator( 	  	   \
										&(self), 											  	   \
										static_cast(const_cstring)(to_find), 				  	   \
										sizeof(to_find), allocator), 				/** NOLINT **/ \
	char[sizeof(to_find)] 		: 	/** NOLINT **/ 												   \
									std_string_find_occurrences_of_cstring_with_allocator( 	  	   \
										&(self), 											  	   \
										static_cast(cstring)(to_find),  					  	   \
										sizeof(to_find), allocator), 				/** NOLINT **/ \
	StdStringView* 				: 	std_string_find_occurrences_of_stringview_with_allocator( 	   \
										&(self), 			   									   \
										static_cast(StdStringView*)(to_find), allocator), 		   \
	const StdStringView* 		: 	std_string_find_occurrences_of_stringview_with_allocator( 	   \
										&(self), 			   									   \
										static_cast(const StdStringView*)(to_find), allocator),    \
	StdString* 					: 	std_string_find_occurrences_of_with_allocator(&(self), 		   \
										static_cast(StdString*)(to_find), allocator),   		   \
	const StdString* 			: 	std_string_find_occurrences_of_with_allocator(&(self), 		   \
										static_cast(const StdString*)(to_find), allocator))

// clang-format on
#undef ___DISABLE_IF_NULL
#endif // STD_STRING_EXT
