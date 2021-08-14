/// @file StdIfLet.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief This module provides the `if_let` operation for `StdOption(T)` and `StdResult(T)`
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

/// @ingroup error_handling
/// @{
///	@defgroup monadics Monadic Interfaces
/// C2nxt provides some uniform interfaces for some of its monad-like types
/// @}

#ifndef STD_IF_LET
/// @brief Definitions for `if_let` and variants thereof
#define STD_IF_LET
/// @brief `if_let` implementation
///
/// @param var - The variable name for the value if `self` holds a value
/// @param self	- The monadic type to conditionally get the value of
#define __if_let(var, self)                                                    \
	({                                                                         \
		let UNIQUE_VAR(var) = (self).m_vtable->as_bool(&(self));               \
		if(UNIQUE_VAR(var)) {                                                  \
			typeof((self).m_vtable->unwrap(&(self)))* UNIQUE_VAR(var2)         \
				= const_cast(typeof((self).m_vtable->unwrap(&(self)))*)(&var); \
			*UNIQUE_VAR(var2) = (self).m_vtable->unwrap(&(self));              \
		}                                                                      \
		UNIQUE_VAR(var);                                                       \
	})
/// @brief `if_let(var, self)` provides an abstraction to conditionally retrieve a value
///
/// `if_let(var, self)` provides an abstraction to conditionally retrieve a value from a conditional
/// wrapper type, like `StdOption(T)` or `StdResult(T)`, and then perform some operations with that
/// value. The distinction between `if_let(var, self)` and `if_let_mut(var, self)` is that in
/// `if_let(var, self)`, `var` is declared as `const`, while in `if_let_mut(var, self)`, `var` is
/// mutable.
/// Example:
///
/// @code {.c}
/// StdOption(u32) do_thing(void);
///
/// void example(void) {
/// 	let_mut maybe_thing = do_thing();
///		if_let(thing, maybe_thing) {
///			// do something with thing...
/// 	}
/// 	else {
///			// handle maybe_thing being `None(T)`
/// 	}
/// }
/// @endcode
///
/// @param var - The name for the variable the value held by `self` will be exctracted to
/// @param self - The `StdOption(T)`, `StdResult(T)`, or other compatible type potentially holding a
/// value
/// @ingroup monadics
#define if_let(var, self)                                     \
	const typeof((self).m_vtable->unwrap(&(self))) var = {0}; \
	if(__if_let(var, self))
/// @brief `if_let_mut` implementation
///
/// @param var - The variable name for the value if `self` holds a value
/// @param self	- The monadic type to conditionally get the value of
#define __if_let_mut(var, self)                                  \
	({                                                           \
		let UNIQUE_VAR(var) = (self).m_vtable->as_bool(&(self)); \
		if(UNIQUE_VAR(var)) {                                    \
			var = (self).m_vtable->unwrap(&(self));              \
		}                                                        \
		UNIQUE_VAR(var);                                         \
	})

/// @brief `if_let_mut(var, self)` provides an abstraction to conditionally retrieve a value
///
/// `if_let_mut(var, self)` provides an abstraction to conditionally retrieve a value from a
/// conditional wrapper type, like `StdOption(T)` or `StdResult(T)`, and then perform some
/// operations with that value. The distinction between `if_let(var, self)` and
/// `if_let_mut(var, self)` is that in `if_let(var, self)`, `var` is declared as `const`, while in
/// `if_let_mut(var, self)`, `var` is mutable.
/// Example:
///
/// @code {.c}
/// StdOption(u32) do_thing(void);
///
/// void example(void) {
/// 	let_mut maybe_thing = do_thing();
///		if_let(thing, maybe_thing) {
///			// do something with thing...
/// 	}
/// 	else {
///			// handle maybe_thing being `None(T)`
/// 	}
/// }
/// @endcode
///
/// @param var - The name for the variable the value held by `self` will be exctracted to
/// @param self - The `StdOption(T)`, `StdResult(T)`, or other compatible type potentially holding a
/// value
/// @ingroup monadics
#define if_let_mut(var, self)                     \
	typeof((self).m_vtable->unwrap(&(self))) var; \
	if(__if_let_mut(var, self))

#endif
