/// @file StdPPBool.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief StdPPBool provides various macros for metaprogramming with conditionals and boolean logic
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
#include <C2nxt/mpl/StdPPStrings.h>

/// @defgroup std_pp_bool Preprocessor Boolean Metaprogramming
/// This module provides various macros for metaprogramming with conditionals and boolean logic

#ifndef STD_PP_BOOL
	#define STD_PP_BOOL

	/// @brief Macro for logical `true`, used to clarify intent over using `1`
	/// @ingroup std_pp_bool
	#define TRUE 1
	/// @brief Macro for logical `false`, used to clarify intent over using `0`
	/// @ingroup std_pp_bool
	#define FALSE 0
	/// @brief Macro for consuming a variadic argument pack and converting it to `true`
	/// @ingroup std_pp_bool
	#define V_TRUE(...) TRUE
	/// @brief Macro for consuming a variadic argument pack and converting it to `false`
	/// @ingroup std_pp_bool
	#define V_FALSE(...) FALSE
	/// @brief Macro for performing if-else conditional branch in the preprocessor
	/// Evaluates condition, and if it is `TRUE`, preprocessing resolves to the token string
	/// `true_case`. Otherwise, processing resolves to the token string `false_case`
	/// @param condition - The boolean condition to evaluate. Must evaluate to `0` (`FALSE`) or `1`
	/// (`TRUE`) in the preprocessor
	/// @param true_case - The token string to evaluate to in the `TRUE` case
	/// @param false_case - The token string to evaluate to in the `FALSE` case
	/// @ingroup std_pp_bool
	#define IF_ELSE(condition, true_case, false_case) IF_ELSE_IMPL(condition)(true_case, false_case)
	/// @brief Implements conditional evaluation for `IF_ELSE`
	#define IF_ELSE_IMPL(condition) CONCAT2(IF_ELSE_, condition)
	/// @brief Implements false case evaluation for `IF_ELSE`
	#define IF_ELSE_0(true_case, false_case) false_case
	/// @brief Implements true case evaluation for `IF_ELSE`
	#define IF_ELSE_1(true_case, false_case) true_case
	/// @brief Macro for performing and if conditional branch in the preprocessor
	/// Evaluates condition, and if it is `TRUE`, preprocessing resolves to the token string
	/// `true_case`. Otherwise, processing resolves to no tokens
	/// @param condition - The boolean condition to evaluate. Must evaluate to `0` (`FALSE`) or `1`
	/// (`TRUE`) in the preprocessor
	/// @param true_case - The token string to evaluate to in the `TRUE` case
	/// @ingroup std_pp_bool
	#define IF(condition, true_case) IF_IMPL(condition)(true_case)
	/// @brief Implements conditional evaluation for `IF`
	#define IF_IMPL(condition) CONCAT2(IF_, condition)
	/// @brief Implements false case evaluation for `IF`
	#define IF_0(true_case)
	/// @brief Implements true case evaluation for `IF`
	#define IF_1(true_case) true_case

	/// @brief Macro for performing logical not (e.g. `!`) in the preprocessor
	/// Evaluates `x`, and if it is `TRUE`, preprocessing resolves to `FALSE`
	/// Otherwise, processing resolves to `TRUE`
	/// @param x - The boolean to evaluate. Must evaluate to `0` (`FALSE`) or `1`
	/// (`TRUE`) in the preprocessor
	/// @ingroup std_pp_bool
	#define NOT(x) NOT_IMPL(x)
	/// @brief Implements conditional evaluation for `NOT`
	#define NOT_IMPL(x) CONCAT2(NOT_, x)
	/// @brief Implements false case evaluation for `NOT`
	#define NOT_0 TRUE
	/// @brief Implements true case evaluation for `NOT`
	#define NOT_1 FALSE
#endif // STD_PP_BOOL
