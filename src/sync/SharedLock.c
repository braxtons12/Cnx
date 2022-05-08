/// @file SharedLock.c
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

#include <Cnx/__thread/__thread.h>

#if !___CNX_HAS_NO_THREADS

	#include <Cnx/Assert.h>
	#include <Cnx/sync/SharedLock.h>

CnxSharedLock __cnx_shared_lock(CnxSharedMutexInterface mutex) {
	let_mut lock = (CnxSharedLock){.__mutex = mutex, .__owned = true};
	trait_call(lock, lock.__mutex);
	return lock;
}

CnxSharedLock __cnx_shared_lock_deferred(CnxSharedMutexInterface mutex,
										 __attr(maybe_unused) cnx_defer_lock_t defer_lock) {
	return (CnxSharedLock){.__mutex = mutex, .__owned = false};
}

CnxSharedLock
__cnx_shared_lock_try(CnxSharedMutexInterface mutex, __attr(maybe_unused) cnx_try_lock_t try_lock) {
	let_mut lock = (CnxSharedLock){.__mutex = mutex, .__owned = false};
	lock.__owned = trait_call(try_lock_shared, lock.__mutex);

	return lock;
}

CnxSharedLock __cnx_shared_lock_adopt(CnxSharedMutexInterface mutex,
									  __attr(maybe_unused) cnx_adopt_lock_t adopt_lock) {
	return (CnxSharedLock){.__mutex = mutex, .__owned = true};
}

void cnx_shared_lock_lock(CnxSharedLock* restrict lock) {
	if(lock->__owned) {
		cnx_panic(
			"cnx_shared_lock_lock called on a CnxSharedLock that has already acquired its mutex");
	}

	cnx_assert(lock->__mutex.m_self != nullptr,
			   "cnx_shared_lock_lock called on a CnxSharedLock that has been moved out of");
	trait_call(lock_shared, lock->__mutex);
	lock->__owned = true;
}

bool cnx_shared_lock_try_lock(CnxSharedLock* restrict lock) {
	if(lock->__owned) {
		cnx_panic("cnx_shared_lock_try_lock called on a CnxSharedLock that has already acquired "
				  "its mutex");
	}

	cnx_assert(lock->__mutex.m_self != nullptr,
			   "cnx_shared_lock_try_lock called on a CnxSharedLock that has been moved out of");
	lock->__owned = trait_call(try_lock_shared, lock->__mutex);
	return lock->__owned;
}

bool cnx_shared_lock_try_lock_for(CnxSharedLock* restrict lock, CnxDuration duration) {
	if(lock->__owned) {
		cnx_panic(
			"cnx_shared_lock_try_lock_for called on a CnxSharedLock that has already acquired "
			"its mutex");
	}

	if(lock->__mutex.m_vtable->try_lock_shared_for == nullptr) {
		cnx_panic("cnx_shared_lock_try_lock_for called on a CnxSharedLock associated with a "
				  "non-timed mutex");
	}

	cnx_assert(lock->__mutex.m_self != nullptr,
			   "cnx_shared_lock_try_lock_for called on a CnxSharedLock that has been moved out of");
	lock->__owned = trait_call(try_lock_shared_for, lock->__mutex, duration);
	return lock->__owned;
}

bool cnx_shared_lock_try_lock_until(CnxSharedLock* restrict lock, CnxTimePoint stop_point) {
	if(lock->__owned) {
		cnx_panic(
			"cnx_shared_lock_try_lock_until called on a CnxSharedLock that has already acquired "
			"its mutex");
	}

	if(lock->__mutex.m_vtable->try_lock_shared_until == nullptr) {
		cnx_panic("cnx_shared_lock_try_lock_until called on a CnxSharedLock associated with a "
				  "non-timed mutex");
	}

	cnx_assert(
		lock->__mutex.m_self != nullptr,
		"cnx_shared_lock_try_lock_until called on a CnxSharedLock that has been moved out of");
	lock->__owned = trait_call(try_lock_shared_until, lock->__mutex, stop_point);
	return lock->__owned;
}

void cnx_shared_lock_unlock(CnxSharedLock* restrict lock) {
	if(!lock->__owned) {
		cnx_panic(
			"cnx_shared_lock_unlock called on a CnxSharedLock that has not acquired its mutex");
	}

	cnx_assert(lock->__mutex.m_self != nullptr,
			   "cnx_shared_lock_unlock called on a CnxSharedLock that has been moved out of");
	trait_call(unlock_shared, lock->__mutex);
}

CnxSharedMutexInterface cnx_shared_lock_mutex(CnxSharedLock* restrict lock) {
	return lock->__mutex;
}

bool cnx_shared_lock_owns_lock(CnxSharedLock* restrict lock) {
	return lock->__owned;
}

void cnx_shared_lock_free(void* lock) {
	IGNORE_RESERVED_IDENTIFIER_WARNING_START
	let_mut __lock = static_cast(CnxSharedLock*)(lock);
	if(__lock->__owned && __lock->__mutex.m_self != nullptr) {
		trait_call(unlock_shared, __lock->__mutex);
	}
	IGNORE_RESERVED_IDENTIFIER_WARNING_STOP
}

#endif // !___CNX_HAS_NO_THREADS
