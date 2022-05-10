/// @file TimePoint.c
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief CnxTimePoint provides functionality for working with specific points in time
/// @version 0.1.3
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

#include <Cnx/time/Clock.h>
#include <Cnx/time/TimePoint.h>
#include <time.h>

#define unlikely(x) __builtin_expect(!!(x), 0)

CnxTimePoint cnx_time_point_new(CnxDuration time_since_epoch) {
	return cnx_time_point_new_with_clock(time_since_epoch, &cnx_system_clock);
}

CnxTimePoint
cnx_time_point_new_with_clock(CnxDuration time_since_epoch, const CnxClock* restrict clock) {
	let precision = trait_call(resolution_as_ratio, *clock);
	CnxDuration time = time_since_epoch;
	if(!cnx_ratio_equal(precision, time_since_epoch.period)) {
		time = cnx_duration_cast(time_since_epoch, precision);
	}
	return (CnxTimePoint){.time_since_epoch = time, .clock = clock, trait_call(locale, *clock)};
}

CnxTimePoint cnx_time_point_new_with_clock_and_locale(CnxDuration time_since_epoch,
													  const CnxClock* restrict clock,
													  CnxTimePointLocale locale) {
	let precision = trait_call(resolution_as_ratio, *clock);
	CnxDuration time = time_since_epoch;
	if(!cnx_ratio_equal(precision, time_since_epoch.period)) {
		time = cnx_duration_cast(time_since_epoch, precision);
	}
	return (CnxTimePoint){.time_since_epoch = time, .clock = clock, .locale = locale};
}

CnxDuration cnx_time_point_time_since_epoch(CnxTimePoint self) {
	return self.time_since_epoch;
}

CnxTimePoint cnx_time_point_min(CnxTimePoint self) {
	return trait_call(min_time_point, (*self.clock));
}

CnxTimePoint cnx_time_point_max(CnxTimePoint self) {
	return trait_call(max_time_point, (*self.clock));
}

// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
CnxTimePoint cnx_time_point_cast(CnxTimePoint to_cast, CnxTimePoint new_precision) {
	return (CnxTimePoint){
		.time_since_epoch
		= cnx_duration_cast(to_cast.time_since_epoch, new_precision.time_since_epoch.period),
		.clock = new_precision.clock};
}

// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
CnxTimePoint cnx_time_point_ceil(CnxTimePoint to_cast, CnxTimePoint new_precision) {
	return (CnxTimePoint){
		.time_since_epoch
		= cnx_duration_ceil(to_cast.time_since_epoch, new_precision.time_since_epoch.period),
		.clock = new_precision.clock};
}

// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
CnxTimePoint cnx_time_point_floor(CnxTimePoint to_cast, CnxTimePoint new_precision) {
	return (CnxTimePoint){
		.time_since_epoch
		= cnx_duration_floor(to_cast.time_since_epoch, new_precision.time_since_epoch.period),
		.clock = new_precision.clock};
}

// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
CnxTimePoint cnx_time_point_round(CnxTimePoint to_cast, CnxTimePoint new_precision) {
	return (CnxTimePoint){
		.time_since_epoch
		= cnx_duration_round(to_cast.time_since_epoch, new_precision.time_since_epoch.period),
		.clock = new_precision.clock};
}

time_t cnx_time_point_as_time_t(CnxTimePoint to_cast) {
	return static_cast(time_t)(
		cnx_duration_cast(to_cast.time_since_epoch, cnx_seconds_period).count);
}

#define RESULT_IMPL TRUE
#define RESULT_T	tm
#include <Cnx/Result.h>
#undef RESULT_T
#undef RESULT_IMPL

CnxResult(tm) cnx_time_point_as_tm(CnxTimePoint to_cast) {
	if(to_cast.locale == CNX_LOCAL_TIME) {
		// localtime requires a
		let point = cnx_convert_local_time_to_utc(to_cast);
		let time = cnx_time_point_as_time_t(point);
#if _POSIX_C_SOURCE >= 1 || _XOPEN_SOURCE || _BSD_SOURCE || _SVID_SOURCE || _POSIX_SOURCE
		tm as_tm;
		let res = localtime_r(&time, &as_tm);
		if(unlikely(res == nullptr)) {
			return Err(tm, cnx_error_new(errno, CNX_POSIX_ERROR_CATEGORY));
		}
		return Ok(tm, as_tm);
#else
		let res = localtime(&time);
		if(unlikely(res == nullptr)) {
			return Err(tm, cnx_error_new(errno, CNX_POSIX_ERROR_CATEGORY));
		}
		return Ok(tm, *res);
#endif
	}
	else {
		let time = cnx_time_point_as_time_t(to_cast);
#if _POSIX_C_SOURCE >= 1 || _XOPEN_SOURCE || _BSD_SOURCE || _SVID_SOURCE || _POSIX_SOURCE
		tm as_tm;
		let res = gmtime_r(&time, &as_tm);
		if(unlikely(res == nullptr)) {
			return Err(tm, cnx_error_new(errno, CNX_POSIX_ERROR_CATEGORY));
		}
		return Ok(tm, as_tm);
#else
		let res = gmtime(&time);
		if(unlikely(res == nullptr)) {
			return Err(tm, cnx_error_new(errno, CNX_POSIX_ERROR_CATEGORY));
		}
		return Ok(tm, *res);
#endif
	}
}

