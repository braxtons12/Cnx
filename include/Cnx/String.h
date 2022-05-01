/// @file String.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief This module provides string and stringview types comparable to C++ for Cnx
/// @version 0.2.2
/// @date 2022-04-30
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

/// @ingroup collections
/// @{
/// @defgroup cnx_string CnxString
/// `CnxString` is a bounds-safe string type that manages its own length and capacity, is fully
/// `cstring` (aka `char*`) compatible, provides Cnx compatible random-access iterators over its
/// contents, and is allocator aware. It aims to provide more modern string-handling capabilities to
/// C, with an API and functionality comparable to C++'s `std::string`. `CnxString` is always
/// null-terminated, so it's completely compatible with C's existing string functions, so long as
/// those functions do not modify the length of the string or insert null terminators (other mutable
/// access is acceptable).
///
/// `CnxString` provides a small size optimization (SSO). This means that for short strings, the
/// data associated with the string will be stored on the stack, up to a certain number of
/// characters, before resorting to heap allocation. This does not require `CnxString` to take up
/// extra stack space in order to implement, `sizeof(CnxString)` would be the same regardless of
/// this optimization. The optimization allows for up to `2 * sizeof(usize) + sizeof(void*) - 1`
/// bytes to be stored on the stack. on x86_64, this translates to 23 characters, excluding the null
/// terminator (24 including the null terminator).
///
/// Example:
/// @code {.c}
/// #include <string.h> // for strlen
/// #include <Cnx/String.h>
/// #include <Cnx/IO.h>
/// let_mut string = cnx_string_from("Cnx");
/// cnx_string_prepend(string, "Hello World ");
/// cnx_string_append(string, " style!");
///
/// // `CnxString is fully compatible with non-mutating standard C string functions,
/// // and partially compatible with mutating standard C string functions
/// // (length must not be modified and null terminators must not be inserted,
/// // outside of `CnxString`'s own APIs)
/// let len_c = strlen(cnx_string_into_cstring(string));
/// println("string is {} chars long (c-style)", len_c);
///
/// let len = cnx_string_length(string);
/// println("string is {} chars long (Cnx-style)", len);
///
/// // print `string` to `stdout`, at once, followed by a newline
/// println("{}", string);
///
/// // range-for loop enabled by Cnx compatible iterators
/// // print `string` to `stdout`, one character at a time, followed by a newline
/// foreach(character, string) {
///		print("{}", character);
/// }
/// print("\n");
/// @endcode
/// @}

/// @ingroup views
/// @{
/// @defgroup cnx_stringview CnxStringView
/// A `CnxStringView` is a "view" into a `CnxString`. It allows you to lazily inspect the contents
/// of its associated string, but not modify them. It is effectively a fat reference to the contents
/// of its associated string.
/// @}

#ifndef CNX_STRING
/// @brief CNX_STRING
#define CNX_STRING

#include <Cnx/Allocators.h>
#include <Cnx/BasicTypes.h>
#include <Cnx/Iterator.h>
#define OPTION_INCLUDE_DEFAULT_INSTANTIATIONS TRUE
#include <Cnx/Option.h>
#undef OPTION_INCLUDE_DEFAULT_INSTANTIATIONS
#include <wchar.h>

/// @brief The size of the long string representation of `CnxString`
#define CNX_STRING_SIZEOF_LONG_REP (static_cast(usize)(sizeof(usize) * 2U + sizeof(cstring)))
/// @brief The number of `char`s in a SSOd `CnxString`
#define CNX_STRING_SHORT_OPTIMIZATION_ARRAY_SIZE \
	(static_cast(usize)((CNX_STRING_SIZEOF_LONG_REP - 1U) / sizeof(char)))
/// @brief The number of `char`s in a SSOd `CnxString`
#define CNX_STRING_SHORT_OPTIMIZATION_CAPACITY CNX_STRING_SHORT_OPTIMIZATION_ARRAY_SIZE

/// @brief The function vector table of methods associated with `CnxStringView`
/// @ingroup cnx_stringview
typedef struct cnx_stringview_vtable_t cnx_stringview_vtable_t;

/// @brief Cnx stringview type.
/// A stringview is a "view" into a `CnxString`. It allows you to lazily inspect the contents of the
/// string, but not modify them. It is effectively a fat pointer into the contents of the associated
/// `CnxString`
/// @ingroup cnx_stringview
typedef struct CnxStringView {
	/// @brief The length of the string view
	usize m_length;
	/// @brief The view
	const_cstring m_view;
	/// @brief The function vector table of methods associated with `CnxStringView`
	const cnx_stringview_vtable_t* m_vtable;
} CnxStringView;

/// @brief The function vector table of methods associated with `CnxString`
/// @ingroup cnx_string
typedef struct cnx_string_vtable_t cnx_string_vtable_t;

/// @brief Cnx string type
/// `CnxString` is a bounds safe, allocator aware, potentially dynamically allocated string type
/// with significantly improved ergonomics over `const char*`s (aka `cstring`s), but maintains
/// compatibility with them
/// (`CnxString` is always null terminated).
/// Provides similar functionality to C++'s `std::string`, but in C
///
/// Example:
/// @code {.c}
/// #include <Cnx/String.h>
/// #include <Cnx/IO.h>
/// // create default allocator (malloc allocates, free frees)
/// let allocator = cnx_allocator_new();
/// // could also call cnx_string_from("This is a string") since we're using the default
/// // allocator
/// let_mut string = cnx_string_from_with_allocator("This is a string", allocator);
///	cnx_string_append(&string, " with some extra text");
///	cnx_string_prepend(&string, "Hello world!\n");
/// // prints to stdout:
/// // Hello world!
/// // This is a string with some extra text
/// println("{}", string);
/// @endcode
/// @ingroup cnx_string
typedef struct CnxString {
	union {
		struct {
			/// @brief the long length string
			cstring m_long;
			/// @brief The length of the string
			usize m_length;
			/// @brief The capacity of the string
			usize m_capacity;
		};
		struct {
			/// @brief The short-length optimized string
			char m_short[CNX_STRING_SHORT_OPTIMIZATION_ARRAY_SIZE + 1];
		};
	};
	/// @brief The allocator to use for memory allocation
	CnxAllocator m_allocator;
	/// @brief The function vector table of methods associated with `CnxString`
	const cnx_string_vtable_t* m_vtable;
} CnxString;

/// @brief Cnx string iterator storage type
/// `CnxStringIterator` is the underlying storage type used by `CnxString` for its
/// non-const iterator type (`CnxRandomAccessIterator(char_ptr)`)
/// @ingroup cnx_string
typedef struct CnxStringIterator {
	/// @brief The current index of the iterator
	isize m_index;
	/// @brief The `CnxString` this iterator iterates over
	CnxString* m_string;
} CnxStringIterator;

/// @brief Cnx string const iterator storage type
/// `CnxStringIterator` is the underlying storage type used by `CnxString` for its
/// const iterator type (`CnxRandomAccessIterator(const_char_ptr)`)
/// @ingroup cnx_string
typedef struct CnxStringConstIterator {
	/// @brief The current index of the iterator
	isize m_index;
	/// @brief The `CnxString` this iterator iterates over
	const CnxString* m_string;
} CnxStringConstIterator;

/// @brief Cnx stringview const iterator storage type
/// `CnxStringViewIterator` is the underlying storage type used by `CnxStringView` for its
/// const iterator type (`CnxRandomAccessIterator(const_char_ptr)`)
/// @ingroup cnx_stringview
typedef struct CnxStringViewIterator {
	/// @brief The current index of the iterator
	isize m_index;
	/// @brief The `CnxStringView` this iterator iterates over
	const CnxStringView* m_view;
} CnxStringViewIterator;

/// @brief type alias for a reference to `CnxString`
typedef CnxString* Ref(CnxString);
/// @brief type alias for a reference to `CnxStringView`
typedef CnxStringView* Ref(CnxStringView);
/// @brief type alias for a const reference to `CnxString`
typedef const CnxString* ConstRef(CnxString);
/// @brief type alias for a const reference to `CnxStringView`
typedef const CnxStringView* ConstRef(CnxStringView);

#define OPTION_DECL TRUE

/// @brief Declares `CnxOption(T)` for `CnxString`
#define OPTION_T CnxString
#include <Cnx/Option.h>
#undef OPTION_T

/// @brief Declares `CnxOption(T)` for `Ref(CnxString)`
#define OPTION_T Ref(CnxString)
#include <Cnx/Option.h>
#undef OPTION_T

/// @brief Declares `CnxOption(T)` for `ConstRef(CnxString)`
#define OPTION_T ConstRef(CnxString)
#include <Cnx/Option.h>
#undef OPTION_T

/// @brief Declares `CnxOption(T)` for `CnxStringView`
#define OPTION_T CnxStringView
#include <Cnx/Option.h>
#undef OPTION_T

/// @brief Declares `CnxOption(T)` for `Ref(CnxStringView)`
#define OPTION_T Ref(CnxStringView)
#include <Cnx/Option.h>
#undef OPTION_T

/// @brief Declares `CnxOption(T)` for `ConstRef(CnxStringView)`
#define OPTION_T ConstRef(CnxStringView)
#include <Cnx/Option.h>
#undef OPTION_T

#undef OPTION_DECL

/// @brief Declares iterator types for `Ref(CnxString)`
DeclCnxIterators(Ref(CnxString));
/// @brief Declares iterator types for `ConstRef(CnxString)`
DeclCnxIterators(ConstRef(CnxString));
/// @brief Declares iterator types for `Ref(CnxStringView)`
DeclCnxIterators(Ref(CnxStringView));
/// @brief Declares iterator types for `ConstRef(CnxStringView)`
DeclCnxIterators(ConstRef(CnxStringView));

/// @brief Creates a new, empty `CnxString`
///
/// @return an empty `CnxString`
/// @ingroup cnx_string
__attr(nodiscard) CnxString cnx_string_new(void);
/// @brief Creates a new, empty `CnxString` that will use the given allocator
///
/// @param allocator - The allocator to use for memory allocations
///
/// @return an empty `CnxString`
/// @ingroup cnx_string
__attr(nodiscard) CnxString cnx_string_new_with_allocator(CnxAllocator allocator);
/// @brief Creates a new `CnxString` with the given initial capacity
///
/// @param capacity - The initial capacity of the string
///
/// @return a `CnxString` with `capacity` initial capacity
/// @ingroup cnx_string
__attr(nodiscard) CnxString cnx_string_new_with_capacity(usize capacity);
/// @brief Creates a new `CnxString` with the given initial capacity
///
/// @param capacity - The initial capacity of the string
/// @param allocator - The allocator to use for memory allocations
///
/// @return a `CnxString` with `capacity` initial capacity
/// @ingroup cnx_string
__attr(nodiscard) CnxString
	cnx_string_new_with_capacity_with_allocator(usize capacity, CnxAllocator allocator);
/// @brief Creates a new `CnxString` from the given `cstring`
///
/// @param string - The `cstring` to create the `CnxString` from
/// @param length - The length of the `cstring`
///
/// @return a `CnxString`
/// @ingroup cnx_string
__attr(nodiscard) __attr(not_null(1)) CnxString
	cnx_string_from_cstring(restrict const_cstring string, usize length)
		cnx_disable_if(!string, "Can't create a CnxString from a nullptr");
/// @brief Creates a new `CnxString` from the given `cstring`
///
/// @param string - The `cstring` to create the `CnxString` from
/// @param length - The length of the `cstring`
/// @param allocator - The allocator to use for memory allocations
///
/// @return a `CnxString`
/// @ingroup cnx_string
__attr(nodiscard) __attr(not_null(1)) CnxString
	cnx_string_from_cstring_with_allocator(restrict const_cstring string,
										   usize length,
										   CnxAllocator allocator)
		cnx_disable_if(!string, "Can't create a CnxString from a nullptr");
/// @brief Creates a new `CnxString` from the given `wcstring`
///
/// @param string - The `wcstring` to create the `CnxString` from
/// @param length - The length of the `wcstring`
///
/// @return a `CnxString`
/// @ingroup cnx_string
__attr(nodiscard) __attr(not_null(1)) CnxString
	cnx_string_from_wcstring(restrict const_wcstring string, usize length)
		cnx_disable_if(!string, "Can't create a CnxString from a nullptr");
/// @brief Creates a new `CnxString` from the given `wcstring`
///
/// @param string - The `wcstring` to create the `CnxString` from
/// @param length - The length of the `wcstring`
/// @param allocator - The allocator to use for memory allocations
///
/// @return a `CnxString`
/// @ingroup cnx_string
__attr(nodiscard) __attr(not_null(1)) CnxString
	cnx_string_from_wcstring_with_allocator(restrict const_wcstring string,
											usize length,
											CnxAllocator allocator)
		cnx_disable_if(!string, "Can't create a CnxString from a nullptr");
/// @brief Creates a new `CnxString` from the given `CnxStringView`
///
/// @param view - The string view to create a `CnxString` from
///
/// @return a `CnxString`
/// @ingroup cnx_string
__attr(nodiscard) __attr(not_null(1)) CnxString
	cnx_string_from_stringview(const CnxStringView* restrict view)
		cnx_disable_if(!view, "Can't create a CnxString from a nullptr");
/// @brief Creates a new `CnxString` that will use the given memory allocator, from the given
/// `CnxStringView`
///
/// @param view - The `CnxStringView` to create the `CnxStringView` from
/// @param allocator - The allocator to use for memory allocations
///
/// @return a `CnxString`
/// @ingroup cnx_string
__attr(nodiscard) __attr(not_null(1)) CnxString
	cnx_string_from_stringview_with_allocator(const CnxStringView* restrict view,
											  CnxAllocator allocator)
		cnx_disable_if(!view, "Can't create a CnxString from a nullptr");
// clang-format off
/// @brief Creates a new `CnxString` from the given string-like type
///
/// @param string - The string-like thing to create the `CnxString` from. Either a
/// `CnxStringView` or a `cstring` (const or non-const)
///
/// @return a `CnxString`
/// @ingroup cnx_string
#define cnx_string_from(string) _Generic((string), 												   \
	const_cstring 				: 	cnx_string_from_cstring(static_cast(const_cstring)(string),    \
										strlen(static_cast(const_cstring)(string))), 			   \
	cstring 					: 	cnx_string_from_cstring(static_cast(cstring)(string), 		   \
										strlen(static_cast(cstring)(string))), 			    	   \
	const char[sizeof(string)]  : 	cnx_string_from_cstring( 						/** NOLINT **/ \
										static_cast(const_cstring)(string),  					   \
										sizeof(string)), 							/** NOLINT **/ \
	char[sizeof(string)] 		: 	cnx_string_from_cstring( 						/** NOLINT **/ \
										static_cast(cstring)(string), 							   \
										sizeof(string)), 			  				/** NOLINT **/ \
	const_wcstring 				: 	cnx_string_from_wcstring(static_cast(const_wcstring)(string),  \
										wcslen(static_cast(const_wcstring)(string))), 			   \
	wcstring 					: 	cnx_string_from_wcstring(static_cast(wcstring)(string), 	   \
										wcslen(static_cast(wcstring)(string))), 			       \
	const wchar_t[																				   \
		sizeof(string) / 																		   \
		sizeof(wchar_t)]  		: 	cnx_string_from_wcstring( 						/** NOLINT **/ \
										static_cast(const_wcstring)(string),  					   \
										sizeof(string) / sizeof(wchar_t)), 			/** NOLINT **/ \
	wchar_t[sizeof(string) / 																	   \
		sizeof(wchar_t)] 		: 	cnx_string_from_wcstring( 						/** NOLINT **/ \
										static_cast(wcstring)(string), 							   \
										sizeof(string) / sizeof(wchar_t)), 			/** NOLINT **/ \
	CnxStringView*              : 	cnx_string_from_stringview( 								   \
										static_cast(CnxStringView*)(string)), 					   \
	const CnxStringView* 		: 	cnx_string_from_stringview( 								   \
										static_cast(const CnxStringView*)(string)))
