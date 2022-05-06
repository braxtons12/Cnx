/// @file SharedMutex.c
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief `CnxSharedMutex` provides several higher-level reader-writer mutex types similar to those
/// provided in C++'s `<shared_mutex>`
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
	#include <Cnx/sync/SharedMutex.h>
	#include <Cnx/sync/UniqueLock.h>
	#include <Cnx/time/Clock.h>

static let shared_mutex_exclusive_mask
	= static_cast(u8)(static_cast(u8)(1) << static_cast(u8)(
						  static_cast(u8)(sizeof(u8)) * static_cast(u8)(8) - static_cast(u8)(1)));
static let shared_mutex_shared_mask = static_cast(u8)(~shared_mutex_exclusive_mask);

CnxSharedMutex cnx_shared_mutex_new(void) {
	return (CnxSharedMutex){.__mutex = cnx_mutex_new(),
							.__exclusive_guard = cnx_condvar_new(),
							.__shared_guard = cnx_condvar_new(),
							.__state = 0};
}

void cnx_shared_mutex_lock(CnxSharedMutex* restrict mutex) {
	UniqueLock lock = cnx_unique_lock(mutex->__mutex);

	while((mutex->__state & shared_mutex_exclusive_mask) != 0) {
		cnx_condvar_wait(&(mutex->__exclusive_guard), &lock);
	}

	mutex->__state |= shared_mutex_exclusive_mask;

	while((mutex->__state & shared_mutex_shared_mask) != 0) {
		cnx_condvar_wait(&(mutex->__shared_guard), &lock);
	}
}

bool cnx_shared_mutex_try_lock(CnxSharedMutex* restrict mutex) {
	__attr(maybe_unused) UniqueLock lock = cnx_unique_lock(mutex->__mutex);
	if(mutex->__state == 0) {
		mutex->__state = shared_mutex_exclusive_mask;
		return true;
	}

	return false;
}

void cnx_shared_mutex_unlock(CnxSharedMutex* restrict mutex) {
	__attr(maybe_unused) UniqueLock lock = cnx_unique_lock(mutex->__mutex);
	mutex->__state = 0;
	cnx_condvar_notify_all(&(mutex->__exclusive_guard));
}

void cnx_shared_mutex_lock_shared(CnxSharedMutex* restrict mutex) {
	UniqueLock lock = cnx_unique_lock(mutex->__mutex);
	while((mutex->__state & shared_mutex_exclusive_mask) != 0
		  || (mutex->__state & shared_mutex_shared_mask) == shared_mutex_shared_mask)
	{
		cnx_condvar_wait(&(mutex->__exclusive_guard), &lock);
	}
	let num_readers = static_cast(u8)(static_cast(u8)(mutex->__state & shared_mutex_shared_mask)
									  + static_cast(u8)(1));
	mutex->__state &= static_cast(u8)(~shared_mutex_shared_mask);
	mutex->__state |= num_readers;
}

bool cnx_shared_mutex_try_lock_shared(CnxSharedMutex* restrict mutex) {
	__attr(maybe_unused) UniqueLock lock = cnx_unique_lock(mutex->__mutex);
	let_mut num_readers = static_cast(u8)(mutex->__state & shared_mutex_shared_mask);
	if((mutex->__state & shared_mutex_exclusive_mask) == 0
	   && num_readers != shared_mutex_shared_mask) {
		++num_readers;
		mutex->__state &= static_cast(u8)(~shared_mutex_shared_mask);
		mutex->__state |= num_readers;
		return true;
	}

	return false;
}

void cnx_shared_mutex_unlock_shared(CnxSharedMutex* restrict mutex) {
	__attr(maybe_unused) UniqueLock lock = cnx_unique_lock(mutex->__mutex);
	let_mut num_readers
		= static_cast(u8)(static_cast(u8)(mutex->__state & shared_mutex_shared_mask) - 1);
	mutex->__state &= static_cast(u8)(~shared_mutex_shared_mask);
	mutex->__state |= num_readers;
	if((mutex->__state & shared_mutex_exclusive_mask)) {
		if(num_readers == 0) {
			cnx_condvar_notify_one(&(mutex->__shared_guard));
		}
	}
	else {
		if(num_readers == shared_mutex_shared_mask - 1) {
			cnx_condvar_notify_one(&(mutex->__exclusive_guard));
		}
	}
}

