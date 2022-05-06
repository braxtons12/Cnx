/// @file Mutex.c
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief `CnxMutex` provides several higher-level mutex types similar to those provided in C++'s
/// `<mutex>`
/// @version 0.2.0
/// @date 2022-05-05
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

	#include <Cnx/sync/Mutex.h>
	#include <Cnx/sync/UniqueLock.h>
	#include <Cnx/time/Clock.h>

CnxMutex cnx_mutex_new(void) {
	let_mut mutex = cnx_basic_mutex_new();
	return (CnxMutex){
		.__mutex
		= cnx_result_expect(mutex, "Failed to create a new CnxBasicMutex in cnx_mutex_new")};
}

void cnx_mutex_lock(CnxMutex* restrict mutex) {
	ignore(cnx_basic_mutex_lock(&(mutex->__mutex)));
}

bool cnx_mutex_try_lock(CnxMutex* restrict mutex) {
	return cnx_basic_mutex_try_lock(&(mutex->__mutex));
}

void cnx_mutex_free(CnxMutex* restrict mutex) {
	ignore(cnx_basic_mutex_free(&(mutex->__mutex)));
}

CnxRecursiveMutex cnx_recursive_mutex_new(void) {
	let_mut mutex = cnx_recursive_basic_mutex_new();
	return (CnxRecursiveMutex){
		.__mutex = cnx_result_expect(
			mutex,
			"Failed to create a new CnxRecursiveBasicMutex in cnx_recursive_mutex_new")};
}

void cnx_recursive_mutex_lock(CnxRecursiveMutex* restrict mutex) {
	ignore(cnx_recursive_basic_mutex_lock(&(mutex->__mutex)));
}

bool cnx_recursive_mutex_try_lock(CnxRecursiveMutex* restrict mutex) {
	return cnx_recursive_basic_mutex_try_lock(&(mutex->__mutex));
}

void cnx_recursive_mutex_unlock(CnxRecursiveMutex* restrict mutex) {
	ignore(cnx_recursive_basic_mutex_unlock(&(mutex->__mutex)));
}

void cnx_recursive_mutex_free(CnxRecursiveMutex* restrict mutex) {
	ignore(cnx_recursive_basic_mutex_free(&(mutex->__mutex)));
}

CnxTimedMutex cnx_timed_mutex_new(void) {
	return (CnxTimedMutex){.__mutex = cnx_mutex_new(),
						   .__timed_guard = cnx_condvar_new(),
						   .__locked = false};
}

void cnx_timed_mutex_lock(CnxTimedMutex* restrict mutex) {
	UniqueLock lock = cnx_unique_lock(mutex->__mutex);
	while(mutex->__locked) {
		cnx_condvar_wait(&(mutex->__timed_guard), &lock);
	}
	mutex->__locked = true;
}

bool cnx_timed_mutex_try_lock(CnxTimedMutex* restrict mutex) {
	UniqueLock lock = cnx_unique_lock(mutex->__mutex, cnx_try_lock);
	if(cnx_unique_lock_owns_lock(&lock) && !mutex->__locked) {
		mutex->__locked = true;
		return true;
	}

	return false;
}

bool cnx_timed_mutex_try_lock_for(CnxTimedMutex* restrict mutex, CnxDuration duration) {
	let now = cnx_system_clock_now();
	let end = cnx_time_point_add(now, duration);
	return cnx_timed_mutex_try_lock_until(mutex, end);
}

bool cnx_timed_mutex_try_lock_until(CnxTimedMutex* restrict mutex, CnxTimePoint stop_point) {
	UniqueLock lock = cnx_unique_lock(mutex->__mutex);
	let_mut not_timedout = cnx_time_point_less_than(cnx_clock_now(stop_point.clock), stop_point);
	while(not_timedout && mutex->__locked) {
		not_timedout = cnx_condvar_wait_until(&(mutex->__timed_guard), &lock, stop_point)
					   == CnxCondvarNoTimeout;
	}

	if(!mutex->__locked) {
		mutex->__locked = true;
		return true;
	}

	return false;
}