CnxTimePoint cnx_time_point_from_time_t(time_t time) {
	let duration = cnx_duration_cast(cnx_duration_new(static_cast(i64)(time), cnx_seconds_period),
									 trait_call(resolution_as_ratio, cnx_system_clock));
	return cnx_time_point_new_with_clock(duration, &cnx_system_clock);
}

void cnx_time_point_increment(CnxTimePoint* restrict self) {
	self->time_since_epoch = cnx_duration_add(self->time_since_epoch,
											  cnx_duration_new(1, self->time_since_epoch.period));
}

void cnx_time_point_decrement(CnxTimePoint* restrict self) {
	self->time_since_epoch
		= cnx_duration_subtract(self->time_since_epoch,
								cnx_duration_new(1, self->time_since_epoch.period));
}

CnxTimePoint cnx_time_point_add(CnxTimePoint lhs, CnxDuration rhs) {
	return (CnxTimePoint){.time_since_epoch = cnx_duration_add(lhs.time_since_epoch, rhs),
						  .clock = lhs.clock};
}

CnxTimePoint cnx_time_point_add_time_point(CnxTimePoint lhs, CnxTimePoint rhs) {
	return cnx_time_point_add(lhs, rhs.time_since_epoch);
}

CnxTimePoint cnx_time_point_add_scalar(CnxTimePoint lhs, i64 rhs) {
	return (CnxTimePoint){.time_since_epoch = cnx_duration_add_scalar(lhs.time_since_epoch, rhs),
						  .clock = lhs.clock};
}

CnxTimePoint cnx_time_point_subtract(CnxTimePoint lhs, CnxDuration rhs) {
	return (CnxTimePoint){.time_since_epoch = cnx_duration_subtract(lhs.time_since_epoch, rhs),
						  .clock = lhs.clock};
}

CnxTimePoint cnx_time_point_subtract_time_point(CnxTimePoint lhs, CnxTimePoint rhs) {
	return cnx_time_point_subtract(lhs, rhs.time_since_epoch);
}

CnxTimePoint cnx_time_point_subtract_scalar(CnxTimePoint lhs, i64 rhs) {
	return (CnxTimePoint){.time_since_epoch
						  = cnx_duration_subtract_scalar(lhs.time_since_epoch, rhs),
						  .clock = lhs.clock};
}

bool cnx_time_point_equal(CnxTimePoint lhs, CnxTimePoint rhs) {
	return cnx_duration_equal(lhs.time_since_epoch, rhs.time_since_epoch);
}

bool cnx_time_point_not_equal(CnxTimePoint lhs, CnxTimePoint rhs) {
	return cnx_duration_not_equal(lhs.time_since_epoch, rhs.time_since_epoch);
}

bool cnx_time_point_less_than(CnxTimePoint lhs, CnxTimePoint rhs) {
	return cnx_duration_less_than(lhs.time_since_epoch, rhs.time_since_epoch);
}

bool cnx_time_point_less_than_or_equal(CnxTimePoint lhs, CnxTimePoint rhs) {
	return cnx_duration_less_than_or_equal(lhs.time_since_epoch, rhs.time_since_epoch);
}

bool cnx_time_point_greater_than(CnxTimePoint lhs, CnxTimePoint rhs) {
	return cnx_duration_greater_than(lhs.time_since_epoch, rhs.time_since_epoch);
}

bool cnx_time_point_greater_than_or_equal(CnxTimePoint lhs, CnxTimePoint rhs) {
	return cnx_duration_greater_than_or_equal(lhs.time_since_epoch, rhs.time_since_epoch);
}

CnxCompare cnx_time_point_compare(CnxTimePoint lhs, CnxTimePoint rhs) {
	return cnx_duration_compare(lhs.time_since_epoch, rhs.time_since_epoch);
}