/// @brief Creates a new `CnxString` from the given string-like type, using the given
/// `CnxAllocator` for memory allocations
///
/// @param string - The string-like thing to create the `CnxString` from. Either a
/// `CnxStringView` or a `cstring` (const or non-const)
/// @param allocator - The allocator to use for memory allocations, if necessary
///
/// @return a `CnxString`
/// @ingroup cnx_string
#define cnx_string_from_with_allocator(string, allocator) _Generic((string),                       \
	const_cstring 				: 	cnx_string_from_cstring_with_allocator( 					   \
										static_cast(const_cstring)(string), 				   	   \
										strlen(static_cast(const_cstring)(string)), allocator),    \
	cstring 					: 	cnx_string_from_cstring_with_allocator( 					   \
										static_cast(cstring)(string), 				    		   \
										strlen(static_cast(cstring)(string)), allocator),      	   \
	const char[sizeof(string)] 	: 	cnx_string_from_cstring_with_allocator( 		/** NOLINT **/ \
										static_cast(const_cstring)(string), 				   	   \
		 								sizeof(string), allocator), 				/** NOLINT **/ \
	char[sizeof(string)] 		: 	cnx_string_from_cstring_with_allocator( 		/** NOLINT **/ \
										static_cast(cstring)(string),  							   \
										sizeof(string), allocator), 				/** NOLINT **/ \
	const_wcstring 				: 	cnx_string_from_wcstring_with_allocator( 					   \
										static_cast(const_wcstring)(string), 				   	   \
										wcslen(static_cast(const_wcstring)(string)), allocator),   \
	wcstring 					: 	cnx_string_from_wcstring_with_allocator( 					   \
										static_cast(wcstring)(string), 				    		   \
										wcslen(static_cast(wcstring)(string)), allocator),         \
	const wchar_t[ 																				   \
		sizeof(string) / 																		   \
		sizeof(wchar_t)] 		: 	cnx_string_from_wcstring_with_allocator( 		/** NOLINT **/ \
										static_cast(const_wcstring)(string), 				   	   \
		 								sizeof(string) / sizeof(wchar_t), 			/** NOLINT **/ \
										allocator), 											   \
	wchar_t[sizeof(string) / 																	   \
		sizeof(wchar_t)] 		: 	cnx_string_from_wcstring_with_allocator( 		/** NOLINT **/ \
										static_cast(wcstring)(string),  						   \
										sizeof(string) / sizeof(wchar_t), 			/** NOLINT **/ \
										allocator), 											   \
	CnxStringView* 				: 	cnx_string_from_stringview_with_allocator( 					   \
										static_cast(CnxStringView*)(string), allocator), 		   \
	const CnxStringView* 		: 	cnx_string_from_stringview_with_allocator( 					   \
										static_cast(const CnxStringView*)(string), allocator))
// clang-format on

#define ___DISABLE_IF_NULL(self) \
	cnx_disable_if(!(self), "Can't perform a CnxString operation on a nullptr")
/// @brief Returns the `cstring` representation of this `CnxString`
///
/// @param self - the string to get the `cstring` representation of
///
/// @return  the `cstring` representation
/// @ingroup cnx_string
__attr(nodiscard) __attr(not_null(1)) __attr(returns_not_null) const_cstring
	cnx_string_into_cstring(const CnxString* restrict self) ___DISABLE_IF_NULL(self);
/// @brief Returns the `wcstring` converted representation of this `CnxString`. The result will be
/// allocated with the same allocator used by this `CnxString`
///
/// @param self - The string to get the `wcstring` converted representation of
///
/// @return the `wcstring` converted representation
/// @ingroup cnx_string
__attr(nodiscard) __attr(not_null(1)) __attr(returns_not_null) const_wcstring
	cnx_string_into_wcstring(const CnxString* restrict self) ___DISABLE_IF_NULL(self);
/// @brief Returns the `wcstring` converted representation of this `CnxString`, allocated with the
/// given allocator;
///
/// @param self - The string to get the `wcstring` converted representation of
/// @param allocator - The allocator to allocate memory for the `wcstring` converted representation
///
/// @return the `wcstring` converted representation
/// @ingroup cnx_string
__attr(nodiscard) __attr(not_null(1)) __attr(returns_not_null) const_wcstring
	cnx_string_into_wcstring_with_allocator(const CnxString* restrict self, CnxAllocator allocator)
		___DISABLE_IF_NULL(self);
/// @brief Returns a `CnxStringView` into this `CnxString`
///
/// @param self - the string to get the `CnxStringView` of
///
/// @return  the `CnxStringview` into this
/// @ingroup cnx_string
__attr(nodiscard) __attr(not_null(1)) CnxStringView
	cnx_string_into_stringview(const CnxString* restrict self) ___DISABLE_IF_NULL(self);
/// @brief Creates a copy of this `CnxString` using the same allocator
///
/// @param self - The `CnxString` to copy
///
/// @return a copy of the `CnxString`
/// @ingroup cnx_string
__attr(nodiscard) __attr(not_null(1)) CnxString cnx_string_clone(const CnxString* restrict self)
	___DISABLE_IF_NULL(self);
/// @brief Creates a copy of this `CnxString` using the given allocator
///
/// @param self - The `CnxString` to copy
/// @param allocator - The allocator to allocate memory with
///
/// @return a copy of the `CnxString`
/// @ingroup cnx_string
__attr(nodiscard) __attr(not_null(1)) CnxString
	cnx_string_clone_with_allocator(const CnxString* restrict self, CnxAllocator allocator)
		___DISABLE_IF_NULL(self);
/// @brief Frees the allocated memory of the string, if it is not small string optimized
///
/// @param self - The `CnxString` to free
/// @ingroup cnx_string
__attr(not_null(1)) void cnx_string_free(void* restrict self) ___DISABLE_IF_NULL(self);
/// @brief declare a `CnxString` variable with this attribute to have `cnx_string_free`
/// automatically called on it at scope end
#define CnxScopedString scoped(cnx_string_free)

/// @brief Returns a pointer to the character at the given index.
///
/// @param self - The `CnxString` to retrieve the character from
/// @param index - The index to get the character for
///
/// @return The character at the given index
/// @ingroup cnx_string
__attr(nodiscard) __attr(not_null(1)) __attr(returns_not_null) char_ptr
	cnx_string_at_mut(CnxString* restrict self, usize index) ___DISABLE_IF_NULL(self);
/// @brief Returns a pointer to the character at the given index.
///
/// @param self - The `CnxString` to retrieve the character from
/// @param index - The index to get the character for
///
/// @return The character at the given index
/// @ingroup cnx_string
__attr(nodiscard) __attr(not_null(1)) __attr(returns_not_null) const_char_ptr
	cnx_string_at_const(const CnxString* restrict self, usize index) ___DISABLE_IF_NULL(self);
/// @brief Returns the character at the beginning of the string
///
/// @param self - The `CnxString` to retrieve the first character from
///
/// @return The first character
/// @ingroup cnx_string
__attr(nodiscard) __attr(not_null(1)) __attr(returns_not_null) char_ptr
	cnx_string_front_mut(CnxString* restrict self) ___DISABLE_IF_NULL(self);
/// @brief Returns the character at the beginning of the string
///
/// @param self - The `CnxString` to retrieve the first character from
///
/// @return The first character
/// @ingroup cnx_string
__attr(nodiscard) __attr(not_null(1)) __attr(returns_not_null) const_char_ptr
	cnx_string_front_const(const CnxString* restrict self) ___DISABLE_IF_NULL(self);
/// @brief Returns the character at the end of the string
///
/// @param self - The `CnxString` to retrieve the last character from
///
/// @return The last character
/// @ingroup cnx_string
__attr(nodiscard) __attr(not_null(1)) __attr(returns_not_null) char_ptr
	cnx_string_back_mut(CnxString* restrict self) ___DISABLE_IF_NULL(self);
/// @brief Returns the character at the end of the string
///
/// @param self - The `CnxString` to retrieve the last character from
///
/// @return The last character
/// @ingroup cnx_string
__attr(nodiscard) __attr(not_null(1)) __attr(returns_not_null) const_char_ptr
	cnx_string_back_const(const CnxString* restrict self) ___DISABLE_IF_NULL(self);
/// @brief Returns whether the string is empty or not
///
/// @param self - The `CnxString` to check for emptiness
///
/// @return `true` if empty, `false` otherwise
/// @ingroup cnx_string
__attr(nodiscard) __attr(not_null(1)) bool cnx_string_is_empty(const CnxString* restrict self)
	___DISABLE_IF_NULL(self);
/// @brief Returns whether the string contains capacity number of characters
///
/// @param self - The `CnxString` to check for fullness
///
/// @return `true` if full, `false` otherwise
/// @ingroup cnx_string
__attr(nodiscard) __attr(not_null(1)) bool cnx_string_is_full(const CnxString* restrict self)
	___DISABLE_IF_NULL(self);
/// @brief Returns the current size of the string
///
/// @param self - The `CnxString` to get the size of
///
/// @return the size of the string
/// @ingroup cnx_string
__attr(nodiscard) __attr(not_null(1)) usize cnx_string_size(const CnxString* restrict self)
	___DISABLE_IF_NULL(self);
/// @brief Returns the current length of the string
///
/// @param self - The `CnxString` to get the length of
///
/// @return the length of the string
/// @note This is equivalent to `cnx_string_size`
/// @ingroup cnx_string
__attr(nodiscard) __attr(not_null(1)) usize cnx_string_length(const CnxString* restrict self)
	___DISABLE_IF_NULL(self);
/// @brief Returns the maximum possible size of a `CnxString`
///
/// @return the maximum possible size
/// @ingroup cnx_string
__attr(nodiscard) usize cnx_string_max_size(void);
/// @brief Returns the current capacity of the string
///
/// @param self - The `CnxString` to get the capacity of
///
/// @return The capacity of the string
/// @ingroup cnx_string
__attr(nodiscard) __attr(not_null(1)) usize cnx_string_capacity(const CnxString* restrict self)
	___DISABLE_IF_NULL(self);
/// @brief Returns the first `num_chars` characters in the string as a new `CnxString`
///
/// @param self - The `CnxString` to get the characters from
/// @param num_chars - The number of characters to get
///
/// @return the first `num_chars` characters
///
/// @note if `num_chars > size` then the returned string will be null-padded with
/// `num_chars - size` null characters
/// @ingroup cnx_string
__attr(nodiscard) __attr(not_null(1)) CnxString
	cnx_string_first(const CnxString* restrict self, usize num_chars) ___DISABLE_IF_NULL(self)
		cnx_disable_if(num_chars == 0, "Can't get a string of 0 (zero) characters");
/// @brief Returns the first `num_chars` characters in the string as a `cstring` allocated with the
/// allocator associated with `self`
///
/// @param self - The `CnxString` to get the characters from
/// @param num_chars - The number of characters to get
///
/// @return the first `num_chars` characters
///
/// @note if `num_chars > size` then the returned string will be null-padded with
/// `num_chars - size` null characters
/// @ingroup cnx_string
__attr(nodiscard) __attr(not_null(1)) __attr(returns_not_null) cstring
	cnx_string_first_cstring(const CnxString* restrict self, usize num_chars)
		___DISABLE_IF_NULL(self)
			cnx_disable_if(num_chars == 0, "Can't get a string of 0 (zero) characters");
/// @brief Returns the first `num_chars` characters in the string as a `CnxStringView`
///
/// @param self - The `CnxString` to get the characters from
/// @param num_chars - The number of characters to get
///
/// @return the first `num_chars` characters
///
/// @note if `num_chars > size` then the returned `CnxStringView`s length will be truncated to
/// `size`
/// @ingroup cnx_string
__attr(nodiscard) __attr(not_null(1)) CnxStringView
	cnx_string_first_stringview(const CnxString* restrict self, usize num_chars)
		___DISABLE_IF_NULL(self)
			cnx_disable_if(num_chars == 0, "Can't get a string of 0 (zero) characters");
/// @brief Returns the last `num_chars` characters in the string as a new `CnxString`
///
/// @param self - The `CnxString` to get the characters from
/// @param num_chars - The number of characters to get
///
/// @return the last `num_chars` characters
///
/// @note if `num_chars > size` then the returned string will be null-padded with
/// `num_chars - size` null characters
/// @ingroup cnx_string
__attr(nodiscard) __attr(not_null(1)) CnxString
	cnx_string_last(const CnxString* restrict self, usize num_chars) ___DISABLE_IF_NULL(self)
		cnx_disable_if(num_chars == 0, "Can't get a string of 0 (zero) characters");
/// @brief Returns the last `num_chars` characters in the string as a `cstring` allocated with the
/// allocator associated with `self`
///
/// @param self - The `CnxString` to get the characters from
/// @param num_chars - The number of characters to get
///
/// @return the last `num_chars` characters
///
/// @note if `num_chars > size` then the returned string will be null-padded with
/// `num_chars - size` null characters
/// @ingroup cnx_string
__attr(nodiscard) __attr(not_null(1)) __attr(returns_not_null) cstring
	cnx_string_last_cstring(const CnxString* restrict self, usize num_chars)
		___DISABLE_IF_NULL(self)
			cnx_disable_if(num_chars == 0, "Can't get a string of 0 (zero) characters");
/// @brief Returns the last `num_chars` characters in the string as a `CnxStringView`
///
/// @param self - The `CnxString` to get the characters from
/// @param num_chars - The number of characters to get
///
/// @return the last `num_chars` characters
///
/// @note if `num_chars > size` then the returned `CnxStringView`s length will be truncated to
/// @ingroup cnx_string
__attr(nodiscard) __attr(not_null(1)) CnxStringView
	cnx_string_last_stringview(const CnxString* restrict self, usize num_chars)
		___DISABLE_IF_NULL(self)
			cnx_disable_if(num_chars == 0, "Can't get a string of 0 (zero) characters");
/// @brief Determines if the given strings are equal
///
/// @param self - The first string to compare
/// @param to_compare The second string to compare
///
/// @return `true` if the two strings are equal, `false otherwise`
/// @note if the lengths of the strings are different, they are __always__ unequal
/// @ingroup cnx_string
__attr(nodiscard) __attr(not_null(1, 2)) bool cnx_string_equal(const CnxString* restrict self,
															   const CnxString* restrict to_compare)
	___DISABLE_IF_NULL(self) cnx_disable_if(!to_compare, "Can't compare a CnxString to a nullptr");
/// @brief Determines if the given strings are equal
///
/// @param self - The first string to compare
/// @param to_compare - The second string to compare
/// @param length - The length of the second string to compare
///
/// @return `true` if the two strings are equal, `false` otherwise
/// @note if the lengths of the strings are different, they are __always__ unequal
/// @ingroup cnx_string
__attr(nodiscard)
	__attr(not_null(1, 2)) bool cnx_string_equal_cstring(const CnxString* restrict self,
														 restrict const_cstring to_compare,
														 usize length) ___DISABLE_IF_NULL(self)
		cnx_disable_if(!to_compare, "Can't compare a CnxString to a nullptr");
/// @brief Determines if the given strings are equal
///
/// @param self - The first string to compare
/// @param to_compare - The second string to compare
///
/// @return `true` if the two strings are equal, `false` otherwise
/// @note if the lengths of the strings are different, they are __always__ unequal
/// @ingroup cnx_string
__attr(nodiscard)
	__attr(not_null(1,
					2)) bool cnx_string_equal_stringview(const CnxString* restrict self,
														 const CnxStringView* restrict to_compare)
		___DISABLE_IF_NULL(self)
			cnx_disable_if(!to_compare, "Can't compare a CnxString to a nullptr");
/// @brief Determines if the string contains the given substring
///
/// @param self - The `CnxString` to search for `substring` in
/// @param substring - The substring to search for
///
/// @return `true` if this contains `substring`, `false` otherwise
/// @note if the length of `substring` is greater than the length of `self`,
/// this always returns false
/// @ingroup cnx_string
__attr(nodiscard)
	__attr(not_null(1, 2)) bool cnx_string_contains(const CnxString* restrict self,
													const CnxString* restrict substring)
		___DISABLE_IF_NULL(self)
			cnx_disable_if(!substring, "Can't check if a CnxString contains a nullptr");
/// @brief Determines if the string contains the given substring
///
/// @param self - The `CnxString` to search for `substring` in
/// @param substring - The substring to search for
/// @param substring_length - The length of the substring to search for
///
/// @return `true` if this contains `substring`, `false` otherwise
/// @note if the length of `substring` is greater than the length of `self`,
/// this always returns false
/// @ingroup cnx_string
__attr(nodiscard)
	__attr(not_null(1, 2)) bool cnx_string_contains_cstring(const CnxString* restrict self,
															restrict const_cstring substring,
															usize substring_length)
		___DISABLE_IF_NULL(self)
			cnx_disable_if(!substring, "Can't check if a CnxString contains a nullptr");