void cnx_timed_mutex_unlock(CnxTimedMutex* restrict mutex) {
	__attr(maybe_unused) UniqueLock lock = cnx_unique_lock(mutex->__mutex);
	mutex->__locked = false;
	cnx_condvar_notify_one(&(mutex->__timed_guard));
}

void cnx_timed_mutex_free(CnxTimedMutex* restrict mutex) {
	__attr(maybe_unused) UniqueLock lock = cnx_unique_lock(mutex->__mutex);
	cnx_condvar_free(&(mutex->__timed_guard));
	cnx_unique_lock_unlock(&lock);
	cnx_mutex_free(&(mutex->__mutex));
}

CnxRecursiveTimedMutex cnx_recursive_timed_mutex_new(void) {
	return (CnxRecursiveTimedMutex){.__mutex = cnx_mutex_new(),
									.__timed_guard = cnx_condvar_new(),
									.__locked_thread_id = __CNX_NULL_THREAD,
									.__times_locked = 0};
}

void cnx_recursive_timed_mutex_lock(CnxRecursiveTimedMutex* restrict mutex) {
	let thread_id = cnx_this_thread_get_id();
	UniqueLock lock = cnx_unique_lock(mutex->__mutex);
	if(thread_id == mutex->__locked_thread_id) {
		if(mutex->__times_locked != cnx_max_value(usize)) {
			mutex->__times_locked++;
		}

		return;
	}

	while(mutex->__times_locked != 0) {
		cnx_condvar_wait(&(mutex->__timed_guard), &lock);
	}

	mutex->__times_locked = 1;
	mutex->__locked_thread_id = thread_id;
}

bool cnx_recursive_timed_mutex_try_lock(CnxRecursiveTimedMutex* restrict mutex) {
	let thread_id = cnx_this_thread_get_id();
	__attr(maybe_unused) UniqueLock lock = cnx_unique_lock(mutex->__mutex);
	if(thread_id == mutex->__locked_thread_id) {
		if(mutex->__times_locked == cnx_max_value(usize)) {
			return false;
		}
		else {
			mutex->__times_locked++;
			return true;
		}
	}

	return false;
}

bool cnx_recursive_timed_mutex_try_lock_for(CnxRecursiveTimedMutex* restrict mutex,
											CnxDuration duration) {
	let now = cnx_system_clock_now();
	let end = cnx_time_point_add(now, duration);
	return cnx_recursive_timed_mutex_try_lock_until(mutex, end);
}

bool cnx_recursive_timed_mutex_try_lock_until(CnxRecursiveTimedMutex* restrict mutex,
											  CnxTimePoint stop_point) {
	let thread_id = cnx_this_thread_get_id();
	UniqueLock lock = cnx_unique_lock(mutex->__mutex);
	if(thread_id == mutex->__locked_thread_id) {
		if(mutex->__times_locked == cnx_max_value(usize)) {
			return false;
		}
		else {
			mutex->__times_locked++;
			return true;
		}
	}

	let_mut not_timedout = cnx_time_point_less_than(cnx_clock_now(stop_point.clock), stop_point);
	while(not_timedout && mutex->__times_locked != 0) {
		not_timedout = cnx_condvar_wait_until(&(mutex->__timed_guard), &lock, stop_point)
					   == CnxCondvarNoTimeout;
	}

	if(mutex->__times_locked == 0) {
		mutex->__times_locked = 1;
		mutex->__locked_thread_id = thread_id;
		return true;
	}

	return false;
}

void cnx_recursive_timed_mutex_unlock(CnxRecursiveTimedMutex* restrict mutex) {
	UniqueLock lock = cnx_unique_lock(mutex->__mutex);
	if(--mutex->__times_locked == 0) {
		mutex->__locked_thread_id = __CNX_NULL_THREAD;
		cnx_unique_lock_unlock(&lock);
		cnx_condvar_notify_one(&(mutex->__timed_guard));
	}
}

void cnx_recursive_timed_mutex_free(CnxRecursiveTimedMutex* restrict mutex) {
	UniqueLock lock = cnx_unique_lock(mutex->__mutex);
	cnx_condvar_free(&(mutex->__timed_guard));
	cnx_unique_lock_unlock(&lock);
	cnx_mutex_free(&(mutex->__mutex));
}

