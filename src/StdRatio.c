/// @file StdRatio.c
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief This module provides lossless methods for dealing with exact fractions
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

#include <C2nxt/StdRatio.h>

StdRatio std_ratio_new(i64 numerator, i64 denominator) {
	std_assert(denominator != 0, "Denominator of a StdRatio cannot be 0");
	let abs_num = abs(numerator);
	let abs_den = abs(denominator);
	let gcd = std_gcd(abs_num, abs_den);
	return (StdRatio){.num = std_sign(numerator) * std_sign(denominator) * abs_num / gcd,
					  .den = abs_den / gcd};
}

StdRatio std_ratio_add(StdRatio lhs, StdRatio rhs) {
	let gcd_r1num_r2num = std_gcd(lhs.num, rhs.num);
	let gcd_r1den_r2den = std_gcd(lhs.den, rhs.den);

	let left = std_ratio_new(gcd_r1num_r2num, lhs.den / gcd_r1den_r2den);
	let_mut right_num1 = std_checked_mul(lhs.num / gcd_r1num_r2num, rhs.den / gcd_r1den_r2den);
	let_mut right_num2 = std_checked_mul(rhs.num / gcd_r1num_r2num, lhs.den / gcd_r1den_r2den);
	let_mut right_num = std_checked_add(std_option_expect(right_num1, "Overflow in std_ratio_add"),
										std_option_expect(right_num2, "Overflow in std_ratio_add"));
	let right
		= std_ratio_new(std_option_expect(right_num, "Overflow in std_multiply_add"), rhs.den);
	return std_ratio_multiply(left, right);
}

StdRatio std_ratio_subtract(StdRatio lhs, StdRatio rhs) {
	let gcd_r1num_r2num = std_gcd(lhs.num, rhs.num);
	let gcd_r1den_r2den = std_gcd(lhs.den, rhs.den);

	let left = std_ratio_new(gcd_r1num_r2num, lhs.den / gcd_r1den_r2den);
	let_mut right_num1 = std_checked_mul(lhs.num / gcd_r1num_r2num, rhs.den / gcd_r1den_r2den);
	let_mut right_num2 = std_checked_mul(rhs.num / gcd_r1num_r2num, lhs.den / gcd_r1den_r2den);
	let_mut right_num = std_checked_sub(std_option_expect(right_num1, "Overflow in std_ratio_add"),
										std_option_expect(right_num2, "Overflow in std_ratio_add"));
	let right
		= std_ratio_new(std_option_expect(right_num, "Overflow in std_multiply_add"), rhs.den);
	return std_ratio_multiply(left, right);
}

StdRatio std_ratio_multiply(StdRatio lhs, StdRatio rhs) {
	let gcd_r1num_r2den = std_gcd(lhs.num, rhs.den);
	let gcd_r1den_r2num = std_gcd(lhs.den, rhs.num);

	let_mut num = std_checked_mul(lhs.num / gcd_r1num_r2den, rhs.num / gcd_r1den_r2num);
	let_mut den = std_checked_mul(rhs.den / gcd_r1num_r2den, lhs.den / gcd_r1den_r2num);

	return std_ratio_new(std_option_expect(num, "Overflow in std_ratio_multiply"),
						 std_option_expect(den, "Overflow in std_ratio_multiply"));
}

StdRatio std_ratio_divide(StdRatio lhs, StdRatio rhs) {
	let reciprocal = (StdRatio){.num = rhs.den, .den = rhs.num};
	return std_ratio_multiply(lhs, reciprocal);
}

bool std_ratio_equal(StdRatio lhs, StdRatio rhs) {
	return std_ratio_compare(lhs, rhs) == STD_EQUAL;
}

bool std_ratio_not_equal(StdRatio lhs, StdRatio rhs) {
	return std_ratio_compare(lhs, rhs) != STD_EQUAL;
}

bool std_ratio_less_than(StdRatio lhs, StdRatio rhs) {
	return std_ratio_compare(lhs, rhs) == STD_LESS_THAN;
}

bool std_ratio_less_than_or_equal(StdRatio lhs, StdRatio rhs) {
	let compare = std_ratio_compare(lhs, rhs);
	return compare == STD_LESS_THAN || compare == STD_EQUAL;
}

bool std_ratio_greater_than(StdRatio lhs, StdRatio rhs) {
	return std_ratio_compare(lhs, rhs) == STD_GREATER_THAN;
}

bool std_ratio_greater_than_or_equal(StdRatio lhs, StdRatio rhs) {
	let compare = std_ratio_compare(lhs, rhs);
	return compare == STD_GREATER_THAN || compare == STD_EQUAL;
}

StdCompare std_ratio_compare(StdRatio lhs, StdRatio rhs) {
	let den = std_lcm(lhs.den, rhs.den);
	let left = lhs.num * (den / lhs.den);
	let right = rhs.num * (den / rhs.den);

	if(lhs.den == rhs.den && lhs.num == rhs.num) {
		return STD_EQUAL;
	}
	else if(left < right) {
		return STD_LESS_THAN;
	}
	else {
		return STD_GREATER_THAN;
	}
}

StdOption(i64) std_ratio_as_scalar_exact(StdRatio ratio) {
	if(ratio.num % ratio.den == 0) {
		return Some(i64, ratio.num / ratio.den);
	}
	else {
		return None(i64);
	}
}

f64 std_ratio_as_scalar(StdRatio ratio) {
	return static_cast(f64)(ratio.num) / static_cast(f64)(ratio.den);
}

StdRatio std_ratio_add_scalar(StdRatio ratio, i64 scalar) {
	return std_ratio_add(ratio, std_ratio_new(scalar * ratio.den, ratio.den));
}

StdRatio std_ratio_subtract_scalar(StdRatio ratio, i64 scalar) {
	return std_ratio_subtract(ratio, std_ratio_new(scalar * ratio.den, ratio.den));
}

StdRatio std_ratio_multiply_scalar(StdRatio ratio, i64 scalar) {
	return std_ratio_new(ratio.num * scalar, ratio.den);
}

StdRatio std_ratio_divide_scalar(StdRatio ratio, i64 scalar) {
	return std_ratio_new(ratio.num, ratio.den * scalar);
}

StdString std_ratio_format(const StdFormat* restrict self, StdFormatSpecifier specifier) {
	return std_ratio_format_with_allocator(self, specifier, std_allocator_new());
}

StdString std_ratio_format_with_allocator(const StdFormat* restrict self,
										  [[maybe_unused]] StdFormatSpecifier specifier,
										  StdAllocator allocator) {
	std_assert(specifier.m_type == STD_FORMAT_TYPE_DEFAULT
				   || specifier.m_type == STD_FORMAT_TYPE_DEBUG,
			   "Can only format a StdRatio with default or debug format specifier");

	let _self = static_cast(const StdRatio*)(self->m_self);
	if(specifier.m_type == STD_FORMAT_TYPE_DEBUG) {
		return std_format_with_allocator(AS_STRING(StdRatio) ": [num = {D}, den = {D}]",
										 allocator,
										 _self->num,
										 _self->den);
	}
	else {
		return std_format_with_allocator("{} / {}", allocator, _self->num, _self->den);
	}
}
