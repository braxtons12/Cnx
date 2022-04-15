#include <Cnx/Platform.h>

#if CNX_PLATFORM_WINDOWS && !CNX_PLATFORM_COMPILER_GCC

	#include <Cnx/Allocators.h>
	#include <Cnx/atomic/AtomicImpl.h>
	#include <intrin.h>
	#include <intrin0.h>

	#if CNX_PLATFORM_COMPILER_CLANG
		#define IGNORE_DEPRECATED_WARNING_START \
			_Pragma("GCC diagnostic push")      \
				_Pragma("GCC diagnostic ignored \"-Wdeprecated-declarations\"")
		#define IGNORE_DEPRECATED_WARNING_STOP _Pragma("GCC diagnostic pop")
	#else
		#define IGNORE_SEQ_CST_WARNING_START
		#define IGNORE_SEQ_CST_WARNING_STOP
	#endif // CNX_PLATFORM_COMPILER_CLANG

	#define __compiler_barrier() \
		IGNORE_DEPRECATED_WARNING_START _ReadWriteBarrier() IGNORE_DEPRECATED_WARNING_STOP

	#define __IS_X86                                    \
		(IS_CNX_PLATFORM_ARCHITECTURE(CNX_PLATFORM_X86) \
		 || IS_CNX_PLATFORM_ARCHITECTURE(CNX_PLATFORM_X86_64))

	#define __IS_ARM IS_CNX_PLATFORM_ARCHITECTURE(CNX_PLATFORM_ARM64)

	#if __IS_X86

		#define __memory_barrier()			   __faststorefence()
		#define __compiler_or_memory_barrier() __memory_barrier()
		#define __CHOOSE_INTRINSIC(order, result, base_intrinsic_name, ...) \
			({                                                              \
				result = base_intrinsic_name(__VA_ARGS__);                  \
				result;                                                     \
			})
	#elif __IS_ARM

		#define __memory_barrier()			   __dmb(0xB)
		#define __compiler_or_memory_barrier() __compiler_barrier()
		#define __CHOOSE_INTRINSIC(order, result, base_intrinsic_name, ...)                      \
			({                                                                                   \
				switch(order) { /** NOLINT(hicpp-multiway-paths-covered) **/                     \
					case memory_order_relaxed:                                                   \
						result = CONCAT2(base_intrinsic_name, _nf)(__VA_ARGS__);                 \
						break;                                                                   \
					case memory_order_consume:                                                   \
					case memory_order_acquire:                                                   \
						result = CONCAT(base_intrinisic_name, _acq)(__VA_ARGS__);                \
						break;                                                                   \
					case memory_order_release:                                                   \
						result = CONCAT(base_intrinsic_name, _rel)(__VA_ARGS__);                 \
						break;                                                                   \
					case memory_order_acq_rel:                                                   \
					case memory_order_seq_cst: result = base_intrinsic_name(__VA_ARGS__); break; \
				}                                                                                \
				result;                                                                          \
			})

	#else

		#error Unsupported hardware

	#endif

IGNORE_RESERVED_IDENTIFIER_WARNING_START
	#define _InterlockedExchangeAdd32(...)	   _InterlockedExchangeAdd(__VA_ARGS__)
	#define _InterlockedOr32(...)			   _InterlockedOr(__VA_ARGS__)
	#define _InterlockedXor32(...)			   _InterlockedXor(__VA_ARGS__)
	#define _InterlockedAnd32(...)			   _InterlockedAnd(__VA_ARGS__)
	#define _InterlockedExchange32(...)		   _InterlockedExchange(__VA_ARGS__)
	#define _InterlockedCompareExchange32(...) _InterlockedCompareExchange(__VA_ARGS__)

[[always_inline]] static inline void __load_barrier(const memory_order order) {
	if((order) != memory_order_relaxed) {
		__compiler_or_memory_barrier();
	}
}
IGNORE_RESERVED_IDENTIFIER_WARNING_STOP

	#if CNX_PLATFORM_COMPILER_CLANG
		#define IGNORE_SEQ_CST_WARNING_START \
			_Pragma("GCC diagnostic push")   \
				_Pragma("GCC diagnostic ignored \"-Watomic-implicit-seq-cst\"")
		#define IGNORE_SEQ_CST_WARNING_STOP _Pragma("GCC diagnostic pop")
	#else
		#define IGNORE_SEQ_CST_WARNING_START
		#define IGNORE_SEQ_CST_WARNING_STOP
	#endif // CNX_PLATFORM_COMPILER_CLANG

