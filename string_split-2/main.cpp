
// MIT License
//
// Copyright (c) 2018, 2019 degski
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstdlib>

#include <array>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <list>
#include <map>
#include <random>
#include <string>
#include <type_traits>
#include <vector>


namespace fs = std::filesystem;


#include <string_split.hpp>

template<typename CharT>
constexpr bool starts_with ( std::basic_string_view<CharT> s, std::basic_string_view<CharT> x ) noexcept {
    return s.size ( ) >= x.size ( ) and s.compare ( 0, x.size ( ), x ) == 0;
}
template<typename CharT>
constexpr bool starts_with ( std::basic_string_view<CharT> s, CharT x ) noexcept {
    return starts_with ( s, std::basic_string_view<CharT> ( std::addressof ( x ), 1 ) );
}
template<typename CharT>
constexpr bool starts_with ( std::basic_string_view<CharT> s, const CharT * x ) noexcept {
    return starts_with ( s, std::basic_string_view<CharT> ( x ) );
}
template<typename CharT>
constexpr bool ends_with ( std::basic_string_view<CharT> s, std::basic_string_view<CharT> x ) noexcept {
    return s.size ( ) >= x.size ( ) && s.compare ( s.size ( ) - x.size ( ), std::basic_string_view<CharT>::npos, x ) == 0;
}
template<typename CharT>
constexpr bool ends_with ( std::basic_string_view<CharT> s, CharT x ) noexcept {
    return ends_with ( s, std::basic_string_view<CharT> ( std::addressof ( x ), 1 ) );
}
template<typename CharT>
constexpr bool ends_with ( std::basic_string_view<CharT> s, const CharT * x ) noexcept {
    return ends_with ( s, std::basic_string_view<CharT> ( x ) );
}

template<typename CharT>
constexpr void remove_starts_with ( std::basic_string_view<CharT> & s, bool & removed, std::basic_string_view<CharT> x ) noexcept {
    if ( starts_with ( s, x ) ) {
        s.remove_prefix ( x.size ( ) );
        removed = removed or true;
    };
}
template<typename CharT>
constexpr void remove_starts_with ( std::basic_string_view<CharT> s, bool & removed, CharT x ) noexcept {
    remove_starts_with ( s, removed, std::basic_string_view<CharT> ( std::addressof ( x ), 1 ) );
}
template<typename CharT>
constexpr void remove_starts_with ( std::basic_string_view<CharT> & s, bool & removed, const CharT * x ) noexcept {
    remove_starts_with ( s, removed, std::basic_string_view<CharT> ( x ) );
}


template<typename ... Args>
void remove_prefix ( std::string_view & v_, Args ... args_ ) noexcept {
    bool removed;
    do {
        removed = false;
        ( remove_starts_with ( v_, removed, std::forward<Args> ( args_ ) ), ... );
    } while ( removed ); // Keep removing untill nothing more can be removed.
}


template<typename CharT>
constexpr void find_first_of ( std::basic_string_view<CharT> s, std::size_t & f_, std::basic_string_view<CharT> x ) noexcept {
    f_ = std::min ( s.find_first_of ( x ), f_ );
}
template<typename CharT>
constexpr void find_first_of ( std::basic_string_view<CharT> s, std::size_t & f_, CharT x ) noexcept {
    f_ = std::min ( s.find_first_of ( std::basic_string_view<CharT> ( std::addressof ( x ), 1 ) ), f_ );
}
template<typename CharT>
constexpr void find_first_of ( std::basic_string_view<CharT> s, std::size_t & f_, const CharT * x ) noexcept {
    f_ = std::min ( s.find_first_of ( std::basic_string_view<CharT> ( x ) ), f_ );
}

template<typename CharT, typename ... Args>
constexpr std::size_t find_first_of ( std::basic_string_view<CharT> & v_, Args ... args_ ) noexcept {
    std::size_t found = std::basic_string_view<CharT>::npos;
    ( find_first_of ( v_, found, std::forward<Args> ( args_ ) ), ... );
    return found;
}




int main ( ) {

    std::string s ( " , \t and the quick brown fox jumps over the lazy dog" );

    std::string_view v ( s );

    remove_prefix ( v, " ", ",", "\t", "and" );

    std::cout << '*' << v << '*' << nl;

    std::size_t p = 0;

    std::cout << ( p = find_first_of ( v, " ", ",", "\t", "and" ) ) << nl;

    std::string_view f = v.substr ( 0, p );

    std::cout << f << nl;

    v.remove_prefix ( p );
    remove_prefix ( v, " ", ",", "\t", "and" );

    std::cout << ( p = find_first_of ( v, " ", ",", "\t", "and" ) ) << nl;

    f = v.substr ( 0, p );

    std::cout << f << nl;

    return EXIT_SUCCESS;
}
