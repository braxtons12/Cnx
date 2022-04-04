/// @file StdString.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief This module provides string and stringview types comparable to C++ for C2nxt
/// @version 0.2.2
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

/// @ingroup collections
/// @{
/// @defgroup std_string StdString
/// `StdString` is a bounds-safe string type that manages its own length and capacity, is fully
/// `cstring` (aka `char*`) compatible, provides C2nxt compatible random-access iterators over its
/// contents, and is allocator aware. It aims to provide more modern string-handling capabilities to
/// C, with an API and functionality comparable to C++'s `std::string`. `StdString` is always
/// null-terminated, so it's completely compatible with C's existing string functions, so long as
/// those functions do not modify the length of the string or insert null terminators (other mutable
/// access is acceptable).
///
/// `StdString` provides a small size optimization (SSO). This means that for short strings, the
/// data associated with the string will be stored on the stack, up to a certain number of
/// characters, before resorting to heap allocation. This does not require `StdString` to take up
/// extra stack space in order to implement, `sizeof(StdString)` would be the same regardless of
/// this optimization. The optimization allows for up to `2 * sizeof(usize) + sizeof(void*) - 1`
/// bytes to be stored on the stack. on x86_64, this translates to 23 characters, excluding the null
/// terminator (24 including the null terminator).
///
/// Example:
/// @code {.c}
/// #include <string.h> // for strlen
/// #include "C2nxt/StdString.h"
/// #include "C2nxt/StdIO.h"
/// let_mut string = std_string_from("C2nxt");
/// std_string_prepend(string, "Hello World ");
/// std_string_append(string, " style!");
///
/// // `StdString is fully compatible with non-mutating standard C string functions,
/// // and partially compatible with mutating standard C string functions
/// // (length must not be modified and null terminators must not be inserted,
/// // outside of `StdString`'s own APIs)
/// let len_c = strlen(std_string_into_cstring(string));
/// println("string is {} chars long (c-style)", len_c);
///
/// let len = std_string_length(string);
/// println("string is {} chars long (C2nxt-style)", len);
///
/// // print `string` to stdout, at once, followed by a newline
/// println("{}", string);
///
/// // range-for loop enabled by C2nxt compatible iterators
/// // print `string` to stdout, one character at a time, followed by a newline
/// foreach(character, string) {
///		print("{}", character);
/// }
/// print("\n");
/// @endcode
/// @}

/// @ingroup views
/// @{
/// @defgroup std_stringview StdStringView
/// A `StdStringView` is a "view" into a `StdString`. It allows you to lazily inspect the contents
/// of its associated string, but not modify them. It is effectively a fat reference to the contents
/// of its associated string.
/// @}

#ifndef STD_STRING
/// @brief STD_STRING
#define STD_STRING

#include <C2nxt/StdAllocators.h>
#include <C2nxt/StdBasicTypes.h>
#include <C2nxt/StdIterator.h>
#include <stdbool.h>
#define OPTION_INCLUDE_DEFAULT_INSTANTIATIONS TRUE
#include <C2nxt/StdOption.h>
#undef OPTION_INCLUDE_DEFAULT_INSTANTIATIONS
#include <wchar.h>

/// @brief The size of the long string representation of `StdString`
#define STD_STRING_SIZEOF_LONG_REP (static_cast(usize)(sizeof(usize) * 2U + sizeof(cstring)))
/// @brief The number of `char`s in a SSOd `StdString`
#define STD_STRING_SHORT_OPTIMIZATION_ARRAY_SIZE \
	(static_cast(usize)((STD_STRING_SIZEOF_LONG_REP - 1U) / sizeof(char)))
/// @brief The number of `char`s in a SSOd `StdString`
#define STD_STRING_SHORT_OPTIMIZATION_CAPACITY STD_STRING_SHORT_OPTIMIZATION_ARRAY_SIZE

/// @brief The function vector table of methods associated with `StdStringView`
/// @ingroup std_stringview
typedef struct std_stringview_vtable_t std_stringview_vtable_t;

/// @brief C2nxt stringview type.
/// A stringview is a "view" into a `StdString`. It allows you to lazily inspect the contents of the
/// string, but not modify them. It is effectively a fat pointer into the contents of the associated
/// `StdString`
/// @ingroup std_stringview
typedef struct StdStringView {
	/// @brief The length of the string view
	usize m_length;
	/// @brief The view
	const_cstring m_view;
	/// @brief The function vector table of methods associated with `StdStringView`
	const std_stringview_vtable_t* m_vtable;
} StdStringView;

/// @brief The function vector table of methods associated with `StdString`
/// @ingroup std_string
typedef struct std_string_vtable_t std_string_vtable_t;

/// @brief C2nxt string type
/// `StdString` is a bounds safe, allocator aware, potentially dynamically allocated string type
/// with significantly improved ergonomics over `const char*`s (aka `cstring`s), but maintains
/// compatibility with them
/// (`StdString` is always null terminated).
/// Provides similar functionality to C++'s `std::string`, but in C
///
/// Example:
/// @code {.c}
/// #include "C2nxt/StdString.h"
/// #include "C2nxt/StdIO.h"
/// // create default allocator (malloc allocates, free frees)
/// let allocator = std_allocator_new();
/// // could also call std_string_from("This is a string") since we're using the default
/// // allocator
/// let_mut string = std_string_from_with_allocator("This is a string", allocator);
///	std_string_append(&string, " with some extra text");
///	std_string_prepend(&string, "Hello world!\n");
/// // prints to stdout:
/// // Hello world!
/// // This is a string with some extra text
/// println("{}", string);
/// @endcode
/// @ingroup std_string
typedef struct StdString {
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
			char m_short[STD_STRING_SHORT_OPTIMIZATION_ARRAY_SIZE + 1];
		};
	};
	/// @brief The allocator to use for memory allocation
	StdAllocator m_allocator;
	/// @brief The function vector table of methods associated with `StdString`
	const std_string_vtable_t* m_vtable;
} StdString;

/// @brief C2nxt string iterator storage type
/// `StdStringIterator` is the underlying storage type used by `StdString` for its
/// non-const iterator type (`StdRandomAccessIterator(char_ptr)`)
/// @ingroup std_string
typedef struct StdStringIterator {
	/// @brief The current index of the iterator
	isize m_index;
	/// @brief The `StdString` this iterator iterates over
	StdString* m_string;
} StdStringIterator;

/// @brief C2nxt string const iterator storage type
/// `StdStringIterator` is the underlying storage type used by `StdString` for its
/// const iterator type (`StdRandomAccessIterator(const_char_ptr)`)
/// @ingroup std_string
typedef struct StdStringConstIterator {
	/// @brief The current index of the iterator
	isize m_index;
	/// @brief The `StdString` this iterator iterates over
	const StdString* m_string;
} StdStringConstIterator;

/// @brief C2nxt stringview const iterator storage type
/// `StdStringViewIterator` is the underlying storage type used by `StdStringView` for its
/// const iterator type (`StdRandomAccessIterator(const_char_ptr)`)
/// @ingroup std_stringview
typedef struct StdStringViewIterator {
	/// @brief The current index of the iterator
	isize m_index;
	/// @brief The `StdStringView` this iterator iterates over
	const StdStringView* m_view;
} StdStringViewIterator;

/// @brief type alias for a reference to `StdString`
typedef StdString* Ref(StdString);
/// @brief type alias for a reference to `StdStringView`
typedef StdStringView* Ref(StdStringView);
/// @brief type alias for a const reference to `StdString`
typedef const StdString* ConstRef(StdString);
/// @brief type alias for a const reference to `StdStringView`
typedef const StdStringView* ConstRef(StdStringView);

#define OPTION_DECL TRUE

/// @brief Declares `StdOption(T)` for `StdString`
#define OPTION_T StdString
#include <C2nxt/StdOption.h>
#undef OPTION_T

/// @brief Declares `StdOption(T)` for `Ref(StdString)`
#define OPTION_T Ref(StdString)
#include <C2nxt/StdOption.h>
#undef OPTION_T

/// @brief Declares `StdOption(T)` for `ConstRef(StdString)`
#define OPTION_T ConstRef(StdString)
#include <C2nxt/StdOption.h>
#undef OPTION_T

/// @brief Declares `StdOption(T)` for `StdStringView`
#define OPTION_T StdStringView
#include <C2nxt/StdOption.h>
#undef OPTION_T

/// @brief Declares `StdOption(T)` for `Ref(StdStringView)`
#define OPTION_T Ref(StdStringView)
#include <C2nxt/StdOption.h>
#undef OPTION_T

/// @brief Declares `StdOption(T)` for `ConstRef(StdStringView)`
#define OPTION_T ConstRef(StdStringView)
#include <C2nxt/StdOption.h>
#undef OPTION_T

#undef OPTION_DECL

/// @brief Declares iterator types for `Ref(StdString)`
DeclStdIterators(Ref(StdString));
/// @brief Declares iterator types for `ConstRef(StdString)`
DeclStdIterators(ConstRef(StdString));
/// @brief Declares iterator types for `Ref(StdStringView)`
DeclStdIterators(Ref(StdStringView));
/// @brief Declares iterator types for `ConstRef(StdStringView)`
DeclStdIterators(ConstRef(StdStringView));

/// @brief Creates a new, empty `StdString`
///
/// @return an empty `StdString`
/// @ingroup std_string
[[nodiscard]] StdString std_string_new(void);
/// @brief Creates a new, empty `StdString` that will use the given allocator
///
/// @param allocator - The allocator to use for memory allocations
///
/// @return an empty `StdString`
/// @ingroup std_string
[[nodiscard]] StdString std_string_new_with_allocator(StdAllocator allocator);
/// @brief Creates a new `StdString` with the given initial capacity
///
/// @param capacity - The initial capacity of the string
///
/// @return a `StdString` with `capacity` initial capacity
/// @ingroup std_string
[[nodiscard]] StdString std_string_new_with_capacity(usize capacity);
/// @brief Creates a new `StdString` with the given initial capacity
///
/// @param capacity - The initial capacity of the string
/// @param allocator - The allocator to use for memory allocations
///
/// @return a `StdString` with `capacity` initial capacity
/// @ingroup std_string
[[nodiscard]] StdString
std_string_new_with_capacity_with_allocator(usize capacity, StdAllocator allocator);
/// @brief Creates a new `StdString` from the given `cstring`
///
/// @param string - The `cstring` to create the `StdString` from
/// @param length - The length of the `cstring`
///
/// @return a `StdString`
/// @ingroup std_string
[[nodiscard]] [[not_null(1)]] StdString
std_string_from_cstring(restrict const_cstring string, usize length)
	std_disable_if(!string, "Can't create a StdString from a nullptr");
/// @brief Creates a new `StdString` from the given `cstring`
///
/// @param string - The `cstring` to create the `StdString` from
/// @param length - The length of the `cstring`
/// @param allocator - The allocator to use for memory allocations
///
/// @return a `StdString`
/// @ingroup std_string
[[nodiscard]] [[not_null(1)]] StdString
std_string_from_cstring_with_allocator(restrict const_cstring string,
									   usize length,
									   StdAllocator allocator)
	std_disable_if(!string, "Can't create a StdString from a nullptr");
/// @brief Creates a new `StdString` from the given `wcstring`
///
/// @param string - The `wcstring` to create the `StdString` from
/// @param length - The length of the `wcstring`
///
/// @return a `StdString`
/// @ingroup std_string
[[nodiscard]] [[not_null(1)]] StdString
std_string_from_wcstring(restrict const_wcstring string, usize length)
	std_disable_if(!string, "Can't create a StdString from a nullptr");
/// @brief Creates a new `StdString` from the given `wcstring`
///
/// @param string - The `wcstring` to create the `StdString` from
/// @param length - The length of the `wcstring`
/// @param allocator - The allocator to use for memory allocations
///
/// @return a `StdString`
/// @ingroup std_string
[[nodiscard]] [[not_null(1)]] StdString
std_string_from_wcstring_with_allocator(restrict const_wcstring string,
										usize length,
										StdAllocator allocator)
	std_disable_if(!string, "Can't create a StdString from a nullptr");
/// @brief Creates a new `StdString` from the given `StdStringView`
///
/// @param view - The string view to create a `StdString` from
///
/// @return a `StdString`
/// @ingroup std_string
[[nodiscard]] [[not_null(1)]] StdString
std_string_from_stringview(const StdStringView* restrict view)
	std_disable_if(!view, "Can't create a StdString from a nullptr");
/// @brief Creates a new `StdString` that will use the given memory allocator, from the given
/// `StdStringView`
///
/// @param view - The `StdStringView` to create the `StdStringView` from
/// @param allocator - The allocator to use for memory allocations
///
/// @return a `StdString`
/// @ingroup std_string
[[nodiscard]] [[not_null(1)]] StdString
std_string_from_stringview_with_allocator(const StdStringView* restrict view,
										  StdAllocator allocator)
	std_disable_if(!view, "Can't create a StdString from a nullptr");
// clang-format off
/// @brief Creates a new `StdString` from the given string-like type
///
/// @param string - The string-like thing to create the `StdString` from. Either a
/// `StdStringView` or a `cstring` (const or non-const)
///
/// @return a `StdString`
/// @ingroup std_string
#define std_string_from(string) _Generic((string), 												   \
	const_cstring 				: 	std_string_from_cstring(static_cast(const_cstring)(string),    \
										strlen(static_cast(const_cstring)(string))), 			   \
	cstring 					: 	std_string_from_cstring(static_cast(cstring)(string), 		   \
										strlen(static_cast(cstring)(string))), 			    	   \
	const char[sizeof(string)]  : 	std_string_from_cstring( 						/** NOLINT **/ \
										static_cast(const_cstring)(string),  					   \
										sizeof(string)), 							/** NOLINT **/ \
	char[sizeof(string)] 		: 	std_string_from_cstring( 						/** NOLINT **/ \
										static_cast(cstring)(string), 							   \
										sizeof(string)), 			  				/** NOLINT **/ \
	const_wcstring 				: 	std_string_from_wcstring(static_cast(const_wcstring)(string),  \
										wcslen(static_cast(const_wcstring)(string))), 			   \
	wcstring 					: 	std_string_from_wcstring(static_cast(wcstring)(string), 	   \
										wcslen(static_cast(wcstring)(string))), 			       \
	const wchar_t[																				   \
		sizeof(string) / 																		   \
		sizeof(wchar_t)]  		: 	std_string_from_wcstring( 						/** NOLINT **/ \
										static_cast(const_wcstring)(string),  					   \
										sizeof(string) / sizeof(wchar_t)), 			/** NOLINT **/ \
	wchar_t[sizeof(string) / 																	   \
		sizeof(wchar_t)] 		: 	std_string_from_wcstring( 						/** NOLINT **/ \
										static_cast(wcstring)(string), 							   \
										sizeof(string) / sizeof(wchar_t)), 			/** NOLINT **/ \
	StdStringView*              : 	std_string_from_stringview( 								   \
										static_cast(StdStringView*)(string)), 					   \
	const StdStringView* 		: 	std_string_from_stringview( 								   \
										static_cast(const StdStringView*)(string)))
