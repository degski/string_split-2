
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
constexpr void remove_starts_with ( std::basic_string_view<CharT> & s, bool & removed, CharT x ) noexcept {
    remove_starts_with ( s, removed, std::basic_string_view<CharT> ( std::addressof ( x ), 1 ) );
}
template<typename CharT>
constexpr void remove_starts_with ( std::basic_string_view<CharT> & s, bool & removed, const CharT * x ) noexcept {
    remove_starts_with ( s, removed, std::basic_string_view<CharT> ( x ) );
}

template<typename CharT>
constexpr void remove_ends_with ( std::basic_string_view<CharT> & s, bool & removed, std::basic_string_view<CharT> x ) noexcept {
    if ( ends_with ( s, x ) ) {
        s.remove_suffix ( x.size ( ) );
        removed = removed or true;
    };
}
template<typename CharT>
constexpr void remove_ends_with ( std::basic_string_view<CharT> & s, bool & removed, CharT x ) noexcept {
    remove_ends_with ( s, removed, std::basic_string_view<CharT> ( std::addressof ( x ), 1 ) );
}
template<typename CharT>
constexpr void remove_ends_with ( std::basic_string_view<CharT> & s, bool & removed, const CharT * x ) noexcept {
    remove_ends_with ( s, removed, std::basic_string_view<CharT> ( x ) );
}



template<typename CharT, typename ... Args>
void remove_prefix ( std::basic_string_view<CharT> & v_, Args ... args_ ) noexcept {
    bool removed;
    do {
        removed = false;
        ( remove_starts_with ( v_, removed, std::forward<Args> ( args_ ) ), ... );
    } while ( removed ); // Keep removing untill nothing more can be removed.
}

template<typename CharT, typename ... Args>
void remove_suffix ( std::basic_string_view<CharT> & v_, Args ... args_ ) noexcept {
    bool removed;
    do {
        removed = false;
        ( remove_ends_with ( v_, removed, std::forward<Args> ( args_ ) ), ... );
    } while ( removed ); // Keep removing untill nothing more can be removed.
}


template<typename CharT, typename SizeT>
constexpr void find_first_of ( std::basic_string_view<CharT> s, SizeT & f_, std::basic_string_view<CharT> x ) noexcept {
    f_ = std::min ( s.find_first_of ( x ), f_ );
}
template<typename CharT, typename SizeT>
constexpr void find_first_of ( std::basic_string_view<CharT> s, SizeT & f_, CharT x ) noexcept {
    f_ = std::min ( s.find_first_of ( std::basic_string_view<CharT> ( std::addressof ( x ), 1 ) ), f_ );
}
template<typename CharT, typename SizeT>
constexpr void find_first_of ( std::basic_string_view<CharT> s, SizeT & f_, const CharT * x ) noexcept {
    f_ = std::min ( s.find_first_of ( std::basic_string_view<CharT> ( x ) ), f_ );
}

template<typename CharT, typename ... Args>
constexpr auto find_first_of ( std::basic_string_view<CharT> & v_, Args ... args_ ) noexcept {
    auto found = std::basic_string_view<CharT>::npos;
    ( find_first_of ( v_, found, std::forward<Args> ( args_ ) ), ... );
    return found;
}

template<typename CharT, typename ... Delimiters>
[[ nodiscard ]] std::vector<std::basic_string_view<CharT>> string_split ( const std::basic_string<CharT> & string_, Delimiters ... delimiters_ ) {
    using size_type = typename std::basic_string_view<CharT>::size_type;
    std::basic_string_view<CharT> string_view ( string_ );
    std::vector<std::basic_string_view<CharT>> string_view_vector;
    string_view_vector.reserve ( 4 );
    // Remove trailing and leading delimiters.
    remove_prefix ( string_view, std::forward<Delimiters> ( delimiters_ ) ... );
    remove_suffix ( string_view, std::forward<Delimiters> ( delimiters_ ) ... );
    // Parse the string_view left to right.
    while ( true ) {
        const size_type pos = find_first_of ( string_view, std::forward<Delimiters> ( delimiters_ ) ... );
        if ( std::basic_string_view<CharT>::npos == pos ) {
            string_view_vector.emplace_back ( std::move ( string_view ) );
            break;
        }
        string_view_vector.emplace_back ( string_view.data ( ), pos );
        string_view.remove_prefix ( pos );
        remove_prefix ( string_view, std::forward<Delimiters> ( delimiters_ ) ... );
    }
    return string_view_vector;
}



template<typename Stream, typename Container>
Stream & operator << ( Stream & out_, const Container & v_ ) noexcept {
    for ( const auto & v : v_ )
        out_ << '\"' << v << "\" ";
    out_ << '\b';
    return out_;
}


int main ( ) {

    std::string s ( " , \t the quick brown fox jumps over the lazy dog      ," );

    auto split = string_split ( s, ' ', ',', '\t' );

    std::cout << split << nl;

    return EXIT_SUCCESS;
}
