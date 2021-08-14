/// @file StdFormat.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief StdFormat bings human readable string formatting, similar to C++'s `std::format` and
/// `fmtlib`, and Rust's std::format, to C.
/// @version 0.1
/// @date 2021-07-20
///
/// MIT License
/// @copyright Copyright (c) 2021 Braxton Salyer <braxtonsalyer@gmail.com>
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

#include <stdarg.h>

#include "StdAssert.h"
#include "StdBasicTypes.h"
#include "StdString.h"
#include "StdTrait.h"

/// @defgroup format String Formatting
/// C2nxt's formatting API creates a composable, ergonomic, human-readable way to format data into
/// strings for storage or I/O.
///
/// The interface and functionality is similar to C++'s `std::format`, and the popular `fmtlib` it's
/// derived from, and Rust's `std::format`.
/// Example:
/// @code {.c}
/// let x = 0.0F;
/// let y = 1.0F;
/// // formats x and y as normal decimal format with one significant figure after the decimal point
/// let formatted = std_format("x: {d1}, y: {d1}", x, y);
/// @endcode
/// Formatting currently supports the following optional format specifiers:
/// 1. ```'d'```: standard decimal formatting. Applies to integral types and floating point numbers.
/// Floating point numbers accept an additional numeric postfix to set the number of digits after
/// the decimal point.
/// 2. ```'x'```: lower-case hexadecimal. Applies to integral types.
/// 3. ```'X'```: upper-case hexadecimal. Applies to integral types.
/// 4. ```'e'```: Scientific notation. Applies to floating point types. Accepts an additional
/// numeric postfix to set the number of digits after the decimal point.
///
/// The default for integral types is decimal, and the default for floating point numbers is
/// scientific. The default number of digits after the decimal point for floating point types is 3.
/// `bool`s are special cased and do not accept a format specifier. They will format directly to
/// "true" or "false".
///
/// Format allows for extension and composition of formatting for user-defined types by allowing the
/// implementation of the `StdFormat` Trait for those types.
///
/// To provide an implementation of `StdFormat` for your type, only two functons and the Trait
/// implementation are required. The functions take the following signatures:
///
/// @code {.c}
/// StdString (*const your_format)(const StdFormat* restrict self, StdFormatSpecifier specifier);
/// StdString (*const your_format_with_allocator)(const StdFormat* restrict self,
/// 											  StdFormatSpecifier specifier,
/// 											  StdAllocator allocator);
/// @endcode
///
/// And providing the Trait implementation is as simple as:
///
/// @code {.c}
/// ImplTraitFor(StdFormat, your_type, your_format, your_format_with_allocator);
/// @endcode
///
/// In practice, you will probably be providing this Trait implementation in a header file, so
/// you'll also probably want to mark it as `static` and `maybe_unused`:
///
/// @code {.c}
/// static maybe_unused ImplTraitFor(StdFormat, your_type, your_format, your_format_with_allocator);
/// @endcode
///
/// By default, the conversion to `StdFormat` is only automatic for builtin types and some extra
/// types like `StdString`, and for user-defined types the conversion will have to be made
/// explicitly by calling `as_format_t(T, x)` on variables of those types. However, automatic
/// conversions for **specific** user defined types can be enabled by defining a pair of macros
/// prior to including "StdFormat.h". First, define @ref STD_AS_FORMAT_USES_USER_SUPPLIED_TYPES to
/// `true`. Then, define @ref STD_AS_FORMAT_USER_SUPPLIED_TYPES to a comma separated list of types
/// and their conversions. For example:
/// @code {.c}
/// #define STD_AS_FORMAT_USES_USER_SUPPLIED_TYPES true
/// #define STD_AS_FORMAT_USER_SUPPLIED_TYPES T* 		: as_format_t(T, x),	\
/// 										  const T* 	: as_format_t(T2, x), 	\
/// 										  T2* 		: as_format_t(T2, x), 	\
/// 										  const T2* : as_format_t(T2, x),
/// @endcode
/// where `T` and `T2` are your supplied types. `x` should always be the second argument to the
/// conversion function (literally always give `x` as the second argument).
/// `std_format(format_string, ...)` automatically applies the `as_format(x)` `_Generic` macro to
/// all format arguments in the parameter pack to perform the automatic conversion to `StdFormat`,
/// and this syntax is necessary to provide valid match arms in the `as_format(x)` macro for that
/// conversion.
///
/// A complete example of implementing and using `StdFormat`:
/// @code {.c}
/// typedef struct Point2D {
/// 	float x;
/// 	float y;
/// } Point2D;
///
/// #define STD_AS_FORMAT_USES_USER_SUPPLIED_TYPES true
/// #define STD_AS_FORMAT_USER_SUPPLIED_TYPES Point2D* 			: as_format_t(Point2D, x), 	\
/// 										  const Point2D* 	: as_format_t(Point2D, x),
///
/// #include "C2nxt/StdFormat.h"
///
/// inline static StdString point2d_format_with_allocator(const StdFormat* restrict self,
/// 													  StdFormatSpecifier specifier,
/// 													  StdAllocator allocator) {
/// 	// we won't user the specifier for our implementation, so ignore it.
/// 	// As an alternative, we could use the specifier to determine how we should format
///		// the `x` and `y` coordinates (decimal, scientific, number of sig figs, etc), but that
/// 	// would be beyond the scope of this simple example.
/// 	ignore(specifier);
/// 	let _self = static_cast(const Point2D*)(self.m_self);
/// 	return std_format_with_allocator("Point2D: [x: {}, y: {}]", allocator, _self->x, _self->y);
/// }
///
/// inline static StdString point2d_format(const StdFormat* restrict self,
/// 									   StdFormatSpecifier specifier) {
///		return point2d_format_with_allocator(self, specifier, std_allocator_new());
/// }
///
/// static maybe_unused
/// ImplTraitFor(StdFormat, Point2D, point2d_format, point2d_format_with_allocator);
///
/// void point2d_print_legacy(const Point2D* restrict self) {
/// 	let formatted = std_format("{}", *self);
///		printf("%s", std_string_into_cstring(formatted));
/// }
/// @endcode

