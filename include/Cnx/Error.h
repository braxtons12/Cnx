/// @file Error.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief This module provides an extensible type for communicating errors via both error codes and
/// message strings.
/// @version 0.2.5
/// @date 2022-12-09
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
/// @defgroup cnx_error CnxError
/// `CnxError` provides an extensible, configurable type for communicating recoverable errors via
/// error codes and error message strings
///
/// Example
/// @code {.c}
/// CnxError do_thing(i64 input, i64* restrict out) {
///		// do things ...
/// 	if(errno) {
///			return cnx_error_new(errno, CNX_POSIX_ERROR_CATEGORY);
/// 	}
/// 	else {
/// 		*out = 42;
///	 		return cnx_error_new(0, CNX_POSIX_ERROR_CATEGORY);
/// 	}
/// }
///
/// void func(void) {
/// 	i64 i = 0;
/// 	let maybe_err = do_thing(10, &i);
/// 	if(cnx_error_code(&maybe_err) != 0) {
/// 		// handle error ...
/// 	}
/// 	// do other things...
/// }
/// @endcode
///
/// In most cases, `CnxError` would be used in tandem with `CnxResult(T)` to enable concise error
/// handling without having to use "out" parameters.
/// @}

#include <Cnx/BasicTypes.h>
#include <Cnx/Format.h>
#include <Cnx/Trait.h>
#include <errno.h>

#ifndef CNX_ERROR
	/// @brief Declarations and definitions related to `CnxError`
	#define CNX_ERROR

/// @brief `CnxErrorCategory` is a `Trait` that provides the mechanism to convert an arbitrary error
/// code into a corresponding message associated with a specific class of errors, and to get the
/// last error that occurred in a particular category (if supported by the category. For example,
/// global categories like `CNX_WIN32_ERROR_CATEGORY` and `CNX_POSIX_ERROR_CATEGORY` support this,
/// but categories local to specific modules probably won't).
///
/// `CnxErrorCategory` is what allows a `CnxError` to communicate error messages unique to the
/// module the error originated from.
/// @ingroup cnx_error
Trait(
	/// @brief `CnxErrorCategory` is a `Trait` that provides the mechanism to convert an arbitrary
	/// error code into a corresponding message associated with a specific class of errors, and to
	/// get the last error that occurred in a particular category (if supported by the category. For
	/// example, global categories like `CNX_WIN32_ERROR_CATEGORY` and `CNX_POSIX_ERROR_CATEGORY`
	/// support this, but categories local to specific modules probably won't).
	///
	/// `CnxErrorCategory` is what allows a `CnxError` to communicate error messages unique to the
	/// module the error originated from.
	/// @ingroup cnx_error
	CnxErrorCategory,
	/// @brief Retrieves the error message as `const_cstring` for the given `i64` error `code`
	const_cstring (*message)(const CnxErrorCategory* restrict self, i64 code);
	/// @brief Retrieves the `i64` code for the last reported error
	i64(*get_last_error)(const CnxErrorCategory* restrict self););

/// @brief `CnxError` provides an extensible, configurable type for communicating recoverable errors
/// via error codes and error message strings
///
/// Example
/// @code {.c}
/// CnxError do_thing(i64 input, i64* restrict out) {
///		// do things ...
/// 	if(errno) {
///			return cnx_error_new(errno, CNX_POSIX_ERROR_CATEGORY);
/// 	}
/// 	else {
/// 		*out = 42;
///	 		return cnx_error_new(0, CNX_POSIX_ERROR_CATEGORY);
/// 	}
/// }
///
/// void func(void) {
/// 	i64 i = 0;
/// 	let maybe_err = do_thing(10, &i);
/// 	if(cnx_error_code(&maybe_err) != 0) {
/// 		// handle error ...
/// 	}
/// 	// do other things...
/// }
/// @endcode
///
/// In most cases, `CnxError` would be used in tandem with `CnxResult(T)` to enable concise error
/// handling without having to use "out" parameters.
/// @ingroup cnx_error
typedef struct CnxError {
	/// @brief The error code associated with this error
	i64 m_error_code;
	/// @brief The error category to convert the error code to a textual message
	CnxErrorCategory m_error_category;
} CnxError;

	#define ___DISABLE_IF_NULL(self) \
		cnx_disable_if(!(self), "Can't perform an operation with a CnxError that is a nullptr")

