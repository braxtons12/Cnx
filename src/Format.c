/// @file Format.c
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief CnxFormat brings human readable string formatting, similar to C++'s `std::format` and
/// `fmtlib`, and Rust's std::format, to C.
/// @version 0.2.3
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

#include <Cnx/Format.h>
#include <Cnx/Math.h>
#include <Cnx/Platform.h>
#include <Cnx/Vector.h>
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

[[nodiscard]] [[always_inline]] static inline u8 cnx_get_digit_u64(u64 num, usize digit) {
	return (digit != 0 ? (num / static_cast(u64)(powers_of_10[digit])) % 10 : // NOLINT
						 num % 10);													  // NOLINT
}

[[nodiscard]] [[always_inline]] static inline u8 cnx_get_digit_i64(i64 num, usize digit) {
	num = abs(num);
	return static_cast(u8)(digit != 0 ?
							   (num / static_cast(i64)(powers_of_10[digit])) % 10 : // NOLINT
							   num % 10);											// NOLINT
}

#if CNX_PLATFORM_APPLE
// clang-format off
#define cnx_get_digit(num, digit) _Generic((num), 		\
		char  	: 	cnx_get_digit_u64, 					\
		u8  	: 	cnx_get_digit_u64, 					\
		u16  	: 	cnx_get_digit_u64, 					\
		u32  	: 	cnx_get_digit_u64, 					\
		u64  	: 	cnx_get_digit_u64, 					\
		usize  	: 	cnx_get_digit_u64, 					\
		i8 		: 	cnx_get_digit_i64, 					\
		i16 	: 	cnx_get_digit_i64, 					\
		i32 	: 	cnx_get_digit_i64, 					\
		i64 	: 	cnx_get_digit_i64, 					\
		isize 	: 	cnx_get_digit_i64)(num, digit)
// clang-format on
#else
// clang-format off
#define cnx_get_digit(num, digit) _Generic((num), 		\
		char  	: 	cnx_get_digit_u64, 					\
		u8  	: 	cnx_get_digit_u64, 					\
		u16  	: 	cnx_get_digit_u64, 					\
		u32  	: 	cnx_get_digit_u64, 					\
		u64  	: 	cnx_get_digit_u64, 					\
		i8 		: 	cnx_get_digit_i64, 					\
		i16 	: 	cnx_get_digit_i64, 					\
		i32 	: 	cnx_get_digit_i64, 					\
		i64 	: 	cnx_get_digit_i64)(num, digit)
// clang-format on
#endif

[[nodiscard]] [[always_inline]] static inline char cnx_num_to_char(u8 num) {
	return static_cast(char)(num) + '0';
}

[[nodiscard]] [[always_inline]] static inline u8 cnx_get_hex(usize num, usize digit) {
	return (num >> (digit * 4U)) & 0xFU; // NOLINT
}

[[nodiscard]] [[always_inline]] static inline char cnx_num_to_hex_lower(u8 num) {
	switch(num) {
		case 0 ... 9: return static_cast(char)(num) + '0';		  // NOLINT
		case 10 ... 15: return 'a' + static_cast(char)(num) - 10; // NOLINT
		default: unreachable();
	}
}

[[nodiscard]] [[always_inline]] static inline char cnx_num_to_hex_upper(u8 num) {
	switch(num) {
		case 0 ... 9: return static_cast(char)(num) + '0';			// NOLINT
		case 10 ... 15: return 'A' + (static_cast(char)(num) - 10); // NOLINT
		default: unreachable();
	}
}

[[nodiscard]] [[always_inline]] static inline u8
// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
cnx_get_digit_before_decimal(f64 num, usize digit) {
	let num_ = static_cast(i64)(num);
	return cnx_get_digit(num_, digit);
}

[[nodiscard]] [[always_inline]] static inline u8
// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
cnx_get_digit_after_decimal(f64 num, usize digit, usize num_sig_figs) {
	let num_ = static_cast(isize)(num * powers_of_10[num_sig_figs]);
	return cnx_get_digit(num_, (num_sig_figs - 1) - digit);
}

[[nodiscard]] [[always_inline]] static inline usize
cnx_get_num_digits_before_decimal(f64 num, i64 exponent) {
	return narrow_cast(usize)(abs(num) < 10.0 ? // NOLINT
								  1 :
								  exponent);
}

typedef enum CnxTypesNumDecimalTextDigits {
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
} CnxTypesNumDecimalTextDigits;

typedef enum CnxTypesNumHexTextDigits {
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
} CnxTypesNumHexTextDigits;

[[nodiscard]] [[always_inline]] static inline CnxString
cnx_format_decimal_u64(u64 num, usize num_digits, CnxAllocator allocator) {
	let_mut string = cnx_string_new_with_capacity_with_allocator(num_digits, allocator);
	let_mut gotten_non_zero = false;
	ranged_for(i, 0U, num_digits) {
		let digit = cnx_get_digit(num, (num_digits - 1) - i);
		if(gotten_non_zero || digit != 0) {
			cnx_string_push_back(string, cnx_num_to_char(digit));
			gotten_non_zero = true;
		}
	}
	return gotten_non_zero ? string : ({
		cnx_string_push_back(string, '0');
		string;
	});
}

