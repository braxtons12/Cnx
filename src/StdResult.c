/// @file StdResult.c
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief This module provides a struct template for representing the value of a fallible operation
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

#include "../include/C2nxt/StdResult.h"

ImplStdResult(bool);
ImplStdResult(char);
ImplStdResult(u8);
ImplStdResult(u16);
ImplStdResult(u32);
ImplStdResult(u64);
ImplStdResult(usize);
ImplStdResult(i8);
ImplStdResult(i16);
ImplStdResult(i32);
ImplStdResult(i64);
ImplStdResult(isize);
ImplStdResult(f32);
ImplStdResult(f64);
ImplStdResult(u8_ptr);
ImplStdResult(u16_ptr);
ImplStdResult(u32_ptr);
ImplStdResult(u64_ptr);
ImplStdResult(usize_ptr);
ImplStdResult(i8_ptr);
ImplStdResult(i16_ptr);
ImplStdResult(i32_ptr);
ImplStdResult(i64_ptr);
ImplStdResult(isize_ptr);
ImplStdResult(f32_ptr);
ImplStdResult(f64_ptr);
ImplStdResult(cstring);
ImplStdResult(const_cstring);
ImplStdResult(char_ptr);
ImplStdResult(const_char_ptr);
ImplStdResult(StdString);
ImplStdResult(StdStringView);
