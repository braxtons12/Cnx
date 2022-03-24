/// @file StdMath.c
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief This module extends `<tgmath.h>` and provides some alternative math functions.
/// @version 0.1.1
/// @date 2022-02-24
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

#include <C2nxt/StdMath.h>
#include <math.h>

// NOLINTNEXTLINE(misc-no-recursion)
i64 std_gcd_i64(i64 left, i64 right) {
	return (right == 0 && left == 0) ? 1 : (right == 0 ? left : std_gcd_i64(right, left % right));
}

// NOLINTNEXTLINE(misc-no-recursion)
u64 std_gcd_u64(u64 left, u64 right) {
	return (right == 0 && left == 0) ? 1 : (right == 0 ? left : std_gcd_u64(right, left % right));
}

i64 std_lcm_i64(i64 left, i64 right) {
	return left / std_gcd_i64(left, right) * right;
}

u64 std_lcm_u64(u64 left, u64 right) {
	return left / std_gcd_u64(left, right) * right;
}

StdOption(u8) std_checked_add_u8(u8 left, u8 right) {
	return left <= std_max_value(u8) - right ? Some(u8, left + right) : None(u8);
}

StdOption(u16) std_checked_add_u16(u16 left, u16 right) {
	return left <= std_max_value(u16) - right ? Some(u16, left + right) : None(u16);
}

StdOption(u32) std_checked_add_u32(u32 left, u32 right) {
	return left <= std_max_value(u32) - right ? Some(u32, left + right) : None(u32);
}

StdOption(u64) std_checked_add_u64(u64 left, u64 right) {
	return left <= std_max_value(u64) - right ? Some(u64, left + right) : None(u64);
}

StdOption(i8) std_checked_add_i8(i8 left, i8 right) {
	if(right >= 0) {
		return (left <= std_max_value(i8) - right) ? Some(i8, left + right) : None(i8);
	}
	else {
		return (std_min_value(i8) - right <= left) ? Some(i8, left + right) : None(i8);
	}
}

StdOption(i16) std_checked_add_i16(i16 left, i16 right) {
	if(right >= 0) {
		return (left <= std_max_value(i16) - right) ? Some(i16, left + right) : None(i16);
	}
	else {
		return (std_min_value(i16) - right <= left) ? Some(i16, left + right) : None(i16);
	}
}

StdOption(i32) std_checked_add_i32(i32 left, i32 right) {
	if(right >= 0) {
		return (left <= std_max_value(i32) - right) ? Some(i32, left + right) : None(i32);
	}
	else {
		return (std_min_value(i32) - right <= left) ? Some(i32, left + right) : None(i32);
	}
}

StdOption(i64) std_checked_add_i64(i64 left, i64 right) {
	if(right >= 0) {
		return (left <= std_max_value(i64) - right) ? Some(i64, left + right) : None(i64);
	}
	else {
		return (std_min_value(i64) - right <= left) ? Some(i64, left + right) : None(i64);
	}
}

// NOLINTNEXTLINE(readability-function-cognitive-complexity)
StdOption(f32) std_checked_add_f32(f32 left, f32 right) {
	StdOption(f32) res;
	if(right >= 0) {
		res = (left <= std_max_value(f32) - right) ? Some(f32, static_cast(f32)(left + right)) :
													   None(f32);
	}
	else {
		res = (std_min_value(f32) - right <= left) ? Some(f32, static_cast(f32)(left + right)) :
													   None(f32);
	}

	if_let(ret, res) {
		return (isnan(ret) || isinf(ret)) ? None(f32) : Some(f32, ret);
	}
	else {
		return None(f32);
	}
}

// NOLINTNEXTLINE(readability-function-cognitive-complexity)
StdOption(f64) std_checked_add_f64(f64 left, f64 right) {
	StdOption(f64) res;
	if(right >= 0) {
		res = (left <= std_max_value(f64) - right) ? Some(f64, static_cast(f64)(left + right)) :
													   None(f64);
	}
	else {
		res = (std_min_value(f64) - right <= left) ? Some(f64, static_cast(f64)(left + right)) :
													   None(f64);
	}

	if_let(ret, res) {
		return (isnan(ret) || isinf(ret)) ? None(f64) : Some(f64, ret);
	}
	else {
		return None(f64);
	}
}

StdOption(u8) std_checked_sub_u8(u8 left, u8 right) {
	return (std_min_value(u8) + right <= left) ? Some(u8, left - right) : None(u8);
}

StdOption(u16) std_checked_sub_u16(u16 left, u16 right) {
	return (std_min_value(u16) + right <= left) ? Some(u16, left - right) : None(u16);
}

StdOption(u32) std_checked_sub_u32(u32 left, u32 right) {
	return (std_min_value(u32) + right <= left) ? Some(u32, left - right) : None(u32);
}

StdOption(u64) std_checked_sub_u64(u64 left, u64 right) {
	return (std_min_value(u64) + right <= left) ? Some(u64, left - right) : None(u64);
}

StdOption(i8) std_checked_sub_i8(i8 left, i8 right) {
	return std_checked_add_i8(left, static_cast(i8)(-right));
}

StdOption(i16) std_checked_sub_i16(i16 left, i16 right) {
	return std_checked_add_i16(left, static_cast(i16)(-right));
}

StdOption(i32) std_checked_sub_i32(i32 left, i32 right) {
	return std_checked_add_i32(left, static_cast(i32)(-right));
}

StdOption(i64) std_checked_sub_i64(i64 left, i64 right) {
	return std_checked_add_i64(left, static_cast(i64)(-right));
}

StdOption(f32) std_checked_sub_f32(f32 left, f32 right) {
	return std_checked_add_f32(left, -right);
}

