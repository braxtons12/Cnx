/// @file StdResult.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief This module provides a struct template for representing the value of a fallible operation
/// @version 0.1.1
/// @date 2022-01-07
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
#include <C2nxt/StdBasicTypes.h>
#include <C2nxt/StdError.h>

/// @ingroup error_handling
/// @{
/// @defgroup std_result StdResult
/// `StdResult(T)` is a "struct template" for representing the value of a fallible operation. It
/// enables a simple, type-safe way of returning, communicating, and handling the result of an
/// operation that can fail, without resorting to heap-allocation and pointers or out-parameters.
///
/// `StdResult(T)` allows for arbitrary user-defined errors through `StdError`, and shares some
/// functionality, such as `if_let(var, self)`, `with `StdOption(T)` (see @ref monadics).
///
/// Example:
///
/// @code {.c}
/// #define IMPORTANT_VALUE 3U
///
///	typedef enum ExampleErrorTypes {
///		EXAMPLE_SUCCESS = 0,
/// 	EXAMPLE_IMPORTANT_BAD_VALUE
/// } ExampleErrorTypes;
///
/// const_cstring example_error_message_function(i64 error_code) {
///		if(error_code == EXAMPLE_IMPORTANT_BAD_VALUE) {
///			return "Error: bad value for `important`";
/// 	}
/// 	else {
///			return "No error: Example successful";
/// 	}
/// }
///
/// [[maybe_unused]] static const StdErrorCategory example_error_category
/// 	= {.m_message_function = example_error_message_function};
///
/// StdResult(u32) operation_that_can_fail(u32 important, u32 right) {
///		if(important != IMPORTANT_VALUE) {
///			return Err(u32, std_error_new(EXAMPLE_IMPORTANT_BAD_VALUE, example_error_category));
/// 	}
/// 	else {
///			let res = // result of some operation with `important` and `right`...
///			return Ok(u32, res);
/// 	}
/// }
///
/// void example(void) {
///		let_mut val1 = IMPORTANT_VALUE;
/// 	let_mut val2 = 10U;
///		// do something with val1 and/or val2...
///		let_mut maybe_value = operation_that_can_fail(val1, val2);
///		if_let(value, maybe_value) {
///			// do something with value...
/// 	}
/// 	else {
/// 		// maybe_value was `Err(u32)`, do something to recover
/// 		// or report the error, eg:...
///			let err = std_result_unwrap_err(maybe_value);
/// 		eprintln("Error in example: {}", as_format_t(StdError, err));
///			// or, even more succinctly:
///			eprintln("Error in example: {}", as_format_t(StdResult(T), maybe_value));
/// 	}
/// }
/// @endcode
/// @}
#ifndef STD_RESULT
	/// @brief Declarations and definitions related to `StdResult(T)`
	#define STD_RESULT

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

	/// @brief Instantiates the declarations for `StdResult(T)` holding type `T`
	///
	/// Instantiates the struct, function, and variable declarations necessary for `StdResult(T)`
	/// holding type `T`
	///
	/// @param T - The type stored in the `StdResult(T)`
	/// @ingroup std_result
	#define DeclStdResult(T)                                                                     \
		typedef struct StdResult(T) StdResult(T);                                                \
                                                                                                 \
		typedef struct StdResultIdentifier(T, vtable) {                                          \
			bool (*const is_ok)(const StdResult(T)* restrict self);                              \
			bool (*const is_err)(const StdResult(T)* restrict self);                             \
			const T* (*const as_const)(const StdResult(T)* restrict self);                       \
			T* (*const as_mut)(StdResult(T)* restrict self);                                     \
			T (*const unwrap)(StdResult(T)* restrict self);                                      \
			T (*const unwrap_or)(StdResult(T)* restrict self, T default_value);                  \
			T(*const unwrap_or_else)                                                             \
			(StdResult(T)* restrict self, T(*default_generator)(void));                          \
			T(*const expect)                                                                     \
			(StdResult(T)* restrict self, restrict const_cstring panic_message);                 \
			StdError (*const unwrap_err)(StdResult(T)* restrict self);                           \
			bool (*const as_bool)(const StdResult(T)* restrict self);                            \
		}                                                                                        \
		StdResultIdentifier(T, vtable);                                                          \
                                                                                                 \
		/** @brief `StdResult` holding the type `T` **/                                          \
		typedef struct StdResult(T) {                                                            \
			const StdResultIdentifier(T, vtable) * m_vtable;                                     \
			bool m_is_ok;                                                                        \
			union {                                                                              \
				T m_ok;                                                                          \
				StdError m_error;                                                                \
			};                                                                                   \
		}                                                                                        \
		StdResult(T);                                                                            \
                                                                                                 \
		/** @brief Creates a `StdResult(T)` holding the given value **/                          \
		/** @param value - The value to store in the `StdResult` **/                             \
		/** @return a `StdResult` containing the given value **/                                 \
		StdResult(T) StdResultIdentifier(T, ok)(T value);                                        \
		/** @brief Creates a `StdResult(T)` holding the given error **/                          \
		/** @param error - The error to store in the `StdResult` **/                             \
		/** @return a `StdResult` containing the given error **/                                 \
		StdResult(T) StdResultIdentifier(T, err)(StdError error);                                \
		/** @brief Returns whether this `StdResult(T)` is holding a value **/                    \
		/** @param self - The `StdResult` to check **/                                           \
		/** @return `true` if this `StdResult(T)` is holding a value, `false` otherwise **/      \
		bool StdResultIdentifier(T, is_ok)(const StdResult(T)* restrict self);                   \
		/** @brief Returns whether this `StdResult(T)` __isn't__ holding a value (aka is holding \
		 * an error) **/                                                                         \
		/** @param self - The `StdResult` to check **/                                           \
		/** @return `true` if this `StdResult(T)` __isn't__ holding a value, `false` if it is    \
		 * holding an error **/                                                                  \
		bool StdResultIdentifier(T, is_err)(const StdResult(T)* restrict self);                  \
		/** @brief Returns a pointer to the const value stored in this `StdResult` **/           \
		/** @param self - The `StdResult` to get the stored value from **/                       \
		/** @return a pointer to the contained const value **/                                   \
		/** @note Panics if `self` does not contain a value **/                                  \
		const T* StdResultIdentifier(T, as_const)(const StdResult(T)* restrict self);            \
		/** @brief Returns a pointer to the value stored in this `StdResult` **/                 \
		/** @param self - The `StdResult` to get the stored value from **/                       \
		/** @return a pointer to the contained value **/                                         \
		/** @note Panics if `self` does not contain a value **/                                  \
		T* StdResultIdentifier(T, as_mut)(StdResult(T)* restrict self);                          \
		/** @brief Returns the value stored in this `StdResult` **/                              \
		/** @param self - The `StdResult` to get the stored value from **/                       \
		/** @return the contained value **/                                                      \
		/** @note Panics if `self` does not contain a value **/                                  \
		T StdResultIdentifier(T, unwrap)(StdResult(T)* restrict self);                           \
		/** @brief Returns the value stored in this `StdResult` , or `default_value` **/         \
		/** if this is `None` **/                                                                \
		/** @param self - The `StdResult` to get the stored value from **/                       \
		/** @param default_value - The value to return if this is `None` **/                     \
		/** @return the contained value, or `default_value` **/                                  \
		T StdResultIdentifier(T, unwrap_or)(StdResult(T)* restrict self, T default_value);       \
		/** @brief Returns the value stored in this `StdResult` , or the value returned by **/   \
		/** `default_generator` if this is `None` **/                                            \
		/** @param self - The `StdResult` to get the stored value from **/                       \
		/** @param default_generator - The function to generate the value **/                    \
		/**  to return if this is `None` **/                                                     \
		/** @return the contained value, or the one generated by `default_generator` **/         \
		T StdResultIdentifier(T, unwrap_or_else)(StdResult(T)* restrict self,                    \
												 T(*default_generator)(void));                   \
		/** @brief Returns the value stored in this `StdResult` **/                              \
		/** @param self - The `StdResult` to get the stored value from **/                       \
		/** @return the contained value **/                                                      \
		/** @note Panics if `self` does not contain a value, **/                                 \
		/** with the custom panic message `panic_message` **/                                    \
		T StdResultIdentifier(T, expect)(StdResult(T)* restrict self,                            \
										 restrict const_cstring panic_mesage);                   \
		/** @brief Returns the error stored in this `StdResult` **/                              \
		/** @param self - The `StdResult` to get the stored value from **/                       \
		/** @return the contained value **/                                                      \
		/** @note Panics if `self` does not contain an error **/                                 \
		StdError StdResultIdentifier(T, unwrap_err)(StdResult(T)* restrict self);                \
                                                                                                 \
		bool StdResultIdentifier(T, as_bool)(const StdResult(T)* restrict self);

	/// @brief Instantiates the declarations for implementing the `StdFormat` trait for
	/// `StdResult(T)` holding type `T`
	///
	/// Instantiates the struct, function, and variable declarations necessary for implementing
	/// `StdFormat` for  `StdResult(T)`. This is separate from the main declaration macro,
	/// to make `StdFormat` support for `StdResult(T)` optional (for cases where `T` does not
	/// support `StdFormat`)
	///
	/// @param T - The type stored in the `StdResult(T)`
	/// @ingroup std_result
	#define DeclStdResultFormat(T)                                                              \
                                                                                                \
		StdString StdResultIdentifier(T, format)(const StdFormat* restrict self,                \
												 StdFormatSpecifier specifier);                 \
                                                                                                \
		StdString StdResultIdentifier(T, format_with_allocator)(const StdFormat* restrict self, \
																StdFormatSpecifier specifier,   \
																StdAllocator allocator);        \
                                                                                                \
		[[maybe_unused]] static ImplTraitFor(StdFormat,                                         \
											 StdResult(T),                                      \
											 StdResultIdentifier(T, format),                    \
											 StdResultIdentifier(T, format_with_allocator));

	/// @brief Creates a `StdResult(T)` holding the given value
	///
	/// @param T - The Ok type to store in the `StdResult(T)`
	/// @param value - The Ok value to store in the `StdResult(T)`
	///
	/// @return a `StdResult` containing the given value
	/// @ingroup std_result
	#define Ok(T, value) StdResultIdentifier(T, ok)(value)
	/// @brief Creates a `StdResult(T)` holding the given error
	///
	/// @param T - The Ok type to store in the `StdResult(T)`
	/// @param error - The error to store in the `StdResult(T)`
	///
	/// @return a `StdResult` containing the given error
	/// @ingroup std_result
	#define Err(T, error) StdResultIdentifier(T, err)(error)
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

	#include "monadic/StdIfLet.h"

	/// @brief Instantiates the definitions for `StdResult(T)` holding type `T`
	///
	/// Instantiates the function and variable definitions required for  `StdResult(T)` holding type
	/// `T`
	///
	/// @param T - The type stored in the `StdResult(T)`
	/// @ingroup std_result
	#define ImplStdResult(T)                                                                   \
                                                                                               \
		static const struct StdResultIdentifier(T, vtable) StdResultIdentifier(T, vtable_impl) \
			= {.is_ok = StdResultIdentifier(T, is_ok),                                         \
			   .is_err = StdResultIdentifier(T, is_err),                                       \
			   .as_const = StdResultIdentifier(T, as_const),                                   \
			   .as_mut = StdResultIdentifier(T, as_mut),                                       \
			   .unwrap = StdResultIdentifier(T, unwrap),                                       \
			   .unwrap_or = StdResultIdentifier(T, unwrap_or),                                 \
			   .unwrap_or_else = StdResultIdentifier(T, unwrap_or_else),                       \
			   .expect = StdResultIdentifier(T, expect),                                       \
			   .unwrap_err = StdResultIdentifier(T, unwrap_err),                               \
			   .as_bool = StdResultIdentifier(T, as_bool)};                                    \
                                                                                               \
		StdResult(T) StdResultIdentifier(T, ok)(T value) {                                     \
			return (StdResult(T)){.m_is_ok = true,                                             \
								  .m_ok = value,                                               \
								  .m_vtable = &StdResultIdentifier(T, vtable_impl)};           \
		}                                                                                      \
                                                                                               \
		StdResult(T) StdResultIdentifier(T, err)(StdError error) {                             \
			return (StdResult(T)){.m_is_ok = false,                                            \
								  .m_error = error,                                            \
								  .m_vtable = &StdResultIdentifier(T, vtable_impl)};           \
		}                                                                                      \
                                                                                               \
		bool StdResultIdentifier(T, is_ok)(const StdResult(T)* restrict self) {                \
			return self->m_is_ok;                                                              \
		}                                                                                      \
                                                                                               \
		bool StdResultIdentifier(T, is_err)(const StdResult(T)* restrict self) {               \
			return !self->m_is_ok;                                                             \
		}                                                                                      \
                                                                                               \
		const T* StdResultIdentifier(T, as_const)(const StdResult(T)* restrict self) {         \
			if(!std_result_is_ok(*self)) {                                                     \
				std_panic("as_const called on an Err, terminating");                           \
			}                                                                                  \
                                                                                               \
			return &(self->m_ok);                                                              \
		}                                                                                      \
                                                                                               \
		T* StdResultIdentifier(T, as_mut)(StdResult(T)* restrict self) {                       \
			if(!std_result_is_ok(*self)) {                                                     \
				std_panic("as_mut called on an Err, terminating");                             \
			}                                                                                  \
                                                                                               \
			return &(self->m_ok);                                                              \
		}                                                                                      \
                                                                                               \
		T StdResultIdentifier(T, unwrap)(StdResult(T)* restrict self) {                        \
			if(!std_result_is_ok(*self)) {                                                     \
				std_panic("unwrap called on an Err, terminating");                             \
			}                                                                                  \
                                                                                               \
			return self->m_ok;                                                                 \
		}                                                                                      \
                                                                                               \
		T StdResultIdentifier(T, unwrap_or)(StdResult(T)* restrict self, T default_value) {    \
			if(!std_result_is_ok(*self)) {                                                     \
				return default_value;                                                          \
			}                                                                                  \
                                                                                               \
			return self->m_ok;                                                                 \
		}                                                                                      \
                                                                                               \
		T StdResultIdentifier(T, unwrap_or_else)(StdResult(T)* restrict self,                  \
												 T(*default_generator)(void)) {                \
			if(!std_result_is_ok(*self)) {                                                     \
				return default_generator();                                                    \
			}                                                                                  \
                                                                                               \
			return self->m_ok;                                                                 \
		}                                                                                      \
                                                                                               \
		T StdResultIdentifier(T, expect)(StdResult(T)* restrict self,                          \
										 restrict const_cstring panic_message) {               \
			if(!std_result_is_ok(*self)) {                                                     \
				std_panic(panic_message);                                                      \
			}                                                                                  \
                                                                                               \
			return self->m_ok;                                                                 \
		}                                                                                      \
                                                                                               \
		StdError StdResultIdentifier(T, unwrap_err)(StdResult(T)* restrict self) {             \
			if(!std_result_is_err(*self)) {                                                    \
				std_panic("unwrap_err called on an Ok, terminating");                          \
			}                                                                                  \
                                                                                               \
			return self->m_error;                                                              \
		}                                                                                      \
                                                                                               \
		bool StdResultIdentifier(T, as_bool)(const StdResult(T)* restrict self) {              \
			return std_result_is_ok(*self);                                                    \
		}

	/// @brief Instantiates the definitions for implementing the `StdFormat` trait for
	/// `StdResult(T)` holding type `T`
	///
	/// Instantiates the function definitions required for implementing `StdFormat` for
	/// `StdResult(T)`. This is separate from the main implementation macro,
	//	/// to make `StdFormat` support for `StdResult(T)` optional (for cases where `T` does not
	//	/// support `StdFormat`)
	///
	/// @param T - The type stored in the `StdResult(T)`
	/// @param TypeToFormatAs - The type of the `StdFormat` implementation to use to format `T`
	/// Not all types can generically map correctly (e.g. `usize` will map to `u64` or `u32` in a
	/// `_Generic`, breaking deduction), so this is used to ensure correct type matching
	/// @ingroup std_result
	#define ImplStdResultFormat(T, TypeToFormatAs)                                              \
		StdString StdResultIdentifier(T, format)(const StdFormat* restrict self,                \
												 StdFormatSpecifier specifier) {                \
			return StdResultIdentifier(T, format_with_allocator)(self,                          \
																 specifier,                     \
																 std_allocator_new());          \
		}                                                                                       \
                                                                                                \
		StdString StdResultIdentifier(T, format_with_allocator)(                                \
			const StdFormat* restrict self,                                                     \
			[[maybe_unused]] StdFormatSpecifier specifier,                                      \
			StdAllocator allocator) {                                                           \
                                                                                                \
			std_assert(specifier.m_type == STD_FORMAT_TYPE_DEFAULT                              \
						   || specifier.m_type == STD_FORMAT_TYPE_DEBUG,                        \
					   "Can only format StdResult with default or debug format specifier");     \
                                                                                                \
			let _self = static_cast(const StdResult(T)*)(self->m_self);                         \
			if(specifier.m_type == STD_FORMAT_TYPE_DEBUG) {                                     \
				if(_self->m_is_ok) {                                                            \
					return std_format_with_allocator(                                           \
						AS_STRING(StdResult(T)) ": [is_ok: {D}, ok: {D}]",                      \
						allocator,                                                              \
						_self->m_is_ok,                                                         \
						as_format_t(TypeToFormatAs, _self->m_ok));                              \
				}                                                                               \
				else { /** NOLINT(readability-else-after-return) **/                            \
					return std_format_with_allocator(                                           \
						AS_STRING(StdResult(T)) ": [is_ok: {D}, error: {D}]",                   \
						allocator,                                                              \
						_self->m_is_ok,                                                         \
						as_format_t(StdError, _self->m_error));                                 \
				}                                                                               \
			}                                                                                   \
			else {                                                                              \
				if(_self->m_is_ok) {                                                            \
					return std_format_with_allocator("Ok: {}",                                  \
													 allocator,                                 \
													 as_format_t(TypeToFormatAs, _self->m_ok)); \
				}                                                                               \
				else { /** NOLINT(readability-else-after-return) **/                            \
					return std_format_with_allocator("Error: {}",                               \
													 allocator,                                 \
													 _self->m_is_ok,                            \
													 as_format_t(StdError, _self->m_error));    \
				}                                                                               \
			}                                                                                   \
		}

	/// @brief Fully instantiates `StdResult(T)` holding type `T`
	///
	/// Instantiates all of the declarations and definitions necessary for `StdResult(T)` holding
	/// type `T`
	///
	/// @param T - The type stored in the `StdResult`
	/// @ingroup std_result
	#define DeclAndImplStdResult(T) DeclStdResult(T) ImplStdResult(T)

