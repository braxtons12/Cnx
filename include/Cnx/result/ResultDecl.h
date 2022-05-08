/// @file ResultDecl.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief This module provides the function declarations and type definitions for a template
/// instantiation of `CnxResult(RESULT_T)`
/// @version 0.2.4
/// @date 2022-05-08
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

#include <Cnx/result/ResultDef.h>

#if defined(RESULT_T) && RESULT_DECL

	#if CNX_TEMPLATE_SUPPRESS_INSTANTIATIONS
		#undef CNX_TEMPLATE_SUPPRESS_INSTANTIATIONS
		#include <Cnx/Error.h>
		#define CNX_TEMPLATE_SUPPRESS_INSTANTIATIONS TRUE
	#else
		#include <Cnx/Error.h>
	#endif // CNX_TEMPLATE_SUPPRESS_INSTANTIATIONS

	#if !CNX_TEMPLATE_SUPPRESS_INSTANTIATIONS
		#define SHOULD_UNDEF_SUPPRESS_INSTANTIATIONS TRUE
		#define CNX_TEMPLATE_SUPPRESS_INSTANTIATIONS TRUE
	#endif // CNX_TEMPLATE_SUPPRESS_INSTANTIATIONS

	#include <Cnx/Allocators.h>
	#include <Cnx/Assert.h>
	#include <Cnx/BasicTypes.h>
	#include <Cnx/monadic/IfLet.h>
	#include <Cnx/Enum.h>

typedef struct CnxResultIdentifier(RESULT_T, vtable) CnxResultIdentifier(RESULT_T, vtable);

EnumWithTag(CnxResult(RESULT_T),
			Result,
			(Ok, RESULT_T),
			(Err, CnxError),
			const CnxResultIdentifier(RESULT_T, vtable) * m_vtable);

	#define ___DISABLE_IF_NULL(self) \
		cnx_disable_if(!(self), "Can't perform an operator on a null result")

/// @brief Returns whether this `CnxResult(RESULT_T)` is holding a value
/// @param self - The `CnxResult` to check
/// @return `true` if this `CnxResult(RESULT_T)` is holding a value, `false` otherwise
__attr(nodiscard) __attr(not_null(1)) RESULT_STATIC RESULT_INLINE
	bool CnxResultIdentifier(RESULT_T, is_ok)(const CnxResult(RESULT_T) * restrict self)
		___DISABLE_IF_NULL(self);
/// @brief Returns whether this `CnxResult(RESULT_T)` __isn't__ holding a value (aka is holding an
/// error)
/// @param self - The `CnxResult` to check
/// @return `true` if this `CnxResult(RESULT_T)` __isn't__ holding a value,
/// `false` if it is  holding an error
__attr(nodiscard) __attr(not_null(1)) RESULT_STATIC RESULT_INLINE
	bool CnxResultIdentifier(RESULT_T, is_err)(const CnxResult(RESULT_T) * restrict self)
		___DISABLE_IF_NULL(self);
/// @brief Returns a pointer to the const value stored in this `CnxResult`
/// @param self - The `CnxResult` to get the stored value from
/// @return a pointer to the contained const value
/// @note Panics if `self` does not contain a value
__attr(nodiscard) __attr(not_null(1)) __attr(returns_not_null) RESULT_STATIC RESULT_INLINE const
	RESULT_T* CnxResultIdentifier(RESULT_T, as_const)(const CnxResult(RESULT_T) * restrict self)
		___DISABLE_IF_NULL(self);
/// @brief Returns a pointer to the value stored in this `CnxResult`
/// @param self - The `CnxResult` to get the stored value from
/// @return a pointer to the contained value
/// @note Panics if `self` does not contain a value
__attr(nodiscard) __attr(not_null(1)) __attr(returns_not_null) RESULT_STATIC RESULT_INLINE
	RESULT_T* CnxResultIdentifier(RESULT_T, as_mut)(CnxResult(RESULT_T) * restrict self)
		___DISABLE_IF_NULL(self);
