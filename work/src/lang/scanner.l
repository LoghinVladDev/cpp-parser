%start CommentStartRuleHeader CommentStartRuleBody
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


CommentStartRule                "/""*"
InlineCommentRule               "/""/"[^\n]*


%%


<INITIAL>{CommentStartRule}     BEGIN(CommentStartRuleHeader);

<CommentStartRuleHeader>"*"     BEGIN(CommentStartRuleBody);
<CommentStartRuleHeader>[^*]    ;

<CommentStartRuleBody>"*"       ;
<CommentStartRuleBody>"/"       BEGIN(INITIAL);
<CommentStartRuleBody>[^*/]     BEGIN(CommentStartRuleHeader);

{InlineCommentRule}             ;

"alignas"                       { return KeywordAlignAs; }
"alignof"                       { return KeywordAlignOf; }
"and"                           { return KeywordAnd; }
"and_eq"                        { return KeywordAndEquals; }
"asm"                           { return KeywordAsm; }
"auto"                          { return KeywordAuto; }

"bitand"                        { return KeywordBitAnd; }
"bitor"                         { return KeywordBitOr; }
"bool"                          { return KeywordBool; }
"break"                         { return KeywordBreak; }

"case"                          { return KeywordCase; }
"catch"                         { return KeywordCatch; }
"char"                          { return KeywordChar; }
"char8_t"                       { return KeywordChar8; }
"char16_t"                      { return KeywordChar16; }
"char32_t"                      { return KeywordChar32; }
"class"                         { return KeywordClass; }
"compl"                         { return KeywordCompl; }
"concept"                       { return KeywordConcept; }
"const"                         { return KeywordConst; }
"consteval"                     { return KeywordConsteval; }
"constexpr"                     { return KeywordConstexpr; }
"constinit"                     { return KeywordConstinit; }
"const_cast"                    { return KeywordConstCast; }
"continue"                      { return KeywordContinue; }
"co_await"                      { return KeywordCoAwait; }
"co_return"                     { return KeywordCoReturn; }
"co_yield"                      { return KeywordCoYield; }

"decltype"                      { return KeywordDecltype; }
"default"                       { return KeywordDefault; }
"delete"                        { return KeywordDelete; }
"do"                            { return KeywordDo; }
"double"                        { return KeywordDouble; }
"dynamic_cast"                  { return KeywordDynamicCast; }

"else"                          { return KeywordElse; }
"enum"                          { return KeywordEnum; }
"explicit"                      { return KeywordExplicit; }
"export"                        { return KeywordExport; }
"extern"                        { return KeywordExtern; }

"false"                         { return KeywordFalse; }
"float"                         { return KeywordFloat; }
"for"                           { return KeywordFor; }
"friend"                        { return KeywordFriend; }
"final"                         { return KeywordFinal; }

"goto"                          { return KeywordGoto; }

"if"                            { return KeywordIf; }
"inline"                        { return KeywordInline; }
"int"                           { return KeywordInt; }
"import"                        { return KeywordImport; }

"long"                          { return KeywordLong; }

"mutable"                       { return KeywordMutable; }

"module"                        { return KeywordModule; }
"namespace"                     { return KeywordNamespace; }
"new"                           { return KeywordNew; }
"noexcept"                      { return KeywordNoexcept; }
"not"                           { return KeywordNot; }
"not_eq"                        { return KeywordNotEquals; }
"nullptr"                       { return KeywordNullptr; }

"operator"                      { return KeywordOperator; }
"or"                            { return KeywordOr; }
"or_eq"                         { return KeywordOrEquals; }
"override"                      { return KeywordOverride; }

"private"                       { return KeywordPrivate; }
"protected"                     { return KeywordProtected; }
"public"                        { return KeywordPublic; }

"register"                      { return KeywordRegister; }
"reinterpret_cast"              { return KeywordReinterpretCast; }
"requires"                      { return KeywordRequires; }
"return"                        { return KeywordReturn; }

