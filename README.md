
# string_split-2 (C++17)


A string splitter allowing for strings to be used as delimiters.

    template<typename CharT, typename ... Delimiters>
    [[ nodiscard ]] std::vector<std::basic_string_view<CharT>> string_split ( std::basic_string<CharT> const & string_, Delimiters const ... delimiters_ );


What the function does: Remove from the string any delimiters passed in, doing that left to right, applying the delimiters left to right and return the now separate bits left as a `std::vector` of string_view's over the original string. The latter means the string has to outlive the vector of string views.

The above does means that depending on what kind of delimiters you put (as they can be strings, which can interact with each other), that the order of the delimiters has significance.
Just pass in a STL-string, followed by a number of delimiters (string-literals or characters).


    std::string s { "Cheech and Chong" };
    auto vector = sax::string_split ( s, "and" );

returns a vector of string_views "Cheech" and "Chong", this implies the passed-in string should be kept alive and stay unmodified.

To deal with tabs, f.e., pass in a "\t" or '\t' as a delimiter.

Lines in a csv-file are easily parsed with the combo of 
`",", " ", "\t"` as delimiters, which will parse most 
csv-files out of the box. 

    std::string s { "Cheech and, Chong" };
    auto vector = sax::string_split ( s, "and" );

returns a vector of string_views "Cheech" and ", Chong".

    std::string s { "Cheech and, Chong" };
    auto vector = sax::string_split ( s, "and", "," );

returns a vector of string_views "Cheech" and "Chong".
    
    std::string s { "Cheech and, Chong" };
    auto vector = sax::string_split ( s, " and ", "," );
    
returns a vector of string_views "Cheech and" and "Chong".


### Dependencies

* [Sax](https://github.com/degski/sax/)
