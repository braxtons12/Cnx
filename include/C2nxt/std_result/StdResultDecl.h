/// @file StdResultDecl.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief This module provides the function declarations and type definitions for a template
/// instantiation of `StdResult(RESULT_T)`
/// @version 0.2.3
/// @date 2022-03-20
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

#include <C2nxt/std_result/StdResultDef.h>

#if defined(RESULT_T) && RESULT_DECL

	#if STD_TEMPLATE_SUPPRESS_INSTANTIATIONS
		#undef STD_TEMPLATE_SUPPRESS_INSTANTIATIONS
		#include <C2nxt/StdError.h>
		#define STD_TEMPLATE_SUPPRESS_INSTANTIATIONS TRUE
	#else
		#include <C2nxt/StdError.h>
	#endif // STD_TEMPLATE_SUPPRESS_INSTANTIATIONS

	#if !STD_TEMPLATE_SUPPRESS_INSTANTIATIONS
		#define SHOULD_UNDEF_SUPPRESS_INSTANTIATIONS TRUE
		#define STD_TEMPLATE_SUPPRESS_INSTANTIATIONS TRUE
	#endif // STD_TEMPLATE_SUPPRESS_INSTANTIATIONS

	#include <C2nxt/StdAllocators.h>
	#include <C2nxt/StdAssert.h>
	#include <C2nxt/StdBasicTypes.h>
	#include <C2nxt/monadic/StdIfLet.h>
	#include <C2nxt/StdEnum.h>

typedef struct StdResultIdentifier(RESULT_T, vtable) StdResultIdentifier(RESULT_T, vtable);

EnumWithTag(StdResult(RESULT_T),
			Result,
			(Ok, RESULT_T),
			(Err, StdError),
			const StdResultIdentifier(RESULT_T, vtable) * m_vtable);

	#define ___DISABLE_IF_NULL(self) \
		std_disable_if(!(self), "Can't perform an operator on a null result")

/// @brief Returns whether this `StdResult(RESULT_T)` is holding a value
/// @param self - The `StdResult` to check
/// @return `true` if this `StdResult(RESULT_T)` is holding a value, `false` otherwise
[[nodiscard]] [[not_null(1)]] bool
	StdResultIdentifier(RESULT_T, is_ok)(const StdResult(RESULT_T) * restrict self)
		___DISABLE_IF_NULL(self);
/// @brief Returns whether this `StdResult(RESULT_T)` __isn't__ holding a value (aka is holding an
/// error)
/// @param self - The `StdResult` to check
/// @return `true` if this `StdResult(RESULT_T)` __isn't__ holding a value,
/// `false` if it is  holding an error
[[nodiscard]] [[not_null(1)]] bool
	StdResultIdentifier(RESULT_T, is_err)(const StdResult(RESULT_T) * restrict self)
		___DISABLE_IF_NULL(self);
/// @brief Returns a pointer to the const value stored in this `StdResult`
/// @param self - The `StdResult` to get the stored value from
/// @return a pointer to the contained const value
/// @note Panics if `self` does not contain a value
[[nodiscard]] [[not_null(1)]] [[returns_not_null]] const RESULT_T*
	StdResultIdentifier(RESULT_T, as_const)(const StdResult(RESULT_T) * restrict self)
		___DISABLE_IF_NULL(self);
/// @brief Returns a pointer to the value stored in this `StdResult`
/// @param self - The `StdResult` to get the stored value from
/// @return a pointer to the contained value
/// @note Panics if `self` does not contain a value
[[nodiscard]] [[not_null(1)]] [[returns_not_null]] RESULT_T*
	StdResultIdentifier(RESULT_T, as_mut)(StdResult(RESULT_T) * restrict self)
		___DISABLE_IF_NULL(self);
/// @brief Returns the value stored in this `StdResult`
/// @param self - The `StdResult` to get the stored value from
/// @return the contained value
/// @note Panics if `self` does not contain a value
[[nodiscard]] [[not_null(1)]] RESULT_T
	StdResultIdentifier(RESULT_T, unwrap)(StdResult(RESULT_T) * restrict self)
		___DISABLE_IF_NULL(self);
/// @brief Returns the value stored in this `StdResult` , or `default_value`
/// if this is `None`
/// @param self - The `StdResult` to get the stored value from
/// @param default_value - The value to return if this is `None`
/// @return the contained value, or `default_value`
[[nodiscard]] [[not_null(1)]] RESULT_T
	StdResultIdentifier(RESULT_T, unwrap_or)(StdResult(RESULT_T) * restrict self,
											 RESULT_T default_value) ___DISABLE_IF_NULL(self);
/// @brief Returns the value stored in this `StdResult` , or the value returned by
/// `default_generator` if this is `None`
/// @param self - The `StdResult` to get the stored value from
/// @param default_generator - The function to generate the value to return if this is `None`
/// @return the contained value, or the one generated by `default_generator`
[[nodiscard]] [[not_null(1, 2)]] RESULT_T
	StdResultIdentifier(RESULT_T, unwrap_or_else)(StdResult(RESULT_T) * restrict self,
												  RESULT_T (*const default_generator)(void))
		___DISABLE_IF_NULL(self)
			std_disable_if(!default_generator, "default_generator cannot be null");
