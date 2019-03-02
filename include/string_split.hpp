
// MIT License
//
// Copyright (c) 2019 degski
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

#pragma once

#include <algorithm>
#include <functional>
#include <sax/iostream.hpp>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>


namespace sax {

template<typename T>
using is_string = std::is_base_of<std::basic_string<typename T::value_type>, T>;

template<typename T, typename = is_string<T>>
void print ( const T & string_ ) noexcept {
    if constexpr ( std::is_same<T, std::string>::value ) { std::cout << '[' << string_ << ']'; }
    else if constexpr ( std::is_same<T, std::wstring>::value ) { std::wcout << L'[' << string_ << L']'; }
}

template<typename T, typename = is_string<T>>
void print ( const std::vector<T> & string_vector_ ) noexcept {
    for ( const auto & string : string_vector_ ) {
        if constexpr ( std::is_same<T, std::string>::value ) { std::cout << '<' << string << '>' << sp; }
        else if constexpr ( std::is_same<T, std::wstring>::value ) { std::wcout << L'<' << string << L'>' << sp; }
    }
}

}


namespace sax::detail {

/*

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

*/

template<typename CharT>
[[ nodiscard ]] constexpr std::basic_string_view<CharT> make_string_view ( const std::basic_string_view<CharT> & x ) noexcept {
    return x; // guaranteed copy elision.
}
template<typename CharT>
[[ nodiscard ]] constexpr std::basic_string_view<CharT> make_string_view ( const CharT & x ) noexcept {
    return std::basic_string_view<CharT> ( std::addressof ( x ), 1 );
}
template<typename CharT>
[[ nodiscard ]] constexpr std::basic_string_view<CharT> make_string_view ( const CharT * const & x ) noexcept {
    return std::basic_string_view<CharT> ( x );
}

template <typename CharT, typename ... Delimiters, std::size_t ... I>
[ [ nodiscard ] ] constexpr auto make_string_views ( const std::tuple<const Delimiters & ... > & delimiters_, std::index_sequence<I...> ) noexcept {
    return std::make_tuple ( make_string_view<CharT> ( std::get<I> ( delimiters_ ) ) ... );
}
template <typename CharT, typename ... Delimiters>
[ [ nodiscard ] ] constexpr auto make_string_views ( const Delimiters & ... delimiters_ ) noexcept {
    return make_string_views<CharT> ( std::forward_as_tuple ( delimiters_ ... ), std::make_index_sequence<sizeof ... ( Delimiters )> ( ) );
}


template<typename CharT>
constexpr void remove_prefix ( std::basic_string_view<CharT> & s, bool & removed, const std::basic_string_view<CharT> x ) noexcept {
    if ( s.size ( ) >= x.size ( ) and s.compare ( 0, x.size ( ), x ) == 0 ) {
        s.remove_prefix ( x.size ( ) );
        removed = true;
    };
}
template<typename CharT, typename ... Args>
constexpr void remove_prefix ( std::basic_string_view<CharT> & s_, Args && ... args_ ) noexcept {
    bool removed = false;
    do {
        removed = false;
        ( remove_prefix ( s_, removed, std::forward<Args> ( args_ ) ), ... );
    } while ( removed ); // Keep removing untill nothing more can be removed.
}


template<typename CharT>
constexpr void remove_suffix ( std::basic_string_view<CharT> & s, bool & removed, const std::basic_string_view<CharT> x ) noexcept {
    if ( s.size ( ) >= x.size ( ) and s.compare ( s.size ( ) - x.size ( ), std::basic_string_view<CharT>::npos, x ) == 0 ) {
        s.remove_suffix ( x.size ( ) );
        removed = true;
    };
}
template<typename CharT, typename ... Args>
constexpr void remove_suffix ( std::basic_string_view<CharT> & s_, Args && ... args_ ) noexcept {
    bool removed = false;
    do {
        removed = false;
        ( remove_suffix ( s_, removed, std::forward<Args> ( args_ ) ), ... );
    } while ( removed ); // Keep removing untill nothing more can be removed.
}


template<typename CharT, typename SizeT>
constexpr void find ( std::basic_string_view<CharT> & s, SizeT & f_, std::basic_string_view<CharT> x_ ) noexcept {
    f_ = std::min ( s.find ( x_ ), f_ );
}
template<typename CharT, typename ... Args>
[[ nodiscard ]] constexpr auto next ( std::basic_string_view<CharT> & s_, Args && ... args_ ) noexcept {
    remove_prefix ( s_, std::forward<Args> ( args_ ) ... );
    auto found = std::basic_string_view<CharT>::npos;
    ( find ( s_, found, std::forward<Args> ( args_ ) ), ... );
    return found;
}

template<typename CharT, typename SizeT>
constexpr void match ( std::basic_string_view<CharT> & s_, SizeT & match_length_, const std::basic_string_view<CharT> x_ ) noexcept {
    if ( s_.size ( ) >= x_.size ( ) and s_.compare ( 0, x_.size ( ), x_ ) == 0 )
        match_length_ = x_.size ( );
}
template<typename CharT, typename ... Args>
[[ nodiscard ]] constexpr auto any_matches ( std::basic_string_view<CharT> & s_, Args && ... args_ ) noexcept {
    using size_type = typename std::basic_string_view<CharT>::size_type;
    size_type match_length = 0;
    ( match ( s_, match_length, std::forward<Args> ( args_ ) ), ... );
    return match_length;
}

}

namespace sax {

template<typename CharT, typename ... Delimiters>
[[ nodiscard ]] std::vector<std::basic_string_view<CharT>> string_split ( const std::basic_string<CharT> & string_, Delimiters ... delimiters_ ) {
    using size_type = typename std::basic_string_view<CharT>::size_type;
    if ( string_.empty ( ) )
        return { };
    std::basic_string_view<CharT> string_view ( string_ );
    std::vector<std::basic_string_view<CharT>> string_view_vector;
    string_view_vector.reserve ( 4 ); // Avoid small size re-allocating, 0 > 1 > 2 > 3 > 4 > 6, now 4 > 6 > 9 etc.
    const auto any_matches = [ & string_view ] ( auto && ... args ) noexcept {
        return detail::any_matches ( string_view, std::forward<decltype ( args )> ( args ) ... );
    };
    const std::tuple params = detail::make_string_views<CharT> ( std::forward<const Delimiters&> ( delimiters_ ) ... );
    do {
        size_type match_length;
        do {
            match_length = std::apply ( any_matches, params );
            string_view.remove_prefix ( match_length );
        } while ( match_length );
        const auto match_start = string_view.data ( );
        do {
            string_view.remove_prefix ( 1 );
        } while ( not ( std::apply ( any_matches, params ) ) );
        string_view_vector.emplace_back ( match_start, string_view.data ( ) - match_start );
    } while ( string_view.size ( ) );
    return string_view_vector;
}

}