/// @brief Creates a new `StdString` from the given string-like type, using the given
/// `StdAllocator` for memory allocations
///
/// @param string - The string-like thing to create the `StdString` from. Either a
/// `StdStringView` or a `cstring` (const or non-const)
/// @param allocator - The allocator to use for memory allocations, if necessary
///
/// @return a `StdString`
/// @ingroup std_string
#define std_string_from_with_allocator(string, allocator) _Generic((string),                       \
	const_cstring 				: 	std_string_from_cstring_with_allocator( 					   \
										static_cast(const_cstring)(string), 				   	   \
										strlen(static_cast(const_cstring)(string)), allocator),    \
	cstring 					: 	std_string_from_cstring_with_allocator( 					   \
										static_cast(cstring)(string), 				    		   \
										strlen(static_cast(cstring)(string)), allocator),      	   \
	const char[sizeof(string)] 	: 	std_string_from_cstring_with_allocator( 		/** NOLINT **/ \
										static_cast(const_cstring)(string), 				   	   \
		 								sizeof(string), allocator), 				/** NOLINT **/ \
	char[sizeof(string)] 		: 	std_string_from_cstring_with_allocator( 		/** NOLINT **/ \
										static_cast(cstring)(string),  							   \
										sizeof(string), allocator), 				/** NOLINT **/ \
	const_wcstring 				: 	std_string_from_wcstring_with_allocator( 					   \
										static_cast(const_wcstring)(string), 				   	   \
										wcslen(static_cast(const_wcstring)(string)), allocator),   \
	wcstring 					: 	std_string_from_wcstring_with_allocator( 					   \
										static_cast(wcstring)(string), 				    		   \
										wcslen(static_cast(wcstring)(string)), allocator),         \
	const wchar_t[ 																				   \
		sizeof(string) / 																		   \
		sizeof(wchar_t)] 		: 	std_string_from_wcstring_with_allocator( 		/** NOLINT **/ \
										static_cast(const_wcstring)(string), 				   	   \
		 								sizeof(string) / sizeof(wchar_t), 			/** NOLINT **/ \
										allocator), 											   \
	wchar_t[sizeof(string) / 																	   \
		sizeof(wchar_t)] 		: 	std_string_from_wcstring_with_allocator( 		/** NOLINT **/ \
										static_cast(wcstring)(string),  						   \
										sizeof(string) / sizeof(wchar_t), 			/** NOLINT **/ \
										allocator), 											   \
	StdStringView* 				: 	std_string_from_stringview_with_allocator( 					   \
										static_cast(StdStringView*)(string), allocator), 		   \
	const StdStringView* 		: 	std_string_from_stringview_with_allocator( 					   \
										static_cast(const StdStringView*)(string), allocator))
// clang-format on

#define ___DISABLE_IF_NULL(self) \
	std_disable_if(!(self), "Can't perform a StdString operation on a nullptr")
/// @brief Returns the `cstring` representation of this `StdString`
///
/// @param self - the string to get the `cstring` representation of
///
/// @return  the `cstring` representation
/// @ingroup std_string
[[nodiscard]] [[not_null(1)]] [[returns_not_null]] const_cstring
std_string_into_cstring(const StdString* restrict self) ___DISABLE_IF_NULL(self);
/// @brief Returns the `wcstring` converted representation of this `StdString`. The result will be
/// allocated with the same allocator used by this `StdString`
///
/// @param self - The string to get the `wcstring` converted representation of
///
/// @return the `wcstring` converted representation
/// @ingroup std_string
[[nodiscard]] [[not_null(1)]] [[returns_not_null]] const_wcstring
std_string_into_wcstring(const StdString* restrict self) ___DISABLE_IF_NULL(self);
/// @brief Returns the `wcstring` converted representation of this `StdString`, allocated with the
/// given allocator;
///
/// @param self - The string to get the `wcstring` converted representation of
/// @param allocator - The allocator to allocate memory for the `wcstring` converted representation
///
/// @return the `wcstring` converted representation
/// @ingroup std_string
[[nodiscard]] [[not_null(1)]] [[returns_not_null]] const_wcstring
std_string_into_wcstring_with_allocator(const StdString* restrict self, StdAllocator allocator)
	___DISABLE_IF_NULL(self);
/// @brief Returns a `StdStringView` into this `StdString`
///
/// @param self - the string to get the `StdStringView` of
///
/// @return  the `StdStringview` into this
/// @ingroup std_string
[[nodiscard]] [[not_null(1)]] StdStringView
std_string_into_stringview(const StdString* restrict self) ___DISABLE_IF_NULL(self);
/// @brief Creates a copy of this `StdString` using the same allocator
///
/// @param self - The `StdString` to copy
///
/// @return a copy of the `StdString`
/// @ingroup std_string
[[nodiscard]] [[not_null(1)]] StdString
std_string_clone(const StdString* restrict self) ___DISABLE_IF_NULL(self);
/// @brief Creates a copy of this `StdString` using the given allocator
///
/// @param self - The `StdString` to copy
/// @param allocator - The allocator to allocate memory with
///
/// @return a copy of the `StdString`
/// @ingroup std_string
[[nodiscard]] [[not_null(1)]] StdString
std_string_clone_with_allocator(const StdString* restrict self, StdAllocator allocator)
	___DISABLE_IF_NULL(self);
/// @brief Frees the allocated memory of the string, if it is not small string optimized
///
/// @param self - The `StdString` to free
/// @ingroup std_string
[[not_null(1)]] void std_string_free(void* restrict self) ___DISABLE_IF_NULL(self);
/// @brief declare a `StdString` variable with this attribute to have `std_string_free`
/// automatically called on it at scope end
#define std_string_scoped scoped(std_string_free)

/// @brief Returns a pointer to the character at the given index.
///
/// @param self - The `StdString` to retrieve the character from
/// @param index - The index to get the character for
///
/// @return The character at the given index
/// @ingroup std_string
[[nodiscard]] [[not_null(1)]] [[returns_not_null]] char_ptr
std_string_at_mut(StdString* restrict self, usize index) ___DISABLE_IF_NULL(self);
/// @brief Returns a pointer to the character at the given index.
///
/// @param self - The `StdString` to retrieve the character from
/// @param index - The index to get the character for
///
/// @return The character at the given index
/// @ingroup std_string
[[nodiscard]] [[not_null(1)]] [[returns_not_null]] const_char_ptr
std_string_at_const(const StdString* restrict self, usize index) ___DISABLE_IF_NULL(self);
/// @brief Returns the character at the beginning of the string
///
/// @param self - The `StdString` to retrieve the first character from
///
/// @return The first character
/// @ingroup std_string
[[nodiscard]] [[not_null(1)]] [[returns_not_null]] char_ptr
std_string_front_mut(StdString* restrict self) ___DISABLE_IF_NULL(self);
/// @brief Returns the character at the beginning of the string
///
/// @param self - The `StdString` to retrieve the first character from
///
/// @return The first character
/// @ingroup std_string
[[nodiscard]] [[not_null(1)]] [[returns_not_null]] const_char_ptr
std_string_front_const(const StdString* restrict self) ___DISABLE_IF_NULL(self);
/// @brief Returns the character at the end of the string
///
/// @param self - The `StdString` to retrieve the last character from
///
/// @return The last character
/// @ingroup std_string
[[nodiscard]] [[not_null(1)]] [[returns_not_null]] char_ptr
std_string_back_mut(StdString* restrict self) ___DISABLE_IF_NULL(self);
/// @brief Returns the character at the end of the string
///
/// @param self - The `StdString` to retrieve the last character from
///
/// @return The last character
/// @ingroup std_string
[[nodiscard]] [[not_null(1)]] [[returns_not_null]] const_char_ptr
std_string_back_const(const StdString* restrict self) ___DISABLE_IF_NULL(self);
/// @brief Returns whether the string is empty or not
///
/// @param self - The `StdString` to check for emptiness
///
/// @return `true` if empty, `false` otherwise
/// @ingroup std_string
[[nodiscard]] [[not_null(1)]] bool
std_string_is_empty(const StdString* restrict self) ___DISABLE_IF_NULL(self);
/// @brief Returns whether the string contains capacity number of characters
///
/// @param self - The `StdString` to check for fullness
///
/// @return `true` if full, `false` otherwise
/// @ingroup std_string
[[nodiscard]] [[not_null(1)]] bool
std_string_is_full(const StdString* restrict self) ___DISABLE_IF_NULL(self);
/// @brief Returns the current size of the string
///
/// @param self - The `StdString` to get the size of
///
/// @return the size of the string
/// @ingroup std_string
[[nodiscard]] [[not_null(1)]] usize
std_string_size(const StdString* restrict self) ___DISABLE_IF_NULL(self);
/// @brief Returns the current length of the string
///
/// @param self - The `StdString` to get the length of
///
/// @return the length of the string
/// @note This is equivalent to `std_string_size`
/// @ingroup std_string
[[nodiscard]] [[not_null(1)]] usize
std_string_length(const StdString* restrict self) ___DISABLE_IF_NULL(self);
/// @brief Returns the maximum possible size of a `StdString`
///
/// @return the maximum possible size
/// @ingroup std_string
[[nodiscard]] usize std_string_max_size(void);
/// @brief Returns the current capacity of the string
///
/// @param self - The `StdString` to get the capacity of
///
/// @return The capacity of the string
/// @ingroup std_string
[[nodiscard]] [[not_null(1)]] usize
std_string_capacity(const StdString* restrict self) ___DISABLE_IF_NULL(self);
/// @brief Returns the first `num_chars` characters in the string as a new `StdString`
///
/// @param self - The `StdString` to get the characters from
/// @param num_chars - The number of characters to get
///
/// @return the first `num_chars` characters
///
/// @note if `num_chars > size` then the returned string will be null-padded with
/// `num_chars - size` null characters
/// @ingroup std_string
[[nodiscard]] [[not_null(1)]] StdString
std_string_first(const StdString* restrict self, usize num_chars) ___DISABLE_IF_NULL(self)
	std_disable_if(num_chars == 0, "Can't get a string of 0 (zero) characters");
/// @brief Returns the first `num_chars` characters in the string as a `cstring` allocated with the
/// allocator associated with `self`
///
/// @param self - The `StdString` to get the characters from
/// @param num_chars - The number of characters to get
///
/// @return the first `num_chars` characters
///
/// @note if `num_chars > size` then the returned string will be null-padded with
/// `num_chars - size` null characters
/// @ingroup std_string
[[nodiscard]] [[not_null(1)]] [[returns_not_null]] cstring
std_string_first_cstring(const StdString* restrict self, usize num_chars) ___DISABLE_IF_NULL(self)
	std_disable_if(num_chars == 0, "Can't get a string of 0 (zero) characters");
/// @brief Returns the first `num_chars` characters in the string as a `StdStringView`
///
/// @param self - The `StdString` to get the characters from
/// @param num_chars - The number of characters to get
///
/// @return the first `num_chars` characters
///
/// @note if `num_chars > size` then the returned `StdStringView`s length will be truncated to
/// `size`
/// @ingroup std_string
[[nodiscard]] [[not_null(1)]] StdStringView
std_string_first_stringview(const StdString* restrict self, usize num_chars)
	___DISABLE_IF_NULL(self)
		std_disable_if(num_chars == 0, "Can't get a string of 0 (zero) characters");
/// @brief Returns the last `num_chars` characters in the string as a new `StdString`
///
/// @param self - The `StdString` to get the characters from
/// @param num_chars - The number of characters to get
///
/// @return the last `num_chars` characters
///
/// @note if `num_chars > size` then the returned string will be null-padded with
/// `num_chars - size` null characters
/// @ingroup std_string
[[nodiscard]] [[not_null(1)]] StdString
std_string_last(const StdString* restrict self, usize num_chars) ___DISABLE_IF_NULL(self)
	std_disable_if(num_chars == 0, "Can't get a string of 0 (zero) characters");
/// @brief Returns the last `num_chars` characters in the string as a `cstring` allocated with the
/// allocator associated with `self`
///
/// @param self - The `StdString` to get the characters from
/// @param num_chars - The number of characters to get
///
/// @return the last `num_chars` characters
///
/// @note if `num_chars > size` then the returned string will be null-padded with
/// `num_chars - size` null characters
/// @ingroup std_string
[[nodiscard]] [[not_null(1)]] [[returns_not_null]] cstring
std_string_last_cstring(const StdString* restrict self, usize num_chars) ___DISABLE_IF_NULL(self)
	std_disable_if(num_chars == 0, "Can't get a string of 0 (zero) characters");
/// @brief Returns the last `num_chars` characters in the string as a `StdStringView`
///
/// @param self - The `StdString` to get the characters from
/// @param num_chars - The number of characters to get
///
/// @return the last `num_chars` characters
///
/// @note if `num_chars > size` then the returned `StdStringView`s length will be truncated to
/// @ingroup std_string
[[nodiscard]] [[not_null(1)]] StdStringView
std_string_last_stringview(const StdString* restrict self, usize num_chars) ___DISABLE_IF_NULL(self)
	std_disable_if(num_chars == 0, "Can't get a string of 0 (zero) characters");
/// @brief Determines if the given strings are equal
///
/// @param self - The first string to compare
/// @param to_compare The second string to compare
///
/// @return `true` if the two strings are equal, `false otherwise`
/// @note if the lengths of the strings are different, they are __always__ unequal
/// @ingroup std_string
[[nodiscard]] [[not_null(1, 2)]] bool
std_string_equal(const StdString* restrict self, const StdString* restrict to_compare)
	___DISABLE_IF_NULL(self) std_disable_if(!to_compare, "Can't compare a StdString to a nullptr");
/// @brief Determines if the given strings are equal
///
/// @param self - The first string to compare
/// @param to_compare - The second string to compare
/// @param length - The length of the second string to compare
///
/// @return `true` if the two strings are equal, `false` otherwise
/// @note if the lengths of the strings are different, they are __always__ unequal
/// @ingroup std_string
[[nodiscard]] [[not_null(1, 2)]] bool std_string_equal_cstring(const StdString* restrict self,
															   restrict const_cstring to_compare,
															   usize length)
	___DISABLE_IF_NULL(self) std_disable_if(!to_compare, "Can't compare a StdString to a nullptr");
/// @brief Determines if the given strings are equal
///
/// @param self - The first string to compare
/// @param to_compare - The second string to compare
///
/// @return `true` if the two strings are equal, `false` otherwise
/// @note if the lengths of the strings are different, they are __always__ unequal
/// @ingroup std_string
[[nodiscard]] [[not_null(1, 2)]] bool
std_string_equal_stringview(const StdString* restrict self,
							const StdStringView* restrict to_compare) ___DISABLE_IF_NULL(self)
	std_disable_if(!to_compare, "Can't compare a StdString to a nullptr");
/// @brief Determines if the string contains the given substring
///
/// @param self - The `StdString` to search for `substring` in
/// @param substring - The substring to search for
///
/// @return `true` if this contains `substring`, `false` otherwise
/// @note if the length of `substring` is greater than the length of `self`,
/// this always returns false
/// @ingroup std_string
[[nodiscard]] [[not_null(1, 2)]] bool
std_string_contains(const StdString* restrict self, const StdString* restrict substring)
	___DISABLE_IF_NULL(self)
		std_disable_if(!substring, "Can't check if a StdString contains a nullptr");
/// @brief Determines if the string contains the given substring
///
/// @param self - The `StdString` to search for `substring` in
/// @param substring - The substring to search for
/// @param substring_length - The length of the substring to search for
///
/// @return `true` if this contains `substring`, `false` otherwise
/// @note if the length of `substring` is greater than the length of `self`,
/// this always returns false
/// @ingroup std_string
[[nodiscard]] [[not_null(1, 2)]] bool
std_string_contains_cstring(const StdString* restrict self,
							restrict const_cstring substring,
							usize substring_length) ___DISABLE_IF_NULL(self)
	std_disable_if(!substring, "Can't check if a StdString contains a nullptr");