[[nodiscard]] [[always_inline]] static inline CnxString
cnx_format_decimal_i64(i64 num, usize num_digits, CnxAllocator allocator) {
	let_mut string = cnx_string_new_with_capacity_with_allocator(num_digits + 1, allocator);
	let_mut gotten_non_zero = false;
	if(num < 0) {
		cnx_string_push_back(string, '-');
	}

	ranged_for(i, 0U, num_digits) {
		let digit = cnx_get_digit(num, (num_digits - 1) - i);
		if(gotten_non_zero || digit != 0) {
			cnx_string_push_back(string, cnx_num_to_char(digit));
			gotten_non_zero = true;
		}
	}
	return gotten_non_zero ? string : ({
		cnx_string_push_back(string, '0');
		string;
	});
}

// clang-format off
#define cnx_format_decimal(num, num_digits, allocator) _Generic((num),\
		char 		: 	cnx_format_decimal_u64,\
		u8 			: 	cnx_format_decimal_u64,\
		u16 		: 	cnx_format_decimal_u64,\
		u32 		: 	cnx_format_decimal_u64,\
		u64 		: 	cnx_format_decimal_u64,\
		i8 			: 	cnx_format_decimal_i64,\
		i16 		: 	cnx_format_decimal_i64,\
		i32 		: 	cnx_format_decimal_i64,\
		i64 		: 	cnx_format_decimal_i64)(num, num_digits, allocator)
// clang-format on

[[nodiscard]] CnxString
// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
cnx_format_hex(u64 num, usize num_digits, CnxFormatTypes type, CnxAllocator allocator) {
	let_mut string = cnx_string_new_with_capacity_with_allocator(num_digits + 2, allocator);
	let_mut gotten_non_zero = false;

	cnx_string_push_back(string, '0');
	if(type == CNX_FORMAT_TYPE_HEX_LOWER) {
		cnx_string_push_back(string, 'x');
		ranged_for(i, 0U, num_digits) {
			let digit = cnx_get_hex(num, (num_digits - 1) - i);
			if(gotten_non_zero || digit != 0) {
				cnx_string_push_back(string, cnx_num_to_hex_lower(digit));
				gotten_non_zero = true;
			}
		}
	}
	else {
		cnx_string_push_back(string, 'X');
		ranged_for(i, 0U, num_digits) {
			let digit = cnx_get_hex(num, (num_digits - 1) - i);
			if(gotten_non_zero || digit != 0) {
				cnx_string_push_back(string, cnx_num_to_hex_upper(digit));
				gotten_non_zero = true;
			}
		}
	}
	return gotten_non_zero ? string : ({
		cnx_string_push_back(string, '0');
		string;
	});
}

CnxString cnx_format_bool(const CnxFormat* restrict self, CnxFormatSpecifier specifier) {
	return cnx_format_bool_with_allocator(self, specifier, cnx_allocator_new());
}

CnxString cnx_format_bool_with_allocator(const CnxFormat* restrict self,
										 [[maybe_unused]] CnxFormatSpecifier specifier,
										 CnxAllocator allocator) {
	cnx_assert(specifier.m_type == CNX_FORMAT_TYPE_DEFAULT,
			   "Can't format bool with custom specifier");

	let _self = *static_cast(bool*)(self->m_self);
	if(_self) {
		return cnx_string_from_with_allocator("true", allocator);
	}
	else {
		return cnx_string_from_with_allocator("false", allocator);
	}
}

CnxString cnx_format_char(const CnxFormat* restrict self, CnxFormatSpecifier specifier) {
	return cnx_format_char_with_allocator(self, specifier, cnx_allocator_new());
}

CnxString cnx_format_char_with_allocator(const CnxFormat* restrict self,
										 CnxFormatSpecifier specifier,
										 CnxAllocator allocator) {
	cnx_assert(specifier.m_type != CNX_FORMAT_TYPE_SCIENTIFIC,
			   "Cannot format char as scientific notation");

	if(specifier.m_type == CNX_FORMAT_TYPE_DEFAULT || specifier.m_type == CNX_FORMAT_TYPE_DEBUG) {
		return cnx_string_from_cstring_with_allocator(static_cast(const_cstring)(self->m_self),
													  1,
													  allocator);
	}
	else if(specifier.m_type == CNX_FORMAT_TYPE_DECIMAL) {
		let num = *static_cast(const char*)(self->m_self);
		return cnx_format_decimal(static_cast(u64)(num), NUM_DIGITS_CHAR, allocator);
	}
	else {
		let num = static_cast(u8)(*(static_cast(const char*)(self->m_self)));
		return cnx_format_hex(num, NUM_HEX_DIGITS_CHAR, specifier.m_type, allocator);
	}
}

CnxString cnx_format_u8(const CnxFormat* restrict self, CnxFormatSpecifier specifier) {
	return cnx_format_u8_with_allocator(self, specifier, cnx_allocator_new());
}

