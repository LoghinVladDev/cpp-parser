//
// Created by loghin on 05.12.2021.
//

#include <iostream>

extern "C" {

    extern FILE * yyin;
    extern int yylineno;

    #include <cstdio>
    int yyerror ( const char * pString ) {
        printf("Line : %d, Message : %s\n", yylineno, pString);
        return 0;
    }
}

extern int yyparse ();

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
    yyparse();

    return 0;
}