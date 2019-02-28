
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

#include <sax/iostream.hpp>
#include <iterator>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>


#include <sax/string_split.hpp>


namespace sax2 {

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

#define CHARACTER(name, character) \
template<typename T, typename = is_string<T>> \
struct name { \
    static constexpr auto c ( ) noexcept { \
        if constexpr ( std::is_same<T, std::string>::value ) \
            return std::string ( character ).back ( ); \
        if constexpr ( std::is_same<T, std::wstring>::value ) \
            return std::wstring ( L##character ).back ( ); \
    } \
    static constexpr auto s ( ) noexcept { \
        if constexpr ( std::is_same<T, std::string>::value ) \
            return std::string ( character ); \
        if constexpr ( std::is_same<T, std::wstring>::value ) \
            return std::wstring ( L##character ); \
    } \
};

CHARACTER ( space, " " )
CHARACTER ( dot, "." )
CHARACTER ( comma, "," )
CHARACTER ( langle, "<" )
CHARACTER ( rangle, ">" )
CHARACTER ( dquote, "\"" )

}


namespace sax2::detail {


template<typename T>
void trim_trailing_spaces ( std::string_view & string_view_ ) noexcept {
    while ( string_view_.back ( ) == space<T>::c ( ) )
        string_view_.remove_suffix ( 1 );
}

template<typename T>
void trim_trailing_spaces ( std::vector<std::string_view> & string_view_vector_ ) noexcept {
    for ( auto & string_view : string_view_vector_ )
        trim_trailing_spaces<T> ( string_view );
}



template<typename T>
void string_split ( std::vector<std::string_view> & string_view_vector_, T * delimiter_ ) {
    std::vector<std::string_view> string_view_vector;
    for ( const T & string : string_view_vector_ )
        // string_split ( string_view_vector, string, delimiter_ );
    string_view_vector_ = std::move ( string_view_vector );
}

}

namespace sax2 {

template<typename T, typename ... Delimiters, typename = is_string<T>>
[[ nodiscard ]] std::vector<std::string_view> string_split ( const T & string_, Delimiters ... delimiters_ ) {
    using value_type = const typename T::value_type;
    std::vector<std::string_view> string_view_vector { string_ };
    ( detail::string_split<value_type> ( string_view_vector, std::forward<Delimiters> ( delimiters_ ) ), ... );
    detail::trim_trailing_spaces<value_type> ( string_view_vector );
    return string_view_vector;
}

}