#ifndef STD_FORMAT
	/// @brief Declarations and Definitons related to `std_format(format_string, ...)`, `StdFormat`,
	/// and C2nxt string formatting
	#define STD_FORMAT

/// @brief C2nxt string formatting valid format specifiers
///
/// Formatting currently supports the following optional format specifiers:
/// 1. ```'d'```: standard decimal formatting. Applies to integral types and floating point numbers.
/// Floating point numbers accept an additional numeric postfix to set the number of digits after
/// the decimal point.
/// 2. ```'x'```: lower-case hexadecimal. Applies to integral types.
/// 3. ```'X'```: upper-case hexadecimal. Applies to integral types.
/// 4. ```'e'```: Scientific notation. Applies to floating point types. Accepts an additional
/// numeric postfix to set the number of digits after the decimal point.
///
/// The default for integral types is decimal, and the default for floating point numbers is
/// scientific. The default number of digits after the decimal point for floating point types is 3.
/// `bool`s are special cased and do not accept a format specifier. They will format directly to
/// "true" or "false".
/// @ingroup format
typedef enum StdFormatTypes
{
	STD_FORMAT_TYPE_DEFAULT = 0,
	STD_FORMAT_TYPE_DECIMAL = 'd',
	STD_FORMAT_TYPE_HEX_LOWER = 'x',
	STD_FORMAT_TYPE_HEX_UPPER = 'X',
	STD_FORMAT_TYPE_SCIENTIFIC = 'e'
} StdFormatTypes;

	/// @brief The default number of significant figures for floating point formatting
	///
	/// By default, C2nxt floating point formatting provides 3 significant figures after the
	/// decimal point in formatted output
	/// @ingroup format
	#define STD_FORMAT_DEFAULT_NUM_SIG_FIGS 3

/// @brief `StdFormatSpecifier` provides a method for passing format specifiers to other steps
/// in the formatting process
///
/// `StdFormatSpecifier` is used internally by the builtin format implementations, and can be used
/// by custom implementations, to communicate the required formatting method to the associated
/// formatting functions
/// @ingroup format
typedef struct StdFormatSpecifier {
	/// @brief The type of the format specifier
	StdFormatTypes m_type;
	/// @brief The number of significant figures after the decimal point, if the specifier is
	/// for a floating point type
	usize m_num_sig_figs;
} StdFormatSpecifier;

/// @brief Formats the various parameter pack arguments into their associated place in the given
/// format string, using the provided allocator
///
/// @param format_string - The string specifying the format positions, specifiers, and other text
/// 				       that should be present in the output string
/// @param allocator - The `StdAllocator` to allocate the output string with
/// @param num_args - The number of arguments in the parameter pack
/// @param ... - The parameter pack of arguments to be formatted
///
/// @return The formatted output string
StdString std_format_with_allocator(const_cstring restrict format_string,
									StdAllocator allocator,
									usize num_args,
									...);

