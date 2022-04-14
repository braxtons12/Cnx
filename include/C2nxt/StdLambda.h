/// @file StdLambda.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief Basic lambda/closure implementation for C.
/// @version 0.1.3
/// @date 2022-04-12
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

#ifndef STD_LAMBDA
#define STD_LAMBDA

#include <C2nxt/StdAllocators.h>
#include <C2nxt/StdAtomic.h>
#include <C2nxt/StdBasicTypes.h>
#include <C2nxt/StdDef.h>

/// @defgroup std_lambda Lambdas/Closures
/// This module provides as-ergonomic-as-possible Lambda/Closure facilities for C.
/// Defining, binding, freeing, and otherwise working with lambdas (even when using a custom
/// allocator) is made as simple as possible.
///
/// "Hello, World!" with Lambdas Example:
/// @code {.c}
/// // we need to name our lambda type in order to take it as a function parameter
/// // (otherwise we'll get incompatible struct errors trying to bind different anonymous structs to
/// // the function parameter)
/// typedef Lambda(void, StdString) LambdaVoidString;
///
/// // declare a lambda function named greet_place that returns `void` and takes in a `StdString`
/// void LambdaFunction(greet_place, StdString greeting) {
/// 	// Retrieve the variable bindings from the lambda.
///		// The binding types (`StdString` here) and the types of the variables passed
/// 	// to the generator macro (`lambda` or `lambda_with_allocator`)
/// 	// __**MUST MATCH**__
/// 	let binding = lambda_binding(StdString);
///		// get the first bound variable
///		let place = binding._1;
///		println("{}, {}!", greeting, place);
/// }
///
/// void greet_with_hello(LambdaVoidString lambda) {
///		// call the lambda with "Hello" as the `greeting`
///		lambda_call(lambda, std_string_from("Hello"));
///		// Free the memory associated w/ the lambda
///		// This will release the binding, and the lambda will no longer be valid
///		lambda_free(lambda);
/// }
///
///	// Prints "Hello, World!"
///	void hello_world(void) {
///		let world = std_string_from("World");
///		// lambdas capture by value, so if you need to bind a pointer,
/// 	// take the address of the variable.
///		// The types of the arguments captured __**MUST MATCH**__ those used
///		// in the binding retrieval in the LambdaFunction's definition
///		// This binds `world` as the `StdString` used as `place` in the
/// 	// `LambdaFunction`'s definition
///		let lambda = lambda(greet_place, world);
///		// lambdas are generated as anonymous structs for brevity, so we have to cast them
///		// to the named type to pass them to a function or store them
///		greet_with_hello(lambda_cast(lambda, LambdaVoidString));
/// }
///
///	// Prints "Hello, C Programmers!"
/// void hello_c_programmers(void) {
///		// multiple lambdas can be bound to a single `LambdaFunction` definition at the same time:
/// 	// the bound data is separate from the `LambdaFunction`'s definition.
///		// Because lambdas capture by value, they can bind either lvalues or rvalues
///		let lambda = lambda(greet_place, std_string_from("C Programmers"));
///		greet_with_hello(lambda_cast(lambda, LambdaVoidString));
/// }
/// @endcode

/// @brief Stores the captures/bindings of a lambda
/// This is implicitly passed as the first parameter to a `LambdaFunction`, but as a user, will
/// never need to be used or interacted with directly
/// @ingroup std_lambda
typedef void* StdLambdaCaptures;

/// @brief The type of a complete lambda instance, binding a lambda function definition with a set
/// of captured variables
///
/// @param ReturnType - The return type of the lambda
/// @param ... - The types of the lambda function arguments
/// @ingroup std_lambda
#define Lambda(ReturnType, ...)                                                 \
	struct {                                                                    \
		ReturnType (*const call)(StdLambdaCaptures __VA_OPT__(, ) __VA_ARGS__); \
		StdLambdaCaptures captures;                                             \
		StdAllocator allocator;                                                 \
	}*

/// @brief A Lambda function definition.
/// Can be bound to an instance of a lambda with `lambda` or `lambda_with_allocator`.
///
/// @param Name - The name of the function
/// @param ... - The function argument list
/// @ingroup std_lambda
#define LambdaFunction(Name, ...) Name(StdLambdaCaptures captures __VA_OPT__(, ) __VA_ARGS__)

/// @brief A type storing a lambda's bound captures
///
/// @param ... - The captured variables (or types of the captures) of the lambda binding
/// @ingroup std_lambda
#define LambdaBinding(...)                                                                   \
	struct {                                                                                 \
		atomic_usize ref_count;                                                              \
		DELIMIT_LIST(;,                                                                      \
					  APPEND_EACH_TO_LIST(SELECTOR_LIST,                                     \
										  APPLY_TO_LIST(typeof __VA_OPT__(, ) __VA_ARGS__))) \
	}

