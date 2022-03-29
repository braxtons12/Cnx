/// @file StdSmartPtrDef.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief This module provides the macro definitions for working with smart pointer templates
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

#define ptr(self) ((self).m_ptr)

#define __SMART_PTR_CHECK_INSTANCE_IS_ARRAY(self) \
	(!(std_types_equal_v(typeof(*((self).m_ptr)), (typeof(*((self).m_type))){0})))
#define __SMART_PTR_IS_ARRAY(T)				  (!(std_types_equal_v(typeof((T){0}), *(&(T){0}))))
#define __SMART_PTR_ELEMENT_PTR_BASE(T)		  ((T){0}, (T){1})
#define __SMART_PTR_CONST_ELEMENT_PTR_BASE(T) ((const T){0}, (const T){1})

// clang-format off
#define __SMART_PTR_ELEMENT_PTR(T) 											   \
			typeof(_Generic(((T){0}),								           \
				typeof(*(&(T){0})) 	: &(T){0},    			                   \
				default					  	: __SMART_PTR_ELEMENT_PTR_BASE(T)) \
			)
#define __SMART_PTR_CONST_ELEMENT_PTR(T) 									         \
			typeof(_Generic(((T){0}),								                 \
				typeof(*(&(T){0})) 	: &(const T){0},    			                 \
				default					  	: __SMART_PTR_CONST_ELEMENT_PTR_BASE(T)) \
			)
// clang-format on

#define __SMART_PTR_ELEMENT(T) typeof(*((__SMART_PTR_ELEMENT_PTR(T)){0}))

#define StdDeleter(T, ...) \
	void (*__VA_ARGS__)(__SMART_PTR_ELEMENT_PTR(T) restrict self, StdAllocator allocator)