CnxString cnx_format_u8_with_allocator(const CnxFormat* restrict self,
									   CnxFormatSpecifier specifier,
									   CnxAllocator allocator) {
	cnx_assert(specifier.m_type != CNX_FORMAT_TYPE_SCIENTIFIC,
			   "Cannot format u8 as scientific notation");

	let num = *static_cast(const u8*)(self->m_self);
	return (specifier.m_type == CNX_FORMAT_TYPE_DEFAULT
			|| specifier.m_type == CNX_FORMAT_TYPE_DECIMAL
			|| specifier.m_type == CNX_FORMAT_TYPE_DEBUG) ?
			   cnx_format_decimal(num, NUM_DIGITS_U8, allocator) :
			   cnx_format_hex(num, NUM_HEX_DIGITS_U8, specifier.m_type, allocator);
}

CnxString cnx_format_u16(const CnxFormat* restrict self, CnxFormatSpecifier specifier) {
	return cnx_format_u16_with_allocator(self, specifier, cnx_allocator_new());
}

CnxString cnx_format_u16_with_allocator(const CnxFormat* restrict self,
										CnxFormatSpecifier specifier,
										CnxAllocator allocator) {
	cnx_assert(specifier.m_type != CNX_FORMAT_TYPE_SCIENTIFIC,
			   "Cannot format u16 as scientific notation");

	let num = *static_cast(const u16*)(self->m_self);
	return (specifier.m_type == CNX_FORMAT_TYPE_DEFAULT
			|| specifier.m_type == CNX_FORMAT_TYPE_DECIMAL
			|| specifier.m_type == CNX_FORMAT_TYPE_DEBUG) ?
			   cnx_format_decimal(num, NUM_DIGITS_U16, allocator) :
			   cnx_format_hex(num, NUM_HEX_DIGITS_U16, specifier.m_type, allocator);
}

CnxString cnx_format_u32(const CnxFormat* restrict self, CnxFormatSpecifier specifier) {
	return cnx_format_u32_with_allocator(self, specifier, cnx_allocator_new());
}

CnxString cnx_format_u32_with_allocator(const CnxFormat* restrict self,
										CnxFormatSpecifier specifier,
										CnxAllocator allocator) {
	cnx_assert(specifier.m_type != CNX_FORMAT_TYPE_SCIENTIFIC,
			   "Cannot format u32 as scientific notation");

	let num = *static_cast(const u32*)(self->m_self);
	return (specifier.m_type == CNX_FORMAT_TYPE_DEFAULT
			|| specifier.m_type == CNX_FORMAT_TYPE_DECIMAL
			|| specifier.m_type == CNX_FORMAT_TYPE_DEBUG) ?
			   cnx_format_decimal(num, NUM_DIGITS_U32, allocator) :
			   cnx_format_hex(num, NUM_HEX_DIGITS_U32, specifier.m_type, allocator);
}

CnxString cnx_format_u64(const CnxFormat* restrict self, CnxFormatSpecifier specifier) {
	return cnx_format_u64_with_allocator(self, specifier, cnx_allocator_new());
}

CnxString cnx_format_u64_with_allocator(const CnxFormat* restrict self,
										CnxFormatSpecifier specifier,
										CnxAllocator allocator) {
	cnx_assert(specifier.m_type != CNX_FORMAT_TYPE_SCIENTIFIC,
			   "Cannot format u64 as scientific notation");

	let num = *static_cast(const u64*)(self->m_self);
	return (specifier.m_type == CNX_FORMAT_TYPE_DEFAULT
			|| specifier.m_type == CNX_FORMAT_TYPE_DECIMAL
			|| specifier.m_type == CNX_FORMAT_TYPE_DEBUG) ?
			   cnx_format_decimal(num, NUM_DIGITS_U64, allocator) :
			   cnx_format_hex(num, NUM_HEX_DIGITS_U64, specifier.m_type, allocator);
}

CnxString cnx_format_i8(const CnxFormat* restrict self, CnxFormatSpecifier specifier) {
	return cnx_format_i8_with_allocator(self, specifier, cnx_allocator_new());
}

CnxString cnx_format_i8_with_allocator(const CnxFormat* restrict self,
									   CnxFormatSpecifier specifier,
									   CnxAllocator allocator) {
	cnx_assert(specifier.m_type != CNX_FORMAT_TYPE_SCIENTIFIC,
			   "Cannot format i8 as scientific notation");

	let num = *static_cast(const i8*)(self->m_self);
	return (specifier.m_type == CNX_FORMAT_TYPE_DEFAULT
			|| specifier.m_type == CNX_FORMAT_TYPE_DECIMAL
			|| specifier.m_type == CNX_FORMAT_TYPE_DEBUG) ?
			   cnx_format_decimal(num, NUM_DIGITS_I8, allocator) :
			   cnx_format_hex(static_cast(u64)(num),
							  NUM_HEX_DIGITS_I8,
							  specifier.m_type,
							  allocator);
}

CnxString cnx_format_i16(const CnxFormat* restrict self, CnxFormatSpecifier specifier) {
	return cnx_format_i16_with_allocator(self, specifier, cnx_allocator_new());
}

