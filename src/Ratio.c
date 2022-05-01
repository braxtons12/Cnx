/// @file Ratio.c
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief This module provides lossless methods for dealing with exact fractions
/// @version 0.1.1
/// @date 2022-04-30
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

#include <Cnx/Ratio.h>

CnxRatio cnx_ratio_new(i64 numerator, i64 denominator) {
	cnx_assert(denominator != 0, "Denominator of a CnxRatio cannot be 0");
	let abs_num = abs(numerator);
	let abs_den = abs(denominator);
	let gcd = cnx_gcd(abs_num, abs_den);
	return (CnxRatio){.num = cnx_sign(numerator) * cnx_sign(denominator) * abs_num / gcd,
					  .den = abs_den / gcd};
}

CnxRatio cnx_ratio_add(CnxRatio lhs, CnxRatio rhs) {
	let gcd_r1num_r2num = cnx_gcd(lhs.num, rhs.num);
	let gcd_r1den_r2den = cnx_gcd(lhs.den, rhs.den);

	let left = cnx_ratio_new(gcd_r1num_r2num, lhs.den / gcd_r1den_r2den);
	let_mut right_num1 = cnx_checked_mul(lhs.num / gcd_r1num_r2num, rhs.den / gcd_r1den_r2den);
	let_mut right_num2 = cnx_checked_mul(rhs.num / gcd_r1num_r2num, lhs.den / gcd_r1den_r2den);
	let_mut right_num = cnx_checked_add(cnx_option_expect(right_num1, "Overflow in cnx_ratio_add"),
										cnx_option_expect(right_num2, "Overflow in cnx_ratio_add"));
	let right
		= cnx_ratio_new(cnx_option_expect(right_num, "Overflow in cnx_multiply_add"), rhs.den);
	return cnx_ratio_multiply(left, right);
}

CnxRatio cnx_ratio_subtract(CnxRatio lhs, CnxRatio rhs) {
	let gcd_r1num_r2num = cnx_gcd(lhs.num, rhs.num);
	let gcd_r1den_r2den = cnx_gcd(lhs.den, rhs.den);

	let left = cnx_ratio_new(gcd_r1num_r2num, lhs.den / gcd_r1den_r2den);
	let_mut right_num1 = cnx_checked_mul(lhs.num / gcd_r1num_r2num, rhs.den / gcd_r1den_r2den);
	let_mut right_num2 = cnx_checked_mul(rhs.num / gcd_r1num_r2num, lhs.den / gcd_r1den_r2den);
	let_mut right_num = cnx_checked_sub(cnx_option_expect(right_num1, "Overflow in cnx_ratio_add"),
										cnx_option_expect(right_num2, "Overflow in cnx_ratio_add"));
	let right
		= cnx_ratio_new(cnx_option_expect(right_num, "Overflow in cnx_multiply_add"), rhs.den);
	return cnx_ratio_multiply(left, right);
}

CnxRatio cnx_ratio_multiply(CnxRatio lhs, CnxRatio rhs) {
	let gcd_r1num_r2den = cnx_gcd(lhs.num, rhs.den);
	let gcd_r1den_r2num = cnx_gcd(lhs.den, rhs.num);

	let_mut num = cnx_checked_mul(lhs.num / gcd_r1num_r2den, rhs.num / gcd_r1den_r2num);
	let_mut den = cnx_checked_mul(rhs.den / gcd_r1num_r2den, lhs.den / gcd_r1den_r2num);

	return cnx_ratio_new(cnx_option_expect(num, "Overflow in cnx_ratio_multiply"),
						 cnx_option_expect(den, "Overflow in cnx_ratio_multiply"));
}

CnxRatio cnx_ratio_divide(CnxRatio lhs, CnxRatio rhs) {
	let reciprocal = (CnxRatio){.num = rhs.den, .den = rhs.num};
	return cnx_ratio_multiply(lhs, reciprocal);
}