static const char digits[100][2] = {
	{'0', '0'},
	{'0', '1'},
	{'0', '2'},
	{'0', '3'},
	{'0', '4'},
	{'0', '5'},
	{'0', '6'},
	{'0', '7'},
	{'0', '8'},
	{'0', '9'},
	{'1', '0'},
	{'1', '1'},
	{'1', '2'},
	{'1', '3'},
	{'1', '4'},
	{'1', '5'},
	{'1', '6'},
	{'1', '7'},
	{'1', '8'},
	{'1', '9'},
	{'2', '0'},
	{'2', '1'},
	{'2', '2'},
	{'2', '3'},
	{'2', '4'},
	{'2', '5'},
	{'2', '6'},
	{'2', '7'},
	{'2', '8'},
	{'2', '9'},
	{'3', '0'},
	{'3', '1'},
	{'3', '2'},
	{'3', '3'},
	{'3', '4'},
	{'3', '5'},
	{'3', '6'},
	{'3', '7'},
	{'3', '8'},
	{'3', '9'},
	{'4', '0'},
	{'4', '1'},
	{'4', '2'},
	{'4', '3'},
	{'4', '4'},
	{'4', '5'},
	{'4', '6'},
	{'4', '7'},
	{'4', '8'},
	{'4', '9'},
	{'5', '0'},
	{'5', '1'},
	{'5', '2'},
	{'5', '3'},
	{'5', '4'},
	{'5', '5'},
	{'5', '6'},
	{'5', '7'},
	{'5', '8'},
	{'5', '9'},
	{'6', '0'},
	{'6', '1'},
	{'6', '2'},
	{'6', '3'},
	{'6', '4'},
	{'6', '5'},
	{'6', '6'},
	{'6', '7'},
	{'6', '8'},
	{'6', '9'},
	{'7', '0'},
	{'7', '1'},
	{'7', '2'},
	{'7', '3'},
	{'7', '4'},
	{'7', '5'},
	{'7', '6'},
	{'7', '7'},
	{'7', '8'},
	{'7', '9'},
	{'8', '0'},
	{'8', '1'},
	{'8', '2'},
	{'8', '3'},
	{'8', '4'},
	{'8', '5'},
	{'8', '6'},
	{'8', '7'},
	{'8', '8'},
	{'8', '9'},
	{'9', '0'},
	{'9', '1'},
	{'9', '2'},
	{'9', '3'},
	{'9', '4'},
	{'9', '5'},
	{'9', '6'},
	{'9', '7'},
	{'9', '8'},
	{'9', '9'}
};

__attr(always_inline) __attr(nodiscard) static inline char timepoint_parse_int(int num, int digit) {
	return static_cast(char)(digit < 2 ? digits[num % 100][digit] // NOLINT
										 :
										 digits[num / 100][digit - 2] // NOLINT
	);
}

CnxString cnx_time_point_human_readable_format(CnxTimePoint self, CnxAllocator allocator) {
	if(unlikely(self.locale == CNX_LOCAL_TIME)) {
		self = cnx_convert_local_time_to_utc(self);
	}

	let_mut maybe_parsed = cnx_time_point_as_tm(self);
	let parsed = cnx_result_unwrap(maybe_parsed);
	let year = parsed.tm_year + 1900; // NOLINT
	let month = parsed.tm_mon + 1;

	//// NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
	const char memory[20] = {
		[0] = timepoint_parse_int(year, 2),
		[1] = timepoint_parse_int(year, 3),
		[2] = timepoint_parse_int(year, 0),
		[3] = timepoint_parse_int(year, 1),
		[4] = '-',
		[5] = timepoint_parse_int(month, 0),		   // NOLINT
		[6] = timepoint_parse_int(month, 1),		   // NOLINT
		[7] = '-',									   // NOLINT
		[8] = timepoint_parse_int(parsed.tm_mday, 0),  // NOLINT
		[9] = timepoint_parse_int(parsed.tm_mday, 1),  // NOLINT
		[10] = '|',									   // NOLINT
		[11] = timepoint_parse_int(parsed.tm_hour, 0), // NOLINT
		[12] = timepoint_parse_int(parsed.tm_hour, 1), // NOLINT
		[13] = ':',									   // NOLINT
		[14] = timepoint_parse_int(parsed.tm_min, 0),  // NOLINT
		[15] = timepoint_parse_int(parsed.tm_min, 1),  // NOLINT
		[16] = ':',									   // NOLINT
		[17] = timepoint_parse_int(parsed.tm_sec, 0),  // NOLINT
		[18] = timepoint_parse_int(parsed.tm_sec,
								   1), // NOLINT
									   //[19] = ' ',									   // NOLINT
									   //[20] = '+',									   // NOLINT
									   //[21] = '0',									   // NOLINT
									   //[22] = '0',									   // NOLINT
									   //[23] = '0',									   // NOLINT
									   //[24] = '0',									   // NOLINT
	};
	//// NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
	return cnx_string_from_cstring_with_allocator(memory, 20, allocator);
}

CnxString cnx_time_point_format(const CnxFormat* restrict self, CnxFormatSpecifier specifier) {
	return cnx_time_point_format_with_allocator(self, specifier, DEFAULT_ALLOCATOR);
}

CnxString cnx_time_point_format_with_allocator(const CnxFormat* restrict self,
											   __attr(maybe_unused) CnxFormatSpecifier specifier,
											   CnxAllocator allocator) {
	cnx_assert(specifier.m_type == CNX_FORMAT_TYPE_DEFAULT
				   || specifier.m_type == CNX_FORMAT_TYPE_DEBUG,
			   "Can only format a CnxTimePoint with default or debug format specifier");

	let _self = static_cast(const CnxTimePoint*)(self->m_self);
	if(specifier.m_type == CNX_FORMAT_TYPE_DEBUG) {
		return cnx_format_with_allocator(
			AS_STRING(CnxTimePoint) ": [time_since_epoch = {D}, clock = {D}]",
			allocator,
			as_format_t(CnxDuration, _self->time_since_epoch),
			as_format_t(CnxClock, *_self->clock));
	}
	else {
		return cnx_time_point_human_readable_format(*_self, allocator);
	}
}
