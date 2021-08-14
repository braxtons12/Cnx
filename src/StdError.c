#if defined(_MSC_VER) || defined(_WIN32) || defined(_WIN64)
	// NOLINTNEXTLINE
	#define _CRT_SECURE_NO_WARNINGS
#endif

#include "../include/C2nxt/StdError.h"

#include <errno.h>

StdError std_error_new(i64 error_code, StdErrorCategory error_category) {
	return (StdError){.m_error_code = error_code, .m_error_category = error_category};
}

i64 std_error_code(const StdError* restrict self) {
	return self->m_error_code;
}

StdString std_error_message(const StdError* restrict self) {
	return std_string_from(
		std_error_category_get_message(self->m_error_category, self->m_error_code));
}

StdString std_error_message_with_allocator(const StdError* restrict self, StdAllocator allocator) {
	return std_string_from_with_allocator(
		std_error_category_get_message(self->m_error_category, self->m_error_code),
		allocator);
}

const_cstring std_error_message_as_cstring(const StdError* restrict self) {
	return std_error_category_get_message(self->m_error_category, self->m_error_code);
}

const_cstring std_error_category_get_message(StdErrorCategory self, i64 error_code) {
	return self.m_message_function(error_code);
}

const_cstring std_error_category_get_posix_message(i64 error_code) {
	return strerror(narrow_cast(int)(error_code));
}

StdString std_error_format(const StdFormat* restrict self, StdFormatSpecifier specifier) {
	return std_error_format_with_allocator(self, specifier, std_allocator_new());
}

StdString std_error_format_with_allocator(const StdFormat* restrict self,
										  StdFormatSpecifier maybe_unused specifier,
										  StdAllocator allocator) {
	std_assert(specifier.m_type == STD_FORMAT_TYPE_DEFAULT,
			   "Can't format StdError with custom specifier");

	let _self = static_cast(const StdError*)(self->m_self);
	let message = std_string_from_with_allocator(
		_self->m_error_category.m_message_function(_self->m_error_code),
		allocator);
	return std_format_with_allocator(AS_STRING(StdError) ": [error_code: {d}, error_message: {}]",
									 allocator,
									 _self->m_error_code,
									 message);
}