/// @brief Formats the various parameter pack arguments into their associated place in the given
/// format string, using the provided allocator
///
/// @param format_string - The string specifying the format positions, specifiers, and other text
/// 				       that should be present in the output string
/// @param allocator - The `StdAllocator` to allocate the output string with
/// @param num_args - The number of arguments in the parameter pack
/// @param list - The parameter pack of arguments to be formatted
///
/// @return The formatted output string
StdString std_vformat_with_allocator(const_cstring restrict format_string,
									 StdAllocator allocator,
									 usize num_args,
									 va_list list);

	/// @brief Converts the given variable into its associated `StdFormat` Trait implementation
	///
	/// There must be an implementation of `StdFormat` for the type `T` and `x` must be an lvalue
	/// of type `T`.
	///
	/// @param T - The concrete type of `x`
	/// @param x - The variable to convert to its `StdFormat` Trait implementation
	///
	/// @return `x` as `StdFormat`
	/// @ingroup format
	#define as_format_t(T, x) as_trait(StdFormat, T, x)

	/// @def STD_AS_FORMAT_USES_USER_SUPPLIED_TYPES
	/// @brief Feature enable macro to allow specific user-defined types to be automatically
	/// converted to their `StdFormat` implementation when passed to
	/// `std_format(format_string, ...)` or a similar C2nxt string formatting function
	///
	/// Requires that @ref STD_AS_FORMAT_USER_SUPPLIED_TYPES is also defined to a comma separated
	/// list of conversions
	/// Example:
	/// @code {.c}
	/// #define STD_AS_FORMAT_USES_USER_SUPPLIED_TYPES true
	/// #define STD_AS_FORMAT_USER_SUPPLIED_TYPES T* 		: as_format_t(T, x),	\
	/// 										  const T* 	: as_format_t(T2, x), 	\
	/// 										  T2* 		: as_format_t(T2, x), 	\
	/// 										  const T2* : as_format_t(T2, x),
	/// @endcode
	/// @ingroup format
	#ifndef STD_AS_FORMAT_USES_USER_SUPPLIED_TYPES
		#define STD_AS_FORMAT_USES_USER_SUPPLIED_TYPES 0
	#endif

	/// @def STD_AS_FORMAT_USER_SUPPLIED_TYPES
	/// @brief Define this macro to a comma separated list of conversions to enable specific
	/// user-defined types to be automatically converted to their `StdFormat` implementation
	/// when passed to `std_format(format_string, ...)` or a similar C2nxt string formatting
	/// function
	///
	/// Requires that @ref STD_AS_FORMAT_USES_USER_SUPPLIED_TYPES is also defined to `true`
	/// Example:
	/// @code {.c}
	/// #define STD_AS_FORMAT_USES_USER_SUPPLIED_TYPES true
	/// #define STD_AS_FORMAT_USER_SUPPLIED_TYPES T* 		: as_format_t(T, x),	\
	/// 										  const T* 	: as_format_t(T2, x), 	\
	/// 										  T2* 		: as_format_t(T2, x), 	\
	/// 										  const T2* : as_format_t(T2, x),
	/// @endcode
	/// @ingroup format
	#ifndef STD_AS_FORMAT_USER_SUPPLIED_TYPES
		#define STD_AS_FORMAT_USER_SUPPLIED_TYPES
	#endif

	#if STD_PLATFORM_APPLE
		#if STD_AS_FORMAT_USES_USER_SUPPLIED_TYPES
		// clang-format off
		/// @brief Converts the given variable into its associated `StdFormat` Trait implementation
		///
		/// There must be an implementation of `StdFormat` for the type of `x` and `x` must be an
		/// lvalue
		///
		/// @param x - The variable to convert to its `StdFormat` Trait implementation
		///
		/// @return `x` as `StdFormat`
		/// @ingroup format
		#define as_format(x) _Generic((&(x)), 										\
				char [sizeof((x))]  		:   as_format_t(cstring, x), 			\
				bool* 						: 	as_format_t(bool, x), 				\
				char* 						: 	as_format_t(char, x), 				\
				u8* 						: 	as_format_t(u8, x), 				\
				u16* 						: 	as_format_t(u16, x), 				\
				u32* 						: 	as_format_t(u32, x), 				\
				u64* 						: 	as_format_t(u64, x), 				\
				usize* 						: 	as_format_t(u64, x), 				\
				i8* 						: 	as_format_t(i8, x), 				\
				i16* 						: 	as_format_t(i16, x), 				\
				i32* 						: 	as_format_t(i32, x), 				\
				i64* 						: 	as_format_t(i64, x), 				\
				isize* 						: 	as_format_t(i64, x), 				\
				f32* 						: 	as_format_t(f32, x), 				\
				f64* 						: 	as_format_t(f64, x), 				\
				nullptr_t* 					: 	as_format_t(nullptr_t, x), 			\
				StdString* 					: 	as_format_t(StdString, x), 			\
				StdError* 					: 	as_format_t(StdError, x), 			\
				StdFormat* 					: 	(x), 								\
				const char [sizeof((x))]    :   as_format_t(cstring, x), 			\
				const bool* 				: 	as_format_t(bool, x), 				\
				const char* 				: 	as_format_t(char, x), 				\
				const u8* 					: 	as_format_t(u8, x), 				\
				const u16* 					: 	as_format_t(u16, x), 				\
				const u32* 					: 	as_format_t(u32, x), 				\
				const u64* 					: 	as_format_t(u64, x), 				\
				const usize* 				: 	as_format_t(u64, x), 				\
				const i8* 					: 	as_format_t(i8, x), 				\
				const i16* 					: 	as_format_t(i16, x), 				\
				const i32* 					: 	as_format_t(i32, x), 				\
				const i64* 					: 	as_format_t(i64, x), 				\
				const isize* 				: 	as_format_t(i64, x), 				\
				const f32* 					: 	as_format_t(f32, x), 				\
				const f64* 					: 	as_format_t(f64, x), 				\
				const nullptr_t*			: 	as_format_t(nullptr_t, x), 			\
				const StdString*			: 	as_format_t(StdString, x), 			\
				const StdError*				: 	as_format_t(StdError, x), 			\
				STD_AS_FORMAT_USER_SUPPLIED_TYPES 									\
				const StdFormat* 			: 	(x))
	#else
		/// @brief Converts the given variable into its associated `StdFormat` Trait implementation
		///
		/// There must be an implementation of `StdFormat` for the type of `x` and `x` must be an
		/// lvalue
		///
		/// @param x - The variable to convert to its `StdFormat` Trait implementation
		///
		/// @return `x` as `StdFormat`
		/// @ingroup format
		#define as_format(x) _Generic((&(x)), 							    		\
				char (*)[sizeof((x))]  		:   as_format_t(cstring, x), 			\
				bool* 						: 	as_format_t(bool, x), 				\
				char* 						: 	as_format_t(char, x), 				\
				u8* 						: 	as_format_t(u8, x), 				\
				u16* 						: 	as_format_t(u16, x), 				\
				u32* 						: 	as_format_t(u32, x), 				\
				u64* 						: 	as_format_t(u64, x), 				\
				usize* 						: 	as_format_t(u64, x), 				\
				i8* 						: 	as_format_t(i8, x), 				\
				i16* 						: 	as_format_t(i16, x), 				\
				i32* 						: 	as_format_t(i32, x), 				\
				i64* 						: 	as_format_t(i64, x), 				\
				isize* 						: 	as_format_t(i64, x), 				\
				f32* 						: 	as_format_t(f32, x), 				\
				f64* 						: 	as_format_t(f64, x), 				\
				nullptr_t* 					: 	as_format_t(nullptr_t, x), 			\
				StdString* 					: 	as_format_t(StdString, x), 			\
				StdError* 					: 	as_format_t(StdError, x), 			\
				StdFormat* 					: 	(x), 								\
				const char (*)[sizeof((x))] :   as_format_t(cstring, x), 			\
				const bool* 				: 	as_format_t(bool, x), 				\
				const char* 				: 	as_format_t(char, x), 				\
				const u8* 					: 	as_format_t(u8, x), 				\
				const u16* 					: 	as_format_t(u16, x), 				\
				const u32* 					: 	as_format_t(u32, x), 				\
				const u64* 					: 	as_format_t(u64, x), 				\
				const usize* 				: 	as_format_t(u64, x), 				\
				const i8* 					: 	as_format_t(i8, x), 				\
				const i16* 					: 	as_format_t(i16, x), 				\
				const i32* 					: 	as_format_t(i32, x), 				\
				const i64* 					: 	as_format_t(i64, x), 				\
				const isize* 				: 	as_format_t(i64, x), 				\
				const f32* 					: 	as_format_t(f32, x), 				\
				const f64* 					: 	as_format_t(f64, x), 				\
				const nullptr_t*			: 	as_format_t(nullptr_t, x), 			\
				const StdString*			: 	as_format_t(StdString, x), 			\
				const StdError*				: 	as_format_t(StdError, x), 			\
				const StdFormat*			: 	(x))
		// clang-format on
		#endif // STD_AS_FORMAT_USES_USER_SUPPLIED_TYPES
	#else
		#if STD_AS_FORMAT_USES_USER_SUPPLIED_TYPES
		// clang-format off
		/// @brief Converts the given variable into its associated `StdFormat` Trait implementation
		///
		/// There must be an implementation of `StdFormat` for the type of `x` and `x` must be an
		/// lvalue
		///
		/// @param x - The variable to convert to its `StdFormat` Trait implementation
		///
		/// @return `x` as `StdFormat`
		/// @ingroup format
		#define as_format(x) _Generic((&(x)), 										\
				char [sizeof((x))]  		:   as_format_t(cstring, x), 			\
				bool* 						: 	as_format_t(bool, x), 				\
				char* 						: 	as_format_t(char, x), 				\
				u8* 						: 	as_format_t(u8, x), 				\
				u16* 						: 	as_format_t(u16, x), 				\
				u32* 						: 	as_format_t(u32, x), 				\
				u64* 						: 	as_format_t(u64, x), 				\
				i8* 						: 	as_format_t(i8, x), 				\
				i16* 						: 	as_format_t(i16, x), 				\
				i32* 						: 	as_format_t(i32, x), 				\
				i64* 						: 	as_format_t(i64, x), 				\
				f32* 						: 	as_format_t(f32, x), 				\
				f64* 						: 	as_format_t(f64, x), 				\
				nullptr_t* 					: 	as_format_t(nullptr_t, x), 			\
				StdString* 					: 	as_format_t(StdString, x), 			\
				StdError* 					: 	as_format_t(StdError, x), 			\
				StdFormat* 					: 	(x), 								\
				const char [sizeof((x))]    :   as_format_t(cstring, x), 			\
				const bool* 				: 	as_format_t(bool, x), 				\
				const char* 				: 	as_format_t(char, x), 				\
				const u8* 					: 	as_format_t(u8, x), 				\
				const u16* 					: 	as_format_t(u16, x), 				\
				const u32* 					: 	as_format_t(u32, x), 				\
				const u64* 					: 	as_format_t(u64, x), 				\
				const i8* 					: 	as_format_t(i8, x), 				\
				const i16* 					: 	as_format_t(i16, x), 				\
				const i32* 					: 	as_format_t(i32, x), 				\
				const i64* 					: 	as_format_t(i64, x), 				\
				const f32* 					: 	as_format_t(f32, x), 				\
				const f64* 					: 	as_format_t(f64, x), 				\
				const nullptr_t*			: 	as_format_t(nullptr_t, x), 			\
				const StdString*			: 	as_format_t(StdString, x), 			\
				const StdError*				: 	as_format_t(StdError, x), 			\
				STD_AS_FORMAT_USER_SUPPLIED_TYPES 									\
				const StdFormat* 			: 	(x))
	#else
		/// @brief Converts the given variable into its associated `StdFormat` Trait implementation
		///
		/// There must be an implementation of `StdFormat` for the type of `x` and `x` must be an
		/// lvalue
		///
		/// @param x - The variable to convert to its `StdFormat` Trait implementation
		///
		/// @return `x` as `StdFormat`
		/// @ingroup format
		#define as_format(x) _Generic((&(x)), 							    		\
				char (*)[sizeof((x))]  		:   as_format_t(cstring, x), 			\
				bool* 						: 	as_format_t(bool, x), 				\
				char* 						: 	as_format_t(char, x), 				\
				u8* 						: 	as_format_t(u8, x), 				\
				u16* 						: 	as_format_t(u16, x), 				\
				u32* 						: 	as_format_t(u32, x), 				\
				u64* 						: 	as_format_t(u64, x), 				\
				i8* 						: 	as_format_t(i8, x), 				\
				i16* 						: 	as_format_t(i16, x), 				\
				i32* 						: 	as_format_t(i32, x), 				\
				i64* 						: 	as_format_t(i64, x), 				\
				f32* 						: 	as_format_t(f32, x), 				\
				f64* 						: 	as_format_t(f64, x), 				\
				nullptr_t* 					: 	as_format_t(nullptr_t, x), 			\
				StdString* 					: 	as_format_t(StdString, x), 			\
				StdError* 					: 	as_format_t(StdError, x), 			\
				StdFormat* 					: 	(x), 								\
				const char (*)[sizeof((x))] :   as_format_t(cstring, x), 			\
				const bool* 				: 	as_format_t(bool, x), 				\
				const char* 				: 	as_format_t(char, x), 				\
				const u8* 					: 	as_format_t(u8, x), 				\
				const u16* 					: 	as_format_t(u16, x), 				\
				const u32* 					: 	as_format_t(u32, x), 				\
				const u64* 					: 	as_format_t(u64, x), 				\
				const i8* 					: 	as_format_t(i8, x), 				\
				const i16* 					: 	as_format_t(i16, x), 				\
				const i32* 					: 	as_format_t(i32, x), 				\
				const i64* 					: 	as_format_t(i64, x), 				\
				const f32* 					: 	as_format_t(f32, x), 				\
				const f64* 					: 	as_format_t(f64, x), 				\
				const nullptr_t*			: 	as_format_t(nullptr_t, x), 			\
				const StdString*			: 	as_format_t(StdString, x), 			\
				const StdError*				: 	as_format_t(StdError, x), 			\
				const StdFormat*			: 	(x))
		// clang-format on
		#endif // STD_AS_FORMAT_USES_USER_SUPPLIED_TYPES
	#endif

	/// @brief Formats the various parameter pack arguments into their associated place in the given
	/// format string, using the provided allocator
	///
	/// @param format_string - The string specifying the format positions, specifiers, and other
	/// 					   text that should be present in the output string
	/// @param allocator - The `StdAllocator` to allocate the output string with
	/// @param ... - The parameter pack of arguments to be formatted
	///
	/// @return The formatted output string
	/// @ingroup format
	#define std_format_with_allocator(format_string, allocator, ...) \
		(std_format_with_allocator)(format_string,                   \
									allocator,                       \
									PP_NUM_ARGS(__VA_ARGS__)         \
										__VA_OPT__(, APPLY_TO_LIST(as_format, __VA_ARGS__)))
	/// @brief Formats the various parameter pack arguments into their associated place in the given
	/// format string, using the default allocator
	///
	/// @param format_string - The string specifying the format positions, specifiers, and other
	/// 					   text that should be present in the output string
	/// @param ... - The parameter pack of arguments to be formatted
	///
	/// @return The formatted output string
	/// @ingroup format
	#define std_format(format_string, ...) \
		std_format_with_allocator(format_string, std_allocator_new() __VA_OPT__(, ) __VA_ARGS__)
	/// @brief Formats the various `va_list` parameter pack arguments into their associated place in
	/// the given format string, using the provided allocator
	///
	/// @param format_string - The string specifying the format positions, specifiers, and other
	/// 					   text that should be present in the output string
	/// @param allocator - The `StdAllocator` to allocate the output string with
	/// @param num_args - The number of arguments in the parameter pack
	/// @param list - The `va_list` parameter pack of arguments to be formatted
	///
	/// @return The formatted output string
	/// @ingroup format
	#define std_vformat_with_allocator(format_string, allocator, num_args, list) \
		(std_vformat_with_allocator)(format_string, allocator, num_args, list)
	/// @brief Formats the various `va_list` parameter pack arguments into their associated place in
	/// the given format string, using the default allocator
	///
	/// @param format_string - The string specifying the format positions, specifiers, and other
	/// 					   text that should be present in the output string
	/// @param num_args - The number of arguments in the parameter pack
	/// @param list - The `va_list` parameter pack of arguments to be formatted
	///
	/// @return The formatted output string
	/// @ingroup format
	#define std_vformat(format_string, num_args, list) \
		std_vformat_with_allocator(format_string, std_allocator_new(), num_args, list)