void __atomic_store_explicit8(volatile atomic_i8* object,
							  // NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
							  i8 value,
							  [[maybe_unused]] memory_order order) {
	let_mut mem = static_cast(volatile char*)(object);
	switch(order) { // NOLINT(hicpp-multiway-paths-covered)
		case memory_order_relaxed: __iso_volatile_store8(mem, value); return;
		case memory_order_release:
			__compiler_or_memory_barrier();
			__iso_volatile_store8(mem, value);
			return;
		case memory_order_consume:
		case memory_order_acquire:
		case memory_order_acq_rel:
		case memory_order_seq_cst: __atomic_store8(object, value); return;
	}
}

void __atomic_store_explicit16(volatile atomic_i16* object,
							   // NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
							   i16 value,
							   [[maybe_unused]] memory_order order) {
	switch(order) { // NOLINT(hicpp-multiway-paths-covered)
		case memory_order_relaxed: __iso_volatile_store16(object, value); return;
		case memory_order_release:
			__compiler_or_memory_barrier();
			__iso_volatile_store16(object, value);
			return;
		case memory_order_consume:
		case memory_order_acquire:
		case memory_order_acq_rel:
		case memory_order_seq_cst: __atomic_store16(object, value); return;
	}
}

void __atomic_store_explicit32(volatile atomic_i32* object,
							   // NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
							   i32 value,
							   [[maybe_unused]] memory_order order) {
	switch(order) { // NOLINT(hicpp-multiway-paths-covered)
		case memory_order_relaxed: __iso_volatile_store32(object, value); return;
		case memory_order_release:
			__compiler_or_memory_barrier();
			__iso_volatile_store32(object, value);
			return;
		case memory_order_consume:
		case memory_order_acquire:
		case memory_order_acq_rel:
		case memory_order_seq_cst: __atomic_store32(object, value); return;
	}
}

void __atomic_store_explicit64(volatile atomic_i64* object,
							   // NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
							   i64 value,
							   [[maybe_unused]] memory_order order) {
	switch(order) { // NOLINT(hicpp-multiway-paths-covered)
		case memory_order_relaxed: __iso_volatile_store64(object, value); return;
		case memory_order_release:
			__compiler_or_memory_barrier();
			__iso_volatile_store64(object, value);
			return;
		case memory_order_consume:
		case memory_order_acquire:
		case memory_order_acq_rel:
		case memory_order_seq_cst: __atomic_store64(object, value); return;
	}
}

void __atomic_store8(volatile atomic_i8* object, i8 value) {
	#if __IS_X86
	ignore(_InterlockedExchange8(static_cast(volatile char*)(object), value));
	#else
	__memory_barrier();
	__iso_volatile_store8(static_cast(volatile char*)(object), value);
	__memory_barrier();
	#endif // __IS_X86
}

void __atomic_store16(volatile atomic_i16* object, i16 value) {
	#if __IS_X86
	ignore(_InterlockedExchange16(object, value));
	#else
	__memory_barrier();
	__iso_volatile_store16(object, value);
	__memory_barrier();
	#endif // __IS_X86
}

void __atomic_store32(volatile atomic_i32* object, i32 value) {
	#if __IS_X86
	ignore(_InterlockedExchange32(static_cast(volatile long*)(object), value));
	#else
	__memory_barrier();
	__iso_volatile_store32(static_cast(volatile int*)(object), value);
	__memory_barrier();
	#endif // __IS_X86
}

void __atomic_store64(volatile atomic_i64* object, i64 value) {
	#if __IS_X86 && !IS_CNX_PLATFORM_ARCHITECTURE(CNX_PLATFORM_X86_64) // is 32bit x86
	__compiler_barrier();
	__iso_volatile_store64(object, value);
	__atomic_thread_fence(memory_order_seq_cst);
	#elif __IS_X86
	ignore(_InterlockedExchange64(object, value));
	#else  // then is ARM
	__memory_barrier();
	__iso_volatile_store8(object, value);
	__memory_barrier();
	#endif // __IS_X86 && !IS_CNX_PLATFORM_ARCHITECTURE(CNX_PLATFORM_X86_64)
}

