//
// Created by loghin on 07.12.2021.
//

#include "Preprocessor.hpp"

#include <fstream>
#include <sstream>

#include <CDS/Path>

#include <Define.hpp>

#include <Error.hpp>
#include <Warning.hpp>

#include <CDS/HashSet>
#include <CDS/Stack>

using namespace cds; // NOLINT(clion-misra-cpp2008-7-3-4)

auto static uncomment ( String const & source ) noexcept -> String {
    enum UncommentParseState {
        RegularCode,
        InlineComment,
        MultiLineComment
    };

    UncommentParseState state = RegularCode;
    String uncommentedCode;

    for ( Index i = 0; static_cast < Size > ( i ) < source.length() - 1u; ++ i ) { // NOLINT(clion-misra-cpp2008-6-5-3)
        switch ( state ) {
            case RegularCode: {
                if (source[i] == '/' && source[i + 1] == '/') {
                    state = InlineComment;
                } else if (source[i] == '/' && source[i + 1] == '*') {
                    state = MultiLineComment;
                } else {
                    uncommentedCode += source[i];
                }

                break;
            }

            case InlineComment: {
                if ( source[i] == '\n' ) {
                    state = RegularCode;
                }

                break;
            }

            case MultiLineComment: {
                if ( source[i] == '*' && source[i + 1] == '/' ) {
                    state = RegularCode;
                    ++i;
                }

                break;
            }
        }
    }

    uncommentedCode += source[static_cast < Index > ( source.length() - 1u )];

    return uncommentedCode;
}

auto Preprocessor :: instance() noexcept -> Preprocessor & {
    static UniquePointer < Preprocessor > preprocessor;

    if ( preprocessor == nullptr ) {
        preprocessor = new Preprocessor();
    }

    return * preprocessor;
}

auto static load ( Path const & path ) noexcept -> String {
    std :: fstream source;
    std :: stringstream stream;

    source.open( path.toString(), std :: ios :: in );

    stream << source.rdbuf();
    return stream.str();
}

auto Preprocessor :: Job :: include ( String const & includeData ) noexcept (false) -> String { // NOLINT(misc-no-recursion)
    bool tryRelative = false;
    String filePath;

    if ( includeData.startsWith("\"") ) {
        tryRelative = true;
        filePath = includeData.removePrefix("\"").removeSuffix("\"");
    } else {
        filePath = includeData.removePrefix("<").removeSuffix(">");
    }

    if ( tryRelative ) {
        try {
            Path relativePath = filePath;

            return this->start ( relativePath );
        } catch ( Exception const & err ) {

        }
    }

    for ( auto const & includedDirectory : * Preprocessor :: instance()._includedDirectories ) {
        try {
            Path completePath = * includedDirectory / filePath;

            return this->start ( completePath );
        } catch ( Exception const & err ) {

        }
    }

    throw Error ( filePath + " not found" );
}

Preprocessor :: Preprocessor() noexcept :
        _includedDirectories( new Array < SharedPointer < Path > > ){

}

auto Preprocessor :: addIncludeDirectory ( Path const & path ) noexcept -> void {
    this->_includedDirectories->add ( path.copy() );
}

