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

typedef struct CnxSharedLock {
	IGNORE_RESERVED_IDENTIFIER_WARNING_START
	CnxSharedMutexInterface __mutex;
	bool __owned;
	IGNORE_RESERVED_IDENTIFIER_WARNING_STOP
} CnxSharedLock;

#define SharedLock scoped(cnx_shared_lock_free)

#define __DISABLE_IF_NULL(lock) \
	cnx_disable_if(!(lock), "Can't perform a CnxSharedLock operation on a nullptr")

#define cnx_shared_lock(...) CONCAT2(__cnx_shared_lock_, PP_NUM_ARGS(__VA_ARGS__))(__VA_ARGS__)

__attr(not_null(1)) void cnx_shared_lock_lock(CnxSharedLock* restrict lock) __DISABLE_IF_NULL(lock);
__attr(nodiscard) __attr(not_null(1)) bool cnx_shared_lock_try_lock(CnxSharedLock* restrict lock)
	__DISABLE_IF_NULL(lock);
__attr(nodiscard)
	__attr(not_null(1)) bool cnx_shared_lock_try_lock_for(CnxSharedLock* restrict lock,
														  CnxDuration duration)
		__DISABLE_IF_NULL(lock) __DISABLE_IF_NULL(lock->__mutex.m_vtable->try_lock_for);
__attr(nodiscard)
	__attr(not_null(1)) bool cnx_shared_lock_try_lock_until(CnxSharedLock* restrict lock,
															CnxTimePoint stop_point)
		__DISABLE_IF_NULL(lock) __DISABLE_IF_NULL(lock->__mutex.m_vtable->try_lock_until);
__attr(not_null(1)) void cnx_shared_lock_unlock(CnxSharedLock* restrict lock)
	__DISABLE_IF_NULL(lock);
__attr(nodiscard) __attr(not_null(1)) CnxSharedMutexInterface
	cnx_shared_lock_mutex(CnxSharedLock* restrict lock) __DISABLE_IF_NULL(lock);
__attr(nodiscard) __attr(not_null(1)) bool cnx_shared_lock_owns_lock(CnxSharedLock* restrict lock)
	__DISABLE_IF_NULL(lock);
__attr(not_null(1)) void cnx_shared_lock_free(void* lock) __DISABLE_IF_NULL(lock);

IGNORE_RESERVED_IDENTIFIER_WARNING_START
#define __cnx_shared_lock_cast(mutex) \
	_Generic((&(mutex)), 																   		   \
				const CnxSharedMutex* 				: as_trait(CnxSharedMutexInterface, 		   \
															   CnxSharedMutex, 					   \
															   mutex),     						   \
				CnxSharedMutex* 						: as_trait(CnxSharedMutexInterface, 	   \
																   CnxSharedMutex, 				   \
																   mutex),     					   \
				const CnxRecursiveMutex* 		: as_trait(CnxSharedMutexInterface, 					   \
														   CnxRecursiveMutex, 					   \
														   mutex), 								   \
				CnxRecursiveMutex* 				: as_trait(CnxSharedMutexInterface, 					   \
														   CnxRecursiveMutex, 					   \
														   mutex), 								   \
				const CnxSharedTimedMutex* 			: as_trait(CnxSharedMutexInterface, 		   \
														   CnxSharedTimedMutex, 				   \
														   mutex), 								   \
				CnxSharedTimedMutex* 					: as_trait(CnxSharedMutexInterface, 	   \
														   CnxSharedTimedMutex, 				   \
														   mutex))

#define __cnx_shared_lock_1(mutex) __cnx_shared_lock(__cnx_shared_lock_cast(mutex))

#define __cnx_shared_lock_2(mutex, lock_type) \
	_Generic((&(lock_type)), 																   \
				const cnx_defer_lock_t* 	: __cnx_shared_lock_deferred, 						   \
				cnx_defer_lock_t* 			: __cnx_shared_lock_deferred, 						   \
				const cnx_try_lock_t* 		: __cnx_shared_lock_try, 							   \
				cnx_try_lock_t* 			: __cnx_shared_lock_try, 							   \
				const cnx_adopt_lock_t* 	: __cnx_shared_lock_adopt, 							   \
				cnx_adopt_lock_t* 			: __cnx_shared_lock_adopt) 							   \
			(__cnx_shared_lock_cast(mutex), lock_type)

__attr(nodiscard) CnxSharedLock __cnx_shared_lock(CnxSharedMutexInterface mutex);
__attr(nodiscard) CnxSharedLock
	__cnx_shared_lock_deferred(CnxSharedMutexInterface mutex, cnx_defer_lock_t defer_lock);
__attr(nodiscard) CnxSharedLock
	__cnx_shared_lock_try(CnxSharedMutexInterface mutex, cnx_try_lock_t try_lock);
__attr(nodiscard) CnxSharedLock
	__cnx_shared_lock_adopt(CnxSharedMutexInterface mutex, cnx_adopt_lock_t adopt_lock);

IGNORE_RESERVED_IDENTIFIER_WARNING_STOP

#undef __DISABLE_IF_NULL

#endif // CNX_SHARED_LOCK
