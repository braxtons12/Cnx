/// @file StdTrait.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief This module provides a mechanism for creating polymorphic interfaces
/// via Trait objects
/// @version 0.1
/// @date 2021-07-20
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

/// @defgroup traits Trait
/// C2nxt provides a simple way to create and utilize polymorphic interfaces via Trait
/// objects. Trait objects are fat pointers, containing a pointer-to-self and a vtable pointer,
/// that allow use of polymorphic interfaces explicitly (you operate via the Trait object), without
/// the need for a type to carry its vtable in its definition, and without the need for heap
/// allocation (in most cases).
/// For specific examples of Trait objects in action, look into `StdFormat` and its use through the
/// C2nxt StdIO, and C2nxt's iterator categories.
///
/// A general example:
/// @code {.c}
/// // declare a Trait `NumberGenerator` that requires the functions `generate` and `seed`
/// Trait(NumberGenerator,
/// 	  u32 (*const generate)(NumberGenerator* restrict self);
/// 	  void (*const seed)(NumberGenerator* restrict self, u32 seed););
///
/// typedef struct ExampleGenerator {
///		u32 m_seed;
/// } ExampleGenerator;
///
/// u32 example_generate(NumberGenerator* restrict self) {
///		let_mut _self = static_cast(ExampleGenerator*)(self->m_self);
/// 	return _self->m_seed++;
/// }
///
/// void example_seed(NumberGenerator* restrict self, u32 seed) {
///		let_mut _self = static_cast(ExampleGenerator*)(self->m_self);
///		_self->m_seed = seed;
/// }
///
/// // Implement `NumberGenerator` for our `ExampleGenerator` type with the `example_generate`
/// // and `example_seed` functions.
/// // The names of the implementation functions do not have to stem from/build on/etc the names
/// // in the Trait definition, but it is good practice/convention to do so.
/// static maybe_unused ImplTraitFor(NumberGenerator,
/// 								 ExampleGenerator,
/// 								 example_generate,
/// 								 example_seed);
/// // a function that uses the `NumberGenerator` Trait as a parameter
/// u32 generate(NumberGenerator generator) {
///		return trait_call(generate, generator);
/// }
///
/// void do_thing(void) {
///		let_mut generator = (ExampleGenerator){.m_seed = 10;}
///		// cast `generator` to its `NumberGenerator` implementation and pass that to `generate`
///		let num = generate(as_trait(NumberGenerator, ExampleGenerator, generator));
/// }
/// @endcode

#include "StdBasicTypes.h"

