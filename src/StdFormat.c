/// @file StdFormat.c
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief StdFormat brings human readable string formatting, similar to C++'s `std::format` and
/// `fmtlib`, and Rust's std::format, to C.
/// @version 0.2.1
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

#include <C2nxt/StdFormat.h>
#include <C2nxt/StdMath.h>
#include <C2nxt/StdPlatform.h>
#include <C2nxt/StdRange.h>
#include <C2nxt/StdVector.h>
#include <math.h>

#define LN_2  0.6931471806F // NOLINT
#define LN_10 2.302585093F	// NOLINT

#define NUM_POWERS_OF_10 40
static const f64 powers_of_10[NUM_POWERS_OF_10] = {
	1.0,
	10.0,
	100.0,
	1000.0,
	10000.0,
	100000.0,
	1000000.0,
	10000000.0,
	100000000.0,
	1000000000.0,
	10000000000.0,
	100000000000.0,
	1000000000000.0,
	10000000000000.0,
	100000000000000.0,
	1000000000000000.0,
	10000000000000000.0,
	100000000000000000.0,
	1000000000000000000.0,
	10000000000000000000.0,
	100000000000000000000.0,
	1000000000000000000000.0,
	10000000000000000000000.0,
	100000000000000000000000.0,
	1000000000000000000000000.0,
	10000000000000000000000000.0,
	100000000000000000000000000.0,
	1000000000000000000000000000.0,
	10000000000000000000000000000.0,
	100000000000000000000000000000.0,
	1000000000000000000000000000000.0,
	10000000000000000000000000000000.0,
	100000000000000000000000000000000.0,
	1000000000000000000000000000000000.0,
	10000000000000000000000000000000000.0,
	100000000000000000000000000000000000.0,
	1000000000000000000000000000000000000.0,
	10000000000000000000000000000000000000.0,
	100000000000000000000000000000000000000.0,
	1000000000000000000000000000000000000000.0,
};

[[nodiscard]] [[always_inline]] static inline u8 std_get_digit_u64(u64 num, usize digit) {
	return (digit != 0 ? (num / static_cast(u64)(powers_of_10[digit])) % 10 : // NOLINT
						 num % 10);													  // NOLINT
}

[[nodiscard]] [[always_inline]] static inline u8 std_get_digit_i64(i64 num, usize digit) {
	num = abs(num);
	return static_cast(u8)(digit != 0 ?
							   (num / static_cast(i64)(powers_of_10[digit])) % 10 : // NOLINT
							   num % 10);											// NOLINT
}

#if STD_PLATFORM_APPLE
	// clang-format off
#define std_get_digit(num, digit) _Generic((num), 		\
		char  	: 	std_get_digit_u64, 					\
		u8  	: 	std_get_digit_u64, 					\
		u16  	: 	std_get_digit_u64, 					\
		u32  	: 	std_get_digit_u64, 					\
		u64  	: 	std_get_digit_u64, 					\
		usize  	: 	std_get_digit_u64, 					\
		i8 		: 	std_get_digit_i64, 					\
		i16 	: 	std_get_digit_i64, 					\
		i32 	: 	std_get_digit_i64, 					\
		i64 	: 	std_get_digit_i64, 					\
		isize 	: 	std_get_digit_i64)(num, digit)
// clang-format on
#else
	// clang-format off
#define std_get_digit(num, digit) _Generic((num), 		\
		char  	: 	std_get_digit_u64, 					\
		u8  	: 	std_get_digit_u64, 					\
		u16  	: 	std_get_digit_u64, 					\
		u32  	: 	std_get_digit_u64, 					\
		u64  	: 	std_get_digit_u64, 					\
		i8 		: 	std_get_digit_i64, 					\
		i16 	: 	std_get_digit_i64, 					\
		i32 	: 	std_get_digit_i64, 					\
		i64 	: 	std_get_digit_i64)(num, digit)
// clang-format on
#endif

[[nodiscard]] [[always_inline]] static inline char std_num_to_char(u8 num) {
	return static_cast(char)(num) + '0';
}

[[nodiscard]] [[always_inline]] static inline u8 std_get_hex(usize num, usize digit) {
	return (num >> (digit * 4U)) & 0xFU; // NOLINT
}

[[nodiscard]] [[always_inline]] static inline char std_num_to_hex_lower(u8 num) {
	switch(num) {
		case 0 ... 9: return static_cast(char)(num) + '0';		  // NOLINT
		case 10 ... 15: return 'a' + static_cast(char)(num) - 10; // NOLINT
		default: unreachable();
	}
}

[[nodiscard]] [[always_inline]] static inline char std_num_to_hex_upper(u8 num) {
	switch(num) {
		case 0 ... 9: return static_cast(char)(num) + '0';			// NOLINT
		case 10 ... 15: return 'A' + (static_cast(char)(num) - 10); // NOLINT
		default: unreachable();
	}
}

[[nodiscard]] [[always_inline]] static inline u8
// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
std_get_digit_before_decimal(f64 num, usize digit) {
	let num_ = static_cast(i64)(num);
	return std_get_digit(num_, digit);
}

[[nodiscard]] [[always_inline]] static inline u8
// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
std_get_digit_after_decimal(f64 num, usize digit, usize num_sig_figs) {
	let num_ = static_cast(isize)(num * powers_of_10[num_sig_figs]);
	return std_get_digit(num_, (num_sig_figs - 1) - digit);
}

