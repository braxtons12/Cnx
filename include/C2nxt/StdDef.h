/// @file StdDef.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief StdDef provides various `#define`s for performing basic tasks and macro-related
/// functions.
/// @version 0.1
/// @date 2021-07-20
///
/// MIT License
/// @copyright Copyright (c) 2021 Braxton Salyer <braxtonsalyer@gmail.com>
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

/// @defgroup std_def Standard Defines
/// This module provides various `#define`s for performing basic tasks and macro-related functions,
/// and some small meta-programming functionality

#include <stdbool.h>

#ifndef STD_DEF
	/// @brief definitions related to Standard Defines
	#define STD_DEF

	/// @brief Concatenates the two arguments
	///
	/// `A` and `B` must be alphanumeric, macro-concatenation compatible sequences of characters
	///
	/// @param A - the left-side portion of the concatenation
	/// @param B - the right-side portion of the concatenation
	///
	/// @return the concatenation of `A` and `B`
	#define ___CONCAT2(A, B) A##B
	/// @brief Concatenates the two arguments
	///
	/// `A` and `B` must be alphanumeric, macro-concatenation compatible sequences of characters
	///
	/// @param A - the left-side portion of the concatenation
	/// @param B - the right-side portion of the concatenation
	///
	/// @return the concatenation of `A` and `B`
	/// @ingroup std_def
	#define CONCAT2(A, B) ___CONCAT2(A, B)
	/// @brief Concatenates the two arguments with one additional layer of indirection
	///
	/// `A` and `B` must be alphanumeric, macro-concatenation compatible sequences of characters
	/// This is useful for use in a macro chain when the result must be deferred through one-further
	/// stage of macro replacement than the normal `CONCAT2(A, B)` provides
	///
	/// @param A - the left-side portion of the concatenation
	/// @param B - the right-side portion of the concatenation
	///
	/// @return the concatenation of `A` and `B`
	/// @ingroup std_def
	#define CONCAT2_DEFERED(A, B) CONCAT2(A, B)

	/// @brief Concatenates the three arguments
	///
	/// `A`, `B`, and `C` must be alphanumeric, macro-concatenation compatible sequences of
	/// characters
	///
	/// @param A - the left-side portion of the concatenation
	/// @param B - the middle portion of the concatenation
	/// @param C - the right-side portion of the concatenation
	///
	/// @return the concatenation of `A`, `B`, and `C`
	#define ___CONCAT3(A, B, C) CONCAT2(A, CONCAT2(B, C))
	/// @brief Concatenates the three arguments
	///
	/// `A`, `B`, and `C` must be alphanumeric, macro-concatenation compatible sequences of
	/// characters
	///
	/// @param A - the left-side portion of the concatenation
	/// @param B - the middle portion of the concatenation
	/// @param C - the right-side portion of the concatenation
	///
	/// @return the concatenation of `A`, `B`, and `C`
	/// @ingroup std_def
	#define CONCAT3(A, B, C) ___CONCAT3(A, B, C)

	/// @brief Concatenates the four arguments
	///
	/// `A`, `B`, `C`, and `D` must be alphanumeric, macro-concatenation compatible sequences of
	/// characters
	///
	/// @param A - the left-side portion of the concatenation
	/// @param B - the left-middle portion of the concatenation
	/// @param C - the right-middle portion of the concatenation
	/// @param D - the right-side portion of the concatenation
	///
	/// @return the concatenation of `A`, `B`, `C`, and `D`
	#define ___CONCAT4(A, B, C, D) CONCAT2(A, CONCAT3(B, C, D))
	/// @brief Concatenates the four arguments
	///
	/// `A`, `B`, `C`, and `D` must be alphanumeric, macro-concatenation compatible sequences of
	/// characters
	///
	/// @param A - the left-side portion of the concatenation
	/// @param B - the left-middle portion of the concatenation
	/// @param C - the right-middle portion of the concatenation
	/// @param D - the right-side portion of the concatenation
	///
	/// @return the concatenation of `A`, `B`, `C`, and `D`
	/// @ingroup std_def
	#define CONCAT4(A, B, C, D) ___CONCAT4(A, B, C, D)

	/// @brief Stringifies the given textual argument
	///
	/// Converts the given argument into a string literal, performing macro expansion if necessary
	///
	/// @param x - The text to stringify
	///
	/// @return `x` as a string literal
	#define ___AS_STRING(x) #x
	/// @brief Stringifies the given textual argument
	///
	/// Converts the given argument into a string literal, performing macro expansion if necessary
	///
	/// @param x - The text to stringify
	///
	/// @return `x` as a string literal
	/// @ingroup std_def
	#define AS_STRING(x) ___AS_STRING(x)

	/// @brief Provides a semi-unique variable name with `x` as a prefix
	///
	/// Useful inside macros to prevent name-collision and/or shadowing.
	/// Does not create a __truly__ unique variable name, so must be used with some care, but is
	/// good for most circumstances. The produced name is not cachable/storable and depends on
	/// `__LINE__`, so cannot be re-referenced later in a file, so this is only useful for
	/// one-time-use names or for use inside macros
	///
	/// @param x - The prefix for the variable name
	///
	/// @return The resulting variable name
	/// @ingroup std_def
	#define UNIQUE_VAR(x) CONCAT3(x, _4x2_, __LINE__)

	/// @brief Returns the type of the given expression
	///
	/// @param value - The expression to get the resulting type of
	///
	/// @return the type of the expression
	/// @ingroup std_def
	#define typeof(value) __typeof__(value)

	#if defined(__clang__)
		/// @brief Disables the preceding/following function at compile-time
		///
		/// Forces a compiler error with the given message if the associated function is called.
		///
		/// @param message - The error message for the compiler to show the user
		/// @ingroup std_def
		#define std_disable(message) __attribute__((diagnose_if(true, (message), "error")))
		/// @brief Disables the preceding/following function at compile-time, based on the given
		/// `condition`
		///
		/// Forces a compiler error with the given message if the associated function is called and
		/// `condition` evaluates to `true`. `condition` can include parameters to the function if
		/// this is applied as a postfix, thus enabling things like:
		/// @code {.c}
		/// int func(int a, int b) std_disable_if(a < b, "a must be greater than b") {
		/// 	// do things here...
		/// }
		/// @endcode
		///
		/// @param condition - The condition indicating whether the function should be disabled
		/// @param message - The error message for the compiler to show the user
		///
		/// @note This is only available when compiling with Clang
		///
		/// @ingroup std_def
		#define std_disable_if(condition, message) \
			__attribute__((diagnose_if((condition), (message), "error")))
		/// @brief Enables the preceding/following function at compile-time, based on the given
		/// `condition`
		///
		/// Forces a compiler error with the given message if the associated function is called and
		/// `condition` evaluates to `false`. `condition` can include parameters to the function if
		/// this is applied as a postfix, thus enabling things like:
		/// @code {.c}
		/// int func(int a, int b) std_enable_if(a > b, "a must be greater than b") {
		/// 	// do things here...
		/// }
		/// @endcode
		///
		/// @param condition - The condition indicating whether the function should be enabled
		/// @param message - The error message for the compiler to show the user
		///
		/// @note This is only available when compiling with Clang
		///
		/// @ingroup std_def
		#define std_enable_if(condition, message) std_disable_if(!(condition), (message))
		/// @brief Emits a warning for using the preceding/following function at compile-time, based
		/// on the given `condition`
		///
		/// Forces a compiler warning with the given message if the associated function is called
		/// and `condition` evaluates to `true`. `condition` can include parameters to the function
		/// if this is applied as a postfix, thus enabling things like:
		/// @code {.c}
		/// int func(int a, int b) std_warn_if(a < b, "a must be greater than b") {
		/// 	// do things here...
		/// }
		/// @endcode
		///
		/// @param condition - The condition indicating whether using the function should cause a
		/// warning
		/// @param message - The error message for the compiler to show the user
		///
		/// @note This is only available when compiling with Clang
		///
		/// @ingroup std_def
		#define std_warn_if(condition, message) \
			__attribute__((diagnose_if((condition), (message), "warning")))
	#else
		/// @brief Disables the preceding/following function at compile-time
		///
		/// Forces a compiler error with the given message if the associated function is called.
		///
		/// @param message - The error message for the compiler to show the user
		/// @ingroup std_def
		#define std_disable(message) __attribute__((error(message)))
		/// @brief Disables the preceding/following function at compile-time, based on the given
		/// `condition`
		///
		/// Forces a compiler error with the given message if the associated function is called and
		/// `condition` evaluates to `true`. `condition` can include parameters to the function if
		/// this is applied as a postfix, thus enabling things like:
		/// @code {.c}
		/// int func(int a, int b) std_disable_if(a < b, "a must be greater than b") {
		/// 	// do things here...
		/// }
		/// @endcode
		///
		/// @param condition - The condition indicating whether the function should be disabled
		/// @param message - The error message for the compiler to show the user
		///
		/// @note This is only available when compiling with Clang
		///
		/// @ingroup std_def
		#define std_disable_if(condition, message)
		/// @brief Enables the preceding/following function at compile-time, based on the given
		/// `condition`
		///
		/// Forces a compiler error with the given message if the associated function is called and
		/// `condition` evaluates to `false`. `condition` can include parameters to the function if
		/// this is applied as a postfix, thus enabling things like:
		/// @code {.c}
		/// int func(int a, int b) std_enable_if(a > b, "a must be greater than b") {
		/// 	// do things here...
		/// }
		/// @endcode
		///
		/// @param condition - The condition indicating whether the function should be enabled
		/// @param message - The error message for the compiler to show the user
		///
		/// @note This is only available when compiling with Clang
		///
		/// @ingroup std_def
		#define std_enable_if(condition, message)
		/// @brief Emits a warning for using the preceding/following function at compile-time, based
		/// on the given `condition`
		///
		/// Forces a compiler warning with the given message if the associated function is called
		/// and `condition` evaluates to `true`. `condition` can include parameters to the function
		/// if this is applied as a postfix, thus enabling things like:
		/// @code {.c}
		/// int func(int a, int b) std_warn_if(a < b, "a must be greater than b") {
		/// 	// do things here...
		/// }
		/// @endcode
		///
		/// @param condition - The condition indicating whether using the function should cause a
		/// warning
		/// @param message - The error message for the compiler to show the user
		///
		/// @note This is only available when compiling with Clang
		///
		/// @ingroup std_def
		#define std_warn_if(condition, message)
	#endif

	/// @brief Specify that the given scope is unreachable
	/// @ingroup std_def
	#define unreachable() __builtin_unreachable()
	/// @brief Specify that the given variable of function may be unused
	/// @ingroup std_def
	#define maybe_unused __attribute__((unused))
	/// @brief Specify that the following function should always be inlined
	/// @ingroup std_def
	#define always_inline __attribute__((always_inline))

	/// @brief Use this macro to declare a variable that will have a cleanup function called on it
	/// at scope end.
	///
	/// This allows for automatic RAII-style handling of resources by guaranteeing that the variable
	/// will have whatever cleanup needs performed on it completed at scope end.
	///
	/// @param scope_end_func - The function to call on the declared variable when it goes out of
	/// scope
	/// @ingroup std_def
	#define scoped(scope_end_func) __attribute__((cleanup(scope_end_func)))

	/// @brief Declare a variable with `let` to create a `const` variable with inferred type.
	///
	/// `let` declares the variable as const and infers the type from the value being assigned to
	/// it. A variable declared with `let` will be `const` and thus read only. `let` should be
	/// preferred for all variable declarations, whenever possible.
	/// @ingroup std_def
	#define let const __auto_type
	/// @brief Declare a variable with `let_mut` to create a mutable variable with inferred type.
	///
	/// `let_mut` declares the variable and infers the type from the value being assigned to it.
	/// A variable declared with `let_mut` will be mutable and thus able to be modified.
	/// `let_mut` should be preferred for all variable declarations when mutability is required,
	/// whenever possible.
	///
	/// @note When compiling with GCC, `let_mut` may infer the constness of the value being assigned
	/// as part of the type, which means a `const_cast` or tempory ignoring of
	/// "-Wdiscarded-qualifiers" may be necessary
	/// @ingroup std_def
	#define let_mut __auto_type

	/// @brief shorthand for writing `T_ptr`, used for generating typedef names needed for macros
	/// that require an alphanumeric name for `T*`
	///
	/// @param T - The type to generate a name for "pointer to" for
	///
	/// @ingroup std_def
	#define Ptr(T) CONCAT2(T, _ptr)
	/// @brief shorthand for writing `T_ptr`, used for generating typedef names needed for macros
	/// that require an alphanumeric name for `T*` when implying reference-like semantics
	///
	/// @param T - The type to generate a name for "pointer to" for
	///
	/// @ingroup std_def
	#define Ref(T) CONCAT2(T, _ref)
	/// @brief shorthand for writing `const_T_ptr`, used for generating typedef names needed for
	/// macros that require an alphanumeric name for `const T*` when implying reference-like
	/// semantics
	///
	/// @param T - The type to generate a name for "pointer to const" for
	///
	/// @ingroup std_def
	#define ConstRef(T) CONCAT3(const_, T, _ref)

	/// @brief Shorthand for writing a `for` loop.
	///
	/// Expands to a `for` loop with loop variable `var` initialized to `begin` and ending at `end`
	/// non-inclusive.
	/// Example:
	/// @code {.c}
	/// // loops over the range [0, 10)
	/// ranged_for(i, 0, 10) {
	///		// do something here ...
	/// }
	/// @endcode
	///
	/// @param var - The name of the loop variable
	/// @param begin - The initial value for `var`
	/// @param end - The stop value for `var`
	/// @ingroup std_def
	#define ranged_for(var, begin, end) for(let_mut var = (begin); (var) < (end); ++(var))

	/// @brief Ignores the given parameters.
	///
	/// Use to ignore the passed parameters, preventing compiler warnings for parameters that are
	/// intentionally unused
	/// @ingroup std_def
	#define ignore(...) (void)(__VA_ARGS__)

	/// @brief Casts to the type `Type`.
	///
	/// This should be preferred for making non-lossy conversions between types (eg `i8`
	/// to `i32` or `void*` to `cstring`)
	///
	/// @param Type - The type to cast to
	/// @ingroup std_def
	#define static_cast(Type) (Type)
	/// @brief Casts to the type `Type`.
	///
	/// This should be preferred for making lossy conversions between types (eg `i32` to `f32` or
	/// `u64` to `i64`)
	///
	/// @param Type - The type to cast to
	/// @ingroup std_def
	#define narrow_cast(Type) (Type)
	/// @brief Casts to the type `Type`.
	///
	/// This should be preferred when casting away const-ness of a value
	/// (eg `const_cstring` to `cstring`)
	///
	/// @param Type - The type to cast to
	/// @ingroup std_def
	#define const_cast(Type) (Type)

	#undef false
	/// @brief Define the boolean constant `false`
	///
	/// Redefine `false` as casted to bool to enforce correct type inference with `let` and
	/// `let_mut`
	/// @ingroup std_def
	#define false static_cast(bool)(0)

	#undef true
	/// @brief Define the boolean constant `true`
	///
	/// Redefine `true` as casted to bool to enforce correct type inference with `let` and
	/// `let_mut`
	/// @ingroup std_def
	#define true static_cast(bool)(1)
