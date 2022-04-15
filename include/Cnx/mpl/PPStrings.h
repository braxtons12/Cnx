/// @file PPStrings.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief PPStrings provides metaprogramming macros for operating on/with text/tokens
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

#ifndef CNX_PP_STRINGS
#define CNX_PP_STRINGS

/// @defgroup cnx_pp_strings Token Metaprogramming
/// This module provides various macros for metaprogramming on/with tokens/text

/// @brief Concatenates zero arguments
///
/// @param ... - The arguments that will not be concatenated
///
/// @return nothing
/// @ingroup cnx_pp_strings
#define CONCAT0(...)

/// @brief Concatenates nothing to one argument (returns the given argument)
///
/// @param A - the argument to concatenate nothing to
///
/// @return `A`
/// @ingroup cnx_pp_strings
#define CONCAT1(A) A

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
/// @ingroup cnx_pp_strings
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
/// @ingroup cnx_pp_strings
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
/// @ingroup cnx_pp_strings
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
/// @ingroup cnx_pp_strings
#define CONCAT4(A, B, C, D) ___CONCAT4(A, B, C, D)

/// @brief Concatenates the five arguments
///
/// `A`, `B`, `C`, `D`, `E` must be alphanumeric, macro-concatenation compatible sequences of
/// characters
///
/// @param A - the first portion of the concatenation
/// @param B - the second portion of the concatenation
/// @param C - the third portion of the concatenation
/// @param D - the fourth portion of the concatenation
/// @param E - the fifth portion of the concatenation
///
/// @return the concatenation of `A`, `B`, `C`, `D`, `E`
#define ___CONCAT5(A, B, C, D, E) CONCAT2(A, CONCAT4(B, C, D, E))
/// @brief Concatenates the five arguments
///
/// `A`, `B`, `C`, `D`, `E` must be alphanumeric, macro-concatenation compatible sequences of
/// characters
///
/// @param A - the first portion of the concatenation
/// @param B - the second portion of the concatenation
/// @param C - the third portion of the concatenation
/// @param D - the fourth portion of the concatenation
/// @param E - the fifth portion of the concatenation
///
/// @return the concatenation of `A`, `B`, `C`, `D`, `E`
/// @ingroup cnx_pp_strings
#define CONCAT5(A, B, C, D, E) ___CONCAT5(A, B, C, D, E)

/// @brief Concatenates the 6 arguments
///
/// `A`, `B`, `C`, `D`, `E`, `F` must be alphanumeric, macro-concatenation compatible sequences of
/// characters
///
/// @param A - the first portion of the concatenation
/// @param B - the second portion of the concatenation
/// @param C - the third portion of the concatenation
/// @param D - the fourth portion of the concatenation
/// @param E - the fifth portion of the concatenation
/// @param F - the sixth portion of the concatenation
///
/// @return the concatenation of `A`, `B`, `C`, `D`, `E`, `F`
#define ___CONCAT6(A, B, C, D, E, F) CONCAT2(A, CONCAT5(B, C, D, E, F))
/// @brief Concatenates the 6 arguments
///
/// `A`, `B`, `C`, `D`, `E`, `F` must be alphanumeric, macro-concatenation compatible sequences of
/// characters
///
/// @param A - the first portion of the concatenation
/// @param B - the second portion of the concatenation
/// @param C - the third portion of the concatenation
/// @param D - the fourth portion of the concatenation
/// @param E - the fifth portion of the concatenation
/// @param F - the sixth portion of the concatenation
///
/// @return the concatenation of `A`, `B`, `C`, `D`, `E`, `F`
/// @ingroup cnx_pp_strings
#define CONCAT6(A, B, C, D, E, F) ___CONCAT6(A, B, C, D, E, F)