[[nodiscard]] [[always_inline]] static inline usize
std_get_num_digits_before_decimal(f64 num, i64 exponent) {
	return narrow_cast(usize)(abs(num) < 10.0 ? // NOLINT
								  1 :
								  exponent);
}

typedef enum StdTypesNumDecimalTextDigits {
	NUM_DIGITS_CHAR = 3,
	NUM_DIGITS_U8 = 3,
	NUM_DIGITS_U16 = 5,
	NUM_DIGITS_U32 = 10,
	NUM_DIGITS_U64 = 20,
	NUM_DIGITS_I8 = 4,
	NUM_DIGITS_I16 = 6,
	NUM_DIGITS_I32 = 11,
	NUM_DIGITS_I64 = 20,
	NUM_DIGITS_PTR = sizeof(nullptr_t) == sizeof(u32) ? NUM_DIGITS_U32 : NUM_DIGITS_U64
} StdTypesNumDecimalTextDigits;

typedef enum StdTypesNumHexTextDigits {
	NUM_HEX_DIGITS_CHAR = 2,
	NUM_HEX_DIGITS_U8 = 2,
	NUM_HEX_DIGITS_U16 = 4,
	NUM_HEX_DIGITS_U32 = 8,
	NUM_HEX_DIGITS_U64 = 16,
	NUM_HEX_DIGITS_I8 = 2,
	NUM_HEX_DIGITS_I16 = 4,
	NUM_HEX_DIGITS_I32 = 8,
	NUM_HEX_DIGITS_I64 = 16,
	NUM_HEX_DIGITS_PTR = sizeof(nullptr_t) == sizeof(u32) ? NUM_HEX_DIGITS_U32 : NUM_HEX_DIGITS_U64
} StdTypesNumHexTextDigits;

[[nodiscard]] [[always_inline]] static inline StdString
std_format_decimal_u64(u64 num, usize num_digits, StdAllocator allocator) {
	let_mut string = std_string_new_with_capacity_with_allocator(num_digits, allocator);
	let_mut gotten_non_zero = false;
	ranged_for(i, 0U, num_digits) {
		let digit = std_get_digit(num, (num_digits - 1) - i);
		if(gotten_non_zero || digit != 0) {
			std_string_push_back(string, std_num_to_char(digit));
			gotten_non_zero = true;
		}
	}
	return gotten_non_zero ? string : ({
		std_string_push_back(string, '0');
		string;
	});
}

[[nodiscard]] [[always_inline]] static inline StdString
std_format_decimal_i64(i64 num, usize num_digits, StdAllocator allocator) {
	let_mut string = std_string_new_with_capacity_with_allocator(num_digits + 1, allocator);
	let_mut gotten_non_zero = false;
	if(num < 0) {
		std_string_push_back(string, '-');
	}

	ranged_for(i, 0U, num_digits) {
		let digit = std_get_digit(num, (num_digits - 1) - i);
		if(gotten_non_zero || digit != 0) {
			std_string_push_back(string, std_num_to_char(digit));
			gotten_non_zero = true;
		}
	}
	return gotten_non_zero ? string : ({
		std_string_push_back(string, '0');
		string;
	});
}

// clang-format off
#define std_format_decimal(num, num_digits, allocator) _Generic((num),\
		char 		: 	std_format_decimal_u64,\
		u8 			: 	std_format_decimal_u64,\
		u16 		: 	std_format_decimal_u64,\
		u32 		: 	std_format_decimal_u64,\
		u64 		: 	std_format_decimal_u64,\
		i8 			: 	std_format_decimal_i64,\
		i16 		: 	std_format_decimal_i64,\
		i32 		: 	std_format_decimal_i64,\
		i64 		: 	std_format_decimal_i64)(num, num_digits, allocator)
// clang-format on

[[nodiscard]] StdString
// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
std_format_hex(u64 num, usize num_digits, StdFormatTypes type, StdAllocator allocator) {
	let_mut string = std_string_new_with_capacity_with_allocator(num_digits + 2, allocator);
	let_mut gotten_non_zero = false;

	std_string_push_back(string, '0');
	if(type == STD_FORMAT_TYPE_HEX_LOWER) {
		std_string_push_back(string, 'x');
		ranged_for(i, 0U, num_digits) {
			let digit = std_get_hex(num, (num_digits - 1) - i);
			if(gotten_non_zero || digit != 0) {
				std_string_push_back(string, std_num_to_hex_lower(digit));
				gotten_non_zero = true;
			}
		}
	}
	else {
		std_string_push_back(string, 'X');
		ranged_for(i, 0U, num_digits) {
			let digit = std_get_hex(num, (num_digits - 1) - i);
			if(gotten_non_zero || digit != 0) {
				std_string_push_back(string, std_num_to_hex_upper(digit));
				gotten_non_zero = true;
			}
		}
	}
	return gotten_non_zero ? string : ({
		std_string_push_back(string, '0');
		string;
	});
}

StdString std_format_bool(const StdFormat* restrict self, StdFormatSpecifier specifier) {
	return std_format_bool_with_allocator(self, specifier, std_allocator_new());
}

