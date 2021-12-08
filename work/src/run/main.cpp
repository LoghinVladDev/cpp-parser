//
// Created by loghin on 05.12.2021.
//

#include <iostream>
#include <State.hpp>

extern "C" {

    extern FILE * yyin;
    extern int yylineno;

    #include <cstdio>
    int yyerror ( const char * pString ) {
        (void) printf("Line : %d, Message : %s\n", yylineno, pString);
        return 0;
    }
}


extern int yyparse ();

#include <CDS/DoubleLinkedList>

#include <Preprocessor.hpp>
#include <CDS/Path>
#include <Define.hpp>
#include <CDS/HashMap>

void parseStateTree ( cds :: PointerBase < State > const & state, int tabCount ) noexcept { // NOLINT(misc-no-recursion)
    std :: cout << "  "_s * tabCount << state->toString() << '\n';
    for ( auto & child : state->children() ) {
        parseStateTree(child, tabCount + 1);
    }
}

using namespace cds; // NOLINT(clion-misra-cpp2008-7-3-4)

Array < String > includeDirectoryValidArgument = { "-I" };

int main ( int argc, char ** argv ) {

    if ( argc == 1 ) {
        std :: cerr << "Error : No input file specified\n";
        return 0;
    }

    DoubleLinkedList < Path > sourcePaths;

    for ( int i = 1; i < argc; ++ i ) { // NOLINT(clion-misra-cpp2008-6-5-3)
        if ( includeDirectoryValidArgument.contains(String(argv[i])) ) {
            ++i;

            try {
                Preprocessor :: addIncludeDirectory( argv[i] );
            } catch ( Exception const & invalidPath ) {
                std :: cerr << "Error : " << argv[i] << " is not a directory\n";
            }

        } else {

            try {
                sourcePaths.add(argv[i]);
            } catch (Exception const &invalidPath) {
                std::cerr << "Error : " << argv[i] << " is not a source file\n";
                return 0;
            }

        }
    }

    auto sourcesDefines = HashMap < Path, Array < SharedPointer < Define > > >();

    auto preprocessedSources = sourcePaths.sequence().map([&](Path const & path){
        (void) sourcesDefines.emplace ( path, { } );
        return Pair { path, Preprocessor :: preprocess ( path, sourcesDefines[path] ) };
    }).toLinkedList();

    preprocessedSources.forEach([& sourcesDefines](auto & source){
        std :: cout << "-"_s * 20 + " Source '" << source.first().nodeName() << "'" << "-"_s * 20 << "\n" << source.second() << "\n\n";
        std :: cout << "-"_s * 20 + " Defines for '" << source.first().nodeName() << "'" << "-"_s * 20 << "\n";

        for ( auto & define : sourcesDefines[source.first()] ) {
            std :: cout << "-\t" << define.toString() << "\n";
        }

        std :: cout << "-"_s * 20 + " End of Defines " << "-"_s * 20 << "\n";
    });

//    FILE * sourceFile = fopen ( argv[1], "r" );
//    if ( sourceFile == nullptr ) {
//        std :: cerr << "Invalid Input File\n";
//        return 0;
//    }

//    yyin = sourceFile;
//    (void) yyparse();



    parseStateTree ( State :: rootState(), 0 );

    return 0;
}