// clang-format off

	/// @brief Gets the 128th argument from the `__VA_ARGS__` pack
	#define ___PP_128TH_ARG( \
          _1, _2, _3, _4, _5, _6, _7, _8, _9,_10, \
         _11,_12,_13,_14,_15,_16,_17,_18,_19,_20, \
         _21,_22,_23,_24,_25,_26,_27,_28,_29,_30, \
         _31,_32,_33,_34,_35,_36,_37,_38,_39,_40, \
         _41,_42,_43,_44,_45,_46,_47,_48,_49,_50, \
         _51,_52,_53,_54,_55,_56,_57,_58,_59,_60, \
         _61,_62,_63,_64,_65,_66,_67,_68,_69,_70, \
         _71,_72,_73,_74,_75,_76,_77,_78,_79,_80, \
         _81,_82,_83,_84,_85,_86,_87,_88,_89,_90, \
         _91,_92,_93,_94,_95,_96,_97,_98,_99,_100, \
         _101,_102,_103,_104,_105,_106,_107,_108,_109,_110, \
         _111,_112,_113,_114,_115,_116,_117,_118,_119,_120, \
         _121,_122,_123,_124,_125,_126,_127,N,...) N

	/// @brief Creates a comma separated reverse sequence from 127 to 0
	///
	/// Useful for macro meta-programming that requires determing the number of arguments passed in
	/// a `__VA_ARGS__` pack
	/// @ingroup std_def
	#define PP_REVERSE_SEQUENCE_N() \
         127,126,125,124,123,122,121,120, \
         119,118,117,116,115,114,113,112,111,110, \
         109,108,107,106,105,104,103,102,101,100, \
         99,98,97,96,95,94,93,92,91,90, \
         89,88,87,86,85,84,83,82,81,80, \
         79,78,77,76,75,74,73,72,71,70, \
         69,68,67,66,65,64,63,62,61,60, \
         59,58,57,56,55,54,53,52,51,50, \
         49,48,47,46,45,44,43,42,41,40, \
         39,38,37,36,35,34,33,32,31,30, \
         29,28,27,26,25,24,23,22,21,20, \
         19,18,17,16,15,14,13,12,11,10, \
         9,8,7,6,5,4,3,2,1,0