auto Preprocessor :: Job :: preprocessLines ( Collection < String > const & lines ) noexcept -> String { // NOLINT(misc-no-recursion)
    Array < String > preprocessedLines;
    Define * multiLineDefine = nullptr;

    String multiLineIf;
    String multiLineElseIf;

    for ( auto const & line : lines ) {

//        this->printBranchTrace( std::clog, line );

        auto trimmed = line.trim(" \t");

        if ( multiLineDefine != nullptr && this->inTrueBranch() ) {
            multiLineDefine->addToBody( String(trimmed).removeSuffix("\\").trim() );

            if ( ! trimmed.endsWith("\\") ) {
                multiLineDefine = nullptr;
            }

            continue;
        }

        if ( ! multiLineIf.empty() && this->inTrueBranch() ) {
            multiLineIf += String(trimmed).removeSuffix("\\").trim();

            if ( ! trimmed.endsWith("\\") ) {
                this->_if ( multiLineIf );
                multiLineIf.clear();
            }

            continue;
        }

        if ( ! multiLineElseIf.empty() && this->inTrueBranch() ) {
            multiLineElseIf += String(trimmed).removeSuffix("\\").trim();

            if ( ! trimmed.endsWith("\\") ) {
                this->elseIf ( multiLineElseIf );
                multiLineElseIf.clear();
            }

            continue;
        }

        if ( trimmed.startsWith("#") ) {
            auto preprocessData = extractPreprocessData( trimmed.removePrefix("#").trim() );

            try {

                switch ( preprocessData.first() ) {
                    case DirectiveType :: Include: {
                        if ( this->inTrueBranch() ) {
                            preprocessedLines.add(this->include(preprocessData.second()));
                        }
                        break;
                    }
                    case DirectiveType :: Define: {
                        if ( this->inTrueBranch() ) {
                            this->define ( preprocessData.second(), multiLineDefine );
                        }
                        break;
                    }
                    case DirectiveType :: Undefine: {
                        if ( this->inTrueBranch() ) {
                            this->undefine ( preprocessData.second() );
                        }
                        break;
                    }
                    case DirectiveType :: If: {
                        this->ifStart ( preprocessData.second(), multiLineIf );
                        break;
                    }
                    case DirectiveType :: ElseIf: {
                        this->elseIfStart ( preprocessData.second(), multiLineElseIf );
                        break;
                    }
                    case DirectiveType :: Else: {
                        this->_else();
                        break;
                    }
                    case DirectiveType :: EndIf: {
                        this->endIf();
                        break;
                    }
                    case DirectiveType :: IfDefined: {
                        this->ifDefined( preprocessData.second() );
                        break;
                    }
                    case DirectiveType :: IfNotDefined: {
                        this->ifNotDefined( preprocessData.second() );
                        break;
                    }
                    case DirectiveType :: Warning:
                        break;
                    case DirectiveType :: Error:
                        break;
                    case DirectiveType :: Pragma:
                        break;
                    case DirectiveType :: InvalidDirective:
                        break;
                }

            } catch ( Warning const & e ) {
                std :: cout << "Warning : " << e.toString() << '\n';
                std :: cout << "Occurred in " << ** ( includeBacktrace->begin() ) << '\n';

                if ( includeBacktrace->size() > 1u ) {
                    for ( auto it = ++ includeBacktrace->begin(), end = includeBacktrace->end(); it != end; ++ it ) { // NOLINT(clion-misra-cpp2008-8-0-1)
                        std :: cout << "\tincluded from " << ** it << '\n';
                    }
                }

                std :: cout << '\n';
            } catch ( Error const & e ) {
                std :: cerr << "Error : " << e.toString() << '\n';
                std :: cerr << "Occurred in " << ** ( includeBacktrace->begin() ) << '\n';

                if ( includeBacktrace->size() > 1u ) {
                    for ( auto it = ++ includeBacktrace->begin(), end = includeBacktrace->end(); it != end; ++ it ) { // NOLINT(clion-misra-cpp2008-8-0-1)
                        std :: cerr << "\tincluded from " << ** it << '\n';
                    }
                }

                std :: cerr << '\n';
            } catch ( Exception const & e ) {
                std :: cerr << "Unexpected Error : " << e.toString() << '\n';
                std :: cerr << "Occurred in " << ** ( includeBacktrace->begin() ) << '\n';

                if ( includeBacktrace->size() > 1u ) {
                    for ( auto it = ++ includeBacktrace->begin(), end = includeBacktrace->end(); it != end; ++ it ) { // NOLINT(clion-misra-cpp2008-8-0-1)
                        std :: cerr << "\tincluded from " << ** it << '\n';
                    }
                }

                std :: cerr << '\n';
            }

        } else {
            if ( this->inTrueBranch() ) {
                preprocessedLines.add(line);
            }
        }
    }

    return String :: join ( preprocessedLines, "\n" );
}

auto Preprocessor :: Job :: inTrueBranch() noexcept -> bool {
    return this->branchBacktrace->empty() || Preprocessor :: isTrueBranchType ( this->branchBacktrace->peek() );
}