CnxString cnx_format_i16_with_allocator(const CnxFormat* restrict self,
										CnxFormatSpecifier specifier,
										CnxAllocator allocator) {
	cnx_assert(specifier.m_type != CNX_FORMAT_TYPE_SCIENTIFIC,
			   "Cannot format i16 as scientific notation");

	let num = *static_cast(const i16*)(self->m_self);
	return (specifier.m_type == CNX_FORMAT_TYPE_DEFAULT
			|| specifier.m_type == CNX_FORMAT_TYPE_DECIMAL
			|| specifier.m_type == CNX_FORMAT_TYPE_DEBUG) ?
			   cnx_format_decimal(num, NUM_DIGITS_I16, allocator) :
			   cnx_format_hex(static_cast(u64)(num),
							  NUM_HEX_DIGITS_I16,
							  specifier.m_type,
							  allocator);
}

CnxString cnx_format_i32(const CnxFormat* restrict self, CnxFormatSpecifier specifier) {
	return cnx_format_i32_with_allocator(self, specifier, cnx_allocator_new());
}

CnxString cnx_format_i32_with_allocator(const CnxFormat* restrict self,
										CnxFormatSpecifier specifier,
										CnxAllocator allocator) {
	cnx_assert(specifier.m_type != CNX_FORMAT_TYPE_SCIENTIFIC,
			   "Cannot format i32 as scientific notation");

	let num = *static_cast(const i32*)(self->m_self);
	return (specifier.m_type == CNX_FORMAT_TYPE_DEFAULT
			|| specifier.m_type == CNX_FORMAT_TYPE_DECIMAL
			|| specifier.m_type == CNX_FORMAT_TYPE_DEBUG) ?
			   cnx_format_decimal(num, NUM_DIGITS_I32, allocator) :
			   cnx_format_hex(static_cast(u64)(num),
							  NUM_HEX_DIGITS_I32,
							  specifier.m_type,
							  allocator);
}

CnxString cnx_format_i64(const CnxFormat* restrict self, CnxFormatSpecifier specifier) {
	return cnx_format_i64_with_allocator(self, specifier, cnx_allocator_new());
}

CnxString cnx_format_i64_with_allocator(const CnxFormat* restrict self,
										CnxFormatSpecifier specifier,
										CnxAllocator allocator) {
	cnx_assert(specifier.m_type != CNX_FORMAT_TYPE_SCIENTIFIC,
			   "Cannot format i64 as scientific notation");

	let num = *static_cast(const i64*)(self->m_self);
	return (specifier.m_type == CNX_FORMAT_TYPE_DEFAULT
			|| specifier.m_type == CNX_FORMAT_TYPE_DECIMAL
			|| specifier.m_type == CNX_FORMAT_TYPE_DEBUG) ?
			   cnx_format_decimal(num, NUM_DIGITS_I64, allocator) :
			   cnx_format_hex(static_cast(u64)(num),
							  NUM_HEX_DIGITS_I64,
							  specifier.m_type,
							  allocator);
}

// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
[[nodiscard]] CnxString cnx_format_f64_scientific(f64 shifted_to_scientific,
												  i64 exponent,
												  usize num_sig_figs,
												  CnxAllocator allocator) {
	// possible -, first digit, point, Exxx, num sig figs
	let num_chars_in_float = 1 + 1 + 1 + 4 + num_sig_figs;
	let_mut string = cnx_string_new_with_capacity_with_allocator(num_chars_in_float, allocator);

	if(shifted_to_scientific < 0) {
		cnx_string_push_back(string, '-');
	}

	cnx_string_push_back(string,
						 cnx_num_to_char(cnx_get_digit_before_decimal(shifted_to_scientific, 0)));
	cnx_string_push_back(string, '.');
	ranged_for(i, 0U, num_sig_figs) {
		cnx_string_push_back(
			string,
			cnx_num_to_char(cnx_get_digit_after_decimal(shifted_to_scientific, i, num_sig_figs)));
	}
	cnx_string_push_back(string, 'E');
	let_mut gotten_non_zero = false;
	ranged_for(i, 0U, 2) {
		let digit = cnx_get_digit(exponent, 1U - i);
		if(gotten_non_zero || digit != 0.0) {
			cnx_string_push_back(string, cnx_num_to_char(digit));
			gotten_non_zero = true;
		}
	}
	return gotten_non_zero ? string : ({
		cnx_string_push_back(string, '0');
		string;
	});
}

[[nodiscard]] CnxString
// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
cnx_format_f64_decimal(f64 num, i64 exponent, usize num_sig_figs, CnxAllocator allocator) {
	let num_chars_before_decimal = cnx_get_num_digits_before_decimal(num, exponent);
	// possible -, num digits before point, point, num sig figs after point
	let num_chars_in_float = 1 + num_chars_before_decimal + 1 + num_sig_figs;
	let_mut string = cnx_string_new_with_capacity_with_allocator(num_chars_in_float, allocator);

	if(num < 0) {
		cnx_string_push_back(string, '-');
	}

	let_mut gotten_non_zero = false;
	ranged_for(i, 0U, num_chars_before_decimal + 1) {
		let digit = cnx_get_digit_before_decimal(num, num_chars_before_decimal - i);
		if(gotten_non_zero || digit != 0.0) {
			cnx_string_push_back(string, cnx_num_to_char(digit));
			gotten_non_zero = true;
		}
	}

	if(!gotten_non_zero) {
		cnx_string_push_back(string, '0');
	}

	cnx_string_push_back(string, '.');
	ranged_for(i, 0U, num_sig_figs) {
		cnx_string_push_back(string,
							 cnx_num_to_char(cnx_get_digit_after_decimal(num, i, num_sig_figs)));
	}

	return string;
}