"short"                         { return KeywordShort; }
"signed"                        { return KeywordSigned; }
"sizeof"                        { return KeywordSizeof; }
"static"                        { return KeywordStatic; }
"static_assert"                 { return KeywordStaticAssert; }
"static_cast"                   { return KeywordStaticCast; }
"struct"                        { return KeywordStruct; }
"switch"                        { return KeywordSwitch; }

"template"                      { return KeywordTemplate; }
"this"                          { return KeywordThis; }
"thread_local"                  { return KeywordThreadLocal; }
"throw"                         { return KeywordThrow; }
"true"                          { return KeywordTrue; }
"try"                           { return KeywordTry; }
"typedef"                       { return KeywordTypedef; }
"typeid"                        { return KeywordTypeid; }
"typename"                      { return KeywordTypename; }

"union"                         { return KeywordUnion; }
"unsigned"                      { return KeywordUnsigned; }
"using"                         { return KeywordUsing; }

"virtual"                       { return KeywordVirtual; }
"void"                          { return KeywordVoid; }
"volatile"                      { return KeywordVolatile; }

"wchar_t"                       { return KeywordWChar; }
"while"                         { return KeywordWhile; }

"xor"                           { return KeywordXor; }
"xor_eq"                        { return KeywordXorEquals; }



"::"                            { return OperatorScopeResolution; }

"++"                            { return OperatorIncrement; }
"--"                            { return OperatorDecrement; }

"."                             { return OperatorMemberAccess; }
"->"                            { return OperatorPointerMemberAccess; }

"!"                             { return OperatorNot; }
"~"                             { return OperatorBitwiseNot; }

".*"                            { return OperatorPointerToMember; }
"->*"                           { return OperatorPointerToPointerMember; }

"*"                             { return OperatorMultiply; }
"/"                             { return OperatorDivide; }
"%"                             { return OperatorModulo; }

"+"                             { return OperatorSum; }
"-"                             { return OperatorSubtract; }

"<<"                            { return OperatorBitwiseLeftShift; }
">>"                            { return OperatorBitwiseRightShift; }

"<=>"                           { return OperatorThreeWayCompare; }

"<"                             { return OperatorLess; }
"<="                            { return OperatorLessEquals; }
">"                             { return OperatorGreater; }
">="                            { return OperatorGreaterEquals; }

"=="                            { return OperatorEquals; }
"!="                            { return OperatorNotEquals; }

"&"                             { return OperatorBitwiseAnd; }
"^"                             { return OperatorBitwiseXor; }
"|"                             { return OperatorBitwiseOr; }

"&&"                            { return OperatorAnd; }
"||"                            { return OperatorOr; }

"?"                             { return OperatorTernaryConditionalIf; }
":"                             { return OperatorTernaryConditionalElse; }

"="                             { return OperatorAssign; }

"+="                            { return OperatorAssignSum; }
"-="                            { return OperatorAssignSubtract; }

"*="                            { return OperatorAssignMultiply; }
"/="                            { return OperatorAssignDivide; }
"%="                            { return OperatorAssignModulo; }

"<<="                           { return OperatorBitwiseLeftShift; }
">>="                           { return OperatorBitwiseRightShift; }

"&="                            { return OperatorBitwiseAnd; }
"^="                            { return OperatorBitwiseXor; }
"|="                            { return OperatorBitwiseOr; }

","                             { return OperatorComma; }


{LiteralIntegerNumberBase2Rule} {
    yylval.syntaxSignedLongLiteral = TokenUtils_positiveLongLiteral_Base2( yytext );
    return SignedLongLiteral;
}

{LiteralIntegerNumberBase2Rule}{LiteralLongPostfixRule} {
    yylval.syntaxSignedLongLiteral = TokenUtils_positiveLongLiteral_Base2Long( yytext );
    return SignedLongLiteral;
}

{LiteralIntegerNumberBase2Rule}{LiteralLongLongPostfixRule} {
    yylval.syntaxSignedLongLiteral = TokenUtils_positiveLongLiteral_Base2LongLong( yytext );
    return SignedLongLiteral;
}

