/// @file StdOption.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief This module provides a struct template for representing an optional value
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

#include "StdAllocators.h"
#include "StdAssert.h"
#include "StdBasicTypes.h"
#include "StdTrait.h"

/// @ingroup error_handling
/// @{
///	@defgroup std_option StdOption
/// `StdOption(T)` is a "struct template" for representing an optional value. It enables a simple,
/// type-safe way of working with values that may or may not exist or be value, without resorting
/// to heap-allocation and pointers or out-parameters.
///
/// `StdOption(T)` shares some functionality, such as `if_let(var, self)`, with
/// `StdResult(T)` (see @ref monadics).
///
/// Example:
///
/// @code {.c}
/// StdOption(u32) checked_add(u32 left, u32 right) {
/// 	return (left < std_max(u32) / 2 && right < std_max(u32) / 2) ?
/// 				Some(u32, left + right) :
/// 				None(u32);
/// }
///
/// void do_thing(void) {
///		let_mut left = 0;
///		let right = 10;
///		// do something with left and/or right ...
///		let_mut maybe_added = checked_add(left, right);
///		if_let(added, maybe_added) {
///			// do something with added...
/// 	}
/// 	else {
///			// maybe_added was `None(u32)`, do something to recover...
/// 	}
/// }
/// @endcode
/// @}