[[nodiscard]] [[always_inline]] static inline CnxString
cnx_format_f64_with_allocator__(f64 num, CnxFormatSpecifier specifier, CnxAllocator allocator) {
	let_mut exponent_base_2 = 0;
	ignore(frexp(num, &exponent_base_2));
	let exponent = static_cast(i64)(static_cast(f32)(exponent_base_2) * LN_2 / LN_10);
	return (specifier.m_type == CNX_FORMAT_TYPE_DEFAULT
			|| specifier.m_type == CNX_FORMAT_TYPE_SCIENTIFIC
			|| specifier.m_type == CNX_FORMAT_TYPE_DEBUG) ?
			   cnx_format_f64_scientific(num / powers_of_10[exponent],
										 exponent,
										 specifier.m_num_sig_figs,
										 allocator) :
			   cnx_format_f64_decimal(num, exponent, specifier.m_num_sig_figs, allocator);
}

CnxString cnx_format_f32(const CnxFormat* restrict self, CnxFormatSpecifier specifier) {
	return cnx_format_f32_with_allocator(self, specifier, cnx_allocator_new());
}

CnxString cnx_format_f32_with_allocator(const CnxFormat* restrict self,
										CnxFormatSpecifier specifier,
										CnxAllocator allocator) {
	cnx_assert(specifier.m_type != CNX_FORMAT_TYPE_HEX_LOWER
				   && specifier.m_type != CNX_FORMAT_TYPE_HEX_UPPER,
			   "Cannot format f32 as hex");

	let num = *static_cast(const f32*)(self->m_self);
	return cnx_format_f64_with_allocator__(num, specifier, allocator);
}

CnxString cnx_format_f64(const CnxFormat* restrict self, CnxFormatSpecifier specifier) {
	return cnx_format_f64_with_allocator(self, specifier, cnx_allocator_new());
}

CnxString cnx_format_f64_with_allocator(const CnxFormat* restrict self,
										CnxFormatSpecifier specifier,
										CnxAllocator allocator) {
	cnx_assert(specifier.m_type != CNX_FORMAT_TYPE_HEX_LOWER
				   && specifier.m_type != CNX_FORMAT_TYPE_HEX_UPPER,
			   "Cannot format f64 as hex");

	let num = *static_cast(const f64*)(self->m_self);
	return cnx_format_f64_with_allocator__(num, specifier, allocator);
}

CnxString cnx_format_ptr(const CnxFormat* restrict self, CnxFormatSpecifier specifier) {
	return cnx_format_ptr_with_allocator(self, specifier, cnx_allocator_new());
}

CnxString cnx_format_ptr_with_allocator(const CnxFormat* restrict self,
										CnxFormatSpecifier specifier,
										CnxAllocator allocator) {
	cnx_assert(specifier.m_type != CNX_FORMAT_TYPE_DECIMAL
				   && specifier.m_type != CNX_FORMAT_TYPE_SCIENTIFIC,
			   "Cannot format a pointer as decimal number or scientific notation");

	let num = static_cast(usize)(static_cast(void*)(self->m_self));
	let_mut string = cnx_string_new_with_capacity_with_allocator(2 + NUM_HEX_DIGITS_PTR, allocator);

	if(specifier.m_type == CNX_FORMAT_TYPE_DEFAULT || specifier.m_type == CNX_FORMAT_TYPE_HEX_LOWER
	   || specifier.m_type == CNX_FORMAT_TYPE_DEBUG)
	{
		cnx_string_append(string, "0x");
		ranged_for(i, 0U, NUM_HEX_DIGITS_PTR) {
			cnx_string_push_back(
				string,
				cnx_num_to_hex_lower(cnx_get_hex(num, (NUM_HEX_DIGITS_PTR - 1) - i)));
		}
	}
	else {
		cnx_string_append(string, "0X");
		ranged_for(i, 0U, NUM_HEX_DIGITS_PTR) {
			cnx_string_push_back(
				string,
				cnx_num_to_hex_upper(cnx_get_hex(num, (NUM_HEX_DIGITS_PTR - 1) - i)));
		}
	}
	return string;
}

CnxString cnx_format_cnx_string(const CnxFormat* restrict self, CnxFormatSpecifier specifier) {
	ignore(specifier);

	let string = static_cast(const CnxString*)(self->m_self);
	return cnx_string_clone(*string);
}

CnxString cnx_format_cnx_string_with_allocator(const CnxFormat* restrict self,
											   CnxFormatSpecifier specifier,
											   CnxAllocator allocator) {
	ignore(specifier);

	let string = static_cast(const CnxString*)(self->m_self);
	return cnx_string_clone_with_allocator(*string, allocator);
}

CnxString cnx_format_cnx_stringview(const CnxFormat* restrict self, CnxFormatSpecifier specifier) {
	ignore(specifier);

	let view = static_cast(const CnxStringView*)(self->m_self);
	return cnx_string_from(view);
}

CnxString cnx_format_cnx_stringview_with_allocator(const CnxFormat* restrict self,
												   CnxFormatSpecifier specifier,
												   CnxAllocator allocator) {
	ignore(specifier);

	let view = static_cast(const CnxStringView*)(self->m_self);
	return cnx_string_from_with_allocator(view, allocator);
}