{LiteralIntegerNumberBase2Rule}{LiteralUnsignedPostfixRule} {
    yylval.syntaxUnsignedLongLiteral = TokenUtils_positiveLongLiteral_Base2Unsigned( yytext );
    return UnsignedLongLiteral;
}

{LiteralIntegerNumberBase2Rule}{LiteralLongPostfixRule}{LiteralUnsignedPostfixRule} {
    yylval.syntaxUnsignedLongLiteral = TokenUtils_positiveLongLiteral_Base2LongUnsigned( yytext );
    return UnsignedLongLiteral;
}

{LiteralIntegerNumberBase2Rule}{LiteralLongLongPostfixRule}{LiteralUnsignedPostfixRule} {
    yylval.syntaxUnsignedLongLiteral = TokenUtils_positiveLongLiteral_Base2LongLongUnsigned( yytext );
    return UnsignedLongLiteral;
}

{LiteralIntegerNumberBase2Rule}{LiteralUnsignedPostfixRule}{LiteralLongPostfixRule} {
    yylval.syntaxUnsignedLongLiteral = TokenUtils_positiveLongLiteral_Base2UnsignedLong( yytext );
    return UnsignedLongLiteral;
}

{LiteralIntegerNumberBase2Rule}{LiteralUnsignedPostfixRule}{LiteralLongLongPostfixRule} {
    yylval.syntaxUnsignedLongLiteral = TokenUtils_positiveLongLiteral_Base2UnsignedLongLong( yytext );
    return UnsignedLongLiteral;
}

"-"{LiteralIntegerNumberBase2Rule} {
    yylval.syntaxSignedLongLiteral = TokenUtils_negativeLongLiteral_Base2( yytext );
    return SignedLongLiteral;
}

"-"{LiteralIntegerNumberBase2Rule}{LiteralLongPostfixRule} {
    yylval.syntaxSignedLongLiteral = TokenUtils_negativeLongLiteral_Base2Long( yytext );
    return SignedLongLiteral;
}

"-"{LiteralIntegerNumberBase2Rule}{LiteralLongLongPostfixRule} {
    yylval.syntaxSignedLongLiteral = TokenUtils_negativeLongLiteral_Base2LongLong( yytext );
    return SignedLongLiteral;
}


{LiteralIntegerNumberBase8Rule} {
    yylval.syntaxSignedLongLiteral = TokenUtils_positiveLongLiteral_Base8( yytext );
    return SignedLongLiteral;
}

{LiteralIntegerNumberBase8Rule}{LiteralLongPostfixRule} {
    yylval.syntaxSignedLongLiteral = TokenUtils_positiveLongLiteral_Base8Long( yytext );
    return SignedLongLiteral;
}

{LiteralIntegerNumberBase8Rule}{LiteralLongLongPostfixRule} {
    yylval.syntaxSignedLongLiteral = TokenUtils_positiveLongLiteral_Base8LongLong( yytext );
    return SignedLongLiteral;
}

{LiteralIntegerNumberBase8Rule}{LiteralUnsignedPostfixRule} {
    yylval.syntaxUnsignedLongLiteral = TokenUtils_positiveLongLiteral_Base8Unsigned( yytext );
    return UnsignedLongLiteral;
}

{LiteralIntegerNumberBase8Rule}{LiteralLongPostfixRule}{LiteralUnsignedPostfixRule} {
    yylval.syntaxUnsignedLongLiteral = TokenUtils_positiveLongLiteral_Base8LongUnsigned( yytext );
    return UnsignedLongLiteral;
}

{LiteralIntegerNumberBase8Rule}{LiteralLongLongPostfixRule}{LiteralUnsignedPostfixRule} {
    yylval.syntaxUnsignedLongLiteral = TokenUtils_positiveLongLiteral_Base8LongLongUnsigned( yytext );
    return UnsignedLongLiteral;
}

{LiteralIntegerNumberBase8Rule}{LiteralUnsignedPostfixRule}{LiteralLongPostfixRule} {
    yylval.syntaxUnsignedLongLiteral = TokenUtils_positiveLongLiteral_Base8UnsignedLong( yytext );
    return UnsignedLongLiteral;
}