/// @brief Determines if the string contains the given substring
///
/// @param self - The `CnxString` to search for `substring` in
/// @param substring - The substring to search for
///
/// @return `true` if this contains `substring`, `false` otherwise
/// @note if the length of the `substring` is greater than the length of `self`,
/// this always returns false
/// @ingroup cnx_string
__attr(nodiscard)
	__attr(not_null(1,
					2)) bool cnx_string_contains_stringview(const CnxString* restrict self,
															const CnxStringView* restrict substring)
		___DISABLE_IF_NULL(self)
			cnx_disable_if(!substring, "Can't check if a CnxString contains a nullptr");
/// @brief Determines if the string begins with the given substring
///
/// @param self - The `CnxString` to search for `substring` in
/// @param substring - The substring to search for
///
/// @return `true` if this begins with `substring`, `false` otherwise
/// @note if the length of `substring` is greater than the length of `self`,
/// this always returns false
/// @ingroup cnx_string
__attr(nodiscard)
	__attr(not_null(1, 2)) bool cnx_string_starts_with(const CnxString* restrict self,
													   const CnxString* restrict substring)
		___DISABLE_IF_NULL(self)
			cnx_disable_if(!substring, "Can't check if a CnxString contains a nullptr");
/// @brief Determines if the string begins with the given substring
///
/// @param self - The `CnxString` to search for `substring` in
/// @param substring - The substring to search for
/// @param substring_length - The length of the substring to search for
///
/// @return `true` if this begins with `substring`, `false` otherwise
/// @note if the length of `substring` is greater than the length of `self`,
/// this always returns false
/// @ingroup cnx_string
__attr(nodiscard)
	__attr(not_null(1, 2)) bool cnx_string_starts_with_cstring(const CnxString* restrict self,
															   restrict const_cstring substring,
															   usize substring_length)
		___DISABLE_IF_NULL(self)
			cnx_disable_if(!substring, "Can't check if a CnxString contains a nullptr");
/// @brief Determines if the string begins with the given substring
///
/// @param self - The `CnxString` to search for `substring` in
/// @param substring - The substring to search for
///
/// @return `true` if this begins with `substring`, `false` otherwise
/// @note if the length of `substring` is greater than the length of `self`,
/// this always returns false
/// @ingroup cnx_string
__attr(nodiscard) __attr(not_null(1, 2)) bool cnx_string_starts_with_stringview(
	const CnxString* restrict self,
	const CnxStringView* restrict substring) ___DISABLE_IF_NULL(self)
	cnx_disable_if(!substring, "Can't check if a CnxString contains a nullptr");
/// @brief Determines if the string ends with the given substring
///
/// @param self - The `CnxString` to search for `substring` in
/// @param substring - The substring to search for
///
/// @return `true` if this ends with `substring`, `false` otherwise
/// @note if the length of `substring` is greater than the length of `self`,
/// this always returns false
/// @ingroup cnx_string
__attr(nodiscard)
	__attr(not_null(1, 2)) bool cnx_string_ends_with(const CnxString* restrict self,
													 const CnxString* restrict substring)
		___DISABLE_IF_NULL(self)
			cnx_disable_if(!substring, "Can't check if a CnxString contains a nullptr");
/// @brief Determines if the string ends with the given substring
///
/// @param self - The `CnxString` to search for `substring` in
/// @param substring - The substring to search for
/// @param substring_length - The length of the substring to search for
///
/// @return `true` if this ends with `substring`, `false` otherwise
/// @note if the length of `substring` is greater than the length of `self`,
/// this always returns false
/// @ingroup cnx_string
__attr(nodiscard)
	__attr(not_null(1, 2)) bool cnx_string_ends_with_cstring(const CnxString* restrict self,
															 restrict const_cstring substring,
															 usize substring_length)
		___DISABLE_IF_NULL(self)
			cnx_disable_if(!substring, "Can't check if a CnxString contains a nullptr");
/// @brief Determines if the string ends with the given substring
///
/// @param self - The `CnxString` to search for `substring` in
/// @param substring - The substring to search for
///
/// @return `true` if this ends with `substring`, `false` otherwise
/// @note if the length of `substring` is greater than the length of `self`,
/// this always returns false
/// @ingroup cnx_string
__attr(nodiscard) __attr(not_null(1, 2)) bool cnx_string_ends_with_stringview(
	const CnxString* restrict self,
	const CnxStringView* restrict substring) ___DISABLE_IF_NULL(self)
	cnx_disable_if(!substring, "Can't check if a CnxString contains a nullptr");
/// @brief Returns the index of the first occurrence of the given substring, or `None` if
/// the substring does not occur
///
/// @param self - The `CnxString` to search for `substring` in
/// @param substring - The substring to search for
///
/// @return The index of the first occurrence of `substring`, or `None`
/// @ingroup cnx_string
__attr(nodiscard) __attr(not_null(1, 2)) CnxOption(usize)
	cnx_string_find_first(const CnxString* restrict self, const CnxString* restrict substring)
		___DISABLE_IF_NULL(self)
			cnx_disable_if(!substring, "Can't check if a CnxString contains a nullptr");
/// @brief Returns the index of the first occurrence of the given substring, or `None` if
/// the substring does not occur
///
/// @param self - The `CnxString` to search for `substring` in
/// @param substring - The substring to search for
/// @param substring_length - The length of the substring to search for
///
/// @return The index of the first occurrence of `substring`, or `None`
/// @ingroup cnx_string
__attr(nodiscard) __attr(not_null(1, 2)) CnxOption(usize)
	cnx_string_find_first_cstring(const CnxString* restrict self,
								  restrict const_cstring substring,
								  usize substring_length) ___DISABLE_IF_NULL(self)
		cnx_disable_if(!substring, "Can't check if a CnxString contains a nullptr");
/// @brief Returns the index of the first occurrence of the given substring, or `None` if
/// the substring does not occur
///
/// @param self - The `CnxString` to search for `substring` in
/// @param substring - The substring to search for
///
/// @return The index of the first occurrence of `substring`, or `None`
/// @ingroup cnx_string
__attr(nodiscard) __attr(not_null(1, 2)) CnxOption(usize)
	cnx_string_find_first_stringview(const CnxString* restrict self,
									 const CnxStringView* restrict substring)
		___DISABLE_IF_NULL(self)
			cnx_disable_if(!substring, "Can't check if a CnxString contains a nullptr");
/// @brief Returns the index of the last occurrence of the given substring, or `None` if
/// the substring does not occur
///
/// @param self - The `CnxString` to search for `substring` in
/// @param substring - The substring to search for
///
/// @return The index of the last occurrence of `substring`, or `None`
/// @ingroup cnx_string
__attr(nodiscard) __attr(not_null(1, 2)) CnxOption(usize)
	cnx_string_find_last(const CnxString* restrict self, const CnxString* restrict substring)
		___DISABLE_IF_NULL(self)
			cnx_disable_if(!substring, "Can't check if a CnxString contains a nullptr");
/// @brief Returns the index of the last occurrence of the given substring, or `None` if
/// the substring does not occur
///
/// @param self - The `CnxString` to search for `substring` in
/// @param substring - The substring to search for
/// @param substring_length - The length of the substring to search for
///
/// @return The index of the last occurrence of `substring`, or `None`
/// @ingroup cnx_string
__attr(nodiscard) __attr(not_null(1, 2)) CnxOption(usize)
	cnx_string_find_last_cstring(const CnxString* restrict self,
								 restrict const_cstring substring,
								 usize substring_length) ___DISABLE_IF_NULL(self)
		cnx_disable_if(!substring, "Can't check if a CnxString contains a nullptr");
/// @brief Returns the index of the last occurrence of the given substring, or `None` if
/// the substring does not occur
///
/// @param self - The `CnxString` to search for `substring` in
/// @param substring - The substring to search for
///
/// @return The index of the last occurrence of `substring`, or `None`
/// @ingroup cnx_string
__attr(nodiscard) __attr(not_null(1, 2)) CnxOption(usize)
	cnx_string_find_last_stringview(const CnxString* restrict self,
									const CnxStringView* restrict substring)
		___DISABLE_IF_NULL(self)
			cnx_disable_if(!substring, "Can't check if a CnxString contains a nullptr");
/// @brief Returns the length `length` substring beginning at `index`
///
/// @param self - The `CnxString` to get the substring from
/// @param index - The index the substring should begin at
/// @param length - The length of the substring to get
///
/// @return the substring beginning at index and ending at length, exclusive
/// (the bounds are [index, length))
///
/// @note if `index + length` is greater than the size of the string, then the substring will be
/// null padded with `(index + length) - size` null characters
/// (its length will effectively be truncated to `size - index`, instead of `length`)
/// @ingroup cnx_string
__attr(nodiscard) __attr(not_null(1)) CnxString
	cnx_string_substring(const CnxString* restrict self, usize index, usize length)
		___DISABLE_IF_NULL(self);
/// @brief Returns the length `length` substring beginning at `index`, allocated with the given
/// allocator
///
/// @param self - The `CnxString` to get the substring from
/// @param index - The index the substring should begin at
/// @param length - The length of the substring to get
/// @param allocator - The allocator to use for memory allocation
///
/// @return the substring beginning at index and ending at length, exclusive
/// (the bounds are [index, length))
///
/// @note if `index + length` is greater than the size of the string, then the substring will be
/// null padded with `(index + length) - size` null characters
/// (its length will effectively be truncated to `size - index`, instead of `length`)
/// @ingroup cnx_string
__attr(nodiscard) __attr(not_null(1)) CnxString
	cnx_string_substring_with_allocator(const CnxString* restrict self,
										usize index,
										usize length,
										CnxAllocator allocator) ___DISABLE_IF_NULL(self);
/// @brief Returns a `CnxStringView` into `self` of the range [index, index + length)
///
/// @param self - The `CnxString` to get the view of
/// @param index - The index of the beginning of the view
/// @param length - The length of the view
///
/// @return a `CnxStringView` into `self`
/// @note if `index + length` is greater than the size of the string, then the substring will be
/// truncated to a length of `size - index`, instead of `length`
/// @ingroup cnx_string
__attr(nodiscard) __attr(not_null(1)) CnxStringView
	cnx_string_stringview_of(const CnxString* restrict self, usize index, usize length)
		___DISABLE_IF_NULL(self);
/// @brief Concatenates the two strings into a new one, using the allocator associated with
/// `left`, if necessary
///
/// @param left - The left-side string of the concatenation
/// @param right - The right-side string of the concatenation
///
/// @return  The concatenated string
/// @ingroup cnx_string
__attr(nodiscard) __attr(not_null(1, 2)) CnxString
	cnx_string_concatenate(const CnxString* restrict left, const CnxString* restrict right)
		___DISABLE_IF_NULL(left)
			cnx_disable_if(!right, "Can't concatenate a nullptr to a CnxString");
/// @brief Concatenates the two strings into a new one, using the allocator associated with
/// `left`, if necessary
///
/// @param left - The left-side string of the concatenation
/// @param right - The right-side string of the concatenation
/// @param right_length - The length of the right-side string of the concatenation
///
/// @return  The concatenated string
/// @ingroup cnx_string
__attr(nodiscard) __attr(not_null(1, 2)) CnxString
	cnx_string_concatenate_cstring(const CnxString* restrict left,
								   restrict const_cstring right,
								   usize right_length) ___DISABLE_IF_NULL(left)
		cnx_disable_if(!right, "Can't concatenate a nullptr to a CnxString");
/// @brief Concatenates the two strings into a new `CnxString`, using the default allocator (malloc)
///
/// @param left - The left (beginning) string to form the concatenation
/// @param left_length - The length of the left string
/// @param right - The right (ending) string to form the concatenation
/// @param right_length - The length of the right string
///
/// @return The concatenated string as a `CnxString`
/// @ingroup cnx_string
__attr(nodiscard) __attr(not_null(1, 3)) CnxString
	cnx_string_concatenate_cstrings(restrict const_cstring left,
									usize left_length,
									restrict const_cstring right,
									usize right_length)
		cnx_disable_if(!left, "Can't concatenate a nullptr into a CnxString")
			cnx_disable_if(!right, "Can't concatenate a nullptr into a CnxString");
/// @brief Concatenates the two strings into a new one, using the allocator associated with
/// `left`, if necessary
///
/// @param left - The left-side string of the concatenation
/// @param right - The right-side string of the concatenation
///
/// @return  The concatenated string
/// @ingroup cnx_string
__attr(nodiscard) __attr(not_null(1, 2)) CnxString
	cnx_string_concatenate_stringview(const CnxString* restrict left,
									  const CnxStringView* restrict right)
		cnx_disable_if(!left, "Can't concatenate a nullptr into a CnxString")
			cnx_disable_if(!right, "Can't concatenate a nullptr into a CnxString");
/// @brief Concatenates the two stringviews into a new `CnxString`, using the default allocator
/// (malloc)
///
/// @param left - The left-side string of the concatenation
/// @param right - The right-side string of the concatenation
///
/// @return  The concatenated string
/// @ingroup cnx_string
__attr(nodiscard) __attr(not_null(1, 2)) CnxString
	cnx_string_concatenate_stringviews(const CnxStringView* restrict left,
									   const CnxStringView* restrict right)
		cnx_disable_if(!left, "Can't concatenate a nullptr into a CnxString")
			cnx_disable_if(!right, "Can't concatenate a nullptr into a CnxString");

/// @brief This function is an __INTENTIONALLY_ undefined function so that improper use of
/// `cnx_string_concatenate` (passing values of invalid type(s)) will result in a build failure
/// @ingroup cnx_string
void invalid_types_passed_to_cnx_string_concatenate(void);

// clang-format off
/// @brief Concatenates the two `cstring`s, `left` and `right` and returns the result as a
/// `CnxString`
///
/// @param left - The left-side string of the concatenation
/// @param left_size - The size of `left_size`
/// @param right - The right-side string of the concatenation. Its size will be inferred
///
/// @return The concatenated result as a `CnxString`
#define cnx_string_concatenate_cstrings__(left, left_size, right) _Generic((right),				   \
	const_cstring 				: cnx_string_concatenate_cstrings(							   	   \
									left,						   								   \
									left_size, 			   	   									   \
									static_cast(const_cstring)(right), 						   	   \
									strlen(static_cast(const_cstring)(right))), 			   	   \
	cstring 					: cnx_string_concatenate_cstrings(							   	   \
									left,						   								   \
									left_size, 			   	   									   \
									static_cast(cstring)(right), 							   	   \
									strlen(static_cast(cstring)(right))), 			   		   	   \
	const char[sizeof(right)]	: cnx_string_concatenate_cstrings( 					/** NOLINT **/ \
									left,						   								   \
									left_size, 			   	   									   \
									static_cast(const_cstring)(right), 						   	   \
									sizeof(right)), 								/** NOLINT **/ \
	char[sizeof(right)]			: cnx_string_concatenate_cstrings(					/** NOLINT **/ \
									left,						   								   \
									left_size, 			   	   									   \
									static_cast(cstring)(right), 							   	   \
									sizeof(right)),									/** NOLINT **/ \
	default 					: invalid_types_passed_to_cnx_string_concatenate())

/// @brief Concatenates the two `CnxStringView`s, `left` and `right` and returns the result as a
/// `CnxString`
///
/// @param left - The left-side string of the concatenation
/// @param right - The right-side string of the concatenation
///
/// @return The concatenated result as a `CnxString`
#define cnx_string_concatenate_stringviews__(left, right) _Generic((right), 					   \
	CnxStringView*				: cnx_string_concatenate_stringviews(left,						   \
									static_cast(CnxStringView*)(right)),						   \
	const CnxStringView* 		: cnx_string_concatenate_stringviews(left,						   \
									static_cast(const CnxStringView*)(right)),					   \
	default						: invalid_types_passed_to_cnx_string_concatenate())

