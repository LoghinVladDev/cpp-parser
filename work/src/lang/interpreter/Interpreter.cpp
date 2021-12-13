//
// Created by loghin on 09.12.2021.
//

#include "Interpreter.hpp"

#include <CDS/Path>
#include <CDS/Array>

using namespace cds; // NOLINT(clion-misra-cpp2008-7-3-4)

Interpreter :: Interpreter () noexcept :
        sourcePaths ( new Array < Path > ),
        includeDirectories ( new Array < Path > ) {

}

auto Interpreter :: currentActiveInterpreter() noexcept -> Interpreter * {
    return Interpreter :: activeInterpreter.get();
}

auto Interpreter :: addSource( Path const & path ) noexcept -> void {
    this->sourcePaths->add ( path );
}

auto Interpreter :: addIncludeDirectory( Path const & path ) noexcept -> void {
    this->includeDirectories->add ( path );
}

#include <Preprocessor.hpp>

auto Interpreter :: start() noexcept -> void {
    Interpreter :: activeInterpreter = this;

    for ( auto const & includeDirectory : * this->includeDirectories ) {
        Preprocessor :: instance().addIncludeDirectory( includeDirectory );
    }

    for ( auto const & sourcePath : * this->sourcePaths ) {
        auto preprocessedCode = Preprocessor :: preprocess ( sourcePath );

        std :: cout << "-"_s * 20 + " Source '" << sourcePath << "'" << "-"_s * 20 << "\n" << preprocessedCode << "\n\n";
    }
}