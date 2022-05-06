/// @file SharedLock.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief `CnxSharedlock` provides a shared-ownership scoped mutex locking mechanism similar to
/// C++'s `std::shared_lock`
/// @version 0.2.0
/// @date 2022-05-06
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

#ifndef CNX_SHARED_LOCK
#define CNX_SHARED_LOCK
#include <Cnx/Def.h>
#include <Cnx/sync/SharedMutex.h>

typedef struct {
	IGNORE_RESERVED_IDENTIFIER_WARNING_START
	CnxSharedMutex* __mutex;
	bool __owned;
	IGNORE_RESERVED_IDENTIFIER_WARNING_STOP
} CnxSharedLock;

#define SharedLock scoped(cnx_shared_lock_free)

#endif // CNX_SHARED_LOCK