StdString std_format_bool_with_allocator(const StdFormat* restrict self,
										 [[maybe_unused]] StdFormatSpecifier specifier,
										 StdAllocator allocator) {
	std_assert(specifier.m_type == STD_FORMAT_TYPE_DEFAULT,
			   "Can't format bool with custom specifier");

	let _self = *static_cast(bool*)(self->m_self);
	if(_self) {
		return std_string_from_with_allocator("true", allocator);
	}
	else {
		return std_string_from_with_allocator("false", allocator);
	}
}

StdString std_format_char(const StdFormat* restrict self, StdFormatSpecifier specifier) {
	return std_format_char_with_allocator(self, specifier, std_allocator_new());
}

StdString std_format_char_with_allocator(const StdFormat* restrict self,
										 StdFormatSpecifier specifier,
										 StdAllocator allocator) {
	std_assert(specifier.m_type != STD_FORMAT_TYPE_SCIENTIFIC,
			   "Cannot format char as scientific notation");

	if(specifier.m_type == STD_FORMAT_TYPE_DEFAULT || specifier.m_type == STD_FORMAT_TYPE_DEBUG) {
		return std_string_from_cstring_with_allocator(static_cast(const_cstring)(self->m_self),
													  1,
													  allocator);
	}
	else if(specifier.m_type == STD_FORMAT_TYPE_DECIMAL) {
		let num = *static_cast(const char*)(self->m_self);
		return std_format_decimal(static_cast(u64)(num), NUM_DIGITS_CHAR, allocator);
	}
	else {
		let num = static_cast(u8)(*(static_cast(const char*)(self->m_self)));
		return std_format_hex(num, NUM_HEX_DIGITS_CHAR, specifier.m_type, allocator);
	}
}

StdString std_format_u8(const StdFormat* restrict self, StdFormatSpecifier specifier) {
	return std_format_u8_with_allocator(self, specifier, std_allocator_new());
}

StdString std_format_u8_with_allocator(const StdFormat* restrict self,
									   StdFormatSpecifier specifier,
									   StdAllocator allocator) {
	std_assert(specifier.m_type != STD_FORMAT_TYPE_SCIENTIFIC,
			   "Cannot format u8 as scientific notation");

	let num = *static_cast(const u8*)(self->m_self);
	return (specifier.m_type == STD_FORMAT_TYPE_DEFAULT
			|| specifier.m_type == STD_FORMAT_TYPE_DECIMAL
			|| specifier.m_type == STD_FORMAT_TYPE_DEBUG) ?
			   std_format_decimal(num, NUM_DIGITS_U8, allocator) :
			   std_format_hex(num, NUM_HEX_DIGITS_U8, specifier.m_type, allocator);
}

StdString std_format_u16(const StdFormat* restrict self, StdFormatSpecifier specifier) {
	return std_format_u16_with_allocator(self, specifier, std_allocator_new());
}

StdString std_format_u16_with_allocator(const StdFormat* restrict self,
										StdFormatSpecifier specifier,
										StdAllocator allocator) {
	std_assert(specifier.m_type != STD_FORMAT_TYPE_SCIENTIFIC,
			   "Cannot format u16 as scientific notation");

	let num = *static_cast(const u16*)(self->m_self);
	return (specifier.m_type == STD_FORMAT_TYPE_DEFAULT
			|| specifier.m_type == STD_FORMAT_TYPE_DECIMAL
			|| specifier.m_type == STD_FORMAT_TYPE_DEBUG) ?
			   std_format_decimal(num, NUM_DIGITS_U16, allocator) :
			   std_format_hex(num, NUM_HEX_DIGITS_U16, specifier.m_type, allocator);
}

StdString std_format_u32(const StdFormat* restrict self, StdFormatSpecifier specifier) {
	return std_format_u32_with_allocator(self, specifier, std_allocator_new());
}

StdString std_format_u32_with_allocator(const StdFormat* restrict self,
										StdFormatSpecifier specifier,
										StdAllocator allocator) {
	std_assert(specifier.m_type != STD_FORMAT_TYPE_SCIENTIFIC,
			   "Cannot format u32 as scientific notation");

	let num = *static_cast(const u32*)(self->m_self);
	return (specifier.m_type == STD_FORMAT_TYPE_DEFAULT
			|| specifier.m_type == STD_FORMAT_TYPE_DECIMAL
			|| specifier.m_type == STD_FORMAT_TYPE_DEBUG) ?
			   std_format_decimal(num, NUM_DIGITS_U32, allocator) :
			   std_format_hex(num, NUM_HEX_DIGITS_U32, specifier.m_type, allocator);
}

StdString std_format_u64(const StdFormat* restrict self, StdFormatSpecifier specifier) {
	return std_format_u64_with_allocator(self, specifier, std_allocator_new());
}

StdString std_format_u64_with_allocator(const StdFormat* restrict self,
										StdFormatSpecifier specifier,
										StdAllocator allocator) {
	std_assert(specifier.m_type != STD_FORMAT_TYPE_SCIENTIFIC,
			   "Cannot format u64 as scientific notation");

	let num = *static_cast(const u64*)(self->m_self);
	return (specifier.m_type == STD_FORMAT_TYPE_DEFAULT
			|| specifier.m_type == STD_FORMAT_TYPE_DECIMAL
			|| specifier.m_type == STD_FORMAT_TYPE_DEBUG) ?
			   std_format_decimal(num, NUM_DIGITS_U64, allocator) :
			   std_format_hex(num, NUM_HEX_DIGITS_U64, specifier.m_type, allocator);
}

