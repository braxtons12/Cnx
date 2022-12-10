/// @file Format.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief CnxFormat brings human readable string formatting, similar to C++'s `std::format` and
/// `fmtlib`, and Rust's std::format, to C.
/// @version 0.3.0
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

#include <Cnx/Assert.h>
#include <Cnx/BasicTypes.h>
#include <Cnx/String.h>
#include <Cnx/Trait.h>
#include <stdarg.h>

/// @defgroup format String Formatting
/// Cnx's formatting API creates a composable, ergonomic, human-readable way to format data into
/// strings for storage or I/O.
///
/// The interface and functionality is similar to C++'s `std::format`, and the popular `fmtlib` it's
/// derived from, and Rust's `std::format`.
/// Example:
/// @code {.c}
/// let x = 0.0F;
/// let y = 1.0F;
/// // formats x and y as normal decimal format with one significant figure after the decimal point
/// let formatted = cnx_format("x: {d1}, y: {d1}", x, y);
/// @endcode
/// Formatting supports custom specifiers within the brackets, "{}". Any character except
/// curly brackets ('{' and '}') are valid to use for format specifies for custom types.
///
/// Builtin types provide some limited formatting options. For integral types, such as `char`,
/// `u32`, or pointers, these include options for notation: 'd' (Decimal, ie Base 10), 'x'
/// (Lower-case Hex), or 'X' (Upper-case Hex). For floating point numbers, such as `f32`, this
/// include options for notation: 'd' (Decimal) or 'e' (Scientific/Exponential notation), and for
/// the number of significant figures after the decimal point: a number directly following the
/// notation specifier, or on its own if no specifier was given. The default for integral types
/// other than pointer is decimal, for pointers is lower-case hexadecimal, and for pointers is
/// scientific notation with 3 significant figures after the decimal point. In addition to these
/// specifiers, builtin types also accept a "Debug" specifier `D`, and `D` should be used as the
/// standard specifier to indicate a debugging format. All Cnx library types providing a `CnxFormat`
/// implementation (such as `CnxDuration` or `CnxString`) support the `D` specifier and provide a
/// debugging format.
///
/// Bools are a special case among builtin types, in that they only support the `D` specifier,
/// even though they are technically integral types. Bools with always format to directly to "true"
/// or "false".
///
/// String formatting is extensible and composable in Cnx because it uses a Trait, `CnxFormat`, to
/// enable user-defined types to provide their own formatting implementations. This trait provides
/// the functionality for validating format specifiers and performing string formatting for an
/// implementing type.
///
/// To provide an implementation of `CnxFormat` for your type, only three functions and the Trait
/// implementation are required. The functions take the following signatures:
///
/// @code {.c}
/// CnxFormatContext (*const is_specifier_valid)(const CnxFormat* restrict self,
///                                                   CnxStringView specifier);
/// CnxString (*const format)(const CnxFormat* restrict self, CnxFormatContext context);
/// CnxString (*const format_with_allocator)(const CnxFormat* restrict self,
/// 											  CnxFormatContext context,
/// 											  CnxAllocator allocator);
/// @endcode
///
/// `is_specifier_valid` takes the specifier in the format string, should validate it, and return
/// the result in a `CnxFormatContext`. A `CnxFormatContext` stores the validation result as well as
/// up to 32 bytes of aligned storage to store any interim state parsed from the format specifier
/// necessary for formatting your type. For example, implementations for Cnx types store whether the
/// debug specifier `D` occurred and other state, such as the number of significant figures to use,
/// in the case of floating point types. This context will later be passed to your formatting
/// function in order to format the instance of your type.
///
/// `format` and `format_with_allocator` use your type (pointed to in the trait object, `self`) and
/// the `CnxFormatContext` (that was obtained when Cnx called your `is_specifier_valid` function)
/// to perform the string formatting. This will likely be implemented in terms of another call to
/// `cnx_format`, formatting the constituent parts of your type. In addition, `format` is generally
/// implemented in terms of calling `format_with_allocator` with the default Cnx allocator.
///
/// Arguments passed to `cnx_format` must be l-values cast to their `CnxFormat` trait object
/// representation with `as_format_t(T, value)` or `as_trait(CnxFormat, T, value)`. Cnx will
/// automatically cast builtin types, cstrings, string-literals, and `CnxString` and `CnxStringView`
/// to their `CnxFormat` implementation. Generally, other types will need to be cast explicitly,
/// however, automatic conversions for **specific** user defined types can be enabled by defining a
/// pair of macros prior to including "CnxFormat.h".
/// First, define @ref CNX_AS_FORMAT_USES_USER_SUPPLIED_TYPES to `TRUE`.
/// Then, define @ref CNX_AS_FORMAT_USER_SUPPLIED_TYPES to a comma separated list of types
/// and their conversions. For example:
///
/// @code {.c}
/// #define CNX_AS_FORMAT_USES_USER_SUPPLIED_TYPES TRUE
/// #define CNX_AS_FORMAT_USER_SUPPLIED_TYPES T* 		: as_format_t(T, x),	\
/// 										  const T* 	: as_format_t(T2, x), 	\
/// 										  T2* 		: as_format_t(T2, x), 	\
/// 										  const T2* : as_format_t(T2, x),
/// @endcode
///
/// where `T` and `T2` are your supplied types. `x` should always be the second argument to the
/// conversion function (literally always give `x` as the second argument).
/// `cnx_format(format_string, ...)` automatically applies the `as_format(x)` `_Generic` macro to
/// all format arguments in the parameter pack to perform the automatic conversion to `CnxFormat`.
/// That is why this syntax is necessary for the @ref CNX_AS_FORMAT_USER_SUPPLIED types macro,
/// to provide valid match arms in the `as_format(x)` macro for that conversion.
///
/// A complete example of implementing and using `CnxFormat`:
///
/// @code {.c}
/// typedef struct Point2D {
/// 	f32 x;
/// 	f32 y;
/// } Point2D;
///
/// #define CNX_AS_FORMAT_USES_USER_SUPPLIED_TYPES TRUE
/// #define CNX_AS_FORMAT_USER_SUPPLIED_TYPES Point2D* 			: as_format_t(Point2D, x), 	\
/// 										  const Point2D* 	: as_format_t(Point2D, x),
///
/// #include <Cnx/Format.h>
///
/// // `specifier` will be a string view over the characters making up the format specifier.
/// // For example, in the specifier `{e}`, `specifier` would view `e`.
/// // For this example, we'll make `d` (for decimal/normal notation) or `e`
/// // (for scientific/exponential notation) followed by a number (ie `4` or `10`, for the number of
/// // significant figures) a valid specifier, and anything else invalid.
/// // This is the same set of options available to floating point numbers, so we can just forward
/// // our implementation to that one.
/// inline static CnxFormatContext point2d_is_format_specifier_valid(const CnxFormat* restrict self,
///                                                                  CnxStringView specifier)
/// {
///     let an_f32 = static_cast(f32)(0.0);
///     let format_obj = as_format_t(f32, an_f32);
///     return trait_call(is_specifier_valid, format_obj, specifier);
/// }
///
/// typedef struct Point2DFormatContext {
///     bool is_exponential;
///     bool is_debug;
///     u32 num_sig_figs;
/// } Point2DFormatContext;
///
/// inline static CnxString point2d_format_with_allocator(const CnxFormat* restrict self,
/// 													  CnxFormatContext context,
/// 													  CnxAllocator allocator) {
///     // first we create our interim format string to use as the specifier for formatting
///     // x and y
///     let context = static_cast(const Point2DFormatContext*)(context.data);
///     let exponential = context->is_exponential ? "e" : "";
///     let debug = context->is_debug ? "D" : "";
///     let num_sig_figs = context->num_sig_figs;
///     CnxScopedString specifier = cnx_format_with_allocator("{}{}{}",
///                                                           allocator,
///                                                           exponential,
///                                                           num_sig_figs,
///                                                           debug);
///     // Then we can forward two copies of the specifier to format to create our format string,
///     // which we'll then pass to our actual format call, letting the implementation for f32
///     // handle the actual details
///     CnxScopedString format_str = cnx_format_with_allocator("Point2D: [x: \{{}\}, y: \{{}\}]",
///                                                            allocator,
///                                                            specifier,
///                                                            specifier);
/// 	let _self = static_cast(const Point2D*)(self.m_self);
/// 	return cnx_format_with_allocator(cnx_string_into_cstring(format_str),
/// 	                                 allocator,
/// 	                                 _self->x,
/// 	                                 _self->y);
/// }
///
/// inline static CnxString point2d_format(const CnxFormat* restrict self,
/// 									   CnxStringView specifier) {
///		return point2d_format_with_allocator(self, specifier, cnx_allocator_new());
/// }
///
/// __attr(maybe_unused) static
/// ImplTraitFor(CnxFormat, Point2D, point2d_format, point2d_format_with_allocator);
///
/// void point2d_print_legacy(const Point2D* restrict self) {
/// 	let formatted = cnx_format("{}", *self);
///		printf("%s", cnx_string_into_cstring(formatted));
/// }
/// @endcode