StdOption(f64) std_checked_sub_f64(f64 left, f64 right) {
	return std_checked_add_f64(left, -right);
}

StdOption(u8) std_checked_mul_u8(u8 left, u8 right) {
	if(left == 0 || right == 0) {
		return Some(u8, 0);
	}
	else {
		let res = static_cast(u8)(left * right);
		return (res / left == right) ? Some(u8, res) : None(u8);
	}
}

StdOption(u16) std_checked_mul_u16(u16 left, u16 right) {
	if(left == 0 || right == 0) {
		return Some(u16, 0);
	}
	else {
		let res = static_cast(u16)(left * right);
		return (res / left == right) ? Some(u16, res) : None(u16);
	}
}

StdOption(u32) std_checked_mul_u32(u32 left, u32 right) {
	if(left == 0 || right == 0) {
		return Some(u32, 0);
	}
	else {
		let res = left * right;
		return (res / left == right) ? Some(u32, res) : None(u32);
	}
}

StdOption(u64) std_checked_mul_u64(u64 left, u64 right) {
	if(left == 0 || right == 0) {
		return Some(u64, 0);
	}
	else {
		let res = left * right;
		return (res / left == right) ? Some(u64, res) : None(u64);
	}
}

StdOption(i8) std_checked_mul_i8(i8 left, i8 right) {
	if(std_sign(left) == 0 || std_sign(right) == 0) {
		return Some(i8, 0);
	}
	else {
		if(std_max_value(i8) / left > right && std_min_value(i8) / left < right) {
			let res = static_cast(i8)(left * right);
			return Some(i8, res);
		}
		else {
			return None(i8);
		}
	}
}

StdOption(i16) std_checked_mul_i16(i16 left, i16 right) {
	if(std_sign(left) == 0 || std_sign(right) == 0) {
		return Some(i16, 0);
	}
	else {
		if(std_max_value(i16) / left > right && std_min_value(i16) / left < right) {
			let res = static_cast(i16)(left * right);
			return Some(i16, res);
		}
		else {
			return None(i16);
		}
	}
}

StdOption(i32) std_checked_mul_i32(i32 left, i32 right) {
	if(std_sign(left) == 0 || std_sign(right) == 0) {
		return Some(i32, 0);
	}
	else {
		if(std_max_value(i32) / left > right && std_min_value(i32) / left < right) {
			let res = left * right;
			return Some(i32, res);
		}
		else {
			return None(i32);
		}
	}
}

StdOption(i64) std_checked_mul_i64(i64 left, i64 right) {
	if(std_sign(left) == 0 || std_sign(right) == 0) {
		return Some(i64, 0);
	}
	else {
		if(std_max_value(i64) / left > right && std_min_value(i64) / left < right) {
			let res = left * right;
			return Some(i64, res);
		}
		else {
			return None(i64);
		}
	}
}

// NOLINTNEXTLINE(readability-function-cognitive-complexity)
StdOption(f32) std_checked_mul_f32(f32 left, f32 right) {
	StdOption(f32) res;
	if(std_max_value(f32) / left > right && std_min_value(f32) / left < right) {
		let val = left * right;
		res = Some(f32, val);
	}
	else {
		res = None(f32);
	}

	if_let(ret, res) {
		return (isnan(ret) || isinf(ret)) ? None(f32) : Some(f32, ret);
	}
	else {
		return None(f32);
	}
}

// NOLINTNEXTLINE(readability-function-cognitive-complexity)
StdOption(f64) std_checked_mul_f64(f64 left, f64 right) {
	StdOption(f64) res;
	if(std_max_value(f64) / left > right && std_min_value(f64) / left < right) {
		let val = left * right;
		res = Some(f64, val);
	}
	else {
		res = None(f64);
	}

	if_let(ret, res) {
		return (isnan(ret) || isinf(ret)) ? None(f64) : Some(f64, ret);
	}
	else {
		return None(f64);
	}
}

StdOption(u8) std_checked_div_u8(u8 left, u8 right) {
	return (right != 0) ? Some(u8, left / right) : None(u8);
}

StdOption(u16) std_checked_div_u16(u16 left, u16 right) {
	return (right != 0) ? Some(u16, left / right) : None(u16);
}

StdOption(u32) std_checked_div_u32(u32 left, u32 right) {
	return (right != 0) ? Some(u32, left / right) : None(u32);
}

StdOption(u64) std_checked_div_u64(u64 left, u64 right) {
	return (right != 0) ? Some(u64, left / right) : None(u64);
}

StdOption(i8) std_checked_div_i8(i8 left, i8 right) {
	return (right != 0) ? Some(i8, left / right) : None(i8);
}

StdOption(i16) std_checked_div_i16(i16 left, i16 right) {
	return (right != 0) ? Some(i16, left / right) : None(i16);
}

StdOption(i32) std_checked_div_i32(i32 left, i32 right) {
	return (right != 0) ? Some(i32, left / right) : None(i32);
}

StdOption(i64) std_checked_div_i64(i64 left, i64 right) {
	return (right != 0) ? Some(i64, left / right) : None(i64);
}

StdOption(f32) std_checked_div_f32(f32 left, f32 right) {
	if(right == 0) {
		return None(f32);
	}
	else {
		let res = std_checked_mul_f32(left, 1.0F / right);
		if(std_option_is_some(res)) {
			let ret = left / right;
			return Some(f32, ret);
		}
		else {
			return None(f32);
		}
	}
}

StdOption(f64) std_checked_div_f64(f64 left, f64 right) {
	if(right == 0) {
		return None(f64);
	}
	else {
		let res = std_checked_mul_f64(left, 1.0 / right);
		if(std_option_is_some(res)) {
			let ret = left / right;
			return Some(f64, ret);
		}
		else {
			return None(f64);
		}
	}
}