StdString std_format_i8(const StdFormat* restrict self, StdFormatSpecifier specifier) {
	return std_format_i8_with_allocator(self, specifier, std_allocator_new());
}

StdString std_format_i8_with_allocator(const StdFormat* restrict self,
									   StdFormatSpecifier specifier,
									   StdAllocator allocator) {
	std_assert(specifier.m_type != STD_FORMAT_TYPE_SCIENTIFIC,
			   "Cannot format i8 as scientific notation");

	let num = *static_cast(const i8*)(self->m_self);
	return (specifier.m_type == STD_FORMAT_TYPE_DEFAULT
			|| specifier.m_type == STD_FORMAT_TYPE_DECIMAL
			|| specifier.m_type == STD_FORMAT_TYPE_DEBUG) ?
			   std_format_decimal(num, NUM_DIGITS_I8, allocator) :
			   std_format_hex(static_cast(u64)(num),
							  NUM_HEX_DIGITS_I8,
							  specifier.m_type,
							  allocator);
}

StdString std_format_i16(const StdFormat* restrict self, StdFormatSpecifier specifier) {
	return std_format_i16_with_allocator(self, specifier, std_allocator_new());
}

StdString std_format_i16_with_allocator(const StdFormat* restrict self,
										StdFormatSpecifier specifier,
										StdAllocator allocator) {
	std_assert(specifier.m_type != STD_FORMAT_TYPE_SCIENTIFIC,
			   "Cannot format i16 as scientific notation");

	let num = *static_cast(const i16*)(self->m_self);
	return (specifier.m_type == STD_FORMAT_TYPE_DEFAULT
			|| specifier.m_type == STD_FORMAT_TYPE_DECIMAL
			|| specifier.m_type == STD_FORMAT_TYPE_DEBUG) ?
			   std_format_decimal(num, NUM_DIGITS_I16, allocator) :
			   std_format_hex(static_cast(u64)(num),
							  NUM_HEX_DIGITS_I16,
							  specifier.m_type,
							  allocator);
}

StdString std_format_i32(const StdFormat* restrict self, StdFormatSpecifier specifier) {
	return std_format_i32_with_allocator(self, specifier, std_allocator_new());
}

StdString std_format_i32_with_allocator(const StdFormat* restrict self,
										StdFormatSpecifier specifier,
										StdAllocator allocator) {
	std_assert(specifier.m_type != STD_FORMAT_TYPE_SCIENTIFIC,
			   "Cannot format i32 as scientific notation");

	let num = *static_cast(const i32*)(self->m_self);
	return (specifier.m_type == STD_FORMAT_TYPE_DEFAULT
			|| specifier.m_type == STD_FORMAT_TYPE_DECIMAL
			|| specifier.m_type == STD_FORMAT_TYPE_DEBUG) ?
			   std_format_decimal(num, NUM_DIGITS_I32, allocator) :
			   std_format_hex(static_cast(u64)(num),
							  NUM_HEX_DIGITS_I32,
							  specifier.m_type,
							  allocator);
}

StdString std_format_i64(const StdFormat* restrict self, StdFormatSpecifier specifier) {
	return std_format_i64_with_allocator(self, specifier, std_allocator_new());
}

StdString std_format_i64_with_allocator(const StdFormat* restrict self,
										StdFormatSpecifier specifier,
										StdAllocator allocator) {
	std_assert(specifier.m_type != STD_FORMAT_TYPE_SCIENTIFIC,
			   "Cannot format i64 as scientific notation");

	let num = *static_cast(const i64*)(self->m_self);
	return (specifier.m_type == STD_FORMAT_TYPE_DEFAULT
			|| specifier.m_type == STD_FORMAT_TYPE_DECIMAL
			|| specifier.m_type == STD_FORMAT_TYPE_DEBUG) ?
			   std_format_decimal(num, NUM_DIGITS_I64, allocator) :
			   std_format_hex(static_cast(u64)(num),
							  NUM_HEX_DIGITS_I64,
							  specifier.m_type,
							  allocator);
}

// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
[[nodiscard]] StdString std_format_f64_scientific(f64 shifted_to_scientific,
												  i64 exponent,
												  usize num_sig_figs,
												  StdAllocator allocator) {
	// possible -, first digit, point, Exxx, num sig figs
	let num_chars_in_float = 1 + 1 + 1 + 4 + num_sig_figs;
	let_mut string = std_string_new_with_capacity_with_allocator(num_chars_in_float, allocator);

	if(shifted_to_scientific < 0) {
		std_string_push_back(string, '-');
	}

	std_string_push_back(string,
						 std_num_to_char(std_get_digit_before_decimal(shifted_to_scientific, 0)));
	std_string_push_back(string, '.');
	ranged_for(i, 0U, num_sig_figs) {
		std_string_push_back(
			string,
			std_num_to_char(std_get_digit_after_decimal(shifted_to_scientific, i, num_sig_figs)));
	}
	std_string_push_back(string, 'E');
	let_mut gotten_non_zero = false;
	ranged_for(i, 0U, 2) {
		let digit = std_get_digit(exponent, 1U - i);
		if(gotten_non_zero || digit != 0.0) {
			std_string_push_back(string, std_num_to_char(digit));
			gotten_non_zero = true;
		}
	}
	return gotten_non_zero ? string : ({
		std_string_push_back(string, '0');
		string;
	});
}

