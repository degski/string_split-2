
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

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstdlib>

#include <iostream>
#include <string>

#include <string_split.hpp>


template<typename Stream, typename Container>
Stream & operator << ( Stream & out_, const Container & s_ ) noexcept {
    for ( const auto & v : s_ )
        out_ << '\"' << v << "\" ";
    out_ << '\b';
    return out_;
}


int main ( ) {

    std::string s1 ( " , \t the quick brown ,, ,fox jumps underover \t  , the lazy dog" );
    std::cout << sax::string_split ( s1, " ", ',' , "\t", "under" ) << nl;

    std::string s2 ( "aaba" );
    std::cout << sax::string_split ( s2, "a", "ab" ) << nl;

    return EXIT_SUCCESS;
}

/* Output

    "the" "quick" "brown" "fox" "jumps" "over" "the" "lazy" "dog"
    "b"

*/
