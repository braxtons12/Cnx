/// @file Format.c
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief CnxFormat brings human readable string formatting, similar to C++'s `std::format` and
/// `fmtlib`, and Rust's std::format, to C.
/// @version 0.3.0
/// @date 2022-12-09
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

__attr(nodiscard) __attr(always_inline) static inline u8 cnx_get_digit_u64(u64 num, usize digit) {
	return (digit != 0 ? (num / static_cast(u64)(powers_of_10[digit])) % 10 : // NOLINT
						 num % 10);													  // NOLINT
}

__attr(nodiscard) __attr(always_inline) static inline u8 cnx_get_digit_i64(i64 num, usize digit) {
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

__attr(nodiscard) __attr(always_inline) static inline char cnx_num_to_char(u8 num) {
	return static_cast(char)(num) + '0';
}

__attr(nodiscard) __attr(always_inline) static inline u8 cnx_get_hex(usize num, usize digit) {
	return (num >> (digit * 4U)) & 0xFU; // NOLINT
}

__attr(nodiscard) __attr(always_inline) static inline char cnx_num_to_hex_lower(u8 num) {
	switch(num) {
		case 0 ... 9: return static_cast(char)(num) + '0';		  // NOLINT
		case 10 ... 15: return 'a' + static_cast(char)(num) - 10; // NOLINT
		default: unreachable();
	}
}

__attr(nodiscard) __attr(always_inline) static inline char cnx_num_to_hex_upper(u8 num) {
	switch(num) {
		case 0 ... 9: return static_cast(char)(num) + '0';			// NOLINT
		case 10 ... 15: return 'A' + (static_cast(char)(num) - 10); // NOLINT
		default: unreachable();
	}
}

__attr(nodiscard) __attr(always_inline) static inline u8
	// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
	cnx_get_digit_before_decimal(f64 num, usize digit) {
	let num_ = static_cast(i64)(num);
	return cnx_get_digit(num_, digit);
}

__attr(nodiscard) __attr(always_inline) static inline u8
	// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
	cnx_get_digit_after_decimal(f64 num, usize digit, usize num_sig_figs) {
	let num_ = static_cast(isize)(num * powers_of_10[num_sig_figs]);
	return cnx_get_digit(num_, (num_sig_figs - 1) - digit);
}

__attr(nodiscard) __attr(always_inline) static inline usize
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

__attr(nodiscard) __attr(always_inline) static inline CnxString
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

__attr(nodiscard) __attr(always_inline) static inline CnxString
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

typedef enum CnxFormatIntegralNotation {
	CNX_FORMAT_UNSIGNED_NOTATION_DEFAULT,
	CNX_FORMAT_UNSIGNED_NOTATION_DECIMAL,
	CNX_FORMAT_UNSIGNED_NOTATION_LOWER_HEX,
	CNX_FORMAT_UNSIGNED_NOTATION_UPPER_HEX
} CnxFormatIntegralNotation;

typedef struct IntegralContext {
	CnxFormatIntegralNotation notation;
	bool is_debug;
} IntegralContext;

__attr(always_inline) __attr(nodiscard) CnxFormatContext
	cnx_format_is_specifier_valid_integral(CnxStringView specifier) {
	let_mut context = (CnxFormatContext){.is_valid = CNX_FORMAT_SUCCESS};
	let length = cnx_stringview_length(specifier);
	if(length == 0) {
		return context;
	}

	let_mut state
		= (IntegralContext){.notation = CNX_FORMAT_UNSIGNED_NOTATION_DEFAULT, .is_debug = false};
	if(length > 2) {
		context.is_valid = CNX_FORMAT_BAD_SPECIFIER_INVALID_CHAR_IN_SPECIFIER;
		return context;
	}

	let first = cnx_stringview_at(specifier, 0);
	if(first != 'x' && first != 'X' && first != 'd' && first != 'D') {
		context.is_valid = CNX_FORMAT_BAD_SPECIFIER_INVALID_CHAR_IN_SPECIFIER;
		return context;
	}

	if(first == 'x') {
		state.notation = CNX_FORMAT_UNSIGNED_NOTATION_LOWER_HEX;
	}
	else if(first == 'X') {
		state.notation = CNX_FORMAT_UNSIGNED_NOTATION_UPPER_HEX;
	}
	else if(first == 'd') {
		state.notation = CNX_FORMAT_UNSIGNED_NOTATION_DECIMAL;
	}
	else if(first == 'D') {
		state.is_debug = true;
	}

	if(length == 2) {
		let second = cnx_stringview_at(specifier, 1);
		if(second != 'D') {
			context.is_valid = CNX_FORMAT_BAD_SPECIFIER_INVALID_CHAR_IN_SPECIFIER;
			return context;
		}

		state.is_debug = -true;
	}

	*(static_cast(IntegralContext*)(context.state)) = state;
	return context;
}

__attr(nodiscard) CnxString
	// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
	cnx_format_hex(u64 num,
				   // NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
				   usize num_digits,
				   CnxFormatIntegralNotation notation,
				   CnxAllocator allocator) {
	let_mut string = cnx_string_new_with_capacity_with_allocator(num_digits + 2, allocator);
	let_mut gotten_non_zero = false;

	cnx_string_push_back(string, '0');
	if(notation == CNX_FORMAT_UNSIGNED_NOTATION_LOWER_HEX) {
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
typedef enum CnxFormatFloatNotation {
	CNX_FORMAT_FLOAT_NOTATION_DEFAULT,
	CNX_FORMAT_FLOAT_NOTATION_DECIMAL,
	CNX_FORMAT_FLOAT_NOTATION_SCIENTIFIC
} CnxFormatFloatNotation;

typedef struct FloatContext {
	usize num_sig_figs;
	CnxFormatFloatNotation notation;
	bool is_debug;
} FloatContext;

cnx_static_assert(
	sizeof(FloatContext) < 32,
	"Context types can't be larger than the 32-byte state buffer in CnxFormatContext.state");

__attr(nodiscard) __attr(always_inline) static inline usize
	// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
	cnx_format_get_num_from_char(char character, usize index, usize current) {
	let multiplier = static_cast(usize)(1 + index);
	if(character >= '0' && character <= '9') {
		return multiplier * current + static_cast(usize)(character - '0');
	}

	return current;
}

__attr(always_inline) __attr(nodiscard) CnxFormatContext
	cnx_format_is_specifier_valid_fp(CnxStringView specifier) {
	let_mut context = (CnxFormatContext){.is_valid = CNX_FORMAT_SUCCESS};
	let_mut state = (FloatContext){.is_debug = false,
								   .notation = CNX_FORMAT_FLOAT_NOTATION_DEFAULT,
								   .num_sig_figs = CNX_FORMAT_DEFAULT_NUM_SIG_FIGS};
	*(static_cast(FloatContext*)(context.state)) = state;
	let length = cnx_stringview_length(specifier);
	if(length == 0) {
		return context;
	}

	let_mut notation = false;
	let_mut sigfigs = false;
	let_mut debug = false;
	let_mut sig_fig_index = static_cast(usize)(0);
	foreach(character, specifier) {
		if(character == 'e' || character == 'd') {
			// notation can only occur once, before sigfig or debug specifiers
			if(notation || sigfigs || debug) {
				context.is_valid = CNX_FORMAT_BAD_SPECIFIER_INVALID_CHAR_IN_SPECIFIER;
				return context;
			}

			if(character == 'e') {
				state.notation = CNX_FORMAT_FLOAT_NOTATION_SCIENTIFIC;
			}
			else {
				state.notation = CNX_FORMAT_FLOAT_NOTATION_DECIMAL;
			}
			notation = true;
		}
		else if(character >= '0' && character <= '9') {
			// sigfigs can only occur before debug specifiers
			if(debug) {
				context.is_valid = CNX_FORMAT_BAD_SPECIFIER_INVALID_CHAR_IN_SPECIFIER;
				return context;
			}

			state.num_sig_figs
				= cnx_format_get_num_from_char(character, sig_fig_index++, state.num_sig_figs);
			sigfigs = true;
		}
		else if(character == 'D') {
			state.is_debug = true;
			debug = true;
		}
		else {
			context.is_valid = CNX_FORMAT_BAD_SPECIFIER_INVALID_CHAR_IN_SPECIFIER;
			return context;
		}
	}

	*(static_cast(FloatContext*)(context.state)) = state;
	return context;
}

typedef struct GenericContext {
	bool is_debug;
} GenericContext;

__attr(always_inline) __attr(nodiscard) CnxFormatContext
	cnx_format_is_specifier_valid_generic(CnxStringView specifier) {
	let_mut context = (CnxFormatContext){.is_valid = CNX_FORMAT_SUCCESS};
	let_mut state = (GenericContext){.is_debug = false};
	*(static_cast(GenericContext*)(context.state)) = state;

	let length = cnx_stringview_length(specifier);
	if(length > 1) {
		context.is_valid = CNX_FORMAT_BAD_SPECIFIER_INVALID_CHAR_IN_SPECIFIER;
		return context;
	}

	if(length == 1) {
		if(cnx_stringview_at(specifier, 0) != 'D') {
			context.is_valid = CNX_FORMAT_BAD_SPECIFIER_INVALID_CHAR_IN_SPECIFIER;
			return context;
		}

		state.is_debug = true;
	}

	*(static_cast(GenericContext*)(context.state)) = state;
	return context;
}

CnxFormatContext
cnx_format_is_specifier_valid_cstring(__attr(maybe_unused) const CnxFormat* restrict self,
									  CnxStringView specifier) {
	return cnx_format_is_specifier_valid_generic(specifier);
}

CnxFormatContext
cnx_format_is_specifier_valid_bool(__attr(maybe_unused) const CnxFormat* restrict self,
								   CnxStringView specifier) {
	return cnx_format_is_specifier_valid_integral(specifier);
}

CnxFormatContext
cnx_format_is_specifier_valid_char(__attr(maybe_unused) const CnxFormat* restrict self,
								   CnxStringView specifier) {
	return cnx_format_is_specifier_valid_integral(specifier);
}

CnxFormatContext
cnx_format_is_specifier_valid_u8(__attr(maybe_unused) const CnxFormat* restrict self,
								 CnxStringView specifier) {
	return cnx_format_is_specifier_valid_integral(specifier);
}

CnxFormatContext
cnx_format_is_specifier_valid_u16(__attr(maybe_unused) const CnxFormat* restrict self,
								  CnxStringView specifier) {
	return cnx_format_is_specifier_valid_integral(specifier);
}

CnxFormatContext
cnx_format_is_specifier_valid_u32(__attr(maybe_unused) const CnxFormat* restrict self,
								  CnxStringView specifier) {
	return cnx_format_is_specifier_valid_integral(specifier);
}

CnxFormatContext
cnx_format_is_specifier_valid_u64(__attr(maybe_unused) const CnxFormat* restrict self,
								  CnxStringView specifier) {
	return cnx_format_is_specifier_valid_integral(specifier);
}

CnxFormatContext
cnx_format_is_specifier_valid_i8(__attr(maybe_unused) const CnxFormat* restrict self,
								 CnxStringView specifier) {
	return cnx_format_is_specifier_valid_integral(specifier);
}

CnxFormatContext
cnx_format_is_specifier_valid_i16(__attr(maybe_unused) const CnxFormat* restrict self,
								  CnxStringView specifier) {
	return cnx_format_is_specifier_valid_integral(specifier);
}

CnxFormatContext
cnx_format_is_specifier_valid_i32(__attr(maybe_unused) const CnxFormat* restrict self,
								  CnxStringView specifier) {
	return cnx_format_is_specifier_valid_integral(specifier);
}

CnxFormatContext
cnx_format_is_specifier_valid_i64(__attr(maybe_unused) const CnxFormat* restrict self,
								  CnxStringView specifier) {
	return cnx_format_is_specifier_valid_integral(specifier);
}

CnxFormatContext
cnx_format_is_specifier_valid_f32(__attr(maybe_unused) const CnxFormat* restrict self,
								  CnxStringView specifier) {
	return cnx_format_is_specifier_valid_fp(specifier);
}

CnxFormatContext
cnx_format_is_specifier_valid_f64(__attr(maybe_unused) const CnxFormat* restrict self,
								  CnxStringView specifier) {
	return cnx_format_is_specifier_valid_fp(specifier);
}

CnxFormatContext
cnx_format_is_specifier_valid_ptr(__attr(maybe_unused) const CnxFormat* restrict self,
								  CnxStringView specifier) {
	return cnx_format_is_specifier_valid_integral(specifier);
}

CnxFormatContext
cnx_format_is_specifier_valid_cnx_string(__attr(maybe_unused) const CnxFormat* restrict self,
										 CnxStringView specifier) {
	return cnx_format_is_specifier_valid_generic(specifier);
}

CnxFormatContext
cnx_format_is_specifier_valid_cnx_stringview(__attr(maybe_unused) const CnxFormat* restrict self,
											 CnxStringView specifier) {
	return cnx_format_is_specifier_valid_generic(specifier);
}

CnxString cnx_format_bool(const CnxFormat* restrict self, CnxFormatContext context) {
	return cnx_format_bool_with_allocator(self, context, cnx_allocator_new());
}

CnxString cnx_format_bool_with_allocator(const CnxFormat* restrict self,
										 __attr(maybe_unused) CnxFormatContext context,
										 CnxAllocator allocator) {
	cnx_assert(context.is_valid == CNX_FORMAT_SUCCESS,
			   "Bad format specifier used to format a bool");

	let _self = *static_cast(bool*)(self->m_self);
	if(_self) {
		return cnx_string_from_with_allocator("true", allocator);
	}
	else {
		return cnx_string_from_with_allocator("false", allocator);
	}
}

CnxString cnx_format_char(const CnxFormat* restrict self, CnxFormatContext context) {
	return cnx_format_char_with_allocator(self, context, cnx_allocator_new());
}

CnxString cnx_format_char_with_allocator(const CnxFormat* restrict self,
										 CnxFormatContext context,
										 CnxAllocator allocator) {
	let state = *(static_cast(const IntegralContext*)(context.state));
	cnx_assert(context.is_valid == CNX_FORMAT_SUCCESS,
			   "Bad format specifier used to format a char");

	if(state.is_debug || state.notation == CNX_FORMAT_UNSIGNED_NOTATION_DEFAULT) {
		return cnx_string_from_cstring_with_allocator(static_cast(const_cstring)(self->m_self),
													  1,
													  allocator);
	}
	else if(state.notation == CNX_FORMAT_UNSIGNED_NOTATION_DECIMAL) {
		let num = *static_cast(const char*)(self->m_self);
		return cnx_format_decimal(static_cast(u64)(num), NUM_DIGITS_CHAR, allocator);
	}
	else {
		let num = static_cast(u8)(*(static_cast(const char*)(self->m_self)));
		return cnx_format_hex(num, NUM_HEX_DIGITS_CHAR, state.notation, allocator);
	}
}

CnxString cnx_format_u8(const CnxFormat* restrict self, CnxFormatContext context) {
	return cnx_format_u8_with_allocator(self, context, cnx_allocator_new());
}

CnxString cnx_format_u8_with_allocator(const CnxFormat* restrict self,
									   CnxFormatContext context,
									   CnxAllocator allocator) {
	let state = *(static_cast(const IntegralContext*)(context.state));
	cnx_assert(context.is_valid == CNX_FORMAT_SUCCESS, "Bad format specifier used to format a u8");

	let num = *static_cast(const u8*)(self->m_self);
	return (state.notation == CNX_FORMAT_UNSIGNED_NOTATION_DEFAULT
			|| state.notation == CNX_FORMAT_UNSIGNED_NOTATION_DECIMAL || state.is_debug) ?
			   cnx_format_decimal(num, NUM_DIGITS_U8, allocator) :
			   cnx_format_hex(num, NUM_HEX_DIGITS_U8, state.notation, allocator);
}

CnxString cnx_format_u16(const CnxFormat* restrict self, CnxFormatContext context) {
	return cnx_format_u16_with_allocator(self, context, cnx_allocator_new());
}

CnxString cnx_format_u16_with_allocator(const CnxFormat* restrict self,
										CnxFormatContext context,
										CnxAllocator allocator) {
	let state = *(static_cast(const IntegralContext*)(context.state));
	cnx_assert(context.is_valid == CNX_FORMAT_SUCCESS, "Bad format specifier used to format a u16");

	let num = *static_cast(const u16*)(self->m_self);
	return (state.notation == CNX_FORMAT_UNSIGNED_NOTATION_DEFAULT
			|| state.notation == CNX_FORMAT_UNSIGNED_NOTATION_DECIMAL || state.is_debug) ?
			   cnx_format_decimal(num, NUM_DIGITS_U16, allocator) :
			   cnx_format_hex(num, NUM_HEX_DIGITS_U16, state.notation, allocator);
}

CnxString cnx_format_u32(const CnxFormat* restrict self, CnxFormatContext context) {
	return cnx_format_u32_with_allocator(self, context, cnx_allocator_new());
}

CnxString cnx_format_u32_with_allocator(const CnxFormat* restrict self,
										CnxFormatContext context,
										CnxAllocator allocator) {
	let state = *(static_cast(const IntegralContext*)(context.state));
	cnx_assert(context.is_valid == CNX_FORMAT_SUCCESS, "Bad format specifier used to format a u32");

	let num = *static_cast(const u32*)(self->m_self);
	return (state.notation == CNX_FORMAT_UNSIGNED_NOTATION_DEFAULT
			|| state.notation == CNX_FORMAT_UNSIGNED_NOTATION_DECIMAL || state.is_debug) ?
			   cnx_format_decimal(num, NUM_DIGITS_U32, allocator) :
			   cnx_format_hex(num, NUM_HEX_DIGITS_U32, state.notation, allocator);
}

CnxString cnx_format_u64(const CnxFormat* restrict self, CnxFormatContext context) {
	return cnx_format_u64_with_allocator(self, context, cnx_allocator_new());
}

CnxString cnx_format_u64_with_allocator(const CnxFormat* restrict self,
										CnxFormatContext context,
										CnxAllocator allocator) {
	let state = *(static_cast(const IntegralContext*)(context.state));
	cnx_assert(context.is_valid == CNX_FORMAT_SUCCESS, "Bad format specifier used to format a u64");

	let num = *static_cast(const u64*)(self->m_self);
	return (state.notation == CNX_FORMAT_UNSIGNED_NOTATION_DEFAULT
			|| state.notation == CNX_FORMAT_UNSIGNED_NOTATION_DECIMAL || state.is_debug) ?
			   cnx_format_decimal(num, NUM_DIGITS_U64, allocator) :
			   cnx_format_hex(num, NUM_HEX_DIGITS_U64, state.notation, allocator);
}

CnxString cnx_format_i8(const CnxFormat* restrict self, CnxFormatContext context) {
	return cnx_format_i8_with_allocator(self, context, cnx_allocator_new());
}

CnxString cnx_format_i8_with_allocator(const CnxFormat* restrict self,
									   CnxFormatContext context,
									   CnxAllocator allocator) {
	let state = *(static_cast(const IntegralContext*)(context.state));
	cnx_assert(context.is_valid == CNX_FORMAT_SUCCESS, "Bad format specifier used to format a i8");

	let num = *static_cast(const i8*)(self->m_self);
	return (state.notation == CNX_FORMAT_UNSIGNED_NOTATION_DEFAULT
			|| state.notation == CNX_FORMAT_UNSIGNED_NOTATION_DECIMAL || state.is_debug) ?
			   cnx_format_decimal(num, NUM_DIGITS_I8, allocator) :
			   cnx_format_hex(static_cast(u64)(num), NUM_HEX_DIGITS_I8, state.notation, allocator);
}

CnxString cnx_format_i16(const CnxFormat* restrict self, CnxFormatContext context) {
	return cnx_format_i16_with_allocator(self, context, cnx_allocator_new());
}

CnxString cnx_format_i16_with_allocator(const CnxFormat* restrict self,
										CnxFormatContext context,
										CnxAllocator allocator) {
	let state = *(static_cast(const IntegralContext*)(context.state));
	cnx_assert(context.is_valid == CNX_FORMAT_SUCCESS, "Bad format specifier used to format a i16");

	let num = *static_cast(const i16*)(self->m_self);
	return (state.notation == CNX_FORMAT_UNSIGNED_NOTATION_DEFAULT
			|| state.notation == CNX_FORMAT_UNSIGNED_NOTATION_DECIMAL || state.is_debug) ?
			   cnx_format_decimal(num, NUM_DIGITS_I16, allocator) :
			   cnx_format_hex(static_cast(u64)(num), NUM_HEX_DIGITS_I16, state.notation, allocator);
}

CnxString cnx_format_i32(const CnxFormat* restrict self, CnxFormatContext context) {
	return cnx_format_i32_with_allocator(self, context, cnx_allocator_new());
}

CnxString cnx_format_i32_with_allocator(const CnxFormat* restrict self,
										CnxFormatContext context,
										CnxAllocator allocator) {
	let state = *(static_cast(const IntegralContext*)(context.state));
	cnx_assert(context.is_valid == CNX_FORMAT_SUCCESS, "Bad format specifier used to format a i32");

	let num = *static_cast(const i32*)(self->m_self);
	return (state.notation == CNX_FORMAT_UNSIGNED_NOTATION_DEFAULT
			|| state.notation == CNX_FORMAT_UNSIGNED_NOTATION_DECIMAL || state.is_debug) ?
			   cnx_format_decimal(num, NUM_DIGITS_I32, allocator) :
			   cnx_format_hex(static_cast(u64)(num), NUM_HEX_DIGITS_I32, state.notation, allocator);
}

CnxString cnx_format_i64(const CnxFormat* restrict self, CnxFormatContext context) {
	return cnx_format_i64_with_allocator(self, context, cnx_allocator_new());
}

CnxString cnx_format_i64_with_allocator(const CnxFormat* restrict self,
										CnxFormatContext context,
										CnxAllocator allocator) {
	let state = *(static_cast(const IntegralContext*)(context.state));
	cnx_assert(context.is_valid == CNX_FORMAT_SUCCESS, "Bad format specifier used to format a i64");

	let num = *static_cast(const i64*)(self->m_self);
	return (state.notation == CNX_FORMAT_UNSIGNED_NOTATION_DEFAULT
			|| state.notation == CNX_FORMAT_UNSIGNED_NOTATION_DECIMAL || state.is_debug) ?
			   cnx_format_decimal(num, NUM_DIGITS_I64, allocator) :
			   cnx_format_hex(static_cast(u64)(num), NUM_HEX_DIGITS_I64, state.notation, allocator);
}

// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
__attr(nodiscard) CnxString cnx_format_f64_scientific(f64 shifted_to_scientific,
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

__attr(nodiscard) CnxString
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

__attr(nodiscard) __attr(always_inline) static inline CnxString
	cnx_format_f64_with_allocator__(f64 num, CnxFormatContext context, CnxAllocator allocator) {
	let state = *(static_cast(const FloatContext*)(context.state));
	let_mut exponent_base_2 = 0;
	ignore(frexp(num, &exponent_base_2));
	let exponent = static_cast(i64)(static_cast(f32)(exponent_base_2) * LN_2 / LN_10);
	return (state.notation == CNX_FORMAT_FLOAT_NOTATION_DEFAULT
			|| state.notation == CNX_FORMAT_FLOAT_NOTATION_SCIENTIFIC || state.is_debug) ?
			   cnx_format_f64_scientific(num / powers_of_10[exponent],
										 exponent,
										 state.num_sig_figs,
										 allocator) :
			   cnx_format_f64_decimal(num, exponent, state.num_sig_figs, allocator);
}

CnxString cnx_format_f32(const CnxFormat* restrict self, CnxFormatContext context) {
	return cnx_format_f32_with_allocator(self, context, cnx_allocator_new());
}

CnxString cnx_format_f32_with_allocator(const CnxFormat* restrict self,
										CnxFormatContext context,
										CnxAllocator allocator) {
	cnx_assert(context.is_valid == CNX_FORMAT_SUCCESS, "Bad format specifier used to format a f32");

	let num = *static_cast(const f32*)(self->m_self);
	return cnx_format_f64_with_allocator__(num, context, allocator);
}

CnxString cnx_format_f64(const CnxFormat* restrict self, CnxFormatContext context) {
	return cnx_format_f64_with_allocator(self, context, cnx_allocator_new());
}

CnxString cnx_format_f64_with_allocator(const CnxFormat* restrict self,
										CnxFormatContext context,
										CnxAllocator allocator) {
	cnx_assert(context.is_valid == CNX_FORMAT_SUCCESS, "Bad format specifier used to format a f64");

	let num = *static_cast(const f64*)(self->m_self);
	return cnx_format_f64_with_allocator__(num, context, allocator);
}

CnxString cnx_format_ptr(const CnxFormat* restrict self, CnxFormatContext context) {
	return cnx_format_ptr_with_allocator(self, context, cnx_allocator_new());
}

CnxString cnx_format_ptr_with_allocator(const CnxFormat* restrict self,
										CnxFormatContext context,
										CnxAllocator allocator) {
	let state = *(static_cast(const IntegralContext*)(context.state));
	cnx_assert(context.is_valid == CNX_FORMAT_SUCCESS,
			   "Bad format specifier used to format a pointer");

	let num = static_cast(usize)(static_cast(const void*)(self->m_self));
	return (state.notation == CNX_FORMAT_UNSIGNED_NOTATION_DECIMAL) ?
			   cnx_format_decimal(num, NUM_DIGITS_I64, allocator) :
			   cnx_format_hex(static_cast(u64)(num), NUM_HEX_DIGITS_I64, state.notation, allocator);
}

CnxString cnx_format_cnx_string(const CnxFormat* restrict self, CnxFormatContext context) {
	return cnx_format_cnx_string_with_allocator(self, context, cnx_allocator_new());
}

CnxString cnx_format_cnx_string_with_allocator(const CnxFormat* restrict self,
											   CnxFormatContext context,
											   CnxAllocator allocator) {
	cnx_assert(context.is_valid == CNX_FORMAT_SUCCESS,
			   "Bad format specifier used to format a CnxString");

	let state = *(static_cast(const GenericContext*)(context.state));
	let string = static_cast(const CnxString*)(self->m_self);
	let_mut str = cnx_string_into_cstring(*string);
	if(state.is_debug) {
		let length = cnx_string_length(*string);
		let capacity = cnx_string_capacity(*string);
		return cnx_format_with_allocator(
			AS_STRING(CnxString) ": [length: {}, capacity: {}, data: {}]",
			allocator,
			length,
			capacity,
			str);
	}

	return cnx_string_clone_with_allocator(*string, allocator);
}

CnxString cnx_format_cnx_stringview(const CnxFormat* restrict self, CnxFormatContext context) {
	return cnx_format_cnx_stringview_with_allocator(self, context, cnx_allocator_new());
}

CnxString cnx_format_cnx_stringview_with_allocator(const CnxFormat* restrict self,
												   CnxFormatContext context,
												   CnxAllocator allocator) {
	cnx_assert(context.is_valid == CNX_FORMAT_SUCCESS,
			   "Bad format specifier used to format a CnxStringView");

	let state = *(static_cast(const GenericContext*)(context.state));
	let string = static_cast(const CnxStringView*)(self->m_self);
	let_mut str = cnx_stringview_into_cstring(*string);
	if(state.is_debug) {
		let length = cnx_stringview_length(*string);
		return cnx_format_with_allocator(AS_STRING(CnxStringView) ": [length: {}, data: {}]",
										 allocator,
										 length,
										 str);
	}

	return cnx_string_from_with_allocator(string, allocator);
}

CnxString cnx_format_cstring(const CnxFormat* restrict self, CnxFormatContext context) {
	return cnx_format_cstring_with_allocator(self, context, cnx_allocator_new());
}

CnxString cnx_format_cstring_with_allocator(const CnxFormat* restrict self,
											__attr(maybe_unused) CnxFormatContext context,
											CnxAllocator allocator) {
	cnx_assert(context.is_valid == CNX_FORMAT_SUCCESS,
			   "Bad format specifier used to format a CnxStringView");
	let string = static_cast(const_cstring)(self->m_self);
	return cnx_string_from_with_allocator(string, allocator);
}

Enum(CnxFormatVariant, (Substring, CnxStringView), (Specifier, CnxStringView));

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

__attr(nodiscard) __attr(always_inline) static inline CnxFormatVariant
	cnx_format_variant_new(__attr(maybe_unused) CnxAllocator allocator) {
	return (CnxFormatVariant){0};
}

__attr(not_null(1))
	__attr(always_inline) static inline void cnx_format_variant_free(__attr(maybe_unused)
																		 CnxFormatVariant* elem,
																	 __attr(maybe_unused)
																		 CnxAllocator allocator) {
}

__attr(nodiscard) __attr(not_null(1)) __attr(always_inline) static inline CnxFormatVariant
	cnx_format_variant_clone(const CnxFormatVariant* restrict elem,
							 __attr(maybe_unused) CnxAllocator allocator) {
	return *elem;
}

__attr(nodiscard) __attr(returns_not_null) const_cstring
	cnx_format_category_get_message(__attr(maybe_unused) const CnxErrorCategory* restrict self,
									i64 error_code) {
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

__attr(nodiscard) i64
	cnx_format_category_get_last_error(__attr(maybe_unused) const CnxErrorCategory* restrict self) {
	return 0;
}

typedef struct CnxFormatErrorCategory {
} CnxFormatErrorCategory;

#if CNX_PLATFORM_COMPILER_CLANG
	#define IGNORE_UNUSED_BUT_SET_WARNING_START \
		_Pragma("GCC diagnostic push")          \
			_Pragma("GCC diagnostic ignored \"-Wunused-but-set-variable\"")
	#define IGNORE_UNUSED_BUT_SET_WARNING_STOP _Pragma("GCC diagnostic pop")
#else
	#define IGNORE_UNUSED_BUT_SET_WARNING_START
	#define IGNORE_UNUSED_BUT_SET_WARNING_STOP
#endif // CNX_PLATFORM_COMPILER_CLANG
__attr(maybe_unused) static ImplTraitFor(CnxErrorCategory,
										 CnxFormatErrorCategory,
										 cnx_format_category_get_message,
										 cnx_format_category_get_last_error);

__attr(maybe_unused) static const CnxFormatErrorCategory cnx_format_error_category = {};

__attr(maybe_unused) static let cnx_format_category
	= as_trait(CnxErrorCategory, CnxFormatErrorCategory, cnx_format_error_category);

static let format_variant_data = (CnxCollectionData(CnxVector(CnxFormatVariant))){
	.m_constructor = cnx_format_variant_new,
	.m_destructor = cnx_format_variant_free,
	.m_copy_constructor = cnx_format_variant_clone};

__attr(nodiscard) __attr(not_null(1)) CnxResult(CnxVector(CnxFormatVariant))
	// NOLINTNEXTLINE(readability-function-cognitive-complexity)
	cnx_format_parse_and_validate_format_string(
		restrict const_cstring format_string,
		// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
		usize length,
		__attr(maybe_unused) usize num_args,
		CnxAllocator allocator) {
	let_mut vec = cnx_vector_new_with_capacity_allocator_and_collection_data(CnxFormatVariant,
																			 num_args * 2,
																			 allocator,
																			 &format_variant_data);

#if CNX_PLATFORM_DEBUG
	let_mut in_specifier = false;
#endif // CNX_PLATFORM_DEBUG
	IGNORE_UNUSED_BUT_SET_WARNING_START
	// NOLINTNEXTLINE(clang-analyzer-deadcode.DeadStores)
	let_mut num_open = 0U;
	// NOLINTNEXTLINE(clang-analyzer-deadcode.DeadStores)
	let_mut num_close = 0U;
	IGNORE_UNUSED_BUT_SET_WARNING_STOP

	let_mut start_index = 0U;
	ranged_for(i, 0U, length) {
		if(format_string[i] == '{') {
#if CNX_PLATFORM_DEBUG
			if(in_specifier) {
				return Err(CnxVector(CnxFormatVariant),
						   cnx_error_new(CNX_FORMAT_BAD_SPECIFIER_INVALID_CHAR_IN_SPECIFIER,
										 cnx_format_category));
			}
			else {
#endif // CNX_PLATFORM_DEBUG
				if(i > 0) {
					if(format_string[i - 1] != '\\') {
						let view = cnx_stringview_from(format_string, start_index, i - start_index);
						cnx_vector_push_back(vec, Substring(view));
					}
				}
				num_open++;
				start_index = i + 1;
#if CNX_PLATFORM_DEBUG
				in_specifier = true;
			}
#endif // CNX_PLATFORM_DEBUG
		}
		else if(format_string[i] == '}') {
#if CNX_PLATFORM_DEBUG
			if(!in_specifier && format_string[i - 1] != '\\') {
				return Err(
					CnxVector(CnxFormatVariant),
					cnx_error_new(CNX_FORMAT_INVALID_CLOSING_BRACE_LOCATION, cnx_format_category));
			}
			else if(format_string[i - 1] == '\\') {
				return Err(CnxVector(CnxFormatVariant),
						   cnx_error_new(CNX_FORMAT_BAD_SPECIFIER_INVALID_CHAR_IN_SPECIFIER,
										 cnx_format_category));
			}
			else {
				cnx_vector_push_back(
					vec,
					Specifier(cnx_stringview_from(format_string, start_index, i - start_index)));
				num_close++;
				start_index = i + 1;
			}
			in_specifier = false;
#else  // CNX_PLATFORM_DEBUG
			cnx_vector_push_back(
				vec,
				Specifier(cnx_stringview_from(format_string, start_index, i - start_index)));
			num_close++;
			start_index = i + 1;
#endif // CNX_PLATFORM_DEBUG
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

// NOLINTNEXTLINE(readability-function-cognitive-complexity, misc-no-recursion)
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

// NOLINTNEXTLINE(readability-function-cognitive-complexity, misc-no-recursion)
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

	// 10 chars per formatted string element is a reasonable first guess
	// tradeof between performance and memory usage
	// NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
	let_mut initial_size = static_cast(usize)(cnx_vector_size(format_variants) * 10U);
	foreach_ref(elem, format_variants) {
		match_let(*elem, Substring, str) {
			initial_size += str.m_length;
		}
	}

	CnxScopedString string = cnx_string_new_with_capacity_with_allocator(initial_size, allocator);

#if CNX_PLATFORM_DEBUG
	let_mut spec_index = static_cast(usize)(0);
#endif
	foreach(elem, format_variants) {
		match(elem) {
			variant(Substring, view) {
				cnx_string_append(string, &view);
			}
			variant(Specifier, specifier) {
				let format = va_arg(list, CnxFormat); // NOLINT(clang-analyzer-valist.Uninitialized)
				let context = trait_call(is_specifier_valid, format, specifier);
#if CNX_PLATFORM_DEBUG
				if(context.is_valid != CNX_FORMAT_SUCCESS) {
					let_mut error_message
						= cnx_format_category_get_message(&cnx_format_category, context.is_valid);
					CnxScopedString message = cnx_format_with_allocator(
						"Invalid specifier used for specifier {} in format string\n{}",
						allocator,
						spec_index,
						error_message);
					cnx_assert(context.is_valid == CNX_FORMAT_SUCCESS,
							   cnx_string_into_cstring(message));
					cnx_panic(error_message);
				}
				spec_index++;
#endif
				CnxScopedString formatted
					= trait_call(format_with_allocator, format, context, allocator);
				cnx_string_append(string, &formatted);
			}
		}
	}

	return move(string);
}
