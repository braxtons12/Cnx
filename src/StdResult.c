/// @file StdResult.c
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief This module provides a struct template for representing the value of a fallible operation
/// @version 0.2.0
/// @date 2022-03-12
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

#define STD_TEMPLATE_INCLUDE_DEFAULT_INSTANTIATIONS TRUE

#include <C2nxt/StdResult.h>

#undef STD_TEMPLATE_INCLUDE_DEFAULT_INSTANTIATIONS

#define STD_TEMPLATE_IMPL TRUE

#define T char
#include <C2nxt/std_result/StdResultImpl.h>
#undef T

#define T u8
#include <C2nxt/std_result/StdResultImpl.h>
#undef T

#define T u16
#include <C2nxt/std_result/StdResultImpl.h>
#undef T

#define T u32
#include <C2nxt/std_result/StdResultImpl.h>
#undef T

#define T u64
#include <C2nxt/std_result/StdResultImpl.h>
#undef T

#define T usize
#include <C2nxt/std_result/StdResultImpl.h>
#undef T

#define T i8
#include <C2nxt/std_result/StdResultImpl.h>
#undef T

#define T i16
#include <C2nxt/std_result/StdResultImpl.h>
#undef T

#define T i32
#include <C2nxt/std_result/StdResultImpl.h>
#undef T

#define T i64
#include <C2nxt/std_result/StdResultImpl.h>
#undef T

#define T isize
#include <C2nxt/std_result/StdResultImpl.h>
#undef T

#define T f32
#include <C2nxt/std_result/StdResultImpl.h>
#undef T

#define T f64
#include <C2nxt/std_result/StdResultImpl.h>
#undef T

#define T cstring
#include <C2nxt/std_result/StdResultImpl.h>
#undef T

#undef STD_TEMPLATE_IMPL
