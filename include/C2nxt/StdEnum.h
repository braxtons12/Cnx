/// @file StdEnum.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief StdEnum provides macros for declaring algebraic datatypes called algebraic `Enum`s,
/// similar to Rust's `Enum`, and pattern matching on their variants.
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

/// @defgroup std_enum Algebraic Enums
/// This module provides macros for declaring algebraic datatypes called algebraic `Enum`s,
/// similar to Rust's `Enum`, and pattern matching on their variants.
///
/// A C2nxt `Enum` is an algebraic data type similar to tagged unions, but with additional
/// capabilities and related abstractions that make working with them simple and pleasant, with no
/// boilerplate.
///
/// `Enum` declarations consist of a type name, followed by one or more variant layouts, followed
/// by zero or more additional member declarations. Variant layouts are tuples of the form
/// `(VariantName, MemberTypes...)` where `VariantName` is required, and `MemberTypes` can be zero
/// or more type names (such as `int`, `StdString`, or `struct timespec`).
/// `Enum` declarations may alternately use a predefined `enum` type for identifying their variants.
/// In this case, the `enum` members' identifiers must exactly match the names of the `Enum`'s
/// variants.
///
/// Example:
/// @code {.c}
/// // An `Enum` declaration consisting of three variants
/// Enum(Example1, (E1Variant1, i32, i32), (E1Variant2, u32), (E1Variant3));
/// // An `Enum` declaration consisting of two variants and one additional member
/// Enum(Example2, (E2Variant1, i32), (E2Variant2, f32), float E2_member);
///
/// typedef enum Example3Enum {
///		E3Variant1 = 0,
///		E3Variant2
/// }
///
/// // An `Enum` declaration with two variants, using an existing enum to tag its variants.
/// // Note that the variant names exactly match the `enum` members, as they must
/// EnumWithTag(Example3, Example3Enum, (E3Variant1, i32), (E3Variant2, f64));
///
/// @endcode
///
/// There are several abstractions that make working with `Enum`s simple and concise, such as
/// pattern matching and conditional binding. Pattern matching is semantically similar to `switch`
/// statements, but unlike `switch`, does not require the `break` keyword to prevent cascading.
/// Binding abstractions allow for extracting an enum variant's constituent members and assigning
/// them to new variables. Binding is always done by value; to discourage mutating an enum in place,
/// reference binding must be done manually.
///
/// Example:
/// @code {.c}
///
/// // we recommend `#define`ing constructor-macros to simplify creating `Enum` variants
/// #define E1Variant2(first_int, second_int) \
/// 	(Example){							  \
/// 		.tag = E1Variant1,				  \
/// 		.variant_identifier(E1Variant2) = {  \
/// 			(first_int), 				  \
/// 			(second_int)},				  \
/// 		},								  \
/// 	}
///
/// // take an `Example1` `Enum` from above as a function parameter
/// void example1(Example1 var) {
///
///		// pattern match on the variant `var` currently is.
///		// Note that while some branches bind variables to variant members in this example,
/// 	// binding is optional, and pattern matching without binding is perfectly valid
///		match(var) {
///			// if `var` is the `E1Variant1` variant, this branch will be taken and `first_int` and
///			// `second_int` will be bound to the members of the variant
/// 		variant(E1Variant1, first_int, second_int) {
/// 			println("E1Variant1: [first_int: {}, second_int: {}]", first_int, second_int);
/// 		}
///			// if `var` is the `E1Variant2` variant, this branch will be taken and `u32_value`
///			// will be bound to the member of the variant
///			variant(E1Variant2, u32_value) {
///				println("E1Variant2: [u32_value: {}]", u32_value);
///			}
///			// if `var` is the `E1Variant3` variant, this branch will be taken. As `E1Variant3` has
///			// no members, we can't bind any variables with this branch.
///			variant(E1Variant3) {
///				println("E1Variant3");
/// 		}
///		}
/// }
///
///	void example2(Example1 var) {
///		// variants we don't care about explicitly matching can be globbed with `wildcard()`
///		match(var) {
///			variant(E1Variant1) {
///				println("E1Variant1");
///			}
///			wildcard() {
///				println("Not E1Variant1");
///			}
///		}
/// }
///
/// void example3(Example1 var) {
///		// conditionally bind if `var` is the specified variant
///		match_let(var, E1Variant1, first_int, second_int) {
///			println("E1Variant1: [first_int: {}, second_int: {}]", first_int, second_int);
///		}
///		else {
///			println("Not E1Variant1");
///		}
/// }
///
/// void example3(Example1 var) {
///		// can `match_let` without binding
///		match_let(var, E1Variant1) {
///			println("Is E1Variant1");
///		}
///		else {
///			println("Not E1Variant1");
///		}
/// }
///
/// void example4(Example1 var) {
///		// check is `var` is the specifier variant
///		if(is_variant(var, E1Variant1)) {
///			println("Is E1Variant1");
///		}
///		else {
///			println("Not E1Variant1");
///		}
/// }
///
/// @endcode