auto Preprocessor :: Job :: start( Path const & path ) noexcept -> String { // NOLINT(misc-no-recursion)
    (void) this->includeBacktrace->push ( path.copy() );
    auto preprocessedCode = this->preprocessLines( uncomment ( load ( path ) ).split ( '\n' ) );
    (void) this->includeBacktrace->pop();

    return preprocessedCode;
}

auto Preprocessor :: preprocess ( Path const & path ) noexcept -> String {
    Job preprocessJob;

    return preprocessJob.start(path);
}

Preprocessor :: Job :: Job() noexcept :
        definitions ( new Array < SharedPointer < Define > > ),
        includeBacktrace ( new Stack < SharedPointer < Path > > ),
        branchBacktrace ( new Stack < BranchType > ) {

}

auto Preprocessor :: Job :: isDefined ( String const & definitionName ) const noexcept -> bool {
    return this->definitions->any([& definitionName](auto const & e){
        return e->name() == definitionName;
    });
}

auto Preprocessor :: Job :: define ( String defineData, Define * & pMultiLineDefine ) noexcept(false) -> void {
    bool toggleMultiLine = false;
    bool defineReplaced = false;

    pMultiLineDefine = nullptr;

    if ( defineData.trim().endsWith("\\") ) {
        (void) defineData.trim().removeSuffix("\\");
        toggleMultiLine = true;
    }

    auto newDefine = new Define ( defineData );

    if ( toggleMultiLine ) {
        pMultiLineDefine = newDefine;
    }

    for ( auto const & define : * this->definitions ) {
        if ( define->name() == newDefine->name() ) {
            (void) this->definitions->removeFirst(define);
            defineReplaced = true;
            break;
        }
    }

    this->definitions->add ( newDefine );

    if ( defineReplaced ) {
        throw Warning ( "Redefinition of Define '" + newDefine->name() + "'" );
    }
}

auto Preprocessor :: Job :: undefine ( String const & definitionName ) noexcept(false) -> void {
    for ( auto const & definition : * this->definitions ) {
        if ( definition->name() == definitionName ) {
            (void) this->definitions->removeFirst( definition );
            return;
        }
    }

    throw Warning ( "Token '" + definitionName + "' never defined" );
}

auto Preprocessor :: Job :: ifDefined ( String const & definitionName ) noexcept -> void {
    if (
            (
                    this->branchBacktrace->empty() ||
                    Preprocessor :: isTrueBranchType ( this->branchBacktrace->peek() )
            ) &&
            this->isDefined ( definitionName )
    ) {
        (void) this->branchBacktrace->push( BranchType :: IfTrue );
    } else {
        (void) this->branchBacktrace->push( BranchType :: IfFalse );
    }
}

auto Preprocessor :: Job :: ifNotDefined ( String const & definitionName ) noexcept -> void {
    if (
            (
                    this->branchBacktrace->empty() ||
                    Preprocessor :: isTrueBranchType ( this->branchBacktrace->peek() )
            ) &&
            ! this->isDefined ( definitionName )
    ) {
        (void) this->branchBacktrace->push ( BranchType :: IfTrue );
    } else {
        (void) this->branchBacktrace->push ( BranchType :: IfFalse );
    }
}

auto Preprocessor :: Job :: _else () noexcept ( false ) -> void {
    if ( this->branchBacktrace->empty() ) {
        throw Error ( "#else preprocessor directive without a previous if" );
    }

    auto currentBranchType = this->branchBacktrace->pop();

    if (
            this->branchBacktrace->empty() ||
            Preprocessor :: isTrueBranchType ( this->branchBacktrace->peek() )
    ) {
        if (
                currentBranchType == BranchType :: IfFalse ||
                currentBranchType == BranchType :: ElseIfFalseUnevaluated
        ) {
            (void) this->branchBacktrace->push ( BranchType :: ElseTrue );
        } else {
            if (
                    currentBranchType == BranchType :: ElseTrue ||
                    currentBranchType == BranchType :: ElseFalse
            ) {
                throw Error ( "#else preprocessor directive without a previous if" );
            } else {
                (void) this->branchBacktrace->push ( BranchType :: ElseFalse );
            }
        }
    } else {
        (void) this->branchBacktrace->push ( BranchType :: ElseFalse );
    }
}