void __cnx_mutex_lock(CnxMutexInterface* restrict mutex) {
	let_mut self = static_cast(CnxMutex*)(mutex->m_self);
	cnx_mutex_lock(self);
}

bool __cnx_mutex_try_lock(CnxMutexInterface* restrict mutex) {
	let_mut self = static_cast(CnxMutex*)(mutex->m_self);
	return cnx_mutex_try_lock(self);
}

void __cnx_mutex_unlock(CnxMutexInterface* restrict mutex) {
	let_mut self = static_cast(CnxMutex*)(mutex->m_self);
	cnx_mutex_unlock(self);
}

void __cnx_recursive_mutex_lock(CnxMutexInterface* restrict mutex) {
	let_mut self = static_cast(CnxRecursiveMutex*)(mutex->m_self);
	cnx_recursive_mutex_lock(self);
}

bool __cnx_recursive_mutex_try_lock(CnxMutexInterface* restrict mutex) {
	let_mut self = static_cast(CnxRecursiveMutex*)(mutex->m_self);
	return cnx_recursive_mutex_try_lock(self);
}

void __cnx_recursive_mutex_unlock(CnxMutexInterface* restrict mutex) {
	let_mut self = static_cast(CnxRecursiveMutex*)(mutex->m_self);
	cnx_recursive_mutex_unlock(self);
}

void __cnx_timed_mutex_lock(CnxMutexInterface* restrict mutex) {
	let_mut self = static_cast(CnxTimedMutex*)(mutex->m_self);
	cnx_timed_mutex_lock(self);
}

bool __cnx_timed_mutex_try_lock(CnxMutexInterface* restrict mutex) {
	let_mut self = static_cast(CnxTimedMutex*)(mutex->m_self);
	return cnx_timed_mutex_try_lock(self);
}

bool __cnx_timed_mutex_try_lock_for(CnxMutexInterface* restrict mutex, CnxDuration duration) {
	let_mut self = static_cast(CnxTimedMutex*)(mutex->m_self);
	return cnx_timed_mutex_try_lock_for(self, duration);
}

bool __cnx_timed_mutex_try_lock_until(CnxMutexInterface* restrict mutex, CnxTimePoint stop_point) {
	let_mut self = static_cast(CnxTimedMutex*)(mutex->m_self);
	return cnx_timed_mutex_try_lock_until(self, stop_point);
}

void __cnx_timed_mutex_unlock(CnxMutexInterface* restrict mutex) {
	let_mut self = static_cast(CnxTimedMutex*)(mutex->m_self);
	cnx_timed_mutex_unlock(self);
}

void __cnx_recursive_timed_mutex_lock(CnxMutexInterface* restrict mutex) {
	let_mut self = static_cast(CnxRecursiveTimedMutex*)(mutex->m_self);
	cnx_recursive_timed_mutex_lock(self);
}

bool __cnx_recursive_timed_mutex_try_lock(CnxMutexInterface* restrict mutex) {
	let_mut self = static_cast(CnxRecursiveTimedMutex*)(mutex->m_self);
	return cnx_recursive_timed_mutex_try_lock(self);
}

bool __cnx_recursive_timed_mutex_try_lock_for(CnxMutexInterface* restrict mutex,
											  CnxDuration duration) {
	let_mut self = static_cast(CnxRecursiveTimedMutex*)(mutex->m_self);
	return cnx_recursive_timed_mutex_try_lock_for(self, duration);
}

bool __cnx_recursive_timed_mutex_try_lock_until(CnxMutexInterface* restrict mutex,
												CnxTimePoint stop_point) {
	let_mut self = static_cast(CnxRecursiveTimedMutex*)(mutex->m_self);
	return cnx_recursive_timed_mutex_try_lock_until(self, stop_point);
}

void __cnx_recursive_timed_mutex_unlock(CnxMutexInterface* restrict mutex) {
	let_mut self = static_cast(CnxRecursiveTimedMutex*)(mutex->m_self);
	cnx_recursive_timed_mutex_unlock(self);
}

#endif // !___CNX_HAS_NO_THREADS