i8 __atomic_load_explicit8(volatile atomic_i8* object, [[maybe_unused]] memory_order order) {
	let ret = __iso_volatile_load8(static_cast(volatile char*)(object));
	__load_barrier(order);
	return (static_cast(i8)(ret));
}

i16 __atomic_load_explicit16(volatile atomic_i16* object, [[maybe_unused]] memory_order order) {
	let ret = __iso_volatile_load16(object);
	__load_barrier(order);
	return (static_cast(i16)(ret));
}

i32 __atomic_load_explicit32(volatile atomic_i32* object, [[maybe_unused]] memory_order order) {
	let ret = __iso_volatile_load32(object);
	__load_barrier(order);
	return (static_cast(i32)(ret));
}

i64 __atomic_load_explicit64(volatile atomic_i64* object, [[maybe_unused]] memory_order order) {
	#if __IS_X86
	let ret = __iso_volatile_load64(object);
	#else
	let ret = __ldrexd(object);
	#endif // __IS_X86
	__load_barrier(order);
	return (static_cast(i64)(ret));
}

i8 __atomic_load8(volatile atomic_i8* object) {
	let ret = __iso_volatile_load8(static_cast(volatile char*)(object));
	__compiler_or_memory_barrier();
	return (static_cast(i8)(ret));
}

i16 __atomic_load16(volatile atomic_i16* object) {
	let ret = __iso_volatile_load16(object);
	__compiler_or_memory_barrier();
	return (static_cast(i16)(ret));
}

i32 __atomic_load32(volatile atomic_i32* object) {
	let ret = __iso_volatile_load32(object);
	__compiler_or_memory_barrier();
	return (static_cast(i32)(ret));
}

i64 __atomic_load64(volatile atomic_i64* object) {
	#if __IS_X86
	let ret = __iso_volatile_load64(object);
	__compiler_or_memory_barrier();
	#else
	let ret = __ldrexd(object);
	__memory_barrier();
	#endif // __IS_X86
	return (static_cast(i64)(ret));
}

i8 __atomic_fetch_add_explicit8(volatile atomic_i8* addend,
								// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
								i8 value,
								[[maybe_unused]] memory_order order) {
	ignore(order);
	i8 res = 0;
	return __CHOOSE_INTRINSIC(order,
							  res,
							  _InterlockedExchangeAdd8,
							  static_cast(volatile char*)(addend),
							  value);
}

i16 __atomic_fetch_add_explicit16(volatile atomic_i16* addend,
								  // NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
								  i16 value,
								  [[maybe_unused]] memory_order order) {
	ignore(order);
	i16 res = 0;
	return __CHOOSE_INTRINSIC(order, res, _InterlockedExchangeAdd16, addend, value);
}

i32 __atomic_fetch_add_explicit32(volatile atomic_i32* addend,
								  // NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
								  i32 value,
								  [[maybe_unused]] memory_order order) {
	ignore(order);
	i32 res = 0;
	return __CHOOSE_INTRINSIC(order,
							  res,
							  _InterlockedExchangeAdd32,
							  static_cast(volatile long*)(addend),
							  value);
}

i64 __atomic_fetch_add_explicit64(volatile atomic_i64* addend,
								  // NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
								  i64 value,
								  [[maybe_unused]] memory_order order) {

	#if __IS_X86
	ignore(order);
	IGNORE_SEQ_CST_WARNING_START
	i64 val = __atomic_load64(addend);
	while(!__atomic_compare_exchange_strong(addend, &val, val + value)) {
	}

	return val;
	IGNORE_SEQ_CST_WARNING_STOP
	#else
	i64 res = 0;
	return __CHOOSE_INTRINSIC(order, res, _InterlockedExchangeAdd64, addend, value);
	#endif
}

i8 __atomic_fetch_or_explicit8(volatile atomic_i8* object,
							   // NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
							   i8 operand,
							   [[maybe_unused]] memory_order order) {
	ignore(order);
	i8 res = 0;
	return __CHOOSE_INTRINSIC(order,
							  res,
							  _InterlockedOr8,
							  static_cast(volatile char*)(object),
							  operand);
}

i16 __atomic_fetch_or_explicit16(volatile atomic_i16* object,
								 // NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
								 i16 operand,
								 [[maybe_unused]] memory_order order) {
	ignore(order);
	i16 res = 0;
	return __CHOOSE_INTRINSIC(order, res, _InterlockedOr16, object, operand);
}

