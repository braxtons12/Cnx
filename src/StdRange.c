#include "../include/C2nxt/StdRange.h"

#include "../include/C2nxt/StdPlatform.h"

// clang-format off
#if STD_PLATFORM_COMPILER_CLANG && STD_PLATFORM_APPLE
	_Pragma("GCC diagnostic push") _Pragma("GCC diagnostic ignored \"-Wnonportable-include-path\"")
#endif

#include "../include/C2nxt/StdIO.h"

#if STD_PLATFORM_COMPILER_CLANG && STD_PLATFORM_APPLE
	_Pragma("GCC diagnostic pop")
#endif

ImplStdRange(char);
ImplStdRange(u8);
ImplStdRange(u16);
ImplStdRange(u32);
ImplStdRange(u64);
ImplStdRange(usize);
ImplStdRange(i8);
ImplStdRange(i16);
ImplStdRange(i32);
ImplStdRange(i64);
ImplStdRange(isize);
ImplStdRange(f32);
ImplStdRange(f64);
ImplStdRange(u8_ptr);
ImplStdRange(u16_ptr);
ImplStdRange(u32_ptr);
ImplStdRange(u64_ptr);
ImplStdRange(usize_ptr);
ImplStdRange(i8_ptr);
ImplStdRange(i16_ptr);
ImplStdRange(i32_ptr);
ImplStdRange(i64_ptr);
ImplStdRange(isize_ptr);
ImplStdRange(f32_ptr);
ImplStdRange(f64_ptr);
ImplStdRange(cstring);
ImplStdRange(const_cstring);
ImplStdRange(char_ptr);
ImplStdRange(const_char_ptr);

// clang-format on