bool cnx_ratio_equal(CnxRatio lhs, CnxRatio rhs) {
	return cnx_ratio_compare(lhs, rhs) == CNX_EQUAL;
}

bool cnx_ratio_not_equal(CnxRatio lhs, CnxRatio rhs) {
	return cnx_ratio_compare(lhs, rhs) != CNX_EQUAL;
}

bool cnx_ratio_less_than(CnxRatio lhs, CnxRatio rhs) {
	return cnx_ratio_compare(lhs, rhs) == CNX_LESS_THAN;
}

bool cnx_ratio_less_than_or_equal(CnxRatio lhs, CnxRatio rhs) {
	let compare = cnx_ratio_compare(lhs, rhs);
	return compare == CNX_LESS_THAN || compare == CNX_EQUAL;
}

bool cnx_ratio_greater_than(CnxRatio lhs, CnxRatio rhs) {
	return cnx_ratio_compare(lhs, rhs) == CNX_GREATER_THAN;
}

bool cnx_ratio_greater_than_or_equal(CnxRatio lhs, CnxRatio rhs) {
	let compare = cnx_ratio_compare(lhs, rhs);
	return compare == CNX_GREATER_THAN || compare == CNX_EQUAL;
}

CnxCompare cnx_ratio_compare(CnxRatio lhs, CnxRatio rhs) {
	let den = cnx_lcm(lhs.den, rhs.den);
	let left = lhs.num * (den / lhs.den);
	let right = rhs.num * (den / rhs.den);

	if(lhs.den == rhs.den && lhs.num == rhs.num) {
		return CNX_EQUAL;
	}
	else if(left < right) {
		return CNX_LESS_THAN;
	}
	else {
		return CNX_GREATER_THAN;
	}
}

CnxOption(i64) cnx_ratio_as_scalar_exact(CnxRatio ratio) {
	if(ratio.num % ratio.den == 0) {
		return Some(i64, ratio.num / ratio.den);
	}
	else {
		return None(i64);
	}
}

f64 cnx_ratio_as_scalar(CnxRatio ratio) {
	return static_cast(f64)(ratio.num) / static_cast(f64)(ratio.den);
}

CnxRatio cnx_ratio_add_scalar(CnxRatio ratio, i64 scalar) {
	return cnx_ratio_add(ratio, cnx_ratio_new(scalar * ratio.den, ratio.den));
}

CnxRatio cnx_ratio_subtract_scalar(CnxRatio ratio, i64 scalar) {
	return cnx_ratio_subtract(ratio, cnx_ratio_new(scalar * ratio.den, ratio.den));
}

CnxRatio cnx_ratio_multiply_scalar(CnxRatio ratio, i64 scalar) {
	return cnx_ratio_new(ratio.num * scalar, ratio.den);
}

CnxRatio cnx_ratio_divide_scalar(CnxRatio ratio, i64 scalar) {
	return cnx_ratio_new(ratio.num, ratio.den * scalar);
}

CnxString cnx_ratio_format(const CnxFormat* restrict self, CnxFormatSpecifier specifier) {
	return cnx_ratio_format_with_allocator(self, specifier, cnx_allocator_new());
}

CnxString cnx_ratio_format_with_allocator(const CnxFormat* restrict self,
										  __attr(maybe_unused) CnxFormatSpecifier specifier,
										  CnxAllocator allocator) {
	cnx_assert(specifier.m_type == CNX_FORMAT_TYPE_DEFAULT
				   || specifier.m_type == CNX_FORMAT_TYPE_DEBUG,
			   "Can only format a CnxRatio with default or debug format specifier");

	let _self = static_cast(const CnxRatio*)(self->m_self);
	if(specifier.m_type == CNX_FORMAT_TYPE_DEBUG) {
		return cnx_format_with_allocator(AS_STRING(CnxRatio) ": [num = {D}, den = {D}]",
										 allocator,
										 _self->num,
										 _self->den);
	}
	else {
		return cnx_format_with_allocator("{} / {}", allocator, _self->num, _self->den);
	}
}
