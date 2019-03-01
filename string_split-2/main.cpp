
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
[[ nodiscard ]] constexpr bool starts_with ( std::basic_string_view<CharT> s, std::basic_string_view<CharT> x ) noexcept {
    return s.size ( ) >= x.size ( ) and s.compare ( 0, x.size ( ), x ) == 0;
}
template<typename CharT>
[[ nodiscard ]] constexpr bool starts_with ( std::basic_string_view<CharT> s, CharT x ) noexcept {
    return starts_with ( s, std::basic_string_view<CharT> ( std::addressof ( x ), 1 ) );
}
template<typename CharT>
[[ nodiscard ]] constexpr bool starts_with ( std::basic_string_view<CharT> s, const CharT * x ) noexcept {
    return starts_with ( s, std::basic_string_view<CharT> ( x ) );
}
template<typename CharT>
[[ nodiscard ]] constexpr bool ends_with ( std::basic_string_view<CharT> s, std::basic_string_view<CharT> x ) noexcept {
    return s.size ( ) >= x.size ( ) && s.compare ( s.size ( ) - x.size ( ), std::basic_string_view<CharT>::npos, x ) == 0;
}
template<typename CharT>
[[ nodiscard ]] constexpr bool ends_with ( std::basic_string_view<CharT> s, CharT x ) noexcept {
    return ends_with ( s, std::basic_string_view<CharT> ( std::addressof ( x ), 1 ) );
}
template<typename CharT>
[[ nodiscard ]] constexpr bool ends_with ( std::basic_string_view<CharT> s, const CharT * x ) noexcept {
    return ends_with ( s, std::basic_string_view<CharT> ( x ) );
}







template<typename CharT, typename StringyThing>
[[ nodiscard ]] constexpr std::basic_string_view<CharT> make_string_view ( StringyThing x ) noexcept {
    if constexpr ( std::is_same<StringyThing, std::basic_string_view<CharT>>::value ) {
        return x; // RVO.
    }
    if constexpr ( std::is_same<StringyThing, CharT>::value ) {
        return std::basic_string_view<CharT> ( std::addressof ( x ), 1 );
    }
    if constexpr ( std::disjunction<std::is_same<StringyThing, const CharT *>, std::is_same<StringyThing, std::basic_string<CharT>>>::value ) {
        return std::basic_string_view<CharT> ( x );
    }
}


template<typename CharT, typename StringyThing>
constexpr void remove_prefix ( std::basic_string_view<CharT> & s, bool & removed, StringyThing x_ ) noexcept {
    const std::basic_string_view<CharT> x = make_string_view<CharT> ( x_ );
    if ( s.size ( ) >= x.size ( ) and s.compare ( 0, x.size ( ), x ) == 0 ) {
        s.remove_prefix ( x.size ( ) );
        removed = removed or true;
    };
}
template<typename CharT, typename ... Args>
constexpr void remove_prefix ( std::basic_string_view<CharT> & s_, Args ... args_ ) noexcept {
    bool removed = false;
    do {
        removed = false;
        ( remove_prefix ( s_, removed, std::forward<Args> ( args_ ) ), ... );
    } while ( removed ); // Keep removing untill nothing more can be removed.
}


template<typename CharT, typename StringyThing>
constexpr void remove_suffix ( std::basic_string_view<CharT> & s, bool & removed, StringyThing x_ ) noexcept {
    const std::basic_string_view<CharT> x = make_string_view<CharT> ( x_ );
    if ( s.size ( ) >= x.size ( ) and s.compare ( s.size ( ) - x.size ( ), std::basic_string_view<CharT>::npos, x ) == 0 ) {
        s.remove_suffix ( x.size ( ) );
        removed = removed or true;
    };
}
template<typename CharT, typename ... Args>
constexpr void remove_suffix ( std::basic_string_view<CharT> & s_, Args ... args_ ) noexcept {
    bool removed = false;
    do {
        removed = false;
        ( remove_suffix ( s_, removed, std::forward<Args> ( args_ ) ), ... );
    } while ( removed ); // Keep removing untill nothing more can be removed.
}


template<typename CharT, typename SizeT, typename StringyThing>
constexpr void find ( std::basic_string_view<CharT> & s, SizeT & f_, StringyThing x_ ) noexcept {
    f_ = std::min ( s.find ( make_string_view<CharT> ( x_ ) ), f_ );
}
template<typename CharT, typename ... Args>
[[ nodiscard ]] constexpr auto find ( std::basic_string_view<CharT> & s_, Args ... args_ ) noexcept {
    auto found = std::basic_string_view<CharT>::npos;
    ( find ( s_, found, std::forward<Args> ( args_ ) ), ... );
    return found;
}


template<typename Stream, typename Container>
Stream & operator << ( Stream & out_, const Container & s_ ) noexcept {
    for ( const auto & v : s_ )
        out_ << '\"' << v << "\" ";
    out_ << '\b';
    return out_;
}


template<typename CharT, typename ... Delimiters>
[[ nodiscard ]] std::vector<std::basic_string_view<CharT>> string_split ( const std::basic_string<CharT> & string_, Delimiters ... delimiters_ ) {
    using size_type = typename std::basic_string_view<CharT>::size_type;
    std::basic_string_view<CharT> string_view ( string_ );
    std::vector<std::basic_string_view<CharT>> string_view_vector;
    string_view_vector.reserve ( 4 ); // Avoid small size re-allocating, 0 > 1 > 2 > 3 > 4 > 6, now 4 > 6 > 9 etc.
    // Remove trailing delimiters.
    remove_suffix ( string_view, std::forward<Delimiters> ( delimiters_ ) ... );
    // Parse the string_view left to right.
    while ( true ) {
        remove_prefix ( string_view, std::forward<Delimiters> ( delimiters_ ) ... );
        const size_type pos = find ( string_view, std::forward<Delimiters> ( delimiters_ ) ... );
        if ( std::basic_string_view<CharT>::npos == pos ) {
            string_view_vector.emplace_back ( std::move ( string_view ) );
            break;
        }
        string_view_vector.emplace_back ( string_view.data ( ), pos );
        string_view.remove_prefix ( pos );
    }
    return string_view_vector;
}


int main ( ) {

    std::string s ( " , \t the quick brown ,fox jumps over uit   , the lazy dog      ," );

    auto split = string_split ( s, " ", ",", "\t", "uit" );

    std::cout << split << nl;

    return EXIT_SUCCESS;
}




template <typename CharT, typename ... Delimiters, std::size_t ... I>
auto make_string_views ( const std::tuple<Delimiters ... > & delimiters_, std::index_sequence<I...> ) {
    return std::make_tuple ( make_string_view<CharT> ( std::get<I> ( delimiters_ ) ) ... );
}
template <typename CharT, typename ... Delimiters>
auto make_string_views ( Delimiters ... delimiters_ ) {
    return make_string_views<CharT> ( std::forward_as_tuple ( delimiters_ ... ), std::make_index_sequence<sizeof ... ( Delimiters )> ( ) );
}
