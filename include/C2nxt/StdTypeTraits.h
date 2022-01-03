/// @file StdTypeTraits.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief This module compile-time facilities for checking traits of arbitrary types and of the
/// types of arbitrary values
/// @version false.true
/// @date 2022-01-02
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

#include <C2nxt/StdPlatform.h>

#if STD_PLATFORM_COMPILER_CLANG && STD_PLATFORM_APPLE
_Pragma("GCC diagnostic push") _Pragma("GCC diagnostic ignored \"-Wnonportable-include-path\"")
#endif

#include <C2nxt/StdDef.h>

#if STD_PLATFORM_COMPILER_CLANG && STD_PLATFORM_APPLE
	_Pragma("GCC diagnostic pop")
#endif

/// @ingroup utils
/// @{
/// @defgroup type_traits Type Traits
/// C2nxt Type Traits are compile-time facilities for checking attributes of types and comparing
/// types, including types of values
/// @}

#ifndef STD_TYPE_TRAITS
	/// @brief definitions related to Type Traits
	#define STD_TYPE_TRAITS

// clang-format off

	/// @brief Determines if the two types are the same
	///
	/// Determines if the two types are the same. This will require the types to be the same at the
	/// compiler level, not the semantic level. Eg: typedefs will __not__ compare as unequal if they
	/// refer to the same underlying type.
	///
	/// @param type1 - The first type to compare
	/// @param type2 - The second type to compare	
	/// @return `true` if the types are equal, `false` otherwise
	/// @ingroup type_traits
	#define std_types_equal(type1, type2) 	 \
			_Generic(((type1){0}), 			 \
				type2   : true, /**NOLINT**/ \
				default : false)             \
			&& _Generic(((type2){0}), 		 \
				type1   : true, /**NOLINT**/ \
				default : false)

	/// @brief Determines if the type of `value` is the same as the given type
	///
	/// Determines if the type of `value` and the type `type` are the same. This will require the
	/// types to be the same at the compiler level, not the semantic level. Eg: typedefs will
	/// __not__ compare as unequal if they refer to the same underlying type.
	///
	/// @param type - The type to compare
	/// @param value - The value to compare the type of
	/// @return `true` if the types are equal, `false` otherwise
	/// @ingroup type_traits
	#define std_types_equal_v(type, value)   	   \
			_Generic(((type){0}), 		  	 	   \
				typeof(value) : true,			   \
				default       : false)             \
			&& _Generic((value), 				   \
				type          : true, /**NOLINT**/ \
				default       : false)

	/// @brief Determines if the type of `value1` is the same as the type of `value2`
	///
	/// Determines if the type of `value1` and the type of `value2` are the same. This will require
	/// the types to be the same at the compiler level, not the semantic level. Eg: typedefs will
	/// __not__ compare as unequal if they refer to the same underlying type.
	///
	/// @param value1 - The first value to compare the type of
	/// @param value2 - The first value to compare the type of
	/// @return `true` if the types are equal, `false` otherwise
	/// @ingroup type_traits
	#define std_types_equal_vs(value1, value2)      \
			_Generic(((typeof(value1)){0}),			\
				typeof(value2) : true,				\
				default        : false)             \
			&& _Generic((value2),					\
				typeof(value1) : true, /**NOLINT**/ \
				default        : false)

	/// @brief Converts the given type to a string literal
	///
	/// @param T - The type to convert to a string literal
	/// @return the type `T` as a string literal
	/// @ingroup type_traits
	#define std_type_as_string(T) AS_STRING(T)

	/// @brief Determines if `value`, of unqualified type `T` is const
	///
	/// Determines if `value` is const. Does not determine if it is a compile-time constant, only
	/// that its type is declared as const at compile-time
	///
	/// @param T - The type of value, without any possible qualifiers
	/// @param value - The value to check constness for
	/// @ingroup type_traits
	#define std_is_const_t(T, value)			\
			_Generic((&(value)), 				\
				T * 	 : false, /** NOLINT**/	\
				const T* : true)
// clang-format on
#endif // STD_TYPE_TRAITS