/// @brief Determines if the string contains the given substring
///
/// @param self - The `StdString` to search for `substring` in
/// @param substring - The substring to search for
///
/// @return `true` if this contains `substring`, `false` otherwise
/// @note if the length of the `substring` is greater than the length of `self`,
/// this always returns false
/// @ingroup std_string
[[nodiscard]] [[not_null(1, 2)]] bool
std_string_contains_stringview(const StdString* restrict self,
							   const StdStringView* restrict substring) ___DISABLE_IF_NULL(self)
	std_disable_if(!substring, "Can't check if a StdString contains a nullptr");
/// @brief Determines if the string begins with the given substring
///
/// @param self - The `StdString` to search for `substring` in
/// @param substring - The substring to search for
///
/// @return `true` if this begins with `substring`, `false` otherwise
/// @note if the length of `substring` is greater than the length of `self`,
/// this always returns false
/// @ingroup std_string
[[nodiscard]] [[not_null(1, 2)]] bool
std_string_starts_with(const StdString* restrict self, const StdString* restrict substring)
	___DISABLE_IF_NULL(self)
		std_disable_if(!substring, "Can't check if a StdString contains a nullptr");
/// @brief Determines if the string begins with the given substring
///
/// @param self - The `StdString` to search for `substring` in
/// @param substring - The substring to search for
/// @param substring_length - The length of the substring to search for
///
/// @return `true` if this begins with `substring`, `false` otherwise
/// @note if the length of `substring` is greater than the length of `self`,
/// this always returns false
/// @ingroup std_string
[[nodiscard]] [[not_null(1, 2)]] bool
std_string_starts_with_cstring(const StdString* restrict self,
							   restrict const_cstring substring,
							   usize substring_length) ___DISABLE_IF_NULL(self)
	std_disable_if(!substring, "Can't check if a StdString contains a nullptr");
/// @brief Determines if the string begins with the given substring
///
/// @param self - The `StdString` to search for `substring` in
/// @param substring - The substring to search for
///
/// @return `true` if this begins with `substring`, `false` otherwise
/// @note if the length of `substring` is greater than the length of `self`,
/// this always returns false
/// @ingroup std_string
[[nodiscard]] [[not_null(1, 2)]] bool
std_string_starts_with_stringview(const StdString* restrict self,
								  const StdStringView* restrict substring) ___DISABLE_IF_NULL(self)
	std_disable_if(!substring, "Can't check if a StdString contains a nullptr");
/// @brief Determines if the string ends with the given substring
///
/// @param self - The `StdString` to search for `substring` in
/// @param substring - The substring to search for
///
/// @return `true` if this ends with `substring`, `false` otherwise
/// @note if the length of `substring` is greater than the length of `self`,
/// this always returns false
/// @ingroup std_string
[[nodiscard]] [[not_null(1, 2)]] bool
std_string_ends_with(const StdString* restrict self, const StdString* restrict substring)
	___DISABLE_IF_NULL(self)
		std_disable_if(!substring, "Can't check if a StdString contains a nullptr");
/// @brief Determines if the string ends with the given substring
///
/// @param self - The `StdString` to search for `substring` in
/// @param substring - The substring to search for
/// @param substring_length - The length of the substring to search for
///
/// @return `true` if this ends with `substring`, `false` otherwise
/// @note if the length of `substring` is greater than the length of `self`,
/// this always returns false
/// @ingroup std_string
[[nodiscard]] [[not_null(1, 2)]] bool
std_string_ends_with_cstring(const StdString* restrict self,
							 restrict const_cstring substring,
							 usize substring_length) ___DISABLE_IF_NULL(self)
	std_disable_if(!substring, "Can't check if a StdString contains a nullptr");
/// @brief Determines if the string ends with the given substring
///
/// @param self - The `StdString` to search for `substring` in
/// @param substring - The substring to search for
///
/// @return `true` if this ends with `substring`, `false` otherwise
/// @note if the length of `substring` is greater than the length of `self`,
/// this always returns false
/// @ingroup std_string
[[nodiscard]] [[not_null(1, 2)]] bool
std_string_ends_with_stringview(const StdString* restrict self,
								const StdStringView* restrict substring) ___DISABLE_IF_NULL(self)
	std_disable_if(!substring, "Can't check if a StdString contains a nullptr");
/// @brief Returns the index of the first occurrence of the given substring, or `std_option_none` if
/// the substring does not occur
///
/// @param self - The `StdString` to search for `substring` in
/// @param substring - The substring to search for
///
/// @return The index of the first occurrence of `substring`, or `std_option_none`
/// @ingroup std_string
[[nodiscard]] [[not_null(1, 2)]] StdOption(usize)
	std_string_find_first(const StdString* restrict self, const StdString* restrict substring)
		___DISABLE_IF_NULL(self)
			std_disable_if(!substring, "Can't check if a StdString contains a nullptr");
/// @brief Returns the index of the first occurrence of the given substring, or `std_option_none` if
/// the substring does not occur
///
/// @param self - The `StdString` to search for `substring` in
/// @param substring - The substring to search for
/// @param substring_length - The length of the substring to search for
///
/// @return The index of the first occurrence of `substring`, or `std_option_none`
/// @ingroup std_string
[[nodiscard]] [[not_null(1, 2)]] StdOption(usize)
	std_string_find_first_cstring(const StdString* restrict self,
								  restrict const_cstring substring,
								  usize substring_length) ___DISABLE_IF_NULL(self)
		std_disable_if(!substring, "Can't check if a StdString contains a nullptr");
/// @brief Returns the index of the first occurrence of the given substring, or `std_option_none` if
/// the substring does not occur
///
/// @param self - The `StdString` to search for `substring` in
/// @param substring - The substring to search for
///
/// @return The index of the first occurrence of `substring`, or `std_option_none`
/// @ingroup std_string
[[nodiscard]] [[not_null(1, 2)]] StdOption(usize)
	std_string_find_first_stringview(const StdString* restrict self,
									 const StdStringView* restrict substring)
		___DISABLE_IF_NULL(self)
			std_disable_if(!substring, "Can't check if a StdString contains a nullptr");
/// @brief Returns the index of the last occurrence of the given substring, or `std_option_none` if
/// the substring does not occur
///
/// @param self - The `StdString` to search for `substring` in
/// @param substring - The substring to search for
///
/// @return The index of the last occurrence of `substring`, or `std_option_none`
/// @ingroup std_string
[[nodiscard]] [[not_null(1, 2)]] StdOption(usize)
	std_string_find_last(const StdString* restrict self, const StdString* restrict substring)
		___DISABLE_IF_NULL(self)
			std_disable_if(!substring, "Can't check if a StdString contains a nullptr");
/// @brief Returns the index of the last occurrence of the given substring, or `std_option_none` if
/// the substring does not occur
///
/// @param self - The `StdString` to search for `substring` in
/// @param substring - The substring to search for
/// @param substring_length - The length of the substring to search for
///
/// @return The index of the last occurrence of `substring`, or `std_option_none`
/// @ingroup std_string
[[nodiscard]] [[not_null(1, 2)]] StdOption(usize)
	std_string_find_last_cstring(const StdString* restrict self,
								 restrict const_cstring substring,
								 usize substring_length) ___DISABLE_IF_NULL(self)
		std_disable_if(!substring, "Can't check if a StdString contains a nullptr");
/// @brief Returns the index of the last occurrence of the given substring, or `std_option_none` if
/// the substring does not occur
///
/// @param self - The `StdString` to search for `substring` in
/// @param substring - The substring to search for
///
/// @return The index of the last occurrence of `substring`, or `std_option_none`
/// @ingroup std_string
[[nodiscard]] [[not_null(1, 2)]] StdOption(usize)
	std_string_find_last_stringview(const StdString* restrict self,
									const StdStringView* restrict substring)
		___DISABLE_IF_NULL(self)
			std_disable_if(!substring, "Can't check if a StdString contains a nullptr");
/// @brief Returns the length `length` substring beginning at `index`
///
/// @param self - The `StdString` to get the substring from
/// @param index - The index the substring should begin at
/// @param length - The length of the substring to get
///
/// @return the substring beginning at index and ending at length, exclusive
/// (the bounds are [index, length))
///
/// @note if `index + length` is greater than the size of the string, then the substring will be
/// null padded with `(index + length) - size` null characters
/// (its length will effectively be truncated to `size - index`, instead of `length`)
/// @ingroup std_string
[[nodiscard]] [[not_null(1)]] StdString
std_string_substring(const StdString* restrict self, usize index, usize length)
	___DISABLE_IF_NULL(self);
/// @brief Returns the length `length` substring beginning at `index`, allocated with the given
/// allocator
///
/// @param self - The `StdString` to get the substring from
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
/// @ingroup std_string
[[nodiscard]] [[not_null(1)]] StdString
std_string_substring_with_allocator(const StdString* restrict self,
									usize index,
									usize length,
									StdAllocator allocator) ___DISABLE_IF_NULL(self);
/// @brief Returns a `StdStringView` into `self` of the range [index, index + length)
///
/// @param self - The `StdString` to get the view of
/// @param index - The index of the beginning of the view
/// @param length - The length of the view
///
/// @return a `StdStringView` into `self`
/// @note if `index + length` is greater than the size of the string, then the substring will be
/// truncated to a length of `size - index`, instead of `length`
/// @ingroup std_string
[[nodiscard]] [[not_null(1)]] StdStringView
std_string_stringview_of(const StdString* restrict self, usize index, usize length)
	___DISABLE_IF_NULL(self);
/// @brief Concatenates the two strings into a new one, using the allocator associated with
/// `left`, if necessary
///
/// @param left - The left-side string of the concatenation
/// @param right - The right-side string of the concatenation
///
/// @return  The concatenated string
/// @ingroup std_string
[[nodiscard]] [[not_null(1, 2)]] StdString
std_string_concatenate(const StdString* restrict left, const StdString* restrict right)
	___DISABLE_IF_NULL(left) std_disable_if(!right, "Can't concatenate a nullptr to a StdString");
/// @brief Concatenates the two strings into a new one, using the allocator associated with
/// `left`, if necessary
///
/// @param left - The left-side string of the concatenation
/// @param right - The right-side string of the concatenation
/// @param right_length - The length of the right-side string of the concatenation
///
/// @return  The concatenated string
/// @ingroup std_string
[[nodiscard]] [[not_null(1, 2)]] StdString
std_string_concatenate_cstring(const StdString* restrict left,
							   restrict const_cstring right,
							   usize right_length) ___DISABLE_IF_NULL(left)
	std_disable_if(!right, "Can't concatenate a nullptr to a StdString");
/// @brief Concatenates the two strings into a new `StdString`, using the default allocator (malloc)
///
/// @param left - The left (beginning) string to form the concatenation
/// @param left_length - The length of the left string
/// @param right - The right (ending) string to form the concatenation
/// @param right_length - The length of the right string
///
/// @return The concatenated string as a `StdString`
/// @ingroup std_string
[[nodiscard]] [[not_null(1, 3)]] StdString
std_string_concatenate_cstrings(restrict const_cstring left,
								usize left_length,
								restrict const_cstring right,
								usize right_length)
	std_disable_if(!left, "Can't concatenate a nullptr into a StdString")
		std_disable_if(!right, "Can't concatenate a nullptr into a StdString");
/// @brief Concatenates the two strings into a new one, using the allocator associated with
/// `left`, if necessary
///
/// @param left - The left-side string of the concatenation
/// @param right - The right-side string of the concatenation
///
/// @return  The concatenated string
/// @ingroup std_string
[[nodiscard]] [[not_null(1, 2)]] StdString
std_string_concatenate_stringview(const StdString* restrict left,
								  const StdStringView* restrict right)
	std_disable_if(!left, "Can't concatenate a nullptr into a StdString")
		std_disable_if(!right, "Can't concatenate a nullptr into a StdString");
/// @brief Concatenates the two stringviews into a new `StdString`, using the default allocator
/// (malloc)
///
/// @param left - The left-side string of the concatenation
/// @param right - The right-side string of the concatenation
///
/// @return  The concatenated string
/// @ingroup std_string
[[nodiscard]] [[not_null(1, 2)]] StdString
std_string_concatenate_stringviews(const StdStringView* restrict left,
								   const StdStringView* restrict right)
	std_disable_if(!left, "Can't concatenate a nullptr into a StdString")
		std_disable_if(!right, "Can't concatenate a nullptr into a StdString");

/// @brief This function is an __INTENTIONALLY_ undefined function so that improper use of
/// `std_string_concatenate` (passing values of invalid type(s)) will result in a build failure
/// @ingroup std_string
void invalid_types_passed_to_std_string_concatenate(void);

// clang-format off
/// @brief Concatenates the two `cstring`s, `left` and `right` and returns the result as a
/// `StdString`
///
/// @param left - The left-side string of the concatenation
/// @param left_size - The size of `left_size`
/// @param right - The right-side string of the concatenation. Its size will be inferred
///
/// @return The concatenated result as a `StdString`
#define std_string_concatenate_cstrings__(left, left_size, right) _Generic((right),				   \
	const_cstring 				: std_string_concatenate_cstrings(							   	   \
									left,						   								   \
									left_size, 			   	   									   \
									static_cast(const_cstring)(right), 						   	   \
									strlen(static_cast(const_cstring)(right))), 			   	   \
	cstring 					: std_string_concatenate_cstrings(							   	   \
									left,						   								   \
									left_size, 			   	   									   \
									static_cast(cstring)(right), 							   	   \
									strlen(static_cast(cstring)(right))), 			   		   	   \
	const char[sizeof(right)]	: std_string_concatenate_cstrings( 					/** NOLINT **/ \
									left,						   								   \
									left_size, 			   	   									   \
									static_cast(const_cstring)(right), 						   	   \
									sizeof(right)), 								/** NOLINT **/ \
	char[sizeof(right)]			: std_string_concatenate_cstrings(					/** NOLINT **/ \
									left,						   								   \
									left_size, 			   	   									   \
									static_cast(cstring)(right), 							   	   \
									sizeof(right)),									/** NOLINT **/ \
	default 					: invalid_types_passed_to_std_string_concatenate())

/// @brief Concatenates the two `StdStringView`s, `left` and `right` and returns the result as a
/// `StdString`
///
/// @param left - The left-side string of the concatenation
/// @param right - The right-side string of the concatenation
///
/// @return The concatenated result as a `StdString`
#define std_string_concatenate_stringviews__(left, right) _Generic((right), 					   \
	StdStringView*				: std_string_concatenate_stringviews(left,						   \
									static_cast(StdStringView*)(right)),						   \
	const StdStringView* 		: std_string_concatenate_stringviews(left,						   \
									static_cast(const StdStringView*)(right)),					   \
	default						: invalid_types_passed_to_std_string_concatenate())