#include <C2nxt/StdAssert.h>
#include <C2nxt/StdDef.h>
#include <C2nxt/StdTuple.h>

#ifndef STD_ENUM
	#define STD_ENUM

	/// @brief Declares and defines an `Enum`, `Type`, with the given variants and additional
	/// members
	///
	/// `Enum` declarations consist of a type name, followed by one or more variant layouts,
	/// followed by zero or more additional member declarations. Variant layouts are tuples of the
	/// form `(VariantName, MemberTypes...)` where `VariantName` is required, and `MemberTypes` can
	/// be zero or more type names (such as `int`, `StdString`, or `struct timespec`).
	///
	/// @param Type - The name for the `Enum`
	/// @param ... - The list of variant tuples and additional members
	/// @ingroup std_enum
	#define Enum(Type, ...)                                              \
		IGNORE_RESERVED_IDENTIFIER_WARNING_START                         \
		typedef struct Type Type;                                        \
		typedef enum { GET_TYPE_NAMES(__VA_ARGS__) } CONCAT2(Type, Tag); \
		typedef struct Type {                                            \
			CONCAT2(Type, Tag) tag;                                      \
			union {                                                      \
				DEFINE_STRUCTS(__VA_ARGS__)                              \
			};                                                           \
			DEFINE_MEMBERS(__VA_ARGS__)                                  \
		} Type;                                                          \
		IGNORE_RESERVED_IDENTIFIER_WARNING_STOP

	/// @brief Declares and defines an `Enum`, `Type`, using an existing C-style `enum`, `TagType`,
	/// to tag/identify its variants
	///
	/// `Enum` declarations consist of a type name, followed by one or more variant layouts,
	/// followed by zero or more additional member declarations. Variant layouts are tuples of the
	/// form `(VariantName, MemberTypes...)` where `VariantName` is required, and `MemberTypes` can
	/// be zero or more type names (such as `int`, `StdString`, or `struct timespec`). `Enum`
	/// declarations may alternately use a predefined C-style `enum` type for identifying their
	/// variants. In this case, the C-style `enum`'s members' identifiers must exactly match the
	/// names of the `Enum`'s variants.
	///
	/// @param Type - The name for the `Enum`
	/// @param TagType - The C-style `enum` to use to identify the `Enum`s variants. The `enum`'s
	/// members' identifiers must exactly match the names of the `Enum`'s variants.
	/// @param ... - The list of variant tuples and additional members
	/// @ingroup std_enum
	#define EnumWithTag(Type, TagType, ...)      \
		IGNORE_RESERVED_IDENTIFIER_WARNING_START \
		typedef struct Type Type;                \
		typedef TagType CONCAT2(Type, Tag);      \
		typedef struct Type {                    \
			CONCAT2(Type, Tag) tag;              \
			union {                              \
				DEFINE_STRUCTS(__VA_ARGS__)      \
			};                                   \
			DEFINE_MEMBERS(__VA_ARGS__)          \
		} Type;                                  \
		IGNORE_RESERVED_IDENTIFIER_WARNING_STOP

	/// @brief Pattern matches on the given `Enum`, `x`
	/// Pattern matching works similarly to `switch` statements, but with slightly different
	/// semantics. First off, pattern cases are declared with `variant(...)`, which can, in addition
	/// to match the variant, optionally decompose the variant into one or more bound variables.
	/// Bound variables are bound by value, as decomposing a variant during pattern matching is
	/// intended to be consuming, so mutating the `Enum` variant's members in place is not
	/// possible without manually doing so. Second, fallthrough is not possible, so `break`ing to
	/// prevent it is not necessary
	///
	/// Example:
	/// @code {.c}
	/// Enum(Example, (Variant1, i32, i32), (Variant2, f32, f32), (Variant3));
	///
	/// void example(Example var) {
	///		match(var) {
	///			variant(Variant1, first_i32, second_i32) {
	///				println("Variant1: [first: {}, second: {}]", first_i32, second_i32);
	///			}
	///			variant(Variant2, first_f32, second_f32) {
	///				println("Variant2: [first: {e}, second: {e}]", first_f32, second_f32);
	///			}
	///			variant(Variant3) {
	///				println("Variant3");
	///			}
	///		}
	/// }
	/// @endcode
	///
	/// @param x - The `Enum` to pattern match on
	/// @ingroup std_enum
	#define match(x)                                                                   \
		IGNORE_SWITCH_ENUM_WARNING_START                                               \
		IGNORE_RESERVED_IDENTIFIER_WARNING_START                                       \
		for(let_mut __to_match = static_cast(typeof(x)*)(&(x)); __to_match != nullptr; \
			__to_match = nullptr)                                                      \
			IGNORE_RESERVED_IDENTIFIER_WARNING_STOP                                    \
		switch((x).tag)                                                                \
		IGNORE_SWITCH_ENUM_WARNING_STOP

	/// @brief Declares a pattern-matching case in a `match` statement
	/// Pattern-matching cases declare a variant to match against and, in addition,
	/// can optionally decompose the variant into one or more bound variables. Bound variables
	/// are bound by value, as decomposing a variant during pattern matching is intended to be
	/// consuming, so mutating the `Enum` variant's members in place is not possible without
	/// manually doing so. Unlike `switch` statements, fallthrough is not possible in `match`
	/// statements, so `break`ing between `variant(...)`s is not necessary.
	///
	/// Example:
	/// @code {.c}
	/// Enum(Example, (Variant1, i32, i32), (Variant2, f32, f32), (Variant3));
	///
	/// void example(Example var) {
	///		match(var) {
	///			variant(Variant1, first_i32, second_i32) {
	///				println("Variant1: [first: {}, second: {}]", first_i32, second_i32);
	///			}
	///			variant(Variant2, first_f32, second_f32) {
	///				println("Variant2: [first: {e}, second: {e}]", first_f32, second_f32);
	///			}
	///			variant(Variant3) {
	///				println("Variant3");
	///			}
	///		}
	/// }
	/// @endcode
	///
	/// @param ... - The variant name, followed by any decomposition variable names to bind the
	/// variant's members to
	///
	/// @ingroup std_enum
	#define variant(...)                    \
		break;                              \
		case FIRST(__VA_ARGS__):            \
			ignore(__to_match);             \
			IF(CONTAINS_COMMA(__VA_ARGS__), \
			   BIND_VARIANT(variant_identifier(FIRST(__VA_ARGS__)), REMOVE_FIRST(__VA_ARGS__)))

	/// @brief Declares a catch-all pattern-matching case in a `match` statement
	/// This will match all variants that have not been explicitly matched against prior. Unlike
	/// `switch` statements, fallthrough is not possible in `match` statements, so `break`ing
	/// between `variant(...)`s is not necessary.
	///
	/// Example:
	/// @code {.c}
	/// Enum(Example, (Variant1, i32, i32), (Variant2, f32, f32), (Variant3));
	///
	/// void example(Example var) {
	///		match(var) {
	///			variant(Variant1, first_i32, second_i32) {
	///				println("Variant1: [first: {}, second: {}]", first_i32, second_i32);
	///			}
	///			wildcard() {
	///				println("Not Variant1");
	// }
	///		}
	/// }
	/// @endcode
	///
	/// @param ... - The variant name, followed by any decomposition variable names to bind the
	/// variant's members to
	///
	/// @ingroup std_enum
	#define wildcard() \
		break;         \
		default: ignore(__to_match);

	/// @brief Conditionally pattern matches on the given `Enum`, `self`
	/// `match_let` works similarly to an `if` statement, but with slightly different
	/// semantics. First off, in addition to conditionally branching if `self` is the given variant,
	/// it can optionally decompose the variant into one or more bound variables. Bound variables
	/// are bound by value, as decomposing a variant during pattern matching is intended to be
	/// consuming, so mutating the `Enum` variant's members in place is not possible without
	/// manually doing so.
	///
	/// Example:
	/// @code {.c}
	/// Enum(Example, (Variant1, i32, i32), (Variant2, f32, f32), (Variant3));
	///
	/// void example(Example var) {
	///		match(var) {
	///			variant(Variant1, first_i32, second_i32) {
	///				println("Variant1: [first: {}, second: {}]", first_i32, second_i32);
	///			}
	///			variant(Variant2, first_f32, second_f32) {
	///				println("Variant2: [first: {e}, second: {e}]", first_f32, second_f32);
	///			}
	///			variant(Variant3) {
	///				println("Variant3");
	///			}
	///		}
	/// }
	/// @endcode
	///
	/// @param x - The `Enum` to pattern match on
	/// @ingroup std_enum
	#define match_let(self, ...)                                    \
		if((self).tag == FIRST(__VA_ARGS__))                        \
		IF(CONTAINS_COMMA(__VA_ARGS__),                             \
		   LET_BIND_VARIANT(self,                                   \
							variant_identifier(FIRST(__VA_ARGS__)), \
							REMOVE_FIRST(__VA_ARGS__)))

	/// @brief Determines if `self` is the `Enum` variant `Variant`
	///
	/// Example:
	/// @code {.c}
	/// Enum(Example, (Variant1, i32, i32), (Variant2, f32, f32), (Variant3));
	///
	///	void example(Example var) {
	///		if(is_variant(self, Variant2)) {
	///			println("Is Variant2");
	///		}
	///		else {
	///			println("Not Variant2");
	/// 	}
	/// }
	/// @endcode
	///
	/// @param self - The `Enum` to check
	/// @param Variant - The variant to check if `self` is
	///
	/// @return whether `self` is the `Variant` variant
	/// @ingroup std_enum
	#define is_variant(self, Variant) ((Variant) == (self).tag)

	/// @brief Extracts the internal value of `self` as the variant `Variant`
	///
	/// If `self` is the variant `Variant`, extracts the internal value of `self` as that variant,
	/// so it can be used for further processing.
	///
	///
	/// Example:
	/// @code {.c}
	/// Enum(Example, (Variant1, i32, i32), (Variant2, f32, f32), (Variant3));
	///
	///	void example(Example var) {
	///		let variant2 = extract_variant(var, Variant2);
	///		println("first: {}, second: {}", variant2._1, variant2._2);
	/// }
	/// @endcode
	///
	/// @param self - The `Enum` to extract
	/// @param Variant - The `Enum` variant to extract `self` as
	///
	/// @return `self`'s internal value, as `Variant`
	/// @ingroup std_enum
	#define extract_variant(self, Variant)                                   \
		({                                                                   \
			std_assert(is_variant(self, Variant),                            \
					   "Enum to be extracted is not the requested variant"); \
			(self).variant_identifier(Variant);                              \
		})

	/// @brief Generates the member identifier for an `Enum` variant, `Variant`, used in the struct
	/// definition of an `Enum`
	///
	/// @param Variant - The name of the `Enum` variant
	///
	/// @return The identifier used for `Variant` in an `Enum` struct definition
	/// @ingroup std_enum
	#define variant_identifier(Variant) CONCAT2(Variant, _variant)

	/// @brief extracts the `Enum` variant names from a list of `Enum` parameters
	#define GET_TYPE_NAMES(...) APPLY_TO_LIST(CHECK_IS_TUPLE_AND_EXTRACT_TYPE, __VA_ARGS__)
	/// @brief extract the `Enum` variant name from an `Enum` parameter
	#define CHECK_IS_TUPLE_AND_EXTRACT_TYPE(x) IF(IS_TUPLE(x), FIRST(EXPAND_TUPLE(x)))

	/// @brief Defines the structs used to represent `Enum` variants in an `Enum` declaration
	#define DEFINE_STRUCTS(...) DELIMIT_LIST(;, APPLY_TO_LIST(DEFINE_STRUCT, __VA_ARGS__))
	/// @brief Defines a struct used to represent an `Enum` variant in an `Enum` declaration
	#define DEFINE_STRUCT(x)                                      \
		IF(IS_TUPLE(x),                                           \
		   IF_ELSE(CONTAINS_COMMA(EXPAND_TUPLE(x)),               \
				   DEFINE_STRUCT_IMPL(FIRST(EXPAND_TUPLE(x)), x), \
				   DEFINE_EMPTY_STRUCT(FIRST(EXPAND_TUPLE(x)))))
	/// @brief Defines a struct used to represent an `Enum` variant in an `Enum` declaration
	#define DEFINE_EMPTY_STRUCT(Name) \
		struct {                      \
		} variant_identifier(Name)
	/// @brief Defines a struct used to represent an `Enum` variant in an `Enum` declaration
	#define DEFINE_STRUCT_IMPL(Name, x) \
		struct EXPAND_STRUCT_IMPL(REMOVE_FIRST(EXPAND_TUPLE(x))) variant_identifier(Name)
	/// @brief Defines a struct used to represent an `Enum` variant in an `Enum` declaration
	#define EXPAND_STRUCT_IMPL(...)                                          \
		{                                                                    \
			DELIMIT_LIST(;, APPEND_EACH_TO_LIST(SELECTOR_LIST, __VA_ARGS__)) \
		}

	/// @brief Defines any additional members of an `Enum` in an `Enum` declaration
	#define DEFINE_MEMBERS(...) DELIMIT_LIST(, APPLY_TO_LIST(DEFINE_MEMBER, __VA_ARGS__))
	/// @brief Defines an additional members of an `Enum` in an `Enum` declaration
	#define DEFINE_MEMBER(x) IF(IS_NOT_TUPLE(x), x;)

	/// @brief Binds an `Enum` to a decomposition of the variant `variant_name`
	#define BIND_VARIANT(variant_name, ...) LET_BIND_VARIANT(*__to_match, variant_name, __VA_ARGS__)

	/// @brief Binds an `Enum` to a decomposition of the variant `variant_name`
	#define LET_BIND_VARIANT(self, variant_name, ...) \
		scoped_tuple_bind((self).variant_name, __VA_ARGS__)

#endif // STD_ENUM
