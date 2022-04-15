/// @file OptionImpl.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief This module provides the function definitions for a template instantiation of
/// `CnxOption(T)`
/// @version 0.2.2
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

#include <Cnx/option/OptionDef.h>

#if defined(OPTION_T) && OPTION_IMPL

	#define CNX_TEMPLATE_SUPPRESS_INSTANTIATIONS TRUE

	#include <Cnx/Allocators.h>
	#include <Cnx/Assert.h>
	#include <Cnx/BasicTypes.h>
	#include <Cnx/monadic/IfLet.h>

bool CnxOptionIdentifier(OPTION_T, is_some)(const CnxOption(OPTION_T) * restrict self) {
	return is_variant(*self, Some);
}

bool CnxOptionIdentifier(OPTION_T, is_none)(const CnxOption(OPTION_T) * restrict self) {
	return is_variant(*self, None);
}

const OPTION_T* CnxOptionIdentifier(OPTION_T, as_const)(const CnxOption(OPTION_T) * restrict self) {
	match_let(*self, None) {
		cnx_panic("as_const called on a None value, terminating");
	}

	return &(self->variant_identifier(Some)._1);
}

OPTION_T* CnxOptionIdentifier(OPTION_T, as_mut)(CnxOption(OPTION_T) * restrict self) {
	match_let(*self, None) {
		cnx_panic("as_mut called on a None value, terminating");
	}

	return &(self->variant_identifier(Some)._1);
}

OPTION_T CnxOptionIdentifier(OPTION_T, unwrap)(CnxOption(OPTION_T) * restrict self) {
	match_let(*self, None) {
		cnx_panic("unwrap called on a None value, terminating");
	}

	return extract_variant(*self, Some)._1;
}

OPTION_T CnxOptionIdentifier(OPTION_T, unwrap_or)(CnxOption(OPTION_T) * restrict self,
												  OPTION_T default_value) {
	match_let(*self, Some, some) {
		return some;
	}

	return default_value;
}

OPTION_T CnxOptionIdentifier(OPTION_T, unwrap_or_else)(CnxOption(OPTION_T) * restrict self,
													   OPTION_T (*const default_generator)(void)) {
	match_let(*self, Some, some) {
		return some;
	}

	return default_generator();
}

OPTION_T CnxOptionIdentifier(OPTION_T, expect)(CnxOption(OPTION_T) * restrict self,
											   restrict const_cstring panic_message) {
	match_let(*self, None) {
		cnx_panic(panic_message);
	}

	return extract_variant(*self, Some)._1;
}

CnxOption(OPTION_T) CnxOptionIdentifier(OPTION_T, or)(const CnxOption(OPTION_T) * restrict self,
													  CnxOption(OPTION_T) option_b) {
	if(cnx_option_is_some(*self)) {
		return *self;
	}
	else {
		return option_b;
	}
}

CnxOption(OPTION_T)
	CnxOptionIdentifier(OPTION_T, or_else)(const CnxOption(OPTION_T) * restrict self,
										   CnxOption(OPTION_T) (*const func)(void)) {
	if(cnx_option_is_some(*self)) {
		return *self;
	}
	else {
		return func();
	}
}

bool CnxOptionIdentifier(OPTION_T, as_bool)(const CnxOption(OPTION_T) * restrict self) {
	return cnx_option_is_some(*self);
}

	#undef CNX_TEMPLATE_SUPPRESS_INSTANTIATIONS
#endif // defined(OPTION_T) && OPTION_IMPL
