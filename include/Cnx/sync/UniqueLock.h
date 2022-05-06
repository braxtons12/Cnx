/// @file UniqueLock.h
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

#ifndef CNX_UNIQUE_LOCK
#define CNX_UNIQUE_LOCK

#include <Cnx/Def.h>
#include <Cnx/Thread.h>
#include <Cnx/sync/Mutex.h>
#include <Cnx/sync/SharedMutex.h>

typedef struct CnxUniqueLock {
	IGNORE_RESERVED_IDENTIFIER_WARNING_START
	CnxMutexInterface __mutex;
	bool __owned;
	IGNORE_RESERVED_IDENTIFIER_WARNING_STOP
} CnxUniqueLock;

#define UniqueLock scoped(cnx_unique_lock_free)

#define __DISABLE_IF_NULL(lock) \
	cnx_disable_if(!(lock), "Can't perform a CnxUniqueLock operation on a nullptr")

#define cnx_unique_lock(...) CONCAT2(__cnx_unique_lock_, PP_NUM_ARGS(__VA_ARGS__))(__VA_ARGS__)

__attr(not_null(1)) void cnx_unique_lock_lock(CnxUniqueLock* restrict lock) __DISABLE_IF_NULL(lock);
__attr(nodiscard) __attr(not_null(1)) bool cnx_unique_lock_try_lock(CnxUniqueLock* restrict lock)
	__DISABLE_IF_NULL(lock);
__attr(nodiscard)
	__attr(not_null(1)) bool cnx_unique_lock_try_lock_for(CnxUniqueLock* restrict lock,
														  CnxDuration duration)
		__DISABLE_IF_NULL(lock) __DISABLE_IF_NULL(lock->__mutex.m_vtable->try_lock_for);
__attr(nodiscard)
	__attr(not_null(1)) bool cnx_unique_lock_try_lock_until(CnxUniqueLock* restrict lock,
															CnxTimePoint stop_point)
		__DISABLE_IF_NULL(lock) __DISABLE_IF_NULL(lock->__mutex.m_vtable->try_lock_until);
__attr(not_null(1)) void cnx_unique_lock_unlock(CnxUniqueLock* restrict lock)
	__DISABLE_IF_NULL(lock);
__attr(nodiscard) __attr(not_null(1)) CnxMutexInterface
	cnx_unique_lock_mutex(CnxUniqueLock* restrict lock) __DISABLE_IF_NULL(lock);
__attr(nodiscard) __attr(not_null(1)) bool cnx_unique_lock_owns_lock(CnxUniqueLock* restrict lock)
	__DISABLE_IF_NULL(lock);
__attr(not_null(1)) void cnx_unique_lock_free(void* lock) __DISABLE_IF_NULL(lock);

IGNORE_RESERVED_IDENTIFIER_WARNING_START
#define __cnx_unique_lock_cast(mutex) \
	_Generic((&(mutex)), 																   \
				const CnxMutex* 				: as_trait(CnxMutexInterface, CnxMutex, mutex),    \
				CnxMutex* 						: as_trait(CnxMutexInterface, CnxMutex, mutex),    \
				const CnxRecursiveMutex* 		: as_trait(CnxMutexInterface, 					   \
														   CnxRecursiveMutex, 					   \
														   mutex), 								   \
				CnxRecursiveMutex* 				: as_trait(CnxMutexInterface, 					   \
														   CnxRecursiveMutex, 					   \
														   mutex), 								   \
				const CnxTimedMutex* 			: as_trait(CnxMutexInterface, 					   \
														   CnxTimedMutex, 						   \
														   mutex), 								   \
				CnxTimedMutex* 					: as_trait(CnxMutexInterface, 					   \
														   CnxTimedMutex, 						   \
														   mutex), 								   \
				const CnxRecursiveTimedMutex* 	: as_trait(CnxMutexInterface, 					   \
														   CnxRecursiveTimedMutex, 				   \
														   mutex), 								   \
				CnxRecursiveTimedMutex* 		: as_trait(CnxMutexInterface, 					   \
														   CnxRecursiveTimedMutex, 				   \
														   mutex), 								   \
				const CnxSharedMutex* 			: (as_trait(CnxMutexInterface, 					   \
														   CnxSharedMutex, 						   \
														   mutex)), 							   \
				CnxSharedMutex* 				: (as_trait(CnxMutexInterface, 					   \
														   CnxSharedMutex, 						   \
														   mutex)), 							   \
				const CnxSharedTimedMutex* 		: as_trait(CnxMutexInterface, 					   \
														   CnxSharedTimedMutex, 				   \
														   mutex), 								   \
				CnxSharedTimedMutex* 			: as_trait(CnxMutexInterface, 					   \
														   CnxSharedTimedMutex, 				   \
														   mutex))

#define __cnx_unique_lock_1(mutex) __cnx_unique_lock(__cnx_unique_lock_cast(mutex))

#define __cnx_unique_lock_2(mutex, lock_type) \
	_Generic((&(lock_type)), 																   \
				const cnx_defer_lock_t* 	: __cnx_unique_lock_deferred, 						   \
				cnx_defer_lock_t* 			: __cnx_unique_lock_deferred, 						   \
				const cnx_try_lock_t* 		: __cnx_unique_lock_try, 							   \
				cnx_try_lock_t* 			: __cnx_unique_lock_try, 							   \
				const cnx_adopt_lock_t* 	: __cnx_unique_lock_adopt, 							   \
				cnx_adopt_lock_t* 			: __cnx_unique_lock_adopt) 							   \
			(__cnx_unique_lock_cast(mutex), lock_type)

__attr(nodiscard) CnxUniqueLock __cnx_unique_lock(CnxMutexInterface mutex);
__attr(nodiscard) CnxUniqueLock
	__cnx_unique_lock_deferred(CnxMutexInterface mutex, cnx_defer_lock_t defer_lock);
__attr(nodiscard) CnxUniqueLock
	__cnx_unique_lock_try(CnxMutexInterface mutex, cnx_try_lock_t try_lock);
__attr(nodiscard) CnxUniqueLock
	__cnx_unique_lock_adopt(CnxMutexInterface mutex, cnx_adopt_lock_t adopt_lock);

IGNORE_RESERVED_IDENTIFIER_WARNING_STOP

#undef __DISABLE_IF_NULL

#endif // CNX_UNIQUE_LOCK