/// @brief Concatenates the given `CnxString`, `left`, with the given string of arbitrary type,
/// `right`, and returns the result as a `CnxString`
///
/// @param left - The left-side string of the concatenation
/// @param right - The right-side string of the concatenation
///
/// @return The concatenated result as a `CnxString`
#define cnx_string_concatenate_string__(left, right) _Generic((right), 						   	   \
	const_cstring 				: cnx_string_concatenate_cstring(							   	   \
									static_cast(CnxString*)(left), 						   	   	   \
									static_cast(const_cstring)(right),  				   	   	   \
									strlen(static_cast(const_cstring)(right))), 		   	   	   \
	cstring 					: cnx_string_concatenate_cstring(							   	   \
									static_cast(CnxString*)(left),  					   	   	   \
									static_cast(cstring)(right), 						   	   	   \
									strlen(static_cast(cstring)(right))), 				   	   	   \
	const char[sizeof(right)] 	: cnx_string_concatenate_cstring(					/** NOLINT **/ \
									static_cast(CnxString*)(left), 							   	   \
									static_cast(const_cstring)(right), 					   	   	   \
									sizeof(right)), 								/** NOLINT **/ \
	char[sizeof(right)] 		: cnx_string_concatenate_cstring( 					/** NOLINT **/ \
									static_cast(CnxString*)(left), 							   	   \
									static_cast(cstring)(right), 						   	   	   \
									sizeof(right)), 								/** NOLINT **/ \
	CnxStringView* 				: cnx_string_concatenate_stringview(						   	   \
									static_cast(CnxString*)(left), 					   		   	   \
									static_cast(CnxStringView*)(right)), 				   	   	   \
	const CnxStringView* 		: cnx_string_concatenate_stringview(						   	   \
									static_cast(CnxString*)(left), 			   		   		   	   \
									static_cast(const CnxStringView*)(right)), 				   	   \
	CnxString* 					: cnx_string_concatenate(									   	   \
									static_cast(CnxString*)(left), 							   	   \
									static_cast(CnxString*)(right)), 					   	   	   \
	const CnxString* 			: cnx_string_concatenate(									   	   \
									static_cast(CnxString*)(left), 							   	   \
									static_cast(const CnxString*)(right)), 						   \
	default						: invalid_types_passed_to_cnx_string_concatenate())

/// @brief Concatenates the two strings and returns the result as a new `CnxString`
///
/// @param left - The string to begin the new string with
/// @param right - The string to end the new string with
///
/// @return the concatenation of the two strings
/// @ingroup cnx_string
#define cnx_string_concatenate(left, right) _Generic((left), 									   \
	const_cstring 				: cnx_string_concatenate_cstrings__( 						   	   \
									static_cast(const_cstring)(left), 						   	   \
									strlen(static_cast(const_cstring)(left)), right),		   	   \
	cstring 					: cnx_string_concatenate_cstrings__( 						   	   \
									static_cast(cstring)(left), 							   	   \
									strlen(static_cast(cstring)(left)), right),				   	   \
	const char[sizeof(left)] 	: cnx_string_concatenate_cstrings__( 				/** NOLINT **/ \
									static_cast(const_cstring)(left), 						   	   \
									sizeof(left), right), 							/** NOLINT **/ \
	char[sizeof(left)]			: cnx_string_concatenate_cstrings__( 				/** NOLINT **/ \
									static_cast(const_cstring)(left), 						   	   \
									sizeof(left), right), 							/** NOLINT **/ \
	CnxStringView* 				: cnx_string_concatenate_stringviews__( 					   	   \
									static_cast(CnxStringView*)(left), right),		   		   	   \
	const CnxStringView* 		: cnx_string_concatenate_stringviews__( 					   	   \
									static_cast(const CnxStringView*)(left), right),		   	   \
	CnxString * 				: cnx_string_concatenate_string__(						   	   	   \
									static_cast(CnxString*)(left), right),				   	   	   \
	const CnxString * 			: cnx_string_concatenate_string__(						   	   	   \
									static_cast(const CnxString*)(left), right))
// clang-format on

/// @brief Concatenates the two strings into a new one, using the given allocator
///
/// @param left - The left-side string of the concatenation
/// @param right - The right-side string of the concatenation
/// @param allocator - The allocator to use for memory allocations
///
/// @return  The concatenated string
/// @ingroup cnx_string
__attr(nodiscard) __attr(not_null(1, 2)) CnxString
	cnx_string_concatenate_with_allocator(const CnxString* restrict left,
										  const CnxString* restrict right,
										  CnxAllocator allocator) ___DISABLE_IF_NULL(left)
		cnx_disable_if(!right, "Can't concatenate a nullptr to a CnxString");
/// @brief Concatenates the two strings into a new one, using the given allocator
///
/// @param left - The left-side string of the concatenation
/// @param right - The right-side string of the concatenation
/// @param right_length - The length of the right-side string of the concatenation
/// @param allocator - The allocator to use for memory allocations
///
/// @return  The concatenated string
/// @ingroup cnx_string
__attr(nodiscard) __attr(not_null(1, 2)) CnxString
	cnx_string_concatenate_cstring_with_allocator(const CnxString* restrict left,
												  restrict const_cstring right,
												  usize right_length,
												  CnxAllocator allocator) ___DISABLE_IF_NULL(left)
		cnx_disable_if(!right, "Can't concatenate a nullptr to a CnxString");
/// @brief Concatenates the two strings into a new one, using the given allocator
///
/// @param left - The left-side string of the concatenation
/// @param right - The right-side string of the concatenation
/// @param allocator - The allocator to use for memory allocations
///
/// @return  The concatenated string
/// @ingroup cnx_string
__attr(nodiscard) __attr(not_null(1, 2)) CnxString
	cnx_string_concatenate_stringview_with_allocator(const CnxString* restrict left,
													 const CnxStringView* restrict right,
													 CnxAllocator allocator)
		___DISABLE_IF_NULL(left)
			cnx_disable_if(!right, "Can't concatenate a nullptr to a CnxString");
/// @brief Concatenates the two strings into a new one, using the given allocator
///
/// @param left - The left-side string of the concatenation
/// @param left_length - The length of the left-side string of the concatenation
/// @param right - The right-side string of the concatenation
/// @param right_length - The length of the right-side string of the concatenation
/// @param allocator - The allocator to use for memory allocations
///
/// @return  The concatenated string
/// @ingroup cnx_string
__attr(nodiscard) __attr(not_null(1, 3)) CnxString
	cnx_string_concatenate_cstrings_with_allocator(restrict const_cstring left,
												   usize left_length,
												   restrict const_cstring right,
												   usize right_length,
												   CnxAllocator allocator)
		cnx_disable_if(!left, "Can't concatenate a nullptr into a CnxString")
			cnx_disable_if(!right, "Can't concatenate a nullptr into a CnxString");
/// @brief Concatenates the two strings into a new one, using the given allocator
///
/// @param left - The left-side string of the concatenation
/// @param right - The right-side string of the concatenation
/// @param allocator - The allocator to use for memory allocations
///
/// @return  The concatenated string
/// @ingroup cnx_string
__attr(nodiscard) __attr(not_null(1, 2)) CnxString
	cnx_string_concatenate_stringviews_with_allocator(const CnxStringView* restrict left,
													  const CnxStringView* restrict right,
													  CnxAllocator allocator)
		cnx_disable_if(!left, "Can't concatenate a nullptr into a CnxString")
			cnx_disable_if(!right, "Can't concatenate a nullptr into a CnxString");

// clang-format off
/// @brief Concatenates the two `cstring`s, `left` and `right` and returns the result as a
/// `CnxString`
///
/// @param left - The left-side string of the concatenation
/// @param left_size - The size of `left_size`
/// @param right - The right-side string of the concatenation. Its size will be inferred
/// @param allocator - The `CnxAllocator` to use for memory allocation
///
/// @return The concatenated result as a `CnxString`
#define cnx_string_concatenate_cstrings_with_allocator__(left, left_size, right, allocator) 	   \
	_Generic((right), 																			   \
		const_cstring 				: cnx_string_concatenate_cstrings_with_allocator(			   \
										left,						   							   \
										left_size, 			   	   								   \
										static_cast(const_cstring)(right), 						   \
										strlen(static_cast(const_cstring)(right)),				   \
										allocator), 			   								   \
		cstring 					: cnx_string_concatenate_cstrings_with_allocator(			   \
										left,						   							   \
										left_size, 			   	   								   \
										static_cast(cstring)(right), 							   \
										strlen(static_cast(cstring)(right)),					   \
										allocator), 			   		   						   \
		const char[sizeof(right)]	: cnx_string_concatenate_cstrings_with_allocator(/** NOLINT **/ \
										left,						   							   \
										left_size, 			   	   								   \
										static_cast(const_cstring)(right), 						   \
										sizeof(right), 								/** NOLINT **/ \
										allocator), 											   \
		char[sizeof(right)]			: cnx_string_concatenate_cstrings_with_allocator(/** NOLINT **/ \
										left,						   							   \
										left_size, 			   	   								   \
										static_cast(cstring)(right), 							   \
										sizeof(right), 								/** NOLINT **/ \
										allocator),												   \
		default 					: invalid_types_passed_to_cnx_string_concatenate())

/// @brief Concatenates the two `CnxStringView`s, `left` and `right` and returns the result as a
/// `CnxString`
///
/// @param left - The left-side string of the concatenation
/// @param right - The right-side string of the concatenation
/// @param allocator - The `CnxAllocator` to use for memory allocation
///
/// @return The concatenated result as a `CnxString`
#define cnx_string_concatenate_stringviews_with_allocator__(left, right, allocator) 			   \
	_Generic((right),						   													   \
		CnxStringView*				: cnx_string_concatenate_stringviews_with_allocator(left,	   \
										static_cast(CnxStringView*)(right), allocator),			   \
		const CnxStringView* 		: cnx_string_concatenate_stringviews_with_allocator(left,	   \
										static_cast(const CnxStringView*)(right), allocator),	   \
		default						: invalid_types_passed_to_cnx_string_concatenate())

/// @brief Concatenates the given `CnxString`, `left`, with the given string of arbitrary type,
/// `right`, and returns the result as a `CnxString`
///
/// @param left - The left-side string of the concatenation
/// @param right - The right-side string of the concatenation
/// @param allocator - The `CnxAllocator` to use for memory allocation
///
/// @return The concatenated result as a `CnxString`
#define cnx_string_concatenate_string_with_allocator__(left, right, allocator) 					   \
	_Generic((right), 						   	   												   \
		const_cstring 				: cnx_string_concatenate_cstring_with_allocator(			   \
										static_cast(CnxString*)(left), 						   	   \
										static_cast(const_cstring)(right),  				   	   \
										strlen(static_cast(const_cstring)(right)), 				   \
										allocator), 		   	   	   							   \
		cstring 					: cnx_string_concatenate_cstring_with_allocator(			   \
										static_cast(CnxString*)(left),  					   	   \
										static_cast(cstring)(right), 						   	   \
										strlen(static_cast(cstring)(right)), 					   \
										allocator), 				   	   	   					   \
		const char[sizeof(right)] 	: cnx_string_concatenate_cstring_with_allocator(/** NOLINT **/ \
										static_cast(CnxString*)(left), 							   \
										static_cast(const_cstring)(right), 					   	   \
										sizeof(right), 								/** NOLINT **/ \
										allocator), 											   \
		char[sizeof(right)] 		: cnx_string_concatenate_cstring_with_allocator(/** NOLINT **/ \
										static_cast(CnxString*)(left), 							   \
										static_cast(cstring)(right), 						   	   \
										sizeof(right), 								/** NOLINT **/ \
										allocator), 											   \
		CnxStringView* 				: cnx_string_concatenate_stringview_with_allocator(			   \
										static_cast(CnxString*)(left), 					   		   \
										static_cast(CnxStringView*)(right), allocator), 		   \
		const CnxStringView* 		: cnx_string_concatenate_stringview_with_allocator(			   \
										static_cast(CnxString*)(left), 			   		   		   \
										static_cast(const CnxStringView*)(right), allocator), 	   \
		CnxString* 					: cnx_string_concatenate_with_allocator(					   \
										static_cast(CnxString*)(left), 							   \
										static_cast(CnxString*)(right), allocator), 			   \
		const CnxString* 			: cnx_string_concatenate_with_allocator(					   \
										static_cast(CnxString*)(left), 							   \
										static_cast(const CnxString*)(right), allocator), 		   \
		default						: invalid_types_passed_to_cnx_string_concatenate())

/// @brief Concatenates the two strings and returns the result as a new `CnxString`
///
/// @param left - The string to begin the new string with
/// @param right - The string to end the new string with
/// @param allocator - The `CnxAllocator` to allocate memory with
///
/// @return the concatenation of the two strings
/// @ingroup cnx_string
#define cnx_string_concatenate_with_allocator(left, right, allocator) _Generic((left), 			   \
	const_cstring 				: cnx_string_concatenate_cstrings_with_allocator__( 			   \
									static_cast(const_cstring)(left), 						   	   \
									strlen(static_cast(const_cstring)(left)), right, 			   \
									allocator),		   	   										   \
	cstring 					: cnx_string_concatenate_cstrings_with_allocator__( 			   \
									static_cast(cstring)(left), 							   	   \
									strlen(static_cast(cstring)(left)), right, 					   \
									allocator),				   	   								   \
	const char[sizeof(left)] 	: cnx_string_concatenate_cstrings_with_allocator__( /** NOLINT **/ \
									static_cast(const_cstring)(left), 						   	   \
									sizeof(left), right, allocator), 				/** NOLINT **/ \
	char[sizeof(left)]			: cnx_string_concatenate_cstrings_with_allocator__( /** NOLINT **/ \
									static_cast(const_cstring)(left), 						   	   \
									sizeof(left), right, allocator), 				/** NOLINT **/ \
	CnxStringView* 				: cnx_string_concatenate_stringviews_with_allocator__( 			   \
									static_cast(CnxStringView*)(left), right, allocator),		   \
	const CnxStringView* 		: cnx_string_concatenate_stringviews_with_allocator__( 			   \
									static_cast(const CnxStringView*)(left), right, allocator),	   \
	CnxString * 				: cnx_string_concatenate_string_with_allocator__(				   \
									static_cast(CnxString*)(left), right, allocator),			   \
	const CnxString * 			: cnx_string_concatenate_string_with_allocator__(				   \
									static_cast(const CnxString*)(left), right, allocator))
// clang-format on

/// @brief Fills the string with the given character
///
/// @param self - The `CnxString` to fill
/// @param character - The character to fill the string with
///
/// @note this fills to capacity, not size
/// @ingroup cnx_string
__attr(not_null(1)) void cnx_string_fill(CnxString* restrict self, char character)
	___DISABLE_IF_NULL(self);
/// @brief Clears the string, filling it with null
///
/// @param self - The `CnxString` to clear
///
/// @note this will fill to capacity with null, thus size might increase
/// @ingroup cnx_string
__attr(not_null(1)) void cnx_string_clear(CnxString* restrict self) ___DISABLE_IF_NULL(self);
/// @brief Shrinks the string so its capacity equals its size
///
/// @param self - The `CnxString` to shrink
/// @ingroup cnx_string
__attr(not_null(1)) void cnx_string_shrink_to_fit(CnxString* restrict self)
	___DISABLE_IF_NULL(self);
/// @brief Inserts the given string `to_insert` into `self` at the given `index`
///
/// @param self - The `CnxString` to insert into
/// @param to_insert - The string to insert
/// @param index - The index at which to insert the string
/// @ingroup cnx_string
__attr(not_null(1, 2)) void cnx_string_insert(CnxString* restrict self,
											  const CnxString* restrict to_insert,
											  usize index) ___DISABLE_IF_NULL(self)
	cnx_disable_if(!to_insert, "Can't insert a nullptr into a CnxString");
/// @brief Inserts the given string `to_insert` into `self` at the given `index`
///
/// @param self - The `CnxString` to insert into
/// @param to_insert - The string to insert
/// @param to_insert_length - The length of the string to insert
/// @param index - The index at which to insert the string
/// @ingroup cnx_string
__attr(not_null(1, 2)) void cnx_string_insert_cstring(CnxString* restrict self,
													  restrict const_cstring to_insert,
													  usize to_insert_length,
													  usize index) ___DISABLE_IF_NULL(self)
	cnx_disable_if(!to_insert, "Can't insert a nullptr into a CnxString");