// clang-format off

/// @typedef StdFormat
/// @brief `StdFormat` is the Trait which allows extensible and composable string formatting
/// of builtin and user-defined types.
///
/// `StdFormat` requires an implementation of `format`, to format the associated type with
/// the default system allocator, and `format_with_allocator`, to format the associated type
/// with a user-provided allocator.
///
/// To provide an implementation of `StdFormat` for your type, only two functons and the
/// Trait implementation are required. The functions take the following signatures:
///
/// @code {.c}
/// StdString (*const your_format)(const StdFormat* restrict self, StdFormatSpecifier specifier);
/// StdString (*const your_format_with_allocator)(const StdFormat* restrict self,
/// 											  StdFormatSpecifier specifier,
/// 											  StdAllocator allocator);
/// @endcode
///
/// And providing the Trait implementation is as simple as:
///
/// @code {.c}
/// ImplTraitFor(StdFormat, your_type, your_format, your_format_with_allocator);
/// @endcode
///
/// In practice, you will probably be providing this Trait implementation in a header file,
/// so you'll also probably want to mark it as `static` and `maybe_unused`:
///
/// @code {.c}
/// static maybe_unused ImplTraitFor(StdFormat, your_type, your_format, your_format_with_allocator);
/// @endcode
///
/// @ingroup format
Trait(
/// @struct StdFormat
/// @brief `StdFormat` is the Trait which allows extensible and composable string formatting
/// of builtin and user-defined types.
///
/// `StdFormat` requires an implementation of `format`, to format the associated type with
/// the default system allocator, and `format_with_allocator`, to format the associated type
/// with a user-provided allocator.
///
/// To provide an implementation of `StdFormat` for your type, only two functons and the
/// Trait implementation are required. The functions take the following signatures:
///
/// @code {.c}
/// StdString (*const your_format)(const StdFormat* restrict self, StdFormatSpecifier specifier);
/// StdString (*const your_format_with_allocator)(const StdFormat* restrict self,
/// 											  StdFormatSpecifier specifier,
/// 											  StdAllocator allocator);
/// @endcode
///
/// And providing the Trait implementation is as simple as:
///
/// @code {.c}
/// ImplTraitFor(StdFormat, your_type, your_format, your_format_with_allocator);
/// @endcode
///
/// In practice, you will probably be providing this Trait implementation in a header file,
/// so you'll also probably want to mark it as `static` and `maybe_unused`:
///
/// @code {.c}
/// static maybe_unused ImplTraitFor(StdFormat, your_type, your_format, your_format_with_allocator);
/// @endcode
///
/// @ingroup format
	StdFormat,
   /** @brief formats the given `StdFormat` Trait object according to the given specifier 		 **/
   /** 																						 	 **/
   /** @param self - The `StdFormat` Trait object to format 									 **/
   /** @param specifier - The `StdFormatSpecifier` indicating what format method to use 		 **/
   /** 																						 	 **/
   /** @return the formatted string associated with `self` 									 	 **/
   StdString (*const format)(const StdFormat* restrict self, StdFormatSpecifier specifier);
   /** @brief formats the given `StdFormat` Trait object according to the given specifier 	 	 **/
   /** 																						 	 **/
   /** @param self - The `StdFormat` Trait object to format 									 **/
   /** @param specifier - The `StdFormatSpecifier` indicating what format method to use 		 **/
   /** @param allocator - The `StdAllocator` to allocate the formatted string with 			 	 **/
   /** 																						 	 **/
   /** @return the formatted string associated with `self` 									 	 **/
   StdString(*const format_with_allocator)(const StdFormat* restrict self,
										   StdFormatSpecifier specifier,
										   StdAllocator allocator););