/// @brief Concatenates the given `StdString`, `left`, with the given string of arbitrary type,
/// `right`, and returns the result as a `StdString`
///
/// @param left - The left-side string of the concatenation
/// @param right - The right-side string of the concatenation
///
/// @return The concatenated result as a `StdString`
#define std_string_concatenate_string__(left, right) _Generic((right), 						   	   \
	const_cstring 				: std_string_concatenate_cstring(							   	   \
									static_cast(StdString*)(left), 						   	   	   \
									static_cast(const_cstring)(right),  				   	   	   \
									strlen(static_cast(const_cstring)(right))), 		   	   	   \
	cstring 					: std_string_concatenate_cstring(							   	   \
									static_cast(StdString*)(left),  					   	   	   \
									static_cast(cstring)(right), 						   	   	   \
									strlen(static_cast(cstring)(right))), 				   	   	   \
	const char[sizeof(right)] 	: std_string_concatenate_cstring(					/** NOLINT **/ \
									static_cast(StdString*)(left), 							   	   \
									static_cast(const_cstring)(right), 					   	   	   \
									sizeof(right)), 								/** NOLINT **/ \
	char[sizeof(right)] 		: std_string_concatenate_cstring( 					/** NOLINT **/ \
									static_cast(StdString*)(left), 							   	   \
									static_cast(cstring)(right), 						   	   	   \
									sizeof(right)), 								/** NOLINT **/ \
	StdStringView* 				: std_string_concatenate_stringview(						   	   \
									static_cast(StdString*)(left), 					   		   	   \
									static_cast(StdStringView*)(right)), 				   	   	   \
	const StdStringView* 		: std_string_concatenate_stringview(						   	   \
									static_cast(StdString*)(left), 			   		   		   	   \
									static_cast(const StdStringView*)(right)), 				   	   \
	StdString* 					: std_string_concatenate(									   	   \
									static_cast(StdString*)(left), 							   	   \
									static_cast(StdString*)(right)), 					   	   	   \
	const StdString* 			: std_string_concatenate(									   	   \
									static_cast(StdString*)(left), 							   	   \
									static_cast(const StdString*)(right)), 						   \
	default						: invalid_types_passed_to_std_string_concatenate())

/// @brief Concatenates the two strings and returns the result as a new `StdString`
///
/// @param left - The string to begin the new string with
/// @param right - The string to end the new string with
///
/// @return the concatenation of the two strings
/// @ingroup std_string
#define std_string_concatenate(left, right) _Generic((left), 									   \
	const_cstring 				: std_string_concatenate_cstrings__( 						   	   \
									static_cast(const_cstring)(left), 						   	   \
									strlen(static_cast(const_cstring)(left)), right),		   	   \
	cstring 					: std_string_concatenate_cstrings__( 						   	   \
									static_cast(cstring)(left), 							   	   \
									strlen(static_cast(cstring)(left)), right),				   	   \
	const char[sizeof(left)] 	: std_string_concatenate_cstrings__( 				/** NOLINT **/ \
									static_cast(const_cstring)(left), 						   	   \
									sizeof(left), right), 							/** NOLINT **/ \
	char[sizeof(left)]			: std_string_concatenate_cstrings__( 				/** NOLINT **/ \
									static_cast(const_cstring)(left), 						   	   \
									sizeof(left), right), 							/** NOLINT **/ \
	StdStringView* 				: std_string_concatenate_stringviews__( 					   	   \
									static_cast(StdStringView*)(left), right),		   		   	   \
	const StdStringView* 		: std_string_concatenate_stringviews__( 					   	   \
									static_cast(const StdStringView*)(left), right),		   	   \
	StdString * 				: std_string_concatenate_string__(						   	   	   \
									static_cast(StdString*)(left), right),				   	   	   \
	const StdString * 			: std_string_concatenate_string__(						   	   	   \
									static_cast(const StdString*)(left), right))
// clang-format on

/// @brief Concatenates the two strings into a new one, using the given allocator
///
/// @param left - The left-side string of the concatenation
/// @param right - The right-side string of the concatenation
/// @param allocator - The allocator to use for memory allocations
///
/// @return  The concatenated string
/// @ingroup std_string
[[nodiscard]] [[not_null(1, 2)]] StdString
std_string_concatenate_with_allocator(const StdString* restrict left,
									  const StdString* restrict right,
									  StdAllocator allocator) ___DISABLE_IF_NULL(left)
	std_disable_if(!right, "Can't concatenate a nullptr to a StdString");
/// @brief Concatenates the two strings into a new one, using the given allocator
///
/// @param left - The left-side string of the concatenation
/// @param right - The right-side string of the concatenation
/// @param right_length - The length of the right-side string of the concatenation
/// @param allocator - The allocator to use for memory allocations
///
/// @return  The concatenated string
/// @ingroup std_string
[[nodiscard]] [[not_null(1, 2)]] StdString
std_string_concatenate_cstring_with_allocator(const StdString* restrict left,
											  restrict const_cstring right,
											  usize right_length,
											  StdAllocator allocator) ___DISABLE_IF_NULL(left)
	std_disable_if(!right, "Can't concatenate a nullptr to a StdString");
/// @brief Concatenates the two strings into a new one, using the given allocator
///
/// @param left - The left-side string of the concatenation
/// @param right - The right-side string of the concatenation
/// @param allocator - The allocator to use for memory allocations
///
/// @return  The concatenated string
/// @ingroup std_string
[[nodiscard]] [[not_null(1, 2)]] StdString
std_string_concatenate_stringview_with_allocator(const StdString* restrict left,
												 const StdStringView* restrict right,
												 StdAllocator allocator) ___DISABLE_IF_NULL(left)
	std_disable_if(!right, "Can't concatenate a nullptr to a StdString");
/// @brief Concatenates the two strings into a new one, using the given allocator
///
/// @param left - The left-side string of the concatenation
/// @param left_length - The length of the left-side string of the concatenation
/// @param right - The right-side string of the concatenation
/// @param right_length - The length of the right-side string of the concatenation
/// @param allocator - The allocator to use for memory allocations
///
/// @return  The concatenated string
/// @ingroup std_string
[[nodiscard]] [[not_null(1, 3)]] StdString
std_string_concatenate_cstrings_with_allocator(restrict const_cstring left,
											   usize left_length,
											   restrict const_cstring right,
											   usize right_length,
											   StdAllocator allocator)
	std_disable_if(!left, "Can't concatenate a nullptr into a StdString")
		std_disable_if(!right, "Can't concatenate a nullptr into a StdString");
/// @brief Concatenates the two strings into a new one, using the given allocator
///
/// @param left - The left-side string of the concatenation
/// @param right - The right-side string of the concatenation
/// @param allocator - The allocator to use for memory allocations
///
/// @return  The concatenated string
/// @ingroup std_string
[[nodiscard]] [[not_null(1, 2)]] StdString
std_string_concatenate_stringviews_with_allocator(const StdStringView* restrict left,
												  const StdStringView* restrict right,
												  StdAllocator allocator)
	std_disable_if(!left, "Can't concatenate a nullptr into a StdString")
		std_disable_if(!right, "Can't concatenate a nullptr into a StdString");

// clang-format off
/// @brief Concatenates the two `cstring`s, `left` and `right` and returns the result as a
/// `StdString`
///
/// @param left - The left-side string of the concatenation
/// @param left_size - The size of `left_size`
/// @param right - The right-side string of the concatenation. Its size will be inferred
/// @param allocator - The `StdAllocator` to use for memory allocation
///
/// @return The concatenated result as a `StdString`
#define std_string_concatenate_cstrings_with_allocator__(left, left_size, right, allocator) 	   \
	_Generic((right), 																			   \
		const_cstring 				: std_string_concatenate_cstrings_with_allocator(			   \
										left,						   							   \
										left_size, 			   	   								   \
										static_cast(const_cstring)(right), 						   \
										strlen(static_cast(const_cstring)(right)),				   \
										allocator), 			   								   \
		cstring 					: std_string_concatenate_cstrings_with_allocator(			   \
										left,						   							   \
										left_size, 			   	   								   \
										static_cast(cstring)(right), 							   \
										strlen(static_cast(cstring)(right)),					   \
										allocator), 			   		   						   \
		const char[sizeof(right)]	: std_string_concatenate_cstrings_with_allocator(/** NOLINT **/ \
										left,						   							   \
										left_size, 			   	   								   \
										static_cast(const_cstring)(right), 						   \
										sizeof(right), 								/** NOLINT **/ \
										allocator), 											   \
		char[sizeof(right)]			: std_string_concatenate_cstrings_with_allocator(/** NOLINT **/ \
										left,						   							   \
										left_size, 			   	   								   \
										static_cast(cstring)(right), 							   \
										sizeof(right), 								/** NOLINT **/ \
										allocator),												   \
		default 					: invalid_types_passed_to_std_string_concatenate())

/// @brief Concatenates the two `StdStringView`s, `left` and `right` and returns the result as a
/// `StdString`
///
/// @param left - The left-side string of the concatenation
/// @param right - The right-side string of the concatenation
/// @param allocator - The `StdAllocator` to use for memory allocation
///
/// @return The concatenated result as a `StdString`
#define std_string_concatenate_stringviews_with_allocator__(left, right, allocator) 			   \
	_Generic((right),						   													   \
		StdStringView*				: std_string_concatenate_stringviews_with_allocator(left,	   \
										static_cast(StdStringView*)(right), allocator),			   \
		const StdStringView* 		: std_string_concatenate_stringviews_with_allocator(left,	   \
										static_cast(const StdStringView*)(right), allocator),	   \
		default						: invalid_types_passed_to_std_string_concatenate())

/// @brief Concatenates the given `StdString`, `left`, with the given string of arbitrary type,
/// `right`, and returns the result as a `StdString`
///
/// @param left - The left-side string of the concatenation
/// @param right - The right-side string of the concatenation
/// @param allocator - The `StdAllocator` to use for memory allocation
///
/// @return The concatenated result as a `StdString`
#define std_string_concatenate_string_with_allocator__(left, right, allocator) 					   \
	_Generic((right), 						   	   												   \
		const_cstring 				: std_string_concatenate_cstring_with_allocator(			   \
										static_cast(StdString*)(left), 						   	   \
										static_cast(const_cstring)(right),  				   	   \
										strlen(static_cast(const_cstring)(right)), 				   \
										allocator), 		   	   	   							   \
		cstring 					: std_string_concatenate_cstring_with_allocator(			   \
										static_cast(StdString*)(left),  					   	   \
										static_cast(cstring)(right), 						   	   \
										strlen(static_cast(cstring)(right)), 					   \
										allocator), 				   	   	   					   \
		const char[sizeof(right)] 	: std_string_concatenate_cstring_with_allocator(/** NOLINT **/ \
										static_cast(StdString*)(left), 							   \
										static_cast(const_cstring)(right), 					   	   \
										sizeof(right), 								/** NOLINT **/ \
										allocator), 											   \
		char[sizeof(right)] 		: std_string_concatenate_cstring_with_allocator(/** NOLINT **/ \
										static_cast(StdString*)(left), 							   \
										static_cast(cstring)(right), 						   	   \
										sizeof(right), 								/** NOLINT **/ \
										allocator), 											   \
		StdStringView* 				: std_string_concatenate_stringview_with_allocator(			   \
										static_cast(StdString*)(left), 					   		   \
										static_cast(StdStringView*)(right), allocator), 		   \
		const StdStringView* 		: std_string_concatenate_stringview_with_allocator(			   \
										static_cast(StdString*)(left), 			   		   		   \
										static_cast(const StdStringView*)(right), allocator), 	   \
		StdString* 					: std_string_concatenate_with_allocator(					   \
										static_cast(StdString*)(left), 							   \
										static_cast(StdString*)(right), allocator), 			   \
		const StdString* 			: std_string_concatenate_with_allocator(					   \
										static_cast(StdString*)(left), 							   \
										static_cast(const StdString*)(right), allocator), 		   \
		default						: invalid_types_passed_to_std_string_concatenate())

/// @brief Concatenates the two strings and returns the result as a new `StdString`
///
/// @param left - The string to begin the new string with
/// @param right - The string to end the new string with
/// @param allocator - The `StdAllocator` to allocate memory with
///
/// @return the concatenation of the two strings
/// @ingroup std_string
#define std_string_concatenate_with_allocator(left, right, allocator) _Generic((left), 			   \
	const_cstring 				: std_string_concatenate_cstrings_with_allocator__( 			   \
									static_cast(const_cstring)(left), 						   	   \
									strlen(static_cast(const_cstring)(left)), right, 			   \
									allocator),		   	   										   \
	cstring 					: std_string_concatenate_cstrings_with_allocator__( 			   \
									static_cast(cstring)(left), 							   	   \
									strlen(static_cast(cstring)(left)), right, 					   \
									allocator),				   	   								   \
	const char[sizeof(left)] 	: std_string_concatenate_cstrings_with_allocator__( /** NOLINT **/ \
									static_cast(const_cstring)(left), 						   	   \
									sizeof(left), right, allocator), 				/** NOLINT **/ \
	char[sizeof(left)]			: std_string_concatenate_cstrings_with_allocator__( /** NOLINT **/ \
									static_cast(const_cstring)(left), 						   	   \
									sizeof(left), right, allocator), 				/** NOLINT **/ \
	StdStringView* 				: std_string_concatenate_stringviews_with_allocator__( 			   \
									static_cast(StdStringView*)(left), right, allocator),		   \
	const StdStringView* 		: std_string_concatenate_stringviews_with_allocator__( 			   \
									static_cast(const StdStringView*)(left), right, allocator),	   \
	StdString * 				: std_string_concatenate_string_with_allocator__(				   \
									static_cast(StdString*)(left), right, allocator),			   \
	const StdString * 			: std_string_concatenate_string_with_allocator__(				   \
									static_cast(const StdString*)(left), right, allocator))
// clang-format on

/// @brief Fills the string with the given character
///
/// @param self - The `StdString` to fill
/// @param character - The character to fill the string with
///
/// @note this fills to capacity, not size
/// @ingroup std_string
[[not_null(1)]] void
std_string_fill(StdString* restrict self, char character) ___DISABLE_IF_NULL(self);
/// @brief Clears the string, filling it with null
///
/// @param self - The `StdString` to clear
///
/// @note this will fill to capacity with null, thus size might increase
/// @ingroup std_string
[[not_null(1)]] void std_string_clear(StdString* restrict self) ___DISABLE_IF_NULL(self);
/// @brief Shrinks the string so its capacity equals its size
///
/// @param self - The `StdString` to shrink
/// @ingroup std_string
[[not_null(1)]] void std_string_shrink_to_fit(StdString* restrict self) ___DISABLE_IF_NULL(self);
/// @brief Inserts the given string `to_insert` into `self` at the given `index`
///
/// @param self - The `StdString` to insert into
/// @param to_insert - The string to insert
/// @param index - The index at which to insert the string
/// @ingroup std_string
[[not_null(1, 2)]] void
std_string_insert(StdString* restrict self, const StdString* restrict to_insert, usize index)
	___DISABLE_IF_NULL(self) std_disable_if(!to_insert, "Can't insert a nullptr into a StdString");
/// @brief Inserts the given string `to_insert` into `self` at the given `index`
///
/// @param self - The `StdString` to insert into
/// @param to_insert - The string to insert
/// @param to_insert_length - The length of the string to insert
/// @param index - The index at which to insert the string
/// @ingroup std_string
[[not_null(1, 2)]] void std_string_insert_cstring(StdString* restrict self,
												  restrict const_cstring to_insert,
												  usize to_insert_length,
												  usize index) ___DISABLE_IF_NULL(self)
	std_disable_if(!to_insert, "Can't insert a nullptr into a StdString");
