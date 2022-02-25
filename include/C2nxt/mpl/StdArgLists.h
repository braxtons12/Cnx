/// @file StdArgLists.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief StdArgLists provides metaprogramming macros for working with __VA_ARGS__ lists
/// @version 0.2
/// @date 2022-01-23
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

#ifndef STD_ARG_LISTS
#define STD_ARG_LISTS

#include <C2nxt/mpl/StdPPStrings.h>
#include <C2nxt/mpl/detail/ArgLists.h>

/// @defgroup std_arg_lists VA_ARGS List Metaprogramming
/// This module provides various macros for metaprogramming with __VA_ARGS__ lists

/// @brief Creates a comma separated reverse sequence from 127 to 0
///
/// Useful for macro meta-programming that requires determining the number of arguments passed in
/// a `__VA_ARGS__` pack
/// @ingroup std_arg_lists
#define PP_REVERSE_SEQUENCE_N()                                                                    \
	127, 126, 125, 124, 123, 122, 121, 120, 119, 118, 117, 116, 115, 114, 113, 112, 111, 110, 109, \
		108, 107, 106, 105, 104, 103, 102, 101, 100, 99, 98, 97, 96, 95, 94, 93, 92, 91, 90, 89,   \
		88, 87, 86, 85, 84, 83, 82, 81, 80, 79, 78, 77, 76, 75, 74, 73, 72, 71, 70, 69, 68, 67,    \
		66, 65, 64, 63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45,    \
		44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23,    \
		22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0

// clang-format on

/// @brief Returns the number of arguments passed in the parameter pack
#define ___PP_NUM_ARGS(...) ___PP_ARG_128(__VA_ARGS__)
/// @brief Returns the number of arguments passed in the `__VA_ARGS__` parameter pack
///
/// Useful for macro meta-programming that requires determining the number of arguments passed in
/// a parameter pack
///
/// @param ... - The `__VA_ARGS__` parameter pack to get the length of
///
/// @return The number of args in the parameter pack
/// @ingroup std_arg_lists
#define PP_NUM_ARGS(...) ___PP_NUM_ARGS(__VA_ARGS__, PP_REVERSE_SEQUENCE_N())

#define __PP_NTH_ARG(N, ...) CONCAT2_DEFERRED(___PP_ARG_, N)(__VA_ARGS__)
/// @brief Returns the Nth argument from the `__VA_ARGS__` parameter pack
///
/// @param N - The 1-indexed index of the argument to get
/// @param ... - The `__VA_ARGS__` parameter pack to index into
///
/// @return The Nth argument
/// @ingroup std_arg_lists
#define PP_NTH_ARG(N, ...) __PP_NTH_ARG(N, __VA_ARGS__)

/// @brief Appends a space followed by `what` to each element of the `__VA_ARGS__` parameter pack
///
/// @param what - The text/token/etc to append to each element of the `__VA_ARGS__` parameter pack
/// @param ... - The `__VA_ARGS__` parameter pack to modify
///
/// @return The modified `__VA_ARGS__` parameter pack
/// @ingroup std_arg_lists
#define APPEND_TO_LIST(what, ...) __APPEND_TO_LIST(PP_NUM_ARGS(__VA_ARGS__), what, __VA_ARGS__)

/// @brief Applies a different delimiter to the list elements of the  `__VA_ARGS__` parameter pack
///
/// @param what - The text/token/etc to delimit each element of the `__VA_ARGS__` parameter pack
/// @param ... - The `__VA_ARGS__` parameter pack to modify
///
/// @return The resulting text from changing the list delimiter
/// @ingroup std_arg_lists
#define DELIMIT_LIST(what, ...) __DELIMIT_LIST(PP_NUM_ARGS(__VA_ARGS__), what, __VA_ARGS__)