[[nodiscard]] StdString
// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
std_format_f64_decimal(f64 num, i64 exponent, usize num_sig_figs, StdAllocator allocator) {
	let num_chars_before_decimal = std_get_num_digits_before_decimal(num, exponent);
	// possible -, num digits before point, point, num sig figs after point
	let num_chars_in_float = 1 + num_chars_before_decimal + 1 + num_sig_figs;
	let_mut string = std_string_new_with_capacity_with_allocator(num_chars_in_float, allocator);

	if(num < 0) {
		std_string_push_back(string, '-');
	}

	let_mut gotten_non_zero = false;
	ranged_for(i, 0U, num_chars_before_decimal + 1) {
		let digit = std_get_digit_before_decimal(num, num_chars_before_decimal - i);
		if(gotten_non_zero || digit != 0.0) {
			std_string_push_back(string, std_num_to_char(digit));
			gotten_non_zero = true;
		}
	}

	if(!gotten_non_zero) {
		std_string_push_back(string, '0');
	}

	std_string_push_back(string, '.');
	ranged_for(i, 0U, num_sig_figs) {
		std_string_push_back(string,
							 std_num_to_char(std_get_digit_after_decimal(num, i, num_sig_figs)));
	}

	return string;
}

[[nodiscard]] [[always_inline]] static inline StdString
std_format_f64_with_allocator__(f64 num, StdFormatSpecifier specifier, StdAllocator allocator) {
	let_mut exponent_base_2 = 0;
	ignore(frexp(num, &exponent_base_2));
	let exponent = static_cast(i64)(static_cast(f32)(exponent_base_2) * LN_2 / LN_10);
	return (specifier.m_type == STD_FORMAT_TYPE_DEFAULT
			|| specifier.m_type == STD_FORMAT_TYPE_SCIENTIFIC
			|| specifier.m_type == STD_FORMAT_TYPE_DEBUG) ?
			   std_format_f64_scientific(num / powers_of_10[exponent],
										 exponent,
										 specifier.m_num_sig_figs,
										 allocator) :
			   std_format_f64_decimal(num, exponent, specifier.m_num_sig_figs, allocator);
}

StdString std_format_f32(const StdFormat* restrict self, StdFormatSpecifier specifier) {
	return std_format_f32_with_allocator(self, specifier, std_allocator_new());
}

StdString std_format_f32_with_allocator(const StdFormat* restrict self,
										StdFormatSpecifier specifier,
										StdAllocator allocator) {
	std_assert(specifier.m_type != STD_FORMAT_TYPE_HEX_LOWER
				   && specifier.m_type != STD_FORMAT_TYPE_HEX_UPPER,
			   "Cannot format f32 as hex");

	let num = *static_cast(const f32*)(self->m_self);
	return std_format_f64_with_allocator__(num, specifier, allocator);
}

StdString std_format_f64(const StdFormat* restrict self, StdFormatSpecifier specifier) {
	return std_format_f64_with_allocator(self, specifier, std_allocator_new());
}

StdString std_format_f64_with_allocator(const StdFormat* restrict self,
										StdFormatSpecifier specifier,
										StdAllocator allocator) {
	std_assert(specifier.m_type != STD_FORMAT_TYPE_HEX_LOWER
				   && specifier.m_type != STD_FORMAT_TYPE_HEX_UPPER,
			   "Cannot format f64 as hex");

	let num = *static_cast(const f64*)(self->m_self);
	return std_format_f64_with_allocator__(num, specifier, allocator);
}

StdString std_format_ptr(const StdFormat* restrict self, StdFormatSpecifier specifier) {
	return std_format_ptr_with_allocator(self, specifier, std_allocator_new());
}

StdString std_format_ptr_with_allocator(const StdFormat* restrict self,
										StdFormatSpecifier specifier,
										StdAllocator allocator) {
	std_assert(specifier.m_type != STD_FORMAT_TYPE_DECIMAL
				   && specifier.m_type != STD_FORMAT_TYPE_SCIENTIFIC,
			   "Cannot format a pointer as decimal number or scientific notation");

	let num = static_cast(usize)(static_cast(void*)(self->m_self));
	let_mut string = std_string_new_with_capacity_with_allocator(2 + NUM_HEX_DIGITS_PTR, allocator);

	if(specifier.m_type == STD_FORMAT_TYPE_DEFAULT || specifier.m_type == STD_FORMAT_TYPE_HEX_LOWER
	   || specifier.m_type == STD_FORMAT_TYPE_DEBUG)
	{
		std_string_append(string, "0x");
		ranged_for(i, 0U, NUM_HEX_DIGITS_PTR) {
			std_string_push_back(
				string,
				std_num_to_hex_lower(std_get_hex(num, (NUM_HEX_DIGITS_PTR - 1) - i)));
		}
	}
	else {
		std_string_append(string, "0X");
		ranged_for(i, 0U, NUM_HEX_DIGITS_PTR) {
			std_string_push_back(
				string,
				std_num_to_hex_upper(std_get_hex(num, (NUM_HEX_DIGITS_PTR - 1) - i)));
		}
	}
	return string;
}