CnxString cnx_format_cstring(const CnxFormat* restrict self, CnxFormatSpecifier specifier) {
	ignore(specifier);

	let string = static_cast(const_cstring)(self->m_self);
	return cnx_string_from(string);
}

CnxString cnx_format_cstring_with_allocator(const CnxFormat* restrict self,
											CnxFormatSpecifier specifier,
											CnxAllocator allocator) {
	ignore(specifier);

	let string = static_cast(const_cstring)(self->m_self);
	return cnx_string_from_with_allocator(string, allocator);
}

typedef enum CnxFormatErrorTypes {
	CNX_FORMAT_SUCCESS = 0,
	CNX_FORMAT_BAD_SPECIFIER_INVALID_CHAR_IN_SPECIFIER,
	CNX_FORMAT_INVALID_CLOSING_BRACE_LOCATION,
	CNX_FORMAT_UNCLOSED_SPECIFIER,
	CNX_FORMAT_MORE_SPECIFIERS_THAN_ARGS,
	CNX_FORMAT_FEWER_SPECIFIERS_THAN_ARGS
} CnxFormatErrorTypes;

Enum(CnxFormatVariant, (Substring, CnxStringView), (Specifier, CnxFormatSpecifier));

typedef CnxFormatVariant* Ref(CnxFormatVariant);
typedef const CnxFormatVariant* ConstRef(CnxFormatVariant);
DeclCnxIterators(Ref(CnxFormatVariant));
DeclCnxIterators(ConstRef(CnxFormatVariant));

#define OPTION_DECL TRUE
#define OPTION_IMPL TRUE

#define OPTION_T CnxFormatVariant
#include <Cnx/Option.h>
#undef OPTION_T

#undef OPTION_IMPL
#undef OPTION_DECL

#define VECTOR_DECL TRUE
#define VECTOR_IMPL TRUE

#define VECTOR_T CnxFormatVariant
#include <Cnx/Vector.h>
#undef VECTOR_T

#undef VECTOR_IMPL
#undef VECTOR_DECL

#define RESULT_DECL TRUE
#define RESULT_IMPL TRUE

#define RESULT_T CnxVector(CnxFormatVariant)
#include <Cnx/Result.h>

#undef RESULT_T
#undef RESULT_IMPL
#undef RESULT_DECL

#define Substring(string_view)                                             \
	(CnxFormatVariant) {                                                   \
		.tag = Substring, .variant_identifier(Substring) = { string_view } \
	}
#define Specifier(specifier)                                             \
	(CnxFormatVariant) {                                                 \
		.tag = Specifier, .variant_identifier(Specifier) = { specifier } \
	}

[[nodiscard]] [[always_inline]] static inline CnxFormatVariant
cnx_format_variant_new([[maybe_unused]] CnxAllocator allocator) {
	return (CnxFormatVariant){0};
}

[[not_null(1)]] [[always_inline]] static inline void
cnx_format_variant_free([[maybe_unused]] CnxFormatVariant* elem,
						[[maybe_unused]] CnxAllocator allocator) {
}

[[nodiscard]] [[not_null(1)]] [[always_inline]] static inline CnxFormatVariant
cnx_format_variant_clone(const CnxFormatVariant* restrict elem,
						 [[maybe_unused]] CnxAllocator allocator) {
	return *elem;
}

// clang-format off
#define cnx_format_pair_from(x) _Generic(&(x), \
		CnxFormatSpecifier* 			: 	cnx_format_pair_from_specifier, 	\
		const CnxFormatSpecifier* 		: 	cnx_format_pair_from_specifier, 	\
		CnxStringView* 					: 	cnx_format_pair_from_stringview, 	\
		const CnxStringView* 			: cnx_format_pair_from_stringview)(x)
//clang-format on

[[nodiscard]] [[returns_not_null]] const_cstring cnx_format_category_get_message(
		[[maybe_unused]] const CnxErrorCategory* restrict self, i64 error_code) {
	if(error_code == CNX_FORMAT_SUCCESS) {
		return "No error: Formatting successful";
	}
	else if(error_code == CNX_FORMAT_BAD_SPECIFIER_INVALID_CHAR_IN_SPECIFIER) {
		return "Error: Bad format specifier provided to cnx_format, invalid char in specifier";
	}
	else if(error_code == CNX_FORMAT_INVALID_CLOSING_BRACE_LOCATION) {
		return "Error: Invalid specifier closing brace location in format string";
	}
	else if(error_code == CNX_FORMAT_UNCLOSED_SPECIFIER) {
		return "Error: Unclosed specifier in format string";
	}
	else if(error_code == CNX_FORMAT_MORE_SPECIFIERS_THAN_ARGS) {
		return "Error: More format specifiers provided than arguments to function";
	}
	else if(error_code == CNX_FORMAT_FEWER_SPECIFIERS_THAN_ARGS) {
		return "Error: Fewer format specifiers provided than arguments to function";
	}
	unreachable();
}

[[nodiscard]] i64 cnx_format_category_get_last_error(
		[[maybe_unused]] const CnxErrorCategory* restrict self) {
	return 0;
}

