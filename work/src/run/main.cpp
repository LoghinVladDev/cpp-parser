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

void parseStateTree ( cds :: PointerBase < State > const & state, int tabCount ) noexcept { // NOLINT(misc-no-recursion)
    std :: cout << "  "_s * tabCount << state->toString() << '\n';
    for ( auto & child : state->children() ) {
        parseStateTree(child, tabCount + 1);
    }
}

int main ( int argc, char ** argv ) {

    if ( argc == 1 ) {
        std :: cerr << "No input file specified\n";
        return 0;
    }

    FILE * sourceFile = fopen ( argv[1], "r" );
    if ( sourceFile == nullptr ) {
        std :: cerr << "Invalid Input File\n";
        return 0;
    }

    yyin = sourceFile;
    (void) yyparse();

    parseStateTree ( State :: rootState(), 0 );

    return 0;
}