// clang-format on

/// @brief implementation of `StdFormat.format` for the builtin `cstring`
///
/// @param self - The `StdFormat` object to format
/// @param specifier - The format specifier to follow
///
/// @return a formatted string associated with `self`
StdString std_format_cstring(const StdFormat* restrict self, StdFormatSpecifier specifier);
/// @brief implementation of `StdFormat.format` for the builtin `bool`
///
/// @param self - The `StdFormat` object to format
/// @param specifier - The format specifier to follow
///
/// @return a formatted string associated with `self`
StdString std_format_bool(const StdFormat* restrict self, StdFormatSpecifier specifier);
/// @brief implementation of `StdFormat.format` for the builtin `char`
///
/// @param self - The `StdFormat` object to format
/// @param specifier - The format specifier to follow
///
/// @return a formatted string associated with `self`
StdString std_format_char(const StdFormat* restrict self, StdFormatSpecifier specifier);
/// @brief implementation of `StdFormat.format` for the builtin `u8`
///
/// @param self - The `StdFormat` object to format
/// @param specifier - The format specifier to follow
///
/// @return a formatted string associated with `self`
StdString std_format_u8(const StdFormat* restrict self, StdFormatSpecifier specifier);
/// @brief implementation of `StdFormat.format` for the builtin `u16`
///
/// @param self - The `StdFormat` object to format
/// @param specifier - The format specifier to follow
///
/// @return a formatted string associated with `self`
StdString std_format_u16(const StdFormat* restrict self, StdFormatSpecifier specifier);
/// @brief implementation of `StdFormat.format` for the builtin `u32`
///
/// @param self - The `StdFormat` object to format
/// @param specifier - The format specifier to follow
///
/// @return a formatted string associated with `self`
StdString std_format_u32(const StdFormat* restrict self, StdFormatSpecifier specifier);
/// @brief implementation of `StdFormat.format` for the builtin `u64`
///
/// @param self - The `StdFormat` object to format
/// @param specifier - The format specifier to follow
///
/// @return a formatted string associated with `self`
StdString std_format_u64(const StdFormat* restrict self, StdFormatSpecifier specifier);
/// @brief implementation of `StdFormat.format` for the builtin `i8`
///
/// @param self - The `StdFormat` object to format
/// @param specifier - The format specifier to follow
///
/// @return a formatted string associated with `self`
StdString std_format_i8(const StdFormat* restrict self, StdFormatSpecifier specifier);
/// @brief implementation of `StdFormat.format` for the builtin `i16`
///
/// @param self - The `StdFormat` object to format
/// @param specifier - The format specifier to follow
///
/// @return a formatted string associated with `self`
StdString std_format_i16(const StdFormat* restrict self, StdFormatSpecifier specifier);
/// @brief implementation of `StdFormat.format` for the builtin `i32`
///
/// @param self - The `StdFormat` object to format
/// @param specifier - The format specifier to follow
///
/// @return a formatted string associated with `self`
StdString std_format_i32(const StdFormat* restrict self, StdFormatSpecifier specifier);
/// @brief implementation of `StdFormat.format` for the builtin `i64`
///
/// @param self - The `StdFormat` object to format
/// @param specifier - The format specifier to follow
///
/// @return a formatted string associated with `self`
StdString std_format_i64(const StdFormat* restrict self, StdFormatSpecifier specifier);
/// @brief implementation of `StdFormat.format` for the builtin `f32`
///
/// @param self - The `StdFormat` object to format
/// @param specifier - The format specifier to follow
///
/// @return a formatted string associated with `self`
StdString std_format_f32(const StdFormat* restrict self, StdFormatSpecifier specifier);
/// @brief implementation of `StdFormat.format` for the builtin `f64`
///
/// @param self - The `StdFormat` object to format
/// @param specifier - The format specifier to follow
///
/// @return a formatted string associated with `self`
StdString std_format_f64(const StdFormat* restrict self, StdFormatSpecifier specifier);
/// @brief implementation of `StdFormat.format` for pointers
///
/// @param self - The `StdFormat` object to format
/// @param specifier - The format specifier to follow
///
/// @return a formatted string associated with `self`
StdString std_format_ptr(const StdFormat* restrict self, StdFormatSpecifier specifier);
/// @brief implementation of `StdFormat.format` for `StdString`
///
/// @param self - The `StdFormat` object to format
/// @param specifier - The format specifier to follow
///
/// @return a formatted string associated with `self`
StdString std_format_std_string(const StdFormat* restrict self, StdFormatSpecifier specifier);
/// @brief implementation of `StdFormat.format_with_allocator` for the builtin `cstring`
///
/// @param self - The `StdFormat` object to format
/// @param specifier - The format specifier to follow
/// @param allocator - The `StdAllocator` to allocate the format string with
///
/// @return a formatted string associated with `self`
StdString std_format_cstring_with_allocator(const StdFormat* restrict self,
											StdFormatSpecifier specifier,
											StdAllocator allocator);
