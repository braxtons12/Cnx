/// @file StdOptionDef.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief This module provides macro definitions for implementing and working with `StdOption(T)`
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

#include <C2nxt/StdDef.h>

#ifndef STD_OPTION_DEF
	#define STD_OPTION_DEF

typedef enum Option {
	Some,
	None
} Option;

	/// @brief Macro alias for `StdOption` holding the type `T`
	///
	/// Used to generate and refer to a typedef name for `StdOption(T)` instantiations of type
	/// `T`
	///
	/// @param T - The type to store in the `StdOption(T)`
	/// @ingroup std_option
	#define StdOption(T) CONCAT2(StdOption, T)

	/// @brief Creates an identifier for a function, variable, or struct associated with
	/// `StdOption(T)`
	#define StdOptionIdentifier(T, Identifier) CONCAT2(std_option_, CONCAT3(T, _, Identifier))

	/// @brief Creates a `StdOption(T)` holding the given value
	///
	/// @param T - The held type of the `StdOption(T)`
	/// @param value - The value to store in the `StdOption(T)`
	///
	/// @return a `StdOption(T)` containing the given value
	/// @ingroup std_option
	#define Some(T, value)                                    \
		(StdOption(T)) {                                      \
			.tag = Some, .variant_identifier(Some) = {value}, \
			.m_vtable = &StdOptionIdentifier(T, vtable_impl)  \
		}

	/// @brief Creates a `StdOption(T)` holding no value
	///
	/// @param T - The held type of the `StdOption(T)`
	///
	/// @return a `StdOption(T)` containing no value
	/// @ingroup std_option
	#define None(T)                                          \
		(StdOption(T)) {                                     \
			.tag = None, .variant_identifier(None) = {},     \
			.m_vtable = &StdOptionIdentifier(T, vtable_impl) \
		}

	/// @brief Returns whether this `StdOption(T)` is holding a value
	///
	/// @param self - The `StdOption(T)` to check
	///
	/// @return `true` if this `StdOption(T)` is holding a value, `false` otherwise
	/// @ingroup std_option
	#define std_option_is_some(self) (self).m_vtable->is_some(&(self))

	/// @brief Returns whether this `StdOption(T)` __isn't__ holding a value
	///
	/// @param self - The `StdOption(T)` to check
	///
	/// @return `true` if this `StdOption(T)` __isn't__ holding a value, `false` otherwise
	/// @ingroup std_option
	#define std_option_is_none(self) (self).m_vtable->is_none(&(self))

	/// @brief Returns a const reference to the value stored in this `StdOption(T)`
	///
	/// @param self - The `StdOption(T)` to get the stored value from
	///
	/// @return a const reference to the contained value
	/// @note Panics if `self` does not contain a value
	/// @ingroup std_option
	#define std_option_as_const(self) *((self).m_vtable->as_const(&(self)))

	/// @brief Returns a reference to the value stored in this `StdOption(T)`
	///
	/// @param self - The `StdOption(T)` to get the stored value from
	///
	/// @return a reference to the contained value
	/// @note Panics if `self` does not contain a value
	/// @ingroup std_option
	#define std_option_as_mut(self) *((self).m_vtable->as_mut(&(self)))

	/// @brief Returns the value stored in this `StdOption(T)`
	///
	/// @param self - The `StdOption(T)` to get the stored value from
	///
	/// @return the contained value
	/// @note Panics if `self` does not contain a value
	/// @ingroup std_option
	#define std_option_unwrap(self) (self).m_vtable->unwrap(&(self))

	/// @brief Returns the value stored in this `StdOption(T)` , or `default_value`
	/// if this is `None(T)`
	///
	/// @param self - The `StdOption(T)` to get the stored value from
	///
	/// @param default_value - The value to return if this is `None(T)`
	/// @return the contained value, or `default_value`
	/// @ingroup std_option
	#define std_option_unwrap_or(self, default_value) \
		(self).m_vtable->unwrap_or(&(self), (default_value))

	/// @brief Returns the value stored in this `StdOption(T)` , or the value returned by
	/// `default_generator` if this is `None(T)`
	///
	/// @param self - The `StdOption(T)` to get the stored value from
	/// @param default_generator - The function to generate the value
	///  to return if this is `None(T)`
	///
	/// @return the contained value, or the one generated by `default_generator`
	/// @ingroup std_option
	#define std_option_unwrap_or_else(self, default_generator) \
		(self).m_vtable->unwrap_or_else(&(self), (default_generator))

	/// @brief Returns the value stored in this `StdOption(T)`
	///
	/// @param self - The `StdOption(T)` to get the stored value from
	/// @param panic_message - The custom panic message to use if `self` is not the `Some(T,
	/// value)` variant
	///
	/// @return the contained value
	/// @note Panics if `self` does not contain a value, with the custom panic message
	/// `panic_message`
	/// @ingroup std_option
	#define std_option_expect(self, panic_message) (self).m_vtable->expect(&(self), (panic_message))

	/// @brief Converts the given `StdOption(T)` to a `bool`
	///
	/// This is equivalent to calling `std_option_is_some(self)`
	/// @param self - The `StdOption(T)` to convert to a `bool`
	///
	/// @return `self` as a `bool`
	/// @ingroup std_option
	#define std_option_as_bool(self) (self).m_vtable->as_bool(&(self))

#endif