#ifndef CNX_FORMAT
	/// @brief Declarations and Definitions related to `cnx_format(format_string, ...)`,
	/// `CnxFormat`, and Cnx string formatting
	#define CNX_FORMAT

/// @brief Specifies possible errors that could occur when parsing a format specifier
/// @ingroup format
typedef enum CnxFormatErrorTypes {
	/// @brief No error, the specifier is valid
	/// @ingroup format
	CNX_FORMAT_SUCCESS = 0,
	/// @brief An invalid character occurred in the specifier sequence
	/// @ingroup format
	CNX_FORMAT_BAD_SPECIFIER_INVALID_CHAR_IN_SPECIFIER,
	/// @brief A closing specifier brace occurred in an invalid location
	/// @ingroup format
	CNX_FORMAT_INVALID_CLOSING_BRACE_LOCATION,
	/// @brief An unclosed format specifier (ie `"{e3"`) was given
	/// @ingroup format
	CNX_FORMAT_UNCLOSED_SPECIFIER,
	/// @brief More format specifiers were given in the format string than there were arguments
	/// to be formatted
	/// @ingroup format
	CNX_FORMAT_MORE_SPECIFIERS_THAN_ARGS,
	/// @brief Fewer format specifiers were given in the format string than there were arguments
	/// to be formatted
	/// @ingroup format
	CNX_FORMAT_FEWER_SPECIFIERS_THAN_ARGS
} CnxFormatErrorTypes;

/// @brief `CnxFormatContext` is the context type used to store parsed format specifier state for
/// passing to the formatting functions `CnxFormat.format` and `CnxFormat.format_with_allocator`.
typedef struct CnxFormatContext {
	_Alignas(32) byte state[static_cast(usize)(32)]; // NOLINT
	CnxFormatErrorTypes is_valid;
} CnxFormatContext;

/// @brief Default values for various formatting parameters for Cnx string formatting
/// @ingroup format
typedef enum CnxFormatDefaults {
	/// @brief The default number of significant figures for floating point formatting
	///
	/// By default, Cnx floating point formatting provides 3 significant figures after the
	/// decimal point in formatted output
	/// @ingroup format
	CNX_FORMAT_DEFAULT_NUM_SIG_FIGS = 3
} CnxFormatDefaults;

/// @brief Formats the various parameter pack arguments into their associated place in the given
/// format string, using the provided allocator
///
/// @param format_string - The string specifying the format positions, specifiers, and other text
/// 				       that should be present in the output string
/// @param allocator - The `CnxAllocator` to allocate the output string with
/// @param num_args - The number of arguments in the parameter pack
/// @param ... - The parameter pack of arguments to be formatted
///
/// @return The formatted output string
__attr(nodiscard) __attr(not_null(1)) CnxString
	cnx_format_with_allocator(restrict const_cstring format_string,
							  CnxAllocator allocator,
							  usize num_args,
							  ...)
		cnx_disable_if(!format_string, "Can't format arguments with a null format_string");