// `StdResult` implementations for built-in datatypes

/// @brief Instantiate `StdResult(T)` for the builtin `bool`
DeclStdResult(bool);
/// @brief Instantiate `StdResult(T)` for the builtin `char`
DeclStdResult(char);
/// @brief Instantiate `StdResult(T)` for the builtin `u8`
DeclStdResult(u8);
/// @brief Instantiate `StdResult(T)` for the builtin `u16`
DeclStdResult(u16);
/// @brief Instantiate `StdResult(T)` for the builtin `u32`
DeclStdResult(u32);
/// @brief Instantiate `StdResult(T)` for the builtin `u64`
DeclStdResult(u64);
/// @brief Instantiate `StdResult(T)` for the builtin `usize`
DeclStdResult(usize);
/// @brief Instantiate `StdResult(T)` for the builtin `i8`
DeclStdResult(i8);
/// @brief Instantiate `StdResult(T)` for the builtin `i16`
DeclStdResult(i16);
/// @brief Instantiate `StdResult(T)` for the builtin `i32`
DeclStdResult(i32);
/// @brief Instantiate `StdResult(T)` for the builtin `i64`
DeclStdResult(i64);
/// @brief Instantiate `StdResult(T)` for the builtin `isize`
DeclStdResult(isize);
/// @brief Instantiate `StdResult(T)` for the builtin `f32`
DeclStdResult(f32);
/// @brief Instantiate `StdResult(T)` for the builtin `f64`
DeclStdResult(f64);
/// @brief Instantiate `StdResult(T)` for the builtin `u8_ptr`
DeclStdResult(u8_ptr);
/// @brief Instantiate `StdResult(T)` for the builtin `u16_ptr`
DeclStdResult(u16_ptr);
/// @brief Instantiate `StdResult(T)` for the builtin `u32_ptr`
DeclStdResult(u32_ptr);
/// @brief Instantiate `StdResult(T)` for the builtin `u64_ptr`
DeclStdResult(u64_ptr);
/// @brief Instantiate `StdResult(T)` for the builtin `usize_ptr`
DeclStdResult(usize_ptr);
/// @brief Instantiate `StdResult(T)` for the builtin `i8_ptr`
DeclStdResult(i8_ptr);
/// @brief Instantiate `StdResult(T)` for the builtin `i16_ptr`
DeclStdResult(i16_ptr);
/// @brief Instantiate `StdResult(T)` for the builtin `i32_ptr`
DeclStdResult(i32_ptr);
/// @brief Instantiate `StdResult(T)` for the builtin `i64_ptr`
DeclStdResult(i64_ptr);
/// @brief Instantiate `StdResult(T)` for the builtin `isize_ptr`
DeclStdResult(isize_ptr);
/// @brief Instantiate `StdResult(T)` for the builtin `f32_ptr`
DeclStdResult(f32_ptr);
/// @brief Instantiate `StdResult(T)` for the builtin `f64_ptr`
DeclStdResult(f64_ptr);
/// @brief Instantiate `StdResult(T)` for the builtin `cstring`
DeclStdResult(cstring);
/// @brief Instantiate `StdResult(T)` for the builtin `const_cstring`
DeclStdResult(const_cstring);
/// @brief Instantiate `StdResult(T)` for the builtin `char_ptr`
DeclStdResult(char_ptr);
/// @brief Instantiate `StdResult(T)` for the builtin `const_char_ptr`
DeclStdResult(const_char_ptr);
/// @brief Instantiate `StdResult(T)` for the builtin `StdString`
DeclStdResult(StdString);
/// @brief Instantiate `StdResult(T)` for the builtin `StdStringView`
DeclStdResult(StdStringView);

