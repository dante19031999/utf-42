/**
 * @file test.cpp
 * @brief Testing tools for this library.
 *
 * @warning Ensure that the appropriate character type is used when
 * instantiating the stream classes. This file requires C++11 or later.
 *
 * @copyright MIT License
 *
 * This file is part of utf42.
 * Copyright (c) 2025 Dante Doménech Martínez
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <cassert>
#include <iostream>
#include <string>
#include <utf8cpp/utf8.h>

#include "utf42.h"

/**
 * @brief Converts a UTF-8 encoded `std::basic_string_view<char8_t>` to a `std::string`.
 *
 * @param sText A constant reference to a `std::basic_string_view<char8_t>` to convert.
 * @return A `std::string` containing the converted characters.
 *
 * @note Uses `std::memcpy` for direct copying when sizes match; otherwise, it converts
 *       characters individually. Be aware of potential data loss with invalid UTF-8.
 */
inline std::string char8_to_char(const std::basic_string_view<char8_t> &sText) {
    std::string sResult(sText.length(), '\0');
    if constexpr (sizeof(char) == sizeof(char8_t)) {
        std::memcpy(sResult.data(), sText.data(), sText.length() * sizeof(char));
    } else {
        for (std::size_t i = 0; i < sText.length(); ++i) {
            sResult[i] = static_cast<char>(sText[i]);
        }
    }
    return sResult;
}

/**
 * @brief Custom assert that displays the strings
 * @param s1 First string
 * @param s2 Second string
 */
void custom_assert(const std::string &s1, const std::string &s2) {
    if (s1 != s2) {
        std::cerr << s1 << " != " << s2 << std::endl;
        std::abort();
    }
}

/**
 * @brief Performs simple tests
 */
void test_simple() {
    constexpr std::basic_string_view<char> strv_a = make_poly_enc(char, "Hello World \U0001F600!");
    constexpr std::basic_string_view<char8_t> strv_8 = make_poly_enc(char8_t, "Hello World \U0001F600!");
    constexpr std::basic_string_view<char16_t> strv_16 = make_poly_enc(char16_t, "Hello World \U0001F600!");
    constexpr std::basic_string_view<char32_t> strv_32 = make_poly_enc(char32_t, "Hello World \U0001F600!");

    const std::string str_a(strv_a);
    const std::string str_8(char8_to_char(strv_8));
    const std::string str_16(utf8::utf16to8(strv_16));
    const std::string str_32(utf8::utf32to8(strv_32));

    custom_assert(str_a, str_8);
    custom_assert(str_a, str_16);
    custom_assert(str_a, str_32);
}

/**
 * @brief Performs tests with typename
 */
void test_template() {
    using char1_t = char;
    using char2_t = char8_t;
    using char3_t = char16_t;
    using char4_t = char32_t;

    constexpr std::basic_string_view<char1_t> strv_a = make_poly_enc(char, "Hello World \U0001F600!");
    constexpr std::basic_string_view<char2_t> strv_8 = make_poly_enc(char8_t, "Hello World \U0001F600!");
    constexpr std::basic_string_view<char3_t> strv_16 = make_poly_enc(char16_t, "Hello World \U0001F600!");
    constexpr std::basic_string_view<char4_t> strv_32 = make_poly_enc(char32_t, "Hello World \U0001F600!");

    const std::string str_a(strv_a);
    const std::string str_8(char8_to_char(strv_8));
    const std::string str_16(utf8::utf16to8(strv_16));
    const std::string str_32(utf8::utf32to8(strv_32));

    custom_assert(str_a, str_8);
    custom_assert(str_a, str_16);
    custom_assert(str_a, str_32);
}

/**
 * @brief Main function
 * @return Exit status
 */
int main() {
    std::cout << "Performing tests..." << std::endl;
    test_simple();
    test_template();
    std::cout << "Tests passed!" << std::endl;
    return 0;
}