/// @brief Inserts the given string `to_insert` into `self` at the given `index`
///
/// @param self - The `StdString` to insert into
/// @param to_insert - The string to insert
/// @param index - The index at which to insert the string
/// @ingroup std_string
[[not_null(1, 2)]] void std_string_insert_stringview(StdString* restrict self,
													 const StdStringView* restrict to_insert,
													 usize index) ___DISABLE_IF_NULL(self)
	std_disable_if(!to_insert, "Can't insert a nullptr into a StdString");
/// @brief Erases the character at the given index from the string
///
/// @param self - The `StdString` to erase from
/// @param index - The index of the character to erase
/// @ingroup std_string
[[not_null(1)]] void
std_string_erase(StdString* restrict self, usize index) ___DISABLE_IF_NULL(self);
/// @brief Erases `num_character` characters from the string, starting at the given index
///
/// @param self - The `StdString` to erase from
/// @param index - The index to start erasing at
/// @param num_characters - The number of characters to erase
///
/// @note if `index + num_characters` is out of bounds, this will erase 'size - index' characters
/// @ingroup std_string
[[not_null(1)]] void std_string_erase_n(StdString* restrict self, usize index, usize num_characters)
	___DISABLE_IF_NULL(self);
/// @brief Resizes the string to the new size, null padding or truncating if necessary
///
/// @param self - The `StdString` to resize
/// @param new_size - The new size for the string
/// @ingroup std_string
[[not_null(1)]] void
std_string_resize(StdString* restrict self, usize new_size) ___DISABLE_IF_NULL(self);
/// @brief Reserves additional capacity in the string
/// This will allocate enough memory to store __at least__ `new_capacity` number of characters,
// /but not necessarily exactly `new_capacity` number of characters
///
/// @param self - The `StdString` to reserve additional space for
/// @param new_capacity - The new capacity for the string
///
/// @note if `new_capacity < capacity` then this will do nothing
/// @ingroup std_string
[[not_null(1)]] void
std_string_reserve(StdString* restrict self, usize new_capacity) ___DISABLE_IF_NULL(self);
/// @brief Appends the given character to the end of the string
///
/// @param self - The `StdString` to append to
/// @param character - The character to append
/// @ingroup std_string
[[not_null(1)]] void
std_string_push_back(StdString* restrict self, char character) ___DISABLE_IF_NULL(self);
/// @brief Prepends the given character to the beginning of the string
///
/// @param self - The `StdString` to prepends to
/// @param character - The character to prepends
/// @ingroup std_string
[[not_null(1)]] void
std_string_push_front(StdString* restrict self, char character) ___DISABLE_IF_NULL(self);
/// @brief Removes the last character in the string and returns it
///
/// @param self - The `StdString` to pop the last character from
///
/// @return the last character
///
/// @note if the string is empty, this returns `std_option_none`
/// @ingroup std_string
[[nodiscard]] [[not_null(1)]] StdOption(char) std_string_pop_back(StdString* restrict self)
	___DISABLE_IF_NULL(self);
/// @brief Removes the first character in the string and returns it
///
/// @param self - The `StdString` to pop the first character from
///
/// @return the first character
///
/// @note if the string is empty, this returns `std_option_none`
/// @ingroup std_string
[[nodiscard]] [[not_null(1)]] StdOption(char) std_string_pop_front(StdString* restrict self)
	___DISABLE_IF_NULL(self);
/// @brief Appends the given string to the end of the string
///
/// @param self - The `StdString` to append to
/// @param to_append - The string to append
/// @ingroup std_string
[[not_null(1, 2)]] void
std_string_append(StdString* restrict self, const StdString* restrict to_append)
	___DISABLE_IF_NULL(self) std_disable_if(!to_append, "Can't append a nullptr to a StdString");
/// @brief Appends the given string to the end of the string
///
/// @param self - The `StdString` to append to
/// @param to_append - The string to append
/// @param to_append_length - The length of the string to append
/// @ingroup std_string
[[not_null(1, 2)]] void std_string_append_cstring(StdString* restrict self,
												  restrict const_cstring to_append,
												  usize to_append_length) ___DISABLE_IF_NULL(self)
	std_disable_if(!to_append, "Can't append a nullptr to a StdString");
/// @brief Appends the given string to the end of the string
///
/// @param self - The `StdString` to append to
/// @param to_append - The string to append
/// @ingroup std_string
[[not_null(1, 2)]] void
std_string_append_stringview(StdString* restrict self, const StdStringView* restrict to_append)
	___DISABLE_IF_NULL(self) std_disable_if(!to_append, "Can't append a nullptr to a StdString");
/// @brief Prepends the given string to the beginning of the string
///
/// @param self - The `StdString` to prepend to
/// @param to_prepend - The string to prepend
/// @ingroup std_string
[[not_null(1, 2)]] void
std_string_prepend(StdString* restrict self, const StdString* restrict to_prepend)
	___DISABLE_IF_NULL(self) std_disable_if(!to_prepend, "Can't prepend a nullptr to a StdString");
/// @brief Prepends the given string to the beginning of the string
///
/// @param self - The `StdString` to prepend to
/// @param to_prepend - The string to prepend
/// @param to_prepend_length - The length of the string to prepend
/// @ingroup std_string
[[not_null(1, 2)]] void std_string_prepend_cstring(StdString* restrict self,
												   restrict const_cstring to_prepend,
												   usize to_prepend_length) ___DISABLE_IF_NULL(self)
	std_disable_if(!to_prepend, "Can't prepend a nullptr to a StdString");
/// @brief Prepends the given string to the beginning of the string
///
/// @param self - The `StdString` to prepend to
/// @param to_prepend - The string to prepend
/// @ingroup std_string
[[not_null(1, 2)]] void
std_string_prepend_stringview(StdString* restrict self, const StdStringView* restrict to_prepend)
	___DISABLE_IF_NULL(self) std_disable_if(!to_prepend, "Can't prepend a nullptr to a StdString");
/// @brief Replaces the substring beginning at `index` with the given one
///
/// @param self - The `StdString` to replace a portion of
/// @param to_replace_with - The substring to replace the portion with
/// @param index - The index to start replacement
/// @ingroup std_string
[[not_null(1, 2)]] void
std_string_replace(StdString* restrict self, const StdString* restrict to_replace_with, usize index)
	___DISABLE_IF_NULL(self)
		std_disable_if(!to_replace_with, "Can't replace a portion of a StdString with a nullptr");
/// @brief Replaces the substring beginning at `index` with the given one
///
/// @param self - The `StdString` to replace a portion of
/// @param to_replace_with - The substring to replace the portion with
/// @param to_replace_with_length - The length of the substring to replace the portion with
/// @param index - The index to start replacement
/// @ingroup std_string
[[not_null(1, 2)]] void std_string_replace_cstring(StdString* restrict self,
												   restrict const_cstring to_replace_with,
												   usize to_replace_with_length,
												   usize index) ___DISABLE_IF_NULL(self)
	std_disable_if(!to_replace_with, "Can't replace a portion of a StdString with a nullptr");
/// @brief Replaces the substring beginning at `index` with the given one
///
/// @param self - The `StdString` to replace a portion of
/// @param to_replace_with - The substring to replace the portion with
/// @param index - The index to start replacement
/// @ingroup std_string
[[not_null(1, 2)]] void std_string_replace_stringview(StdString* restrict self,
													  const StdStringView* restrict to_replace_with,
													  usize index) ___DISABLE_IF_NULL(self)
	std_disable_if(!to_replace_with, "Can't replace a portion of a StdString with a nullptr");

/// @brief Implement the Trait `StdRandomAccessIterator` for `StdStringIterator`
DeclIntoStdRandomAccessIterator(StdString, char_ref, std_string_into_iter, into);
/// @brief Implement the Trait `StdRandomAccessIterator` for a reverse `StdStringIterator`
DeclIntoStdRandomAccessIterator(StdString, char_ref, std_string_into_reverse_iter, into_reverse);

/// @brief Implement the Trait `StdRandomAccessIterator` for `StdStringConstIterator`
DeclIntoStdRandomAccessIterator(StdString, const_char_ref, std_string_into_const_iter, into);
/// @brief Implement the Trait `StdRandomAccessIterator` for a reverse `StdStringConstIterator`
DeclIntoStdRandomAccessIterator(StdString,
								const_char_ref,
								std_string_into_reverse_const_iter,
								into_reverse);

/// @brief Returns a `StdRandomAccessIterator(char_ref)` into the given
/// `StdString`, at the beginning of the iteration (pointing at the beginning of the string)
///
/// @param self - The `StdString` to get an iterator into
///
/// @return an iterator at the beginning of the iteration (pointing at the beginning of the
/// string)
/// @ingroup std_string
[[nodiscard]] [[not_null(1)]] StdRandomAccessIteratorchar_ref
std_string_begin(StdString* restrict self) ___DISABLE_IF_NULL(self);
/// @brief Returns a `StdRandomAccessIterator(char_ref)` into the given
/// `StdString`, at the end of the iteration (pointing at the end of the string)
///
/// @param self - The `StdString` to get an iterator into
///
/// @return an iterator at the end of the iteration (pointing at the end of the
/// string)
/// @ingroup std_string
[[nodiscard]] [[not_null(1)]] StdRandomAccessIteratorchar_ref
std_string_end(StdString* restrict self) ___DISABLE_IF_NULL(self);
/// @brief Returns a `StdRandomAccessIterator(char_ref)` into the given
/// `StdString`, at the beginning of the reversed iteration (pointing at the end of the string)
///
/// @param self - The `StdString` to get an iterator into
///
/// @return an iterator at the beginning of the reversed iteration (pointing at the end of the
/// string)
/// @ingroup std_string
[[nodiscard]] [[not_null(1)]] StdRandomAccessIteratorchar_ref
std_string_rbegin(StdString* restrict self) ___DISABLE_IF_NULL(self);
/// @brief Returns a `StdRandomAccessIterator(char_ref)` into the given
/// `StdString`, at the end of the reversed iteration (pointing at the beginning of the string)
///
/// @param self - The `StdString` to get an iterator into
///
/// @return an iterator at the end of the reversed iteration (pointing at the beginning of the
/// string)
/// @ingroup std_string
[[nodiscard]] [[not_null(1)]] StdRandomAccessIteratorchar_ref
std_string_rend(StdString* restrict self) ___DISABLE_IF_NULL(self);
/// @brief Returns a `StdRandomAccessIterator(const_char_ref)` into the given
/// `StdString`, at the beginning of the iteration (pointing at the beginning of the string)
///
/// @param self - The `StdString` to get an iterator into
///
/// @return an iterator at the beginning of the iteration (pointing at the beginning of the
/// string)
/// @ingroup std_string
[[nodiscard]] [[not_null(1)]] StdRandomAccessIteratorconst_char_ref
std_string_cbegin(const StdString* restrict self) ___DISABLE_IF_NULL(self);
/// @brief Returns a `StdRandomAccessIterator(const_char_ref)` into the given
/// `StdString`, at the end of the iteration (pointing at the end of the string)
///
/// @param self - The `StdString` to get an iterator into
///
/// @return an iterator at the end of the iteration (pointing at the end of the
/// string)
/// @ingroup std_string
[[nodiscard]] [[not_null(1)]] StdRandomAccessIteratorconst_char_ref
std_string_cend(const StdString* restrict self) ___DISABLE_IF_NULL(self);
/// @brief Returns a `StdRandomAccessIterator(const_char_ref)` into the given
/// `StdString`, at the beginning of the reversed iteration (pointing at the end of the string)
///
/// @param self - The `StdString` to get an iterator into
///
/// @return an iterator at the beginning of the reversed iteration (pointing at the end of the
/// string)
/// @ingroup std_string
[[nodiscard]] [[not_null(1)]] StdRandomAccessIteratorconst_char_ref
std_string_crbegin(const StdString* restrict self) ___DISABLE_IF_NULL(self);
/// @brief Returns a `StdRandomAccessIterator(const_char_ref)` into the given
/// `StdString`, at the end of the reversed iteration (pointing at the beginning of the string)
///
/// @param self - The `StdString` to get an iterator into
///
/// @return an iterator at the end of the reversed iteration (pointing at the beginning of the
/// string)
/// @ingroup std_string
[[nodiscard]] [[not_null(1)]] StdRandomAccessIteratorconst_char_ref
std_string_crend(const StdString* restrict self) ___DISABLE_IF_NULL(self);

/// @brief Returns a new `StdStringView` into the given `StdString`
///
/// @param string - The `StdString` to get the view of
///
/// @return a `StdStringView` into the given string
/// @ingroup std_stringview
[[nodiscard]] [[not_null(1)]] StdStringView
std_stringview_new(const StdString* restrict string) ___DISABLE_IF_NULL(string);

#undef ___DISABLE_IF_NULL
#define ___DISABLE_IF_NULL(self) \
	std_disable_if(!(self), "Can't perform a stringview operation on a nullptr")
/// @brief Returns a new `StdStringView` into the given cstring
///
/// @param string - The cstring to get the view of
/// @param index - The index to begin the view at
/// @param length - The length of the view
///
/// @return a `StdStringView` into the given string
/// @ingroup std_stringview
[[nodiscard]] [[not_null(1)]] StdStringView
std_stringview_from(restrict const_cstring string, usize index, usize length)
	std_disable_if(!string, "Can't create a stringview from a nullptr");
/// @brief Returns the character in the view located at the given index
///
/// @param self - The `StdStringView` to retrieve a character from
/// @param index - The index of the character to get
///
/// @return the character at index
/// @ingroup std_stringview
[[nodiscard]] [[not_null(1)]] const_char_ptr
std_stringview_at(const StdStringView* restrict self, usize index) ___DISABLE_IF_NULL(self);
/// @brief Returns the length of the `StdStringView`
///
/// @param self - The `StdStringView` to get the length of
///
/// @return the length of the stringview
/// @ingroup std_stringview
[[nodiscard]] [[not_null(1)]] usize
std_stringview_length(const StdStringView* restrict self) ___DISABLE_IF_NULL(self);
/// @brief Returns the size of the `StdStringView`
///
/// @param self - The `StdStringView` to get the size of
///
/// @return the size of the stringview
/// @ingroup std_stringview
[[nodiscard]] [[not_null(1)]] usize
std_stringview_size(const StdStringView* restrict self) ___DISABLE_IF_NULL(self);

/// @brief Returns whether the given `StdStringView`s are equivalent
///
/// @param self - The first `StdStringView` to compare
/// @param to_compare - The `StdStringView` to compare to
///
/// @return whether the stringviews are equal
/// @ingroup std_stringview
[[nodiscard]] [[not_null(1, 2)]] bool
std_stringview_equal(const StdStringView* restrict self, const StdStringView* restrict to_compare)
	___DISABLE_IF_NULL(self) std_disable_if(!to_compare, "Can't compare a stringview to a nullptr");

/// @brief Returns whether the given `StdStringView`s are equivalent
///
/// @param self - The first `StdStringView` to compare
/// @param to_compare - The `StdStringView` to compare to
///
/// @return whether the stringviews are equal
/// @ingroup std_stringview
[[nodiscard]] [[not_null(1, 2)]] bool
std_stringview_equal_string(const StdStringView* restrict self,
							const StdString* restrict to_compare) ___DISABLE_IF_NULL(self)
	std_disable_if(!to_compare, "Can't compare a stringview to a nullptr");

