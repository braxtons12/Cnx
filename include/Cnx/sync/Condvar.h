/// @file Condvar.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief `CnxCondvar` provides a higher-level condition variable type similar to that provided in
/// C++'s `<condition_variable>`
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

#include <Cnx/__thread/__thread.h>

#if !___CNX_HAS_NO_THREADS

	#ifndef CNX_CONDVAR
		#define CNX_CONDVAR

		#include <Cnx/Def.h>
		#include <Cnx/Thread.h>
		#include <Cnx/time/TimePoint.h>

typedef struct {
	IGNORE_RESERVED_IDENTIFIER_WARNING_START
	CnxBasicCondvar __condvar;
	IGNORE_RESERVED_IDENTIFIER_WARNING_STOP
} CnxCondvar;

typedef enum {
	CnxCondvarNoTimeout = 0,
	CnxCondvarTimeout
} CnxCondvarStatus;

struct CnxUniqueLock;

		#define __DISABLE_IF_NULL(val) \
			cnx_disable_if(!(val), "Can't perform a CnxCondvar operation with a nullptr")

__attr(nodiscard) CnxCondvar cnx_condvar_new(void);
__attr(not_null(1, 2)) void cnx_condvar_wait(CnxCondvar* restrict condvar,
											 struct CnxUniqueLock* restrict lock)
	__DISABLE_IF_NULL(condvar) __DISABLE_IF_NULL(lock);
__attr(nodiscard) __attr(not_null(1, 2)) CnxCondvarStatus
	cnx_condvar_wait_for(CnxCondvar* restrict condvar,
						 struct CnxUniqueLock* restrict lock,
						 CnxDuration duration) __DISABLE_IF_NULL(condvar) __DISABLE_IF_NULL(lock);
__attr(nodiscard) __attr(not_null(1, 2)) CnxCondvarStatus
	cnx_condvar_wait_until(CnxCondvar* restrict condvar,
						   struct CnxUniqueLock* restrict lock,
						   CnxTimePoint stop_point) __DISABLE_IF_NULL(condvar)
		__DISABLE_IF_NULL(lock);
__attr(not_null(1)) void cnx_condvar_notify_one(CnxCondvar* restrict condvar)
	__DISABLE_IF_NULL(condvar);
__attr(not_null(1)) void cnx_condvar_notify_all(CnxCondvar* restrict condvar)
	__DISABLE_IF_NULL(condvar);
__attr(not_null(1)) void cnx_condvar_free(CnxCondvar* restrict condvar) __DISABLE_IF_NULL(condvar);

		#undef __DISABLE_IF_NULL

	#endif // CNX_CONDVAR

#endif //!___CNX_HAS_NO_THREADS