#ifndef STD_OPTION
	/// @brief Declarations and definitions related to `StdOption(T)`
	#define STD_OPTION

	/// @brief Macro alias for `StdOption` holding the type `T`
	///
	/// Used to generate and refer to a typedef name for `StdOption(T)` instantiations of type
	/// `T`
	///
	/// @param T - The type to store in the `StdOption(T)`
	/// @ingroup std_option
	#define StdOption(T) CONCAT2(StdOption, T)
	/// @brief Creates an identifier for a function, variable, or struct associated with
	/// `StdOption(T)`
	#define StdOptionIdentifier(T, Identifier) CONCAT2(std_option_, CONCAT3(T, _, Identifier))

	/// @brief Instantiates `StdOption(T)` holding the type `T`
	///
	/// Instantiates the function and struct declarations for `StdOption(T)`
	///
	/// @param T - The type stored in the `StdOption(T)`
	/// @ingroup std_option
	#define DeclStdOption(T)                                                                   \
		typedef struct StdOption(T) StdOption(T);                                              \
                                                                                               \
		typedef struct StdOptionIdentifier(T, vtable) {                                        \
			bool (*const is_some)(const StdOption(T)* restrict self);                          \
			bool (*const is_none)(const StdOption(T)* restrict self);                          \
			const T* (*const as_const)(const StdOption(T)* restrict self);                     \
			T* (*const as_mut)(StdOption(T)* restrict self);                                   \
			T (*const unwrap)(StdOption(T)* restrict self);                                    \
			T (*const unwrap_or)(StdOption(T)* restrict self, T default_value);                \
			T(*const unwrap_or_else)                                                           \
			(StdOption(T)* restrict self, T(*default_generator)(void));                        \
			T(*const expect)                                                                   \
			(StdOption(T)* restrict self, const_cstring restrict panic_message);               \
			bool (*const as_bool)(const StdOption(T)* restrict self);                          \
		}                                                                                      \
		StdOptionIdentifier(T, vtable);                                                        \
                                                                                               \
		/** @brief `StdOption` holding the type `T` **/                                        \
		typedef struct StdOption(T) {                                                          \
			T m_value;                                                                         \
			bool m_is_some;                                                                    \
			const StdOptionIdentifier(T, vtable) * m_vtable;                                   \
		}                                                                                      \
		StdOption(T);                                                                          \
                                                                                               \
		/** @brief Creates a `StdOption(T)` holding the given value **/                        \
		/** @param value - The value to store in the `StdOption` **/                           \
		/** @return a `StdOption` containing the given value **/                               \
		StdOption(T) StdOptionIdentifier(T, some)(T value);                                    \
		/** @brief Creates a `StdOption(T)` holding no value **/                               \
		/** @return a `StdOption` containing no value **/                                      \
		StdOption(T) StdOptionIdentifier(T, none)(void);                                       \
		/** @brief Returns whether this `StdOption(T)` is holding a value **/                  \
		/** @param self - The `StdOption` to check **/                                         \
		/** @return `true` if this `StdOption(T)` is holding a value, `false` otherwise **/    \
		bool StdOptionIdentifier(T, is_some)(const StdOption(T)* restrict self);               \
		/** @brief Returns whether this `StdOption(T)` __isn't__ holding a value **/           \
		/** @param self - The `StdOption` to check **/                                         \
		/** @return `true` if this `StdOption(T)` __isn't__ holding a value, `false` otherwise \
		 * **/                                                                                 \
		bool StdOptionIdentifier(T, is_none)(const StdOption(T)* restrict self);               \
		/** @brief Returns a pointer to the const value stored in this `StdOption` **/         \
		/** @param self - The `StdOption` to get the stored value from **/                     \
		/** @return a pointer to the contained const value **/                                 \
		/** @note Panics if `self` does not contain a value **/                                \
		const T* StdOptionIdentifier(T, as_const)(const StdOption(T)* restrict self);          \
		/** @brief Returns a pointer to the value stored in this `StdOption` **/               \
		/** @param self - The `StdOption` to get the stored value from **/                     \
		/** @return a pointer to the contained value **/                                       \
		/** @note Panics if `self` does not contain a value **/                                \
		T* StdOptionIdentifier(T, as_mut)(StdOption(T)* restrict self);                        \
		/** @brief Returns the value stored in this `StdOption` **/                            \
		/** @param self - The `StdOption` to get the stored value from **/                     \
		/** @return the contained value **/                                                    \
		/** @note Panics if `self` does not contain a value **/                                \
		T StdOptionIdentifier(T, unwrap)(StdOption(T)* restrict self);                         \
		/** @brief Returns the value stored in this `StdOption` , or `default_value` **/       \
		/** if this is `None` **/                                                              \
		/** @param self - The `StdOption` to get the stored value from **/                     \
		/** @param default_value - The value to return if this is `None` **/                   \
		/** @return the contained value, or `default_value` **/                                \
		T StdOptionIdentifier(T, unwrap_or)(StdOption(T)* restrict self, T default_value);     \
		/** @brief Returns the value stored in this `StdOption` , or the value returned by **/ \
		/** `default_generator` if this is `None` **/                                          \
		/** @param self - The `StdOption` to get the stored value from **/                     \
		/** @param default_generator - The function to generate the value **/                  \
		/**  to return if this is `None` **/                                                   \
		/** @return the contained value, or the one generated by `default_generator` **/       \
		T StdOptionIdentifier(T, unwrap_or_else)(StdOption(T)* restrict self,                  \
												 T(*default_generator)(void));                 \
		/** @brief Returns the value stored in this `StdOption` **/                            \
		/** @param self - The `StdOption` to get the stored value from **/                     \
		/** @return the contained value **/                                                    \
		/** @note Panics if `self` does not contain a value, **/                               \
		/** with the custom panic message `panic_message` **/                                  \
		T StdOptionIdentifier(T, expect)(StdOption(T)* restrict self,                          \
										 const_cstring restrict panic_mesage);                 \
                                                                                               \
		bool StdOptionIdentifier(T, as_bool)(const StdOption(T)* restrict self);

	/// @brief Creates a `StdOption(T)` holding the given value
	///
	/// @param T - The held type of the `StdOption(T)`
	/// @param value - The value to store in the `StdOption(T)`
	///
	/// @return a `StdOption(T)` containing the given value
	/// @ingroup std_option
	#define Some(T, value) StdOptionIdentifier(T, some)(value)
	/// @brief Creates a `StdOption(T)` holding no value
	///
	/// @param T - The held type of the `StdOption(T)`
	///
	/// @return a `StdOption(T)` containing no value
	/// @ingroup std_option
	#define None(T) StdOptionIdentifier(T, none)()
	/// @brief Returns whether this `StdOption(T)` is holding a value
	///
	/// @param self - The `StdOption(T)` to check
	///
	/// @return `true` if this `StdOption(T)` is holding a value, `false` otherwise
	/// @ingroup std_option
	#define std_option_is_some(self) (self).m_vtable->is_some(&(self))
	/// @brief Returns whether this `StdOption(T)` __isn't__ holding a value
	///
	/// @param self - The `StdOption(T)` to check
	///
	/// @return `true` if this `StdOption(T)` __isn't__ holding a value, `false` otherwise
	/// @ingroup std_option
	#define std_option_is_none(self) (self).m_vtable->is_none(&(self))
	/// @brief Returns a const reference to the value stored in this `StdOption(T)`
	///
	/// @param self - The `StdOption(T)` to get the stored value from
	///
	/// @return a const reference to the contained value
	/// @note Panics if `self` does not contain a value
	/// @ingroup std_option
	#define std_option_as_const(self) *((self).m_vtable->as_const(&(self)))
	/// @brief Returns a reference to the value stored in this `StdOption(T)`
	///
	/// @param self - The `StdOption(T)` to get the stored value from
	///
	/// @return a reference to the contained value
	/// @note Panics if `self` does not contain a value
	/// @ingroup std_option
	#define std_option_as_mut(self) *((self).m_vtable->as_mut(&(self)))
	/// @brief Returns the value stored in this `StdOption(T)`
	///
	/// @param self - The `StdOption(T)` to get the stored value from
	///
	/// @return the contained value
	/// @note Panics if `self` does not contain a value
	/// @ingroup std_option
	#define std_option_unwrap(self) (self).m_vtable->unwrap(&(self))
	/// @brief Returns the value stored in this `StdOption(T)` , or `default_value`
	/// if this is `None(T)`
	///
	/// @param self - The `StdOption(T)` to get the stored value from
	///
	/// @param default_value - The value to return if this is `None(T)`
	/// @return the contained value, or `default_value`
	/// @ingroup std_option
	#define std_option_unwrap_or(self, default_value) \
		(self).m_vtable->unwrap_or(&(self), (default_value))
	/// @brief Returns the value stored in this `StdOption(T)` , or the value returned by
	/// `default_generator` if this is `None(T)`
	///
	/// @param self - The `StdOption(T)` to get the stored value from
	/// @param default_generator - The function to generate the value
	///  to return if this is `None(T)`
	///
	/// @return the contained value, or the one generated by `default_generator`
	/// @ingroup std_option
	#define std_option_unwrap_or_else(self, default_generator) \
		(self).m_vtable->unwrap_or_else(&(self), (default_generator))
	/// @brief Returns the value stored in this `StdOption(T)`
	///
	/// @param self - The `StdOption(T)` to get the stored value from
	/// @param panic_message - The custom panic message to use if `self` is not the `Some(T, value)`
	/// variant
	///
	/// @return the contained value
	/// @note Panics if `self` does not contain a value, with the custom panic message
	/// `panic_message`
	/// @ingroup std_option
	#define std_option_expect(self, panic_message) (self).m_vtable->expect(&(self), (panic_message))

	/// @brief Converts the given `StdOption(T)` to a `bool`
	///
	/// This is equivalent to calling `std_option_is_some(self)`
	/// @param self - The `StdOption(T)` to convert to a `bool`
	///
	/// @return `self` as a `bool`
	/// @ingroup std_option
	#define std_option_as_bool(self) (self).m_vtable->as_bool(&(self))

	#include "monadic/StdIfLet.h"

	/// @brief Instantiates the definitions for `StdOption(T)` holding the type `T`
	///
	/// Instantiates the variable and function definitions necessary for `StdOption(T)` holding
	/// type `T`
	///
	/// @param T - The type stored in the `StdOption`
	/// @ingroup std_option
	#define ImplStdOption(T)                                                                \
                                                                                            \
		static const StdOptionIdentifier(T, vtable) StdOptionIdentifier(T, vtable_impl)     \
			= {.is_some = StdOptionIdentifier(T, is_some),                                  \
			   .is_none = StdOptionIdentifier(T, is_none),                                  \
			   .as_const = StdOptionIdentifier(T, as_const),                                \
			   .as_mut = StdOptionIdentifier(T, as_mut),                                    \
			   .unwrap = StdOptionIdentifier(T, unwrap),                                    \
			   .unwrap_or = StdOptionIdentifier(T, unwrap_or),                              \
			   .unwrap_or_else = StdOptionIdentifier(T, unwrap_or_else),                    \
			   .expect = StdOptionIdentifier(T, expect),                                    \
			   .as_bool = StdOptionIdentifier(T, as_bool)};                                 \
                                                                                            \
		StdOption(T) StdOptionIdentifier(T, some)(T value) {                                \
			return (StdOption(T)){.m_is_some = true,                                        \
								  .m_value = value,                                         \
								  .m_vtable = &StdOptionIdentifier(T, vtable_impl)};        \
		}                                                                                   \
                                                                                            \
		StdOption(T) StdOptionIdentifier(T, none)(void) {                                   \
			return (StdOption(T)){.m_is_some = false,                                       \
								  .m_vtable = &StdOptionIdentifier(T, vtable_impl)};        \
		}                                                                                   \
                                                                                            \
		bool StdOptionIdentifier(T, is_some)(const StdOption(T)* restrict self) {           \
			return self->m_is_some;                                                         \
		}                                                                                   \
                                                                                            \
		bool StdOptionIdentifier(T, is_none)(const StdOption(T)* restrict self) {           \
			return !(self->m_is_some);                                                      \
		}                                                                                   \
                                                                                            \
		const T* StdOptionIdentifier(T, as_const)(const StdOption(T)* restrict self) {      \
			if(!std_option_is_some(*self)) {                                                \
				std_panic("as_const called on a None value, terminating");                  \
			}                                                                               \
                                                                                            \
			return &(self->m_value);                                                        \
		}                                                                                   \
                                                                                            \
		T* StdOptionIdentifier(T, as_mut)(StdOption(T)* restrict self) {                    \
			if(!std_option_is_some(*self)) {                                                \
				std_panic("as_mut called on a None value, terminating");                    \
			}                                                                               \
                                                                                            \
			return &(self->m_value);                                                        \
		}                                                                                   \
                                                                                            \
		T StdOptionIdentifier(T, unwrap)(StdOption(T)* restrict self) {                     \
			if(!std_option_is_some(*self)) {                                                \
				std_panic("unwrap called on a None value, terminating");                    \
			}                                                                               \
                                                                                            \
			return self->m_value;                                                           \
		}                                                                                   \
                                                                                            \
		T StdOptionIdentifier(T, unwrap_or)(StdOption(T)* restrict self, T default_value) { \
			if(!std_option_is_some(*self)) {                                                \
				return default_value;                                                       \
			}                                                                               \
                                                                                            \
			return self->m_value;                                                           \
		}                                                                                   \
                                                                                            \
		T StdOptionIdentifier(T, unwrap_or_else)(StdOption(T)* restrict self,               \
												 T(*default_generator)(void)) {             \
			if(!std_option_is_some(*self)) {                                                \
				return default_generator();                                                 \
			}                                                                               \
                                                                                            \
			return self->m_value;                                                           \
		}                                                                                   \
                                                                                            \
		T StdOptionIdentifier(T, expect)(StdOption(T)* restrict self,                       \
										 const_cstring restrict panic_message) {            \
			if(!std_option_is_some(*self)) {                                                \
				std_panic(panic_message);                                                   \
			}                                                                               \
                                                                                            \
			return self->m_value;                                                           \
		}                                                                                   \
                                                                                            \
		bool StdOptionIdentifier(T, as_bool)(const StdOption(T)* restrict self) {           \
			return std_option_is_some(*self);                                               \
		}

	/// @brief Fully instantiates `StdOption(T)` holding the type `T`
	///
	/// Provides all function, variable, and struct declarations __and__ definitions necessary
	/// for `StdOption(T)` holding type `T`
	///
	/// @param T - The type stored in the `StdOption`
	/// @ingroup std_option
	#define DeclAndImplStdOption(T) DeclStdOption(T) ImplStdOption(T)