i32 __atomic_fetch_or_explicit32(volatile atomic_i32* object,
								 // NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
								 i32 operand,
								 [[maybe_unused]] memory_order order) {
	ignore(order);
	i32 res = 0;
	return __CHOOSE_INTRINSIC(order,
							  res,
							  _InterlockedOr32,
							  static_cast(volatile long*)(object),
							  operand);
}

i64 __atomic_fetch_or_explicit64(volatile atomic_i64* object,
								 // NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
								 i64 operand,
								 [[maybe_unused]] memory_order order) {

	#if __IS_X86
	ignore(order);
	IGNORE_SEQ_CST_WARNING_START
	i64 val = __atomic_load64(object);
	// NOLINTNEXTLINE(hicpp-signed-bitwise)
	while(!__atomic_compare_exchange_strong(object, &val, val | operand)) {
	}

	return val;
	IGNORE_SEQ_CST_WARNING_STOP
	#else
	i64 res = 0;
	return __CHOOSE_INTRINSIC(order, res, _InterlockedOr64, object, operand);
	#endif
}

i8 __atomic_fetch_xor_explicit8(volatile atomic_i8* object,
								// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
								i8 operand,
								[[maybe_unused]] memory_order order) {
	ignore(order);
	i8 res = 0;
	return __CHOOSE_INTRINSIC(order,
							  res,
							  _InterlockedXor8,
							  static_cast(volatile char*)(object),
							  operand);
}

i16 __atomic_fetch_xor_explicit16(volatile atomic_i16* object,
								  // NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
								  i16 operand,
								  [[maybe_unused]] memory_order order) {
	ignore(order);
	i16 res = 0;
	return __CHOOSE_INTRINSIC(order, res, _InterlockedXor16, object, operand);
}

i32 __atomic_fetch_xor_explicit32(volatile atomic_i32* object,
								  // NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
								  i32 operand,
								  [[maybe_unused]] memory_order order) {
	ignore(order);
	i32 res = 0;
	return __CHOOSE_INTRINSIC(order,
							  res,
							  _InterlockedXor32,
							  static_cast(volatile long*)(object),
							  operand);
}

i64 __atomic_fetch_xor_explicit64(volatile atomic_i64* object,
								  // NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
								  i64 operand,
								  [[maybe_unused]] memory_order order) {

	#if __IS_X86
	ignore(order);
	IGNORE_SEQ_CST_WARNING_START
	i64 val = __atomic_load64(object);
	// NOLINTNEXTLINE(hicpp-signed-bitwise)
	while(!__atomic_compare_exchange_strong(object, &val, val ^ operand)) {
	}

	return val;
	IGNORE_SEQ_CST_WARNING_STOP
	#else
	i64 res = 0;
	return __CHOOSE_INTRINSIC(order, res, _InterlockedXor64, object, operand);
	#endif
}

i8 __atomic_fetch_and_explicit8(volatile atomic_i8* object,
								// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
								i8 operand,
								[[maybe_unused]] memory_order order) {
	ignore(order);
	i8 res = 0;
	return __CHOOSE_INTRINSIC(order,
							  res,
							  _InterlockedAnd8,
							  static_cast(volatile char*)(object),
							  operand);
}

i16 __atomic_fetch_and_explicit16(volatile atomic_i16* object,
								  // NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
								  i16 operand,
								  [[maybe_unused]] memory_order order) {
	ignore(order);
	i16 res = 0;
	return __CHOOSE_INTRINSIC(order, res, _InterlockedAnd16, object, operand);
}

i32 __atomic_fetch_and_explicit32(volatile atomic_i32* object,
								  // NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
								  i32 operand,
								  [[maybe_unused]] memory_order order) {
	ignore(order);
	i32 res = 0;
	return __CHOOSE_INTRINSIC(order,
							  res,
							  _InterlockedAnd32,
							  static_cast(volatile long*)(object),
							  operand);
}