/// @brief Returns the value stored in this `StdResult`
/// @param self - The `StdResult` to get the stored value from
/// @return the contained value
/// @note Panics if `self` does not contain a value, with the custom panic message `panic_message`
[[nodiscard]] [[not_null(1, 2)]] RESULT_T
	StdResultIdentifier(RESULT_T, expect)(StdResult(RESULT_T) * restrict self,
										  restrict const_cstring panic_message)
		___DISABLE_IF_NULL(self) std_disable_if(!panic_message, "panic_message cannot be null");
/// @brief Returns the error stored in this `StdResult`
/// @param self - The `StdResult` to get the stored value from
/// @return the contained value
/// @note Panics if `self` does not contain an error
StdError StdResultIdentifier(RESULT_T, unwrap_err)(StdResult(RESULT_T) * restrict self);

/// @brief Returns `self` if it is `Ok`, otherwise returns `result_b`
///
/// @param self - The `StdResult(RESULT_T)` to "or" with `result_b`
/// @param result_b - Another `StdResult(RESULT_T)` to "or" with `self`.
///
/// @return `self` if it is `Ok`. Otherwise, `result_b`
/// @ingroup std_result
StdResult(RESULT_T) StdResultIdentifier(RESULT_T, or)(const StdResult(RESULT_T) * restrict self,
													  StdResult(RESULT_T) option_b);

/// @brief Returns `self` if it is `Ok`, otherwise returns the result of calling `func`
///
/// @param self - The `StdResult(RESULT_T)` to "or" with `result_b`
/// @param func - The function to call if `self` is `Err`
///
/// @return `self` if it is `Ok`. Otherwise, `func()`
/// @ingroup std_result
StdResult(RESULT_T)
	StdResultIdentifier(RESULT_T, or_else)(const StdResult(RESULT_T) * restrict self,
										   StdResult(RESULT_T) (*const func)(void));

/// @brief Converts this `StdResult(RESULT_T)` to a `bool`
///
/// This is effectively the same as calling `std_result_is_ok(self)`, as it converts to a bool
/// on the condition that this is the `Ok(T, value)` variant.
///
/// @param self - The `StdResult(RESULT_T)` to convert to a `bool`
///
/// @return this `StdResult(RESULT_T)` as a `bool`
/// @ingroup std_result
bool StdResultIdentifier(RESULT_T, as_bool)(const StdResult(RESULT_T) * restrict self);

typedef struct StdResultIdentifier(RESULT_T, vtable) {
	bool (*const is_ok)(const StdResult(RESULT_T)* restrict self);
	bool (*const is_err)(const StdResult(RESULT_T)* restrict self);
	const RESULT_T* (*const as_const)(const StdResult(RESULT_T)* restrict self);
	RESULT_T* (*const as_mut)(StdResult(RESULT_T)* restrict self);
	RESULT_T (*const unwrap)(StdResult(RESULT_T)* restrict self);
	RESULT_T (*const unwrap_or)(StdResult(RESULT_T)* restrict self, RESULT_T default_value);
	RESULT_T(*const unwrap_or_else)
	(StdResult(RESULT_T)* restrict self, RESULT_T(*default_generator)(void));
	RESULT_T(*const expect)
	(StdResult(RESULT_T)* restrict self, restrict const_cstring panic_message);
	StdError (*const unwrap_err)(StdResult(RESULT_T)* restrict self);
	StdResult(RESULT_T) (*const _or)(const StdResult(RESULT_T)* restrict self,
									 StdResult(RESULT_T) option_b);
	StdResult(RESULT_T) (*const or_else)(const StdResult(RESULT_T)* restrict self,
										 StdResult(RESULT_T) (*const func)(void));
	bool (*const as_bool)(const StdResult(RESULT_T)* restrict self);
}
StdResultIdentifier(RESULT_T, vtable);

[[maybe_unused]] static const StdResultIdentifier(RESULT_T, vtable)
	StdResultIdentifier(RESULT_T, vtable_impl)
	= {.is_ok = StdResultIdentifier(RESULT_T, is_ok),
	   .is_err = StdResultIdentifier(RESULT_T, is_err),
	   .as_const = StdResultIdentifier(RESULT_T, as_const),
	   .as_mut = StdResultIdentifier(RESULT_T, as_mut),
	   .unwrap = StdResultIdentifier(RESULT_T, unwrap),
	   .unwrap_or = StdResultIdentifier(RESULT_T, unwrap_or),
	   .unwrap_or_else = StdResultIdentifier(RESULT_T, unwrap_or_else),
	   .expect = StdResultIdentifier(RESULT_T, expect),
	   .unwrap_err = StdResultIdentifier(RESULT_T, unwrap_err),
	   ._or = StdResultIdentifier(RESULT_T, or),
	   .or_else = StdResultIdentifier(RESULT_T, or_else),
	   .as_bool = StdResultIdentifier(RESULT_T, as_bool)};

	#undef ___DISABLE_IF_NULL

	#if SHOULD_UNDEF_SUPPRESS_INSTANTIATIONS
		#undef STD_TEMPLATE_SUPPRESS_INSTANTIATIONS
	#endif // SHOULD_UNDEF_SUPPRESS_INSTANTIATIONS

#endif // defined(RESULT_T) && RESULT_DECL
