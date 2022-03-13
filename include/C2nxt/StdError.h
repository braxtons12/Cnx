/// @file StdError.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief This module provides an extensible type for communicating errors via both error codes and
/// message strings.
/// @version 0.2
/// @date 2022-03-09
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

/// @ingroup error_handling
/// @{
/// @defgroup std_error StdError
/// `StdError` provides an extensible, configurable type for communicating recoverable errors via
/// error codes and error message strings
///
/// Example
/// @code {.c}
/// StdError do_thing(i64 input, i64* restrict out) {
///		// do things ...
/// 	if(errno) {
///			return std_error_new(errno, STD_POSIX_ERROR_CATEGORY);
/// 	}
/// 	else {
///	 		return std_error_new(0, STD_POSIX_ERROR_CATEGORY);
/// 	}
/// }
///
/// void func(void) {
/// 	i64 i = 0;
/// 	let maybe_err = do_thing(10, &i);
/// 	if(std_error_code(&maybe_err) != 0) {
/// 		// handle error ...
/// 	}
/// 	// do other things...
/// }
/// @endcode
///
/// In most cases, `StdError` would be used in tandem with `StdResult(T)` to enable concise error
/// handling without having to use "out" parameters.
/// @}

#include <C2nxt/StdBasicTypes.h>
#include <errno.h>

#ifndef STD_ERROR
	/// @brief Declarations and definitions related to `StdError`
	#define STD_ERROR

/// @brief A `std_error_category_message_function` is a function that converts an `i64` error code
/// into its corresponding message string, for the error category the function is associated with.
/// @ingroup std_error
typedef const_cstring (*const std_error_category_message_function)(i64 error_code);

/// @brief `StdErrorCategory` provides the mechanism to convert an arbitrary error code into a
/// corresponding message associated with a specific class of errors
///
/// `StdErrorCategory` is what allows a `StdError` to communicate error messages unique to the
/// module the error originated from.
/// @ingroup std_error
typedef struct StdErrorCategory {
	/// @brief converts a given `i64` error code into a `cstring`
	const std_error_category_message_function m_message_function;
} StdErrorCategory;

/// @brief `StdError` provides an extensible, configurable type for communicating recoverable errors
/// via error codes and error message strings
///
/// Example
/// @code {.c}
/// StdError do_thing(i64 input, i64* restrict out) {
///		// do things ...
/// 	if(errno) {
///			return std_error_new(errno, STD_POSIX_ERROR_CATEGORY);
/// 	}
/// 	else {
///	 		return std_error_new(0, STD_POSIX_ERROR_CATEGORY);
/// 	}
/// }
///
/// void func(void) {
/// 	i64 i = 0;
/// 	let maybe_err = do_thing(10, &i);
/// 	if(std_error_code(&maybe_err) != 0) {
/// 		// handle error ...
/// 	}
/// 	// do other things...
/// }
/// @endcode
///
/// In most cases, `StdError` would be used in tandem with `StdResult(T)` to enable concise error
/// handling without having to use "out" parameters.
/// @ingroup std_error
typedef struct StdError {
	/// @brief The error code associated with this error
	i64 m_error_code;
	/// @brief The error category to convert the error code to a textual message
	StdErrorCategory m_error_category;
} StdError;

	#define ___DISABLE_IF_NULL(self) \
		std_disable_if(!(self), "Can't perform an operation with a StdError that is a nullptr")

/// @brief Creates a `StdError` with the given error code and category
///
/// @param error_code - The error code associated with the error
/// @param error_category - The `StdErrorCategory` that will generate the message associated with
/// the error code
///
/// @return a `StdError`
/// @ingroup std_error
[[nodiscard]] StdError std_error_new(i64 error_code, StdErrorCategory error_category);
/// @brief Returns the error code associated with the given error
///
/// @param self - The error
///
/// @return the associated error code
/// @ingroup std_error
[[nodiscard]] [[not_null(1)]] i64
std_error_code(const StdError* restrict self) ___DISABLE_IF_NULL(self);
/// @brief Returns the error message associated with the given error, as a `cstring`
///
/// @param self - The error
///
/// @return the associated error message
/// @ingroup std_error
[[nodiscard]] [[not_null(1)]] [[returns_not_null]] const_cstring
std_error_message(const StdError* restrict self) ___DISABLE_IF_NULL(self);

/// @brief Returns the error message associated with the given error code, as a `cstring`
///
/// @param self - The error category to get the message from
/// @param error_code - The error code
///
/// @return the message associated with the error code
/// @ingroup std_error
[[nodiscard]] [[returns_not_null]] const_cstring
std_error_category_get_message(StdErrorCategory self, i64 error_code);
/// @brief Returns the POSIX error message associated with the given error code, as a `cstring`
///
/// @param error_code - The error code to get the message for
///
/// @return the message associated with the error code
/// @ingroup std_error
[[nodiscard]] [[returns_not_null]] const_cstring
std_error_category_get_posix_message(i64 error_code);

	/// @brief The `StdErrorCategory` to map POSIX error codes
	///
	/// `STD_POSIX_ERROR_CATEGORY` is the error category to map POSIX error codes to their
	/// associated error messages. It will produce equivalent results to `strerror` in standard C
	/// @ingroup std_error
	#define STD_POSIX_ERROR_CATEGORY ((StdErrorCategory){std_error_category_get_posix_message})
	#ifndef STD_DEFAULT_ERROR_CATEGORY
		/// @brief the default `StdErrorCategory`. By default, this is `STD_POSIX_ERROR_CATEGORY`
		///
		/// This can be overridden by defining `STD_DEFAULT_ERROR_CATEGORY` to a compound literal
		/// of your chosen `StdErrorCategory` prior to including "StdError.h"
		/// @ingroup std_error
		#define STD_DEFAULT_ERROR_CATEGORY STD_POSIX_ERROR_CATEGORY
	#endif // STD_DEFAULT_ERROR_CATEGORY

	#undef ___DISABLE_IF_NULL
#endif // STD_ERROR