/// @brief Calls `what` on the arguments in the parameter pack
///
/// Supports up to 12 arguments in the parameter pack.
/// For every argument, `what` is called/applied on it in place in the pack,
/// eg., `what(x), what(y)`, where `x` and `y` are arguments in the pack.
/// Example:
/// @code {.c}
/// i32* add_one(i32* i) {
///		++(*i);
///		return i;
/// }
/// i32 zero = 0, one = 1, two = 2;
/// // prints "1, 2, 3" to `stdout`
/// println("{}, {}, {}", APPLY_TO_LIST(&, APPLY_TO_LIST(add_one, &zero, &one, &two)));
/// @endcode
///
/// This is most useful for applying a macro or function to a list of values before passing them
/// into another macro or variadic function.
///
/// @param what - The function or function-like macro to apply to the arguments in the pack
/// @param ... - The parameter pack to apply `what` to
///
/// @return The parameter pack, with `what` applied
/// @ingroup std_arg_lists
#define APPLY_TO_LIST(what, ...) ___APPLY_TO_LIST(PP_NUM_ARGS(__VA_ARGS__), what, __VA_ARGS__)

/// @brief Appends the Nth element in `what` to the Nth element in the `__VA_ARGS__` parameter pack
///
/// @param what - The 1-indexible macro generating the token(s) to append to the element in the
/// `__VA_ARGS__` parameter pack with the associated index
/// @param ... - The `__VA_ARGS__` parameter pack to modify
///
/// @return The modified `__VA_ARGS__` parameter pack
/// @ingroup std_arg_lists
#define APPEND_EACH_TO_LIST(what, ...) \
	___APPEND_EACH_TO_LIST(PP_NUM_ARGS(__VA_ARGS__), what, __VA_ARGS__)

/// @brief Returns a numeric token (number prefixed by underscore, eg `_1`) representing the given
/// number
///
/// @param N - The number to get the token for
///
/// @return N as a numeric token
/// @ingroup std_arg_lists
#define SELECTOR_LIST(N) CONCAT2_DEFERRED(__SELECTOR_LIST_, N)

/// @brief Converts any list of arguments into an empty list
/// @ingroup std_arg_lists
#define EMPTY(...)
/// @brief Defers the preprocessing of the list of arguments one step
/// @ingroup std_arg_lists
#define DEFER(...) __VA_ARGS__ EMPTY()
/// @brief Expands the list of arguments
/// @ingroup std_arg_lists
#define EXPAND(...) __VA_ARGS__
/// @brief Converts the list of arguments into a single comma
/// @ingroup std_arg_lists
#define COMMA(...) ,
/// @brief Determines if the list of arguments contains a comma (contains more than one element)
/// @ingroup std_arg_lists
#define CONTAINS_COMMA(...) X_AS_COMMA(__VA_ARGS__, COMMA() ~)
/// @brief Determines if the list of arguments contains a comma (contains more than one element)
#define X_AS_COMMA(_head, x, ...) CONTAINS_COMMA_RESULT(x, FALSE, TRUE, ~)
/// @brief Determines if the list of arguments contains a comma (contains more than one element)
#define CONTAINS_COMMA_RESULT(x, _, result, ...) result
/// @brief Returns the first element from the list
/// @ingroup std_arg_lists
#define FIRST(...) FIRST_IMPL(__VA_ARGS__, ~)
/// @brief Returns the first element from the list
#define FIRST_IMPL(first, ...) first
/// @brief Returns the second element from the list
/// @ingroup std_arg_lists
#define SECOND(...) SECOND_IMPL(__VA_ARGS__, ~)
/// @brief Returns the second element from the list
/// @ingroup std_arg_lists
#define SECOND_IMPL(first, second, ...) second
/// @brief Removes the first element from the list, and returns the remaining elements
/// @ingroup std_arg_lists
#define REMOVE_FIRST(...) REMOVE_FIRST_IMPL(__VA_ARGS__)
/// @brief Removes the first element from the list, and returns the remaining elements
#define REMOVE_FIRST_IMPL(first, ...) __VA_ARGS__

#endif