typedef struct CnxFormatErrorCategory {} CnxFormatErrorCategory;

[[maybe_unused]] static ImplTraitFor(CnxErrorCategory,
									 CnxFormatErrorCategory,
									 cnx_format_category_get_message,
									 cnx_format_category_get_last_error);

[[maybe_unused]] static const CnxFormatErrorCategory cnx_format_error_category = {};

[[maybe_unused]] static let cnx_format_category = as_trait(CnxErrorCategory,
														   CnxFormatErrorCategory,
														   cnx_format_error_category);

[[nodiscard]] [[not_null(1)]] [[always_inline]]
static inline usize cnx_format_get_num_sig_figs_from_substring(
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
		return CNX_FORMAT_DEFAULT_NUM_SIG_FIGS;
	}

	ranged_for(i, first, last + 1) {
		num_figs += static_cast(u32)(multiplier * (format_string[i] - '0'));
		multiplier /= 10; // NOLINT
	}

	return num_figs;
}

[[nodiscard]] bool cnx_format_is_char_valid_in_specifier(char character) {
	return character == CNX_FORMAT_TYPE_DECIMAL || character == CNX_FORMAT_TYPE_HEX_LOWER
		   || character == CNX_FORMAT_TYPE_HEX_UPPER || character == CNX_FORMAT_TYPE_SCIENTIFIC
		   || character == CNX_FORMAT_TYPE_DEBUG || (character >= '0' && character <= '9');
}


static let format_variant_data = (CnxCollectionData(CnxVector(CnxFormatVariant))) {
.m_constructor = cnx_format_variant_new,
.m_destructor = cnx_format_variant_free,
.m_copy_constructor = cnx_format_variant_clone};

[[nodiscard]] [[not_null(1)]] CnxResult(CnxVector(CnxFormatVariant))
	cnx_format_parse_and_validate_format_string(restrict const_cstring format_string, // NOLINT
											// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
											usize length,
											[[maybe_unused]] usize num_args,
											CnxAllocator allocator)
{
	let_mut vec = cnx_vector_new_with_capacity_allocator_and_collection_data(CnxFormatVariant,
			num_args * 2, allocator, &format_variant_data);

	let_mut in_specifier = false;
	let_mut pushed_specifier = false;
	let_mut num_open = 0U;
	let_mut num_close = 0U;
	let_mut start_index = 0U;
	ranged_for(i, 0U, length) {
		if(format_string[i] == '{') {
#if CNX_PLATFORM_DEBUG
			if(in_specifier) {
				return Err(CnxVector(CnxFormatVariant),
						   cnx_error_new(CNX_FORMAT_BAD_SPECIFIER_INVALID_CHAR_IN_SPECIFIER,
										 cnx_format_category));
			}
			else
#endif // CNX_PLATFORM_DEBUG
			if(i > 0) {
				if(format_string[i - 1] != '\\') {
					in_specifier = true;
					pushed_specifier = false;
					num_open++;
					let view = cnx_stringview_from(format_string, start_index, i - start_index);
					cnx_vector_push_back(vec,
									 Substring(view));
				}
			}
			else {
				in_specifier = true;
				num_open++;
			}
		}
		else if(format_string[i] == '}') {
#if CNX_PLATFORM_DEBUG
			if(!in_specifier) {
				return Err(CnxVector(CnxFormatVariant),
						   cnx_error_new(CNX_FORMAT_INVALID_CLOSING_BRACE_LOCATION,
										 cnx_format_category));
			}
			else if(format_string[i - 1] == '\\') {
				return Err(CnxVector(CnxFormatVariant),
						   cnx_error_new(CNX_FORMAT_BAD_SPECIFIER_INVALID_CHAR_IN_SPECIFIER,
										 cnx_format_category));
			}
			else {
				if(!pushed_specifier) {
					cnx_vector_push_back(vec,
										 Specifier(((CnxFormatSpecifier){
											 .m_type = CNX_FORMAT_TYPE_DEFAULT,
											 .m_num_sig_figs = CNX_FORMAT_DEFAULT_NUM_SIG_FIGS})));
				}
				in_specifier = false;
				num_close++;
				start_index = i + 1;
			}
#else  // CNX_PLATFORM_DEBUG
			if(!pushed_specifier) {
				cnx_vector_push_back(vec,
									 Specifier(((CnxFormatSpecifier){
										 .m_type = CNX_FORMAT_TYPE_DEFAULT,
										 .m_num_sig_figs = CNX_FORMAT_DEFAULT_NUM_SIG_FIGS})));
			}
			in_specifier = false;
			num_close++;
			start_index = i + 1;
#endif // CNX_PLATFORM_DEBUG
		}
		else if(in_specifier) {
#if CNX_PLATFORM_DEBUG
			if(!cnx_format_is_char_valid_in_specifier(format_string[i])) {
				return Err(CnxVector(CnxFormatVariant),
						   cnx_error_new(CNX_FORMAT_BAD_SPECIFIER_INVALID_CHAR_IN_SPECIFIER,
										 cnx_format_category));
			}
#endif // CNX_PLATFORM_DEBUG

			if(format_string[i] == CNX_FORMAT_TYPE_DECIMAL) {
				cnx_vector_push_back(
					vec,
					Specifier(((CnxFormatSpecifier){
						.m_type = CNX_FORMAT_TYPE_DECIMAL,
						.m_num_sig_figs = cnx_format_get_num_sig_figs_from_substring(format_string,
																					 length,
																					 i + 1)})));
				pushed_specifier = true;
			}
			else if(format_string[i] == CNX_FORMAT_TYPE_HEX_LOWER) {
				cnx_vector_push_back(
					vec,
					Specifier(((CnxFormatSpecifier){.m_type = CNX_FORMAT_TYPE_HEX_LOWER,
															   .m_num_sig_figs = 0})));
				pushed_specifier = true;
			}
			else if(format_string[i] == CNX_FORMAT_TYPE_HEX_UPPER) {
				cnx_vector_push_back(
					vec,
					Specifier(((CnxFormatSpecifier){.m_type = CNX_FORMAT_TYPE_HEX_UPPER,
															   .m_num_sig_figs = 0})));
				pushed_specifier = true;
			}
			else if(format_string[i] == CNX_FORMAT_TYPE_SCIENTIFIC) {
				cnx_vector_push_back(
					vec,
					Specifier(((CnxFormatSpecifier){
						.m_type = CNX_FORMAT_TYPE_SCIENTIFIC,
						.m_num_sig_figs = cnx_format_get_num_sig_figs_from_substring(format_string,
																					 length,
																					 i + 1)})));
				pushed_specifier = true;
			}
			else if(format_string[i] == CNX_FORMAT_TYPE_DEBUG) {
				cnx_vector_push_back(
					vec,
					Specifier(((CnxFormatSpecifier){
						.m_type = CNX_FORMAT_TYPE_DEBUG,
						.m_num_sig_figs = cnx_format_get_num_sig_figs_from_substring(format_string,
																					 length,
																					 i + 1)})));
				pushed_specifier = true;
			}
		}
	}

	if(start_index < length) {
		let view = cnx_stringview_from(format_string, start_index, length - start_index);
		cnx_vector_push_back(vec, Substring(view));
	}

#if CNX_PLATFORM_DEBUG
	if(num_open != num_close) {
		return Err(CnxVector(CnxFormatVariant),
				   cnx_error_new(CNX_FORMAT_UNCLOSED_SPECIFIER, cnx_format_category));
	}
	else if(num_open > num_args) {
		return Err(CnxVector(CnxFormatVariant),
				   cnx_error_new(CNX_FORMAT_MORE_SPECIFIERS_THAN_ARGS, cnx_format_category));
	}
	// we can't reliably check for 0 args, so make an exception for that
	else if(num_open < num_args && num_args != 1) {
		return Err(CnxVector(CnxFormatVariant),
				   cnx_error_new(CNX_FORMAT_FEWER_SPECIFIERS_THAN_ARGS, cnx_format_category));
	}
#endif // CNX_PLATFORM_DEBUG

	return (cnx_vector_size(vec) == 0) ? ({
		let view = cnx_stringview_from(format_string, 0, length);
		cnx_vector_push_back(vec, Substring(view));
		Ok(CnxVector(CnxFormatVariant), vec);
	}) :
										   Ok(CnxVector(CnxFormatVariant), vec);
}