auto Preprocessor :: Job :: endIf() noexcept(false) -> void {
    if ( this->branchBacktrace->empty() ) {
        throw Error ( "#endif preprocessor directive without a previous if" );
    }

    (void) this->branchBacktrace->pop();
}

auto Preprocessor :: Job :: ifStart ( String const & condition, String & multiLineIf ) noexcept (false) -> void {
    multiLineIf.clear();

    if ( condition.trim().endsWith("\\") ) {
        multiLineIf = condition.trim().removeSuffix("\\");
    }

    if ( ! multiLineIf.empty() ) {
        return;
    }

    return this->_if ( condition.trim() );
}

auto Preprocessor :: Job :: elseIfStart ( String const & condition, String & multiLineElseIf ) noexcept(false) -> void {
    multiLineElseIf.clear();

    if ( condition.trim().endsWith("\\") ) {
        multiLineElseIf = condition.trim().removeSuffix("\\");
    }

    if ( ! multiLineElseIf.empty() ) {
        return;
    }

    return this->elseIf ( condition.trim() );
}

auto Preprocessor :: Job :: _if ( String const & condition ) noexcept (false) -> void {
    if (
            (
                    this->branchBacktrace->empty() ||
                    Preprocessor :: isTrueBranchType ( this->branchBacktrace->peek() )
            ) &&
            this->evaluate ( condition )
    ) {
        (void) this->branchBacktrace->push ( BranchType :: IfTrue );
    } else {
        (void) this->branchBacktrace->push ( BranchType :: IfFalse );
    }
}

auto Preprocessor :: Job :: elseIf ( String const & condition ) noexcept (false) -> void {
    if ( this->branchBacktrace->empty() ) {
        throw Error ( "#elif preprocessor directive without a previous if" );
    }

    auto currentBranchType = this->branchBacktrace->pop();

    if (
            this->branchBacktrace->empty() ||
            Preprocessor :: isTrueBranchType ( this->branchBacktrace->peek() )
    ) {
        if (
                currentBranchType == BranchType :: IfTrue       ||
                currentBranchType == BranchType :: ElseIfTrue   ||
                currentBranchType == BranchType :: ElseIfFalseEvaluated
        ) {
            (void) this->branchBacktrace->push ( BranchType :: ElseIfFalseEvaluated );
        } else if (
                currentBranchType == BranchType :: ElseIfFalseUnevaluated ||
                currentBranchType == BranchType :: IfFalse
        ) {
            if ( this->evaluate ( condition ) ) {
                (void) this->branchBacktrace->push(BranchType::ElseIfTrue);
            } else {
                (void) this->branchBacktrace->push(BranchType::ElseIfFalseUnevaluated);
            }
        } else {
            throw Error ("Invalid Evaluation of Else If");
        }
    } else {
        (void) this->branchBacktrace->push ( BranchType :: ElseIfFalseUnevaluated );
    }
}

#include <CDS/HashMap>
#include <CDS/Queue>