{LiteralIntegerNumberBase8Rule}{LiteralUnsignedPostfixRule}{LiteralLongLongPostfixRule} {
    yylval.syntaxUnsignedLongLiteral = TokenUtils_positiveLongLiteral_Base8UnsignedLongLong( yytext );
    return UnsignedLongLiteral;
}

"-"{LiteralIntegerNumberBase8Rule} {
    yylval.syntaxSignedLongLiteral = TokenUtils_negativeLongLiteral_Base8( yytext );
    return SignedLongLiteral;
}

"-"{LiteralIntegerNumberBase8Rule}{LiteralLongPostfixRule} {
    yylval.syntaxSignedLongLiteral = TokenUtils_negativeLongLiteral_Base8Long( yytext );
    return SignedLongLiteral;
}

"-"{LiteralIntegerNumberBase8Rule}{LiteralLongLongPostfixRule} {
    yylval.syntaxSignedLongLiteral = TokenUtils_negativeLongLiteral_Base8LongLong( yytext );
    return SignedLongLiteral;
}


{LiteralIntegerNumberBase10Rule} {
    yylval.syntaxSignedLongLiteral = TokenUtils_positiveLongLiteral_Base10( yytext );
    return SignedLongLiteral;
}

{LiteralIntegerNumberBase10Rule}{LiteralLongPostfixRule} {
    yylval.syntaxSignedLongLiteral = TokenUtils_positiveLongLiteral_Base10Long( yytext );
    return SignedLongLiteral;
}

{LiteralIntegerNumberBase10Rule}{LiteralLongLongPostfixRule} {
    yylval.syntaxSignedLongLiteral = TokenUtils_positiveLongLiteral_Base10LongLong( yytext );
    return SignedLongLiteral;
}

{LiteralIntegerNumberBase10Rule}{LiteralUnsignedPostfixRule} {
    yylval.syntaxUnsignedLongLiteral = TokenUtils_positiveLongLiteral_Base10Unsigned( yytext );
    return UnsignedLongLiteral;
}

{LiteralIntegerNumberBase10Rule}{LiteralLongPostfixRule}{LiteralUnsignedPostfixRule} {
    yylval.syntaxUnsignedLongLiteral = TokenUtils_positiveLongLiteral_Base10LongUnsigned( yytext );
    return UnsignedLongLiteral;
}

{LiteralIntegerNumberBase10Rule}{LiteralLongLongPostfixRule}{LiteralUnsignedPostfixRule} {
    yylval.syntaxUnsignedLongLiteral = TokenUtils_positiveLongLiteral_Base10LongLongUnsigned( yytext );
    return UnsignedLongLiteral;
}

{LiteralIntegerNumberBase10Rule}{LiteralUnsignedPostfixRule}{LiteralLongPostfixRule} {
    yylval.syntaxUnsignedLongLiteral = TokenUtils_positiveLongLiteral_Base10UnsignedLong( yytext );
    return UnsignedLongLiteral;
}

{LiteralIntegerNumberBase10Rule}{LiteralUnsignedPostfixRule}{LiteralLongLongPostfixRule} {
    yylval.syntaxUnsignedLongLiteral = TokenUtils_positiveLongLiteral_Base10UnsignedLongLong( yytext );
    return UnsignedLongLiteral;
}

"-"{LiteralIntegerNumberBase10Rule} {
    yylval.syntaxSignedLongLiteral = TokenUtils_negativeLongLiteral_Base10( yytext );
    return SignedLongLiteral;
}

"-"{LiteralIntegerNumberBase10Rule}{LiteralLongPostfixRule} {
    yylval.syntaxSignedLongLiteral = TokenUtils_negativeLongLiteral_Base10Long( yytext );
    return SignedLongLiteral;
}

"-"{LiteralIntegerNumberBase10Rule}{LiteralLongLongPostfixRule} {
    yylval.syntaxSignedLongLiteral = TokenUtils_negativeLongLiteral_Base10LongLong( yytext );
    return SignedLongLiteral;
}


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