StdString std_format_std_string(const StdFormat* restrict self, StdFormatSpecifier specifier) {
	ignore(specifier);

	let string = static_cast(const StdString*)(self->m_self);
	return std_string_clone(*string);
}

StdString std_format_std_string_with_allocator(const StdFormat* restrict self,
											   StdFormatSpecifier specifier,
											   StdAllocator allocator) {
	ignore(specifier);

	let string = static_cast(const StdString*)(self->m_self);
	return std_string_clone_with_allocator(*string, allocator);
}

StdString std_format_std_stringview(const StdFormat* restrict self, StdFormatSpecifier specifier) {
	ignore(specifier);

	let view = static_cast(const StdStringView*)(self->m_self);
	return std_string_from(view);
}

StdString std_format_std_stringview_with_allocator(const StdFormat* restrict self,
												   StdFormatSpecifier specifier,
												   StdAllocator allocator) {
	ignore(specifier);

	let view = static_cast(const StdStringView*)(self->m_self);
	return std_string_from_with_allocator(view, allocator);
}

StdString std_format_cstring(const StdFormat* restrict self, StdFormatSpecifier specifier) {
	ignore(specifier);

	let string = static_cast(const_cstring)(self->m_self);
	return std_string_from(string);
}

StdString std_format_cstring_with_allocator(const StdFormat* restrict self,
											StdFormatSpecifier specifier,
											StdAllocator allocator) {
	ignore(specifier);

	let string = static_cast(const_cstring)(self->m_self);
	return std_string_from_with_allocator(string, allocator);
}

typedef enum StdFormatErrorTypes {
	STD_FORMAT_SUCCESS = 0,
	STD_FORMAT_BAD_SPECIFIER_INVALID_CHAR_IN_SPECIFIER,
	STD_FORMAT_INVALID_CLOSING_BRACE_LOCATION,
	STD_FORMAT_UNCLOSED_SPECIFIER,
	STD_FORMAT_MORE_SPECIFIERS_THAN_ARGS,
	STD_FORMAT_FEWER_SPECIFIERS_THAN_ARGS
} StdFormatErrorTypes;

Enum(StdFormatVariant, (Substring, StdStringView), (Specifier, StdFormatSpecifier));

typedef StdFormatVariant* Ref(StdFormatVariant);
typedef const StdFormatVariant* ConstRef(StdFormatVariant);
DeclStdIterators(Ref(StdFormatVariant));
DeclStdIterators(ConstRef(StdFormatVariant));

#define STD_TEMPLATE_DECL TRUE
#define STD_TEMPLATE_IMPL TRUE

#define OPTION_T StdFormatVariant
#include <C2nxt/StdOption.h>
#undef OPTION_T

#define T				  StdFormatVariant
#include <C2nxt/StdVector.h>
#undef T

#undef STD_TEMPLATE_DECL
#undef STD_TEMPLATE_IMPL

#define T				  StdVector(StdFormatVariant)
#define STD_TEMPLATE_DECL TRUE
#define STD_TEMPLATE_IMPL TRUE

#include <C2nxt/StdResult.h>

#undef T
#undef STD_TEMPLATE_DECL
#undef STD_TEMPLATE_IMPL

#define Substring(string_view)                                             \
	(StdFormatVariant) {                                                   \
		.tag = Substring, .variant_identifier(Substring) = { string_view } \
	}
#define Specifier(specifier)                                             \
	(StdFormatVariant) {                                                 \
		.tag = Specifier, .variant_identifier(Specifier) = { specifier } \
	}

[[nodiscard]] [[always_inline]] static inline StdFormatVariant
std_format_variant_new([[maybe_unused]] StdAllocator allocator) {
	return (StdFormatVariant){0};
}

[[not_null(1)]] [[always_inline]] static inline void
std_format_variant_free([[maybe_unused]] StdFormatVariant* elem,
						[[maybe_unused]] StdAllocator allocator) {
}

[[nodiscard]] [[not_null(1)]] [[always_inline]] static inline StdFormatVariant
std_format_variant_clone(const StdFormatVariant* restrict elem,
						 [[maybe_unused]] StdAllocator allocator) {
	return *elem;
}

// clang-format off
#define std_format_pair_from(x) _Generic(&(x), \
		StdFormatSpecifier* 			: 	std_format_pair_from_specifier, 	\
		const StdFormatSpecifier* 		: 	std_format_pair_from_specifier, 	\
		StdStringView* 					: 	std_format_pair_from_stringview, 	\
		const StdStringView* 			: std_format_pair_from_stringview)(x)
//clang-format on

[[nodiscard]] [[returns_not_null]] const_cstring std_format_error_message_function(i64 error_code) {
	if(error_code == STD_FORMAT_SUCCESS) {
		return "No error: Formatting successful";
	}
	else if(error_code == STD_FORMAT_BAD_SPECIFIER_INVALID_CHAR_IN_SPECIFIER) {
		return "Error: Bad format specifier provided to std_format, invalid char in specifier";
	}
	else if(error_code == STD_FORMAT_INVALID_CLOSING_BRACE_LOCATION) {
		return "Error: Invalid specifier closing brace location in format string";
	}
	else if(error_code == STD_FORMAT_UNCLOSED_SPECIFIER) {
		return "Error: Unclosed specifier in format string";
	}
	else if(error_code == STD_FORMAT_MORE_SPECIFIERS_THAN_ARGS) {
		return "Error: More format specifiers provided than arguments to function";
	}
	else if(error_code == STD_FORMAT_FEWER_SPECIFIERS_THAN_ARGS) {
		return "Error: Fewer format specifiers provided than arguments to function";
	}
	unreachable();
}