CnxString(cnx_format_with_allocator)(restrict const_cstring format_string,
									 CnxAllocator allocator,
									 usize num_args,
									 ...) {
	va_list list = {0};
	va_start(list, num_args);
	let string = cnx_vformat_with_allocator(format_string, allocator, num_args, list);
	va_end(list);
	return string;
}

// NOLINTNEXTLINE(readability-function-cognitive-complexity)
CnxString(cnx_vformat_with_allocator)(restrict const_cstring format_string,
									  CnxAllocator allocator,
									  usize num_args,
									  va_list list) { // NOLINT
	let string_length = strlen(format_string);
	let_mut maybe_format_variants = cnx_format_parse_and_validate_format_string(format_string,
																				string_length,
																				num_args,
																				allocator);
#if CNX_PLATFORM_DEBUG
	if(cnx_result_is_err(maybe_format_variants)) {
		let err = cnx_result_unwrap_err(maybe_format_variants);
		ignore(err);
		cnx_assert(false, cnx_error_message(&err));
	}
#endif // CNX_PLATFORM_DEBUG

	CnxScopedVector(CnxFormatVariant) format_variants
		= cnx_result_expect(maybe_format_variants, "Invalid format string");
	let_mut initial_size = static_cast(usize)(0);
	foreach_ref(elem, format_variants) {
		match(*elem) {
			variant(Substring, view) {
				initial_size += cnx_stringview_length(view);
			}
			wildcard() {
				// NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
				initial_size += 10U;
			}
		}
	}

	let_mut string = cnx_string_new_with_capacity_with_allocator(initial_size, allocator);

	foreach(elem, format_variants) {
		match(elem) {
			variant(Substring, view) {
				cnx_string_append(string, &view);
			}
			variant(Specifier, specifier) {
				let format = va_arg(list, CnxFormat); // NOLINT(clang-analyzer-valist.Uninitialized)
				CnxScopedString formatted
						   = trait_call(format_with_allocator, format, specifier, allocator);
				cnx_string_append(string, &formatted);
			}
		}
	}

	return string;
}