/// @brief Creates a `CnxError` with the given error code and category
///
/// @param error_code - The error code associated with the error
/// @param error_category - The `CnxErrorCategory` that will generate the message associated with
/// the error code
///
/// @return a `CnxError`
/// @ingroup cnx_error
__attr(nodiscard) CnxError cnx_error_new(i64 error_code, CnxErrorCategory error_category);
/// @brief Returns the error code associated with the given error
///
/// @param self - The error
///
/// @return the associated error code
/// @ingroup cnx_error
__attr(nodiscard) __attr(not_null(1)) i64 cnx_error_code(const CnxError* restrict self)
	___DISABLE_IF_NULL(self);
/// @brief Returns the error message associated with the given error, as a `cstring`
///
/// @param self - The error
///
/// @return the associated error message
/// @ingroup cnx_error
__attr(nodiscard) __attr(not_null(1)) __attr(returns_not_null) const_cstring
	cnx_error_message(const CnxError* restrict self) ___DISABLE_IF_NULL(self);

/// @brief Returns the error message associated with the given error code, as a `cstring`
///
/// @param self - The error category to get the message from
/// @param error_code - The error code
///
/// @return the message associated with the error code
/// @ingroup cnx_error
__attr(nodiscard) __attr(returns_not_null) const_cstring
	cnx_error_category_get_message(CnxErrorCategory self, i64 error_code);
/// @brief Returns the error code for the last reported error
///
/// @param self - The error category to get the last error from
///
/// @return the `i64` error code representing the last reported error
/// @ingroup cnx_error
__attr(nodiscard) i64 cnx_error_category_get_last_error(CnxErrorCategory self);

	#undef ___DISABLE_IF_NULL

	#define ___DISABLE_IF_NULL(self) \
		cnx_disable_if(!(self),      \
					   "Can't perform an operation with a CnxErrorCategory that is a nullptr")

/// @brief Returns the POSIX error message associated with the given error code, as a `cstring`
///
/// @param error_code - The error code to get the message for
///
/// @return the message associated with the error code
/// @ingroup cnx_error
__attr(nodiscard) __attr(not_null(1)) __attr(returns_not_null) const_cstring
	cnx_posix_category_get_message(const CnxErrorCategory* restrict self, i64 error_code)
		___DISABLE_IF_NULL(self);
/// @brief Returns the error code for the last reported POSIX error
///
/// @return the error code for the last POSIX error
/// @ingroup cnx_error
__attr(nodiscard) __attr(not_null(1)) i64
	cnx_posix_category_get_last_error(const CnxErrorCategory* restrict self)
		___DISABLE_IF_NULL(self);

	#if CNX_PLATFORM_WINDOWS
/// @brief Returns the Win32 error message associated with the given error code, as a `cstring`
///
/// @param error_code - The error code to get the message for
///
/// @return the message associated with the error code
/// @ingroup cnx_error
__attr(nodiscard) __attr(not_null(1)) __attr(returns_not_null) const_cstring
	cnx_win32_category_get_message(const CnxErrorCategory* restrict self, i64 error_code)
		___DISABLE_IF_NULL(self);
/// @brief Returns the error code for the last reported Win32 error
///
/// @return the error code for the last Win32 error
/// @ingroup cnx_error
__attr(nodiscard) __attr(not_null(1)) i64
	cnx_win32_category_get_last_error(const CnxErrorCategory* restrict self)
		___DISABLE_IF_NULL(self);
	#endif // CNX_PLATFORM_WINDOWS

	#undef ___DISABLE_IF_NULL
	#define ___DISABLE_IF_NULL(self) \
		cnx_disable_if(!(self), "Can't perform an operation with a CnxError that is a nullptr")

/// @brief Implementation of `CnxFormat.is_specifier_valid` for `CnxError`
///
/// @param self - The `CnxError` to format as a `CnxFormat` trait object
/// @param specifier - The `CnxStringView` viewing the format specifier to validate
///
/// @return The `CnxFormatContext` indicating whether specifier was valid and storing the state
/// holding the format settings and necessary info to format the `CnxError`
__attr(nodiscard) __attr(not_null(1)) CnxFormatContext
	cnx_error_is_specifier_valid(const CnxFormat* restrict self, CnxStringView specifier)
		___DISABLE_IF_NULL(self);