/// @brief Formats the various parameter pack arguments into their associated place in the given
/// format string, using the provided allocator
///
/// @param format_string - The string specifying the format positions, specifiers, and other text
/// 				       that should be present in the output string
/// @param allocator - The `CnxAllocator` to allocate the output string with
/// @param num_args - The number of arguments in the parameter pack
/// @param list - The parameter pack of arguments to be formatted
///
/// @return The formatted output string
__attr(nodiscard) __attr(not_null(1)) CnxString
	cnx_vformat_with_allocator(restrict const_cstring format_string,
							   CnxAllocator allocator,
							   usize num_args,
							   va_list list)
		cnx_disable_if(!format_string, "Can't format arguments with a null format_string");

	/// @brief Converts the given variable into its associated `CnxFormat` Trait implementation
	///
	/// There must be an implementation of `CnxFormat` for the type `T` and `x` must be an lvalue
	/// of type `T`.
	///
	/// @param T - The concrete type of `x`
	/// @param x - The variable to convert to its `CnxFormat` Trait implementation
	///
	/// @return `x` as `CnxFormat`
	/// @ingroup format
	#define as_format_t(T, x) as_trait(CnxFormat, T, x)

	/// @def CNX_AS_FORMAT_USES_USER_SUPPLIED_TYPES
	/// @brief Feature enable macro to allow specific user-defined types to be automatically
	/// converted to their `CnxFormat` implementation when passed to
	/// `cnx_format(format_string, ...)` or a similar Cnx string formatting function
	///
	/// Requires that @ref CNX_AS_FORMAT_USER_SUPPLIED_TYPES is also defined to a comma separated
	/// list of conversions
	/// Example:
	/// @code {.c}
	/// #define CNX_AS_FORMAT_USES_USER_SUPPLIED_TYPES true
	/// #define CNX_AS_FORMAT_USER_SUPPLIED_TYPES T* 		: as_format_t(T, x),	\
	/// 										  const T* 	: as_format_t(T2, x), 	\
	/// 										  T2* 		: as_format_t(T2, x), 	\
	/// 										  const T2* : as_format_t(T2, x),
	/// @endcode
	/// @ingroup format
	#ifndef CNX_AS_FORMAT_USES_USER_SUPPLIED_TYPES
		#define CNX_AS_FORMAT_USES_USER_SUPPLIED_TYPES 0
	#endif

	/// @def CNX_AS_FORMAT_USER_SUPPLIED_TYPES
	/// @brief Define this macro to a comma separated list of conversions to enable specific
	/// user-defined types to be automatically converted to their `CnxFormat` implementation
	/// when passed to `cnx_format(format_string, ...)` or a similar Cnx string formatting
	/// function
	///
	/// Requires that @ref CNX_AS_FORMAT_USES_USER_SUPPLIED_TYPES is also defined to `true`
	/// Example:
	/// @code {.c}
	/// #define CNX_AS_FORMAT_USES_USER_SUPPLIED_TYPES true
	/// #define CNX_AS_FORMAT_USER_SUPPLIED_TYPES T* 		: as_format_t(T, x),	\
	/// 										  const T* 	: as_format_t(T2, x), 	\
	/// 										  T2* 		: as_format_t(T2, x), 	\
	/// 										  const T2* : as_format_t(T2, x),
	/// @endcode
	/// @ingroup format
	#ifndef CNX_AS_FORMAT_USER_SUPPLIED_TYPES
		#define CNX_AS_FORMAT_USER_SUPPLIED_TYPES
	#endif

	#if CNX_PLATFORM_APPLE
		#if CNX_AS_FORMAT_USES_USER_SUPPLIED_TYPES
		// clang-format off
		/// @brief Converts the given variable into its associated `CnxFormat` Trait implementation
		///
		/// There must be an implementation of `CnxFormat` for the type of `x` and `x` must be an
		/// lvalue
		///
		/// @param x - The variable to convert to its `CnxFormat` Trait implementation
		///
		/// @return `x` as `CnxFormat`
		/// @ingroup format
		#define as_format(x) _Generic((&(x)), 											\
				char (*)[sizeof((x))]  		:   as_format_t(cstring, x), 				\
				char** 						: 	as_format_t(cstring, 					\
													**static_cast(const char**)(&(x))), \
				bool* 						: 	as_format_t(bool, x), 					\
				char* 						: 	as_format_t(char, x), 					\
				u8* 						: 	as_format_t(u8, x), 					\
				u16* 						: 	as_format_t(u16, x), 					\
				u32* 						: 	as_format_t(u32, x), 					\
				u64* 						: 	as_format_t(u64, x), 					\
				usize* 						: 	as_format_t(u64, x), 					\
				i8* 						: 	as_format_t(i8, x), 					\
				i16* 						: 	as_format_t(i16, x), 					\
				i32* 						: 	as_format_t(i32, x), 					\
				i64* 						: 	as_format_t(i64, x), 					\
				isize* 						: 	as_format_t(i64, x), 					\
				f32* 						: 	as_format_t(f32, x), 					\
				f64* 						: 	as_format_t(f64, x), 					\
				nullptr_t* 					: 	as_format_t(nullptr_t, x), 				\
				CnxString* 					: 	as_format_t(CnxString, x), 				\
				CnxStringView* 				: 	as_format_t(CnxStringView, x), 			\
				CnxFormat* 					: 	(x), 									\
				const char (*)[sizeof((x))] :   as_format_t(cstring, x), 				\
				const char** 				: 	as_format_t(cstring, 					\
													**static_cast(const char**)(&(x))), \
				const bool* 				: 	as_format_t(bool, x), 					\
				const char* 				: 	as_format_t(char, x), 					\
				const u8* 					: 	as_format_t(u8, x), 					\
				const u16* 					: 	as_format_t(u16, x), 					\
				const u32* 					: 	as_format_t(u32, x), 					\
				const u64* 					: 	as_format_t(u64, x), 					\
				const usize* 				: 	as_format_t(u64, x), 					\
				const i8* 					: 	as_format_t(i8, x), 					\
				const i16* 					: 	as_format_t(i16, x), 					\
				const i32* 					: 	as_format_t(i32, x), 					\
				const i64* 					: 	as_format_t(i64, x), 					\
				const isize* 				: 	as_format_t(i64, x), 					\
				const f32* 					: 	as_format_t(f32, x), 					\
				const f64* 					: 	as_format_t(f64, x), 					\
				const nullptr_t*			: 	as_format_t(nullptr_t, x), 				\
				const CnxString*			: 	as_format_t(CnxString, x), 				\
				const CnxStringView*		: 	as_format_t(CnxStringView, x), 			\
				CNX_AS_FORMAT_USER_SUPPLIED_TYPES 										\
				const CnxFormat* 			: 	(x))
	#else
		/// @brief Converts the given variable into its associated `CnxFormat` Trait implementation
		///
		/// There must be an implementation of `CnxFormat` for the type of `x` and `x` must be an
		/// lvalue
		///
		/// @param x - The variable to convert to its `CnxFormat` Trait implementation
		///
		/// @return `x` as `CnxFormat`
		/// @ingroup format
		#define as_format(x) _Generic((&(x)), 							    			\
				char (*)[sizeof((x))]  		:   as_format_t(cstring, x), 				\
				char** 						: 	as_format_t(cstring, 					\
													**static_cast(const char**)(&(x))), \
				bool* 						: 	as_format_t(bool, x), 					\
				char* 						: 	as_format_t(char, x), 					\
				u8* 						: 	as_format_t(u8, x), 					\
				u16* 						: 	as_format_t(u16, x), 					\
				u32* 						: 	as_format_t(u32, x), 					\
				u64* 						: 	as_format_t(u64, x), 					\
				usize* 						: 	as_format_t(u64, x), 					\
				i8* 						: 	as_format_t(i8, x), 					\
				i16* 						: 	as_format_t(i16, x), 					\
				i32* 						: 	as_format_t(i32, x), 					\
				i64* 						: 	as_format_t(i64, x), 					\
				isize* 						: 	as_format_t(i64, x), 					\
				f32* 						: 	as_format_t(f32, x), 					\
				f64* 						: 	as_format_t(f64, x), 					\
				nullptr_t* 					: 	as_format_t(nullptr_t, x), 				\
				CnxString* 					: 	as_format_t(CnxString, x), 				\
				CnxStringView* 				: 	as_format_t(CnxStringView, x), 			\
				CnxFormat* 					: 	(x), 									\
				const char (*)[sizeof((x))] :   as_format_t(cstring, x), 				\
				const char** 				: 	as_format_t(cstring, 					\
													**static_cast(const char**)(&(x))), \
				const bool* 				: 	as_format_t(bool, x), 					\
				const char* 				: 	as_format_t(char, x), 					\
				const u8* 					: 	as_format_t(u8, x), 					\
				const u16* 					: 	as_format_t(u16, x), 					\
				const u32* 					: 	as_format_t(u32, x), 					\
				const u64* 					: 	as_format_t(u64, x), 					\
				const usize* 				: 	as_format_t(u64, x), 					\
				const i8* 					: 	as_format_t(i8, x), 					\
				const i16* 					: 	as_format_t(i16, x), 					\
				const i32* 					: 	as_format_t(i32, x), 					\
				const i64* 					: 	as_format_t(i64, x), 					\
				const isize* 				: 	as_format_t(i64, x), 					\
				const f32* 					: 	as_format_t(f32, x), 					\
				const f64* 					: 	as_format_t(f64, x), 					\
				const nullptr_t*			: 	as_format_t(nullptr_t, x), 				\
				const CnxString*			: 	as_format_t(CnxString, x), 				\
				const CnxStringView*		: 	as_format_t(CnxStringView, x), 			\
				const CnxFormat*			: 	(x))
		// clang-format on
		#endif // CNX_AS_FORMAT_USES_USER_SUPPLIED_TYPES
	#else
		#if CNX_AS_FORMAT_USES_USER_SUPPLIED_TYPES
		// clang-format off
		/// @brief Converts the given variable into its associated `CnxFormat` Trait implementation
		///
		/// There must be an implementation of `CnxFormat` for the type of `x` and `x` must be an
		/// lvalue
		///
		/// @param x - The variable to convert to its `CnxFormat` Trait implementation
		///
		/// @return `x` as `CnxFormat`
		/// @ingroup format
		#define as_format(x) _Generic((&(x)), 											\
				char (*)[sizeof((x))]  		:   as_format_t(cstring, x), 				\
				char** 						: 	as_format_t(cstring, 					\
													**static_cast(const char**)(&(x))), \
				bool* 						: 	as_format_t(bool, x), 					\
				char* 						: 	as_format_t(char, x), 					\
				u8* 						: 	as_format_t(u8, x), 					\
				u16* 						: 	as_format_t(u16, x), 					\
				u32* 						: 	as_format_t(u32, x), 					\
				u64* 						: 	as_format_t(u64, x), 					\
				i8* 						: 	as_format_t(i8, x), 					\
				i16* 						: 	as_format_t(i16, x), 					\
				i32* 						: 	as_format_t(i32, x), 					\
				i64* 						: 	as_format_t(i64, x), 					\
				f32* 						: 	as_format_t(f32, x), 					\
				f64* 						: 	as_format_t(f64, x), 					\
				nullptr_t* 					: 	as_format_t(nullptr_t, x), 				\
				CnxString* 					: 	as_format_t(CnxString, x), 				\
				CnxStringView* 				: 	as_format_t(CnxStringView, x), 		    \
				CnxFormat* 					: 	(x), 									\
				const char (*)[sizeof((x))] :   as_format_t(cstring, x), 				\
				const bool* 				: 	as_format_t(bool, x), 					\
				const char** 				: 	as_format_t(cstring, 					\
													**static_cast(const char**)(&(x))), \
				const bool* 				: 	as_format_t(bool, x), 					\
				const char* 				: 	as_format_t(char, x), 					\
				const u8* 					: 	as_format_t(u8, x), 					\
				const u16* 					: 	as_format_t(u16, x), 					\
				const u32* 					: 	as_format_t(u32, x), 					\
				const u64* 					: 	as_format_t(u64, x), 					\
				const i8* 					: 	as_format_t(i8, x), 					\
				const i16* 					: 	as_format_t(i16, x), 					\
				const i32* 					: 	as_format_t(i32, x), 					\
				const i64* 					: 	as_format_t(i64, x), 					\
				const f32* 					: 	as_format_t(f32, x), 					\
				const f64* 					: 	as_format_t(f64, x), 					\
				const nullptr_t*			: 	as_format_t(nullptr_t, x), 				\
				const CnxString*			: 	as_format_t(CnxString, x), 				\
				const CnxStringView*		: 	as_format_t(CnxStringView, x), 		    \
				CNX_AS_FORMAT_USER_SUPPLIED_TYPES 										\
				const CnxFormat* 			: 	(x))
	#else
		/// @brief Converts the given variable into its associated `CnxFormat` Trait implementation
		///
		/// There must be an implementation of `CnxFormat` for the type of `x` and `x` must be an
		/// lvalue
		///
		/// @param x - The variable to convert to its `CnxFormat` Trait implementation
		///
		/// @return `x` as `CnxFormat`
		/// @ingroup format
		#define as_format(x) _Generic((&(x)), 							    			\
				char (*)[sizeof((x))]  		:   as_format_t(cstring, x), 				\
				char** 						: 	as_format_t(cstring, 					\
													**static_cast(const char**)(&(x))), \
				bool* 						: 	as_format_t(bool, x), 					\
				char* 						: 	as_format_t(char, x), 					\
				u8* 						: 	as_format_t(u8, x), 					\
				u16* 						: 	as_format_t(u16, x), 					\
				u32* 						: 	as_format_t(u32, x), 					\
				u64* 						: 	as_format_t(u64, x), 					\
				i8* 						: 	as_format_t(i8, x), 					\
				i16* 						: 	as_format_t(i16, x), 					\
				i32* 						: 	as_format_t(i32, x), 					\
				i64* 						: 	as_format_t(i64, x), 					\
				f32* 						: 	as_format_t(f32, x), 					\
				f64* 						: 	as_format_t(f64, x), 					\
				nullptr_t* 					: 	as_format_t(nullptr_t, x), 				\
				CnxString* 					: 	as_format_t(CnxString, x), 				\
				CnxStringView* 				: 	as_format_t(CnxStringView, x), 		    \
				CnxFormat* 					: 	(x), 									\
				const char (*)[sizeof((x))] :   as_format_t(cstring, x), 				\
				const char** 				: 	as_format_t(cstring, 					\
													**static_cast(const char**)(&(x))), \
				const bool* 				: 	as_format_t(bool, x), 					\
				const char* 				: 	as_format_t(char, x), 					\
				const u8* 					: 	as_format_t(u8, x), 					\
				const u16* 					: 	as_format_t(u16, x), 					\
				const u32* 					: 	as_format_t(u32, x), 					\
				const u64* 					: 	as_format_t(u64, x), 					\
				const i8* 					: 	as_format_t(i8, x), 					\
				const i16* 					: 	as_format_t(i16, x), 					\
				const i32* 					: 	as_format_t(i32, x), 					\
				const i64* 					: 	as_format_t(i64, x), 					\
				const f32* 					: 	as_format_t(f32, x), 					\
				const f64* 					: 	as_format_t(f64, x), 					\
				const nullptr_t*			: 	as_format_t(nullptr_t, x), 				\
				const CnxString*			: 	as_format_t(CnxString, x), 				\
				const CnxStringView*		: 	as_format_t(CnxStringView, x), 		    \
				const CnxFormat*			: 	(x))
		// clang-format on
		#endif // CNX_AS_FORMAT_USES_USER_SUPPLIED_TYPES
	#endif

	/// @brief Formats the various parameter pack arguments into their associated place in the given
	/// format string, using the provided allocator
	///
	/// @param format_string - The string specifying the format positions, specifiers, and other
	/// 					   text that should be present in the output string
	/// @param allocator - The `CnxAllocator` to allocate the output string with
	/// @param ... - The parameter pack of arguments to be formatted
	///
	/// @return The formatted output string
	/// @ingroup format
	#define cnx_format_with_allocator(format_string, allocator, ...) \
		(cnx_format_with_allocator)(format_string,                   \
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
	#define cnx_format(format_string, ...) \
		cnx_format_with_allocator(format_string, cnx_allocator_new() __VA_OPT__(, ) __VA_ARGS__)
	/// @brief Formats the various `va_list` parameter pack arguments into their associated place in
	/// the given format string, using the provided allocator
	///
	/// @param format_string - The string specifying the format positions, specifiers, and other
	/// 					   text that should be present in the output string
	/// @param allocator - The `CnxAllocator` to allocate the output string with
	/// @param num_args - The number of arguments in the parameter pack
	/// @param list - The `va_list` parameter pack of arguments to be formatted
	///
	/// @return The formatted output string
	/// @ingroup format
	#define cnx_vformat_with_allocator(format_string, allocator, num_args, list) \
		(cnx_vformat_with_allocator)(format_string, allocator, num_args, list)
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
	#define cnx_vformat(format_string, num_args, list) \
		cnx_vformat_with_allocator(format_string, cnx_allocator_new(), num_args, list)

// clang-format off

/// @typedef CnxFormat
/// @brief `CnxFormat` is the Trait which allows extensible and composable string formatting
/// of builtin and user-defined types.
///
/// `CnxFormat` requires an implementation of `is_specifer_valid` to validate and parse the format
/// specifier corresponding with your type in a call to `cnx_format`, `format`, to format your type
/// using the default system allocator for allocations, and `format_with_allocator`, to format your
/// type using a user-provided allocator for allocations.
///
/// To provide an implementation of `CnxFormat` for your type, only three functions and the
/// Trait implementation are required. The functions take the following signatures:
///
/// @code {.c}
/// CnxFormatContext (*const your_is_specifier_valid)(const CnxFormat* restrict self,
///                                                   CnxStringView specifier);
/// CnxString (*const your_format)(const CnxFormat* restrict self, CnxFormatContext context);
/// CnxString (*const your_format_with_allocator)(const CnxFormat* restrict self,
/// 											  CnxFormatContext context,
/// 											  CnxAllocator allocator);
/// @endcode
///
/// And providing the Trait implementation is as simple as:
///
/// @code {.c}
/// ImplTraitFor(CnxFormat,
///              your_type,
///              your_is_specifier_valid,
///              your_format,
///              your_format_with_allocator);
/// @endcode
///
/// In practice, you will probably be providing this Trait implementation in a header file,
/// so you'll also probably want to mark it as `static` and `__attr(maybe_unused)
///
/// @code {.c}
/// __attr(maybe_unused) static ImplTraitFor(CnxFormat,
///                                          your_type,
///                                          your_is_specifier_valid,
///                                          your_format,
///                                          your_format_with_allocator);
/// @endcode
///
/// @ingroup format
Trait(
/// @struct CnxFormat
/// @brief `CnxFormat` is the Trait which allows extensible and composable string formatting
/// of builtin and user-defined types.
///
/// `CnxFormat` requires an implementation of `is_specifer_valid` to validate and parse the format
/// specifier corresponding with your type in a call to `cnx_format`, `format`, to format your type
/// using the default system allocator for allocations, and `format_with_allocator`, to format your
/// type using a user-provided allocator for allocations.
///
/// To provide an implementation of `CnxFormat` for your type, only three functions and the
/// Trait implementation are required. The functions take the following signatures:
///
/// @code {.c}
/// CnxFormatContext (*const your_is_specifier_valid)(const CnxFormat* restrict self,
///                                                   CnxStringView specifier);
/// CnxString (*const your_format)(const CnxFormat* restrict self, CnxFormatContext context);
/// CnxString (*const your_format_with_allocator)(const CnxFormat* restrict self,
/// 											  CnxFormatContext context,
/// 											  CnxAllocator allocator);
/// @endcode
///
/// And providing the Trait implementation is as simple as:
///
/// @code {.c}
/// ImplTraitFor(CnxFormat,
///              your_type,
///              your_is_specifier_valid,
///              your_format,
///              your_format_with_allocator);
/// @endcode
///
/// In practice, you will probably be providing this Trait implementation in a header file,
/// so you'll also probably want to mark it as `static` and `__attr(maybe_unused)
///
/// @code {.c}
/// __attr(maybe_unused) static ImplTraitFor(CnxFormat,
///                                          your_type,
///                                          your_is_specifier_valid,
///                                          your_format,
///                                          your_format_with_allocator);
/// @endcode
///
/// @ingroup format
	CnxFormat,
    /** @brief Returns whether the given format specifier is valid for the underlying type of    **/
    /** this `CnxFormat` Trait object                                                            **/
    /** 																						 **/
    /** @param self - The `CnxFormat` Trait object to format 									 **/
    /** @param specifier - The `CnxStringView` indicating what format method to use 		     **/
    /** 																						 **/
    /** @return whether `specifier` is valid 									                 **/
    CnxFormatContext (*const is_specifier_valid)(const CnxFormat* restrict self,
                                                 CnxStringView specifier);
    /** @brief formats the given `CnxFormat` Trait object according to the given specifier 		 **/
    /** 																						 **/
    /** @param self - The `CnxFormat` Trait object to format 									 **/
    /** @param context - The `CnxFormatContext` indicating what format method to use 		     **/
    /** 																						 **/
    /** @return the formatted string associated with `self` 									 **/
    CnxString (*const format)(const CnxFormat* restrict self, CnxFormatContext context);
    /** @brief formats the given `CnxFormat` Trait object according to the given specifier 	 	 **/
    /** 																						 **/
    /** @param self - The `CnxFormat` Trait object to format 									 **/
    /** @param specifier - The `CnxFormatContext` indicating what format method to use 		     **/
    /** @param allocator - The `CnxAllocator` to allocate the formatted string with 			 **/
    /** 																						 **/
    /** @return the formatted string associated with `self` 									 **/
    CnxString(*const format_with_allocator)(const CnxFormat* restrict self,
										    CnxFormatContext context,
										    CnxAllocator allocator););
// clang-format on

	#define ___DISABLE_IF_NULL(self) \
		cnx_disable_if(!(self), "The data being formatted can't be a nullptr")

/// @brief implementation of `CnxFormat.is_specifier_valid` for the builtin `cstring`
///
/// @param self - The `CnxFormat` Trait object to format
/// @param specifier - The format specifier to follow
///
/// @return whether `specifier` is valid
__attr(nodiscard) __attr(not_null(1)) CnxFormatContext
	cnx_format_is_specifier_valid_cstring(const CnxFormat* restrict self, CnxStringView specifier)
		___DISABLE_IF_NULL(self);
/// @brief implementation of `CnxFormat.is_specifier_valid` for the builtin `bool`
///
/// @param self - The `CnxFormat` Trait object to format
/// @param specifier - The format specifier to follow
///
/// @return whether `specifier` is valid
__attr(nodiscard) __attr(not_null(1)) CnxFormatContext
	cnx_format_is_specifier_valid_bool(const CnxFormat* restrict self, CnxStringView specifier)
		___DISABLE_IF_NULL(self);
/// @brief implementation of `CnxFormat.is_specifier_valid` for the builtin `char`
///
/// @param self - The `CnxFormat` Trait object to format
/// @param specifier - The format specifier to follow
///
/// @return whether `specifier` is valid
__attr(nodiscard) __attr(not_null(1)) CnxFormatContext
	cnx_format_is_specifier_valid_char(const CnxFormat* restrict self, CnxStringView specifier)
		___DISABLE_IF_NULL(self);
/// @brief implementation of `CnxFormat.is_specifier_valid` for the builtin `u8`
///
/// @param self - The `CnxFormat` Trait object to format
/// @param specifier - The format specifier to follow
///
/// @return whether `specifier` is valid
__attr(nodiscard) __attr(not_null(1)) CnxFormatContext
	cnx_format_is_specifier_valid_u8(const CnxFormat* restrict self, CnxStringView specifier)
		___DISABLE_IF_NULL(self);
/// @brief implementation of `CnxFormat.is_specifier_valid` for the builtin `u16`
///
/// @param self - The `CnxFormat` Trait object to format
/// @param specifier - The format specifier to follow
///
/// @return whether `specifier` is valid
__attr(nodiscard) __attr(not_null(1)) CnxFormatContext
	cnx_format_is_specifier_valid_u16(const CnxFormat* restrict self, CnxStringView specifier)
		___DISABLE_IF_NULL(self);
/// @brief implementation of `CnxFormat.is_specifier_valid` for the builtin `u32`
///
/// @param self - The `CnxFormat` Trait object to format
/// @param specifier - The format specifier to follow
///
/// @return whether `specifier` is valid
__attr(nodiscard) __attr(not_null(1)) CnxFormatContext
	cnx_format_is_specifier_valid_u32(const CnxFormat* restrict self, CnxStringView specifier)
		___DISABLE_IF_NULL(self);
/// @brief implementation of `CnxFormat.is_specifier_valid` for the builtin `u64`
///
/// @param self - The `CnxFormat` Trait object to format
/// @param specifier - The format specifier to follow
///
/// @return whether `specifier` is valid
__attr(nodiscard) __attr(not_null(1)) CnxFormatContext
	cnx_format_is_specifier_valid_u64(const CnxFormat* restrict self, CnxStringView specifier)
		___DISABLE_IF_NULL(self);
/// @brief implementation of `CnxFormat.is_specifier_valid` for the builtin `i8`
///
/// @param self - The `CnxFormat` Trait object to format
/// @param specifier - The format specifier to follow
///
/// @return whether `specifier` is valid
__attr(nodiscard) __attr(not_null(1)) CnxFormatContext
	cnx_format_is_specifier_valid_i8(const CnxFormat* restrict self, CnxStringView specifier)
		___DISABLE_IF_NULL(self);
/// @brief implementation of `CnxFormat.is_specifier_valid` for the builtin `i16`
///
/// @param self - The `CnxFormat` Trait object to format
/// @param specifier - The format specifier to follow
///
/// @return whether `specifier` is valid
__attr(nodiscard) __attr(not_null(1)) CnxFormatContext
	cnx_format_is_specifier_valid_i16(const CnxFormat* restrict self, CnxStringView specifier)
		___DISABLE_IF_NULL(self);
/// @brief implementation of `CnxFormat.is_specifier_valid` for the builtin `i32`
///
/// @param self - The `CnxFormat` Trait object to format
/// @param specifier - The format specifier to follow
///
/// @return whether `specifier` is valid
__attr(nodiscard) __attr(not_null(1)) CnxFormatContext
	cnx_format_is_specifier_valid_i32(const CnxFormat* restrict self, CnxStringView specifier)
		___DISABLE_IF_NULL(self);
/// @brief implementation of `CnxFormat.is_specifier_valid` for the builtin `i64`
///
/// @param self - The `CnxFormat` Trait object to format
/// @param specifier - The format specifier to follow
///
/// @return whether `specifier` is valid
__attr(nodiscard) __attr(not_null(1)) CnxFormatContext
	cnx_format_is_specifier_valid_i64(const CnxFormat* restrict self, CnxStringView specifier)
		___DISABLE_IF_NULL(self);
/// @brief implementation of `CnxFormat.is_specifier_valid` for the builtin `f32`
///
/// @param self - The `CnxFormat` Trait object to format
/// @param specifier - The format specifier to follow
///
/// @return whether `specifier` is valid
__attr(nodiscard) __attr(not_null(1)) CnxFormatContext
	cnx_format_is_specifier_valid_f32(const CnxFormat* restrict self, CnxStringView specifier)
		___DISABLE_IF_NULL(self);

/// @brief implementation of `CnxFormat.is_specifier_valid` for the builtin `f64`
///
/// @param self - The `CnxFormat` Trait object to format
/// @param specifier - The format specifier to follow
///
/// @return whether `specifier` is valid
__attr(nodiscard) __attr(not_null(1)) CnxFormatContext
	cnx_format_is_specifier_valid_f64(const CnxFormat* restrict self, CnxStringView specifier)
		___DISABLE_IF_NULL(self);
/// @brief implementation of `CnxFormat.is_specifier_valid` for pointers
///
/// @param self - The `CnxFormat` Trait object to format
/// @param specifier - The format specifier to follow
///
/// @return whether `specifier` is valid
__attr(nodiscard) __attr(not_null(1)) CnxFormatContext
	cnx_format_is_specifier_valid_ptr(const CnxFormat* restrict self, CnxStringView specifier)
		___DISABLE_IF_NULL(self);
/// @brief implementation of `CnxFormat.is_specifier_valid` for `CnxString`
///
/// @param self - The `CnxFormat` Trait object to format
/// @param specifier - The format specifier to follow
///
/// @return whether `specifier` is valid
__attr(nodiscard) __attr(not_null(1)) CnxFormatContext
	cnx_format_is_specifier_valid_cnx_string(const CnxFormat* restrict self,
											 CnxStringView specifier) ___DISABLE_IF_NULL(self);
/// @brief implementation of `CnxFormat.is_specifier_valid` for `CnxStringView`
///
/// @param self - The `CnxFormat` Trait object to format
/// @param specifier - The format specifier to follow
///
/// @return whether `specifier` is valid
__attr(nodiscard) __attr(not_null(1)) CnxFormatContext
	cnx_format_is_specifier_valid_cnx_stringview(const CnxFormat* restrict self,
												 CnxStringView specifier) ___DISABLE_IF_NULL(self);

/// @brief implementation of `CnxFormat.format` for the builtin `cstring`
///
/// @param self - The `CnxFormat` Trait object to format
/// @param context - The format specifier to follow
///
/// @return a formatted string associated with `self`
__attr(nodiscard) __attr(not_null(1)) CnxString
	cnx_format_cstring(const CnxFormat* restrict self, CnxFormatContext context)
		___DISABLE_IF_NULL(self);
/// @brief implementation of `CnxFormat.format` for the builtin `bool`
///
/// @param self - The `CnxFormat` Trait object to format
/// @param context - The format specifier to follow
///
/// @return a formatted string associated with `self`
__attr(nodiscard) __attr(not_null(1)) CnxString
	cnx_format_bool(const CnxFormat* restrict self, CnxFormatContext context)
		___DISABLE_IF_NULL(self);
/// @brief implementation of `CnxFormat.format` for the builtin `char`
///
/// @param self - The `CnxFormat` Trait object to format
/// @param context - The format specifier to follow
///
/// @return a formatted string associated with `self`
__attr(nodiscard) __attr(not_null(1)) CnxString
	cnx_format_char(const CnxFormat* restrict self, CnxFormatContext context)
		___DISABLE_IF_NULL(self);
/// @brief implementation of `CnxFormat.format` for the builtin `u8`
///
/// @param self - The `CnxFormat` Trait object to format
/// @param context - The format specifier to follow
///
/// @return a formatted string associated with `self`
__attr(nodiscard) __attr(not_null(1)) CnxString
	cnx_format_u8(const CnxFormat* restrict self, CnxFormatContext context)
		___DISABLE_IF_NULL(self);
/// @brief implementation of `CnxFormat.format` for the builtin `u16`
///
/// @param self - The `CnxFormat` Trait object to format
/// @param context - The format specifier to follow
///
/// @return a formatted string associated with `self`
__attr(nodiscard) __attr(not_null(1)) CnxString
	cnx_format_u16(const CnxFormat* restrict self, CnxFormatContext context)
		___DISABLE_IF_NULL(self);
/// @brief implementation of `CnxFormat.format` for the builtin `u32`
///
/// @param self - The `CnxFormat` Trait object to format
/// @param context - The format specifier to follow
///
/// @return a formatted string associated with `self`
__attr(nodiscard) __attr(not_null(1)) CnxString
	cnx_format_u32(const CnxFormat* restrict self, CnxFormatContext context)
		___DISABLE_IF_NULL(self);
/// @brief implementation of `CnxFormat.format` for the builtin `u64`
///
/// @param self - The `CnxFormat` Trait object to format
/// @param context - The format specifier to follow
///
/// @return a formatted string associated with `self`
__attr(nodiscard) __attr(not_null(1)) CnxString
	cnx_format_u64(const CnxFormat* restrict self, CnxFormatContext context)
		___DISABLE_IF_NULL(self);
/// @brief implementation of `CnxFormat.format` for the builtin `i8`
///
/// @param self - The `CnxFormat` Trait object to format
/// @param context - The format specifier to follow
///
/// @return a formatted string associated with `self`
__attr(nodiscard) __attr(not_null(1)) CnxString
	cnx_format_i8(const CnxFormat* restrict self, CnxFormatContext context)
		___DISABLE_IF_NULL(self);
/// @brief implementation of `CnxFormat.format` for the builtin `i16`
///
/// @param self - The `CnxFormat` Trait object to format
/// @param context - The format specifier to follow
///
/// @return a formatted string associated with `self`
__attr(nodiscard) __attr(not_null(1)) CnxString
	cnx_format_i16(const CnxFormat* restrict self, CnxFormatContext context)
		___DISABLE_IF_NULL(self);
/// @brief implementation of `CnxFormat.format` for the builtin `i32`
///
/// @param self - The `CnxFormat` Trait object to format
/// @param context - The format specifier to follow
///
/// @return a formatted string associated with `self`
__attr(nodiscard) __attr(not_null(1)) CnxString
	cnx_format_i32(const CnxFormat* restrict self, CnxFormatContext context)
		___DISABLE_IF_NULL(self);
/// @brief implementation of `CnxFormat.format` for the builtin `i64`
///
/// @param self - The `CnxFormat` Trait object to format
/// @param context - The format specifier to follow
///
/// @return a formatted string associated with `self`
__attr(nodiscard) __attr(not_null(1)) CnxString
	cnx_format_i64(const CnxFormat* restrict self, CnxFormatContext context)
		___DISABLE_IF_NULL(self);
/// @brief implementation of `CnxFormat.format` for the builtin `f32`
///
/// @param self - The `CnxFormat` Trait object to format
/// @param context - The format specifier to follow
///
/// @return a formatted string associated with `self`
__attr(nodiscard) __attr(not_null(1)) CnxString
	cnx_format_f32(const CnxFormat* restrict self, CnxFormatContext context)
		___DISABLE_IF_NULL(self);
/// @brief implementation of `CnxFormat.format` for the builtin `f64`
///
/// @param self - The `CnxFormat` Trait object to format
/// @param context - The format specifier to follow
///
/// @return a formatted string associated with `self`
__attr(nodiscard) __attr(not_null(1)) CnxString
	cnx_format_f64(const CnxFormat* restrict self, CnxFormatContext context)
		___DISABLE_IF_NULL(self);
/// @brief implementation of `CnxFormat.format` for pointers
///
/// @param self - The `CnxFormat` Trait object to format
/// @param context - The format specifier to follow
///
/// @return a formatted string associated with `self`
__attr(nodiscard) __attr(not_null(1)) CnxString
	cnx_format_ptr(const CnxFormat* restrict self, CnxFormatContext context)
		___DISABLE_IF_NULL(self);
/// @brief implementation of `CnxFormat.format` for `CnxString`
///
/// @param self - The `CnxFormat` Trait object to format
/// @param context - The format specifier to follow
///
/// @return a formatted string associated with `self`
__attr(nodiscard) __attr(not_null(1)) CnxString
	cnx_format_cnx_string(const CnxFormat* restrict self, CnxFormatContext context)
		___DISABLE_IF_NULL(self);
/// @brief implementation of `CnxFormat.format` for `CnxStringView`
///
/// @param self - The `CnxFormat` Trait object to format
/// @param context - The format specifier to follow
///
/// @return a formatted string associated with `self`
__attr(nodiscard) __attr(not_null(1)) CnxString
	cnx_format_cnx_stringview(const CnxFormat* restrict self, CnxFormatContext context)
		___DISABLE_IF_NULL(self);
/// @brief implementation of `CnxFormat.format_with_allocator` for the builtin `cstring`
///
/// @param self - The `CnxFormat` Trait object to format
/// @param context - The format specifier to follow
/// @param allocator - The `CnxAllocator` to allocate the format string with
///
/// @return a formatted string associated with `self`
__attr(nodiscard) __attr(not_null(1)) CnxString
	cnx_format_cstring_with_allocator(const CnxFormat* restrict self,
									  CnxFormatContext context,
									  CnxAllocator allocator) ___DISABLE_IF_NULL(self);
/// @brief implementation of `CnxFormat.format_with_allocator` for the builtin `bool`
///
/// @param self - The `CnxFormat` Trait object to format
/// @param context - The format specifier to follow
/// @param allocator - The `CnxAllocator` to allocate the format string with
///
/// @return a formatted string associated with `self`
__attr(nodiscard) __attr(not_null(1)) CnxString
	cnx_format_bool_with_allocator(const CnxFormat* restrict self,
								   CnxFormatContext context,
								   CnxAllocator allocator) ___DISABLE_IF_NULL(self);
/// @brief implementation of `CnxFormat.format_with_allocator` for the builtin `char`
///
/// @param self - The `CnxFormat` Trait object to format
/// @param context - The format specifier to follow
/// @param allocator - The `CnxAllocator` to allocate the format string with
///
/// @return a formatted string associated with `self`
__attr(nodiscard) __attr(not_null(1)) CnxString
	cnx_format_char_with_allocator(const CnxFormat* restrict self,
								   CnxFormatContext context,
								   CnxAllocator allocator) ___DISABLE_IF_NULL(self);
/// @brief implementation of `CnxFormat.format_with_allocator` for the builtin `u8`
///
/// @param self - The `CnxFormat` Trait object to format
/// @param context - The format specifier to follow
/// @param allocator - The `CnxAllocator` to allocate the format string with
///
/// @return a formatted string associated with `self`
__attr(nodiscard) __attr(not_null(1)) CnxString
	cnx_format_u8_with_allocator(const CnxFormat* restrict self,
								 CnxFormatContext context,
								 CnxAllocator allocator) ___DISABLE_IF_NULL(self);
/// @brief implementation of `CnxFormat.format_with_allocator` for the builtin `u16`
///
/// @param self - The `CnxFormat` Trait object to format
/// @param context - The format specifier to follow
/// @param allocator - The `CnxAllocator` to allocate the format string with
///
/// @return a formatted string associated with `self`
__attr(nodiscard) __attr(not_null(1)) CnxString
	cnx_format_u16_with_allocator(const CnxFormat* restrict self,
								  CnxFormatContext context,
								  CnxAllocator allocator) ___DISABLE_IF_NULL(self);
/// @brief implementation of `CnxFormat.format_with_allocator` for the builtin `u32`
///
/// @param self - The `CnxFormat` Trait object to format
/// @param context - The format specifier to follow
/// @param allocator - The `CnxAllocator` to allocate the format string with
///
/// @return a formatted string associated with `self`
__attr(nodiscard) __attr(not_null(1)) CnxString
	cnx_format_u32_with_allocator(const CnxFormat* restrict self,
								  CnxFormatContext context,
								  CnxAllocator allocator) ___DISABLE_IF_NULL(self);
/// @brief implementation of `CnxFormat.format_with_allocator` for the builtin `u64`
///
/// @param self - The `CnxFormat` Trait object to format
/// @param context - The format specifier to follow
/// @param allocator - The `CnxAllocator` to allocate the format string with
///
/// @return a formatted string associated with `self`
__attr(nodiscard) __attr(not_null(1)) CnxString
	cnx_format_u64_with_allocator(const CnxFormat* restrict self,
								  CnxFormatContext context,
								  CnxAllocator allocator) ___DISABLE_IF_NULL(self);
/// @brief implementation of `CnxFormat.format_with_allocator` for the builtin `i8`
///
/// @param self - The `CnxFormat` Trait object to format
/// @param context - The format specifier to follow
/// @param allocator - The `CnxAllocator` to allocate the format string with
///
/// @return a formatted string associated with `self`
__attr(nodiscard) __attr(not_null(1)) CnxString
	cnx_format_i8_with_allocator(const CnxFormat* restrict self,
								 CnxFormatContext context,
								 CnxAllocator allocator) ___DISABLE_IF_NULL(self);
/// @brief implementation of `CnxFormat.format_with_allocator` for the builtin `i16`
///
/// @param self - The `CnxFormat` Trait object to format
/// @param context - The format specifier to follow
/// @param allocator - The `CnxAllocator` to allocate the format string with
///
/// @return a formatted string associated with `self`
__attr(nodiscard) __attr(not_null(1)) CnxString
	cnx_format_i16_with_allocator(const CnxFormat* restrict self,
								  CnxFormatContext context,
								  CnxAllocator allocator) ___DISABLE_IF_NULL(self);
/// @brief implementation of `CnxFormat.format_with_allocator` for the builtin `i32`
///
/// @param self - The `CnxFormat` Trait object to format
/// @param context - The format specifier to follow
/// @param allocator - The `CnxAllocator` to allocate the format string with
///
/// @return a formatted string associated with `self`
__attr(nodiscard) __attr(not_null(1)) CnxString
	cnx_format_i32_with_allocator(const CnxFormat* restrict self,
								  CnxFormatContext context,
								  CnxAllocator allocator) ___DISABLE_IF_NULL(self);
/// @brief implementation of `CnxFormat.format_with_allocator` for the builtin `i64`
///
/// @param self - The `CnxFormat` Trait object to format
/// @param context - The format specifier to follow
/// @param allocator - The `CnxAllocator` to allocate the format string with
///
/// @return a formatted string associated with `self`
__attr(nodiscard) __attr(not_null(1)) CnxString
	cnx_format_i64_with_allocator(const CnxFormat* restrict self,
								  CnxFormatContext context,
								  CnxAllocator allocator) ___DISABLE_IF_NULL(self);
/// @brief implementation of `CnxFormat.format_with_allocator` for the builtin `f32`
///
/// @param self - The `CnxFormat` Trait object to format
/// @param context - The format specifier to follow
/// @param allocator - The `CnxAllocator` to allocate the format string with
///
/// @return a formatted string associated with `self`
__attr(nodiscard) __attr(not_null(1)) CnxString
	cnx_format_f32_with_allocator(const CnxFormat* restrict self,
								  CnxFormatContext context,
								  CnxAllocator allocator) ___DISABLE_IF_NULL(self);
/// @brief implementation of `CnxFormat.format_with_allocator` for the builtin `f64`
///
/// @param self - The `CnxFormat` Trait object to format
/// @param context - The format specifier to follow
/// @param allocator - The `CnxAllocator` to allocate the format string with
///
/// @return a formatted string associated with `self`
__attr(nodiscard) __attr(not_null(1)) CnxString
	cnx_format_f64_with_allocator(const CnxFormat* restrict self,
								  CnxFormatContext context,
								  CnxAllocator allocator) ___DISABLE_IF_NULL(self);
/// @brief implementation of `CnxFormat.format_with_allocator` for pointers
///
/// @param self - The `CnxFormat` Trait object to format
/// @param context - The format specifier to follow
/// @param allocator - The `CnxAllocator` to allocate the format string with
///
/// @return a formatted string associated with `self`
__attr(nodiscard) __attr(not_null(1)) CnxString
	cnx_format_ptr_with_allocator(const CnxFormat* restrict self,
								  CnxFormatContext context,
								  CnxAllocator allocator) ___DISABLE_IF_NULL(self);
/// @brief implementation of `CnxFormat.format_with_allocator` for `CnxString`
///
/// @param self - The `CnxFormat` Trait object to format
/// @param context - The format specifier to follow
/// @param allocator - The `CnxAllocator` to allocate the format string with
///
/// @return a formatted string associated with `self`
__attr(nodiscard) __attr(not_null(1)) CnxString
	cnx_format_cnx_string_with_allocator(const CnxFormat* restrict self,
										 CnxFormatContext context,
										 CnxAllocator allocator) ___DISABLE_IF_NULL(self);
/// @brief implementation of `CnxFormat.format_with_allocator` for `CnxStringView`
///
/// @param self - The `CnxFormat` Trait object to format
/// @param context - The format specifier to follow
/// @param allocator - The `CnxAllocator` to allocate the format string with
///
/// @return a formatted string associated with `self`
__attr(nodiscard) __attr(not_null(1)) CnxString
	cnx_format_cnx_stringview_with_allocator(const CnxFormat* restrict self,
											 CnxFormatContext context,
											 CnxAllocator allocator) ___DISABLE_IF_NULL(self);

/// @brief implementation of `CnxFormat` for the builtin `cstring`
/// @return The `CnxFormat` implementation
__attr(maybe_unused) static ImplTraitFor(CnxFormat,
										 cstring,
										 cnx_format_is_specifier_valid_cstring,
										 cnx_format_cstring,
										 cnx_format_cstring_with_allocator);
/// @brief implementation of `CnxFormat` for the builtin `bool`
/// @return The `CnxFormat` implementation
__attr(maybe_unused) static ImplTraitFor(CnxFormat,
										 bool,
										 cnx_format_is_specifier_valid_bool,
										 cnx_format_bool,
										 cnx_format_bool_with_allocator);
/// @brief implementation of `CnxFormat` for the builtin `char`
/// @return The `CnxFormat` implementation
__attr(maybe_unused) static ImplTraitFor(CnxFormat,
										 char,
										 cnx_format_is_specifier_valid_char,
										 cnx_format_char,
										 cnx_format_char_with_allocator);
/// @brief implementation of `CnxFormat` for the builtin `u8`
/// @return The `CnxFormat` implementation
__attr(maybe_unused) static ImplTraitFor(CnxFormat,
										 u8,
										 cnx_format_is_specifier_valid_u8,
										 cnx_format_u8,
										 cnx_format_u8_with_allocator);
/// @brief implementation of `CnxFormat` for the builtin `u16`
/// @return The `CnxFormat` implementation
__attr(maybe_unused) static ImplTraitFor(CnxFormat,
										 u16,
										 cnx_format_is_specifier_valid_u16,
										 cnx_format_u16,
										 cnx_format_u16_with_allocator);
/// @brief implementation of `CnxFormat` for the builtin `u32`
/// @return The `CnxFormat` implementation
__attr(maybe_unused) static ImplTraitFor(CnxFormat,
										 u32,
										 cnx_format_is_specifier_valid_u32,
										 cnx_format_u32,
										 cnx_format_u32_with_allocator);
/// @brief implementation of `CnxFormat` for the builtin `u64`
/// @return The `CnxFormat` implementation
__attr(maybe_unused) static ImplTraitFor(CnxFormat,
										 u64,
										 cnx_format_is_specifier_valid_u64,
										 cnx_format_u64,
										 cnx_format_u64_with_allocator);
/// @brief implementation of `CnxFormat` for the builtin `i8`
/// @return The `CnxFormat` implementation
__attr(maybe_unused) static ImplTraitFor(CnxFormat,
										 i8,
										 cnx_format_is_specifier_valid_i8,
										 cnx_format_i8,
										 cnx_format_i8_with_allocator);
/// @brief implementation of `CnxFormat` for the builtin `i16`
/// @return The `CnxFormat` implementation
__attr(maybe_unused) static ImplTraitFor(CnxFormat,
										 i16,
										 cnx_format_is_specifier_valid_i16,
										 cnx_format_i16,
										 cnx_format_i16_with_allocator);
/// @brief implementation of `CnxFormat` for the builtin `i32`
/// @return The `CnxFormat` implementation
__attr(maybe_unused) static ImplTraitFor(CnxFormat,
										 i32,
										 cnx_format_is_specifier_valid_i32,
										 cnx_format_i32,
										 cnx_format_i32_with_allocator);
/// @brief implementation of `CnxFormat` for the builtin `i64`
/// @return The `CnxFormat` implementation
__attr(maybe_unused) static ImplTraitFor(CnxFormat,
										 i64,
										 cnx_format_is_specifier_valid_i64,
										 cnx_format_i64,
										 cnx_format_i64_with_allocator);
/// @brief implementation of `CnxFormat` for the builtin `f32`
/// @return The `CnxFormat` implementation
__attr(maybe_unused) static ImplTraitFor(CnxFormat,
										 f32,
										 cnx_format_is_specifier_valid_f32,
										 cnx_format_f32,
										 cnx_format_f32_with_allocator);
/// @brief implementation of `CnxFormat` for the builtin `f64`
/// @return The `CnxFormat` implementation
__attr(maybe_unused) static ImplTraitFor(CnxFormat,
										 f64,
										 cnx_format_is_specifier_valid_f64,
										 cnx_format_f64,
										 cnx_format_f64_with_allocator);
/// @brief implementation of `CnxFormat` for pointers
/// @return The `CnxFormat` implementation
__attr(maybe_unused) static ImplTraitFor(CnxFormat,
										 nullptr_t,
										 cnx_format_is_specifier_valid_ptr,
										 cnx_format_ptr,
										 cnx_format_ptr_with_allocator);
/// @brief implementation of `CnxFormat` for `CnxString`
/// @return The `CnxFormat` implementation
__attr(maybe_unused) static ImplTraitFor(CnxFormat,
										 CnxString,
										 cnx_format_is_specifier_valid_cnx_string,
										 cnx_format_cnx_string,
										 cnx_format_cnx_string_with_allocator);
/// @brief implementation of `CnxFormat` for `CnxStringView`
/// @return The `CnxFormat` implementation
__attr(maybe_unused) static ImplTraitFor(CnxFormat,
										 CnxStringView,
										 cnx_format_is_specifier_valid_cnx_stringview,
										 cnx_format_cnx_stringview,
										 cnx_format_cnx_stringview_with_allocator);
	#undef ___DISABLE_IF_NULL
#endif