[[maybe_unused]] static const StdErrorCategory std_format_error_category
	= {.m_message_function = std_format_error_message_function};

[[nodiscard]] [[not_null(1)]] [[always_inline]]
static inline usize std_format_get_num_sig_figs_from_substring(
									restrict const_cstring format_string,
									// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
									usize length,
									usize start_index)
{
	let_mut multiplier = 1;
	let_mut num_figs = 0U;
	let_mut num_digits = 0U;
	let_mut first = start_index - 1;
	let_mut last = start_index - 1;
	ranged_for(i, start_index, length) {
		if(format_string[i] == '}') {
			break;
		}
		else if(format_string[i] >= '0' && format_string[i] <= '9') {
			num_digits++;
			last = i;
			if(first < start_index) {
				first = i;
			}
			else {
				multiplier *= 10; // NOLINT
			}
		}
	}

	if(first < start_index) {
		return STD_FORMAT_DEFAULT_NUM_SIG_FIGS;
	}

	ranged_for(i, first, last + 1) {
		num_figs += static_cast(u32)(multiplier * (format_string[i] - '0'));
		multiplier /= 10; // NOLINT
	}

	return num_figs;
}

[[nodiscard]] bool std_format_is_char_valid_in_specifier(char character) {
	return character == STD_FORMAT_TYPE_DECIMAL || character == STD_FORMAT_TYPE_HEX_LOWER
		   || character == STD_FORMAT_TYPE_HEX_UPPER || character == STD_FORMAT_TYPE_SCIENTIFIC
		   || character == STD_FORMAT_TYPE_DEBUG || (character >= '0' && character <= '9');
}


static let format_variant_data = (StdCollectionData(StdVector(StdFormatVariant))) {
.m_constructor = std_format_variant_new,
.m_destructor = std_format_variant_free,
.m_copy_constructor = std_format_variant_clone};

