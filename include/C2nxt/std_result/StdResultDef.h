/// @file StdResultDef.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief This module provides macro definitions for implementing and working with `StdResult(T)`
/// @version 0.2.2
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

#include <C2nxt/StdAssert.h>
#include <C2nxt/StdDef.h>
#include <C2nxt/StdEnum.h>

#ifndef STD_RESULT_DEF
	#define STD_RESULT_DEF

typedef enum Result {
	Ok,
	Err
} Result;

	/// @brief Macro Alias used to refer to an instantiation of `StdResult(T)` for type `T`
	///
	/// Used to create and refer to a typedef for an instantiation of `StdResult(T)` holding type
	/// `T`
	///
	/// @param T - The type held by the result
	/// @ingroup std_result
	#define StdResult(T) CONCAT2(StdResult, T)
	/// @brief Macro to create and refer to an identifier (function, variable, struct) related to an
	/// instantiation of `StdResult(T)` of type `T`
	#define StdResultIdentifier(T, Identifier) CONCAT2(std_result_, CONCAT3(T, _, Identifier))

	/// @brief Creates a `StdResult(T)` holding the given value
	///
	/// @param T - The Ok type to store in the `StdResult(T)`
	/// @param value - The Ok value to store in the `StdResult(T)`
	///
	/// @return a `StdResult` containing the given value
	/// @ingroup std_result
	#define Ok(T, value)                                   \
		((StdResult(T)){.tag = Ok,                         \
						.variant_identifier(Ok) = {value}, \
						.m_vtable = &StdResultIdentifier(T, vtable_impl)})

	/// @brief Creates a `StdResult(T)` holding the given error
	///
	/// @param T - The Ok type to store in the `StdResult(T)`
	/// @param error - The error to store in the `StdResult(T)`
	///
	/// @return a `StdResult` containing the given error
	/// @ingroup std_result
	#define Err(T, error)                                   \
		((StdResult(T)){.tag = Err,                         \
						.variant_identifier(Err) = {error}, \
						.m_vtable = &StdResultIdentifier(T, vtable_impl)})

	/// @brief Returns whether this `StdResult(T)` is holding a value
	///
	/// This returns whether the `StdResult(T)` is `Ok(T, value)`, ie the associated operation that
	/// created this `StdResult(T)` was successful and this holds the resulting value.
	///
	/// @param self - The `StdResult(T)` to check
	///
	/// @return `true` if this `StdResult(T)` is holding a value, `false` otherwise
	/// @ingroup std_result
	#define std_result_is_ok(self) (self).m_vtable->is_ok(&(self))

	/// @brief Returns whether this `StdResult(T)` is holding an error
	///
	/// This returns whether the `StdResult(T)` is `Err(T, error)`, ie the associated operation that
	/// created this `StdResult(T)` failed and his holds an error.
	///
	/// @param self - The `StdResult(T)` to check
	///
	/// @return `true` if this `StdResult(T)` is holding an error
	/// @ingroup std_result
	#define std_result_is_err(self) (self).m_vtable->is_err(&(self))

	/// @brief Returns a const reference to the value stored in this `StdResult(T)`
	///
	/// Returns a const reference (an immediately dereferenced pointer-to-const) to the value stored
	///	in this `StdResult(T)`. This requires that this **does** hold a value. If this is the
	///`Err(T, error)`
	/// variant, this function will invoke a panic.
	///
	/// @param self - The `StdResult(T)` to get the stored value from
	///
	/// @return a const reference to the contained value
	/// @note Panics if `self` does not contain a value
	/// @ingroup std_result
	#define std_result_as_const(self) *((self).m_vtable->as_const(&(self)))

	/// @brief Returns a reference to the value stored in this `StdResult(T)`
	///
	/// Returns a reference (an immediately dereferenced pointer) to the value stored in this
	/// `StdResult(T)`. This requires that this **does** hold a value. If this is the
	/// `Err(T, error)` variant, this function will invoke a panic.
	///
	/// @param self - The `StdResult(T)` to get the stored value from
	///
	/// @return a reference to the contained value
	/// @note Panics if `self` does not contain a value
	/// @ingroup std_result
	#define std_result_as_mut(self) *((self).m_vtable->as_mut(&(self)))

	/// @brief Returns the value stored in this `StdResult(T)`
	///
	/// This requires that this **does** hold a value. If this is the `Err(T, error)` variant, this
	/// function will invoke a panic.
	///
	/// @param self - The `StdResult(T)` to get the stored value from
	///
	/// @return the contained value
	/// @note Panics if `self` does not contain a value
	/// @ingroup std_result
	#define std_result_unwrap(self) (self).m_vtable->unwrap(&(self))

	/// @brief Returns the value stored in this `StdResult(T)` , or `default_value`
	///
	/// If this `StdResult(T)` is the `Ok(T, value)` variant, then this returns the stored value, if
	/// this is the `Err(T, error)` variant, then this returns `default_value`
	///
	/// @param self - The `StdResult(T)` to get the stored value from
	/// @param default_value - The value to return if this is the `Err(T, error)` variant
	///
	/// @return the contained value, or `default_value`
	/// @ingroup std_result
	#define std_result_unwrap_or(self, default_value) \
		(self).m_vtable->unwrap_or(&(self), (default_value))

	/// @brief Returns the value stored in this `StdResult(T)` , or `default_generator()`
	///
	/// If this `StdResult(T)` is the `Ok(T, value)` variant, then this returns the stored value, if
	/// this is the `Err(T, error)` variant, then this returns the value generated by calling
	/// `default_generator`.
	///
	/// @param self - The `StdResult(T)` to get the stored value from
	/// @param default_generator - The function to generate the value to return if this is the
	/// `Err(T, error)` variant
	///
	/// @return the contained value, or the one generated by `default_generator`
	/// @ingroup std_result
	#define std_result_unwrap_or_else(self, default_generator) \
		(self).m_vtable->unwrap_or_else(&(self), (default_generator))

	/// @brief Returns the value stored in this `StdResult(T)`
	///
	/// Returns the value stored in this `StdResult(T)` if this is the `Ok(T, value)` variant.
	/// Otherwise, invokes a panic with the given panic message.
	///
	/// @param self - The `StdResult(T)` to get the stored value from
	/// @param panic_message - The custom panic message to use if `self` is not the `Ok(T, value)`
	/// variant
	///
	/// @return the contained value
	/// @note Panics if `self` does not contain a value, with the custom panic message
	/// `panic_message`
	/// @ingroup std_result
	#define std_result_expect(self, panic_message) (self).m_vtable->expect(&(self), (panic_message))

	/// @brief Returns the error stored in this `StdResult(T)`
	///
	/// Returns the error stored in this `StdResult(T)` if this is the `Err(T, error)` variant.
	/// If this is not the `Err(T, error)` variant, then this invokes a panic.
	///
	/// @param self - The `StdResult(T)` to get the error from
	///
	/// @return the contained error
	/// @note Panics if `self` does not contain an error
	/// @ingroup std_result
	#define std_result_unwrap_err(self) (self).m_vtable->unwrap_err(&(self))

	/// @brief Maps the value stored in this `StdResult(T)`
	///
	/// If `self` is `Ok`, maps the value stored in it by calling `map_func` on it.
	/// Otherwise, `self` is `Err` and this returns the error contained in it.
	///
	/// `map_func` must be a function type callable with `T` and returning `MapType`
	///
	/// @param self - The `StdResult(T)` to map
	/// @param MapType - The type to map to. This is the type that will be held
	/// by the returned `StdResult`, and `map_func` must return this type.
	/// @param map_func - The function to map the value stored in `self` to a `MapType`.
	///
	/// @return The `StdResult(MapType)` resulting from the mapping
	/// @ingroup std_result
	#define std_result_map(self, MapType, map_func)                               \
		({                                                                        \
			std_type_assert(typeof(map_func(std_result_unwrap(self))), MapType);  \
			StdResult(MapType) UNIQUE_VAR(res);                                   \
			if(std_result_is_ok(self)) {                                          \
				UNIQUE_VAR(res) = Ok(MapType, map_func(std_result_unwrap(self))); \
			}                                                                     \
			else {                                                                \
				UNIQUE_VAR(res) = Err(MapType, std_result_unwrap_err(self));      \
			}                                                                     \
			UNIQUE_VAR(res);                                                      \
		})

	/// @brief Maps the value stored in this `StdResult(T)`
	///
	/// If `self` is `Ok`, maps the value stored in it by calling `map_func` on it.
	/// Otherwise, returns `default_value`.
	///
	/// `map_func` must be a function type callable with `T` and returning the
	/// same type as `default_value`
	///
	/// @param self - The `StdResult(T)` to map.
	/// @param map_func - The function to map the value stored in `self`.
	/// @param default_value - The value to return if `self` is `Err`.
	///
	/// @return The value resulting from the mapping
	/// @ingroup std_result
	#define std_result_map_or(self, map_func, default_value)                                   \
		({                                                                                     \
			std_type_assert(typeof(map_func(std_result_unwrap(self))), typeof(default_value)); \
			typeof(default_value) UNIQUE_VAR(res);                                             \
			if(std_result_is_some(self)) {                                                     \
				UNIQUE_VAR(res) = map_func(std_result_unwrap(self));                           \
			}                                                                                  \
			else {                                                                             \
				UNIQUE_VAR(res) = (default_value);                                             \
			}                                                                                  \
			UNIQUE_VAR(res);                                                                   \
		})

	/// @brief Maps the value stored in this `StdResult(T)`
	///
	/// If `self` is `Ok`, maps the value stored in it by calling `map_func` on it.
	/// Otherwise, returns the result of calling `default_generator`.
	///
	/// `map_func` must be a function type callable with `T`.
	/// `default_generator` must be a function type taking no arguments.
	/// Both `map_func` and `default_generator` must return the same type.
	///
	/// @param self - The `StdResult(T)` to map.
	/// @param map_func - The function to map the value stored in `self`.
	/// @param default_generator - The function to generate the returned value if `self` is `Err`.
	///
	/// @return The value resulting from the mapping
	/// @ingroup std_result
	#define std_result_map_or_else(self, map_func, default_generator)  \
		({                                                             \
			std_type_assert(typeof(map_func(std_result_unwrap(self))), \
							typeof(default_generator()));              \
			typeof(default_generator()) UNIQUE_VAR(res);               \
			if(std_result_is_ok(self)) {                               \
				UNIQUE_VAR(res) = map_func(std_result_unwrap(self));   \
			}                                                          \
			else {                                                     \
				UNIQUE_VAR(res) = default_generator();                 \
			}                                                          \
			UNIQUE_VAR(res);                                           \
		})

	/// @brief Maps the error stored in this `StdResult(T)`
	///
	/// If `self` is `Err`, maps the error stored in it by calling `map_func` on it.
	/// Otherwise, returns `self`.
	///
	/// `map_func` must be a function returning a `StdError`
	///
	/// @param self - The `StdResult(T)` to map.
	/// @param map_func - The function to map the error stored in `self`.
	///
	/// @return The `StdResult(T)` resulting from the mapping
	/// @ingroup std_result
	#define std_result_map_err(self, map_func)                                                   \
		({                                                                                       \
			std_type_assert(typeof(map_func(std_result_unwrap_err(self))),                       \
							typeof(std_result_unwrap_err(self)));                                \
			UNIQUE_VAR(res) = self;                                                              \
			if(std_result_is_err(self)) {                                                        \
				UNIQUE_VAR(res).tag = Err;                                                       \
				UNIQUE_VAR(res).variant_identifier(Err) = map_func(std_result_unwrap_err(self)); \
			}                                                                                    \
			UNIQUE_VAR(res);                                                                     \
		})

	/// @brief Returns `result_b` is `self` is `Ok`. Otherwise returns the `Err` value of `self`.
	///
	/// @param self - The `StdResult(T)` to "and" with `result_b`.
	/// @param result_b - Another `StdResult`, of a potentially different `Ok` type to "and" with
	/// `self`
	///
	/// @return `result_b` if `self` is `Ok`. Otherwise the `Err` value of `self`
	/// @ingroup std_result
	#define std_result_and(self, result_b)                                 \
		({                                                                 \
			let_mut UNIQUE_VAR(res) = (result_b);                          \
			if(std_result_is_err(self)) {                                  \
				UNIQUE_VAR(res).variant_identifier(Ok)                     \
					= (typeof(UNIQUE_VAR(res).variant_identifier(Ok))){0}; \
				UNIQUE_VAR(res).tag = Err;                                 \
				UNIQUE_VAR(res).variant_identifier(Err)                    \
					= (typeof(UNIQUE_VAR(res).variant_identifier(Err))){   \
						std_result_unwrap_err(self)};                      \
			}                                                              \
			UNIQUE_VAR(res);                                               \
		})

	/// @brief Returns the result of calling `next_func` with the contained value if `self` is `Ok`.
	/// Otherwise, returns the `Err` value contained in `self`
	///
	/// @param self - The `StdResult(T)` to operate on.
	/// @param next_func - The function type to call with the value stored in `self`
	///
	/// @return The result of calling `next_func` with the value stored in `self`.
	/// Otherwise, returns the `Err` value contained in `self`
	/// @ingroup std_result
	#define std_result_and_then(self, next_func)                                          \
		({                                                                                \
			typeof(next_func(std_result_unwrap(self))) UNIQUE_VAR(res);                   \
			if(std_result_is_ok(self)) {                                                  \
				UNIQUE_VAR(res) = next_func(std_result_unwrap(self));                     \
			}                                                                             \
			else {                                                                        \
				UNIQUE_VAR(res).tag = Err;                                                \
				UNIQUE_VAR(res).variant_identifier(Err)._1 = std_result_unwrap_err(self); \
			}                                                                             \
			UNIQUE_VAR(res);                                                              \
		})

	/// @brief Returns `self` if it is `Ok`, otherwise returns `result_b`
	///
	/// @param self - The `StdResult(T)` to "or" with `result_b`
	/// @param result_b - Another `StdResult(T)` to "or" with `self`.
	///
	/// @return `self` if it is `Ok`. Otherwise, `result_b`
	/// @ingroup std_result
	#define std_result_or(self, result_b) (self).m_vtable->_or(&(self), (result_b))

	/// @brief Returns `self` if it is `Ok`, otherwise returns the result of calling `func`
	///
	/// @param self - The `StdResult(T)` to "or" with `result_b`
	/// @param func - The function to call if `self` is `Err`
	///
	/// @return `self` if it is `Ok`. Otherwise, `func()`
	/// @ingroup std_result
	#define std_result_or_else(self, func) (self).m_vtable->or_else(&(self), (func))

	/// @brief Converts this `StdResult(T)` to a `bool`
	///
	/// This is effectively the same as calling `std_result_is_ok(self)`, as it converts to a bool
	/// on the condition that this is the `Ok(T, value)` variant.
	///
	/// @param self - The `StdResult(T)` to convert to a `bool`
	///
	/// @return this `StdResult(T)` as a `bool`
	/// @ingroup std_result
	#define std_result_as_bool(self) (self).m_vtable->as_bool(&(self))

	#include <C2nxt/monadic/StdIfLet.h>

#endif // STD_RESULT_DEF
