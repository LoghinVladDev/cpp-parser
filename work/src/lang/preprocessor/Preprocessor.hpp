//
// Created by loghin on 07.12.2021.
//

#ifndef CPP_PARSER_PREPROCESSOR_HPP
#define CPP_PARSER_PREPROCESSOR_HPP

#include <CDS/Pointer>

namespace cds {
    class Path;
    class String;

    template < typename T >
    class Collection;

    template < typename T >
    class Array;
}

class Define;

class Preprocessor : cds :: Object {
private:
    cds :: UniquePointer < cds :: Array < cds :: SharedPointer < cds :: Path > > > _includedDirectories;

    Preprocessor () noexcept;

    auto static instance () noexcept -> Preprocessor &;
    auto static preprocessLines ( cds :: Collection < cds :: String > const &, cds :: Collection < cds :: SharedPointer < Define > > const & ) noexcept -> cds :: String;
    auto static include ( cds :: String const &, cds :: Collection < cds :: SharedPointer < Define > > const & ) noexcept (false) -> cds :: String;

public:
    auto static preprocess ( cds :: Path const &, cds :: Collection < cds :: SharedPointer < Define > > const & ) noexcept -> cds :: String;
    auto static addIncludeDirectory ( cds :: Path const & ) noexcept -> void;
};


#endif //CPP_PARSER_PREPROCESSOR_HPP
