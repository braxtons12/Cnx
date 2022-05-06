/// @file UniqueLock.c
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief `CnxUniqueLock` provides an exlusive-ownership scoped mutex locking mechanism similar to
/// C++'s `std::unique_lock`
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
	#include <Cnx/sync/UniqueLock.h>

CnxUniqueLock __cnx_unique_lock(CnxMutexInterface mutex) {
	let_mut lock = (CnxUniqueLock){.__mutex = mutex, .__owned = true};
	trait_call(lock, lock.__mutex);
	return lock;
}

CnxUniqueLock __cnx_unique_lock_deferred(CnxMutexInterface mutex,
										 __attr(maybe_unused) cnx_defer_lock_t defer_lock) {
	return (CnxUniqueLock){.__mutex = mutex, .__owned = false};
}

CnxUniqueLock
__cnx_unique_lock_try(CnxMutexInterface mutex, __attr(maybe_unused) cnx_try_lock_t try_lock) {
	let_mut lock = (CnxUniqueLock){.__mutex = mutex, .__owned = false};
	lock.__owned = trait_call(try_lock, lock.__mutex);

	return lock;
}

CnxUniqueLock
__cnx_unique_lock_adopt(CnxMutexInterface mutex, __attr(maybe_unused) cnx_adopt_lock_t adopt_lock) {
	return (CnxUniqueLock){.__mutex = mutex, .__owned = true};
}

void cnx_unique_lock_lock(CnxUniqueLock* restrict lock) {
	if(lock->__owned) {
		cnx_panic(
			"cnx_unique_lock_lock called on a CnxUniqueLock that has already acquired its mutex");
	}
	trait_call(lock, lock->__mutex);
	lock->__owned = true;
}

bool cnx_unique_lock_try_lock(CnxUniqueLock* restrict lock) {
	if(lock->__owned) {
		cnx_panic("cnx_unique_lock_try_lock called on a CnxUniqueLock that has already acquired "
				  "its mutex");
	}
	lock->__owned = trait_call(try_lock, lock->__mutex);
	return lock->__owned;
}

bool cnx_unique_lock_try_lock_for(CnxUniqueLock* restrict lock, CnxDuration duration) {
	if(lock->__owned) {
		cnx_panic(
			"cnx_unique_lock_try_lock_for called on a CnxUniqueLock that has already acquired "
			"its mutex");
	}

	if(lock->__mutex.m_vtable->try_lock_for == nullptr) {
		cnx_panic("cnx_unique_lock_try_lock_for called on a CnxUniqueLock associated with a "
				  "non-timed mutex");
	}

	lock->__owned = trait_call(try_lock_for, lock->__mutex, duration);
	return lock->__owned;
}

bool cnx_unique_lock_try_lock_until(CnxUniqueLock* restrict lock, CnxTimePoint stop_point) {
	if(lock->__owned) {
		cnx_panic(
			"cnx_unique_lock_try_lock_until called on a CnxUniqueLock that has already acquired "
			"its mutex");
	}

	if(lock->__mutex.m_vtable->try_lock_until == nullptr) {
		cnx_panic("cnx_unique_lock_try_lock_until called on a CnxUniqueLock associated with a "
				  "non-timed mutex");
	}

	lock->__owned = trait_call(try_lock_until, lock->__mutex, stop_point);
	return lock->__owned;
}

void cnx_unique_lock_unlock(CnxUniqueLock* restrict lock) {
	if(!lock->__owned) {
		cnx_panic(
			"cnx_unique_lock_unlock called on a CnxUniqueLock that has not acquired its mutex");
	}

	trait_call(unlock, lock->__mutex);
}

CnxMutexInterface cnx_unique_lock_mutex(CnxUniqueLock* restrict lock) {
	return lock->__mutex;
}

bool cnx_unique_lock_owns_lock(CnxUniqueLock* restrict lock) {
	return lock->__owned;
}

void cnx_unique_lock_free(void* lock) {
	IGNORE_RESERVED_IDENTIFIER_WARNING_START
	let_mut __lock = static_cast(CnxUniqueLock*)(lock);
	if(__lock->__owned) {
		trait_call(unlock, __lock->__mutex);
	}
	IGNORE_RESERVED_IDENTIFIER_WARNING_STOP
}

#endif // !___CNX_HAS_NO_THREADS
