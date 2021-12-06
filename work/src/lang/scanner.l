%{
    #include "tokenUtils.h"
    #include "syntax.hpp"
%}

%option yylineno
%option noyywrap

LiteralUnsignedPostfixRule      [uU]
LiteralLongPostfixRule          [lL]
LiteralLongLongPostfixRule      [lL][lL]
LiteralFloatPostfixRule         [fF]

LiteralIntegerNumberBase2Rule   ((0[bB][0-1]+))
LiteralIntegerNumberBase8Rule   ((0[0-7]+))
LiteralIntegerNumberBase10Rule  ((0|([1-9][0-9]*)))
LiteralIntegerNumberBase16Rule  ((0([xX])([0-9a-fA-F])+))

LiteralRealNumberRule           ([:blank:]|0|[1-9][0-9]*)"."[0-9]*

LiteralStringRule               \"([^\\\"]|\\.)*\"
LiteralCharacterRule            \'[:blank:]|\\.\'


TokenRule                       [a-zA-Z_]([a-zA-Z0-9_]*)

%%

{LiteralIntegerNumberBase16Rule} {
    yylval.syntaxSignedLongLiteral = TokenUtils_positiveLongLiteral_Base16( yytext );
    return SignedLongLiteral;
}

{LiteralIntegerNumberBase16Rule}{LiteralLongPostfixRule} {
    yylval.syntaxSignedLongLiteral = TokenUtils_positiveLongLiteral_Base16Long( yytext );
    return SignedLongLiteral;
}

{LiteralIntegerNumberBase16Rule}{LiteralLongLongPostfixRule} {
    yylval.syntaxSignedLongLiteral = TokenUtils_positiveLongLiteral_Base16LongLong( yytext );
    return SignedLongLiteral;
}

{LiteralIntegerNumberBase16Rule}{LiteralUnsignedPostfixRule} {
    yylval.syntaxUnsignedLongLiteral = TokenUtils_positiveLongLiteral_Base16Unsigned( yytext );
    return UnsignedLongLiteral;
}

{LiteralIntegerNumberBase16Rule}{LiteralLongPostfixRule}{LiteralUnsignedPostfixRule} {
    yylval.syntaxUnsignedLongLiteral = TokenUtils_positiveLongLiteral_Base16LongUnsigned( yytext );
    return UnsignedLongLiteral;
}

{LiteralIntegerNumberBase16Rule}{LiteralLongLongPostfixRule}{LiteralUnsignedPostfixRule} {
    yylval.syntaxUnsignedLongLiteral = TokenUtils_positiveLongLiteral_Base16LongLongUnsigned( yytext );
    return UnsignedLongLiteral;
}

{LiteralIntegerNumberBase16Rule}{LiteralUnsignedPostfixRule}{LiteralLongPostfixRule} {
    yylval.syntaxUnsignedLongLiteral = TokenUtils_positiveLongLiteral_Base16UnsignedLong( yytext );
    return UnsignedLongLiteral;
}

{LiteralIntegerNumberBase16Rule}{LiteralUnsignedPostfixRule}{LiteralLongLongPostfixRule} {
    yylval.syntaxUnsignedLongLiteral = TokenUtils_positiveLongLiteral_Base16UnsignedLongLong( yytext );
    return UnsignedLongLiteral;
}

"-"{LiteralIntegerNumberBase16Rule} {
    yylval.syntaxSignedLongLiteral = TokenUtils_negativeLongLiteral_Base16( yytext );
    return SignedLongLiteral;
}

"-"{LiteralIntegerNumberBase16Rule}{LiteralLongPostfixRule} {
    yylval.syntaxSignedLongLiteral = TokenUtils_negativeLongLiteral_Base16Long( yytext );
    return SignedLongLiteral;
}

"-"{LiteralIntegerNumberBase16Rule}{LiteralLongLongPostfixRule} {
    yylval.syntaxSignedLongLiteral = TokenUtils_negativeLongLiteral_Base16LongLong( yytext );
    return SignedLongLiteral;
}

{LiteralRealNumberRule}{LiteralFloatPostfixRule} {
    yylval.syntaxFloatLiteral = TokenUtils_positiveFloatLiteral( yytext );
    return FloatLiteral; 
}

"-"{LiteralRealNumberRule}{LiteralFloatPostfixRule} {
    yylval.syntaxFloatLiteral = TokenUtils_negativeFloatLiteral( yytext );
    return FloatLiteral; 
}

{LiteralRealNumberRule} {
    yylval.syntaxDoubleLiteral = TokenUtils_positiveDoubleLiteral( yytext );
    return DoubleLiteral;
}

"-"{LiteralRealNumberRule} {
    yylval.syntaxDoubleLiteral = TokenUtils_negativeDoubleLiteral( yytext );
    return DoubleLiteral;
}

{LiteralCharacterRule} {
    yylval.syntaxCharLiteral = TokenUtils_characterLiteral( yytext );
    return CharLiteral;
}

{LiteralStringRule} {
    yylval.syntaxStringLiteral = TokenUtils_stringLiteral( yytext );
    return StringLiteral;
}

{TokenRule} { 
    yylval.syntaxStringLiteral = TokenUtils_token( yytext );  
    return Token;
}


[ \t\f] ;
"\r\n"|"\n" ;

. { 
    return yytext[0]; 
}

%%