/// @brief Returns the value stored in this `CnxResult`
/// @param self - The `CnxResult` to get the stored value from
/// @return the contained value
/// @note Panics if `self` does not contain a value
__attr(nodiscard) __attr(not_null(1)) RESULT_STATIC RESULT_INLINE RESULT_T
	CnxResultIdentifier(RESULT_T, unwrap)(CnxResult(RESULT_T) * restrict self)
		___DISABLE_IF_NULL(self);
/// @brief Returns the value stored in this `CnxResult` , or `default_value`
/// if this is `None`
/// @param self - The `CnxResult` to get the stored value from
/// @param default_value - The value to return if this is `None`
/// @return the contained value, or `default_value`
__attr(nodiscard) __attr(not_null(1)) RESULT_STATIC RESULT_INLINE RESULT_T
	CnxResultIdentifier(RESULT_T, unwrap_or)(CnxResult(RESULT_T) * restrict self,
											 RESULT_T default_value) ___DISABLE_IF_NULL(self);
/// @brief Returns the value stored in this `CnxResult` , or the value returned by
/// `default_generator` if this is `None`
/// @param self - The `CnxResult` to get the stored value from
/// @param default_generator - The function to generate the value to return if this is `None`
/// @return the contained value, or the one generated by `default_generator`
__attr(nodiscard) __attr(not_null(1, 2)) RESULT_STATIC RESULT_INLINE RESULT_T
	CnxResultIdentifier(RESULT_T, unwrap_or_else)(CnxResult(RESULT_T) * restrict self,
												  RESULT_T (*const default_generator)(void))
		___DISABLE_IF_NULL(self)
			cnx_disable_if(!default_generator, "default_generator cannot be null");
/// @brief Returns the value stored in this `CnxResult`
/// @param self - The `CnxResult` to get the stored value from
/// @return the contained value
/// @note Panics if `self` does not contain a value, with the custom panic message `panic_message`
__attr(nodiscard) __attr(not_null(1, 2)) RESULT_STATIC RESULT_INLINE RESULT_T
	CnxResultIdentifier(RESULT_T, expect)(CnxResult(RESULT_T) * restrict self,
										  restrict const_cstring panic_message)
		___DISABLE_IF_NULL(self) cnx_disable_if(!panic_message, "panic_message cannot be null");
/// @brief Returns the error stored in this `CnxResult`
/// @param self - The `CnxResult` to get the stored value from
/// @return the contained value
/// @note Panics if `self` does not contain an error
__attr(nodiscard) __attr(not_null(1)) RESULT_STATIC RESULT_INLINE CnxError
	CnxResultIdentifier(RESULT_T, unwrap_err)(CnxResult(RESULT_T) * restrict self)
		___DISABLE_IF_NULL(self);

/// @brief Returns `self` if it is `Ok`, otherwise returns `result_b`
///
/// @param self - The `CnxResult(RESULT_T)` to "or" with `result_b`
/// @param result_b - Another `CnxResult(RESULT_T)` to "or" with `self`.
///
/// @return `self` if it is `Ok`. Otherwise, `result_b`
/// @ingroup cnx_result
__attr(nodiscard) __attr(not_null(1)) RESULT_STATIC RESULT_INLINE CnxResult(RESULT_T)
	CnxResultIdentifier(RESULT_T, or)(const CnxResult(RESULT_T) * restrict self,
									  CnxResult(RESULT_T) option_b) ___DISABLE_IF_NULL(self);

/// @brief Returns `self` if it is `Ok`, otherwise returns the result of calling `func`
///
/// @param self - The `CnxResult(RESULT_T)` to "or" with `result_b`
/// @param func - The function to call if `self` is `Err`
///
/// @return `self` if it is `Ok`. Otherwise, `func()`
/// @ingroup cnx_result
__attr(nodiscard) __attr(not_null(1, 2)) RESULT_STATIC RESULT_INLINE CnxResult(RESULT_T)
	CnxResultIdentifier(RESULT_T, or_else)(const CnxResult(RESULT_T) * restrict self,
										   CnxResult(RESULT_T) (*const func)(void))
		___DISABLE_IF_NULL(self)
			cnx_disable_if(!func, "The function to call if `self` is `Err` can't be null");