/// @brief Concatenates the 7 arguments
///
/// `A`, `B`, `C`, `D`, `E`, `F`, `G` must be alphanumeric, macro-concatenation compatible sequences
/// of characters
///
/// @param A - the first portion of the concatenation
/// @param B - the second portion of the concatenation
/// @param C - the third portion of the concatenation
/// @param D - the fourth portion of the concatenation
/// @param E - the fifth portion of the concatenation
/// @param F - the sixth portion of the concatenation
/// @param G - the seventh portion of the concatenation
///
/// @return the concatenation of `A`, `B`, `C`, `D`, `E`, `F`, `G`
#define ___CONCAT7(A, B, C, D, E, F, G) CONCAT2(A, CONCAT6(B, C, D, E, F, G))
/// @brief Concatenates the 7 arguments
///
/// `A`, `B`, `C`, `D`, `E`, `F`, `G` must be alphanumeric, macro-concatenation compatible sequences
/// of characters
///
/// @param A - the first portion of the concatenation
/// @param B - the second portion of the concatenation
/// @param C - the third portion of the concatenation
/// @param D - the fourth portion of the concatenation
/// @param E - the fifth portion of the concatenation
/// @param F - the sixth portion of the concatenation
/// @param G - the seventh portion of the concatenation
///
/// @return the concatenation of `A`, `B`, `C`, `D`, `E`, `F`, `G`
/// @ingroup cnx_pp_strings
#define CONCAT7(A, B, C, D, E, F, G) ___CONCAT7(A, B, C, D, E, F, G)

/// @brief Concatenates the 8 arguments
///
/// `A`, `B`, `C`, `D`, `E`, `F`, `G`, `H` must be alphanumeric, macro-concatenation compatible
/// sequences of characters
///
/// @param A - the first portion of the concatenation
/// @param B - the second portion of the concatenation
/// @param C - the third portion of the concatenation
/// @param D - the fourth portion of the concatenation
/// @param E - the fifth portion of the concatenation
/// @param F - the sixth portion of the concatenation
/// @param G - the seventh portion of the concatenation
/// @param H - the eight portion of the concatenation
///
/// @return the concatenation of `A`, `B`, `C`, `D`, `E`, `F`, `G`, `H`
#define ___CONCAT8(A, B, C, D, E, F, G, H) CONCAT2(A, CONCAT7(B, C, D, E, F, G, H))
/// @brief Concatenates the 8 arguments
///
/// `A`, `B`, `C`, `D`, `E`, `F`, `G`, `H` must be alphanumeric, macro-concatenation compatible
/// sequences of characters
///
/// @param A - the first portion of the concatenation
/// @param B - the second portion of the concatenation
/// @param C - the third portion of the concatenation
/// @param D - the fourth portion of the concatenation
/// @param E - the fifth portion of the concatenation
/// @param F - the sixth portion of the concatenation
/// @param G - the seventh portion of the concatenation
/// @param H - the eight portion of the concatenation
///
/// @return the concatenation of `A`, `B`, `C`, `D`, `E`, `F`, `G`, `H`
/// @ingroup cnx_pp_strings
#define CONCAT8(A, B, C, D, E, F, G, H) ___CONCAT8(A, B, C, D, E, F, G, H)

/// @brief Concatenates the 9 arguments
///
/// `A`, `B`, `C`, `D`, `E`, `F`, `G`, `H`, `I` must be alphanumeric, macro-concatenation compatible
/// sequences of characters
///
/// @param A - the first portion of the concatenation
/// @param B - the second portion of the concatenation
/// @param C - the third portion of the concatenation
/// @param D - the fourth portion of the concatenation
/// @param E - the fifth portion of the concatenation
/// @param F - the sixth portion of the concatenation
/// @param G - the seventh portion of the concatenation
/// @param H - the eight portion of the concatenation
/// @param I - the ninth portion of the concatenation
///
/// @return the concatenation of `A`, `B`, `C`, `D`, `E`, `F`, `G`, `H`, `I`
#define ___CONCAT9(A, B, C, D, E, F, G, H, I) CONCAT2(A, CONCAT8(B, C, D, E, F, G, H, I))
/// @brief Concatenates the 9 arguments
///
/// `A`, `B`, `C`, `D`, `E`, `F`, `G`, `H`, `I` must be alphanumeric, macro-concatenation compatible
/// sequences of characters
///
/// @param A - the first portion of the concatenation
/// @param B - the second portion of the concatenation
/// @param C - the third portion of the concatenation
/// @param D - the fourth portion of the concatenation
/// @param E - the fifth portion of the concatenation
/// @param F - the sixth portion of the concatenation
/// @param G - the seventh portion of the concatenation
/// @param H - the eight portion of the concatenation
/// @param I - the ninth portion of the concatenation
///
/// @return the concatenation of `A`, `B`, `C`, `D`, `E`, `F`, `G`, `H`, `I`
/// @ingroup cnx_pp_strings
#define CONCAT9(A, B, C, D, E, F, G, H, I) ___CONCAT9(A, B, C, D, E, F, G, H, I)

