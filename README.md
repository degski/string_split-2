
# string_split-2 (C++17)


A string splitter allowing for strings to be used as delimiters.

    template<typename CharT, typename ... Delimiters>
    [[ nodiscard ]] std::vector<std::basic_string_view<CharT>> string_split ( const std::basic_string<CharT> & string_, Delimiters ... delimiters_ );


Just pass in a STL-string, followed by a number of delimiters (string-literals or characters).


    std::string s { "Cheech and Chong" };
    auto vector = sax::string_split ( s, "and" );

returns a vector of string_views "Cheech" and "Chong", this implies the passed-in string should be ket alive and unmodified.

To deal with tabs, pass in a "\t" or '\t' as a delimiter.

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