/// @brief Binds the given function and captures as a lambda type, that can be called later
///
/// Binds the given function with the captures list (must capture at least one variable).
/// Captures are captured by value and can be either lvalues or rvalues:
///
/// This version uses the provided memory allocator for memory allocation
///
/// @param alloc - The allocator with which to allocate memory to store the captured values in
/// @param function_name - The function to bind captures to as a lambda
/// @param ... - The list of variables to capture and bind to the lambda
///
/// @return a bound lambda
/// @ingroup std_lambda
#define lambda_with_allocator(alloc, function_name, ...)                                 \
	({                                                                                   \
		let_mut UNIQUE_VAR(binding)                                                      \
			= std_allocator_allocate_t(LambdaBinding(__VA_ARGS__), (alloc));             \
		*UNIQUE_VAR(binding)                                                             \
			= (typeof(*UNIQUE_VAR(binding))){.ref_count = 1 __VA_OPT__(, ) __VA_ARGS__}; \
                                                                                         \
		let_mut UNIQUE_VAR(function) = std_allocator_allocate_t(                         \
			struct {                                                                     \
				typeof(function_name)* call;                                             \
				StdLambdaCaptures captures;                                              \
				StdAllocator allocator;                                                  \
			},                                                                           \
			(alloc));                                                                    \
		*UNIQUE_VAR(function) = (typeof(*UNIQUE_VAR(function))){                         \
			.call = function_name,                                                       \
			.captures = UNIQUE_VAR(binding),                                             \
			.allocator = (alloc),                                                        \
		};                                                                               \
                                                                                         \
		UNIQUE_VAR(function);                                                            \
	}) /** NOLINT(performance-no-int-to-ptr)**/

/// @brief Binds the given function and captures as a lambda type, that can be called later
///
/// Binds the given function with the captures list (must capture at least one variable).
/// Captures are captured by value and can be either lvalues or rvalues:
///
/// This version uses the default system allocator, `DEFAULT_ALLOCATOR`, for memory allocation
///
/// @param function_name - The function to bind captures to as a lambda
/// @param ... - The list of variables to capture and bind to the lambda
///
/// @return a bound lambda
/// @ingroup std_lambda
#define lambda(function_name, ...) \
	lambda_with_allocator(DEFAULT_ALLOCATOR, function_name __VA_OPT__(, ) __VA_ARGS__)

/// @brief Calls the given lambda with the provided arguments as function parameters
///
/// @param lambda - The lambda to call
/// @param ... - The arguments to pass to the lambda function
///
/// @return The return value of the lambda function
/// @ingroup std_lambda
#define lambda_call(lambda, ...) (lambda)->call((lambda)->captures __VA_OPT__(, ) __VA_ARGS__)

/// @brief Returns a clone of the given lambda, ensuring that (by-value) captures stay valid for the
/// lifetime of the clone
///
/// @param lambda - The lambda to clone
///
/// @return a clone of the lambda
///
/// @note Only the lifetime of by-value captures can be extended to match a clone. By-reference
/// captures must still outlive all clones of the lambda, otherwise undefined behavior
/// (likely resulting in segmentation faults from accessing invalid memory) will occur
/// @ingroup std_lambda
#define lambda_clone(lambda)                                                    \
	({                                                                          \
		atomic_fetch_add((static_cast(atomic_usize*)((lambda)->captures)), 1);  \
		let_mut UNIQUE_VAR(new)                                                 \
			= std_allocator_allocate_t(typeof(*(lambda)), (lambda)->allocator); \
		*UNIQUE_VAR(new) = *(lambda);                                           \
		UNIQUE_VAR(new);                                                        \
	})

/// @brief Frees the given lambda, freeing associated memory and making it invalid for future use
///
/// @param lambda - The lambda to free
/// @ingroup std_lambda
[[always_inline]] [[not_null(1)]] static inline void lambda_free(void* lambda) {
	let _lambda = *static_cast(struct {
		void* func;
		StdLambdaCaptures captures;
		StdAllocator allocator;
	}**)(lambda);
	let_mut alloc = _lambda->allocator;
	if(atomic_fetch_sub((static_cast(atomic_usize*)(_lambda->captures)), 1) == 1) {
		std_allocator_deallocate(alloc, const_cast(void*)(_lambda->captures));
	}
	std_allocator_deallocate(alloc, _lambda);
	lambda = nullptr;
}

/// @brief Declaration tag to scope a lambda, ensuring it is freed when it goes out of scoped and
/// associated resources are cleaned up
/// @ingroup std_lambda
#define lambda_scoped scoped(lambda_free)

/// @brief Casts the given `StdLambdaCaptures` to the given `BindingType`
/// @param captures - The `StdLambdaCaptures` to cast
/// @param BindingType - The type to cast to
#define __lambda_binding_cast(captures, BindingType) (*static_cast(BindingType*)((captures)))

/// @brief Retrieves the bound captures from the enclosing lambda
///
/// @param BindingType - The `LambdaBinding` type listing the types of the captured variables
///
/// @return The struct containing the bound captures from the enclosing lambda
/// @ingroup std_lambda
#define lambda_binding(...) __lambda_binding_cast(captures, LambdaBinding(__VA_ARGS__))

/// @brief Casts the given lambda to the given named type,
/// so it can be passed to a function or stored
///
/// @param lambda - The lambda to cast
/// @param Type - The __named__ `Lambda` type to cast to. This must be a named typedef in order
/// for the lambda to be able to be passed as a function parameter or stored
///
/// @return `lambda` as `Type`
/// @ingroup std_lambda
#define lambda_cast(lambda, Type) (static_cast(Type)((lambda)))
#endif // STD_LAMBDA