/// @brief Concatenates the 10 arguments
///
/// `A`, `B`, `C`, `D`, `E`, `F`, `G`, `H`, `I`, `J` must be alphanumeric, macro-concatenation
/// compatible sequences of characters
///
/// @param A - the first portion of the concatenation
/// @param B - the second portion of the concatenation
/// @param C - the third portion of the concatenation
/// @param D - the fourth portion of the concatenation
/// @param E - the fifth portion of the concatenation
/// @param F - the sixth portion of the concatenation
/// @param G - the seventh portion of the concatenation
/// @param H - the eight portion of the concatenation
/// @param I - the ninth portion of the concatenation
/// @param J - the tenth portion of the concatenation
///
/// @return the concatenation of `A`, `B`, `C`, `D`, `E`, `F`, `G`, `H`, `I`, `J`
#define ___CONCAT10(A, B, C, D, E, F, G, H, I, J) CONCAT2(A, CONCAT9(B, C, D, E, F, G, H, I, J))
/// @brief Concatenates the 10 arguments
///
/// `A`, `B`, `C`, `D`, `E`, `F`, `G`, `H`, `I`, `J` must be alphanumeric, macro-concatenation
/// compatible sequences of characters
///
/// @param A - the first portion of the concatenation
/// @param B - the second portion of the concatenation
/// @param C - the third portion of the concatenation
/// @param D - the fourth portion of the concatenation
/// @param E - the fifth portion of the concatenation
/// @param F - the sixth portion of the concatenation
/// @param G - the seventh portion of the concatenation
/// @param H - the eight portion of the concatenation
/// @param I - the ninth portion of the concatenation
/// @param J - the tenth portion of the concatenation
///
/// @return the concatenation of `A`, `B`, `C`, `D`, `E`, `F`, `G`, `H`, `I`, `J`
/// @ingroup cnx_pp_strings
#define CONCAT10(A, B, C, D, E, F, G, H, I, J) ___CONCAT10(A, B, C, D, E, F, G, H, I, J)

/// @brief Concatenates the 11 arguments
///
/// `A`, `B`, `C`, `D`, `E`, `F`, `G`, `H`, `I`, `J`, `K` must be alphanumeric, macro-concatenation
/// compatible sequences of characters
///
/// @param A - the first portion of the concatenation
/// @param B - the second portion of the concatenation
/// @param C - the third portion of the concatenation
/// @param D - the fourth portion of the concatenation
/// @param E - the fifth portion of the concatenation
/// @param F - the sixth portion of the concatenation
/// @param G - the seventh portion of the concatenation
/// @param H - the eight portion of the concatenation
/// @param I - the ninth portion of the concatenation
/// @param J - the tenth portion of the concatenation
/// @param K - the eleventh portion of the concatenation
///
/// @return the concatenation of `A`, `B`, `C`, `D`, `E`, `F`, `G`, `H`, `I`, `J`, `K`
#define ___CONCAT11(A, B, C, D, E, F, G, H, I, J, K) \
	CONCAT2(A, CONCAT10(B, C, D, E, F, G, H, I, J, K))
/// @brief Concatenates the 11 arguments
///
/// `A`, `B`, `C`, `D`, `E`, `F`, `G`, `H`, `I`, `J`, `K` must be alphanumeric, macro-concatenation
/// compatible sequences of characters
///
/// @param A - the first portion of the concatenation
/// @param B - the second portion of the concatenation
/// @param C - the third portion of the concatenation
/// @param D - the fourth portion of the concatenation
/// @param E - the fifth portion of the concatenation
/// @param F - the sixth portion of the concatenation
/// @param G - the seventh portion of the concatenation
/// @param H - the eight portion of the concatenation
/// @param I - the ninth portion of the concatenation
/// @param J - the tenth portion of the concatenation
/// @param K - the eleventh portion of the concatenation
///
/// @return the concatenation of `A`, `B`, `C`, `D`, `E`, `F`, `G`, `H`, `I`, `J`, `K`
/// @ingroup cnx_pp_strings
#define CONCAT11(A, B, C, D, E, F, G, H, I, J, K) ___CONCAT11(A, B, C, D, E, F, G, H, I, J, K)

