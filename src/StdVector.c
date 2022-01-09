/// @file StdVector.c
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief This module provides a dynamic-array type comparable to C++'s `std::vector` and Rust's
/// `std::vec::Vec` for C2nxt
/// @version 0.2
/// @date 2022-01-08
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

#include <C2nxt/StdVector.h>

#define T				  char
#define STD_TEMPLATE_IMPL 1
#include <C2nxt/std_vector/StdVectorImpl.h>
#undef T
#undef STD_TEMPLATE_IMPL

#define T				  u8
#define STD_TEMPLATE_IMPL 1
#include <C2nxt/std_vector/StdVectorImpl.h>
#undef T
#undef STD_TEMPLATE_IMPL

#define T				  u16
#define STD_TEMPLATE_IMPL 1
#include <C2nxt/std_vector/StdVectorImpl.h>
#undef T
#undef STD_TEMPLATE_IMPL

#define T				  u32
#define STD_TEMPLATE_IMPL 1
#include <C2nxt/std_vector/StdVectorImpl.h>
#undef T
#undef STD_TEMPLATE_IMPL

#define T				  u64
#define STD_TEMPLATE_IMPL 1
#include <C2nxt/std_vector/StdVectorImpl.h>
#undef T
#undef STD_TEMPLATE_IMPL

#define T				  usize
#define STD_TEMPLATE_IMPL 1
#include <C2nxt/std_vector/StdVectorImpl.h>
#undef T
#undef STD_TEMPLATE_IMPL

#define T				  i8
#define STD_TEMPLATE_IMPL 1
#include <C2nxt/std_vector/StdVectorImpl.h>
#undef T
#undef STD_TEMPLATE_IMPL

#define T				  i16
#define STD_TEMPLATE_IMPL 1
#include <C2nxt/std_vector/StdVectorImpl.h>
#undef T
#undef STD_TEMPLATE_IMPL

#define T				  i32
#define STD_TEMPLATE_IMPL 1
#include <C2nxt/std_vector/StdVectorImpl.h>
#undef T
#undef STD_TEMPLATE_IMPL

#define T				  i64
#define STD_TEMPLATE_IMPL 1
#include <C2nxt/std_vector/StdVectorImpl.h>
#undef T
#undef STD_TEMPLATE_IMPL

#define T				  isize
#define STD_TEMPLATE_IMPL 1
#include <C2nxt/std_vector/StdVectorImpl.h>
#undef T
#undef STD_TEMPLATE_IMPL

#define T				  f32
#define STD_TEMPLATE_IMPL 1
#include <C2nxt/std_vector/StdVectorImpl.h>
#undef T
#undef STD_TEMPLATE_IMPL

#define T				  f64
#define STD_TEMPLATE_IMPL 1
#include <C2nxt/std_vector/StdVectorImpl.h>
#undef T
#undef STD_TEMPLATE_IMPL

#define T				  u8_ptr
#define STD_TEMPLATE_IMPL 1
#include <C2nxt/std_vector/StdVectorImpl.h>
#undef T
#undef STD_TEMPLATE_IMPL

#define T				  u16_ptr
#define STD_TEMPLATE_IMPL 1
#include <C2nxt/std_vector/StdVectorImpl.h>
#undef T
#undef STD_TEMPLATE_IMPL

#define T				  u32_ptr
#define STD_TEMPLATE_IMPL 1
#include <C2nxt/std_vector/StdVectorImpl.h>
#undef T
#undef STD_TEMPLATE_IMPL

#define T				  u64_ptr
#define STD_TEMPLATE_IMPL 1
#include <C2nxt/std_vector/StdVectorImpl.h>
#undef T
#undef STD_TEMPLATE_IMPL

#define T				  usize_ptr
#define STD_TEMPLATE_IMPL 1
#include <C2nxt/std_vector/StdVectorImpl.h>
#undef T
#undef STD_TEMPLATE_IMPL

#define T				  i8_ptr
#define STD_TEMPLATE_IMPL 1
#include <C2nxt/std_vector/StdVectorImpl.h>
#undef T
#undef STD_TEMPLATE_IMPL

#define T				  i16_ptr
#define STD_TEMPLATE_IMPL 1
#include <C2nxt/std_vector/StdVectorImpl.h>
#undef T
#undef STD_TEMPLATE_IMPL

#define T				  i32_ptr
#define STD_TEMPLATE_IMPL 1
#include <C2nxt/std_vector/StdVectorImpl.h>
#undef T
#undef STD_TEMPLATE_IMPL

#define T				  i64_ptr
#define STD_TEMPLATE_IMPL 1
#include <C2nxt/std_vector/StdVectorImpl.h>
#undef T
#undef STD_TEMPLATE_IMPL

#define T				  isize_ptr
#define STD_TEMPLATE_IMPL 1
#include <C2nxt/std_vector/StdVectorImpl.h>
#undef T
#undef STD_TEMPLATE_IMPL

#define T				  f32_ptr
#define STD_TEMPLATE_IMPL 1
#include <C2nxt/std_vector/StdVectorImpl.h>
#undef T
#undef STD_TEMPLATE_IMPL

#define T				  f64_ptr
#define STD_TEMPLATE_IMPL 1
#include <C2nxt/std_vector/StdVectorImpl.h>
#undef T
#undef STD_TEMPLATE_IMPL

#define T				  cstring
#define STD_TEMPLATE_IMPL 1
#include <C2nxt/std_vector/StdVectorImpl.h>
#undef T
#undef STD_TEMPLATE_IMPL

#define T				  char_ptr
#define STD_TEMPLATE_IMPL 1
#include <C2nxt/std_vector/StdVectorImpl.h>
#undef T
#undef STD_TEMPLATE_IMPL

#define T				  StdString
#define STD_TEMPLATE_IMPL 1
#include <C2nxt/std_vector/StdVectorImpl.h>
#undef T
#undef STD_TEMPLATE_IMPL

#define T				  StdStringView
#define STD_TEMPLATE_IMPL 1
#include <C2nxt/std_vector/StdVectorImpl.h>
#undef T
#undef STD_TEMPLATE_IMPL