auto Preprocessor :: Job :: evaluate ( String const & condition ) const noexcept (false) -> bool {
//    std :: cout << condition << '\n';

    static auto removeWhitespace = []( String const & str, String const & whitespace = " \t\n\r" ) noexcept -> String {
        String removed;
        for ( auto c : str ) {
            if ( ! whitespace.contains(c) ) {
                removed += c;
            }
        }

        return removed;
    };

    static auto replacePrefixOperators = []( String const & str, String const & prefixPossibleOperators = "-+~!" ) noexcept -> String {
        String final;
        bool prefixPossible = true;

        for ( Index i = 0; static_cast < Size > ( i ) < str.length(); ++ i ) {
            if ( prefixPossible && prefixPossibleOperators.contains (str[i]) ) {
                final += String(1u, str[i]) + "u";
            } else {
                if (
                        str[i] == '(' && prefixPossibleOperators.contains(str[i+1]) ||
                        ( ! String::isAlpha(str[i]) && str[i] != '_' )
                ) {
                    prefixPossible = true;
                } else {
                    prefixPossible = false;
                }

                final += str[i];
            }
        }

        return final;
    };

    const int bitwisePrecedence = 1;
    const int logicalPrecedence = 2;
    const int booleanPrecedence = 3;
    const int comparisonPrecendece = 4;
    const int linearArithmeticPrecendece = 5;
    const int compoundArithmeticPrecedence = 6;

    HashMap < String, Pair < int, int > > operatorPrecedenceAndOpCount = {
            { "&", { bitwisePrecedence, 2 } },
            { "|", { bitwisePrecedence, 2 } },
            { "^", { bitwisePrecedence, 2 } },
            { "~u", { bitwisePrecedence, 1 } },
            { "<<", { bitwisePrecedence, 2 } },
            { ">>", { bitwisePrecedence, 2 } },
            { "!u", { booleanPrecedence, 1 } },
            { "&&", { booleanPrecedence, 2 } },
            { "||", { booleanPrecedence, 2 } },
            { "==", { comparisonPrecendece, 2 } },
            { "!=", { comparisonPrecendece, 2 } },
            { ">", { comparisonPrecendece, 2 } },
            { "<", { comparisonPrecendece, 2 } },
            { "<=", { comparisonPrecendece, 2 } },
            { ">=", { comparisonPrecendece, 2 } },
            { "+", { linearArithmeticPrecendece, 2 } },
            { "-", { linearArithmeticPrecendece, 2 } },
            { "+u", { linearArithmeticPrecendece, 1 } },
            { "-u", { linearArithmeticPrecendece, 1 } },
            { "*", { compoundArithmeticPrecedence, 2 } },
            { "/", { compoundArithmeticPrecedence, 2 } },
            { "%", { compoundArithmeticPrecedence, 2 } },
    };

    static auto conditionToPostfix = []( String const & condition, HashMap < String, Pair < int, int > > const & precedenceAndOpCount ) noexcept (false) -> String {
        Stack < String > bracketedStack;
        String token;

        String postfixed;

        static constexpr auto isTokenCharacter = [] (char c) noexcept -> bool {
            return String :: isAlpha ( c ) || c == '_';
        };

        auto str = replacePrefixOperators(removeWhitespace(condition));
        for ( Index i = 0; static_cast < Size > ( i ) < str.length(); ++ i ) { // NOLINT(clion-misra-cpp2008-6-5-3)
            if ( isTokenCharacter (str[i]) ) {
                token += str[i];
                continue;
            } else if ( ! token.empty() ) {
                postfixed += token + ' ';
                token.clear();
            } else {
                /// do nothing
            }

            if ( str[i] == '(' ) {
                (void) bracketedStack.push ( String(1u, str[i]) );
            } else if ( str[i] == ')' ) {
                while ( ! bracketedStack.empty() && bracketedStack.peek() != "(" ) {
                    postfixed += bracketedStack.pop() + " ";
                }

                if ( ! bracketedStack.empty() && bracketedStack.peek() == "(" ) {
                    (void)bracketedStack.pop();
                }
            } else {
                String op ( 1u, str[i] );
                String op2 ( { str[i], str[i + 1] } );

                String fOp = "";

                if ( precedenceAndOpCount.containsKey( op2 ) ) {
                    fOp = op2;
                    ++ i;
                } else if ( precedenceAndOpCount.containsKey( op ) ) {
                    fOp = op;
                } else {
                    throw Error ("Invalid Operator : "_s + str[i]);
                }

                while ( ! bracketedStack.empty() && precedenceAndOpCount[fOp].first() <= precedenceAndOpCount[bracketedStack.peek()].first() ) {
                    postfixed += bracketedStack.pop() + " ";
                }

                (void) bracketedStack.push(fOp);
            }
        }

        if ( ! token.empty() ) {
            postfixed += token + " ";
        }

        while ( ! bracketedStack.empty() ) {
            postfixed += bracketedStack.pop() + " ";
        }

        return postfixed;
    };

    auto postfixed = conditionToPostfix ( condition, operatorPrecedenceAndOpCount );

    auto convertToLong = [this] ( cds :: String const & token ) noexcept (false) -> sint64 {
        auto trimmed = token.trim();

        if ( trimmed == "true" ) { return 1; }
        if ( trimmed == "false" ) { return 0; }

        //// add token replacement

        (void) token
            .removeSuffix("l").removeSuffix("L")
            .removeSuffix("l").removeSuffix("L")
            .removeSuffix("u").removeSuffix("U")
            .removeSuffix("l").removeSuffix("L")
            .removeSuffix("l").removeSuffix("L");

        if ( token.lower().startsWith( "0b" ) ) {
            if ( token.any ( [](char c){ return ! "01bB"_s.contains(c); } ) ) {
                throw Error ( "Invalid character in binary integer : " + token );
            }

            return strtoll ( token.cStr() + 2, nullptr, 2 );
        } else if ( token.startsWith( "0" ) && token.length() > 1u ) {
            if ( token.any ( [](char c){ return ! "01234567"_s.contains(c); } ) ) {
                throw Error ( "Invalid character in octal integer : " + token );
            }

            return strtoll ( token.cStr() + 1, nullptr, 8 );
        } else if ( token.startsWith( "0x" ) ) {
            if ( token.any ( [](char c){ return ! "0123456789abcdefABCDEFxX"_s.contains(c); } ) ) {
                throw Error ( "Invalid character in hexadecimal integer : " + token );
            }

            return strtoll ( token.cStr() + 2, nullptr, 16 );
        } else {
            if ( token.any ( [](char c){ return ! "0123456789"_s.contains(c); } ) ) {
                throw Error ( "Invalid character in integer : " + token );
            }

            return strtoll ( token.cStr(), nullptr, 10 );
        }
    };

    static auto treeTransformation = [& convertToLong] ( String const & postfixed, HashMap < String, Pair < int, int > > const & opsWithPrecendeceAndOpCount ) noexcept (false) -> ConditionNode * {
        Stack < ConditionNode * > stack;

        for ( auto const & token : postfixed.split(' ') ) {
            if ( ! opsWithPrecendeceAndOpCount.containsKey( token ) ) {
                (void) stack.push ( new ConditionNode { "", convertToLong (token), nullptr, nullptr } );
            } else {
                if ( opsWithPrecendeceAndOpCount[token].second() == 1 ) {
                    if ( stack.empty() ) {
                        throw Error ( "Invalid Parse Result, Expected one operand for unary "_s + token );
                    }

                    (void) stack.push ( new ConditionNode { token, 0, stack.pop(), nullptr } );
                } else if ( opsWithPrecendeceAndOpCount[token].second() == 2 ) {
                    if ( stack.size() <= 1u ) {
                        throw Error ( "Invalid Parse Result, Expected two operands for binary "_s + token );
                    }

                    auto rightOp = stack.pop();
                    (void) stack.push ( new ConditionNode { token, 0, stack.pop(), rightOp } );
                } else {
                    throw Error ( "Invalid Operand, No operand count specified for operator "_s + token );
                }
            }
        }

        return stack.peek();
    };

    static auto destroyParseTree = [] ( ConditionNode * & tree, bool debug = false ) noexcept -> void {
        Queue < ConditionNode * > pToParse;
        (void) pToParse.push ( tree );

        while ( ! pToParse.empty() ) {
            auto current = pToParse.pop();

            if ( debug ) {
                std :: cout << current->operand << ' ';
            }

            if ( current->pLeft != nullptr ) {
                (void) pToParse.push( current->pLeft );
            }

            if ( current->pRight != nullptr ) {
                (void) pToParse.push( current->pRight );
            }

            delete current;
        }

        if ( debug ) {
            std :: cout << '\n';
        }
    };

    auto tree = treeTransformation ( postfixed, operatorPrecedenceAndOpCount );
    auto retVal = this->evaluateTree ( tree );

    destroyParseTree ( tree );

    return retVal;
}