/// @brief implementation of `StdFormat.format_with_allocator` for the builtin `bool`
///
/// @param self - The `StdFormat` object to format
/// @param specifier - The format specifier to follow
/// @param allocator - The `StdAllocator` to allocate the format string with
///
/// @return a formatted string associated with `self`
StdString std_format_bool_with_allocator(const StdFormat* restrict self,
										 StdFormatSpecifier specifier,
										 StdAllocator allocator);
/// @brief implementation of `StdFormat.format_with_allocator` for the builtin `char`
///
/// @param self - The `StdFormat` object to format
/// @param specifier - The format specifier to follow
/// @param allocator - The `StdAllocator` to allocate the format string with
///
/// @return a formatted string associated with `self`
StdString std_format_char_with_allocator(const StdFormat* restrict self,
										 StdFormatSpecifier specifier,
										 StdAllocator allocator);
/// @brief implementation of `StdFormat.format_with_allocator` for the builtin `u8`
///
/// @param self - The `StdFormat` object to format
/// @param specifier - The format specifier to follow
/// @param allocator - The `StdAllocator` to allocate the format string with
///
/// @return a formatted string associated with `self`
StdString std_format_u8_with_allocator(const StdFormat* restrict self,
									   StdFormatSpecifier specifier,
									   StdAllocator allocator);
