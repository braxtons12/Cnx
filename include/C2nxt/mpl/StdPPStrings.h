/// @file StdPPStrings.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief StdPPStrings provides metaprogramming macros for operating on/with text/tokens
/// @version 0.1
/// @date 2022-01-02
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

#ifndef STD_PP_STRINGS
#define STD_PP_STRINGS

/// @defgroup std_pp_strings Token Metaprogramming
/// This module provides various macros for metaprogramming on/with tokens/text

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
/// @ingroup std_pp_strings
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
/// @ingroup std_pp_strings
#define CONCAT2_DEFERRED(A, B) CONCAT2(A, B)

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
/// @ingroup std_pp_strings
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
/// @ingroup std_pp_strings
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
/// @ingroup std_pp_strings
#define AS_STRING(x) ___AS_STRING(x)

#endif // STD_PP_STRINGS