/// @brief Converts this `CnxResult(RESULT_T)` to a `bool`
///
/// This is effectively the same as calling `cnx_result_is_ok(self)`, as it converts to a bool
/// on the condition that this is the `Ok(T, value)` variant.
///
/// @param self - The `CnxResult(RESULT_T)` to convert to a `bool`
///
/// @return this `CnxResult(RESULT_T)` as a `bool`
/// @ingroup cnx_result
__attr(nodiscard) __attr(not_null(1)) RESULT_STATIC RESULT_INLINE
	bool CnxResultIdentifier(RESULT_T, as_bool)(const CnxResult(RESULT_T) * restrict self)
		___DISABLE_IF_NULL(self);

typedef struct CnxResultIdentifier(RESULT_T, vtable) {
	bool (*const is_ok)(const CnxResult(RESULT_T)* restrict self);
	bool (*const is_err)(const CnxResult(RESULT_T)* restrict self);
	const RESULT_T* (*const as_const)(const CnxResult(RESULT_T)* restrict self);
	RESULT_T* (*const as_mut)(CnxResult(RESULT_T)* restrict self);
	RESULT_T (*const unwrap)(CnxResult(RESULT_T)* restrict self);
	RESULT_T (*const unwrap_or)(CnxResult(RESULT_T)* restrict self, RESULT_T default_value);
	RESULT_T(*const unwrap_or_else)
	(CnxResult(RESULT_T)* restrict self, RESULT_T(*default_generator)(void));
	RESULT_T(*const expect)
	(CnxResult(RESULT_T)* restrict self, restrict const_cstring panic_message);
	CnxError (*const unwrap_err)(CnxResult(RESULT_T)* restrict self);
	CnxResult(RESULT_T) (*const _or)(const CnxResult(RESULT_T)* restrict self,
									 CnxResult(RESULT_T) option_b);
	CnxResult(RESULT_T) (*const or_else)(const CnxResult(RESULT_T)* restrict self,
										 CnxResult(RESULT_T) (*const func)(void));
	bool (*const as_bool)(const CnxResult(RESULT_T)* restrict self);
}
CnxResultIdentifier(RESULT_T, vtable);

__attr(maybe_unused) static const CnxResultIdentifier(RESULT_T, vtable)
	CnxResultIdentifier(RESULT_T, vtable_impl)
	= {.is_ok = CnxResultIdentifier(RESULT_T, is_ok),
	   .is_err = CnxResultIdentifier(RESULT_T, is_err),
	   .as_const = CnxResultIdentifier(RESULT_T, as_const),
	   .as_mut = CnxResultIdentifier(RESULT_T, as_mut),
	   .unwrap = CnxResultIdentifier(RESULT_T, unwrap),
	   .unwrap_or = CnxResultIdentifier(RESULT_T, unwrap_or),
	   .unwrap_or_else = CnxResultIdentifier(RESULT_T, unwrap_or_else),
	   .expect = CnxResultIdentifier(RESULT_T, expect),
	   .unwrap_err = CnxResultIdentifier(RESULT_T, unwrap_err),
	   ._or = CnxResultIdentifier(RESULT_T, or),
	   .or_else = CnxResultIdentifier(RESULT_T, or_else),
	   .as_bool = CnxResultIdentifier(RESULT_T, as_bool)};

	#undef ___DISABLE_IF_NULL

	#if SHOULD_UNDEF_SUPPRESS_INSTANTIATIONS
		#undef CNX_TEMPLATE_SUPPRESS_INSTANTIATIONS
	#endif // SHOULD_UNDEF_SUPPRESS_INSTANTIATIONS

#endif // defined(RESULT_T) && RESULT_DECL
