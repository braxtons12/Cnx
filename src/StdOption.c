/// @file StdOption.c
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief This module provides a struct template for representing an optional value
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

#include "../include/C2nxt/StdOption.h"

ImplStdOption(char);
ImplStdOption(u8);
ImplStdOption(u16);
ImplStdOption(u32);
ImplStdOption(u64);
ImplStdOption(usize);
ImplStdOption(i8);
ImplStdOption(i16);
ImplStdOption(i32);
ImplStdOption(i64);
ImplStdOption(isize);
ImplStdOption(f32);
ImplStdOption(f64);
ImplStdOption(u8_ptr);
ImplStdOption(u16_ptr);
ImplStdOption(u32_ptr);
ImplStdOption(u64_ptr);
ImplStdOption(usize_ptr);
ImplStdOption(i8_ptr);
ImplStdOption(i16_ptr);
ImplStdOption(i32_ptr);
ImplStdOption(i64_ptr);
ImplStdOption(isize_ptr);
ImplStdOption(f32_ptr);
ImplStdOption(f64_ptr);
ImplStdOption(cstring);
ImplStdOption(const_cstring);
ImplStdOption(char_ptr);
ImplStdOption(const_char_ptr);
