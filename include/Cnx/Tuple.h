/// @file Tuple.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief CnxTuple provides macros for declaring and decomposing tuples
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

/// @defgroup cnx_tuple Tuples
/// This module provides macros for declaring and decomposing tuples.
/// A tuple is a group of values that may or may not be related that are stored in a single object.
/// They can be useful for things like combining function parameters into a single named entity
/// or returning multiple values from a function call.
///
/// A Tuple type can be declared with `DeclTuple(...)` or `DeclNamedTuple(...)` and referred to with
/// `Tuple(...)` or the given name. Tuples can be easily decomposed with `tuple_bind(self, ...)` and
/// `scoped_tuple_bind(self, ...)`.
///
/// Example:
/// @code {.c}
/// // declare a named tuple type called `Rectangle` that holds 4 `i32`s
///	DeclNamedTuple(Rectangle, i32, i32, i32, i32);
///
/// void example(Rectangle rect) {
///		// bind the tuple's members to the given variable names, inside the enclosing scope
/// 	tuple_bind(rect, x, y, width, height);
/// 	println("x: {}, y: {}, width: {}, height: {}", x, y, width, height);
/// }
///
/// DeclTuple(f32, f32, f32, f32);
///
/// void example2(Tuple(f32, f32, f32, f32) params) {
///		// bind the tuple's members to the given variable names, inside a new scope
///		scoped_tuple_bind(params, first, second, third, fourth) {
///			println("first: {}, second: {}, third: {}, fourth: {}", first, second, third, fourth);
///		}
/// }
/// @endcode

#include <Cnx/Def.h>