// `StdOption` implementations for built-in datatypes

/// @brief Instantiate `StdOption(T)` for the builtin `char`
DeclStdOption(char);
/// @brief Instantiate `StdOption(T)` for the builtin `u8`
DeclStdOption(u8);
/// @brief Instantiate `StdOption(T)` for the builtin `u16`
DeclStdOption(u16);
/// @brief Instantiate `StdOption(T)` for the builtin `u32`
DeclStdOption(u32);
/// @brief Instantiate `StdOption(T)` for the builtin `u64`
DeclStdOption(u64);
/// @brief Instantiate `StdOption(T)` for the builtin `usize`
DeclStdOption(usize);
/// @brief Instantiate `StdOption(T)` for the builtin `i8`
DeclStdOption(i8);
/// @brief Instantiate `StdOption(T)` for the builtin `i16`
DeclStdOption(i16);
/// @brief Instantiate `StdOption(T)` for the builtin `i32`
DeclStdOption(i32);
/// @brief Instantiate `StdOption(T)` for the builtin `i64`
DeclStdOption(i64);
/// @brief Instantiate `StdOption(T)` for the builtin `isize`
DeclStdOption(isize);
/// @brief Instantiate `StdOption(T)` for the builtin `f32`
DeclStdOption(f32);
/// @brief Instantiate `StdOption(T)` for the builtin `f64`
DeclStdOption(f64);
/// @brief Instantiate `StdOption(T)` for the builtin `u8_ptr`
DeclStdOption(u8_ptr);
/// @brief Instantiate `StdOption(T)` for the builtin `u16_ptr`
DeclStdOption(u16_ptr);
/// @brief Instantiate `StdOption(T)` for the builtin `u32_ptr`
DeclStdOption(u32_ptr);
/// @brief Instantiate `StdOption(T)` for the builtin `u64_ptr`
DeclStdOption(u64_ptr);
/// @brief Instantiate `StdOption(T)` for the builtin `usize_ptr`
DeclStdOption(usize_ptr);
/// @brief Instantiate `StdOption(T)` for the builtin `i8_ptr`
DeclStdOption(i8_ptr);
/// @brief Instantiate `StdOption(T)` for the builtin `i16_ptr`
DeclStdOption(i16_ptr);
/// @brief Instantiate `StdOption(T)` for the builtin `i32_ptr`
DeclStdOption(i32_ptr);
/// @brief Instantiate `StdOption(T)` for the builtin `i64_ptr`
DeclStdOption(i64_ptr);
/// @brief Instantiate `StdOption(T)` for the builtin `isize_ptr`
DeclStdOption(isize_ptr);
/// @brief Instantiate `StdOption(T)` for the builtin `f32_ptr`
DeclStdOption(f32_ptr);
/// @brief Instantiate `StdOption(T)` for the builtin `f64_ptr`
DeclStdOption(f64_ptr);
/// @brief Instantiate `StdOption(T)` for the builtin `cstring`
DeclStdOption(cstring);
/// @brief Instantiate `StdOption(T)` for the builtin `const_cstring`
DeclStdOption(const_cstring);
/// @brief Instantiate `StdOption(T)` for the builtin `char_ptr`
DeclStdOption(char_ptr);
/// @brief Instantiate `StdOption(T)` for the builtin `const_char_ptr`
DeclStdOption(const_char_ptr);

#endif // STD_OPTION