/// @brief implementation of `StdFormat.format_with_allocator` for the builtin `u16`
///
/// @param self - The `StdFormat` object to format
/// @param specifier - The format specifier to follow
/// @param allocator - The `StdAllocator` to allocate the format string with
///
/// @return a formatted string associated with `self`
StdString std_format_u16_with_allocator(const StdFormat* restrict self,
										StdFormatSpecifier specifier,
										StdAllocator allocator);
/// @brief implementation of `StdFormat.format_with_allocator` for the builtin `u32`
///
/// @param self - The `StdFormat` object to format
/// @param specifier - The format specifier to follow
/// @param allocator - The `StdAllocator` to allocate the format string with
///
/// @return a formatted string associated with `self`
StdString std_format_u32_with_allocator(const StdFormat* restrict self,
										StdFormatSpecifier specifier,
										StdAllocator allocator);
/// @brief implementation of `StdFormat.format_with_allocator` for the builtin `u64`
///
/// @param self - The `StdFormat` object to format
/// @param specifier - The format specifier to follow
/// @param allocator - The `StdAllocator` to allocate the format string with
///
/// @return a formatted string associated with `self`
StdString std_format_u64_with_allocator(const StdFormat* restrict self,
										StdFormatSpecifier specifier,
										StdAllocator allocator);
/// @brief implementation of `StdFormat.format_with_allocator` for the builtin `i8`
///
/// @param self - The `StdFormat` object to format
/// @param specifier - The format specifier to follow
/// @param allocator - The `StdAllocator` to allocate the format string with
///
/// @return a formatted string associated with `self`
StdString std_format_i8_with_allocator(const StdFormat* restrict self,
									   StdFormatSpecifier specifier,
									   StdAllocator allocator);
/// @brief implementation of `StdFormat.format_with_allocator` for the builtin `i16`
///
/// @param self - The `StdFormat` object to format
/// @param specifier - The format specifier to follow
/// @param allocator - The `StdAllocator` to allocate the format string with
///
/// @return a formatted string associated with `self`
StdString std_format_i16_with_allocator(const StdFormat* restrict self,
										StdFormatSpecifier specifier,
										StdAllocator allocator);
/// @brief implementation of `StdFormat.format_with_allocator` for the builtin `i32`
///
/// @param self - The `StdFormat` object to format
/// @param specifier - The format specifier to follow
/// @param allocator - The `StdAllocator` to allocate the format string with
///
/// @return a formatted string associated with `self`
StdString std_format_i32_with_allocator(const StdFormat* restrict self,
										StdFormatSpecifier specifier,
										StdAllocator allocator);
