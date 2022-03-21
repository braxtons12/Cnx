/// @file StdOptionImpl.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief This module provides the function definitions for a template instantiation of
/// `StdOption(T)`
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

#include <C2nxt/std_option/StdOptionDef.h>

#if defined(OPTION_T) && OPTION_IMPL

	#define STD_TEMPLATE_SUPPRESS_INSTANTIATIONS TRUE

	#include <C2nxt/StdAllocators.h>
	#include <C2nxt/StdAssert.h>
	#include <C2nxt/StdBasicTypes.h>
	#include <C2nxt/monadic/StdIfLet.h>

bool StdOptionIdentifier(OPTION_T, is_some)(const StdOption(OPTION_T) * restrict self) {
	return is_variant(*self, Some);
}

bool StdOptionIdentifier(OPTION_T, is_none)(const StdOption(OPTION_T) * restrict self) {
	return is_variant(*self, None);
}

const OPTION_T* StdOptionIdentifier(OPTION_T, as_const)(const StdOption(OPTION_T) * restrict self) {
	match_let(*self, None) {
		std_panic("as_const called on a None value, terminating");
	}

	return &(self->variant_identifier(Some)._1);
}

OPTION_T* StdOptionIdentifier(OPTION_T, as_mut)(StdOption(OPTION_T) * restrict self) {
	match_let(*self, None) {
		std_panic("as_mut called on a None value, terminating");
	}

	return &(self->variant_identifier(Some)._1);
}

OPTION_T StdOptionIdentifier(OPTION_T, unwrap)(StdOption(OPTION_T) * restrict self) {
	match_let(*self, None) {
		std_panic("unwrap called on a None value, terminating");
	}

	return extract_variant(*self, Some)._1;
}

OPTION_T StdOptionIdentifier(OPTION_T, unwrap_or)(StdOption(OPTION_T) * restrict self,
												  OPTION_T default_value) {
	match_let(*self, Some, some) {
		return some;
	}

	return default_value;
}

OPTION_T StdOptionIdentifier(OPTION_T, unwrap_or_else)(StdOption(OPTION_T) * restrict self,
													   OPTION_T (*const default_generator)(void)) {
	match_let(*self, Some, some) {
		return some;
	}

	return default_generator();
}

OPTION_T StdOptionIdentifier(OPTION_T, expect)(StdOption(OPTION_T) * restrict self,
											   restrict const_cstring panic_message) {
	match_let(*self, None) {
		std_panic(panic_message);
	}

	return extract_variant(*self, Some)._1;
}

StdOption(OPTION_T) StdOptionIdentifier(OPTION_T, or)(const StdOption(OPTION_T) * restrict self,
													  StdOption(OPTION_T) option_b) {
	if(std_option_is_some(*self)) {
		return *self;
	}
	else {
		return option_b;
	}
}

StdOption(OPTION_T)
	StdOptionIdentifier(OPTION_T, or_else)(const StdOption(OPTION_T) * restrict self,
										   StdOption(OPTION_T) (*const func)(void)) {
	if(std_option_is_some(*self)) {
		return *self;
	}
	else {
		return func();
	}
}

bool StdOptionIdentifier(OPTION_T, as_bool)(const StdOption(OPTION_T) * restrict self) {
	return std_option_is_some(*self);
}

	#undef STD_TEMPLATE_SUPPRESS_INSTANTIATIONS
#endif // defined(OPTION_T) && OPTION_IMPL
