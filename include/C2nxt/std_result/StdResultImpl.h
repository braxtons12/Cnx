/// @file StdResultImpl.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief This module provides the function definitions for a template instantiation of
/// `StdResult(T)`
/// @version 0.2.1
/// @date 2022-03-12
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

	#define STD_TEMPLATE_SUPPRESS_INSTANTIATIONS 1

	#include <C2nxt/StdAllocators.h>
	#include <C2nxt/StdAssert.h>
	#include <C2nxt/StdBasicTypes.h>
	#include <C2nxt/StdEnum.h>
	#include <C2nxt/monadic/StdIfLet.h>
	#include <C2nxt/std_result/StdResultDef.h>
	#include <C2nxt/StdError.h>

bool StdResultIdentifier(T, is_ok)(const StdResult(T) * restrict self) {
	return is_variant(*self, Ok);
}

bool StdResultIdentifier(T, is_err)(const StdResult(T) * restrict self) {
	return is_variant(*self, Err);
}

const T* StdResultIdentifier(T, as_const)(const StdResult(T) * restrict self) {
	match_let(*self, Err) {
		std_panic("as_const called on an Err, terminating");
	}

	return &(self->variant_identifier(Ok)._1);
}

T* StdResultIdentifier(T, as_mut)(StdResult(T) * restrict self) {
	match_let(*self, Err) {
		std_panic("as_mut called on an Err, terminating");
	}

	return &(self->variant_identifier(Ok)._1);
}

T StdResultIdentifier(T, unwrap)(StdResult(T) * restrict self) {
	match_let(*self, Err) {
		std_panic("unwrap called on an Err, terminating");
	}

	return self->variant_identifier(Ok)._1;
}

T StdResultIdentifier(T, unwrap_or)(StdResult(T) * restrict self, T default_value) {
	match_let(*self, Ok, ok) {
		return ok;
	}

	return default_value;
}

T StdResultIdentifier(T, unwrap_or_else)(StdResult(T) * restrict self,
										 T (*const default_generator)(void)) {
	match_let(*self, Ok, ok) {
		return ok;
	}

	return default_generator();
}

T StdResultIdentifier(T, expect)(StdResult(T) * restrict self,
								 restrict const_cstring panic_message) {
	match_let(*self, Err) {
		std_panic(panic_message);
	}

	return self->variant_identifier(Ok)._1;
}

StdError StdResultIdentifier(T, unwrap_err)(StdResult(T) * restrict self) {
	match_let(*self, Ok) {
		std_panic("unwrap_err called on an Ok, terminating");
	}

	return self->variant_identifier(Err)._1;
}

StdResult(T) StdResultIdentifier(T, or)(const StdResult(T) * restrict self, StdResult(T) result_b) {
	match_let(*self, Ok) {
		return *self;
	}
	else {
		return result_b;
	}
}

StdResult(T) StdResultIdentifier(T, or_else)(const StdResult(T) * restrict self,
											 StdResult(T) (*const func)(void)) {
	match_let(*self, Ok) {
		return *self;
	}
	else {
		return func();
	}
}

bool StdResultIdentifier(T, as_bool)(const StdResult(T) * restrict self) {
	return std_result_is_ok(*self);
}

	#undef STD_TEMPLATE_SUPPRESS_INSTANTIATIONS
#endif // defined(T) && STD_TEMPLATE_IMPL
