/// @file Condvar.c
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief `CnxCondvar` provides a higher-level condition variable type similar to that provided in
/// C++'s `<condition_variable>`
/// @version 0.2.0
/// @date 2022-05-07
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

#include <Cnx/sync/Condvar.h>

#if !___CNX_HAS_NO_THREADS

	#include <Cnx/Assert.h>
	#include <Cnx/sync/UniqueLock.h>
	#include <Cnx/time/Clock.h>

CnxCondvar cnx_condvar_new(void) {
	let_mut maybe_condvar = cnx_basic_condvar_new();
	return (CnxCondvar){
		.__condvar = cnx_result_expect(maybe_condvar,
									   "Failed to create new CnxBasicCondvar in cnx_condvar_new")};
}

	#if CNX_PLATFORM_COMPILER_GCC
_Pragma("GCC diagnostic push") _Pragma("GCC diagnostic ignored \"-Wattributes\"")
	#endif
	__attr(always_inline) __attr(nodiscard) CnxBasicMutex* get_mutex_ptr(CnxMutexInterface mutex) {
	switch(trait_call(type_id, mutex)) {
		case __CNX_MUTEX_ID_MUTEX:
			{
				let_mut mtx = static_cast(CnxMutex*)(mutex.m_self);
				return &(mtx->__mutex);
			}
		case __CNX_MUTEX_ID_RECURSIVE_MUTEX:
			{
				let_mut mtx = static_cast(CnxRecursiveMutex*)(mutex.m_self);
				return &(mtx->__mutex);
			}
		case __CNX_MUTEX_ID_TIMED_MUTEX:
			{
				let_mut mtx = static_cast(CnxTimedMutex*)(mutex.m_self);
				return &(mtx->__mutex.__mutex);
			}
		case __CNX_MUTEX_ID_RECURSIVE_TIMED_MUTEX:
			{
				let_mut mtx = static_cast(CnxRecursiveTimedMutex*)(mutex.m_self);
				return &(mtx->__mutex.__mutex);
			}
		case __CNX_MUTEX_ID_SHARED_MUTEX:
			{
				let_mut mtx = static_cast(CnxSharedMutex*)(mutex.m_self);
				return &(mtx->__mutex.__mutex);
			}
		case __CNX_MUTEX_ID_SHARED_TIMED_MUTEX:
			{
				let_mut mtx = static_cast(CnxSharedTimedMutex*)(mutex.m_self);
				return &(mtx->__mutex.__mutex.__mutex);
			}
	}
	unreachable();
}

	#if CNX_PLATFORM_COMPILER_GCC
_Pragma("GCC diagnostic pop")
	#endif

	void cnx_condvar_wait(CnxCondvar* restrict condvar, struct CnxUniqueLock* restrict lock) {
	if(!cnx_unique_lock_owns_lock(lock)) {
		cnx_panic("cnx_condvar_wait called with a CnxUniqueLock that doesn't own its lock!");
	}

	let_mut mutex = cnx_unique_lock_mutex(lock);
	ignore(cnx_basic_condvar_wait(&(condvar->__condvar), get_mutex_ptr(mutex)));
}

CnxCondvarStatus cnx_condvar_wait_for(CnxCondvar* restrict condvar,
									  struct CnxUniqueLock* restrict lock,
									  CnxDuration duration) {
	let now = cnx_system_clock_now();
	let end = cnx_time_point_add(now, duration);
	return cnx_condvar_wait_until(condvar, lock, end);
}

CnxCondvarStatus cnx_condvar_wait_until(CnxCondvar* restrict condvar,
										struct CnxUniqueLock* restrict lock,
										CnxTimePoint stop_point) {

	let now = cnx_clock_now(stop_point.clock);
	if(cnx_time_point_less_than(stop_point, now)) {
		return CnxCondvarTimeout;
	}

	let_mut mutex = cnx_unique_lock_mutex(lock);
	ignore(cnx_basic_condvar_wait_until(&(condvar->__condvar), get_mutex_ptr(mutex), stop_point));

	return cnx_time_point_less_than(cnx_clock_now(stop_point.clock), stop_point) ?
			   CnxCondvarNoTimeout :
			   CnxCondvarTimeout;
}

void cnx_condvar_notify_one(CnxCondvar* restrict condvar) {
	ignore(cnx_basic_condvar_signal(&(condvar->__condvar)));
}

void cnx_condvar_notify_all(CnxCondvar* restrict condvar) {
	ignore(cnx_basic_condvar_broadcast(&(condvar->__condvar)));
}

void cnx_condvar_free(CnxCondvar* restrict condvar) {
	ignore(cnx_basic_condvar_free(&(condvar->__condvar)));
}

#endif // !___CNX_HAS_NO_THREADSinclude <Cnx/sync/Condvar.h>