/// @brief Returns whether the given `StdStringView` is equivalent to the given `cstring`
///
/// @param self - The first `StdStringView` to compare
/// @param to_compare - The `cstring` to compare to
/// @param to_compare_length - The length of the `cstring`
///
/// @return whether `self` is equalivalent to the given `cstring`
/// @ingroup std_stringview
[[nodiscard]] [[not_null(1, 2)]] bool
std_stringview_equal_cstring(const StdStringView* restrict self,
							 restrict const_cstring to_compare,
							 usize to_compare_length) ___DISABLE_IF_NULL(self)
	std_disable_if(!to_compare, "Can't compare a stringview to a nullptr");

/// @brief Returns a `const_cstring` (`const char*`) representation of the `StdStringView`
///
/// @param self - The `StdStringView` to get the `const_cstring` representation of
///
/// @return the `const_cstring` representation of the stringview
/// @ingroup std_stringview
[[nodiscard]] [[not_null(1)]] [[returns_not_null]] const_cstring
std_stringview_into_cstring(const StdStringView* restrict self) ___DISABLE_IF_NULL(self);

/// @brief Implement the Trait `StdRandomAccessIterator` for `StdStringViewIterator`
DeclIntoStdRandomAccessIterator(StdStringView, const_char_ref, std_stringview_into_iter, into);
/// @brief Implement the Trait `StdRandomAccessIterator` for `StdStringViewIterator`
DeclIntoStdRandomAccessIterator(StdStringView,
								const_char_ref,
								std_stringview_into_reverse_iter,
								into_reverse);

/// @brief Returns a `StdRandomAccessIterator(const_char_ref)` into the given
/// `StdStringView`, at the beginning of the iteration (pointing at the beginning of the view)
///
/// @param self - The `StdStringView` to get an iterator into
///
/// @return an iterator at the beginning of the iteration (pointing at the beginning of the view)
/// @ingroup std_stringview
[[nodiscard]] [[not_null(1)]] StdRandomAccessIteratorconst_char_ref
std_stringview_begin(const StdStringView* restrict self) ___DISABLE_IF_NULL(self);
/// @brief Returns a `StdRandomAccessIterator(const_char_ref)` into the given
/// `StdStringView`, at the end of the iteration (pointing at the end of the view)
///
/// @param self - The `StdStringView` to get an iterator into
///
/// @return an iterator at the end of the iteration (pointing at the end of the view)
/// @ingroup std_stringview
[[nodiscard]] [[not_null(1)]] StdRandomAccessIteratorconst_char_ref
std_stringview_end(const StdStringView* restrict self) ___DISABLE_IF_NULL(self);
/// @brief Returns a `StdRandomAccessIterator(const_char_ref)` into the given
/// `StdStringView` in reverse, at the beginning of the iteration (pointing at the end of the view)
///
/// @param self - The `StdStringView` to get an iterator into
///
/// @return an iterator at the beginning of the reverse iteration (pointing at the end of the view)
/// @ingroup std_stringview
[[nodiscard]] [[not_null(1)]] StdRandomAccessIteratorconst_char_ref
std_stringview_rbegin(const StdStringView* restrict self) ___DISABLE_IF_NULL(self);
/// @brief Returns a `StdRandomAccessIterator(const_char_ref)` into the given
/// `StdStringView` in reverse, at the end of the iteration (pointing at the beginning of the view)
///
/// @param self - The `StdStringView` to get an iterator into
///
/// @return an iterator at the end of the reverse iteration (pointing at the beginning of the view)
/// @ingroup std_stringview
[[nodiscard]] [[not_null(1)]] StdRandomAccessIteratorconst_char_ref
std_stringview_rend(const StdStringView* restrict self) ___DISABLE_IF_NULL(self);

/// @brief Returns the `cstring` representation of the given `StdString`
///
/// @param self - The `StdString` to get the `cstring` view of
///
/// @return The `cstring` representation of `self`
/// @ingroup std_string
#define std_string_into_cstring(self) std_string_into_cstring(&(self))
/// @brief Returns the `wcstring` representation of the given `StdString`
///
/// @param self - The `StdString` to get the `wcstring` representation of
///
/// @return The `wcstring` representation of `self`
/// @ingroup std_string
#define std_string_into_wcstring(self) std_string_into_wcstring(&(self))
/// @brief Returns the `wcstring` representation of the given `StdString`
///
/// @param self - The `StdString` to get the `wcstring` representation of
/// @param allocator - The `StdAllocator` to allocator memory for the `wcstring` with
///
/// @return The `wcstring` representation of `self`
/// @ingroup std_string
#define std_string_into_wcstring_with_allocator(self, allocator) \
	std_string_into_wcstring_with_allocator(&(self), (allocator))
/// @brief Returns a `StdStringView` into self
///
/// @param self - The `StdString` to get the `StdStringView` of
///
/// @return a `StdStringView` into `self`
/// @ingroup std_string
#define std_string_into_stringview(self) std_string_into_stringview(&(self))
/// @brief Creates a copy of the given `StdString`, `self`, using the `StdAllocator` associated
/// with `self`
///
/// @param self - The `StdString` to copy
///
/// @return a copy of `self`
/// @ingroup std_string
#define std_string_clone(self) std_string_clone(&(self))
/// @brief Creates a copy of the given `StdString`, `self`, using the given `StdAllocator`
///
/// @param self - The `StdString` to copy
/// @param allocator - The `StdAllocator` to associate with the new string
///
/// @return a copy of `self`
/// @ingroup std_string
#define std_string_clone_with_allocator(self, allocator) \
	std_string_clone_with_allocator(&(self), (allocator))
/// @brief Cleans up the given `StdString`, freeing any allocated memory
///
/// @param self - The `StdString` to cleanup
/// @ingroup std_string
#define std_string_free(self) std_string_free(&(self))
// clang-format off
/// @brief Returns a reference to the character at the given index.
///
/// @param self - The `StdString` to retrieve the character from
/// @param index - The index to get the character for
///
/// @return The character at the given index
/// @ingroup std_string
#define std_string_at(self, index) *(_Generic((&(self)), 			\
	StdString* 			: std_string_at_mut,  						\
	const StdString* 	: std_string_at_const)(&(self), (index)))
// clang-format on
// clang-format off
/// @brief Returns a reference to the character at the beginning of the string
///
/// @param self - The `StdString` to retrieve the first character from
///
/// @return The first character
/// @ingroup std_string
#define std_string_front(self) *(_Generic((&(self)), 			\
	StdString*  		: std_string_front_mut, 				\
	const StdString* 	: std_string_front_const)(&(self)))
// clang-format on
// clang-format off
/// @brief Returns a reference to the character at the end of the string
///
/// @param self - The `StdString` to retrieve the last character from
///
/// @return The last character
/// @ingroup std_string
#define std_string_back(self) *(_Generic((&(self)), 		\
	StdString* 			: std_string_back_mut, 				\
	const StdString* 	: std_string_back_const)(&(self)))
// clang-format on

/// @brief Returns whether the given `StdString` is empty or not
///
/// @param self - The `StdString` to check if empty
///
/// @return `true` if `self` is empty, false otherwise
/// @ingroup std_string
#define std_string_is_empty(self) std_string_is_empty(&(self))
/// @brief Returns whether the given `StdString` is full
///
/// @param self - The `StdString` to check if full
///
/// @return `true` if `self` is full, false otherwise
/// @ingroup std_string
#define std_string_is_full(self) std_string_is_full(&(self))
/// @brief Returns the current size of the given `StdString`
///
/// @param self - The `StdString` to get the size of
///
/// @return the size of `self`
/// @ingroup std_string
#define std_string_size(self) std_string_size(&(self))
/// @brief Returns the current length of the given `StdString`
///
/// @param self - The `StdString` to get the length of
///
/// @return the length of `self`
/// @ingroup std_string
#define std_string_length(self) std_string_length(&(self))
/// @brief Returns the current capacity of the given `StdString`
///
/// @param self - The `StdString` to get the capacity of
///
/// @return the capacity of `self`
/// @ingroup std_string
#define std_string_capacity(self) std_string_capacity(&(self))
/// @brief Returns the first `num_chars` characters of `self` as a new `StdString`, using
/// the `StdAllocator` associated with `self`
///
/// @param self - The `StdString` to get the first `num_chars` characters from
/// @param num_chars - The number of chars to get from the beginning of `self`
///
/// @return the first `num_chars` characters of `self` as a `StdString`
/// @ingroup std_string
#define std_string_first(self, num_chars) std_string_first(&(self), (num_chars))
/// @brief Returns the first `num_chars` characters of `self` as a new `cstring`, using
/// the `StdAllocator` associated with `self`
///
/// @param self - The `StdString` to get the first `num_chars` characters from
/// @param num_chars - The number of chars to get from the beginning of `self`
///
/// @return the first `num_chars` characters of `self` as a `cstring`
/// @ingroup std_string
#define std_string_first_cstring(self, num_chars) std_string_first_cstring(&(self), (num_chars))
/// @brief Returns the first `num_chars` characters of `self` as a `StdStringView`
///
/// @param self - The `StdString` to get the first `num_chars` characters from
/// @param num_chars - The number of chars to get from the beginning of `self`
///
/// @return the first `num_chars` characters of `self` as a `StdStringView`
/// @ingroup std_string
#define std_string_first_stringview(self, num_chars) \
	std_string_first_stringview(&(self), (num_chars))
/// @brief Returns the last `num_chars` characters of `self` as a new `StdString`, using
/// the `StdAllocator` associated with `self`
///
/// @param self - The `StdString` to get the last `num_chars` characters from
/// @param num_chars - The number of chars to get from the end of `self`
///
/// @return the last `num_chars` characters of `self` as a `StdString`
/// @ingroup std_string
#define std_string_last(self, num_chars) std_string_last(&(self), (num_chars))
/// @brief Returns the last `num_chars` characters of `self` as a new `cstring`, using
/// the `StdAllocator` associated with `self`
///
/// @param self - The `StdString` to get the last `num_chars` characters from
/// @param num_chars - The number of chars to get from the end of `self`
///
/// @return the last `num_chars` characters of `self` as a `cstring`
/// @ingroup std_string
#define std_string_last_cstring(self, num_chars) std_string_last_cstring(&(self), (num_chars))
/// @brief Returns the last `num_chars` characters of `self` as a `StdStringView`
///
/// @param self - The `StdString` to get the last `num_chars` characters from
/// @param num_chars - The number of chars to get from the end of `self`
///
/// @return the last `num_chars` characters of `self` as a `StdStringView`
/// @ingroup std_string
#define std_string_last_stringview(self, num_chars) std_string_last_stringview(&(self), (num_chars))
// clang-format off
/// @brief Determines if this string and the given one are equal
///
/// @param self - The `StdString` to be compared with
/// @param to_compare - The string to compare to
///
/// @return `true` if the two strings are equal, `false` otherwise
/// @ingroup std_string
#define std_string_equal(self, to_compare) _Generic((to_compare),                       		   \
	const_cstring 					: 	std_string_equal_cstring(&(self), 					   	   \
											static_cast(const_cstring)(to_compare), 			   \
											strlen(static_cast(const_cstring)(to_compare))),       \
	cstring 						: 	std_string_equal_cstring(&(self), 				   	   	   \
											static_cast(cstring)(to_compare), 				       \
											strlen(static_cast(cstring)(to_compare))),      	   \
	const char[sizeof(to_compare)] 	: 	std_string_equal_cstring(&(self), 			/** NOLINT **/ \
											static_cast(const_cstring)(to_compare), 			   \
											sizeof(to_compare)), 					/** NOLINT **/ \
	char[sizeof(to_compare)] 		: 	std_string_equal_cstring(&(self), 			/** NOLINT **/ \
											static_cast(cstring)(to_compare),  					   \
											sizeof(to_compare)), 					/** NOLINT **/ \
	StdStringView* 					: 	std_string_equal_stringview(&(self), 			    	   \
											static_cast(StdStringView*)(to_compare)), 		   	   \
	const StdStringView* 			: 	std_string_equal_stringview(&(self), 			   	   	   \
											static_cast(const StdStringView*)(to_compare)), 	   \
	StdString* 						: 	std_string_equal(&(self), 							   	   \
											static_cast(StdString*)(to_compare)),   			   \
	const StdString* 				: 	std_string_equal(&(self), 							   	   \
											static_cast(const StdString*)(to_compare)))
// clang-format on
// clang-format off
/// @brief Determines whether the string contains the given substring
///
/// @param self - The `StdString` to search for `substring` in
/// @param substring - The substring to search for
///
/// @return `true` if the string contains `substring`, otherwise `false`
/// @ingroup std_string
#define std_string_contains(self, substring) _Generic((substring), 								   \
	const_cstring 					: std_string_contains_cstring(&(self), 				   		   \
										static_cast(const_cstring)(substring),  				   \
										strlen(static_cast(const_cstring)(substring))), 		   \
	cstring 						: std_string_contains_cstring(&(self),  				   	   \
										static_cast(cstring)(substring), 						   \
										strlen(static_cast(cstring)(substring))), 				   \
	const char[sizeof(substring)] 	: std_string_contains_cstring(&(self), 			/** NOLINT **/ \
										static_cast(const_cstring)(substring), 					   \
										sizeof(substring)), 						/** NOLINT **/ \
	char[sizeof(substring)] 		: std_string_contains_cstring(&(self), 			/** NOLINT **/ \
										static_cast(cstring)(substring), 						   \
										sizeof(substring)), 						/** NOLINT **/ \
	StdStringView* 					: std_string_contains_stringview(&(self), 			   		   \
										static_cast(StdStringView*)(substring)), 				   \
	const StdStringView* 			: std_string_contains_stringview(&(self), 			   		   \
										static_cast(const StdStringView*)(substring)), 			   \
	StdString* 						: std_string_contains(&(self), 						   		   \
										static_cast(StdString*)(substring)), 					   \
	const StdString* 				: std_string_contains(&(self), 						   		   \
										static_cast(const StdString*)(substring)))
// clang-format on
// clang-format off
/// @brief Determines whether the string starts with the given substring
///
/// @param self - The `StdString` to search for `substring` in
/// @param substring - The substring to search for
///
/// @return `true` if the string starts with `substring`, otherwise `false`
/// @ingroup std_string
#define std_string_starts_with(self, substring) _Generic((substring), 							   \
	const_cstring 					: std_string_starts_with_cstring(&(self), 			   		   \
										static_cast(const_cstring)(substring),  				   \
										strlen(static_cast(const_cstring)(substring))), 		   \
	cstring 						: std_string_starts_with_cstring(&(self),  			   		   \
										static_cast(cstring)(substring), 						   \
										strlen(static_cast(cstring)(substring))), 				   \
	const char[sizeof(substring)] 	: std_string_starts_with_cstring(&(self), 		/** NOLINT **/ \
										static_cast(const_cstring)(substring), 					   \
										sizeof(substring)), 						/** NOLINT **/ \
	char[sizeof(substring)] 		: std_string_starts_with_cstring(&(self), 		/** NOLINT **/ \
										static_cast(cstring)(substring), 						   \
										sizeof(substring)), 						/** NOLINT **/ \
	StdStringView* 					: std_string_starts_with_stringview(&(self), 			   	   \
										static_cast(StdStringView*)(substring)), 				   \
	const StdStringView* 			: std_string_starts_with_stringview(&(self), 			   	   \
										static_cast(const StdStringView*)(substring)), 			   \
	StdString* 						: std_string_starts_with(&(self), 					   		   \
										static_cast(StdString*)(substring)), 					   \
	const StdString* 				: std_string_starts_with(&(self), 					   		   \
										static_cast(const StdString*)(substring)))