/// @brief Inserts the given string `to_insert` into `self` at the given `index`
///
/// @param self - The `CnxString` to insert into
/// @param to_insert - The string to insert
/// @param index - The index at which to insert the string
/// @ingroup cnx_string
__attr(not_null(1, 2)) void cnx_string_insert_stringview(CnxString* restrict self,
														 const CnxStringView* restrict to_insert,
														 usize index) ___DISABLE_IF_NULL(self)
	cnx_disable_if(!to_insert, "Can't insert a nullptr into a CnxString");
/// @brief Erases the character at the given index from the string
///
/// @param self - The `CnxString` to erase from
/// @param index - The index of the character to erase
/// @ingroup cnx_string
__attr(not_null(1)) void cnx_string_erase(CnxString* restrict self, usize index)
	___DISABLE_IF_NULL(self);
/// @brief Erases `num_character` characters from the string, starting at the given index
///
/// @param self - The `CnxString` to erase from
/// @param index - The index to start erasing at
/// @param num_characters - The number of characters to erase
///
/// @note if `index + num_characters` is out of bounds, this will erase 'size - index' characters
/// @ingroup cnx_string
__attr(not_null(1)) void cnx_string_erase_n(CnxString* restrict self,
											usize index,
											usize num_characters) ___DISABLE_IF_NULL(self);
/// @brief Resizes the string to the new size, null padding or truncating if necessary
///
/// @param self - The `CnxString` to resize
/// @param new_size - The new size for the string
/// @ingroup cnx_string
__attr(not_null(1)) void cnx_string_resize(CnxString* restrict self, usize new_size)
	___DISABLE_IF_NULL(self);
/// @brief Reserves additional capacity in the string
/// This will allocate enough memory to store __at least__ `new_capacity` number of characters,
// /but not necessarily exactly `new_capacity` number of characters
///
/// @param self - The `CnxString` to reserve additional space for
/// @param new_capacity - The new capacity for the string
///
/// @note if `new_capacity < capacity` then this will do nothing
/// @ingroup cnx_string
__attr(not_null(1)) void cnx_string_reserve(CnxString* restrict self, usize new_capacity)
	___DISABLE_IF_NULL(self);
/// @brief Appends the given character to the end of the string
///
/// @param self - The `CnxString` to append to
/// @param character - The character to append
/// @ingroup cnx_string
__attr(not_null(1)) void cnx_string_push_back(CnxString* restrict self, char character)
	___DISABLE_IF_NULL(self);
/// @brief Prepends the given character to the beginning of the string
///
/// @param self - The `CnxString` to prepends to
/// @param character - The character to prepends
/// @ingroup cnx_string
__attr(not_null(1)) void cnx_string_push_front(CnxString* restrict self, char character)
	___DISABLE_IF_NULL(self);
/// @brief Removes the last character in the string and returns it
///
/// @param self - The `CnxString` to pop the last character from
///
/// @return the last character
///
/// @note if the string is empty, this returns `None`
/// @ingroup cnx_string
__attr(nodiscard) __attr(not_null(1)) CnxOption(char) cnx_string_pop_back(CnxString* restrict self)
	___DISABLE_IF_NULL(self);
/// @brief Removes the first character in the string and returns it
///
/// @param self - The `CnxString` to pop the first character from
///
/// @return the first character
///
/// @note if the string is empty, this returns `None`
/// @ingroup cnx_string
__attr(nodiscard) __attr(not_null(1)) CnxOption(char) cnx_string_pop_front(CnxString* restrict self)
	___DISABLE_IF_NULL(self);
/// @brief Appends the given string to the end of the string
///
/// @param self - The `CnxString` to append to
/// @param to_append - The string to append
/// @ingroup cnx_string
__attr(not_null(1, 2)) void cnx_string_append(CnxString* restrict self,
											  const CnxString* restrict to_append)
	___DISABLE_IF_NULL(self) cnx_disable_if(!to_append, "Can't append a nullptr to a CnxString");
/// @brief Appends the given string to the end of the string
///
/// @param self - The `CnxString` to append to
/// @param to_append - The string to append
/// @param to_append_length - The length of the string to append
/// @ingroup cnx_string
__attr(not_null(1, 2)) void cnx_string_append_cstring(CnxString* restrict self,
													  restrict const_cstring to_append,
													  usize to_append_length)
	___DISABLE_IF_NULL(self) cnx_disable_if(!to_append, "Can't append a nullptr to a CnxString");
/// @brief Appends the given string to the end of the string
///
/// @param self - The `CnxString` to append to
/// @param to_append - The string to append
/// @ingroup cnx_string
__attr(not_null(1, 2)) void cnx_string_append_stringview(CnxString* restrict self,
														 const CnxStringView* restrict to_append)
	___DISABLE_IF_NULL(self) cnx_disable_if(!to_append, "Can't append a nullptr to a CnxString");
/// @brief Prepends the given string to the beginning of the string
///
/// @param self - The `CnxString` to prepend to
/// @param to_prepend - The string to prepend
/// @ingroup cnx_string
__attr(not_null(1, 2)) void cnx_string_prepend(CnxString* restrict self,
											   const CnxString* restrict to_prepend)
	___DISABLE_IF_NULL(self) cnx_disable_if(!to_prepend, "Can't prepend a nullptr to a CnxString");
/// @brief Prepends the given string to the beginning of the string
///
/// @param self - The `CnxString` to prepend to
/// @param to_prepend - The string to prepend
/// @param to_prepend_length - The length of the string to prepend
/// @ingroup cnx_string
__attr(not_null(1, 2)) void cnx_string_prepend_cstring(CnxString* restrict self,
													   restrict const_cstring to_prepend,
													   usize to_prepend_length)
	___DISABLE_IF_NULL(self) cnx_disable_if(!to_prepend, "Can't prepend a nullptr to a CnxString");
/// @brief Prepends the given string to the beginning of the string
///
/// @param self - The `CnxString` to prepend to
/// @param to_prepend - The string to prepend
/// @ingroup cnx_string
__attr(not_null(1, 2)) void cnx_string_prepend_stringview(CnxString* restrict self,
														  const CnxStringView* restrict to_prepend)
	___DISABLE_IF_NULL(self) cnx_disable_if(!to_prepend, "Can't prepend a nullptr to a CnxString");
/// @brief Replaces the substring beginning at `index` with the given one
///
/// @param self - The `CnxString` to replace a portion of
/// @param to_replace_with - The substring to replace the portion with
/// @param index - The index to start replacement
/// @ingroup cnx_string
__attr(not_null(1, 2)) void cnx_string_replace(CnxString* restrict self,
											   const CnxString* restrict to_replace_with,
											   usize index) ___DISABLE_IF_NULL(self)
	cnx_disable_if(!to_replace_with, "Can't replace a portion of a CnxString with a nullptr");
/// @brief Replaces the substring beginning at `index` with the given one
///
/// @param self - The `CnxString` to replace a portion of
/// @param to_replace_with - The substring to replace the portion with
/// @param to_replace_with_length - The length of the substring to replace the portion with
/// @param index - The index to start replacement
/// @ingroup cnx_string
__attr(not_null(1, 2)) void cnx_string_replace_cstring(CnxString* restrict self,
													   restrict const_cstring to_replace_with,
													   usize to_replace_with_length,
													   usize index) ___DISABLE_IF_NULL(self)
	cnx_disable_if(!to_replace_with, "Can't replace a portion of a CnxString with a nullptr");
/// @brief Replaces the substring beginning at `index` with the given one
///
/// @param self - The `CnxString` to replace a portion of
/// @param to_replace_with - The substring to replace the portion with
/// @param index - The index to start replacement
/// @ingroup cnx_string
__attr(not_null(1, 2)) void cnx_string_replace_stringview(
	CnxString* restrict self,
	const CnxStringView* restrict to_replace_with,
	usize index) ___DISABLE_IF_NULL(self)
	cnx_disable_if(!to_replace_with, "Can't replace a portion of a CnxString with a nullptr");

/// @brief Implement the Trait `CnxRandomAccessIterator` for `CnxStringIterator`
DeclIntoCnxRandomAccessIterator(CnxString, char_ref, cnx_string_into_iter, into);
/// @brief Implement the Trait `CnxRandomAccessIterator` for a reverse `CnxStringIterator`
DeclIntoCnxRandomAccessIterator(CnxString, char_ref, cnx_string_into_reverse_iter, into_reverse);

/// @brief Implement the Trait `CnxRandomAccessIterator` for `CnxStringConstIterator`
DeclIntoCnxRandomAccessIterator(CnxString, const_char_ref, cnx_string_into_const_iter, into);
/// @brief Implement the Trait `CnxRandomAccessIterator` for a reverse `CnxStringConstIterator`
DeclIntoCnxRandomAccessIterator(CnxString,
								const_char_ref,
								cnx_string_into_reverse_const_iter,
								into_reverse);

/// @brief Returns a `CnxRandomAccessIterator(char_ref)` into the given
/// `CnxString`, at the beginning of the iteration (pointing at the beginning of the string)
///
/// @param self - The `CnxString` to get an iterator into
///
/// @return an iterator at the beginning of the iteration (pointing at the beginning of the
/// string)
/// @ingroup cnx_string
__attr(nodiscard) __attr(not_null(1)) CnxRandomAccessIteratorchar_ref
	cnx_string_begin(CnxString* restrict self) ___DISABLE_IF_NULL(self);
/// @brief Returns a `CnxRandomAccessIterator(char_ref)` into the given
/// `CnxString`, at the end of the iteration (pointing at the end of the string)
///
/// @param self - The `CnxString` to get an iterator into
///
/// @return an iterator at the end of the iteration (pointing at the end of the
/// string)
/// @ingroup cnx_string
__attr(nodiscard) __attr(not_null(1)) CnxRandomAccessIteratorchar_ref
	cnx_string_end(CnxString* restrict self) ___DISABLE_IF_NULL(self);
/// @brief Returns a `CnxRandomAccessIterator(char_ref)` into the given
/// `CnxString`, at the beginning of the reversed iteration (pointing at the end of the string)
///
/// @param self - The `CnxString` to get an iterator into
///
/// @return an iterator at the beginning of the reversed iteration (pointing at the end of the
/// string)
/// @ingroup cnx_string
__attr(nodiscard) __attr(not_null(1)) CnxRandomAccessIteratorchar_ref
	cnx_string_rbegin(CnxString* restrict self) ___DISABLE_IF_NULL(self);
/// @brief Returns a `CnxRandomAccessIterator(char_ref)` into the given
/// `CnxString`, at the end of the reversed iteration (pointing at the beginning of the string)
///
/// @param self - The `CnxString` to get an iterator into
///
/// @return an iterator at the end of the reversed iteration (pointing at the beginning of the
/// string)
/// @ingroup cnx_string
__attr(nodiscard) __attr(not_null(1)) CnxRandomAccessIteratorchar_ref
	cnx_string_rend(CnxString* restrict self) ___DISABLE_IF_NULL(self);
/// @brief Returns a `CnxRandomAccessIterator(const_char_ref)` into the given
/// `CnxString`, at the beginning of the iteration (pointing at the beginning of the string)
///
/// @param self - The `CnxString` to get an iterator into
///
/// @return an iterator at the beginning of the iteration (pointing at the beginning of the
/// string)
/// @ingroup cnx_string
__attr(nodiscard) __attr(not_null(1)) CnxRandomAccessIteratorconst_char_ref
	cnx_string_cbegin(const CnxString* restrict self) ___DISABLE_IF_NULL(self);
/// @brief Returns a `CnxRandomAccessIterator(const_char_ref)` into the given
/// `CnxString`, at the end of the iteration (pointing at the end of the string)
///
/// @param self - The `CnxString` to get an iterator into
///
/// @return an iterator at the end of the iteration (pointing at the end of the
/// string)
/// @ingroup cnx_string
__attr(nodiscard) __attr(not_null(1)) CnxRandomAccessIteratorconst_char_ref
	cnx_string_cend(const CnxString* restrict self) ___DISABLE_IF_NULL(self);
/// @brief Returns a `CnxRandomAccessIterator(const_char_ref)` into the given
/// `CnxString`, at the beginning of the reversed iteration (pointing at the end of the string)
///
/// @param self - The `CnxString` to get an iterator into
///
/// @return an iterator at the beginning of the reversed iteration (pointing at the end of the
/// string)
/// @ingroup cnx_string
__attr(nodiscard) __attr(not_null(1)) CnxRandomAccessIteratorconst_char_ref
	cnx_string_crbegin(const CnxString* restrict self) ___DISABLE_IF_NULL(self);
/// @brief Returns a `CnxRandomAccessIterator(const_char_ref)` into the given
/// `CnxString`, at the end of the reversed iteration (pointing at the beginning of the string)
///
/// @param self - The `CnxString` to get an iterator into
///
/// @return an iterator at the end of the reversed iteration (pointing at the beginning of the
/// string)
/// @ingroup cnx_string
__attr(nodiscard) __attr(not_null(1)) CnxRandomAccessIteratorconst_char_ref
	cnx_string_crend(const CnxString* restrict self) ___DISABLE_IF_NULL(self);

/// @brief Returns a new `CnxStringView` into the given `CnxString`
///
/// @param string - The `CnxString` to get the view of
///
/// @return a `CnxStringView` into the given string
/// @ingroup cnx_stringview
__attr(nodiscard) __attr(not_null(1)) CnxStringView
	cnx_stringview_new(const CnxString* restrict string) ___DISABLE_IF_NULL(string);

#undef ___DISABLE_IF_NULL
#define ___DISABLE_IF_NULL(self) \
	cnx_disable_if(!(self), "Can't perform a stringview operation on a nullptr")
/// @brief Returns a new `CnxStringView` into the given cstring
///
/// @param string - The cstring to get the view of
/// @param index - The index to begin the view at
/// @param length - The length of the view
///
/// @return a `CnxStringView` into the given string
/// @ingroup cnx_stringview
__attr(nodiscard) __attr(not_null(1)) CnxStringView
	cnx_stringview_from(restrict const_cstring string, usize index, usize length)
		cnx_disable_if(!string, "Can't create a stringview from a nullptr");
/// @brief Returns the character in the view located at the given index
///
/// @param self - The `CnxStringView` to retrieve a character from
/// @param index - The index of the character to get
///
/// @return the character at index
/// @ingroup cnx_stringview
__attr(nodiscard) __attr(not_null(1)) const_char_ptr
	cnx_stringview_at(const CnxStringView* restrict self, usize index) ___DISABLE_IF_NULL(self);
/// @brief Returns the length of the `CnxStringView`
///
/// @param self - The `CnxStringView` to get the length of
///
/// @return the length of the stringview
/// @ingroup cnx_stringview
__attr(nodiscard) __attr(not_null(1)) usize
	cnx_stringview_length(const CnxStringView* restrict self) ___DISABLE_IF_NULL(self);
/// @brief Returns the size of the `CnxStringView`
///
/// @param self - The `CnxStringView` to get the size of
///
/// @return the size of the stringview
/// @ingroup cnx_stringview
__attr(nodiscard) __attr(not_null(1)) usize cnx_stringview_size(const CnxStringView* restrict self)
	___DISABLE_IF_NULL(self);

/// @brief Returns whether the given `CnxStringView`s are equivalent
///
/// @param self - The first `CnxStringView` to compare
/// @param to_compare - The `CnxStringView` to compare to
///
/// @return whether the stringviews are equal
/// @ingroup cnx_stringview
__attr(nodiscard)
	__attr(not_null(1, 2)) bool cnx_stringview_equal(const CnxStringView* restrict self,
													 const CnxStringView* restrict to_compare)
		___DISABLE_IF_NULL(self)
			cnx_disable_if(!to_compare, "Can't compare a stringview to a nullptr");

/// @brief Returns whether the given `CnxStringView`s are equivalent
///
/// @param self - The first `CnxStringView` to compare
/// @param to_compare - The `CnxStringView` to compare to
///
/// @return whether the stringviews are equal
/// @ingroup cnx_stringview
__attr(nodiscard)
	__attr(not_null(1, 2)) bool cnx_stringview_equal_string(const CnxStringView* restrict self,
															const CnxString* restrict to_compare)
		___DISABLE_IF_NULL(self)
			cnx_disable_if(!to_compare, "Can't compare a stringview to a nullptr");

