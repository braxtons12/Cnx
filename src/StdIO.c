#include "../include/C2nxt/StdPlatform.h"
// clang-format off
#if STD_PLATFORM_COMPILER_CLANG && STD_PLATFORM_APPLE
	_Pragma("GCC diagnostic push") _Pragma("GCC diagnostic ignored \"-Wnonportable-include-path\"")
#endif

#include "../include/C2nxt/StdIO.h"

#if STD_PLATFORM_COMPILER_CLANG && STD_PLATFORM_APPLE
	_Pragma("GCC diagnostic pop")
#endif

void print_(const_cstring restrict format_string, StdAllocator allocator, usize num_args, ...) {
	va_list list = {0};
	va_start(list, num_args);
	let_mut std_string_scoped string
		= std_vformat_with_allocator(format_string, allocator, num_args, list);
	puts(std_string_into_cstring(string));
	va_end(list);
}

// clang-format on

void eprint_(const_cstring restrict format_string, StdAllocator allocator, usize num_args, ...) {
	va_list list = {0};
	va_start(list, num_args);
	let_mut std_string_scoped string
		= std_vformat_with_allocator(format_string, allocator, num_args, list);
	fputs(std_string_into_cstring(string), stderr);
	va_end(list);
}

void fprint_(FILE* file,
			 const_cstring restrict format_string,
			 StdAllocator allocator,
			 usize num_args,
			 ...) {
	va_list list = {0};
	va_start(list, num_args);
	let_mut std_string_scoped string
		= std_vformat_with_allocator(format_string, allocator, num_args, list);
	fputs(std_string_into_cstring(string), file);
	va_end(list);
}

void println_(const_cstring restrict format_string, StdAllocator allocator, usize num_args, ...) {
	va_list list = {0};
	va_start(list, num_args);
	let_mut std_string_scoped string
		= std_vformat_with_allocator(format_string, allocator, num_args, list);
	puts(std_string_into_cstring(string));
	va_end(list);
}

void eprintln_(const_cstring restrict format_string, StdAllocator allocator, usize num_args, ...) {
	va_list list = {0};
	va_start(list, num_args);
	let_mut std_string_scoped string
		= std_vformat_with_allocator(format_string, allocator, num_args, list);
	fputs(std_string_into_cstring(string), stderr);
	putc('\n', stderr);
	va_end(list);
}

void fprintln_(FILE* file,
			   const_cstring restrict format_string,
			   StdAllocator allocator,
			   usize num_args,
			   ...) {
	va_list list = {0};
	va_start(list, num_args);
	let_mut std_string_scoped string
		= std_vformat_with_allocator(format_string, allocator, num_args, list);
	fputs(std_string_into_cstring(string), file);
	putc('\n', file);
#ifdef STD_FPRINTLN_FLUSHES
	fflush(file);
#endif // STD_FPRINTLN_FLUSHES
	va_end(list);
}
