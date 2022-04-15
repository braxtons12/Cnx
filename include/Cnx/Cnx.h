/// @file Cnx.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief Cnx is a GNU C23 library providing type-safe collections and
/// ergonomic features typical in higher-level languages, to C.
/// @version 0.2
/// @date 2022-04-04
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

/// @defgroup collections Collections
/// Cnx Collections provide ergonomic type-generic (where-applicable) type-safe collections with
/// modern features and ergonomics

/// @defgroup views Views
/// Cnx Views provide lazy, immutable, type-safe access to a collections elements in a
/// collection-agnostic fashion

/// @defgroup error_handling Error Handling
/// Cnx provides ergonomic error handling and reporting facilities

/// @defgroup memory Memory
/// Cnx provides abstractions and ergonomics features for dealing with memory allocation,
/// deallocation, and handling custom allocators

/// @defgroup utils Utilities
/// Cnx provides a variety of utilities and utility types such as optional value types,
/// a small selection of metaprogramming macros, a small set of math functions, and
/// semantic-intention-indicating casts

#include <Cnx/Allocators.h>
#include <Cnx/Array.h>
#include <Cnx/Assert.h>
#include <Cnx/Atomic.h>
#include <Cnx/BasicTypes.h>
#include <Cnx/CollectionData.h>
#include <Cnx/Def.h>
#include <Cnx/Enum.h>
#include <Cnx/Error.h>
#include <Cnx/Format.h>
#include <Cnx/IO.h>
#include <Cnx/Iterator.h>
#include <Cnx/Lambda.h>
#include <Cnx/Math.h>
#define OPTION_INCLUDE_DEFAULT_INSTANTATIONS TRUE
#include <Cnx/Option.h>
#undef OPTION_INCLUDE_DEFAULT_INSTANTATIONS
#include <Cnx/Platform.h>
#include <Cnx/Range.h>
#include <Cnx/Ratio.h>
#define RESULT_INCLUDE_DEFAULT_INSTANTIATIONS TRUE
#include <Cnx/Result.h>
#undef RESULT_INCLUDE_DEFAULT_INSTANTIATIONS
#include <Cnx/String.h>
#include <Cnx/StringExt.h>
#include <Cnx/Time.h>
#include <Cnx/Trait.h>
#include <Cnx/Tuple.h>
#include <Cnx/TypeTraits.h>
#define VECTOR_INCLUDE_DEFAULT_INSTANTIATIONS TRUE
#include <Cnx/Vector.h>
#undef VECTOR_INCLUDE_DEFAULT_INSTANTIATIONS
#include <Cnx/UniquePtr.h>
