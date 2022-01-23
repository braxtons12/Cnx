/// @file C2nxt.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief C2nxt is a GNU C11 library providing type-safe collections and
/// ergonomic features typical in higher-level languages, to C.
/// @version 0.1
/// @date 2022-01-23
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
/// C2nxt Collections provide ergonomic type-generic (where-applicable) type-safe collections with
/// modern features and ergonomics

/// @defgroup views Views
/// C2nxt Views provide lazy, immutable, type-safe access to a collections elements in a
/// collection-agnostic fashion

/// @defgroup error_handling Error Handling
/// C2nxt provides ergonomic error handling and reporting facilities

/// @defgroup memory Memory
/// C2nxt provides abstractions and ergonomics features for dealing with memory allocation,
/// deallocation, and handling custom allocators

/// @defgroup utils Utilities
/// C2nxt provides a variety of utilities and utility types such as optional value types,
/// a small selection of metaprogramming macros, a small set of math functions, and
/// semantic-intention-indicating casts

#include "StdAllocators.h"
#include "StdArray.h"
#include "StdAssert.h"
#include "StdBasicTypes.h"
#include "StdCollectionsData.h"
#include "StdDef.h"
#include "StdError.h"
#include "StdFormat.h"
#include "StdIO.h"
#include "StdIterator.h"
#include "StdLambda.h"
#include "StdMath.h"
#include "StdOption.h"
#include "StdRange.h"
#include "StdResult.h"
#include "StdString.h"
#include "StdTime.h"
#include "StdTrait.h"
#include "StdTypeTraits.h"
#include "StdVector.h"
#include "StdEnum.h"
