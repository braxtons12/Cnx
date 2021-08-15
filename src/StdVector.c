/// @file StdVector.c
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief This module provides a dynamic-array type comparable to C++'s `std::vector` and Rust's
/// `std::vec::Vec` for C2nxt
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

#include "../include/C2nxt/StdVector.h"

ImplStdVector(char);
ImplStdVector(u8);
ImplStdVector(u16);
ImplStdVector(u32);
ImplStdVector(u64);
ImplStdVector(usize);
ImplStdVector(i8);
ImplStdVector(i16);
ImplStdVector(i32);
ImplStdVector(i64);
ImplStdVector(f32);
ImplStdVector(f64);
ImplStdVector(u8_ptr);
ImplStdVector(u16_ptr);
ImplStdVector(u32_ptr);
ImplStdVector(u64_ptr);
ImplStdVector(usize_ptr);
ImplStdVector(i8_ptr);
ImplStdVector(i16_ptr);
ImplStdVector(i32_ptr);
ImplStdVector(i64_ptr);
ImplStdVector(f32_ptr);
ImplStdVector(f64_ptr);
ImplStdVector(cstring);
ImplStdVector(char_ptr);
ImplStdVector(StdString);
ImplStdVector(StdStringView);