void cnx_shared_mutex_free(CnxSharedMutex* restrict mutex) {
	UniqueLock lock = cnx_unique_lock(mutex->__mutex);
	mutex->__state = 0;
	cnx_condvar_free(&(mutex->__exclusive_guard));
	cnx_condvar_free(&(mutex->__shared_guard));
	cnx_unique_lock_unlock(&lock);
	cnx_mutex_free(&(mutex->__mutex));
}

CnxSharedTimedMutex cnx_shared_timed_mutex_new(void) {
	return (CnxSharedTimedMutex){.__mutex = cnx_shared_mutex_new()};
}

void cnx_shared_timed_mutex_lock(CnxSharedTimedMutex* restrict mutex) {
	cnx_shared_mutex_lock(&(mutex->__mutex));
}

bool cnx_shared_timed_mutex_try_lock(CnxSharedTimedMutex* restrict mutex) {
	return cnx_shared_mutex_try_lock(&(mutex->__mutex));
}

bool cnx_shared_timed_mutex_try_lock_for(CnxSharedTimedMutex* restrict mutex,
										 CnxDuration duration) {
	let now = cnx_system_clock_now();
	let end = cnx_time_point_add(now, duration);
	return cnx_shared_timed_mutex_try_lock_until(mutex, end);
}

bool cnx_shared_timed_mutex_try_lock_until(CnxSharedTimedMutex* restrict mutex,
										   CnxTimePoint stop_point) {
	UniqueLock lock = cnx_unique_lock(mutex->__mutex.__mutex);
	if((mutex->__mutex.__state & shared_mutex_exclusive_mask) != 0) {
		loop {
			let status
				= cnx_condvar_wait_until(&(mutex->__mutex.__exclusive_guard), &lock, stop_point);
			if((mutex->__mutex.__state & shared_mutex_exclusive_mask) == 0) {
				break;
			}
			if(status == CnxCondvarTimeout) {
				return false;
			}
		}
	}
	mutex->__mutex.__state |= shared_mutex_exclusive_mask;
	if(mutex->__mutex.__state & shared_mutex_shared_mask) {
		loop {
			let status
				= cnx_condvar_wait_until(&(mutex->__mutex.__shared_guard), &lock, stop_point);
			if((mutex->__mutex.__state & shared_mutex_shared_mask) == 0) {
				break;
			}
			if(status == CnxCondvarTimeout) {
				mutex->__mutex.__state &= static_cast(u8)(~shared_mutex_exclusive_mask);
				cnx_condvar_notify_all(&(mutex->__mutex.__exclusive_guard));
				return false;
			}
		}
	}

	return true;
}

void cnx_shared_timed_mutex_unlock(CnxSharedTimedMutex* restrict mutex) {
	cnx_shared_mutex_unlock(&(mutex->__mutex));
}

void cnx_shared_timed_mutex_lock_shared(CnxSharedTimedMutex* restrict mutex) {
	cnx_shared_mutex_lock_shared(&(mutex->__mutex));
}

bool cnx_shared_timed_mutex_try_lock_shared(CnxSharedTimedMutex* restrict mutex) {
	return cnx_shared_mutex_try_lock_shared(&(mutex->__mutex));
}

bool cnx_shared_timed_mutex_try_lock_shared_for(CnxSharedTimedMutex* restrict mutex,
												CnxDuration duration) {
	let now = cnx_system_clock_now();
	let end = cnx_time_point_add(now, duration);
	return cnx_shared_timed_mutex_try_lock_shared_until(mutex, end);
}

