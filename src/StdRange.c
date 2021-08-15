/// @file StdRange.c
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief This module provides collection agnostic ways for passing, modifying, and working with
/// collections of elements as "Ranges"
/// @version 0.1
/// @date 2021-08-15
///
/// MIT License
/// @copyright Copyright (c) 2021 Braxton Salyer <braxtonsalyer@gmail.com>
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