i64 __atomic_fetch_and_explicit64(volatile atomic_i64* object,
								  // NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
								  i64 operand,
								  [[maybe_unused]] memory_order order) {
	#if __IS_X86
	ignore(order);
	IGNORE_SEQ_CST_WARNING_START
	i64 val = __atomic_load64(object);
	// NOLINTNEXTLINE(hicpp-signed-bitwise)
	while(!__atomic_compare_exchange_strong(object, &val, val & operand)) {
	}

	return val;
	IGNORE_SEQ_CST_WARNING_STOP
	#else
	i64 res = 0;
	return __CHOOSE_INTRINSIC(order, res, _InterlockedAnd64, object, operand);
	#endif
}

i8 __atomic_exchange_explicit8(volatile atomic_i8* object,
							   // NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
							   i8 desired,
							   [[maybe_unused]] memory_order order) {
	ignore(order);
	i8 res = 0;
	return __CHOOSE_INTRINSIC(order,
							  res,
							  _InterlockedExchange8,
							  static_cast(volatile char*)(object),
							  desired);
}

i16 __atomic_exchange_explicit16(volatile atomic_i16* object,
								 // NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
								 i16 desired,
								 [[maybe_unused]] memory_order order) {
	ignore(order);
	i16 res = 0;
	return __CHOOSE_INTRINSIC(order, res, _InterlockedExchange16, object, desired);
}

i32 __atomic_exchange_explicit32(volatile atomic_i32* object,
								 // NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
								 i32 desired,
								 [[maybe_unused]] memory_order order) {
	ignore(order);
	i32 res = 0;
	return __CHOOSE_INTRINSIC(order,
							  res,
							  _InterlockedExchange32,
							  static_cast(volatile long*)(object),
							  desired);
}

i64 __atomic_exchange_explicit64(volatile atomic_i64* object,
								 // NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
								 i64 desired,
								 [[maybe_unused]] memory_order order) {
	#if __IS_X86 && !IS_CNX_PLATFORM_ARCHITECTURE(CNX_PLATFORM_X86_64) // 32bit x86
	i64 val = __atomic_load64(object);
	while(!__atomic_compare_exchange_strong_explicit64(object, &val, desired, order)) {
	}

	return val;
	#else // 64bit x86 or ARM
	ignore(order);
	i64 res = 0;
	return __CHOOSE_INTRINSIC(order, res, _InterlockedExchange64, object, desired);
	#endif
}

bool __atomic_compare_exchange_strong_explicit8(
	volatile atomic_i8* object,
	i8* restrict expected,
	// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
	i8 desired,
	[[maybe_unused]] memory_order order) {
	ignore(order);
	i8 res = 0;
	res = __CHOOSE_INTRINSIC(order,
							 res,
							 _InterlockedCompareExchange8,
							 static_cast(volatile char*)(object),
							 desired,
							 *expected);
	if(res == *expected) {
		return true;
	}

	*expected = res;
	cnx_memcpy(typeof(res), expected, &res, 1);
	return false;
}

bool __atomic_compare_exchange_strong_explicit16(
	volatile atomic_i16* object,
	i16* restrict expected,
	// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
	i16 desired,
	[[maybe_unused]] memory_order order) {
	ignore(order);
	i16 res = 0;
	res = __CHOOSE_INTRINSIC(order, res, _InterlockedCompareExchange16, object, desired, *expected);
	if(res == *expected) {
		return true;
	}

	*expected = res;
	cnx_memcpy(typeof(res), expected, &res, 1);
	return false;
}

bool __atomic_compare_exchange_strong_explicit32(
	volatile atomic_i32* object,
	i32* restrict expected,
	// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
	i32 desired,
	[[maybe_unused]] memory_order order) {
	ignore(order);
	i32 res = 0;
	res = __CHOOSE_INTRINSIC(order,
							 res,
							 _InterlockedCompareExchange32,
							 static_cast(volatile long*)(object),
							 desired,
							 *expected);
	if(res == *expected) {
		return true;
	}

	*expected = res;
	cnx_memcpy(typeof(res), expected, &res, 1);
	return false;
}

bool __atomic_compare_exchange_strong_explicit64(
	volatile atomic_i64* object,
	i64* restrict expected,
	// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
	i64 desired,
	[[maybe_unused]] memory_order order) {
	ignore(order);
	i64 res = 0;
	res = __CHOOSE_INTRINSIC(order, res, _InterlockedCompareExchange64, object, desired, *expected);
	if(res == *expected) {
		return true;
	}

	*expected = res;
	cnx_memcpy(typeof(res), expected, &res, 1);
	return false;
}

#endif