/// @brief Implementation of `CnxFormat.format` for `CnxError`
///
/// @param self - The `CnxError` to format as a `CnxFormat` trait object
/// @param context - The `CnxFormatContext` specifying how formatting should be done
///
/// @return `self` formatted as a `CnxString`
__attr(nodiscard) __attr(not_null(1)) CnxString
	cnx_error_format(const CnxFormat* restrict self, CnxFormatContext context)
		___DISABLE_IF_NULL(self);
/// @brief Implementation of `CnxFormat.format_with_allocator` for `CnxError`
///
/// @param self - The `CnxError` to format as a `CnxFormat` trait object
/// @param context - The `CnxFormatContext` specifying how formatting should be done
/// @param allocator - The `CnxAllocator` to allocate the formatted string with
///
/// @return `self` formatted as a `CnxString`
__attr(nodiscard) __attr(not_null(1)) CnxString
	cnx_error_format_with_allocator(const CnxFormat* restrict self,
									CnxFormatContext context,
									CnxAllocator allocator) ___DISABLE_IF_NULL(self);

/// @brief Implement `CnxFormat` for `CnxError`
/// @return The `CnxFormat` trait implementation for `CnxError`
/// @ingroup cnx_error
__attr(maybe_unused) static ImplTraitFor(CnxFormat,
										 CnxError,
										 cnx_error_is_specifier_valid,
										 cnx_error_format,
										 cnx_error_format_with_allocator);

typedef struct CnxPosixErrorCategory {
} CnxPosixErrorCategory;

__attr(maybe_unused) static ImplTraitFor(CnxErrorCategory,
										 CnxPosixErrorCategory,
										 cnx_posix_category_get_message,
										 cnx_posix_category_get_last_error);

IGNORE_RESERVED_IDENTIFIER_WARNING_START
__attr(maybe_unused) static const CnxPosixErrorCategory __cnx_posix_error_category = {};
__attr(maybe_unused) static const CnxErrorCategory __cnx_posix_category
	= as_trait(CnxErrorCategory, CnxPosixErrorCategory, __cnx_posix_error_category);
IGNORE_RESERVED_IDENTIFIER_WARNING_STOP

	/// @brief The `CnxErrorCategory` to map POSIX error codes
	///
	/// `CNX_POSIX_ERROR_CATEGORY` is the error category to map POSIX error codes to their
	/// associated error messages. It will produce equivalent results to `strerror` in standard C
	/// @ingroup cnx_error
	#define CNX_POSIX_ERROR_CATEGORY __cnx_posix_category

	#if CNX_PLATFORM_WINDOWS

typedef struct CnxWin32ErrorCategory {
} CnxWin32ErrorCategory;

__attr(maybe_unused) static ImplTraitFor(CnxErrorCategory,
										 CnxWin32ErrorCategory,
										 cnx_win32_category_get_message,
										 cnx_win32_category_get_last_error);

IGNORE_RESERVED_IDENTIFIER_WARNING_START
__attr(maybe_unused) static const CnxWin32ErrorCategory __cnx_win32_error_category = {};
__attr(maybe_unused) static const CnxErrorCategory __cnx_win32_category
	= as_trait(CnxErrorCategory, CnxWin32ErrorCategory, __cnx_win32_error_category);

		/// @brief The `CnxErrorCategory` to map Win32 error codes
		///
		/// `CNX_WIN32_ERROR_CATEGORY` is the error category to map Win32 error codes to their
		/// associated error messages. It will produce equivalent results to `FormatMessageW` in
		/// the Win32 API
		/// @ingroup cnx_error
		#define CNX_WIN32_ERROR_CATEGORY __cnx_win32_category
IGNORE_RESERVED_IDENTIFIER_WARNING_STOP

	#endif // CNX_PLATFORM_WINDOWS

	#ifndef CNX_DEFAULT_ERROR_CATEGORY
		/// @brief the default `CnxErrorCategory`. By default, this is `CNX_POSIX_ERROR_CATEGORY`
		///
		/// This can be overridden by defining `CNX_DEFAULT_ERROR_CATEGORY` to a compound literal
		/// of your chosen `CnxErrorCategory` prior to including <C2nxt/Error.h>
		/// @ingroup cnx_error
		#define CNX_DEFAULT_ERROR_CATEGORY CNX_POSIX_ERROR_CATEGORY
	#endif // CNX_DEFAULT_ERROR_CATEGORY

	#undef ___DISABLE_IF_NULL
#endif // CNX_ERROR