#ifndef STD_TRAIT
	// @brief Definitions related to Traits
	#define STD_TRAIT

	/// @brief Macro alias to create and refer to a Trait vtable typedef
	/// @param TraitName - The name of the Trait
	#define TraitVTable(TraitName) CONCAT2(TraitName, VTable)
	/// @brief Macro alias to create and refer to an implemented Trait vtable typedef
	/// @param TraitName - The name of the Trait
	/// @param TypeName - The type implementing the Trait
	#define ImpledTraitVTable(TraitName, TypeName) \
		CONCAT2(TraitVTable(TraitName), CONCAT3(_, TypeName, _impl))

	/// @brief Instantiates a Trait vtable declaration for the given Trait with the given functions
	/// @param TraitName - The name of the Trait
	/// @param functions - The functions required in the trait
	#define DeclTraitVTable(TraitName, functions) \
		typedef struct TraitVTable(TraitName) {   \
			functions;                            \
		}                                         \
		TraitVTable(TraitName)

	/// @brief Instantiates a Trait vtable implementation of the given Trait for the given type
	///
	/// @param TraitName - The name of the Trait
	/// @param TypeName - The type implementing the Trait
	/// @param ... - The functions to implement the trait, comma separated, in Trait-defined order
	#define ImplTraitVTable(TraitName, TypeName, ...) \
		const TraitVTable(TraitName) ImpledTraitVTable(TraitName, TypeName) = {__VA_ARGS__}

	/// @brief Declares a Trait
	///
	/// Declares a Trait with the given name which requires the semicolon separated list of
	/// functions to be implemented.
	///
	/// @param TraitName - The name to refer to the Trait
	/// @param functions - The semicolon separated list of functions, in function-pointer syntax,
	/// that the Trait requires to be implemented
	/// @ingroup traits
	#define Trait(TraitName, functions)                          \
		typedef struct TraitName TraitName;                      \
		/** @brief vtable for trait implementation **/           \
		DeclTraitVTable(TraitName, functions);                   \
                                                                 \
		typedef struct TraitName {                               \
			/** @brief vtable for trait implementation **/       \
			const TraitVTable(TraitName) * m_vtable;             \
			/** @brief self pointer for trait implementation **/ \
			void* m_self;                                        \
		} TraitName

	/// @brief Declares a SizedTrait
	///
	/// Declares a SizedTrait with the given name which requires the semicolon separated list of
	/// functions to be implemented. A SizedTrait is a trait that copies its implementing type into
	/// a fixed-size, type-erased buffer instead of referring to it via pointer. This allows Trait
	/// objects to be constructed from short-lived values and used past the lifetime of said value
	/// without needing heap allocation, but requires that all types implementing the Trait fit
	/// within this fixed size buffer. All C2nxt iterators use this type of trait for their
	/// interface for these reasons.
	///
	/// @param TraitName - The name to refer to the Trait
	/// @param concrete_type_size - The maximum possible size of a concrete type implementing this
	/// Trait
	/// @param functions - The semicolon separated list of functions, in function-pointer syntax,
	/// that the Trait requires to be implemented
	/// @ingroup traits
	#define SizedTrait(TraitName, concrete_type_size, functions) \
		typedef struct TraitName TraitName;                      \
                                                                 \
		DeclTraitVTable(TraitName, functions);                   \
                                                                 \
		typedef struct TraitName {                               \
			const TraitVTable(TraitName) * m_vtable;             \
			u8 m_self[concrete_type_size];                       \
		} TraitName

	/// @brief Implements the given trait for the given type
	///
	/// Implements the given trait for the given type.
	///
	/// @param TraitName - The Trait to implement
	/// @param TypeName - The type implementing the trait
	/// @param ... - The comma separated list of function names with which to implement the trait.
	/// They are required to be in Trait-definition order, and giving them in another order will
	/// result in either a compiler error or undefined behavior at run time (function signature and
	/// order dependent as to which would occur).
	/// @ingroup traits
	#define ImplTraitFor(TraitName, TypeName, ...) \
		ImplTraitVTable(TraitName, TypeName __VA_OPT__(, ) __VA_ARGS__)

	/// @brief Casts the given object to its implementation for the given Trait
	///
	/// Casts `self`, of type `TypeName` to its implementation of the Trait `TraitName`
	///
	/// @param TraitName - The Trait to cast to
	/// @param TypeName - The type to cast from (the type of `self`)
	/// @param self - The object to cast to its Trait implementation
	/// @ingroup traits
	#define as_trait(TraitName, TypeName, self)                             \
		((TraitName){.m_vtable = &(ImpledTraitVTable(TraitName, TypeName)), \
					 .m_self = static_cast(void*)(&(self))})

	/// @brief Casts the given object to its implementation for the given SizedTrait
	///
	/// Casts `self`, of type `TypeName` to its implementation of the SizedTrait `TraitName`
	///
	/// @param TraitName - The SizedTrait to cast to
	/// @param TypeName - The type to cast from (the type of `self`)
	/// @param self - The object to cast to its SizedTrait implementation
	/// @ingroup traits
	#define as_sized_trait(TraitName, TypeName, self)                                 \
		({                                                                            \
			let_mut UNIQUE_VAR(_trait)                                                \
				= (TraitName){.m_vtable = &(ImpledTraitVTable(TraitName, TypeName))}; \
			memcpy(UNIQUE_VAR(_trait).m_self,                                         \
				   static_cast(void*)(&(self)),                                       \
				   sizeof(UNIQUE_VAR(_trait).m_self));                                \
			UNIQUE_VAR(_trait);                                                       \
		})

	/// @brief Calls the trait function `FunctionName` on `self`
	///
	/// Calls the function `FunctionName` on `self`, with the parameters contained in the
	/// `__VA_ARGS__` parameter pack
	///
	/// @param FunctionName - The Trait function to call on `self`
	/// @param self - The Trait object to call `FunctionName` on
	/// @param ... - The parameter pack of (possible, can contain no parameters) additional
	/// parameters to pass to `FunctionName`
	/// @ingroup traits
	#define trait_call(FunctionName, self, ...) \
		(self).m_vtable->FunctionName(&(self)__VA_OPT__(, ) __VA_ARGS__)

#endif // STD_TRAIT
