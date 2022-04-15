/// @file ResultDef.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief This module provides macro definitions for implementing and working with `CnxResult(T)`
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

#include <Cnx/Assert.h>
#include <Cnx/Def.h>
#include <Cnx/Enum.h>

#ifndef CNX_RESULT_DEF
	#define CNX_RESULT_DEF

typedef enum Result {
	Ok,
	Err
} Result;

	/// @brief Macro Alias used to refer to an instantiation of `CnxResult(T)` for type `T`
	///
	/// Used to create and refer to a typedef for an instantiation of `CnxResult(T)` holding type
	/// `T`
	///
	/// @param T - The type held by the result
	/// @ingroup cnx_result
	#define CnxResult(T) CONCAT2(CnxResult, T)
	/// @brief Macro to create and refer to an identifier (function, variable, struct) related to an
	/// instantiation of `CnxResult(T)` of type `T`
	#define CnxResultIdentifier(T, Identifier) CONCAT2(cnx_result_, CONCAT3(T, _, Identifier))

	/// @brief Creates a `CnxResult(T)` holding the given value
	///
	/// @param T - The Ok type to store in the `CnxResult(T)`
	/// @param value - The Ok value to store in the `CnxResult(T)`
	///
	/// @return a `CnxResult` containing the given value
	/// @ingroup cnx_result
	#define Ok(T, value)                                   \
		((CnxResult(T)){.tag = Ok,                         \
						.variant_identifier(Ok) = {value}, \
						.m_vtable = &CnxResultIdentifier(T, vtable_impl)})

	/// @brief Creates a `CnxResult(T)` holding the given error
	///
	/// @param T - The Ok type to store in the `CnxResult(T)`
	/// @param error - The error to store in the `CnxResult(T)`
	///
	/// @return a `CnxResult` containing the given error
	/// @ingroup cnx_result
	#define Err(T, error)                                   \
		((CnxResult(T)){.tag = Err,                         \
						.variant_identifier(Err) = {error}, \
						.m_vtable = &CnxResultIdentifier(T, vtable_impl)})

	/// @brief Returns whether this `CnxResult(T)` is holding a value
	///
	/// This returns whether the `CnxResult(T)` is `Ok(T, value)`, ie the associated operation that
	/// created this `CnxResult(T)` was successful and this holds the resulting value.
	///
	/// @param self - The `CnxResult(T)` to check
	///
	/// @return `true` if this `CnxResult(T)` is holding a value, `false` otherwise
	/// @ingroup cnx_result
	#define cnx_result_is_ok(self) (self).m_vtable->is_ok(&(self))

	/// @brief Returns whether this `CnxResult(T)` is holding an error
	///
	/// This returns whether the `CnxResult(T)` is `Err(T, error)`, ie the associated operation that
	/// created this `CnxResult(T)` failed and his holds an error.
	///
	/// @param self - The `CnxResult(T)` to check
	///
	/// @return `true` if this `CnxResult(T)` is holding an error
	/// @ingroup cnx_result
	#define cnx_result_is_err(self) (self).m_vtable->is_err(&(self))

	/// @brief Returns a const reference to the value stored in this `CnxResult(T)`
	///
	/// Returns a const reference (an immediately dereferenced pointer-to-const) to the value stored
	///	in this `CnxResult(T)`. This requires that this **does** hold a value. If this is the
	///`Err(T, error)`
	/// variant, this function will invoke a panic.
	///
	/// @param self - The `CnxResult(T)` to get the stored value from
	///
	/// @return a const reference to the contained value
	/// @note Panics if `self` does not contain a value
	/// @ingroup cnx_result
	#define cnx_result_as_const(self) *((self).m_vtable->as_const(&(self)))

	/// @brief Returns a reference to the value stored in this `CnxResult(T)`
	///
	/// Returns a reference (an immediately dereferenced pointer) to the value stored in this
	/// `CnxResult(T)`. This requires that this **does** hold a value. If this is the
	/// `Err(T, error)` variant, this function will invoke a panic.
	///
	/// @param self - The `CnxResult(T)` to get the stored value from
	///
	/// @return a reference to the contained value
	/// @note Panics if `self` does not contain a value
	/// @ingroup cnx_result
	#define cnx_result_as_mut(self) *((self).m_vtable->as_mut(&(self)))

	/// @brief Returns the value stored in this `CnxResult(T)`
	///
	/// This requires that this **does** hold a value. If this is the `Err(T, error)` variant, this
	/// function will invoke a panic.
	///
	/// @param self - The `CnxResult(T)` to get the stored value from
	///
	/// @return the contained value
	/// @note Panics if `self` does not contain a value
	/// @ingroup cnx_result
	#define cnx_result_unwrap(self) (self).m_vtable->unwrap(&(self))

	/// @brief Returns the value stored in this `CnxResult(T)` , or `default_value`
	///
	/// If this `CnxResult(T)` is the `Ok(T, value)` variant, then this returns the stored value, if
	/// this is the `Err(T, error)` variant, then this returns `default_value`
	///
	/// @param self - The `CnxResult(T)` to get the stored value from
	/// @param default_value - The value to return if this is the `Err(T, error)` variant
	///
	/// @return the contained value, or `default_value`
	/// @ingroup cnx_result
	#define cnx_result_unwrap_or(self, default_value) \
		(self).m_vtable->unwrap_or(&(self), (default_value))

	/// @brief Returns the value stored in this `CnxResult(T)` , or `default_generator()`
	///
	/// If this `CnxResult(T)` is the `Ok(T, value)` variant, then this returns the stored value, if
	/// this is the `Err(T, error)` variant, then this returns the value generated by calling
	/// `default_generator`.
	///
	/// @param self - The `CnxResult(T)` to get the stored value from
	/// @param default_generator - The function to generate the value to return if this is the
	/// `Err(T, error)` variant
	///
	/// @return the contained value, or the one generated by `default_generator`
	/// @ingroup cnx_result
	#define cnx_result_unwrap_or_else(self, default_generator) \
		(self).m_vtable->unwrap_or_else(&(self), (default_generator))

	/// @brief Returns the value stored in this `CnxResult(T)`
	///
	/// Returns the value stored in this `CnxResult(T)` if this is the `Ok(T, value)` variant.
	/// Otherwise, invokes a panic with the given panic message.
	///
	/// @param self - The `CnxResult(T)` to get the stored value from
	/// @param panic_message - The custom panic message to use if `self` is not the `Ok(T, value)`
	/// variant
	///
	/// @return the contained value
	/// @note Panics if `self` does not contain a value, with the custom panic message
	/// `panic_message`
	/// @ingroup cnx_result
	#define cnx_result_expect(self, panic_message) (self).m_vtable->expect(&(self), (panic_message))

	/// @brief Returns the error stored in this `CnxResult(T)`
	///
	/// Returns the error stored in this `CnxResult(T)` if this is the `Err(T, error)` variant.
	/// If this is not the `Err(T, error)` variant, then this invokes a panic.
	///
	/// @param self - The `CnxResult(T)` to get the error from
	///
	/// @return the contained error
	/// @note Panics if `self` does not contain an error
	/// @ingroup cnx_result
	#define cnx_result_unwrap_err(self) (self).m_vtable->unwrap_err(&(self))

	/// @brief Maps the value stored in this `CnxResult(T)`
	///
	/// If `self` is `Ok`, maps the value stored in it by calling `map_func` on it.
	/// Otherwise, `self` is `Err` and this returns the error contained in it.
	///
	/// `map_func` must be a function type callable with `T` and returning `MapType`
	///
	/// @param self - The `CnxResult(T)` to map
	/// @param MapType - The type to map to. This is the type that will be held
	/// by the returned `CnxResult`, and `map_func` must return this type.
	/// @param map_func - The function to map the value stored in `self` to a `MapType`.
	///
	/// @return The `CnxResult(MapType)` resulting from the mapping
	/// @ingroup cnx_result
	#define cnx_result_map(self, MapType, map_func)                               \
		({                                                                        \
			cnx_type_assert(typeof(map_func(cnx_result_unwrap(self))), MapType);  \
			CnxResult(MapType) UNIQUE_VAR(res);                                   \
			if(cnx_result_is_ok(self)) {                                          \
				UNIQUE_VAR(res) = Ok(MapType, map_func(cnx_result_unwrap(self))); \
			}                                                                     \
			else {                                                                \
				UNIQUE_VAR(res) = Err(MapType, cnx_result_unwrap_err(self));      \
			}                                                                     \
			UNIQUE_VAR(res);                                                      \
		})

	/// @brief Maps the value stored in this `CnxResult(T)`
	///
	/// If `self` is `Ok`, maps the value stored in it by calling `map_func` on it.
	/// Otherwise, returns `default_value`.
	///
	/// `map_func` must be a function type callable with `T` and returning the
	/// same type as `default_value`
	///
	/// @param self - The `CnxResult(T)` to map.
	/// @param map_func - The function to map the value stored in `self`.
	/// @param default_value - The value to return if `self` is `Err`.
	///
	/// @return The value resulting from the mapping
	/// @ingroup cnx_result
	#define cnx_result_map_or(self, map_func, default_value)                                   \
		({                                                                                     \
			cnx_type_assert(typeof(map_func(cnx_result_unwrap(self))), typeof(default_value)); \
			typeof(default_value) UNIQUE_VAR(res);                                             \
			if(cnx_result_is_some(self)) {                                                     \
				UNIQUE_VAR(res) = map_func(cnx_result_unwrap(self));                           \
			}                                                                                  \
			else {                                                                             \
				UNIQUE_VAR(res) = (default_value);                                             \
			}                                                                                  \
			UNIQUE_VAR(res);                                                                   \
		})

	/// @brief Maps the value stored in this `CnxResult(T)`
	///
	/// If `self` is `Ok`, maps the value stored in it by calling `map_func` on it.
	/// Otherwise, returns the result of calling `default_generator`.
	///
	/// `map_func` must be a function type callable with `T`.
	/// `default_generator` must be a function type taking no arguments.
	/// Both `map_func` and `default_generator` must return the same type.
	///
	/// @param self - The `CnxResult(T)` to map.
	/// @param map_func - The function to map the value stored in `self`.
	/// @param default_generator - The function to generate the returned value if `self` is `Err`.
	///
	/// @return The value resulting from the mapping
	/// @ingroup cnx_result
	#define cnx_result_map_or_else(self, map_func, default_generator)  \
		({                                                             \
			cnx_type_assert(typeof(map_func(cnx_result_unwrap(self))), \
							typeof(default_generator()));              \
			typeof(default_generator()) UNIQUE_VAR(res);               \
			if(cnx_result_is_ok(self)) {                               \
				UNIQUE_VAR(res) = map_func(cnx_result_unwrap(self));   \
			}                                                          \
			else {                                                     \
				UNIQUE_VAR(res) = default_generator();                 \
			}                                                          \
			UNIQUE_VAR(res);                                           \
		})

	/// @brief Maps the error stored in this `CnxResult(T)`
	///
	/// If `self` is `Err`, maps the error stored in it by calling `map_func` on it.
	/// Otherwise, returns `self`.
	///
	/// `map_func` must be a function returning a `CnxError`
	///
	/// @param self - The `CnxResult(T)` to map.
	/// @param map_func - The function to map the error stored in `self`.
	///
	/// @return The `CnxResult(T)` resulting from the mapping
	/// @ingroup cnx_result
	#define cnx_result_map_err(self, map_func)                                                   \
		({                                                                                       \
			cnx_type_assert(typeof(map_func(cnx_result_unwrap_err(self))),                       \
							typeof(cnx_result_unwrap_err(self)));                                \
			UNIQUE_VAR(res) = self;                                                              \
			if(cnx_result_is_err(self)) {                                                        \
				UNIQUE_VAR(res).tag = Err;                                                       \
				UNIQUE_VAR(res).variant_identifier(Err) = map_func(cnx_result_unwrap_err(self)); \
			}                                                                                    \
			UNIQUE_VAR(res);                                                                     \
		})

	/// @brief Returns `result_b` is `self` is `Ok`. Otherwise returns the `Err` value of `self`.
	///
	/// @param self - The `CnxResult(T)` to "and" with `result_b`.
	/// @param result_b - Another `CnxResult`, of a potentially different `Ok` type to "and" with
	/// `self`
	///
	/// @return `result_b` if `self` is `Ok`. Otherwise the `Err` value of `self`
	/// @ingroup cnx_result
	#define cnx_result_and(self, result_b)                                 \
		({                                                                 \
			let_mut UNIQUE_VAR(res) = (result_b);                          \
			if(cnx_result_is_err(self)) {                                  \
				UNIQUE_VAR(res).variant_identifier(Ok)                     \
					= (typeof(UNIQUE_VAR(res).variant_identifier(Ok))){0}; \
				UNIQUE_VAR(res).tag = Err;                                 \
				UNIQUE_VAR(res).variant_identifier(Err)                    \
					= (typeof(UNIQUE_VAR(res).variant_identifier(Err))){   \
						cnx_result_unwrap_err(self)};                      \
			}                                                              \
			UNIQUE_VAR(res);                                               \
		})

	/// @brief Returns the result of calling `next_func` with the contained value if `self` is `Ok`.
	/// Otherwise, returns the `Err` value contained in `self`
	///
	/// @param self - The `CnxResult(T)` to operate on.
	/// @param next_func - The function type to call with the value stored in `self`
	///
	/// @return The result of calling `next_func` with the value stored in `self`.
	/// Otherwise, returns the `Err` value contained in `self`
	/// @ingroup cnx_result
	#define cnx_result_and_then(self, next_func)                                          \
		({                                                                                \
			typeof(next_func(cnx_result_unwrap(self))) UNIQUE_VAR(res);                   \
			if(cnx_result_is_ok(self)) {                                                  \
				UNIQUE_VAR(res) = next_func(cnx_result_unwrap(self));                     \
			}                                                                             \
			else {                                                                        \
				UNIQUE_VAR(res).tag = Err;                                                \
				UNIQUE_VAR(res).variant_identifier(Err)._1 = cnx_result_unwrap_err(self); \
			}                                                                             \
			UNIQUE_VAR(res);                                                              \
		})

	/// @brief Returns `self` if it is `Ok`, otherwise returns `result_b`
	///
	/// @param self - The `CnxResult(T)` to "or" with `result_b`
	/// @param result_b - Another `CnxResult(T)` to "or" with `self`.
	///
	/// @return `self` if it is `Ok`. Otherwise, `result_b`
	/// @ingroup cnx_result
	#define cnx_result_or(self, result_b) (self).m_vtable->_or(&(self), (result_b))

	/// @brief Returns `self` if it is `Ok`, otherwise returns the result of calling `func`
	///
	/// @param self - The `CnxResult(T)` to "or" with `result_b`
	/// @param func - The function to call if `self` is `Err`
	///
	/// @return `self` if it is `Ok`. Otherwise, `func()`
	/// @ingroup cnx_result
	#define cnx_result_or_else(self, func) (self).m_vtable->or_else(&(self), (func))

	/// @brief Converts this `CnxResult(T)` to a `bool`
	///
	/// This is effectively the same as calling `cnx_result_is_ok(self)`, as it converts to a bool
	/// on the condition that this is the `Ok(T, value)` variant.
	///
	/// @param self - The `CnxResult(T)` to convert to a `bool`
	///
	/// @return this `CnxResult(T)` as a `bool`
	/// @ingroup cnx_result
	#define cnx_result_as_bool(self) (self).m_vtable->as_bool(&(self))

	#include <Cnx/monadic/IfLet.h>

#endif // CNX_RESULT_DEF
