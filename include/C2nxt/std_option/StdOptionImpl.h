/// @file StdOptionImpl.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief This module provides the function definitions for a template instantiation of
/// `StdOption(T)`
/// @version 0.2.1
/// @date 2022-03-05
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

#if defined(T) && STD_TEMPLATE_IMPL

	#define STD_TEMPLATE_SUPPRESS_INSTANTIATIONS TRUE

	#include <C2nxt/StdAllocators.h>
	#include <C2nxt/StdAssert.h>
	#include <C2nxt/StdBasicTypes.h>
	#include <C2nxt/monadic/StdIfLet.h>
	#include <C2nxt/std_option/StdOptionDef.h>

bool StdOptionIdentifier(T, is_some)(const StdOption(T) * restrict self) {
	return is_variant(*self, Some);
}

bool StdOptionIdentifier(T, is_none)(const StdOption(T) * restrict self) {
	return is_variant(*self, None);
}

const T* StdOptionIdentifier(T, as_const)(const StdOption(T) * restrict self) {
	match_let(*self, None) {
		std_panic("as_const called on a None value, terminating");
	}

	return &(self->variant_identifier(Some)._1);
}

T* StdOptionIdentifier(T, as_mut)(StdOption(T) * restrict self) {
	match_let(*self, None) {
		std_panic("as_mut called on a None value, terminating");
	}

	return &(self->variant_identifier(Some)._1);
}

T StdOptionIdentifier(T, unwrap)(StdOption(T) * restrict self) {
	match_let(*self, None) {
		std_panic("unwrap called on a None value, terminating");
	}

	return extract_variant(*self, Some)._1;
}

T StdOptionIdentifier(T, unwrap_or)(StdOption(T) * restrict self, T default_value) {
	match_let(*self, Some, some) {
		return some;
	}

	return default_value;
}

T StdOptionIdentifier(T, unwrap_or_else)(StdOption(T) * restrict self,
										 T (*default_generator)(void)) {
	match_let(*self, Some, some) {
		return some;
	}

	return default_generator();
}

T StdOptionIdentifier(T, expect)(StdOption(T) * restrict self,
								 restrict const_cstring panic_message) {
	match_let(*self, None) {
		std_panic(panic_message);
	}

	return extract_variant(*self, Some)._1;
}

StdOption(T) StdOptionIdentifier(T, or)(const StdOption(T) * restrict self, StdOption(T) option_b) {
	if(std_option_is_some(*self)) {
		return *self;
	}
	else {
		return option_b;
	}
}

StdOption(T) StdOptionIdentifier(T, or_else)(const StdOption(T) * restrict self,
											 StdOption(T) (*const func)(void)) {
	if(std_option_is_some(*self)) {
		return *self;
	}
	else {
		return func();
	}
}

bool StdOptionIdentifier(T, as_bool)(const StdOption(T) * restrict self) {
	return std_option_is_some(*self);
}

	#undef STD_TEMPLATE_SUPPRESS_INSTANTIATIONS
#endif // defined(T) && STD_TEMPLATE_IMPL
