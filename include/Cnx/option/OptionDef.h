/// @file OptionDef.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief This module provides macro definitions for implementing and working with `CnxOption(T)`
/// @version 0.2.1
/// @date 2022-04-07
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
#include <Cnx/Def.h>

#ifndef CNX_OPTION_DEF
	#define CNX_OPTION_DEF

typedef enum Option {
	Some,
	None
} Option;

	/// @brief Macro alias for `CnxOption` holding the type `T`
	///
	/// Used to generate and refer to a typedef name for `CnxOption(T)` instantiations of type
	/// `T`
	///
	/// @param T - The type to store in the `CnxOption(T)`
	/// @ingroup cnx_option
	#define CnxOption(T) CONCAT2(CnxOption, T)

	/// @brief Creates an identifier for a function, variable, or struct associated with
	/// `CnxOption(T)`
	#define CnxOptionIdentifier(T, Identifier) CONCAT2(cnx_option_, CONCAT3(T, _, Identifier))

	/// @brief Creates a `CnxOption(T)` holding the given value
	///
	/// @param T - The held type of the `CnxOption(T)`
	/// @param value - The value to store in the `CnxOption(T)`
	///
	/// @return a `CnxOption(T)` containing the given value
	/// @ingroup cnx_option
	#define Some(T, value)                                   \
		((CnxOption(T)){.tag = Some,                         \
						.variant_identifier(Some) = {value}, \
						.m_vtable = &CnxOptionIdentifier(T, vtable_impl)})

	/// @brief Creates a `CnxOption(T)` holding no value
	///
	/// @param T - The held type of the `CnxOption(T)`
	///
	/// @return a `CnxOption(T)` containing no value
	/// @ingroup cnx_option
	#define None(T)                                     \
		((CnxOption(T)){.tag = None,                    \
						.variant_identifier(None) = {}, \
						.m_vtable = &CnxOptionIdentifier(T, vtable_impl)})

	/// @brief Returns whether this `CnxOption(T)` is holding a value
	///
	/// @param self - The `CnxOption(T)` to check
	///
	/// @return `true` if this `CnxOption(T)` is holding a value, `false` otherwise
	/// @ingroup cnx_option
	#define cnx_option_is_some(self) (self).m_vtable->is_some(&(self))

	/// @brief Returns whether this `CnxOption(T)` __isn't__ holding a value
	///
	/// @param self - The `CnxOption(T)` to check
	///
	/// @return `true` if this `CnxOption(T)` __isn't__ holding a value, `false` otherwise
	/// @ingroup cnx_option
	#define cnx_option_is_none(self) (self).m_vtable->is_none(&(self))

	/// @brief Returns a const reference to the value stored in this `CnxOption(T)`
	///
	/// @param self - The `CnxOption(T)` to get the stored value from
	///
	/// @return a const reference to the contained value
	/// @note Panics if `self` does not contain a value
	/// @ingroup cnx_option
	#define cnx_option_as_const(self) *((self).m_vtable->as_const(&(self)))

	/// @brief Returns a reference to the value stored in this `CnxOption(T)`
	///
	/// @param self - The `CnxOption(T)` to get the stored value from
	///
	/// @return a reference to the contained value
	/// @note Panics if `self` does not contain a value
	/// @ingroup cnx_option
	#define cnx_option_as_mut(self) *((self).m_vtable->as_mut(&(self)))

	/// @brief Returns the value stored in this `CnxOption(T)`
	///
	/// @param self - The `CnxOption(T)` to get the stored value from
	///
	/// @return the contained value
	/// @note Panics if `self` does not contain a value
	/// @ingroup cnx_option
	#define cnx_option_unwrap(self) (self).m_vtable->unwrap(&(self))

	/// @brief Returns the value stored in this `CnxOption(T)` , or `default_value`
	/// if this is `None(T)`
	///
	/// @param self - The `CnxOption(T)` to get the stored value from
	///
	/// @param default_value - The value to return if this is `None(T)`
	/// @return the contained value, or `default_value`
	/// @ingroup cnx_option
	#define cnx_option_unwrap_or(self, default_value) \
		(self).m_vtable->unwrap_or(&(self), (default_value))

	/// @brief Returns the value stored in this `CnxOption(T)` , or the value returned by
	/// `default_generator` if this is `None(T)`
	///
	/// @param self - The `CnxOption(T)` to get the stored value from
	/// @param default_generator - The function to generate the value
	///  to return if this is `None(T)`
	///
	/// @return the contained value, or the one generated by `default_generator`
	/// @ingroup cnx_option
	#define cnx_option_unwrap_or_else(self, default_generator) \
		(self).m_vtable->unwrap_or_else(&(self), (default_generator))

	/// @brief Returns the value stored in this `CnxOption(T)`
	///
	/// @param self - The `CnxOption(T)` to get the stored value from
	/// @param panic_message - The custom panic message to use if `self` is not the `Some(T,
	/// value)` variant
	///
	/// @return the contained value
	/// @note Panics if `self` does not contain a value, with the custom panic message
	/// `panic_message`
	/// @ingroup cnx_option
	#define cnx_option_expect(self, panic_message) (self).m_vtable->expect(&(self), (panic_message))

	/// @brief Maps the value stored in this `CnxOption(T)`
	///
	/// If `self` is `Some`, maps the value stored in it by calling `map_func` on it.
	/// Otherwise, returns `None`.
	///
	/// `map_func` must be a function type callable with `T` and returning `MapType`.
	///
	/// @param self - The `CnxOption(T)` to map
	/// @param MapType - The type to map to. This is the type that will be held by
	/// the returned `CnxOption`, and `map_func` must return this type.
	/// @param map_func - The function to map the value stored in `self` to a `MapType`.
	///
	/// @return The `CnxOption(MapType)` resulting from the mapping
	/// @ingroup cnx_option
	#define cnx_option_map(self, MapType, map_func)                                 \
		({                                                                          \
			CnxOption(MapType) UNIQUE_VAR(res);                                     \
			if(cnx_option_is_some(self)) {                                          \
				UNIQUE_VAR(res) = Some(MapType, map_func(cnx_option_unwrap(self))); \
			}                                                                       \
			else {                                                                  \
				UNIQUE_VAR(res) = None(MapType);                                    \
			}                                                                       \
			UNIQUE_VAR(res);                                                        \
		})

	/// @brief Maps the value stored in this `CnxOption(T)`
	///
	/// If `self` is `Some`, maps the value stored in it by calling `map_func` on it.
	/// Otherwise, returns `default_value`.
	///
	/// `map_func` must be a function type callable with `T` and returning the
	/// same type as `default_value`.
	///
	/// @param self - The `CnxOption(T)` to map
	/// @param map_func - The function to map the value stored in `self`.
	/// @param default_value - The value to return if `self` is `None`.
	///
	/// @return The value resulting from the mapping
	/// @ingroup cnx_option
	#define cnx_option_map_or(self, map_func, default_value)                                   \
		({                                                                                     \
			cnx_type_assert(typeof(map_func(cnx_option_unwrap(self))), typeof(default_value)); \
			typeof(default_value) UNIQUE_VAR(res);                                             \
			if(cnx_option_is_some(self)) {                                                     \
				UNIQUE_VAR(res) = map_func(cnx_option_unwrap(self));                           \
			}                                                                                  \
			else {                                                                             \
				UNIQUE_VAR(res) = (default_value);                                             \
			}                                                                                  \
			UNIQUE_VAR(res);                                                                   \
		})

	/// @brief Maps the value stored in this `CnxOption(T)`
	///
	/// If `self` is `Some`, maps the value stored in it by calling `map_func` on it.
	/// Otherwise, returns the result of calling `default_generator`.
	///
	/// `map_func` must be a function type callable with `T`.
	/// `default_generator` must be a function type taking no arguments.
	/// Both `map_func` and `default_generator` must return the same type.
	///
	/// @param self - The `CnxOption(T)` to map
	/// @param map_func - The function to map the value stored in `self`.
	/// @param default_generator - The function to generate the returned value if `self` is `None`.
	///
	/// @return The value resulting from the mapping
	/// @ingroup cnx_option
	#define cnx_option_map_or_else(self, map_func, default_generator)  \
		({                                                             \
			cnx_type_assert(typeof(map_func(cnx_option_unwrap(self))), \
							typeof(default_generator()));              \
			typeof(default_generator()) UNIQUE_VAR(res);               \
			if(cnx_option_is_some(self)) {                             \
				UNIQUE_VAR(res) = map_func(cnx_option_unwrap(self));   \
			}                                                          \
			else {                                                     \
				UNIQUE_VAR(res) = default_generator();                 \
			}                                                          \
			UNIQUE_VAR(res);                                           \
		})

	/// @brief Returns `option_b` if `self` is `Some`. Otherwise, returns `None`
	///
	/// @param self - The `CnxOption(T)` to "and" with `option_b`.
	/// @param option_b - another `CnxOption` to "and" with `self`.
	///
	/// @return `option_b` if both `self` is `Some`. Otherwise, `None`.
	/// @ingroup cnx_option
	#define cnx_option_and(self, option_b)                     \
		({                                                     \
			typeof(option_b) UNIQUE_VAR(res);                  \
			if(cnx_option_is_some(self)) {                     \
				UNIQUE_VAR(res) = (option_b);                  \
			}                                                  \
			else {                                             \
				UNIQUE_VAR(res).tag = None;                    \
				UNIQUE_VAR(res).variant_identifer(Some) = {0}; \
			}                                                  \
			UNIQUE_VAR(res);                                   \
		})

	/// @brief Returns the result of calling `next_func` with the contained value if this is `Some`,
	/// otherwise returns `None`.
	///
	/// @param self - The `CnxOption(T)` to operate on.
	/// @param next_func - The function type to call with the value stored in `self`.
	///
	/// @return The result of calling `next_func` with the value stored in `self`.
	/// Otherwise, `None`.
	/// @ingroup cnx_option
	#define cnx_option_and_then(self, next_func)                        \
		({                                                              \
			typeof(next_func(cnx_option_unwrap(self))) UNIQUE_VAR(res); \
			if(cnx_option_is_some(self)) {                              \
				UNIQUE_VAR(res) = = next_func(cnx_option_unwrap(self)); \
			}                                                           \
			else {                                                      \
				UNIQUE_VAR(res).tag = None;                             \
				UNIQUE_VAR(res).variant_identifer(Some) = {0};          \
			}                                                           \
			UNIQUE_VAR(res);                                            \
		})

	/// @brief Returns `self` if it is `Some`, otherwise returns `option_b`
	///
	/// @param self - The `CnxOption(T)` to "or" with `option_b`.
	/// @param option_b - another `CnxOption` to "or" with `self`.
	///
	/// @return `self` if is is `Some` Otherwise, `option_b`.
	/// @ingroup cnx_option
	#define cnx_option_or(self, option_b) (self).m_vtable->_or(&(self), (option_b))

	/// @brief Returns `self` if it is `Some`, otherwise returns the result of
	/// calling `func`
	///
	/// @param self - The `CnxOption(T)`.
	/// @param func - The function to call if `self` is `None`.
	///
	/// @return `self` if is is `Some` Otherwise, `func()`
	/// @ingroup cnx_option
	#define cnx_option_or_else(self, func) (self).m_vtable->or_else(&(self), (func))

	/// @brief Converts the given `CnxOption(T)` to a `bool`
	///
	/// This is equivalent to calling `Cnx_option_is_some(self)`
	/// @param self - The `CnxOption(T)` to convert to a `bool`
	///
	/// @return `self` as a `bool`
	/// @ingroup cnx_option
	#define cnx_option_as_bool(self) (self).m_vtable->as_bool(&(self))

#endif // CNX_OPTION_DEF
