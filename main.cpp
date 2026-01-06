/**
 * @file main.cpp
 * @brief Example code for this library.
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

#include <iostream>
#include <string>
#include <utf8cpp/utf8.h>

#include "utf42.h"

#if __cplusplus >= 202002L
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
#endif

/**
 * @brief First example
 */
void example1();

/**
 * @brief Second example
 */
void example2();

/**
 * @brief Main function
 * @return Exit status
 */
int main() {
    example1();
    example2();
    return 0;
}

#if __cplusplus <= 201402L
namespace std {
    ///< Non-standard typedef in order to recycle code
    template<typename char_t>
    using basic_string_view = utf42::basic_string_view<char_t>;
}
#endif


void example1() {
    // Example code
    using char42_t = char32_t;

    // Create different encoding string views
    constexpr std::basic_string_view<char> strv_a = make_poly_enc(char, "Hello World \U0001F600!");
#if __cplusplus >= 202002L
    constexpr std::basic_string_view<char8_t> strv_8 = make_poly_enc(char8_t, "Hello World \U0001F600!");
#endif
    constexpr std::basic_string_view<char16_t> strv_16 = make_poly_enc(char16_t, "Hello World \U0001F600!");
    constexpr std::basic_string_view<char32_t> strv_32 = make_poly_enc(char32_t, "Hello World \U0001F600!");
    constexpr std::basic_string_view<char42_t> strv_42 = make_poly_enc(char32_t, "Hello World \U0001F600!");

#if __cplusplus >= 201703L
    // Re-encode everything to utf-8
    const std::string str_a(strv_a);
#if __cplusplus >= 202002L
    const std::string str_8(char8_to_char(strv_8));
#endif
    const std::string str_16(utf8::utf16to8(strv_16));
    const std::string str_32(utf8::utf32to8(strv_32));
    const std::string str_42(utf8::utf32to8(strv_42));
#elif   __cplusplus >= 201103L
    // Re-encode everything to utf-8
    const std::string str_a(strv_a.str());
    const std::string str_16(utf8::utf16to8(strv_16.str()));
    const std::string str_32(utf8::utf32to8(strv_32.str()));
    const std::string str_42(utf8::utf32to8(strv_42.str()));
#endif
    // Display everything on the terminal
    std::cout << "Original: " << str_a << std::endl;
#if __cplusplus >= 202002L
    std::cout << "utf-8:    " << str_8 << std::endl;
#endif
    std::cout << "utf-16:   " << str_16 << std::endl;
    std::cout << "utf-32:   " << str_32 << std::endl;
    std::cout << "utf-42:   " << str_42 << std::endl;
}

void example2() {
    // Example code
    using char42_t = char32_t;

    // Create all different encoding string views
    constexpr utf42::poly_enc oText = cons_poly_enc("Hello World \U0001F600!");

#if __cplusplus >= 201703L
    // Re-encode everything to utf-8
    const std::string str_a(oText.TXT_CHAR);
#if __cplusplus >= 202002L
    const std::string str_8(char8_to_char(oText.TXT_CHAR_8));
#endif
    const std::string str_16(utf8::utf16to8(oText.TXT_CHAR_16));
    const std::string str_32(utf8::utf32to8(oText.TXT_CHAR_32));
    const std::string str_42(utf8::utf32to8(oText.visit<char42_t>()));
#elif   __cplusplus >= 201103L
    // Re-encode everything to utf-8
    const std::string str_a(oText.TXT_CHAR.str());
    const std::string str_16(utf8::utf16to8(oText.TXT_CHAR_16.str()));
    const std::string str_32(utf8::utf32to8(oText.TXT_CHAR_32.str()));
    const std::string str_42(utf8::utf32to8(oText.visit<char42_t>().str()));
#endif


    // Display everything on the terminal
    std::cout << "Original: " << str_a << std::endl;
#if __cplusplus >= 202002L
    std::cout << "utf-8:    " << str_8 << std::endl;
#endif
    std::cout << "utf-16:   " << str_16 << std::endl;
    std::cout << "utf-32:   " << str_32 << std::endl;
    std::cout << "utf-42:   " << str_42 << std::endl;
}