// clang-format on
// clang-format off
/// @brief Determines whether the string ends with the given substring
///
/// @param self - The `StdString` to search for `substring` in
/// @param substring - The substring to search for
///
/// @return `true` if the string ends with `substring`, otherwise `false`
/// @ingroup std_string
#define std_string_ends_with(self, substring) _Generic((substring), 							   \
	const_cstring 					: std_string_ends_with_cstring(&(self), 				   	   \
										static_cast(const_cstring)(substring),  				   \
										strlen(static_cast(const_cstring)(substring))), 		   \
	cstring 						: std_string_ends_with_cstring(&(self),  				   	   \
										static_cast(cstring)(substring), 						   \
										strlen(static_cast(cstring)(substring))), 				   \
	const char[sizeof(substring)] 	: std_string_ends_with_cstring(&(self), 		/** NOLINT **/ \
										static_cast(const_cstring)(substring), 					   \
										sizeof(substring)), 						/** NOLINT **/ \
	char[sizeof(substring)] 		: std_string_ends_with_cstring(&(self), 		/** NOLINT **/ \
										static_cast(cstring)(substring), 						   \
										sizeof(substring)), 						/** NOLINT **/ \
	StdStringView* 					: std_string_ends_with_stringview(&(self), 			   		   \
										static_cast(StdStringView*)(substring)), 				   \
	const StdStringView* 			: std_string_ends_with_stringview(&(self), 			   		   \
										static_cast(const StdStringView*)(substring)), 			   \
	StdString* 						: std_string_ends_with(&(self), 						   	   \
										static_cast(StdString*)(substring)), 					   \
	const StdString* 				: std_string_ends_with(&(self), 						   	   \
										static_cast(const StdString*)(substring)))
// clang-format on
// clang-format off
/// @brief Finds the first occurrence of the given substring in `self`, if any
///
/// @param self - The `StdString` to search for `substring` in
/// @param substring - The substring to search for
///
/// @return `std_option_some` of the index into `self` at the beginning of the substring's
/// occurrence in `self`, if `self` contains the substring, otherwise `std_option_none`
/// @ingroup std_string
#define std_string_find_first(self, substring) _Generic((substring), 							   \
	const_cstring 					: std_string_find_first_cstring(&(self), 				   	   \
										static_cast(const_cstring)(substring),  				   \
										strlen(static_cast(const_cstring)(substring))), 		   \
	cstring 						: std_string_find_first_cstring(&(self),  			   		   \
										static_cast(cstring)(substring), 						   \
										strlen(static_cast(cstring)(substring))), 				   \
	const char[sizeof(substring)] 	: std_string_find_first_cstring(&(self), 		/** NOLINT **/ \
										static_cast(const_cstring)(substring), 					   \
										sizeof(substring)), 						/** NOLINT **/ \
	char[sizeof(substring)] 		: std_string_find_first_cstring(&(self), 		/** NOLINT **/ \
										static_cast(cstring)(substring), 						   \
										sizeof(substring)), 						/** NOLINT **/ \
	StdStringView* 					: std_string_find_first_stringview(&(self), 			   	   \
										static_cast(StdStringView*)(substring)), 				   \
	const StdStringView* 			: std_string_find_first_stringview(&(self), 			   	   \
										static_cast(const StdStringView*)(substring)), 			   \
	StdString* 						: std_string_find_first(&(self), 						   	   \
										static_cast(StdString*)(substring)), 					   \
	const StdString* 				: std_string_find_first(&(self), 						   	   \
										static_cast(const StdString*)(substring)))
// clang-format on
// clang-format off
/// @brief Finds the last occurrence of the given substring in `self`, if any
///
/// @param self - The `StdString` to search for `substring` in
/// @param substring - The substring to search for
///
/// @return `std_option_some` of the index into `self` at the beginning of the substring's
/// occurrence in `self`, if `self` contains the substring, otherwise `std_option_none`
/// @ingroup std_string
#define std_string_find_last(self, substring) _Generic((substring), 							   \
	const_cstring 					: std_string_find_last_cstring(&(self), 				   	   \
										static_cast(const_cstring)(substring),  				   \
										strlen(static_cast(const_cstring)(substring))), 		   \
	cstring 						: std_string_find_last_cstring(&(self),  				   	   \
										static_cast(cstring)(substring), 						   \
										strlen(static_cast(cstring)(substring))), 				   \
	const char[sizeof(substring)] 	: std_string_find_last_cstring(&(self), 		/** NOLINT **/ \
										static_cast(const_cstring)(substring), 					   \
										sizeof(substring)), 						/** NOLINT **/ \
	char[sizeof(substring)] 		: std_string_find_last_cstring(&(self), 		/** NOLINT **/ \
										static_cast(cstring)(substring), 						   \
										sizeof(substring)), 						/** NOLINT **/ \
	StdStringView* 					: std_string_find_last_stringview(&(self), 			   		   \
										static_cast(StdStringView*)(substring)), 				   \
	const StdStringView* 			: std_string_find_last_stringview(&(self), 			   		   \
										static_cast(const StdStringView*)(substring)), 			   \
	StdString* 						: std_string_find_last(&(self), 						   	   \
										static_cast(StdString*)(substring)), 					   \
	const StdString* 				: std_string_find_last(&(self), 						   	   \
										static_cast(const StdString*)(substring)))
// clang-format on

/// @brief Returns the substring of `self` starting at `index` with length `length` as a
/// `StdString`, using the `StdAllocator` associated with `self`
///
/// @param self - The `StdString` to get a substring from
/// @param index - The index to start the substring at
/// @param length - The length of the substring
///
/// @return a substring of `self`
/// @ingroup std_string
#define std_string_substring(self, index, length) std_string_substring(&(self), index, length)
/// @brief Returns the substring of `self` starting at `index` with length `length` as a
/// `StdString`, associating the given `StdAllocator` with the substring
///
/// @param self - The `StdString` to get a substring from
/// @param index - The index to start the substring at
/// @param length - The length of the substring
/// @param allocator - The `StdAllocator` to associate with the substring
///
/// @return a substring of `self`
/// @ingroup std_string
#define std_string_substring_with_allocator(self, index, length, allocator) \
	std_string_substring_with_allocator(&(self), index, length, allocator)
/// @brief Returns a `StdStringView` into `self` beginning at `index` with length `length`
///
/// @param self - The `StdString` to get a `StdStringView` into
/// @param index - The index to begin the view at
/// @param length - The length of the view
///
/// @return a `StdStringView` into self
/// @ingroup std_string
#define std_string_stringview_of(self, index, length) \
	std_string_stringview_of(&(self), index, length)
/// @brief Fills the given `StdString` with the given character
///
/// @param self - The `StdString` to fill
/// @param character - The character to fill `self` with
/// @ingroup std_string
#define std_string_fill(self, character) std_string_fill(&(self), character)
/// @brief Clears the given `StdString`
///
/// @param self - The `StdString` to clear
/// @ingroup std_string
#define std_string_clear(self) std_string_clear(&(self))
/// @brief Shrinks the capacity of the given `StdString` to its size
///
/// @param self - The `StdString` to shrink
/// @ingroup std_string
#define std_string_shrink_to_fit(self) std_string_shrink_to_fit(&(self))
// clang-format off
/// @brief Inserts `to_insert` into `self` at the given index
///
/// @param self - The `StdString` to insert into
/// @param to_insert - The string to insert
/// @param index - The index into `self` at which to insert the `to_insert`
/// @ingroup std_string
#define std_string_insert(self, to_insert, index) _Generic((to_insert), 						   \
	const_cstring 					: std_string_insert_cstring(&(self), 					   	   \
										static_cast(const_cstring)(to_insert),  				   \
										strlen(static_cast(const_cstring)(to_insert)), index), 	   \
	cstring 						: std_string_insert_cstring(&(self),  				   		   \
										static_cast(cstring)(to_insert), 						   \
										strlen(static_cast(cstring)(to_insert)), index), 		   \
	const char[sizeof(to_insert)] 	: std_string_insert_cstring(&(self), 			/** NOLINT **/ \
										static_cast(const_cstring)(to_insert), 					   \
										sizeof(to_insert), index), 					/** NOLINT **/ \
	char[sizeof(to_insert)] 		: std_string_insert_cstring(&(self), 			/** NOLINT **/ \
										static_cast(cstring)(to_insert), 						   \
										sizeof(to_insert), index), 					/** NOLINT **/ \
	StdStringView* 					: std_string_insert_stringview(&(self), 				   	   \
										static_cast(StdStringView*)(to_insert), index), 		   \
	const StdStringView* 			: std_string_insert_stringview(&(self), 				   	   \
										static_cast(const StdStringView*)(to_insert), index), 	   \
	StdString* 						: std_string_insert(&(self), 							   	   \
										static_cast(StdString*)(to_insert), index), 			   \
	const StdString* 				: std_string_insert(&(self), 							   	   \
										static_cast(const StdString*)(to_insert), index))
// clang-format on
/// @brief Erases the character at the given `index` from `self`
///
/// @param self - The `StdString` to remove a character from
/// @param index - The index of the character to remove
/// @ingroup std_string
#define std_string_erase(self, index) std_string_erase(&(self), index)
/// @brief Erases `num_characters` characters from `self`, beginning at `index`
///
/// @param self - The `StdString` to remove characters from
/// @param index - The index where removal should begin
/// @param num_characters - The number of characters to remove
/// @ingroup std_string
#define std_string_erase_n(self, index, num_characters) \
	std_string_erase_n(&(self), index, num_characters)
/// @brief Resizes the given `StdString` to the given `new_size`, filling the characters in the
/// expanded size with null
///
/// @param self - The `StdString` to resize
/// @param new_size - The new size of the string
/// @ingroup std_string
#define std_string_resize(self, new_size) std_string_resize(&(self), new_size)
/// @brief Allocates memory such that at least `new_capacity` number of characters can be stored
/// in the given `StdString`
///
/// @param self - The `StdString` to reserve memory in
/// @param new_capacity - The number of characters to be able to store
/// @ingroup std_string
#define std_string_reserve(self, new_capacity) std_string_reserve(&(self), new_capacity)
/// @brief Pushes the given character onto the end of the given `StdString`
///
/// @param self - The `StdString` to append a character to
/// @param character - The character to append to `self`
/// @ingroup std_string
#define std_string_push_back(self, character) std_string_push_back(&(self), character)
/// @brief Pushes the given character onto the beginning of the given `StdString`
///
/// @param self - The `StdString` to prepend a character to
/// @param character - The character to prepend to `self`
/// @ingroup std_string
#define std_string_push_front(self, character) std_string_push_front(&(self), character)
/// @brief Removes the last character in `self` if `std_string_size(self) > 0`, and returns it
///
/// @param self - The `StdString` to pop the last character from
///
/// @return `Some(char)` if `std_string_size(self) > 0`, else `None(char)`
/// @ingroup std_string
#define std_string_pop_back(self) std_string_pop_back(&(self))
/// @brief Removes the first character in `self` if `std_string_size(self) > 0`, and returns it
///
/// @param self - The `StdString` to pop the first character from
///
/// @return `Some(char)` if `std_string_size(self) > 0`, else `None(char)`
/// @ingroup std_string
#define std_string_pop_front(self) std_string_pop_front(&(self))
// clang-format off
/// @brief Appends `to_append` to the end of `self`
///
/// @param self - The `StdString` to append to
/// @param to_append - The string to append
/// @ingroup std_string
#define std_string_append(self, to_append) _Generic((to_append), 						   		   \
	const_cstring 					: std_string_append_cstring(&(self), 					   	   \
										static_cast(const_cstring)(to_append),  				   \
										strlen(static_cast(const_cstring)(to_append))), 	   	   \
	cstring 						: std_string_append_cstring(&(self),  				   		   \
										static_cast(cstring)(to_append), 						   \
										strlen(static_cast(cstring)(to_append))), 		   		   \
	const char[sizeof(to_append)] 	: std_string_append_cstring(&(self), 			/** NOLINT **/ \
										static_cast(const_cstring)(to_append), 					   \
										sizeof(to_append)), 						/** NOLINT **/ \
	char[sizeof(to_append)] 		: std_string_append_cstring(&(self), 			/** NOLINT **/ \
										static_cast(cstring)(to_append), 						   \
										sizeof(to_append)), 						/** NOLINT **/ \
	StdStringView* 					: std_string_append_stringview(&(self), 				   	   \
										static_cast(StdStringView*)(to_append)), 		   		   \
	const StdStringView* 			: std_string_append_stringview(&(self), 				   	   \
										static_cast(const StdStringView*)(to_append)), 		   	   \
	StdString* 						: std_string_append(&(self), 							   	   \
										static_cast(StdString*)(to_append)), 			   		   \
	const StdString* 				: std_string_append(&(self), 							   	   \
										static_cast(const StdString*)(to_append)))
// clang-format on
// clang-format off
/// @brief Prepends `to_prepend` to the beginning of `self`
///
/// @param self - The `StdString` to prepend to
/// @param to_prepend - The string to prepend
/// @ingroup std_string
#define std_string_prepend(self, to_prepend) _Generic((to_prepend), 						   	   \
	const_cstring 					: std_string_prepend_cstring(&(self), 				   		   \
										static_cast(const_cstring)(to_prepend),  				   \
										strlen(static_cast(const_cstring)(to_prepend))), 	   	   \
	cstring 						: std_string_prepend_cstring(&(self),  				   		   \
										static_cast(cstring)(to_prepend), 						   \
										strlen(static_cast(cstring)(to_prepend))), 		   		   \
	const char[sizeof(to_prepend)] 	: std_string_prepend_cstring(&(self), 			/** NOLINT **/ \
										static_cast(const_cstring)(to_prepend), 				   \
										sizeof(to_prepend)), 						/** NOLINT **/ \
	char[sizeof(to_prepend)] 		: std_string_prepend_cstring(&(self), 			/** NOLINT **/ \
										static_cast(cstring)(to_prepend), 						   \
										sizeof(to_prepend)), 						/** NOLINT **/ \
	StdStringView* 					: std_string_prepend_stringview(&(self), 				   	   \
										static_cast(StdStringView*)(to_prepend)), 		   		   \
	const StdStringView* 			: std_string_prepend_stringview(&(self), 				   	   \
										static_cast(const StdStringView*)(to_prepend)), 		   \
	StdString* 						: std_string_prepend(&(self), 						   		   \
										static_cast(StdString*)(to_prepend)), 			   		   \
	const StdString* 				: std_string_prepend(&(self), 						   		   \
										static_cast(const StdString*)(to_prepend)))