[[nodiscard]] [[not_null(1)]] StdResult(StdVector(StdFormatVariant))
	std_format_parse_and_validate_format_string(restrict const_cstring format_string, // NOLINT
											// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
											usize length,
											[[maybe_unused]] usize num_args,
											StdAllocator allocator)
{
	let_mut vec = std_vector_new_with_capacity_allocator_and_collection_data(StdFormatVariant,
			num_args * 2, allocator, &format_variant_data);

	let_mut in_specifier = false;
	let_mut pushed_specifier = false;
	let_mut num_open = 0U;
	let_mut num_close = 0U;
	let_mut start_index = 0U;
	ranged_for(i, 0U, length) {
		if(format_string[i] == '{') {
#if STD_PLATFORM_DEBUG
			if(in_specifier) {
				return Err(StdVector(StdFormatVariant),
						   std_error_new(STD_FORMAT_BAD_SPECIFIER_INVALID_CHAR_IN_SPECIFIER,
										 std_format_error_category));
			}
			else
#endif // STD_PLATFORM_DEBUG
			if(i > 0) {
				if(format_string[i - 1] != '\\') {
					in_specifier = true;
					pushed_specifier = false;
					num_open++;
					let view = std_stringview_from(format_string, start_index, i - start_index);
					std_vector_push_back(vec,
									 Substring(view));
				}
			}
			else {
				in_specifier = true;
				num_open++;
			}
		}
		else if(format_string[i] == '}') {
#if STD_PLATFORM_DEBUG
			if(!in_specifier) {
				return Err(StdVector(StdFormatVariant),
						   std_error_new(STD_FORMAT_INVALID_CLOSING_BRACE_LOCATION,
										 std_format_error_category));
			}
			else if(format_string[i - 1] == '\\') {
				return Err(StdVector(StdFormatVariant),
						   std_error_new(STD_FORMAT_BAD_SPECIFIER_INVALID_CHAR_IN_SPECIFIER,
										 std_format_error_category));
			}
			else {
				if(!pushed_specifier) {
					std_vector_push_back(vec,
										 Specifier(((StdFormatSpecifier){
											 .m_type = STD_FORMAT_TYPE_DEFAULT,
											 .m_num_sig_figs = STD_FORMAT_DEFAULT_NUM_SIG_FIGS})));
				}
				in_specifier = false;
				num_close++;
				start_index = i + 1;
			}
#else  // STD_PLATFORM_DEBUG
			if(!pushed_specifier) {
				std_vector_push_back(vec,
									 Specifier(((StdFormatSpecifier){
										 .m_type = STD_FORMAT_TYPE_DEFAULT,
										 .m_num_sig_figs = STD_FORMAT_DEFAULT_NUM_SIG_FIGS})));
			}
			in_specifier = false;
			num_close++;
			start_index = i + 1;
#endif // STD_PLATFORM_DEBUG
		}
		else if(in_specifier) {
#if STD_PLATFORM_DEBUG
			if(!std_format_is_char_valid_in_specifier(format_string[i])) {
				return Err(StdVector(StdFormatVariant),
						   std_error_new(STD_FORMAT_BAD_SPECIFIER_INVALID_CHAR_IN_SPECIFIER,
										 std_format_error_category));
			}
#endif // STD_PLATFORM_DEBUG

			if(format_string[i] == STD_FORMAT_TYPE_DECIMAL) {
				std_vector_push_back(
					vec,
					Specifier(((StdFormatSpecifier){
						.m_type = STD_FORMAT_TYPE_DECIMAL,
						.m_num_sig_figs = std_format_get_num_sig_figs_from_substring(format_string,
																					 length,
																					 i + 1)})));
				pushed_specifier = true;
			}
			else if(format_string[i] == STD_FORMAT_TYPE_HEX_LOWER) {
				std_vector_push_back(
					vec,
					Specifier(((StdFormatSpecifier){.m_type = STD_FORMAT_TYPE_HEX_LOWER,
															   .m_num_sig_figs = 0})));
				pushed_specifier = true;
			}
			else if(format_string[i] == STD_FORMAT_TYPE_HEX_UPPER) {
				std_vector_push_back(
					vec,
					Specifier(((StdFormatSpecifier){.m_type = STD_FORMAT_TYPE_HEX_UPPER,
															   .m_num_sig_figs = 0})));
				pushed_specifier = true;
			}
			else if(format_string[i] == STD_FORMAT_TYPE_SCIENTIFIC) {
				std_vector_push_back(
					vec,
					Specifier(((StdFormatSpecifier){
						.m_type = STD_FORMAT_TYPE_SCIENTIFIC,
						.m_num_sig_figs = std_format_get_num_sig_figs_from_substring(format_string,
																					 length,
																					 i + 1)})));
				pushed_specifier = true;
			}
			else if(format_string[i] == STD_FORMAT_TYPE_DEBUG) {
				std_vector_push_back(
					vec,
					Specifier(((StdFormatSpecifier){
						.m_type = STD_FORMAT_TYPE_DEBUG,
						.m_num_sig_figs = std_format_get_num_sig_figs_from_substring(format_string,
																					 length,
																					 i + 1)})));
				pushed_specifier = true;
			}
		}
	}

	if(start_index < length) {
		let view = std_stringview_from(format_string, start_index, length - start_index);
		std_vector_push_back(vec, Substring(view));
	}

#if STD_PLATFORM_DEBUG
	if(num_open != num_close) {
		return Err(StdVector(StdFormatVariant),
				   std_error_new(STD_FORMAT_UNCLOSED_SPECIFIER, std_format_error_category));
	}
	else if(num_open > num_args) {
		return Err(StdVector(StdFormatVariant),
				   std_error_new(STD_FORMAT_MORE_SPECIFIERS_THAN_ARGS, std_format_error_category));
	}
	// we can't reliably check for 0 args, so make an exception for that
	else if(num_open < num_args && num_args != 1) {
		return Err(StdVector(StdFormatVariant),
				   std_error_new(STD_FORMAT_FEWER_SPECIFIERS_THAN_ARGS, std_format_error_category));
	}
#endif // STD_PLATFORM_DEBUG

	return (std_vector_size(vec) == 0) ? ({
		let view = std_stringview_from(format_string, 0, length);
		std_vector_push_back(vec, Substring(view));
		Ok(StdVector(StdFormatVariant), vec);
	}) :
										   Ok(StdVector(StdFormatVariant), vec);
}

StdString(std_format_with_allocator)(restrict const_cstring format_string,
									 StdAllocator allocator,
									 usize num_args,
									 ...) {
	va_list list = {0};
	va_start(list, num_args);
	let string = std_vformat_with_allocator(format_string, allocator, num_args, list);
	va_end(list);
	return string;
}

// NOLINTNEXTLINE(readability-function-cognitive-complexity)
StdString(std_vformat_with_allocator)(restrict const_cstring format_string,
									  StdAllocator allocator,
									  usize num_args,
									  va_list list) { // NOLINT
	let string_length = strlen(format_string);
	let_mut maybe_format_variants = std_format_parse_and_validate_format_string(format_string,
																				string_length,
																				num_args,
																				allocator);
#if STD_PLATFORM_DEBUG
	if(std_result_is_err(maybe_format_variants)) {
		let err = std_result_unwrap_err(maybe_format_variants);
		ignore(err);
		std_assert(false, std_error_message(&err));
	}
#endif // STD_PLATFORM_DEBUG

	std_vector_scoped(StdFormatVariant) format_variants
		= std_result_expect(maybe_format_variants, "Invalid format string");
	let_mut initial_size = static_cast(usize)(0);
	foreach_ref(elem, format_variants) {
		match(*elem) {
			variant(Substring, view) {
				initial_size += std_stringview_length(view);
			}
			wildcard() {
				// NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
				initial_size += 10U;
			}
		}
	}

	let_mut string = std_string_new_with_capacity_with_allocator(initial_size, allocator);

	foreach(elem, format_variants) {
		match(elem) {
			variant(Substring, view) {
				std_string_append(string, &view);
			}
			variant(Specifier, specifier) {
				let format = va_arg(list, StdFormat); // NOLINT(clang-analyzer-valist.Uninitialized)
				std_string_scoped formatted
						   = trait_call(format_with_allocator, format, specifier, allocator);
				std_string_append(string, &formatted);
			}
		}
	}

	return string;
}
