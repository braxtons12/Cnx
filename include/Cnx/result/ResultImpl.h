/// @file ResultImpl.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief This module provides the function definitions for a template instantiation of
/// `CnxResult(RESULT_T)`
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

#if defined(RESULT_T) && RESULT_IMPL

	#if CNX_TEMPLATE_SUPPRESS_INSTANTIATIONS
		#undef CNX_TEMPLATE_SUPPRESS_INSTANTIATIONS

		#undef CNX_TEMPLATE_IMPL
		#include <Cnx/Error.h>
		#define CNX_TEMPLATE_IMPL TRUE

		#define CNX_TEMPLATE_SUPPRESS_INSTANTIATIONS TRUE
	#else
		#undef CNX_TEMPLATE_IMPL
		#include <Cnx/Error.h>
		#define CNX_TEMPLATE_IMPL TRUE
	#endif // CNX_TEMPLATE_SUPPRESS_INSTANTIATIONS

	#if !CNX_TEMPLATE_SUPPRESS_INSTANTIATIONS
		#define SHOULD_UNDEF_SUPPRESS_INSTANTIATIONS TRUE
		#define CNX_TEMPLATE_SUPPRESS_INSTANTIATIONS TRUE
	#endif // CNX_TEMPLATE_SUPPRESS_INSTANTIATIONS

	#include <Cnx/Allocators.h>
	#include <Cnx/Assert.h>
	#include <Cnx/BasicTypes.h>
	#include <Cnx/Enum.h>
	#include <Cnx/monadic/IfLet.h>

RESULT_STATIC RESULT_INLINE bool
CnxResultIdentifier(RESULT_T, is_ok)(const CnxResult(RESULT_T) * restrict self) {
	return is_variant(*self, Ok);
}

RESULT_STATIC RESULT_INLINE bool
CnxResultIdentifier(RESULT_T, is_err)(const CnxResult(RESULT_T) * restrict self) {
	return is_variant(*self, Err);
}

RESULT_STATIC RESULT_INLINE const RESULT_T*
CnxResultIdentifier(RESULT_T, as_const)(const CnxResult(RESULT_T) * restrict self) {
	match_let(*self, Err) {
		cnx_panic("as_const called on an Err, terminating");
	}

	return &(self->variant_identifier(Ok)._1);
}

RESULT_STATIC RESULT_INLINE RESULT_T*
CnxResultIdentifier(RESULT_T, as_mut)(CnxResult(RESULT_T) * restrict self) {
	match_let(*self, Err) {
		cnx_panic("as_mut called on an Err, terminating");
	}

	return &(self->variant_identifier(Ok)._1);
}

RESULT_STATIC RESULT_INLINE RESULT_T
CnxResultIdentifier(RESULT_T, unwrap)(CnxResult(RESULT_T) * restrict self) {
	match_let(*self, Err) {
		cnx_panic("unwrap called on an Err, terminating");
	}

	return self->variant_identifier(Ok)._1;
}

RESULT_STATIC RESULT_INLINE RESULT_T
CnxResultIdentifier(RESULT_T, unwrap_or)(CnxResult(RESULT_T) * restrict self,
										 RESULT_T default_value) {
	match_let(*self, Ok, ok) {
		return ok;
	}

	return default_value;
}

RESULT_STATIC RESULT_INLINE RESULT_T
CnxResultIdentifier(RESULT_T, unwrap_or_else)(CnxResult(RESULT_T) * restrict self,
											  RESULT_T (*const default_generator)(void)) {
	match_let(*self, Ok, ok) {
		return ok;
	}

	return default_generator();
}

RESULT_STATIC RESULT_INLINE RESULT_T
CnxResultIdentifier(RESULT_T, expect)(CnxResult(RESULT_T) * restrict self,
									  restrict const_cstring panic_message) {
	match_let(*self, Err) {
		cnx_panic(panic_message);
	}

	return self->variant_identifier(Ok)._1;
}

RESULT_STATIC RESULT_INLINE CnxError
CnxResultIdentifier(RESULT_T, unwrap_err)(CnxResult(RESULT_T) * restrict self) {
	match_let(*self, Ok) {
		cnx_panic("unwrap_err called on an Ok, terminating");
	}

	return self->variant_identifier(Err)._1;
}

RESULT_STATIC RESULT_INLINE CnxResult(RESULT_T)
	CnxResultIdentifier(RESULT_T, or)(const CnxResult(RESULT_T) * restrict self,
									  CnxResult(RESULT_T) result_b) {
	match_let(*self, Ok) {
		return *self;
	}
	else {
		return result_b;
	}
}

RESULT_STATIC RESULT_INLINE CnxResult(RESULT_T)
	CnxResultIdentifier(RESULT_T, or_else)(const CnxResult(RESULT_T) * restrict self,
										   CnxResult(RESULT_T) (*const func)(void)) {
	match_let(*self, Ok) {
		return *self;
	}
	else {
		return func();
	}
}

RESULT_STATIC RESULT_INLINE bool
CnxResultIdentifier(RESULT_T, as_bool)(const CnxResult(RESULT_T) * restrict self) {
	return cnx_result_is_ok(*self);
}

	#if SHOULD_UNDEF_SUPPRESS_INSTANTIATIONS
		#undef CNX_TEMPLATE_SUPPRESS_INSTANTIATIONS
	#endif // SHOULD_UNDEF_SUPPRESS_INSTANTIATIONS

#endif // defined(RESULT_T) && RESULT_IMPL