/// @brief Returns whether the given `CnxStringView` is equivalent to the given `cstring`
///
/// @param self - The first `CnxStringView` to compare
/// @param to_compare - The `cstring` to compare to
/// @param to_compare_length - The length of the `cstring`
///
/// @return whether `self` is equalivalent to the given `cstring`
/// @ingroup cnx_stringview
__attr(nodiscard)
	__attr(not_null(1, 2)) bool cnx_stringview_equal_cstring(const CnxStringView* restrict self,
															 restrict const_cstring to_compare,
															 usize to_compare_length)
		___DISABLE_IF_NULL(self)
			cnx_disable_if(!to_compare, "Can't compare a stringview to a nullptr");

/// @brief Returns a `const_cstring` (`const char*`) representation of the `CnxStringView`
///
/// @param self - The `CnxStringView` to get the `const_cstring` representation of
///
/// @return the `const_cstring` representation of the stringview
/// @ingroup cnx_stringview
__attr(nodiscard) __attr(not_null(1)) __attr(returns_not_null) const_cstring
	cnx_stringview_into_cstring(const CnxStringView* restrict self) ___DISABLE_IF_NULL(self);

/// @brief Implement the Trait `CnxRandomAccessIterator` for `CnxStringViewIterator`
DeclIntoCnxRandomAccessIterator(CnxStringView, const_char_ref, cnx_stringview_into_iter, into);
/// @brief Implement the Trait `CnxRandomAccessIterator` for `CnxStringViewIterator`
DeclIntoCnxRandomAccessIterator(CnxStringView,
								const_char_ref,
								cnx_stringview_into_reverse_iter,
								into_reverse);

/// @brief Returns a `CnxRandomAccessIterator(const_char_ref)` into the given
/// `CnxStringView`, at the beginning of the iteration (pointing at the beginning of the view)
///
/// @param self - The `CnxStringView` to get an iterator into
///
/// @return an iterator at the beginning of the iteration (pointing at the beginning of the view)
/// @ingroup cnx_stringview
__attr(nodiscard) __attr(not_null(1)) CnxRandomAccessIteratorconst_char_ref
	cnx_stringview_begin(const CnxStringView* restrict self) ___DISABLE_IF_NULL(self);
/// @brief Returns a `CnxRandomAccessIterator(const_char_ref)` into the given
/// `CnxStringView`, at the end of the iteration (pointing at the end of the view)
///
/// @param self - The `CnxStringView` to get an iterator into
///
/// @return an iterator at the end of the iteration (pointing at the end of the view)
/// @ingroup cnx_stringview
__attr(nodiscard) __attr(not_null(1)) CnxRandomAccessIteratorconst_char_ref
	cnx_stringview_end(const CnxStringView* restrict self) ___DISABLE_IF_NULL(self);
/// @brief Returns a `CnxRandomAccessIterator(const_char_ref)` into the given
/// `CnxStringView` in reverse, at the beginning of the iteration (pointing at the end of the view)
///
/// @param self - The `CnxStringView` to get an iterator into
///
/// @return an iterator at the beginning of the reverse iteration (pointing at the end of the view)
/// @ingroup cnx_stringview
__attr(nodiscard) __attr(not_null(1)) CnxRandomAccessIteratorconst_char_ref
	cnx_stringview_rbegin(const CnxStringView* restrict self) ___DISABLE_IF_NULL(self);
/// @brief Returns a `CnxRandomAccessIterator(const_char_ref)` into the given
/// `CnxStringView` in reverse, at the end of the iteration (pointing at the beginning of the view)
///
/// @param self - The `CnxStringView` to get an iterator into
///
/// @return an iterator at the end of the reverse iteration (pointing at the beginning of the view)
/// @ingroup cnx_stringview
__attr(nodiscard) __attr(not_null(1)) CnxRandomAccessIteratorconst_char_ref
	cnx_stringview_rend(const CnxStringView* restrict self) ___DISABLE_IF_NULL(self);

/// @brief Returns the `cstring` representation of the given `CnxString`
///
/// @param self - The `CnxString` to get the `cstring` view of
///
/// @return The `cstring` representation of `self`
/// @ingroup cnx_string
#define cnx_string_into_cstring(self) cnx_string_into_cstring(&(self))
/// @brief Returns the `wcstring` representation of the given `CnxString`
///
/// @param self - The `CnxString` to get the `wcstring` representation of
///
/// @return The `wcstring` representation of `self`
/// @ingroup cnx_string
#define cnx_string_into_wcstring(self) cnx_string_into_wcstring(&(self))
/// @brief Returns the `wcstring` representation of the given `CnxString`
///
/// @param self - The `CnxString` to get the `wcstring` representation of
/// @param allocator - The `CnxAllocator` to allocator memory for the `wcstring` with
///
/// @return The `wcstring` representation of `self`
/// @ingroup cnx_string
#define cnx_string_into_wcstring_with_allocator(self, allocator) \
	cnx_string_into_wcstring_with_allocator(&(self), (allocator))
/// @brief Returns a `CnxStringView` into self
///
/// @param self - The `CnxString` to get the `CnxStringView` of
///
/// @return a `CnxStringView` into `self`
/// @ingroup cnx_string
#define cnx_string_into_stringview(self) cnx_string_into_stringview(&(self))
/// @brief Creates a copy of the given `CnxString`, `self`, using the `CnxAllocator` associated
/// with `self`
///
/// @param self - The `CnxString` to copy
///
/// @return a copy of `self`
/// @ingroup cnx_string
#define cnx_string_clone(self) cnx_string_clone(&(self))
/// @brief Creates a copy of the given `CnxString`, `self`, using the given `CnxAllocator`
///
/// @param self - The `CnxString` to copy
/// @param allocator - The `CnxAllocator` to associate with the new string
///
/// @return a copy of `self`
/// @ingroup cnx_string
#define cnx_string_clone_with_allocator(self, allocator) \
	cnx_string_clone_with_allocator(&(self), (allocator))
/// @brief Cleans up the given `CnxString`, freeing any allocated memory
///
/// @param self - The `CnxString` to cleanup
/// @ingroup cnx_string
#define cnx_string_free(self) cnx_string_free(&(self))
// clang-format off
/// @brief Returns a reference to the character at the given index.
///
/// @param self - The `CnxString` to retrieve the character from
/// @param index - The index to get the character for
///
/// @return The character at the given index
/// @ingroup cnx_string
#define cnx_string_at(self, index) *(_Generic((&(self)), 			\
	CnxString* 			: cnx_string_at_mut,  						\
	const CnxString* 	: cnx_string_at_const)(&(self), (index)))
// clang-format on
// clang-format off
/// @brief Returns a reference to the character at the beginning of the string
///
/// @param self - The `CnxString` to retrieve the first character from
///
/// @return The first character
/// @ingroup cnx_string
#define cnx_string_front(self) *(_Generic((&(self)), 			\
	CnxString*  		: cnx_string_front_mut, 				\
	const CnxString* 	: cnx_string_front_const)(&(self)))
// clang-format on
// clang-format off
/// @brief Returns a reference to the character at the end of the string
///
/// @param self - The `CnxString` to retrieve the last character from
///
/// @return The last character
/// @ingroup cnx_string
#define cnx_string_back(self) *(_Generic((&(self)), 		\
	CnxString* 			: cnx_string_back_mut, 				\
	const CnxString* 	: cnx_string_back_const)(&(self)))
// clang-format on
// clang-format off
/// @brief Returns the raw character array of the string
///
/// @param self - The `CnxString` to retrieve the raw array from
///
/// @return The raw character array
/// @ingroup cnx_string
#define cnx_string_data(self) (_Generic((&(self)), 				\
	CnxString*  		: cnx_string_front_mut, 				\
	const CnxString* 	: cnx_string_front_const)(&(self)))
// clang-format on

/// @brief Returns whether the given `CnxString` is empty or not
///
/// @param self - The `CnxString` to check if empty
///
/// @return `true` if `self` is empty, false otherwise
/// @ingroup cnx_string
#define cnx_string_is_empty(self) cnx_string_is_empty(&(self))
/// @brief Returns whether the given `CnxString` is full
///
/// @param self - The `CnxString` to check if full
///
/// @return `true` if `self` is full, false otherwise
/// @ingroup cnx_string
#define cnx_string_is_full(self) cnx_string_is_full(&(self))
/// @brief Returns the current size of the given `CnxString`
///
/// @param self - The `CnxString` to get the size of
///
/// @return the size of `self`
/// @ingroup cnx_string
#define cnx_string_size(self) cnx_string_size(&(self))
/// @brief Returns the current length of the given `CnxString`
///
/// @param self - The `CnxString` to get the length of
///
/// @return the length of `self`
/// @ingroup cnx_string
#define cnx_string_length(self) cnx_string_length(&(self))
/// @brief Returns the current capacity of the given `CnxString`
///
/// @param self - The `CnxString` to get the capacity of
///
/// @return the capacity of `self`
/// @ingroup cnx_string
#define cnx_string_capacity(self) cnx_string_capacity(&(self))
/// @brief Returns the first `num_chars` characters of `self` as a new `CnxString`, using
/// the `CnxAllocator` associated with `self`
///
/// @param self - The `CnxString` to get the first `num_chars` characters from
/// @param num_chars - The number of chars to get from the beginning of `self`
///
/// @return the first `num_chars` characters of `self` as a `CnxString`
/// @ingroup cnx_string
#define cnx_string_first(self, num_chars) cnx_string_first(&(self), (num_chars))
/// @brief Returns the first `num_chars` characters of `self` as a new `cstring`, using
/// the `CnxAllocator` associated with `self`
///
/// @param self - The `CnxString` to get the first `num_chars` characters from
/// @param num_chars - The number of chars to get from the beginning of `self`
///
/// @return the first `num_chars` characters of `self` as a `cstring`
/// @ingroup cnx_string
#define cnx_string_first_cstring(self, num_chars) cnx_string_first_cstring(&(self), (num_chars))
/// @brief Returns the first `num_chars` characters of `self` as a `CnxStringView`
///
/// @param self - The `CnxString` to get the first `num_chars` characters from
/// @param num_chars - The number of chars to get from the beginning of `self`
///
/// @return the first `num_chars` characters of `self` as a `CnxStringView`
/// @ingroup cnx_string
#define cnx_string_first_stringview(self, num_chars) \
	cnx_string_first_stringview(&(self), (num_chars))
/// @brief Returns the last `num_chars` characters of `self` as a new `CnxString`, using
/// the `CnxAllocator` associated with `self`
///
/// @param self - The `CnxString` to get the last `num_chars` characters from
/// @param num_chars - The number of chars to get from the end of `self`
///
/// @return the last `num_chars` characters of `self` as a `CnxString`
/// @ingroup cnx_string
#define cnx_string_last(self, num_chars) cnx_string_last(&(self), (num_chars))
/// @brief Returns the last `num_chars` characters of `self` as a new `cstring`, using
/// the `CnxAllocator` associated with `self`
///
/// @param self - The `CnxString` to get the last `num_chars` characters from
/// @param num_chars - The number of chars to get from the end of `self`
///
/// @return the last `num_chars` characters of `self` as a `cstring`
/// @ingroup cnx_string
#define cnx_string_last_cstring(self, num_chars) cnx_string_last_cstring(&(self), (num_chars))
/// @brief Returns the last `num_chars` characters of `self` as a `CnxStringView`
///
/// @param self - The `CnxString` to get the last `num_chars` characters from
/// @param num_chars - The number of chars to get from the end of `self`
///
/// @return the last `num_chars` characters of `self` as a `CnxStringView`
/// @ingroup cnx_string
#define cnx_string_last_stringview(self, num_chars) cnx_string_last_stringview(&(self), (num_chars))
// clang-format off
/// @brief Determines if this string and the given one are equal
///
/// @param self - The `CnxString` to be compared with
/// @param to_compare - The string to compare to
///
/// @return `true` if the two strings are equal, `false` otherwise
/// @ingroup cnx_string
#define cnx_string_equal(self, to_compare) _Generic((to_compare),                       		   \
	const_cstring 					: 	cnx_string_equal_cstring(&(self), 					   	   \
											static_cast(const_cstring)(to_compare), 			   \
											strlen(static_cast(const_cstring)(to_compare))),       \
	cstring 						: 	cnx_string_equal_cstring(&(self), 				   	   	   \
											static_cast(cstring)(to_compare), 				       \
											strlen(static_cast(cstring)(to_compare))),      	   \
	const char[sizeof(to_compare)] 	: 	cnx_string_equal_cstring(&(self), 			/** NOLINT **/ \
											static_cast(const_cstring)(to_compare), 			   \
											sizeof(to_compare)), 					/** NOLINT **/ \
	char[sizeof(to_compare)] 		: 	cnx_string_equal_cstring(&(self), 			/** NOLINT **/ \
											static_cast(cstring)(to_compare),  					   \
											sizeof(to_compare)), 					/** NOLINT **/ \
	CnxStringView* 					: 	cnx_string_equal_stringview(&(self), 			    	   \
											static_cast(CnxStringView*)(to_compare)), 		   	   \
	const CnxStringView* 			: 	cnx_string_equal_stringview(&(self), 			   	   	   \
											static_cast(const CnxStringView*)(to_compare)), 	   \
	CnxString* 						: 	cnx_string_equal(&(self), 							   	   \
											static_cast(CnxString*)(to_compare)),   			   \
	const CnxString* 				: 	cnx_string_equal(&(self), 							   	   \
											static_cast(const CnxString*)(to_compare)))
// clang-format on
// clang-format off
/// @brief Determines whether the string contains the given substring
///
/// @param self - The `CnxString` to search for `substring` in
/// @param substring - The substring to search for
///
/// @return `true` if the string contains `substring`, otherwise `false`
/// @ingroup cnx_string
#define cnx_string_contains(self, substring) _Generic((substring), 								   \
	const_cstring 					: cnx_string_contains_cstring(&(self), 				   		   \
										static_cast(const_cstring)(substring),  				   \
										strlen(static_cast(const_cstring)(substring))), 		   \
	cstring 						: cnx_string_contains_cstring(&(self),  				   	   \
										static_cast(cstring)(substring), 						   \
										strlen(static_cast(cstring)(substring))), 				   \
	const char[sizeof(substring)] 	: cnx_string_contains_cstring(&(self), 			/** NOLINT **/ \
										static_cast(const_cstring)(substring), 					   \
										sizeof(substring)), 						/** NOLINT **/ \
	char[sizeof(substring)] 		: cnx_string_contains_cstring(&(self), 			/** NOLINT **/ \
										static_cast(cstring)(substring), 						   \
										sizeof(substring)), 						/** NOLINT **/ \
	CnxStringView* 					: cnx_string_contains_stringview(&(self), 			   		   \
										static_cast(CnxStringView*)(substring)), 				   \
	const CnxStringView* 			: cnx_string_contains_stringview(&(self), 			   		   \
										static_cast(const CnxStringView*)(substring)), 			   \
	CnxString* 						: cnx_string_contains(&(self), 						   		   \
										static_cast(CnxString*)(substring)), 					   \
	const CnxString* 				: cnx_string_contains(&(self), 						   		   \
										static_cast(const CnxString*)(substring)))
// clang-format on
// clang-format off
/// @brief Determines whether the string starts with the given substring
///
/// @param self - The `CnxString` to search for `substring` in
/// @param substring - The substring to search for
///
/// @return `true` if the string starts with `substring`, otherwise `false`
/// @ingroup cnx_string
#define cnx_string_starts_with(self, substring) _Generic((substring), 							   \
	const_cstring 					: cnx_string_starts_with_cstring(&(self), 			   		   \
										static_cast(const_cstring)(substring),  				   \
										strlen(static_cast(const_cstring)(substring))), 		   \
	cstring 						: cnx_string_starts_with_cstring(&(self),  			   		   \
										static_cast(cstring)(substring), 						   \
										strlen(static_cast(cstring)(substring))), 				   \
	const char[sizeof(substring)] 	: cnx_string_starts_with_cstring(&(self), 		/** NOLINT **/ \
										static_cast(const_cstring)(substring), 					   \
										sizeof(substring)), 						/** NOLINT **/ \
	char[sizeof(substring)] 		: cnx_string_starts_with_cstring(&(self), 		/** NOLINT **/ \
										static_cast(cstring)(substring), 						   \
										sizeof(substring)), 						/** NOLINT **/ \
	CnxStringView* 					: cnx_string_starts_with_stringview(&(self), 			   	   \
										static_cast(CnxStringView*)(substring)), 				   \
	const CnxStringView* 			: cnx_string_starts_with_stringview(&(self), 			   	   \
										static_cast(const CnxStringView*)(substring)), 			   \
	CnxString* 						: cnx_string_starts_with(&(self), 					   		   \
										static_cast(CnxString*)(substring)), 					   \
	const CnxString* 				: cnx_string_starts_with(&(self), 					   		   \
										static_cast(const CnxString*)(substring)))