// clang-format on
// clang-format off
/// @brief Replaces the contents of `self` with `to_replace_with`, beginning at the given index
///
/// @param self - The `StdString` to replace a portion of
/// @param to_replace_with - The string to replace `self`'s contents with
/// @param index - The index to start replacement at
/// @ingroup std_string
#define std_string_replace(self, to_replace_with, index) _Generic((to_replace_with), 			   \
	const_cstring 						: std_string_replace_cstring(&(self), 			   		   \
											static_cast(const_cstring)(to_replace_with),  		   \
											strlen(static_cast(const_cstring)(to_replace_with)),   \
											index), 	   										   \
	cstring 							: std_string_replace_cstring(&(self),  			   		   \
											static_cast(cstring)(to_replace_with), 				   \
											strlen(static_cast(cstring)(to_replace_with)),		   \
											index), 		   									   \
	const char[sizeof(to_replace_with)] : std_string_replace_cstring(&(self), 		/** NOLINT **/ \
											static_cast(const_cstring)(to_replace_with), 		   \
											sizeof(to_replace_with), 				/** NOLINT **/ \
											index), 											   \
	char[sizeof(to_replace_with)] 		: std_string_replace_cstring(&(self), 		/** NOLINT **/ \
											static_cast(cstring)(to_replace_with), 				   \
											sizeof(to_replace_with), 				/** NOLINT **/ \
											index), 											   \
	StdStringView* 						: std_string_replace_stringview(&(self), 			   	   \
											static_cast(StdStringView*)(to_replace_with), 		   \
											index), 		       								   \
	const StdStringView* 				: std_string_replace_stringview(&(self), 			   	   \
											static_cast(const StdStringView*)(to_replace_with),	   \
											index), 	   										   \
	StdString* 							: std_string_replace(&(self), 					   		   \
											static_cast(StdString*)(to_replace_with),			   \
											index), 			   	   							   \
	const StdString* 					: std_string_replace(&(self), 					   		   \
											static_cast(const StdString*)(to_replace_with), index))
// clang-format on

/// @brief Returns a `StdRandomAccessIterator(char_ref)` into the given
/// `StdString`, at the beginning of the iteration (pointing at the beginning of the string)
///
/// @param self - The `StdString` to get an iterator into
///
/// @return an iterator at the beginning of the iteration (pointing at the beginning of the
/// string)
/// @ingroup std_string
#define std_string_begin(self) std_string_begin(&(self))
/// @brief Returns a `StdRandomAccessIterator(char_ref)` into the given
/// `StdString`, at the end of the iteration (pointing at the end of the string)
///
/// @param self - The `StdString` to get an iterator into
///
/// @return an iterator at the end of the iteration (pointing at the end of the
/// string)
/// @ingroup std_string
#define std_string_end(self) std_string_end(&(self))
/// @brief Returns a `StdRandomAccessIterator(char_ref)` into the given
/// `StdString`, at the beginning of the reversed iteration (pointing at the end of the string)
///
/// @param self - The `StdString` to get an iterator into
///
/// @return an iterator at the beginning of the reversed iteration (pointing at the end of the
/// string)
/// @ingroup std_string
#define std_string_rbegin(self) std_string_rbegin(&(self))
/// @brief Returns a `StdRandomAccessIterator(char_ref)` into the given
/// `StdString`, at the end of the reversed iteration (pointing at the beginning of the string)
///
/// @param self - The `StdString` to get an iterator into
///
/// @return an iterator at the end of the reversed iteration (pointing at the beginning of the
/// string)
/// @ingroup std_string
#define std_string_rend(self) std_string_rend(&(self))
/// @brief Returns a `StdRandomAccessIterator(const_char_ref)` into the given
/// `StdString`, at the beginning of the iteration (pointing at the beginning of the string)
///
/// @param self - The `StdString` to get an iterator into
///
/// @return an iterator at the beginning of the iteration (pointing at the beginning of the
/// string)
/// @ingroup std_string
#define std_string_cbegin(self) std_string_cbegin(&(self))
/// @brief Returns a `StdRandomAccessIterator(const_char_ref)` into the given
/// `StdString`, at the end of the iteration (pointing at the end of the string)
///
/// @param self - The `StdString` to get an iterator into
///
/// @return an iterator at the end of the iteration (pointing at the end of the
/// string)
/// @ingroup std_string
#define std_string_cend(self) std_string_cend(&(self))
/// @brief Returns a `StdRandomAccessIterator(const_char_ref)` into the given
/// `StdString`, at the beginning of the reversed iteration (pointing at the end of the string)
///
/// @param self - The `StdString` to get an iterator into
///
/// @return an iterator at the beginning of the reversed iteration (pointing at the end of the
/// string)
/// @ingroup std_string
#define std_string_crbegin(self) std_string_crbegin(&(self))
/// @brief Returns a `StdRandomAccessIterator(const_char_ref)` into the given
/// `StdString`, at the end of the reversed iteration (pointing at the beginning of the string)
///
/// @param self - The `StdString` to get an iterator into
///
/// @return an iterator at the end of the reversed iteration (pointing at the beginning of the
/// string)
/// @ingroup std_string
#define std_string_crend(self) std_string_crend(&(self))

/// @brief Returns a const reference to the character at `index` in the given `StdStringView`
///
/// @param self - The `StdStringView` to get a character from
/// @param index - The index of the desired character in `self`
///
/// @return a const reference to the character at `index` in `self
/// @ingroup std_stringview
#define std_stringview_at(self, index) std_stringview_at(&(self), index)
/// @brief Returns the current length of the given `StdStringView`
///
/// @param self - The `StdStringView` to get the length of
///
/// @return the length of `self`
/// @ingroup std_stringview
#define std_stringview_length(self) std_stringview_length(&(self))
/// @brief Returns the current size of the given `StdStringView`
///
/// @param self - The `StdStringView` to get the size of
///
/// @return the size of `self`
/// @ingroup std_stringview
#define std_stringview_size(self) std_stringview_size(&(self))
// clang-format off

/// @brief Returns whether the given `StdStringView`s are equivalent
///
/// @param self - The first `StdStringView` to compare
/// @param to_compare - The `StdStringView` to compare to
///
/// @return whether the stringviews are equal
/// @ingroup std_stringview
#define std_stringview_equal(self, to_compare) _Generic((to_compare), 							   \
	const_cstring 					: 	std_stringview_equal_cstring(&(self), 					   \
											static_cast(const_cstring)(to_compare), 			   \
											strlen(static_cast(const_cstring)(to_compare))),       \
	cstring 						: 	std_stringview_equal_cstring(&(self), 				   	   \
											static_cast(cstring)(to_compare), 				       \
											strlen(static_cast(const_cstring)(to_compare))),       \
	const char[sizeof(to_compare)] 	: 	std_stringview_equal_cstring(&(self), 		/** NOLINT **/ \
											static_cast(const_cstring)(to_compare), 			   \
											sizeof(to_compare)), 					/** NOLINT **/ \
	char[sizeof(to_compare)] 		: 	std_stringview_equal_cstring(&(self), 		/** NOLINT **/ \
											static_cast(const_cstring)(to_compare),  			   \
											sizeof(to_compare)), 					/** NOLINT **/ \
	StdStringView* 					: 	std_stringview_equal(&(self), 			    	   		   \
											static_cast(const StdStringView*)(to_compare)), 	   \
	const StdStringView* 			: 	std_stringview_equal(&(self), 			   	   			   \
											static_cast(const StdStringView*)(to_compare)), 	   \
	StdString* 						: 	std_stringview_equal_string(&(self), 			    	   \
											static_cast(const StdString*)(to_compare)), 		   \
	const StdString* 				: 	std_stringview_equal_string(&(self), 			   	   	   \
											static_cast(const StdString*)(to_compare)))
// clang-format on

/// @brief Returns the `cstring` view of the given `StdStringView`
///
/// @param self - The `StdStringView` to get the `cstring` view of
///
/// @return `self` as a `cstring`
/// @ingroup std_stringview
#define std_stringview_into_cstring(self) std_stringview_into_cstring(&(self))
/// @brief Returns a `StdRandomAccessIterator(const_char_ref)` into the given
/// `StdStringView`, at the beginning of the iteration (pointing at the beginning of the view)
///
/// @param self - The `StdStringView` to get an iterator into
///
/// @return an iterator at the beginning of the iteration (pointing at the beginning of the
/// view)
/// @ingroup std_stringview
#define std_stringview_begin(self) std_stringview_begin(&(self))
/// @brief Returns a `StdRandomAccessIterator(const_char_ref)` into the given
/// `StdStringView`, at the end of the iteration (pointing at the end of the view)
///
/// @param self - The `StdStringView` to get an iterator into
///
/// @return an iterator at the end of the iteration (pointing at the end of the view)
/// @ingroup std_stringview
#define std_stringview_end(self) std_stringview_end(&(self))
/// @brief Returns a `StdRandomAccessIterator(const_char_ref)` into the given
/// `StdStringView`, at the beginning of the reversed iteration (pointing at the end of the
/// view)
///
/// @param self - The `StdStringView` to get an iterator into
///
/// @return an iterator at the beginning of the reversed iteration (pointing at the end of the
/// view)
/// @ingroup std_stringview
#define std_stringview_rbegin(self) std_stringview_rbegin(&(self))
/// @brief Returns a `StdRandomAccessIterator(const_char_ref)` into the given
/// `StdStringView`, at the end of the reversed iteration (pointing at the beginning of the
/// view)
///
/// @param self - The `StdStringView` to get an iterator into
///
/// @return an iterator at the end of the reversed iteration (pointing at the beginning of the
/// view)
/// @ingroup std_stringview
#define std_stringview_rend(self) std_stringview_rend(&(self))
/// @brief Returns a `StdRandomAccessIterator(const_char_ref)` into the given
/// `StdStringView`, at the beginning of the iteration (pointing at the beginning of the view)
///
/// @param self - The `StdStringView` to get an iterator into
///
/// @return an iterator at the beginning of the iteration (pointing at the beginning of the
/// view)
/// @ingroup std_stringview
#define std_stringview_cbegin(self) std_stringview_cbegin(&(self))
/// @brief Returns a `StdRandomAccessIterator(const_char_ref)` into the given
/// `StdStringView`, at the end of the iteration (pointing at the end of the view)
///
/// @param self - The `StdStringView` to get an iterator into
///
/// @return an iterator at the end of the iteration (pointing at the end of the view)
/// @ingroup std_stringview
#define std_stringview_cend(self) std_stringview_cend(&(self))
/// @brief Returns a `StdRandomAccessIterator(const_char_ref)` into the given
/// `StdStringView`, at the beginning of the reversed iteration (pointing at the end of the
/// view)
///
/// @param self - The `StdStringView` to get an iterator into
///
/// @return an iterator at the beginning of the reversed iteration (pointing at the end of the
/// view)
/// @ingroup std_stringview
#define std_stringview_crbegin(self) std_stringview_crbegin(&(self))
/// @brief Returns a `StdRandomAccessIterator(const_char_ref)` into the given
/// `StdStringView`, at the end of the reversed iteration (pointing at the beginning of the
/// view)
///
/// @param self - The `StdStringView` to get an iterator into
///
/// @return an iterator at the end of the reversed iteration (pointing at the beginning of the
/// view)
/// @ingroup std_stringview
#define std_stringview_crend(self) std_stringview_crend(&(self))

/// @brief The function vector table of methods associated with `StdStringView`
typedef struct std_stringview_vtable_t {
	/// @brief Returns a `StdRandomAccessIterator(const_char_ref)` into the given
	/// `StdStringView`, at the beginning of the iteration (pointing at the beginning of the view)
	StdRandomAccessIterator(const_char_ref) (*const begin)(const StdStringView* restrict self);
	/// @brief Returns a `StdRandomAccessIterator(const_char_ref)` into the given
	/// `StdStringView`, at the end of the iteration (pointing at the end of the view)
	StdRandomAccessIterator(const_char_ref) (*const end)(const StdStringView* restrict self);
	/// @brief Returns a `StdRandomAccessIterator(const_char_ref)` into the given
	/// `StdStringView`, at the beginning of the reversed iteration (pointing at the end of the
	/// view)
	StdRandomAccessIterator(const_char_ref) (*const rbegin)(const StdStringView* restrict self);
	/// @brief Returns a `StdRandomAccessIterator(const_char_ref)` into the given
	/// `StdStringView`, at the end of the reversed iteration (pointing at the beginning of the
	/// view)
	StdRandomAccessIterator(const_char_ref) (*const rend)(const StdStringView* restrict self);
	/// @brief Returns a `StdRandomAccessIterator(const_char_ref)` into the given
	/// `StdStringView`, at the beginning of the iteration (pointing at the beginning of the view)
	StdRandomAccessIterator(const_char_ref) (*const cbegin)(const StdStringView* restrict self);
	/// @brief Returns a `StdRandomAccessIterator(const_char_ref)` into the given
	/// `StdStringView`, at the end of the iteration (pointing at the end of the view)
	StdRandomAccessIterator(const_char_ref) (*const cend)(const StdStringView* restrict self);
	/// @brief Returns a `StdRandomAccessIterator(const_char_ref)` into the given
	/// `StdStringView`, at the beginning of the reversed iteration (pointing at the end of the
	/// view)
	StdRandomAccessIterator(const_char_ref) (*const crbegin)(const StdStringView* restrict self);
	/// @brief Returns a `StdRandomAccessIterator(const_char_ref)` into the given
	/// `StdStringView`, at the end of the reversed iteration (pointing at the beginning of the
	/// view)
	StdRandomAccessIterator(const_char_ref) (*const crend)(const StdStringView* restrict self);
} std_stringview_vtable_t;

/// @brief The function vector table of methods associated with `StdString`
/// @ingroup std_string
typedef struct std_string_vtable_t {
	/// @brief Returns a `StdRandomAccessIterator(char_ref)` into the given
	/// `StdString`, at the beginning of the iteration (pointing at the beginning of the string)
	StdRandomAccessIterator(char_ref) (*const begin)(StdString* restrict self);
	/// @brief Returns a `StdRandomAccessIterator(char_ref)` into the given
	/// `StdString`, at the end of the iteration (pointing at the end of the string)
	StdRandomAccessIterator(char_ref) (*const end)(StdString* restrict self);
	/// @brief Returns a `StdRandomAccessIterator(char_ref)` into the given
	/// `StdString`, at the beginning of the reversed iteration (pointing at the end of the
	/// string)
	StdRandomAccessIterator(char_ref) (*const rbegin)(StdString* restrict self);
	/// @brief Returns a `StdRandomAccessIterator(char_ref)` into the given
	/// `StdString`, at the end of the reversed iteration (pointing at the beginning of the
	/// string)
	StdRandomAccessIterator(char_ref) (*const rend)(StdString* restrict self);
	/// @brief Returns a `StdRandomAccessIterator(const_char_ref)` into the given
	/// `StdString`, at the beginning of the iteration (pointing at the beginning of the string)
	StdRandomAccessIterator(const_char_ref) (*const cbegin)(const StdString* restrict self);
	/// @brief Returns a `StdRandomAccessIterator(const_char_ref)` into the given
	/// `StdString`, at the end of the iteration (pointing at the end of the string)
	StdRandomAccessIterator(const_char_ref) (*const cend)(const StdString* restrict self);
	/// @brief Returns a `StdRandomAccessIterator(const_char_ref)` into the given
	/// `StdString`, at the beginning of the reversed iteration (pointing at the end of the
	/// string)
	StdRandomAccessIterator(const_char_ref) (*const crbegin)(const StdString* restrict self);
	/// @brief Returns a `StdRandomAccessIterator(const_char_ref)` into the given
	/// `StdString`, at the end of the reversed iteration (pointing at the beginning of the
	/// string)
	StdRandomAccessIterator(const_char_ref) (*const crend)(const StdString* restrict self);
} std_string_vtable_t;

#undef ___DISABLE_IF_NULL
#endif // STD_STRING