/// @brief implementation of `StdFormat.format_with_allocator` for the builtin `i64`
///
/// @param self - The `StdFormat` object to format
/// @param specifier - The format specifier to follow
/// @param allocator - The `StdAllocator` to allocate the format string with
///
/// @return a formatted string associated with `self`
StdString std_format_i64_with_allocator(const StdFormat* restrict self,
										StdFormatSpecifier specifier,
										StdAllocator allocator);
/// @brief implementation of `StdFormat.format_with_allocator` for the builtin `f32`
///
/// @param self - The `StdFormat` object to format
/// @param specifier - The format specifier to follow
/// @param allocator - The `StdAllocator` to allocate the format string with
///
/// @return a formatted string associated with `self`
StdString std_format_f32_with_allocator(const StdFormat* restrict self,
										StdFormatSpecifier specifier,
										StdAllocator allocator);
/// @brief implementation of `StdFormat.format_with_allocator` for the builtin `f64`
///
/// @param self - The `StdFormat` object to format
/// @param specifier - The format specifier to follow
/// @param allocator - The `StdAllocator` to allocate the format string with
///
/// @return a formatted string associated with `self`
StdString std_format_f64_with_allocator(const StdFormat* restrict self,
										StdFormatSpecifier specifier,
										StdAllocator allocator);
/// @brief implementation of `StdFormat.format_with_allocator` for pointers
///
/// @param self - The `StdFormat` object to format
/// @param specifier - The format specifier to follow
/// @param allocator - The `StdAllocator` to allocate the format string with
///
/// @return a formatted string associated with `self`
StdString std_format_ptr_with_allocator(const StdFormat* restrict self,
										StdFormatSpecifier specifier,
										StdAllocator allocator);
/// @brief implementation of `StdFormat.format_with_allocator` for `StdString`
///
/// @param self - The `StdFormat` object to format
/// @param specifier - The format specifier to follow
/// @param allocator - The `StdAllocator` to allocate the format string with
///
/// @return a formatted string associated with `self`
StdString std_format_std_string_with_allocator(const StdFormat* restrict self,
											   StdFormatSpecifier specifier,
											   StdAllocator allocator);

/// @brief implementation of `StdFormat` for the builtin `cstring`
/// @return The `StdFormat` implementation
static maybe_unused
	ImplTraitFor(StdFormat, cstring, std_format_cstring, std_format_cstring_with_allocator);
/// @brief implementation of `StdFormat` for the builtin `bool`
/// @return The `StdFormat` implementation
static maybe_unused ImplTraitFor(StdFormat, bool, std_format_bool, std_format_bool_with_allocator);
/// @brief implementation of `StdFormat` for the builtin `char`
/// @return The `StdFormat` implementation
static maybe_unused ImplTraitFor(StdFormat, char, std_format_char, std_format_char_with_allocator);
/// @brief implementation of `StdFormat` for the builtin `u8`
/// @return The `StdFormat` implementation
static maybe_unused ImplTraitFor(StdFormat, u8, std_format_u8, std_format_u8_with_allocator);
/// @brief implementation of `StdFormat` for the builtin `u16`
/// @return The `StdFormat` implementation
static maybe_unused ImplTraitFor(StdFormat, u16, std_format_u16, std_format_u16_with_allocator);
/// @brief implementation of `StdFormat` for the builtin `u32`
/// @return The `StdFormat` implementation
static maybe_unused ImplTraitFor(StdFormat, u32, std_format_u32, std_format_u32_with_allocator);
/// @brief implementation of `StdFormat` for the builtin `u64`
/// @return The `StdFormat` implementation
static maybe_unused ImplTraitFor(StdFormat, u64, std_format_u64, std_format_u64_with_allocator);
/// @brief implementation of `StdFormat` for the builtin `i8`
/// @return The `StdFormat` implementation
static maybe_unused ImplTraitFor(StdFormat, i8, std_format_i8, std_format_i8_with_allocator);
/// @brief implementation of `StdFormat` for the builtin `i16`
/// @return The `StdFormat` implementation
static maybe_unused ImplTraitFor(StdFormat, i16, std_format_i16, std_format_i16_with_allocator);
/// @brief implementation of `StdFormat` for the builtin `i32`
/// @return The `StdFormat` implementation
static maybe_unused ImplTraitFor(StdFormat, i32, std_format_i32, std_format_i32_with_allocator);
/// @brief implementation of `StdFormat` for the builtin `i64`
/// @return The `StdFormat` implementation
static maybe_unused ImplTraitFor(StdFormat, i64, std_format_i64, std_format_i64_with_allocator);
/// @brief implementation of `StdFormat` for the builtin `f32`
/// @return The `StdFormat` implementation
static maybe_unused ImplTraitFor(StdFormat, f32, std_format_f32, std_format_f32_with_allocator);
/// @brief implementation of `StdFormat` for the builtin `f64`
/// @return The `StdFormat` implementation
static maybe_unused ImplTraitFor(StdFormat, f64, std_format_f64, std_format_f64_with_allocator);
/// @brief implementation of `StdFormat` for pointers
/// @return The `StdFormat` implementation
static maybe_unused
	ImplTraitFor(StdFormat, nullptr_t, std_format_ptr, std_format_ptr_with_allocator);
/// @brief implementation of `StdFormat` for `StdString`
/// @return The `StdFormat` implementation
static maybe_unused
	ImplTraitFor(StdFormat, StdString, std_format_std_string, std_format_std_string_with_allocator);
#endif