// clang-format on
// clang-format off
/// @brief Determines whether the string ends with the given substring
///
/// @param self - The `CnxString` to search for `substring` in
/// @param substring - The substring to search for
///
/// @return `true` if the string ends with `substring`, otherwise `false`
/// @ingroup cnx_string
#define cnx_string_ends_with(self, substring) _Generic((substring), 							   \
	const_cstring 					: cnx_string_ends_with_cstring(&(self), 				   	   \
										static_cast(const_cstring)(substring),  				   \
										strlen(static_cast(const_cstring)(substring))), 		   \
	cstring 						: cnx_string_ends_with_cstring(&(self),  				   	   \
										static_cast(cstring)(substring), 						   \
										strlen(static_cast(cstring)(substring))), 				   \
	const char[sizeof(substring)] 	: cnx_string_ends_with_cstring(&(self), 		/** NOLINT **/ \
										static_cast(const_cstring)(substring), 					   \
										sizeof(substring)), 						/** NOLINT **/ \
	char[sizeof(substring)] 		: cnx_string_ends_with_cstring(&(self), 		/** NOLINT **/ \
										static_cast(cstring)(substring), 						   \
										sizeof(substring)), 						/** NOLINT **/ \
	CnxStringView* 					: cnx_string_ends_with_stringview(&(self), 			   		   \
										static_cast(CnxStringView*)(substring)), 				   \
	const CnxStringView* 			: cnx_string_ends_with_stringview(&(self), 			   		   \
										static_cast(const CnxStringView*)(substring)), 			   \
	CnxString* 						: cnx_string_ends_with(&(self), 						   	   \
										static_cast(CnxString*)(substring)), 					   \
	const CnxString* 				: cnx_string_ends_with(&(self), 						   	   \
										static_cast(const CnxString*)(substring)))
// clang-format on
// clang-format off
/// @brief Finds the first occurrence of the given substring in `self`, if any
///
/// @param self - The `CnxString` to search for `substring` in
/// @param substring - The substring to search for
///
/// @return `Some` of the index into `self` at the beginning of the substring's
/// occurrence in `self`, if `self` contains the substring, otherwise `None`
/// @ingroup cnx_string
#define cnx_string_find_first(self, substring) _Generic((substring), 							   \
	const_cstring 					: cnx_string_find_first_cstring(&(self), 				   	   \
										static_cast(const_cstring)(substring),  				   \
										strlen(static_cast(const_cstring)(substring))), 		   \
	cstring 						: cnx_string_find_first_cstring(&(self),  			   		   \
										static_cast(cstring)(substring), 						   \
										strlen(static_cast(cstring)(substring))), 				   \
	const char[sizeof(substring)] 	: cnx_string_find_first_cstring(&(self), 		/** NOLINT **/ \
										static_cast(const_cstring)(substring), 					   \
										sizeof(substring)), 						/** NOLINT **/ \
	char[sizeof(substring)] 		: cnx_string_find_first_cstring(&(self), 		/** NOLINT **/ \
										static_cast(cstring)(substring), 						   \
										sizeof(substring)), 						/** NOLINT **/ \
	CnxStringView* 					: cnx_string_find_first_stringview(&(self), 			   	   \
										static_cast(CnxStringView*)(substring)), 				   \
	const CnxStringView* 			: cnx_string_find_first_stringview(&(self), 			   	   \
										static_cast(const CnxStringView*)(substring)), 			   \
	CnxString* 						: cnx_string_find_first(&(self), 						   	   \
										static_cast(CnxString*)(substring)), 					   \
	const CnxString* 				: cnx_string_find_first(&(self), 						   	   \
										static_cast(const CnxString*)(substring)))
// clang-format on
// clang-format off
/// @brief Finds the last occurrence of the given substring in `self`, if any
///
/// @param self - The `CnxString` to search for `substring` in
/// @param substring - The substring to search for
///
/// @return `Some` of the index into `self` at the beginning of the substring's
/// occurrence in `self`, if `self` contains the substring, otherwise `None`
/// @ingroup cnx_string
#define cnx_string_find_last(self, substring) _Generic((substring), 							   \
	const_cstring 					: cnx_string_find_last_cstring(&(self), 				   	   \
										static_cast(const_cstring)(substring),  				   \
										strlen(static_cast(const_cstring)(substring))), 		   \
	cstring 						: cnx_string_find_last_cstring(&(self),  				   	   \
										static_cast(cstring)(substring), 						   \
										strlen(static_cast(cstring)(substring))), 				   \
	const char[sizeof(substring)] 	: cnx_string_find_last_cstring(&(self), 		/** NOLINT **/ \
										static_cast(const_cstring)(substring), 					   \
										sizeof(substring)), 						/** NOLINT **/ \
	char[sizeof(substring)] 		: cnx_string_find_last_cstring(&(self), 		/** NOLINT **/ \
										static_cast(cstring)(substring), 						   \
										sizeof(substring)), 						/** NOLINT **/ \
	CnxStringView* 					: cnx_string_find_last_stringview(&(self), 			   		   \
										static_cast(CnxStringView*)(substring)), 				   \
	const CnxStringView* 			: cnx_string_find_last_stringview(&(self), 			   		   \
										static_cast(const CnxStringView*)(substring)), 			   \
	CnxString* 						: cnx_string_find_last(&(self), 						   	   \
										static_cast(CnxString*)(substring)), 					   \
	const CnxString* 				: cnx_string_find_last(&(self), 						   	   \
										static_cast(const CnxString*)(substring)))
// clang-format on

/// @brief Returns the substring of `self` starting at `index` with length `length` as a
/// `CnxString`, using the `CnxAllocator` associated with `self`
///
/// @param self - The `CnxString` to get a substring from
/// @param index - The index to start the substring at
/// @param length - The length of the substring
///
/// @return a substring of `self`
/// @ingroup cnx_string
#define cnx_string_substring(self, index, length) cnx_string_substring(&(self), index, length)
/// @brief Returns the substring of `self` starting at `index` with length `length` as a
/// `CnxString`, associating the given `CnxAllocator` with the substring
///
/// @param self - The `CnxString` to get a substring from
/// @param index - The index to start the substring at
/// @param length - The length of the substring
/// @param allocator - The `CnxAllocator` to associate with the substring
///
/// @return a substring of `self`
/// @ingroup cnx_string
#define cnx_string_substring_with_allocator(self, index, length, allocator) \
	cnx_string_substring_with_allocator(&(self), index, length, allocator)
/// @brief Returns a `CnxStringView` into `self` beginning at `index` with length `length`
///
/// @param self - The `CnxString` to get a `CnxStringView` into
/// @param index - The index to begin the view at
/// @param length - The length of the view
///
/// @return a `CnxStringView` into self
/// @ingroup cnx_string
#define cnx_string_stringview_of(self, index, length) \
	cnx_string_stringview_of(&(self), index, length)
/// @brief Fills the given `CnxString` with the given character
///
/// @param self - The `CnxString` to fill
/// @param character - The character to fill `self` with
/// @ingroup cnx_string
#define cnx_string_fill(self, character) cnx_string_fill(&(self), character)
/// @brief Clears the given `CnxString`
///
/// @param self - The `CnxString` to clear
/// @ingroup cnx_string
#define cnx_string_clear(self) cnx_string_clear(&(self))
/// @brief Shrinks the capacity of the given `CnxString` to its size
///
/// @param self - The `CnxString` to shrink
/// @ingroup cnx_string
#define cnx_string_shrink_to_fit(self) cnx_string_shrink_to_fit(&(self))
// clang-format off
/// @brief Inserts `to_insert` into `self` at the given index
///
/// @param self - The `CnxString` to insert into
/// @param to_insert - The string to insert
/// @param index - The index into `self` at which to insert the `to_insert`
/// @ingroup cnx_string
#define cnx_string_insert(self, to_insert, index) _Generic((to_insert), 						   \
	const_cstring 					: cnx_string_insert_cstring(&(self), 					   	   \
										static_cast(const_cstring)(to_insert),  				   \
										strlen(static_cast(const_cstring)(to_insert)), index), 	   \
	cstring 						: cnx_string_insert_cstring(&(self),  				   		   \
										static_cast(cstring)(to_insert), 						   \
										strlen(static_cast(cstring)(to_insert)), index), 		   \
	const char[sizeof(to_insert)] 	: cnx_string_insert_cstring(&(self), 			/** NOLINT **/ \
										static_cast(const_cstring)(to_insert), 					   \
										sizeof(to_insert), index), 					/** NOLINT **/ \
	char[sizeof(to_insert)] 		: cnx_string_insert_cstring(&(self), 			/** NOLINT **/ \
										static_cast(cstring)(to_insert), 						   \
										sizeof(to_insert), index), 					/** NOLINT **/ \
	CnxStringView* 					: cnx_string_insert_stringview(&(self), 				   	   \
										static_cast(CnxStringView*)(to_insert), index), 		   \
	const CnxStringView* 			: cnx_string_insert_stringview(&(self), 				   	   \
										static_cast(const CnxStringView*)(to_insert), index), 	   \
	CnxString* 						: cnx_string_insert(&(self), 							   	   \
										static_cast(CnxString*)(to_insert), index), 			   \
	const CnxString* 				: cnx_string_insert(&(self), 							   	   \
										static_cast(const CnxString*)(to_insert), index))
// clang-format on
/// @brief Erases the character at the given `index` from `self`
///
/// @param self - The `CnxString` to remove a character from
/// @param index - The index of the character to remove
/// @ingroup cnx_string
#define cnx_string_erase(self, index) cnx_string_erase(&(self), index)
/// @brief Erases `num_characters` characters from `self`, beginning at `index`
///
/// @param self - The `CnxString` to remove characters from
/// @param index - The index where removal should begin
/// @param num_characters - The number of characters to remove
/// @ingroup cnx_string
#define cnx_string_erase_n(self, index, num_characters) \
	cnx_string_erase_n(&(self), index, num_characters)
/// @brief Resizes the given `CnxString` to the given `new_size`, filling the characters in the
/// expanded size with null
///
/// @param self - The `CnxString` to resize
/// @param new_size - The new size of the string
/// @ingroup cnx_string
#define cnx_string_resize(self, new_size) cnx_string_resize(&(self), new_size)
/// @brief Allocates memory such that at least `new_capacity` number of characters can be stored
/// in the given `CnxString`
///
/// @param self - The `CnxString` to reserve memory in
/// @param new_capacity - The number of characters to be able to store
/// @ingroup cnx_string
#define cnx_string_reserve(self, new_capacity) cnx_string_reserve(&(self), new_capacity)
/// @brief Pushes the given character onto the end of the given `CnxString`
///
/// @param self - The `CnxString` to append a character to
/// @param character - The character to append to `self`
/// @ingroup cnx_string
#define cnx_string_push_back(self, character) cnx_string_push_back(&(self), character)
/// @brief Pushes the given character onto the beginning of the given `CnxString`
///
/// @param self - The `CnxString` to prepend a character to
/// @param character - The character to prepend to `self`
/// @ingroup cnx_string
#define cnx_string_push_front(self, character) cnx_string_push_front(&(self), character)
/// @brief Removes the last character in `self` if `cnx_string_size(self) > 0`, and returns it
///
/// @param self - The `CnxString` to pop the last character from
///
/// @return `Some(char)` if `cnx_string_size(self) > 0`, else `None(char)`
/// @ingroup cnx_string
#define cnx_string_pop_back(self) cnx_string_pop_back(&(self))
/// @brief Removes the first character in `self` if `cnx_string_size(self) > 0`, and returns it
///
/// @param self - The `CnxString` to pop the first character from
///
/// @return `Some(char)` if `cnx_string_size(self) > 0`, else `None(char)`
/// @ingroup cnx_string
#define cnx_string_pop_front(self) cnx_string_pop_front(&(self))
// clang-format off
/// @brief Appends `to_append` to the end of `self`
///
/// @param self - The `CnxString` to append to
/// @param to_append - The string to append
/// @ingroup cnx_string
#define cnx_string_append(self, to_append) _Generic((to_append), 						   		   \
	const_cstring 					: cnx_string_append_cstring(&(self), 					   	   \
										static_cast(const_cstring)(to_append),  				   \
										strlen(static_cast(const_cstring)(to_append))), 	   	   \
	cstring 						: cnx_string_append_cstring(&(self),  				   		   \
										static_cast(cstring)(to_append), 						   \
										strlen(static_cast(cstring)(to_append))), 		   		   \
	const char[sizeof(to_append)] 	: cnx_string_append_cstring(&(self), 			/** NOLINT **/ \
										static_cast(const_cstring)(to_append), 					   \
										sizeof(to_append)), 						/** NOLINT **/ \
	char[sizeof(to_append)] 		: cnx_string_append_cstring(&(self), 			/** NOLINT **/ \
										static_cast(cstring)(to_append), 						   \
										sizeof(to_append)), 						/** NOLINT **/ \
	CnxStringView* 					: cnx_string_append_stringview(&(self), 				   	   \
										static_cast(CnxStringView*)(to_append)), 		   		   \
	const CnxStringView* 			: cnx_string_append_stringview(&(self), 				   	   \
										static_cast(const CnxStringView*)(to_append)), 		   	   \
	CnxString* 						: cnx_string_append(&(self), 							   	   \
										static_cast(CnxString*)(to_append)), 			   		   \
	const CnxString* 				: cnx_string_append(&(self), 							   	   \
										static_cast(const CnxString*)(to_append)))
// clang-format on
// clang-format off
/// @brief Prepends `to_prepend` to the beginning of `self`
///
/// @param self - The `CnxString` to prepend to
/// @param to_prepend - The string to prepend
/// @ingroup cnx_string
#define cnx_string_prepend(self, to_prepend) _Generic((to_prepend), 						   	   \
	const_cstring 					: cnx_string_prepend_cstring(&(self), 				   		   \
										static_cast(const_cstring)(to_prepend),  				   \
										strlen(static_cast(const_cstring)(to_prepend))), 	   	   \
	cstring 						: cnx_string_prepend_cstring(&(self),  				   		   \
										static_cast(cstring)(to_prepend), 						   \
										strlen(static_cast(cstring)(to_prepend))), 		   		   \
	const char[sizeof(to_prepend)] 	: cnx_string_prepend_cstring(&(self), 			/** NOLINT **/ \
										static_cast(const_cstring)(to_prepend), 				   \
										sizeof(to_prepend)), 						/** NOLINT **/ \
	char[sizeof(to_prepend)] 		: cnx_string_prepend_cstring(&(self), 			/** NOLINT **/ \
										static_cast(cstring)(to_prepend), 						   \
										sizeof(to_prepend)), 						/** NOLINT **/ \
	CnxStringView* 					: cnx_string_prepend_stringview(&(self), 				   	   \
										static_cast(CnxStringView*)(to_prepend)), 		   		   \
	const CnxStringView* 			: cnx_string_prepend_stringview(&(self), 				   	   \
										static_cast(const CnxStringView*)(to_prepend)), 		   \
	CnxString* 						: cnx_string_prepend(&(self), 						   		   \
										static_cast(CnxString*)(to_prepend)), 			   		   \
	const CnxString* 				: cnx_string_prepend(&(self), 						   		   \
										static_cast(const CnxString*)(to_prepend)))
