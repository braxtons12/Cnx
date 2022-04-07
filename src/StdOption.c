/// @file StdOption.c
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief This module provides a struct template for representing an optional value
/// @version 0.2.3
/// @date 2022-04-06
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

#define OPTION_INCLUDE_DEFAULT_INSTANTIATIONS TRUE

#include <C2nxt/StdOption.h>

#undef OPTION_INCLUDE_DEFAULT_INSTANTIATIONS

#define OPTION_IMPL TRUE

#define OPTION_T char
#include <C2nxt/std_option/StdOptionImpl.h>
#undef OPTION_T

#define OPTION_T u8
// NOLINTNEXTLINE(readability-duplicate-include)
#include <C2nxt/std_option/StdOptionImpl.h>
#undef OPTION_T

#define OPTION_T u16
// NOLINTNEXTLINE(readability-duplicate-include)
#include <C2nxt/std_option/StdOptionImpl.h>
#undef OPTION_T

#define OPTION_T u32
// NOLINTNEXTLINE(readability-duplicate-include)
#include <C2nxt/std_option/StdOptionImpl.h>
#undef OPTION_T

#define OPTION_T u64
// NOLINTNEXTLINE(readability-duplicate-include)
#include <C2nxt/std_option/StdOptionImpl.h>
#undef OPTION_T

#define OPTION_T usize
// NOLINTNEXTLINE(readability-duplicate-include)
#include <C2nxt/std_option/StdOptionImpl.h>
#undef OPTION_T

#define OPTION_T i8
// NOLINTNEXTLINE(readability-duplicate-include)
#include <C2nxt/std_option/StdOptionImpl.h>
#undef OPTION_T

#define OPTION_T i16
// NOLINTNEXTLINE(readability-duplicate-include)
#include <C2nxt/std_option/StdOptionImpl.h>
#undef OPTION_T

#define OPTION_T i32
// NOLINTNEXTLINE(readability-duplicate-include)
#include <C2nxt/std_option/StdOptionImpl.h>
#undef OPTION_T

#define OPTION_T i64
// NOLINTNEXTLINE(readability-duplicate-include)
#include <C2nxt/std_option/StdOptionImpl.h>
#undef OPTION_T

#define OPTION_T isize
// NOLINTNEXTLINE(readability-duplicate-include)
#include <C2nxt/std_option/StdOptionImpl.h>
#undef OPTION_T

#define OPTION_T f32
// NOLINTNEXTLINE(readability-duplicate-include)
#include <C2nxt/std_option/StdOptionImpl.h>
#undef OPTION_T

#define OPTION_T f64
// NOLINTNEXTLINE(readability-duplicate-include)
#include <C2nxt/std_option/StdOptionImpl.h>
#undef OPTION_T

#define OPTION_T cstring
// NOLINTNEXTLINE(readability-duplicate-include)
#include <C2nxt/std_option/StdOptionImpl.h>
#undef OPTION_T

#undef OPTION_IMPL