// clang-format on

	/// @brief Returns the number of arguments passed in the parameter pack
	#define ___PP_NUM_ARGS(...) ___PP_128TH_ARG(__VA_ARGS__)
	/// @brief Returns the number of arguments passed in the `__VA_ARGS__` parameter pack
	///
	/// Useful for macro meta-programming that requires determing the number of arguments passed in
	/// a parameter pack
	///
	/// @param ... - The `__VA_ARGS__` parameter pack to get the length of
	///
	/// @return The number of args in the parameter pack
	/// @ingroup std_def
	#define PP_NUM_ARGS(...) ___PP_NUM_ARGS(__VA_ARGS__, PP_REVERSE_SEQUENCE_N())

	/// @brief applies `what` to `x` and the `__VA_ARGS__` parameter pack
	#define ___APPLY_TO_LIST_1(what, x, ...) what(x)
	/// @brief applies `what` to `x` and the `__VA_ARGS__` parameter pack
	#define ___APPLY_TO_LIST_2(what, x, ...) what(x), ___APPLY_TO_LIST_1(what, __VA_ARGS__)
	/// @brief applies `what` to `x` and the `__VA_ARGS__` parameter pack
	#define ___APPLY_TO_LIST_3(what, x, ...) what(x), ___APPLY_TO_LIST_2(what, __VA_ARGS__)
	/// @brief applies `what` to `x` and the `__VA_ARGS__` parameter pack
	#define ___APPLY_TO_LIST_4(what, x, ...) what(x), ___APPLY_TO_LIST_3(what, __VA_ARGS__)
	/// @brief applies `what` to `x` and the `__VA_ARGS__` parameter pack
	#define ___APPLY_TO_LIST_5(what, x, ...) what(x), ___APPLY_TO_LIST_4(what, __VA_ARGS__)
	/// @brief applies `what` to `x` and the `__VA_ARGS__` parameter pack
	#define ___APPLY_TO_LIST_6(what, x, ...) what(x), ___APPLY_TO_LIST_5(what, __VA_ARGS__)
	/// @brief applies `what` to `x` and the `__VA_ARGS__` parameter pack
	#define ___APPLY_TO_LIST_7(what, x, ...) what(x), ___APPLY_TO_LIST_6(what, __VA_ARGS__)
	/// @brief applies `what` to `x` and the `__VA_ARGS__` parameter pack
	#define ___APPLY_TO_LIST_8(what, x, ...) what(x), ___APPLY_TO_LIST_7(what, __VA_ARGS__)
	/// @brief applies `what` to `x` and the `__VA_ARGS__` parameter pack
	#define ___APPLY_TO_LIST_9(what, x, ...) what(x), ___APPLY_TO_LIST_8(what, __VA_ARGS__)
	/// @brief applies `what` to `x` and the `__VA_ARGS__` parameter pack
	#define ___APPLY_TO_LIST_10(what, x, ...) what(x), ___APPLY_TO_LIST_9(what, __VA_ARGS__)
	/// @brief applies `what` to `x` and the `__VA_ARGS__` parameter pack
	#define ___APPLY_TO_LIST_11(what, x, ...) what(x), ___APPLY_TO_LIST_10(what, __VA_ARGS__)
	/// @brief applies `what` to `x` and the `__VA_ARGS__` parameter pack
	#define ___APPLY_TO_LIST_12(what, x, ...) what(x), ___APPLY_TO_LIST_11(what, __VA_ARGS__)

	/// @brief Returns the 13th arg
	#define ___APPLY_TO_LIST_13TH_ARG(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, N, ...) N

	/// @brief Creates a comma seperated reverse sequence from 12 to 0
	#define ___APPLY_TO_LIST_REVERSE_SEQUENCE_N() 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0
	/// @brief Returns the 13th arg
	#define ___APPLY_TO_LIST_13TH_ARG___(...) ___APPLY_TO_LIST_13TH_ARG(__VA_ARGS__)
	/// @brief Gets the number of arguments in the list
	#define ___APPLY_TO_LIST_NUM_ARGS(...) \
		___APPLY_TO_LIST_13TH_ARG___(__VA_ARGS__, ___APPLY_TO_LIST_REVERSE_SEQUENCE_N())

	/// @brief Applies `what` to the arguments in the `__VA_ARGS__` parameter pack
	#define ___APPLY_TO_LIST(N, what, ...) CONCAT2_DEFERED(___APPLY_TO_LIST_, N)(what, __VA_ARGS__)
	/// @brief Calls `what` on the arguments in the parameter pack
	///
	/// Supports up to 12 arguments in the parameter pack.
	/// For every argument, `what` is called/applied on it in place in the pack,
	/// eg., `what(x), what(y)`, where `x` and `y` are arguments in the pack.
	/// Example:
	/// @code {.c}
	/// i32* add_one(i32* i) {
	///		++(*i);
	///		return i;
	/// }
	/// i32 zero = 0, one = 1, two = 2;
	/// // prints "1, 2, 3" to `stdout`
	/// println("{}, {}, {}", APPLY_TO_LIST(&, APPLY_TO_LIST(add_one, &zero, &one, &two)));
	/// @endcode
	///
	/// This is most useful for applying a macro or function to a list of values before passing them
	/// into another macro or variadic function.
	///
	/// @param what - The function or function-like macro to apply to the arguments in the pack
	/// @param ... - The parameter pack to apply `what` to
	///
	/// @return The parameter pack, with `what` applied
	/// @ingroup std_def
	#define APPLY_TO_LIST(what, ...) \
		___APPLY_TO_LIST(___APPLY_TO_LIST_NUM_ARGS(__VA_ARGS__), what, __VA_ARGS__)

#endif // STD_DEF