// @brief Instantiate `StdFormat` implementation for `StdResult(T)` for the builtin `bool`
DeclStdResultFormat(bool);
// @brief Instantiate `StdFormat` implementation for `StdResult(T)` for the builtin `char`
DeclStdResultFormat(char);
// @brief Instantiate `StdFormat` implementation for `StdResult(T)` for the builtin `u8`
DeclStdResultFormat(u8);
// @brief Instantiate `StdFormat` implementation for `StdResult(T)` for the builtin `u16`
DeclStdResultFormat(u16);
// @brief Instantiate `StdFormat` implementation for `StdResult(T)` for the builtin `u32`
DeclStdResultFormat(u32);
// @brief Instantiate `StdFormat` implementation for `StdResult(T)` for the builtin `u64`
DeclStdResultFormat(u64);
// @brief Instantiate `StdFormat` implementation for `StdResult(T)` for the builtin `usize`
DeclStdResultFormat(usize);
// @brief Instantiate `StdFormat` implementation for `StdResult(T)` for the builtin `i8`
DeclStdResultFormat(i8);
// @brief Instantiate `StdFormat` implementation for `StdResult(T)` for the builtin `i16`
DeclStdResultFormat(i16);
// @brief Instantiate `StdFormat` implementation for `StdResult(T)` for the builtin `i32`
DeclStdResultFormat(i32);
// @brief Instantiate `StdFormat` implementation for `StdResult(T)` for the builtin `i64`
DeclStdResultFormat(i64);
// @brief Instantiate `StdFormat` implementation for `StdResult(T)` for the builtin `isize`
DeclStdResultFormat(isize);
// @brief Instantiate `StdFormat` implementation for `StdResult(T)` for the builtin `f32`
DeclStdResultFormat(f32);
// @brief Instantiate `StdFormat` implementation for `StdResult(T)` for the builtin `f64`
DeclStdResultFormat(f64);
// @brief Instantiate `StdFormat` implementation for `StdResult(T)` for the builtin `u8_ptr`
DeclStdResultFormat(u8_ptr);
// @brief Instantiate `StdFormat` implementation for `StdResult(T)` for the builtin `u16_ptr`
DeclStdResultFormat(u16_ptr);
// @brief Instantiate `StdFormat` implementation for `StdResult(T)` for the builtin `u32_ptr`
DeclStdResultFormat(u32_ptr);
// @brief Instantiate `StdFormat` implementation for `StdResult(T)` for the builtin `u64_ptr`
DeclStdResultFormat(u64_ptr);
// @brief Instantiate `StdFormat` implementation for `StdResult(T)` for the builtin `usize_ptr`
DeclStdResultFormat(usize_ptr);
// @brief Instantiate `StdFormat` implementation for `StdResult(T)` for the builtin `i8_ptr`
DeclStdResultFormat(i8_ptr);
// @brief Instantiate `StdFormat` implementation for `StdResult(T)` for the builtin `i16_ptr`
DeclStdResultFormat(i16_ptr);
// @brief Instantiate `StdFormat` implementation for `StdResult(T)` for the builtin `i32_ptr`
DeclStdResultFormat(i32_ptr);
// @brief Instantiate `StdFormat` implementation for `StdResult(T)` for the builtin `i64_ptr`
DeclStdResultFormat(i64_ptr);
// @brief Instantiate `StdFormat` implementation for `StdResult(T)` for the builtin `isize_ptr`
DeclStdResultFormat(isize_ptr);
// @brief Instantiate `StdFormat` implementation for `StdResult(T)` for the builtin `f32_ptr`
DeclStdResultFormat(f32_ptr);
// @brief Instantiate `StdFormat` implementation for `StdResult(T)` for the builtin `f64_ptr`
DeclStdResultFormat(f64_ptr);
// @brief Instantiate `StdFormat` implementation for `StdResult(T)` for the builtin `cstring`
DeclStdResultFormat(cstring);
// @brief Instantiate `StdFormat` implementation for `StdResult(T)` for the builtin `const_cstring`
DeclStdResultFormat(const_cstring);
// @brief Instantiate `StdFormat` implementation for `StdResult(T)` for the builtin `char_ptr`
DeclStdResultFormat(char_ptr);
// @brief Instantiate `StdFormat` implementation for `StdResult(T)` for the builtin `const_char_ptr`
DeclStdResultFormat(const_char_ptr);
// @brief Instantiate `StdFormat` implementation for `StdResult(T)` for the builtin `StdString`
DeclStdResultFormat(StdString);
// @brief Instantiate `StdFormat` implementation for `StdResult(T)` for the builtin `StdStringView`
DeclStdResultFormat(StdStringView);
#endif // STD_RESULT