bool cnx_shared_timed_mutex_try_lock_shared_until(CnxSharedTimedMutex* restrict mutex,
												  CnxTimePoint stop_point) {
	UniqueLock lock = cnx_unique_lock(mutex->__mutex.__mutex);
	if((mutex->__mutex.__state & shared_mutex_exclusive_mask) != 0
	   || (mutex->__mutex.__state & shared_mutex_shared_mask) == shared_mutex_shared_mask)
	{
		loop {
			let status
				= cnx_condvar_wait_until(&(mutex->__mutex.__exclusive_guard), &lock, stop_point);
			if((mutex->__mutex.__state & shared_mutex_exclusive_mask) == 0
			   && (mutex->__mutex.__state & shared_mutex_shared_mask) < shared_mutex_shared_mask)
			{
				break;
			}
			if(status == CnxCondvarTimeout) {
				return false;
			}
		}
	}
	let num_readers
		= static_cast(u8)(static_cast(u8)(mutex->__mutex.__state & shared_mutex_shared_mask) + 1);
	mutex->__mutex.__state &= static_cast(u8)(~shared_mutex_shared_mask);
	mutex->__mutex.__state |= num_readers;
	return true;
}

void cnx_shared_timed_mutex_unlock_shared(CnxSharedTimedMutex* restrict mutex) {
	cnx_shared_mutex_unlock_shared(&(mutex->__mutex));
}

