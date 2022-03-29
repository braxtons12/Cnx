/// @file StdUniquePtr.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief This module provides a struct template for representing a uniquely owned pointer
/// @version 0.2.0
/// @date 2022-03-28
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

#include <C2nxt/StdDef.h>
#include <C2nxt/std_unique_ptr/StdUniquePtrDef.h>

#if !defined(UNIQUE_DECL) && (!defined(UNIQUE_IMPL) || !UNIQUE_IMPL) && defined(UNIQUE_T)
	#define UNIQUE_DECL TRUE
#endif // !defined(UNIQUE_DECL) && (!defined(UNIQUE_IMPL) || !UNIQUE_IMPL) && defined(UNIQUE_T)

#if !defined(UNIQUE_DELETER) && (UNIQUE_DECL || UNIQUE_IMPL)
	#define UNIQUE_DELETER			   StdUniquePtrIdentifier(UNIQUE_T, default_deleter)
	#define __UNIQUE_DEFAULTED_DELETER TRUE
#endif // !defined(UNIQUE_DELETER) && UNIQUE_DECL || UNIQUE_IMPL

#if !defined(UNIQUE_T) && UNIQUE_DECL
	#error \
		"StdUniquePtr.h included with UNIQUE_DECL defined true but template parameter UNIQUE_T \
			not defined "
#endif // !defined(UNIQUE_T) && UNIQUE_DECL

#if !defined(UNIQUE_T) && UNIQUE_IMPL
	#error \
		"StdUniquePtr.h included with UNIQUE_IMPL defined true but template parameter UNIQUE_T \
			not defined "
#endif // !defined(UNIQUE_T) && UNIQUE_IMPL

#if defined(UNIQUE_T) && UNIQUE_DECL && !STD_TEMPLATE_SUPPRESS_INSTANTIATIONS
	#include <C2nxt/std_unique_ptr/StdUniquePtrDecl.h>
#endif // defined(UNIQUE_T) && UNIQUE_DECL && !STD_TEMPLATE_SUPPRESS_INSTANTIATIONS

#if defined(UNIQUE_T) && UNIQUE_IMPL && !STD_TEMPLATE_SUPPRESS_INSTANTIATIONS
	#include <C2nxt/std_unique_ptr/StdUniquePtrImpl.h>
#endif // defined(UNIQUE_T) && UNIQUE_IMPL && !STD_TEMPLATE_SUPPRESS_INSTANTIATIONS

#if UNIQUE_UNDEF_PARAMS
	#undef UNIQUE_T
	#undef UNIQUE_DELETER
	#undef UNIQUE_DECL
	#undef UNIQUE_IMPL
#endif // UNIQUE_UNDEF_PARAMS

#if __UNIQUE_DEFAULTED_DELETER
	#undef UNIQUE_DELETER
	#undef __UNIQUE_DEFAULTED_DELETER
#endif // __UNIQUE_DEFAULTED_DELETER
