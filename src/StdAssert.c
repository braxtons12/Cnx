#include "../include/C2nxt/StdAssert.h"

#include <assert.h>

#include "../include/C2nxt/StdIO.h"

#if STD_PLATFORM_DEBUG && !defined(STD_DISABLE_ASSERTIONS)
void std_assert_cstring(bool condition, const_cstring error_message, const_cstring file, i64 line) {
	if(!condition) {
		let message = std_string_from(error_message);
		let _file = std_string_from(file);
		eprintln("Assertion triggered at {}:{}: {}", _file, line, message);
		assert(condition);
	}
}
#endif // (defined(DEBUG) || !defined(NDEBUG)) && !defined(STD_DISABLE_ASSERTIONS)