void cnx_shared_timed_mutex_free(CnxSharedTimedMutex* restrict mutex) {
	cnx_shared_mutex_free(&(mutex->__mutex));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////// INTERFACE IMPLEMENTATIONS //////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

void __cnx_shared_mutex_lock_imutex(CnxMutexInterface* restrict mutex) {
	let_mut self = static_cast(CnxSharedMutex*)(mutex->m_self);
	cnx_shared_mutex_lock(self);
}

bool __cnx_shared_mutex_try_lock_imutex(CnxMutexInterface* restrict mutex) {
	let_mut self = static_cast(CnxSharedMutex*)(mutex->m_self);
	return cnx_shared_mutex_try_lock(self);
}

void __cnx_shared_mutex_unlock_imutex(CnxMutexInterface* restrict mutex) {
	let_mut self = static_cast(CnxSharedMutex*)(mutex->m_self);
	cnx_shared_mutex_unlock(self);
}

void __cnx_shared_timed_mutex_lock_imutex(CnxMutexInterface* restrict mutex) {
	let_mut self = static_cast(CnxSharedTimedMutex*)(mutex->m_self);
	cnx_shared_timed_mutex_lock(self);
}

bool __cnx_shared_timed_mutex_try_lock_imutex(CnxMutexInterface* restrict mutex) {
	let_mut self = static_cast(CnxSharedTimedMutex*)(mutex->m_self);
	return cnx_shared_timed_mutex_try_lock(self);
}

bool __cnx_shared_timed_mutex_try_lock_for_imutex(CnxMutexInterface* restrict mutex,
												  CnxDuration duration) {
	let_mut self = static_cast(CnxSharedTimedMutex*)(mutex->m_self);
	return cnx_shared_timed_mutex_try_lock_for(self, duration);
}

bool __cnx_shared_timed_mutex_try_lock_until_imutex(CnxMutexInterface* restrict mutex,
													CnxTimePoint stop_point) {
	let_mut self = static_cast(CnxSharedTimedMutex*)(mutex->m_self);
	return cnx_shared_timed_mutex_try_lock_until(self, stop_point);
}

void __cnx_shared_timed_mutex_unlock_imutex(CnxMutexInterface* restrict mutex) {
	let_mut self = static_cast(CnxSharedTimedMutex*)(mutex->m_self);
	cnx_shared_timed_mutex_unlock(self);
}

/////////////////////////////// SHARED MUTEX INTERFACE IMPLEMENTATIONS /////////////////////////////

void __cnx_shared_mutex_lock_ishared(CnxSharedMutexInterface* restrict mutex) {
	let_mut self = static_cast(CnxSharedMutex*)(mutex->m_self);
	cnx_shared_mutex_lock(self);
}

bool __cnx_shared_mutex_try_lock_ishared(CnxSharedMutexInterface* restrict mutex) {
	let_mut self = static_cast(CnxSharedMutex*)(mutex->m_self);
	return cnx_shared_mutex_try_lock(self);
}

void __cnx_shared_mutex_unlock_ishared(CnxSharedMutexInterface* restrict mutex) {
	let_mut self = static_cast(CnxSharedMutex*)(mutex->m_self);
	cnx_shared_mutex_unlock(self);
}

void __cnx_shared_mutex_lock_shared_ishared(CnxSharedMutexInterface* restrict mutex) {
	let_mut self = static_cast(CnxSharedMutex*)(mutex->m_self);
	cnx_shared_mutex_lock_shared(self);
}

bool __cnx_shared_mutex_try_lock_shared_ishared(CnxSharedMutexInterface* restrict mutex) {
	let_mut self = static_cast(CnxSharedMutex*)(mutex->m_self);
	return cnx_shared_mutex_try_lock_shared(self);
}

void __cnx_shared_mutex_unlock_shared_ishared(CnxSharedMutexInterface* restrict mutex) {
	let_mut self = static_cast(CnxSharedMutex*)(mutex->m_self);
	cnx_shared_mutex_unlock_shared(self);
}

void __cnx_shared_timed_mutex_lock_ishared(CnxSharedMutexInterface* restrict mutex) {
	let_mut self = static_cast(CnxSharedTimedMutex*)(mutex->m_self);
	cnx_shared_timed_mutex_lock(self);
}

bool __cnx_shared_timed_mutex_try_lock_ishared(CnxSharedMutexInterface* restrict mutex) {
	let_mut self = static_cast(CnxSharedTimedMutex*)(mutex->m_self);
	return cnx_shared_timed_mutex_try_lock(self);
}

bool __cnx_shared_timed_mutex_try_lock_for_ishared(CnxSharedMutexInterface* restrict mutex,
												   CnxDuration duration) {
	let_mut self = static_cast(CnxSharedTimedMutex*)(mutex->m_self);
	return cnx_shared_timed_mutex_try_lock_for(self, duration);
}

bool __cnx_shared_timed_mutex_try_lock_until_ishared(CnxSharedMutexInterface* restrict mutex,
													 CnxTimePoint stop_point) {
	let_mut self = static_cast(CnxSharedTimedMutex*)(mutex->m_self);
	return cnx_shared_timed_mutex_try_lock_until(self, stop_point);
}

void __cnx_shared_timed_mutex_unlock_ishared(CnxSharedMutexInterface* restrict mutex) {
	let_mut self = static_cast(CnxSharedTimedMutex*)(mutex->m_self);
	cnx_shared_timed_mutex_unlock(self);
}

void __cnx_shared_timed_mutex_lock_shared_ishared(CnxSharedMutexInterface* restrict mutex) {
	let_mut self = static_cast(CnxSharedTimedMutex*)(mutex->m_self);
	cnx_shared_timed_mutex_lock_shared(self);
}

bool __cnx_shared_timed_mutex_try_lock_shared_ishared(CnxSharedMutexInterface* restrict mutex) {
	let_mut self = static_cast(CnxSharedTimedMutex*)(mutex->m_self);
	return cnx_shared_timed_mutex_try_lock_shared(self);
}

bool __cnx_shared_timed_mutex_try_lock_shared_for_ishared(CnxSharedMutexInterface* restrict mutex,
														  CnxDuration duration) {
	let_mut self = static_cast(CnxSharedTimedMutex*)(mutex->m_self);
	return cnx_shared_timed_mutex_try_lock_shared_for(self, duration);
}

bool __cnx_shared_timed_mutex_try_lock_shared_until_ishared(CnxSharedMutexInterface* restrict mutex,
															CnxTimePoint stop_point) {
	let_mut self = static_cast(CnxSharedTimedMutex*)(mutex->m_self);
	return cnx_shared_timed_mutex_try_lock_shared_until(self, stop_point);
}

void __cnx_shared_timed_mutex_unlock_shared_ishared(CnxSharedMutexInterface* restrict mutex) {
	let_mut self = static_cast(CnxSharedTimedMutex*)(mutex->m_self);
	cnx_shared_timed_mutex_unlock_shared(self);
}

#endif // !___CNX_HAS_NO_THREADS
