/// @file StdVector.c
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief This module provides a dynamic-array type comparable to C++'s `std::vector` and Rust's
/// `std::vec::Vec` for C2nxt
/// @version 0.2.1
/// @date 2022-02-24
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

#define STD_TEMPLATE_INCLUDE_DEFAULT_INSTANTIATIONS TRUE

#include <C2nxt/StdVector.h>

#undef STD_TEMPLATE_INCLUDE_DEFAULT_INSTANTIATIONS

#define STD_TEMPLATE_IMPL  TRUE
#define SMALL_OPT_CAPACITY 8

#define T char
#include <C2nxt/std_vector/StdVectorImpl.h>
#undef T

#define T u8
#include <C2nxt/std_vector/StdVectorImpl.h>
#undef T

#define T u16
#include <C2nxt/std_vector/StdVectorImpl.h>
#undef T

#define T u32
#include <C2nxt/std_vector/StdVectorImpl.h>
#undef T

#define T u64
#include <C2nxt/std_vector/StdVectorImpl.h>
#undef T

#define T usize
#include <C2nxt/std_vector/StdVectorImpl.h>
#undef T

#define T i8
#include <C2nxt/std_vector/StdVectorImpl.h>
#undef T

#define T i16
#include <C2nxt/std_vector/StdVectorImpl.h>
#undef T

#define T i32
#include <C2nxt/std_vector/StdVectorImpl.h>
#undef T

#define T i64
#include <C2nxt/std_vector/StdVectorImpl.h>
#undef T

#define T isize
#include <C2nxt/std_vector/StdVectorImpl.h>
#undef T

#define T f32
#include <C2nxt/std_vector/StdVectorImpl.h>
#undef T

#define T f64
#include <C2nxt/std_vector/StdVectorImpl.h>
#undef T

#define T cstring
#include <C2nxt/std_vector/StdVectorImpl.h>
#undef T

#define T StdString
#include <C2nxt/std_vector/StdVectorImpl.h>
#undef T

#define T StdStringView
#include <C2nxt/std_vector/StdVectorImpl.h>
#undef T

#undef SMALL_OPT_CAPACITY
#undef STD_TEMPLATE_IMPL