// clang-format on
// clang-format off
/// @brief Replaces the contents of `self` with `to_replace_with`, beginning at the given index
///
/// @param self - The `CnxString` to replace a portion of
/// @param to_replace_with - The string to replace `self`'s contents with
/// @param index - The index to start replacement at
/// @ingroup cnx_string
#define cnx_string_replace(self, to_replace_with, index) _Generic((to_replace_with), 			   \
	const_cstring 						: cnx_string_replace_cstring(&(self), 			   		   \
											static_cast(const_cstring)(to_replace_with),  		   \
											strlen(static_cast(const_cstring)(to_replace_with)),   \
											index), 	   										   \
	cstring 							: cnx_string_replace_cstring(&(self),  			   		   \
											static_cast(cstring)(to_replace_with), 				   \
											strlen(static_cast(cstring)(to_replace_with)),		   \
											index), 		   									   \
	const char[sizeof(to_replace_with)] : cnx_string_replace_cstring(&(self), 		/** NOLINT **/ \
											static_cast(const_cstring)(to_replace_with), 		   \
											sizeof(to_replace_with), 				/** NOLINT **/ \
											index), 											   \
	char[sizeof(to_replace_with)] 		: cnx_string_replace_cstring(&(self), 		/** NOLINT **/ \
											static_cast(cstring)(to_replace_with), 				   \
											sizeof(to_replace_with), 				/** NOLINT **/ \
											index), 											   \
	CnxStringView* 						: cnx_string_replace_stringview(&(self), 			   	   \
											static_cast(CnxStringView*)(to_replace_with), 		   \
											index), 		       								   \
	const CnxStringView* 				: cnx_string_replace_stringview(&(self), 			   	   \
											static_cast(const CnxStringView*)(to_replace_with),	   \
											index), 	   										   \
	CnxString* 							: cnx_string_replace(&(self), 					   		   \
											static_cast(CnxString*)(to_replace_with),			   \
											index), 			   	   							   \
	const CnxString* 					: cnx_string_replace(&(self), 					   		   \
											static_cast(const CnxString*)(to_replace_with), index))
// clang-format on

/// @brief Returns a `CnxRandomAccessIterator(char_ref)` into the given
/// `CnxString`, at the beginning of the iteration (pointing at the beginning of the string)
///
/// @param self - The `CnxString` to get an iterator into
///
/// @return an iterator at the beginning of the iteration (pointing at the beginning of the
/// string)
/// @ingroup cnx_string
#define cnx_string_begin(self) cnx_string_begin(&(self))
/// @brief Returns a `CnxRandomAccessIterator(char_ref)` into the given
/// `CnxString`, at the end of the iteration (pointing at the end of the string)
///
/// @param self - The `CnxString` to get an iterator into
///
/// @return an iterator at the end of the iteration (pointing at the end of the
/// string)
/// @ingroup cnx_string
#define cnx_string_end(self) cnx_string_end(&(self))
/// @brief Returns a `CnxRandomAccessIterator(char_ref)` into the given
/// `CnxString`, at the beginning of the reversed iteration (pointing at the end of the string)
///
/// @param self - The `CnxString` to get an iterator into
///
/// @return an iterator at the beginning of the reversed iteration (pointing at the end of the
/// string)
/// @ingroup cnx_string
#define cnx_string_rbegin(self) cnx_string_rbegin(&(self))
/// @brief Returns a `CnxRandomAccessIterator(char_ref)` into the given
/// `CnxString`, at the end of the reversed iteration (pointing at the beginning of the string)
///
/// @param self - The `CnxString` to get an iterator into
///
/// @return an iterator at the end of the reversed iteration (pointing at the beginning of the
/// string)
/// @ingroup cnx_string
#define cnx_string_rend(self) cnx_string_rend(&(self))
/// @brief Returns a `CnxRandomAccessIterator(const_char_ref)` into the given
/// `CnxString`, at the beginning of the iteration (pointing at the beginning of the string)
///
/// @param self - The `CnxString` to get an iterator into
///
/// @return an iterator at the beginning of the iteration (pointing at the beginning of the
/// string)
/// @ingroup cnx_string
#define cnx_string_cbegin(self) cnx_string_cbegin(&(self))
/// @brief Returns a `CnxRandomAccessIterator(const_char_ref)` into the given
/// `CnxString`, at the end of the iteration (pointing at the end of the string)
///
/// @param self - The `CnxString` to get an iterator into
///
/// @return an iterator at the end of the iteration (pointing at the end of the
/// string)
/// @ingroup cnx_string
#define cnx_string_cend(self) cnx_string_cend(&(self))
/// @brief Returns a `CnxRandomAccessIterator(const_char_ref)` into the given
/// `CnxString`, at the beginning of the reversed iteration (pointing at the end of the string)
///
/// @param self - The `CnxString` to get an iterator into
///
/// @return an iterator at the beginning of the reversed iteration (pointing at the end of the
/// string)
/// @ingroup cnx_string
#define cnx_string_crbegin(self) cnx_string_crbegin(&(self))
/// @brief Returns a `CnxRandomAccessIterator(const_char_ref)` into the given
/// `CnxString`, at the end of the reversed iteration (pointing at the beginning of the string)
///
/// @param self - The `CnxString` to get an iterator into
///
/// @return an iterator at the end of the reversed iteration (pointing at the beginning of the
/// string)
/// @ingroup cnx_string
#define cnx_string_crend(self) cnx_string_crend(&(self))

/// @brief Returns a const reference to the character at `index` in the given `CnxStringView`
///
/// @param self - The `CnxStringView` to get a character from
/// @param index - The index of the desired character in `self`
///
/// @return a const reference to the character at `index` in `self
/// @ingroup cnx_stringview
#define cnx_stringview_at(self, index) cnx_stringview_at(&(self), index)
/// @brief Returns the current length of the given `CnxStringView`
///
/// @param self - The `CnxStringView` to get the length of
///
/// @return the length of `self`
/// @ingroup cnx_stringview
#define cnx_stringview_length(self) cnx_stringview_length(&(self))
/// @brief Returns the current size of the given `CnxStringView`
///
/// @param self - The `CnxStringView` to get the size of
///
/// @return the size of `self`
/// @ingroup cnx_stringview
#define cnx_stringview_size(self) cnx_stringview_size(&(self))
// clang-format off

/// @brief Returns whether the given `CnxStringView`s are equivalent
///
/// @param self - The first `CnxStringView` to compare
/// @param to_compare - The `CnxStringView` to compare to
///
/// @return whether the stringviews are equal
/// @ingroup cnx_stringview
#define cnx_stringview_equal(self, to_compare) _Generic((to_compare), 							   \
	const_cstring 					: 	cnx_stringview_equal_cstring(&(self), 					   \
											static_cast(const_cstring)(to_compare), 			   \
											strlen(static_cast(const_cstring)(to_compare))),       \
	cstring 						: 	cnx_stringview_equal_cstring(&(self), 				   	   \
											static_cast(cstring)(to_compare), 				       \
											strlen(static_cast(const_cstring)(to_compare))),       \
	const char[sizeof(to_compare)] 	: 	cnx_stringview_equal_cstring(&(self), 		/** NOLINT **/ \
											static_cast(const_cstring)(to_compare), 			   \
											sizeof(to_compare)), 					/** NOLINT **/ \
	char[sizeof(to_compare)] 		: 	cnx_stringview_equal_cstring(&(self), 		/** NOLINT **/ \
											static_cast(const_cstring)(to_compare),  			   \
											sizeof(to_compare)), 					/** NOLINT **/ \
	CnxStringView* 					: 	cnx_stringview_equal(&(self), 			    	   		   \
											static_cast(const CnxStringView*)(to_compare)), 	   \
	const CnxStringView* 			: 	cnx_stringview_equal(&(self), 			   	   			   \
											static_cast(const CnxStringView*)(to_compare)), 	   \
	CnxString* 						: 	cnx_stringview_equal_string(&(self), 			    	   \
											static_cast(const CnxString*)(to_compare)), 		   \
	const CnxString* 				: 	cnx_stringview_equal_string(&(self), 			   	   	   \
											static_cast(const CnxString*)(to_compare)))
// clang-format on

/// @brief Returns the `cstring` view of the given `CnxStringView`
///
/// @param self - The `CnxStringView` to get the `cstring` view of
///
/// @return `self` as a `cstring`
/// @ingroup cnx_stringview
#define cnx_stringview_into_cstring(self) cnx_stringview_into_cstring(&(self))
/// @brief Returns a `CnxRandomAccessIterator(const_char_ref)` into the given
/// `CnxStringView`, at the beginning of the iteration (pointing at the beginning of the view)
///
/// @param self - The `CnxStringView` to get an iterator into
///
/// @return an iterator at the beginning of the iteration (pointing at the beginning of the
/// view)
/// @ingroup cnx_stringview
#define cnx_stringview_begin(self) cnx_stringview_begin(&(self))
/// @brief Returns a `CnxRandomAccessIterator(const_char_ref)` into the given
/// `CnxStringView`, at the end of the iteration (pointing at the end of the view)
///
/// @param self - The `CnxStringView` to get an iterator into
///
/// @return an iterator at the end of the iteration (pointing at the end of the view)
/// @ingroup cnx_stringview
#define cnx_stringview_end(self) cnx_stringview_end(&(self))
/// @brief Returns a `CnxRandomAccessIterator(const_char_ref)` into the given
/// `CnxStringView`, at the beginning of the reversed iteration (pointing at the end of the
/// view)
///
/// @param self - The `CnxStringView` to get an iterator into
///
/// @return an iterator at the beginning of the reversed iteration (pointing at the end of the
/// view)
/// @ingroup cnx_stringview
#define cnx_stringview_rbegin(self) cnx_stringview_rbegin(&(self))
/// @brief Returns a `CnxRandomAccessIterator(const_char_ref)` into the given
/// `CnxStringView`, at the end of the reversed iteration (pointing at the beginning of the
/// view)
///
/// @param self - The `CnxStringView` to get an iterator into
///
/// @return an iterator at the end of the reversed iteration (pointing at the beginning of the
/// view)
/// @ingroup cnx_stringview
#define cnx_stringview_rend(self) cnx_stringview_rend(&(self))
/// @brief Returns a `CnxRandomAccessIterator(const_char_ref)` into the given
/// `CnxStringView`, at the beginning of the iteration (pointing at the beginning of the view)
///
/// @param self - The `CnxStringView` to get an iterator into
///
/// @return an iterator at the beginning of the iteration (pointing at the beginning of the
/// view)
/// @ingroup cnx_stringview
#define cnx_stringview_cbegin(self) cnx_stringview_cbegin(&(self))
/// @brief Returns a `CnxRandomAccessIterator(const_char_ref)` into the given
/// `CnxStringView`, at the end of the iteration (pointing at the end of the view)
///
/// @param self - The `CnxStringView` to get an iterator into
///
/// @return an iterator at the end of the iteration (pointing at the end of the view)
/// @ingroup cnx_stringview
#define cnx_stringview_cend(self) cnx_stringview_cend(&(self))
/// @brief Returns a `CnxRandomAccessIterator(const_char_ref)` into the given
/// `CnxStringView`, at the beginning of the reversed iteration (pointing at the end of the
/// view)
///
/// @param self - The `CnxStringView` to get an iterator into
///
/// @return an iterator at the beginning of the reversed iteration (pointing at the end of the
/// view)
/// @ingroup cnx_stringview
#define cnx_stringview_crbegin(self) cnx_stringview_crbegin(&(self))
/// @brief Returns a `CnxRandomAccessIterator(const_char_ref)` into the given
/// `CnxStringView`, at the end of the reversed iteration (pointing at the beginning of the
/// view)
///
/// @param self - The `CnxStringView` to get an iterator into
///
/// @return an iterator at the end of the reversed iteration (pointing at the beginning of the
/// view)
/// @ingroup cnx_stringview
#define cnx_stringview_crend(self) cnx_stringview_crend(&(self))

/// @brief The function vector table of methods associated with `CnxStringView`
typedef struct cnx_stringview_vtable_t {
	/// @brief Returns a `CnxRandomAccessIterator(const_char_ref)` into the given
	/// `CnxStringView`, at the beginning of the iteration (pointing at the beginning of the view)
	CnxRandomAccessIterator(const_char_ref) (*const begin)(const CnxStringView* restrict self);
	/// @brief Returns a `CnxRandomAccessIterator(const_char_ref)` into the given
	/// `CnxStringView`, at the end of the iteration (pointing at the end of the view)
	CnxRandomAccessIterator(const_char_ref) (*const end)(const CnxStringView* restrict self);
	/// @brief Returns a `CnxRandomAccessIterator(const_char_ref)` into the given
	/// `CnxStringView`, at the beginning of the reversed iteration (pointing at the end of the
	/// view)
	CnxRandomAccessIterator(const_char_ref) (*const rbegin)(const CnxStringView* restrict self);
	/// @brief Returns a `CnxRandomAccessIterator(const_char_ref)` into the given
	/// `CnxStringView`, at the end of the reversed iteration (pointing at the beginning of the
	/// view)
	CnxRandomAccessIterator(const_char_ref) (*const rend)(const CnxStringView* restrict self);
	/// @brief Returns a `CnxRandomAccessIterator(const_char_ref)` into the given
	/// `CnxStringView`, at the beginning of the iteration (pointing at the beginning of the view)
	CnxRandomAccessIterator(const_char_ref) (*const cbegin)(const CnxStringView* restrict self);
	/// @brief Returns a `CnxRandomAccessIterator(const_char_ref)` into the given
	/// `CnxStringView`, at the end of the iteration (pointing at the end of the view)
	CnxRandomAccessIterator(const_char_ref) (*const cend)(const CnxStringView* restrict self);
	/// @brief Returns a `CnxRandomAccessIterator(const_char_ref)` into the given
	/// `CnxStringView`, at the beginning of the reversed iteration (pointing at the end of the
	/// view)
	CnxRandomAccessIterator(const_char_ref) (*const crbegin)(const CnxStringView* restrict self);
	/// @brief Returns a `CnxRandomAccessIterator(const_char_ref)` into the given
	/// `CnxStringView`, at the end of the reversed iteration (pointing at the beginning of the
	/// view)
	CnxRandomAccessIterator(const_char_ref) (*const crend)(const CnxStringView* restrict self);
} cnx_stringview_vtable_t;

/// @brief The function vector table of methods associated with `CnxString`
/// @ingroup cnx_string
typedef struct cnx_string_vtable_t {
	/// @brief Returns a `CnxRandomAccessIterator(char_ref)` into the given
	/// `CnxString`, at the beginning of the iteration (pointing at the beginning of the string)
	CnxRandomAccessIterator(char_ref) (*const begin)(CnxString* restrict self);
	/// @brief Returns a `CnxRandomAccessIterator(char_ref)` into the given
	/// `CnxString`, at the end of the iteration (pointing at the end of the string)
	CnxRandomAccessIterator(char_ref) (*const end)(CnxString* restrict self);
	/// @brief Returns a `CnxRandomAccessIterator(char_ref)` into the given
	/// `CnxString`, at the beginning of the reversed iteration (pointing at the end of the
	/// string)
	CnxRandomAccessIterator(char_ref) (*const rbegin)(CnxString* restrict self);
	/// @brief Returns a `CnxRandomAccessIterator(char_ref)` into the given
	/// `CnxString`, at the end of the reversed iteration (pointing at the beginning of the
	/// string)
	CnxRandomAccessIterator(char_ref) (*const rend)(CnxString* restrict self);
	/// @brief Returns a `CnxRandomAccessIterator(const_char_ref)` into the given
	/// `CnxString`, at the beginning of the iteration (pointing at the beginning of the string)
	CnxRandomAccessIterator(const_char_ref) (*const cbegin)(const CnxString* restrict self);
	/// @brief Returns a `CnxRandomAccessIterator(const_char_ref)` into the given
	/// `CnxString`, at the end of the iteration (pointing at the end of the string)
	CnxRandomAccessIterator(const_char_ref) (*const cend)(const CnxString* restrict self);
	/// @brief Returns a `CnxRandomAccessIterator(const_char_ref)` into the given
	/// `CnxString`, at the beginning of the reversed iteration (pointing at the end of the
	/// string)
	CnxRandomAccessIterator(const_char_ref) (*const crbegin)(const CnxString* restrict self);
	/// @brief Returns a `CnxRandomAccessIterator(const_char_ref)` into the given
	/// `CnxString`, at the end of the reversed iteration (pointing at the beginning of the
	/// string)
	CnxRandomAccessIterator(const_char_ref) (*const crend)(const CnxString* restrict self);
} cnx_string_vtable_t;

#undef ___DISABLE_IF_NULL
#endif // CNX_STRING