#ifndef CNX_TUPLE
	#define CNX_TUPLE

	/// @brief Used to refer to a tuple type
	///
	/// @param ... - The types contained by the tuple
	/// @ingroup cnx_tuple
	#define Tuple(...) CONCAT_DEFERRED(Tuple __VA_OPT__(, ) __VA_ARGS__)

	/// @brief Used to declare a tuple type
	///
	/// @param ... - The types contained by the tuple
	/// @ingroup cnx_tuple
	#define DeclTuple(...) ___DEFINE_TUPLE(__VA_ARGS__)

	/// @brief Used to declare a tuple type with a unique name
	/// The first argument must be the unique name to use for this tuple type.
	/// Subsequent arguments should be the types held by the tuple.
	///
	/// @param ... - The first argument must be the unique name. Subsequent arguments should be the
	/// types contained by the tuple.
	/// @ingroup cnx_tuple
	#define DeclNamedTuple(...) ___DEFINE_NAMED_TUPLE(__VA_ARGS__)

	/// @brief Decomposes the given tuple `self` into one or more of its members, in declared order
	/// Decomposes `self` into one or more of its members, using the given names to refer to its
	/// members. The given names exist within the current scope.
	///
	/// Example:
	/// @code {.c}
	/// DeclNamedTuple(Point, i32, i32);
	///
	/// void example(Point point) {
	///		// binds the first member of point to `x`, the second to `y`
	///		// `x` and `y` exist in the current scope
	/// 	tuple_bind(point, x, y);
	///		println("Point: [x: {}, y: {}]", x, y);
	/// }
	/// @endcode
	///
	/// @param self - The tuple to decompose
	/// @param ... - The names to bind the tuple's members to
	/// @ingroup cnx_tuple
	#define tuple_bind(self, ...) ___TUPLE_BIND_IMPL(self, PP_NUM_ARGS(__VA_ARGS__), __VA_ARGS__)

	/// @brief Decomposes the given tuple `self` into one or more of its members, in declared order
	/// Decomposes `self` into one or more of its members, using the given names to refer to its
	/// members. The given names are contained within a new scope.
	///
	/// Example:
	/// @code {.c}
	/// DeclNamedTuple(Point, i32, i32);
	///
	/// void example(Point point) {
	///		// binds the first member of point to `x`, the second to `y`
	/// 	scoped_tuple_bind(point, x, y) {
	///			// `x` and `y` only exist within this scope
	///			println("Point: [x: {}, y: {}]", x, y);
	/// 	}
	///		// `x` and `y` don't exist here
	/// }
	/// @endcode
	///
	/// @param self - The tuple to decompose
	/// @param ... - The names to bind the tuple's members to
	/// @ingroup cnx_tuple
	#define scoped_tuple_bind(self, ...) \
		___SCOPED_TUPLE_BIND_IMPL(self, PP_NUM_ARGS(__VA_ARGS__), __VA_ARGS__)

	/// @brief Declares a tuple type
	#define ___DEFINE_TUPLE(...)                   \
		IF_ELSE(CONTAINS_COMMA(__VA_ARGS__),       \
				___DEFINE_TUPLE_IMPL(__VA_ARGS__), \
				___DEFINE_EMPTY_TUPLE)
	/// @brief Declares a tuple type
	#define ___DEFINE_TUPLE_IMPL(...) \
		typedef struct ___DEFINE_TUPLE_MEMBERS_IMPL(__VA_ARGS__) Tuple(__VA_ARGS__)
	/// @brief Declares the members of a tuple type
	#define ___DEFINE_TUPLE_MEMBERS_IMPL(...)                                \
		{                                                                    \
			DELIMIT_LIST(;, APPEND_EACH_TO_LIST(SELECTOR_LIST, __VA_ARGS__)) \
		}
	/// @brief Declares an empty tuple type
	#define ___DEFINE_EMPTY_TUPLE \
		typedef struct {          \
		} Tuple()

	/// @brief Declares a named tuple type
	#define ___DEFINE_NAMED_TUPLE(...)                   \
		IF_ELSE(CONTAINS_COMMA(__VA_ARGS__),             \
				___DEFINE_NAMED_TUPLE_IMPL(__VA_ARGS__), \
				___DEFINE_EMPTY_NAMED_TUPLE(FIRST(__VA_ARGS__)))
	/// @brief Declares a named tuple type
	#define ___DEFINE_NAMED_TUPLE_IMPL(...) \
		typedef struct ___DEFINE_TUPLE_MEMBERS_IMPL(REMOVE_FIRST(__VA_ARGS__)) FIRST(__VA_ARGS__)
	/// @brief Declares an empty named tuple type
	#define ___DEFINE_EMPTY_NAMED_TUPLE(Name) \
		typedef struct {                      \
		} Name

	/// @brief Decomposes `self` into one or more of its members
	#define ___TUPLE_BIND_IMPL(self, N, ...) \
		CONCAT2_DEFERRED(___TUPLE_BIND_IMPL_, N)(self, 1, __VA_ARGS__)

	/// @brief Decomposes `self` into one or more of its members
	#define ___TUPLE_BIND_IMPL_1(self, N, ...) let_mut FIRST(__VA_ARGS__) = (self.SELECTOR_LIST(N));
	/// @brief Decomposes `self` into one or more of its members
	#define ___TUPLE_BIND_IMPL_2(self, N, ...)                \
		let_mut FIRST(__VA_ARGS__) = (self.SELECTOR_LIST(N)); \
		___TUPLE_BIND_IMPL_1(self, ADD1(N), REMOVE_FIRST(__VA_ARGS__))
	/// @brief Decomposes `self` into one or more of its members
	#define ___TUPLE_BIND_IMPL_3(self, N, ...)                \
		let_mut FIRST(__VA_ARGS__) = (self.SELECTOR_LIST(N)); \
		___TUPLE_BIND_IMPL_2(self, ADD1(N), REMOVE_FIRST(__VA_ARGS__))
	/// @brief Decomposes `self` into one or more of its members
	#define ___TUPLE_BIND_IMPL_4(self, N, ...)                \
		let_mut FIRST(__VA_ARGS__) = (self.SELECTOR_LIST(N)); \
		___TUPLE_BIND_IMPL_3(self, ADD1(N), REMOVE_FIRST(__VA_ARGS__))
	/// @brief Decomposes `self` into one or more of its members
	#define ___TUPLE_BIND_IMPL_5(self, N, ...)                \
		let_mut FIRST(__VA_ARGS__) = (self.SELECTOR_LIST(N)); \
		___TUPLE_BIND_IMPL_4(self, ADD1(N), REMOVE_FIRST(__VA_ARGS__))
	/// @brief Decomposes `self` into one or more of its members
	#define ___TUPLE_BIND_IMPL_6(self, N, ...)                \
		let_mut FIRST(__VA_ARGS__) = (self.SELECTOR_LIST(N)); \
		___TUPLE_BIND_IMPL_5(self, ADD1(N), REMOVE_FIRST(__VA_ARGS__))
	/// @brief Decomposes `self` into one or more of its members
	#define ___TUPLE_BIND_IMPL_7(self, N, ...)                \
		let_mut FIRST(__VA_ARGS__) = (self.SELECTOR_LIST(N)); \
		___TUPLE_BIND_IMPL_6(self, ADD1(N), REMOVE_FIRST(__VA_ARGS__))
	/// @brief Decomposes `self` into one or more of its members
	#define ___TUPLE_BIND_IMPL_8(self, N, ...)                \
		let_mut FIRST(__VA_ARGS__) = (self.SELECTOR_LIST(N)); \
		___TUPLE_BIND_IMPL_7(self, ADD1(N), REMOVE_FIRST(__VA_ARGS__))
	/// @brief Decomposes `self` into one or more of its members
	#define ___TUPLE_BIND_IMPL_9(self, N, ...)                \
		let_mut FIRST(__VA_ARGS__) = (self.SELECTOR_LIST(N)); \
		___TUPLE_BIND_IMPL_8(self, ADD1(N), REMOVE_FIRST(__VA_ARGS__))
	/// @brief Decomposes `self` into one or more of its members
	#define ___TUPLE_BIND_IMPL_10(self, N, ...)               \
		let_mut FIRST(__VA_ARGS__) = (self.SELECTOR_LIST(N)); \
		___TUPLE_BIND_IMPL_9(self, ADD1(N), REMOVE_FIRST(__VA_ARGS__))
	/// @brief Decomposes `self` into one or more of its members
	#define ___TUPLE_BIND_IMPL_11(self, N, ...)               \
		let_mut FIRST(__VA_ARGS__) = (self.SELECTOR_LIST(N)); \
		___TUPLE_BIND_IMPL_10(self, ADD1(N), REMOVE_FIRST(__VA_ARGS__))
	/// @brief Decomposes `self` into one or more of its members
	#define ___TUPLE_BIND_IMPL_12(self, N, ...)               \
		let_mut FIRST(__VA_ARGS__) = (self.SELECTOR_LIST(N)); \
		___TUPLE_BIND_IMPL_11(self, ADD1(N), REMOVE_FIRST(__VA_ARGS__))

	/// @brief Decomposes `self` into one or more of its members, in a new scope
	#define ___SCOPED_TUPLE_BIND_IMPL(self, N, ...) \
		BEGIN_SCOPED_VARIABLES()                    \
		CONCAT2_DEFERRED(___SCOPED_TUPLE_BIND_IMPL_, N)(self, 1, __VA_ARGS__)

	/// @brief Decomposes `self` into one or more of its members, in a new scope
	#define ___SCOPED_TUPLE_BIND_IMPL_1(self, N, ...) \
		SCOPE_VARIABLE(let_mut FIRST(__VA_ARGS__) = (self.SELECTOR_LIST(N)))
	/// @brief Decomposes `self` into one or more of its members, in a new scope
	#define ___SCOPED_TUPLE_BIND_IMPL_2(self, N, ...)                        \
		SCOPE_VARIABLE(let_mut FIRST(__VA_ARGS__) = (self.SELECTOR_LIST(N))) \
		___SCOPED_TUPLE_BIND_IMPL_1(self, ADD1(N), REMOVE_FIRST(__VA_ARGS__))
	/// @brief Decomposes `self` into one or more of its members, in a new scope
	#define ___SCOPED_TUPLE_BIND_IMPL_3(self, N, ...)                        \
		SCOPE_VARIABLE(let_mut FIRST(__VA_ARGS__) = (self.SELECTOR_LIST(N))) \
		___SCOPED_TUPLE_BIND_IMPL_2(self, ADD1(N), REMOVE_FIRST(__VA_ARGS__))
	/// @brief Decomposes `self` into one or more of its members, in a new scope
	#define ___SCOPED_TUPLE_BIND_IMPL_4(self, N, ...)                        \
		SCOPE_VARIABLE(let_mut FIRST(__VA_ARGS__) = (self.SELECTOR_LIST(N))) \
		___SCOPED_TUPLE_BIND_IMPL_3(self, ADD1(N), REMOVE_FIRST(__VA_ARGS__))
	/// @brief Decomposes `self` into one or more of its members, in a new scope
	#define ___SCOPED_TUPLE_BIND_IMPL_5(self, N, ...)                        \
		SCOPE_VARIABLE(let_mut FIRST(__VA_ARGS__) = (self.SELECTOR_LIST(N))) \
		___SCOPED_TUPLE_BIND_IMPL_4(self, ADD1(N), REMOVE_FIRST(__VA_ARGS__))
	/// @brief Decomposes `self` into one or more of its members, in a new scope
	#define ___SCOPED_TUPLE_BIND_IMPL_6(self, N, ...)                        \
		SCOPE_VARIABLE(let_mut FIRST(__VA_ARGS__) = (self.SELECTOR_LIST(N))) \
		___SCOPED_TUPLE_BIND_IMPL_5(self, ADD1(N), REMOVE_FIRST(__VA_ARGS__))
	/// @brief Decomposes `self` into one or more of its members, in a new scope
	#define ___SCOPED_TUPLE_BIND_IMPL_7(self, N, ...)                        \
		SCOPE_VARIABLE(let_mut FIRST(__VA_ARGS__) = (self.SELECTOR_LIST(N))) \
		___SCOPED_TUPLE_BIND_IMPL_6(self, ADD1(N), REMOVE_FIRST(__VA_ARGS__))
	/// @brief Decomposes `self` into one or more of its members, in a new scope
	#define ___SCOPED_TUPLE_BIND_IMPL_8(self, N, ...)                        \
		SCOPE_VARIABLE(let_mut FIRST(__VA_ARGS__) = (self.SELECTOR_LIST(N))) \
		___SCOPED_TUPLE_BIND_IMPL_7(self, ADD1(N), REMOVE_FIRST(__VA_ARGS__))
	/// @brief Decomposes `self` into one or more of its members, in a new scope
	#define ___SCOPED_TUPLE_BIND_IMPL_9(self, N, ...)                        \
		SCOPE_VARIABLE(let_mut FIRST(__VA_ARGS__) = (self.SELECTOR_LIST(N))) \
		___SCOPED_TUPLE_BIND_IMPL_8(self, ADD1(N), REMOVE_FIRST(__VA_ARGS__))
	/// @brief Decomposes `self` into one or more of its members, in a new scope
	#define ___SCOPED_TUPLE_BIND_IMPL_10(self, N, ...)                       \
		SCOPE_VARIABLE(let_mut FIRST(__VA_ARGS__) = (self.SELECTOR_LIST(N))) \
		___SCOPED_TUPLE_BIND_IMPL_9(self, ADD1(N), REMOVE_FIRST(__VA_ARGS__))
	/// @brief Decomposes `self` into one or more of its members, in a new scope
	#define ___SCOPED_TUPLE_BIND_IMPL_11(self, N, ...)                       \
		SCOPE_VARIABLE(let_mut FIRST(__VA_ARGS__) = (self.SELECTOR_LIST(N))) \
		___SCOPED_TUPLE_BIND_IMPL_10(self, ADD1(N), REMOVE_FIRST(__VA_ARGS__))
	/// @brief Decomposes `self` into one or more of its members, in a new scope
	#define ___SCOPED_TUPLE_BIND_IMPL_12(self, N, ...)                       \
		SCOPE_VARIABLE(let_mut FIRST(__VA_ARGS__) = (self.SELECTOR_LIST(N))) \
		___SCOPED_TUPLE_BIND_IMPL_11(self, ADD1(N), REMOVE_FIRST(__VA_ARGS__))

#endif