auto Preprocessor :: Job :: evaluateTree (ConditionNode * pNode) const noexcept (false) -> sint64 {
    if ( pNode->_operator == "&" ) {
        return this->evaluateTree( pNode->pLeft ) & this->evaluateTree( pNode->pRight );
    } else if ( pNode->_operator == "|" ) {
        return this->evaluateTree( pNode->pLeft ) | this->evaluateTree( pNode->pRight );
    } else if ( pNode->_operator == "^" ) {
        return this->evaluateTree( pNode->pLeft ) ^ this->evaluateTree( pNode->pRight );
    } else if ( pNode->_operator == "~u" ) {
        return ~ this->evaluateTree( pNode->pLeft );
    } else if ( pNode->_operator == "<<" ) {
        return this->evaluateTree( pNode->pLeft ) << this->evaluateTree( pNode->pRight );
    } else if ( pNode->_operator == ">>" ) {
        return this->evaluateTree( pNode->pLeft ) >> this->evaluateTree( pNode->pRight );
    } else if ( pNode->_operator == "!u" ) {
        return static_cast < sint64 > ( ! static_cast < bool > ( this->evaluateTree( pNode->pLeft ) ) );
    } else if ( pNode->_operator == "&&" ) {
        return
            static_cast < sint64 > (
                static_cast < bool > ( this->evaluateTree( pNode->pLeft ) ) &&
                static_cast < bool > ( this->evaluateTree( pNode->pRight ) )
            );
    } else if ( pNode->_operator == "||" ) {
        return
            static_cast < sint64 > (
                    static_cast < bool > ( this->evaluateTree( pNode->pLeft ) ) ||
                    static_cast < bool > ( this->evaluateTree( pNode->pRight ) )
            );
    } else if ( pNode->_operator == "==" ) {
        return static_cast < sint64 > ( this->evaluateTree( pNode->pLeft ) == this->evaluateTree( pNode->pRight ) );
    } else if ( pNode->_operator == "!=" ) {
        return static_cast < sint64 > ( this->evaluateTree( pNode->pLeft ) != this->evaluateTree( pNode->pRight ) );
    } else if ( pNode->_operator == ">" ) {
        return static_cast < sint64 > ( this->evaluateTree( pNode->pLeft ) > this->evaluateTree( pNode->pRight ) );
    } else if ( pNode->_operator == "<" ) {
        return static_cast < sint64 > ( this->evaluateTree( pNode->pLeft ) < this->evaluateTree( pNode->pRight ) );
    } else if ( pNode->_operator == "<=" ) {
        return static_cast < sint64 > ( this->evaluateTree( pNode->pLeft ) <= this->evaluateTree( pNode->pRight ) );
    } else if ( pNode->_operator == ">=" ) {
        return static_cast < sint64 > ( this->evaluateTree( pNode->pLeft ) >= this->evaluateTree( pNode->pRight ) );
    } else if ( pNode->_operator == "+" ) {
        return this->evaluateTree( pNode->pLeft ) + this->evaluateTree( pNode->pRight );
    } else if ( pNode->_operator == "-" ) {
        return this->evaluateTree( pNode->pLeft ) - this->evaluateTree( pNode->pRight );
    } else if ( pNode->_operator == "+u" ) {
        return + this->evaluateTree( pNode->pLeft );
    } else if ( pNode->_operator == "-u" ) {
        return - this->evaluateTree( pNode->pLeft );
    } else if ( pNode->_operator == "*" ) {
        return this->evaluateTree( pNode->pLeft ) * this->evaluateTree( pNode->pRight );
    } else if ( pNode->_operator == "/" ) {
        return this->evaluateTree( pNode->pLeft ) / this->evaluateTree( pNode->pRight );
    } else if ( pNode->_operator == "%" ) {
        return this->evaluateTree( pNode->pLeft ) % this->evaluateTree( pNode->pRight );
    } else {
        return pNode->operand;
    }
}

auto Preprocessor :: Job :: replace ( String const & str ) const noexcept (false) -> String {

}

#ifndef NDEBUG

__CDS_MaybeUnused auto Preprocessor :: Job :: printBranchTrace ( std :: ostream & out, String const & additionalInfo ) noexcept -> void {
    out << "-"_s * 30 << " Conditional Backtrace " << "-"_s * 30 << '\n';
    out << "\t\tAdditional Info : " << additionalInfo << '\n';
    for ( auto const & e : * this->branchBacktrace ) {
        out << "\t" << Preprocessor :: branchTypeToString ( e ) << '\n';
    }

    out << '\n';
}

#endif