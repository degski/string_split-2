
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
#include <utility>
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

} // namespace sax


namespace sax::detail {

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


template <typename CharT, std::size_t Size>
struct StringViewArray {
    using size_type = typename std::basic_string_view<CharT>::size_type;
    template<typename ... Delimiters>
    constexpr StringViewArray ( Delimiters const & ... delimiters_ ) noexcept :
        data { make_string_view<CharT> ( delimiters_ ) ... } { }
    constexpr std::basic_string_view<CharT> const & operator [ ] ( std::size_t i_ ) const noexcept {
        return data [ i_ ];
    }
    constexpr static size_type size ( ) {
        return static_cast<size_type> ( Size );
    }
    private:
    std::basic_string_view<CharT> data [ Size ];
};


template<typename CharT, typename SizeT>
[[ nodiscard ]] constexpr SizeT match ( std::basic_string_view<CharT> & s_, std::basic_string_view<CharT> const & x_ ) noexcept {
    if ( s_.size ( ) >= x_.size ( ) and s_.compare ( 0, x_.size ( ), x_ ) == 0 )
        return x_.size ( );
    return 0;
}

template<typename CharT, typename Array>
[[ nodiscard ]] constexpr auto any_matches ( std::basic_string_view<CharT> & s_, const Array & array_ ) noexcept {
    using size_type = typename std::basic_string_view<CharT>::size_type;
    size_type match_length = 0;
    for ( size_type i = 0; i < Array::size ( ); ++i )
        if ( ( match_length = match<CharT, size_type> ( s_, array_ [ i ] ) ) )
            break;
    return match_length;
}

} // namespace sax::detail


namespace sax {

template<typename CharT, typename ... Delimiters>
[[ nodiscard ]] std::vector<std::basic_string_view<CharT>> string_split ( const std::basic_string<CharT> & string_, const Delimiters ... delimiters_ ) {
    using size_type = typename std::basic_string_view<CharT>::size_type;
    if ( string_.empty ( ) )
        return { };
    std::basic_string_view<CharT> string_view ( string_ );
    std::vector<std::basic_string_view<CharT>> string_view_vector;
    string_view_vector.reserve ( 4 ); // Avoid small size re-allocating, 0 > 1 > 2 > 3 > 4 > 6, now 4 > 6 > 9 etc.
    detail::StringViewArray<CharT, sizeof ... ( Delimiters )> params ( delimiters_ ... );
    do {
        size_type match_length;
        do {
            match_length = detail::any_matches ( string_view, params );
            string_view.remove_prefix ( match_length );
        } while ( match_length );
        const auto match_start = string_view.data ( );
        do {
            string_view.remove_prefix ( 1 );
        } while ( not ( detail::any_matches ( string_view, params ) ) );
        string_view_vector.emplace_back ( match_start, string_view.data ( ) - match_start );
        std::cout << string_view_vector.back ( ) << nl;
    } while ( string_view.size ( ) );
    return string_view_vector;
}

} // namespace sax
