//
// Created by loghin on 09.12.2021.
//

#ifndef CPP_PARSER_INTERPRETER_HPP
#define CPP_PARSER_INTERPRETER_HPP

#include <CDS/Pointer>

namespace cds {
    template < typename T >
    class Array;

    class Path;
}

class Interpreter : cds :: Object {
private:
    cds :: UniquePointer < cds :: Array < cds :: Path > > sourcePaths;
    cds :: UniquePointer < cds :: Array < cds :: Path > > includeDirectories;

    static inline cds :: ForeignPointer < Interpreter > activeInterpreter = nullptr;

public:
    static auto currentActiveInterpreter () noexcept -> Interpreter *;

    Interpreter () noexcept;

    auto addSource ( cds :: Path const & ) noexcept -> void;
    auto addIncludeDirectory ( cds :: Path const & ) noexcept -> void;

    auto start () noexcept -> void;
};


#endif //CPP_PARSER_INTERPRETER_HPP