/// @brief Concatenates the 12 arguments
///
/// `A`, `B`, `C`, `D`, `E`, `F`, `G`, `H`, `I`, `J`, `K`, `L` must be alphanumeric,
/// macro-concatenation compatible sequences of characters
///
/// @param A - the first portion of the concatenation
/// @param B - the second portion of the concatenation
/// @param C - the third portion of the concatenation
/// @param D - the fourth portion of the concatenation
/// @param E - the fifth portion of the concatenation
/// @param F - the sixth portion of the concatenation
/// @param G - the seventh portion of the concatenation
/// @param H - the eight portion of the concatenation
/// @param I - the ninth portion of the concatenation
/// @param J - the tenth portion of the concatenation
/// @param K - the eleventh portion of the concatenation
/// @param L - the twelfth portion of the concatenation
///
/// @return the concatenation of `A`, `B`, `C`, `D`, `E`, `F`, `G`, `H`, `I`, `J`, `K`, `L`
#define ___CONCAT12(A, B, C, D, E, F, G, H, I, J, K, L) \
	CONCAT2(A, CONCAT11(B, C, D, E, F, G, H, I, J, K, L))
/// @brief Concatenates the 12 arguments
///
/// `A`, `B`, `C`, `D`, `E`, `F`, `G`, `H`, `I`, `J`, `K`, `L` must be alphanumeric,
/// macro-concatenation compatible sequences of characters
///
/// @param A - the first portion of the concatenation
/// @param B - the second portion of the concatenation
/// @param C - the third portion of the concatenation
/// @param D - the fourth portion of the concatenation
/// @param E - the fifth portion of the concatenation
/// @param F - the sixth portion of the concatenation
/// @param G - the seventh portion of the concatenation
/// @param H - the eight portion of the concatenation
/// @param I - the ninth portion of the concatenation
/// @param J - the tenth portion of the concatenation
/// @param K - the eleventh portion of the concatenation
/// @param L - the twelfth portion of the concatenation
///
/// @return the concatenation of `A`, `B`, `C`, `D`, `E`, `F`, `G`, `H`, `I`, `J`, `K`, `L`
/// @ingroup cnx_pp_strings
#define CONCAT12(A, B, C, D, E, F, G, H, I, J, K, L) ___CONCAT12(A, B, C, D, E, F, G, H, I, J, K, L)

/// @brief Concatenates the 13 arguments
///
/// `A`, `B`, `C`, `D`, `E`, `F`, `G`, `H`, `I`, `J`, `K`, `L`, `M` must be alphanumeric,
/// macro-concatenation compatible sequences of characters
///
/// @param A - the first portion of the concatenation
/// @param B - the second portion of the concatenation
/// @param C - the third portion of the concatenation
/// @param D - the fourth portion of the concatenation
/// @param E - the fifth portion of the concatenation
/// @param F - the sixth portion of the concatenation
/// @param G - the seventh portion of the concatenation
/// @param H - the eight portion of the concatenation
/// @param I - the ninth portion of the concatenation
/// @param J - the tenth portion of the concatenation
/// @param K - the eleventh portion of the concatenation
/// @param L - the twelfth portion of the concatenation
/// @param M - the thirteenth portion of the concatenation
///
/// @return the concatenation of `A`, `B`, `C`, `D`, `E`, `F`, `G`, `H`, `I`, `J`, `K`, `L`, `M`
#define ___CONCAT13(A, B, C, D, E, F, G, H, I, J, K, L, M) \
	CONCAT2(A, CONCAT12(B, C, D, E, F, G, H, I, J, K, L, M))
/// @brief Concatenates the 13 arguments
///
/// `A`, `B`, `C`, `D`, `E`, `F`, `G`, `H`, `I`, `J`, `K`, `L`, `M` must be alphanumeric,
/// macro-concatenation compatible sequences of characters
///
/// @param A - the first portion of the concatenation
/// @param B - the second portion of the concatenation
/// @param C - the third portion of the concatenation
/// @param D - the fourth portion of the concatenation
/// @param E - the fifth portion of the concatenation
/// @param F - the sixth portion of the concatenation
/// @param G - the seventh portion of the concatenation
/// @param H - the eight portion of the concatenation
/// @param I - the ninth portion of the concatenation
/// @param J - the tenth portion of the concatenation
/// @param K - the eleventh portion of the concatenation
/// @param L - the twelfth portion of the concatenation
/// @param M - the thirteenth portion of the concatenation
///
/// @return the concatenation of `A`, `B`, `C`, `D`, `E`, `F`, `G`, `H`, `I`, `J`, `K`, `L`, `M`
/// @ingroup cnx_pp_strings
#define CONCAT13(A, B, C, D, E, F, G, H, I, J, K, L, M) \
	___CONCAT13(A, B, C, D, E, F, G, H, I, J, K, L, M)

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
/// @ingroup cnx_pp_strings
#define AS_STRING(x) ___AS_STRING(x)

#endif // CNX_PP_STRINGS
