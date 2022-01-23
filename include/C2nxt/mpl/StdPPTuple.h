/// @file StdPPTuple.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief StdPPTuple provides various macros for metaprogramming with tuple-like constructs
/// @version 0.2
/// @date 2022-01-19
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
#include <C2nxt/mpl/StdArgLists.h>
#include <C2nxt/mpl/StdPPBool.h>
#include <C2nxt/mpl/StdPPStrings.h>

/// @defgroup std_pp_tuple Preprocessor Tuple Metaprogramming
/// This module provides various macros for metaprogramming with tuple-like constructs.
/// A Preprocessing tuple is a token string of the form `(x, y, z, ...)` where `x`, `y`, `z`, etc.
/// are any valid token strings used in a preprocessing context

#ifndef STD_PP_TUPLE
	#define STD_PP_TUPLE

	/// @brief Determines if `x` is a tuple
	/// A tuple is a token string of the form `(x, y, z, ...)` where `x`, `y`, `z`, etc. are valid
	/// token strings
	///
	/// @param x - The token string to determine if it is a tuple
	///
	/// @return `TRUE` if `x` is a tuple, otherwise `FALSE`
	/// @ingroup std_pp_tuple
	#define IS_TUPLE(x) NOT(IS_NOT_TUPLE(x))

	/// @brief Determines if `x` is **NOT** a tuple
	/// A tuple is a token string of the form `(x, y, z, ...)` where `x`, `y`, `z`, etc. are valid
	/// token strings
	///
	/// @param x - The token string to determine if it is **NOT** a tuple
	///
	/// @return `TRUE` if `x` is **NOT** a tuple, otherwise `FALSE`
	/// @ingroup std_pp_tuple
	#define IS_NOT_TUPLE(x) IF_ELSE(IS_TWO_TUPLES(x), TRUE, IS_NOT_TUPLE_FAST)(x)

	/// @brief Expands the given tuple into a comma separated list of elements
	/// A tuple is a token string of the form `(x, y, z, ...)` where `x`, `y`, `z`, etc. are valid
	/// token strings
	///
	/// @param x - The tuple to expand
	///
	/// @return the elements of the tuple `x` in a comma separated list
	/// @ingroup std_pp_tuple
	#define EXPAND_TUPLE(x) EXPAND x

	/// @brief Determines if `x` is two tuples
	/// A tuple is a token string of the form `(x, y, z, ...)` where `x`, `y`, `z`, etc. are valid
	/// token strings
	///
	/// @param x - The token string to determine if it is two tuples
	///
	/// @return `TRUE` if `x` is two tuples, otherwise `FALSE`
	#define IS_TWO_TUPLES(x) CONTAINS_COMMA(EXPAND(COMMA EMPTY x))
	/// @brief Determines if `x` is **NOT** a tuple
	/// A tuple is a token string of the form `(x, y, z, ...)` where `x`, `y`, `z`, etc. are valid
	/// token strings
	///
	/// @param x - The token string to determine if it is **NOT** a tuple
	///
	/// @return `TRUE` if `x` is **NOT** a tuple, otherwise `FALSE`
	#define IS_NOT_TUPLE_FAST(x)		  SECOND(__IS_NOT_TUPLE_FAST_IMPL x, TRUE)
	#define __IS_NOT_TUPLE_FAST_IMPL(...) ~, FALSE

#endif // STD_PP_TUPLE
