//
// Created by loghin on 07.12.2021.
//

#include "Preprocessor.hpp"

using namespace cds; // NOLINT(clion-misra-cpp2008-7-3-4)

auto Preprocessor :: instance() noexcept -> Preprocessor & {
    static UniquePointer < Preprocessor > preprocessor;

    if ( preprocessor == nullptr ) {
        preprocessor = new Preprocessor();
    }

    return * preprocessor;
}

#include <fstream>
#include <sstream>

#include <CDS/Path>

#include <Define.hpp>

auto static load ( Path const & path ) noexcept -> String {
    std :: fstream source;
    std :: stringstream stream;

    source.open( path.toString(), std :: ios :: in );

    stream << source.rdbuf();
    return stream.str();
}

enum class DirectiveType {
    Include,
    Define,
    Undefine,
    If,
    ElseIf,
    Else,
    EndIf,
    IfDefined,
    IfNotDefined,
    Warning,
    Error,
    Pragma,
    InvalidDirective
};

constexpr auto static directiveTypeAsString ( DirectiveType type ) noexcept -> StringLiteral {
    StringLiteral code;

    switch ( type ) {
        case DirectiveType :: Include:       { code = "include";     break; }
        case DirectiveType :: Define:        { code = "define";      break; }
        case DirectiveType :: Undefine:      { code = "undef";       break; }
        case DirectiveType :: If:            { code = "if";          break; }
        case DirectiveType :: ElseIf:        { code = "elif";        break; }
        case DirectiveType :: Else:          { code = "else";        break; }
        case DirectiveType :: EndIf:         { code = "endif";       break; }
        case DirectiveType :: IfDefined:     { code = "ifdef";       break; }
        case DirectiveType :: IfNotDefined:  { code = "ifndef";      break; }
        case DirectiveType :: Warning:       { code = "warning";     break; }
        case DirectiveType :: Error:         { code = "error";       break; }
        case DirectiveType :: Pragma:        { code = "pragma";      break; }
        default:            { break; }
    }

    return code;
}

auto static extractPreprocessData ( String const & line ) noexcept -> Pair < DirectiveType, String > {
    if ( line.startsWith ( directiveTypeAsString ( DirectiveType :: Include ) ) ) {
        return { DirectiveType :: Include, line.removePrefix( directiveTypeAsString ( DirectiveType :: Include ) ).trim() };
    } else if ( line.startsWith ( directiveTypeAsString ( DirectiveType :: Define ) ) ) {
        return { DirectiveType :: Define, line.removePrefix( directiveTypeAsString ( DirectiveType :: Define ) ).trim() };
    } else if ( line.startsWith ( directiveTypeAsString ( DirectiveType :: Undefine ) ) ) {
        return { DirectiveType :: Undefine, line.removePrefix( directiveTypeAsString ( DirectiveType :: Undefine ) ).trim() };
    } else if ( line.startsWith ( directiveTypeAsString ( DirectiveType :: If ) ) ) {
        return { DirectiveType :: If, line.removePrefix( directiveTypeAsString ( DirectiveType :: If ) ).trim() };
    } else if ( line.startsWith ( directiveTypeAsString ( DirectiveType :: ElseIf ) ) ) {
        return { DirectiveType :: ElseIf, line.removePrefix( directiveTypeAsString ( DirectiveType :: ElseIf ) ).trim() };
    } else if ( line.startsWith ( directiveTypeAsString ( DirectiveType :: Else ) ) ) {
        return { DirectiveType :: Else, "" };
    } else if ( line.startsWith ( directiveTypeAsString ( DirectiveType :: EndIf ) ) ) {
        return { DirectiveType :: EndIf, "" };
    } else if ( line.startsWith ( directiveTypeAsString ( DirectiveType :: IfDefined ) ) ) {
        return { DirectiveType :: IfDefined, line.removePrefix( directiveTypeAsString ( DirectiveType :: IfDefined ) ).trim() };
    } else if ( line.startsWith ( directiveTypeAsString ( DirectiveType :: IfNotDefined ) ) ) {
        return { DirectiveType :: IfNotDefined, line.removePrefix( directiveTypeAsString ( DirectiveType :: IfNotDefined ) ).trim() };
    } else if ( line.startsWith ( directiveTypeAsString ( DirectiveType :: Warning ) ) ) {
        return { DirectiveType :: Warning, line.removePrefix( directiveTypeAsString ( DirectiveType :: Warning ) ).trim() };
    } else if ( line.startsWith ( directiveTypeAsString ( DirectiveType :: Error ) ) ) {
        return { DirectiveType :: Error, line.removePrefix( directiveTypeAsString ( DirectiveType :: Error ) ).trim() };
    } else if ( line.startsWith ( directiveTypeAsString ( DirectiveType :: Pragma ) ) ) {
        return { DirectiveType :: Pragma, line.removePrefix( directiveTypeAsString ( DirectiveType :: Pragma ) ).trim() };
    } else {
        return { DirectiveType :: InvalidDirective, "" };
    }
}

#include <CDS/HashSet>

auto Preprocessor :: include ( String const & includeData, Collection < SharedPointer < Define > > const & defines ) noexcept (false) -> String {
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

            return preprocess ( relativePath, defines );
        } catch ( Exception const & err ) {

        }
    }

    for ( auto const & includedDirectory : * Preprocessor :: instance()._includedDirectories ) {
        try {
            Path completePath = * includedDirectory / filePath;

            return preprocess ( completePath, defines );
        } catch ( Exception const & err ) {

        }
    }

    throw IllegalArgumentException ( filePath + " not found" );
}

Preprocessor :: Preprocessor() noexcept :
        _includedDirectories( new Array < SharedPointer < Path > > ){

}

auto Preprocessor :: addIncludeDirectory ( Path const & path ) noexcept -> void {
    Preprocessor :: instance()._includedDirectories->add ( path.copy() );
}

auto Preprocessor :: preprocessLines ( Collection < String > const & lines, Collection < SharedPointer < Define > > const & defines ) noexcept -> String { // NOLINT(misc-no-recursion)
    Array < String > preprocessedLines;
    Array < SharedPointer < Define > > currentDefines ( defines.begin(), defines.end() );

    for ( auto const & line : lines ) {

        auto trimmed = line.trim(" \t");
        if ( trimmed.startsWith("#") ) {
            auto preprocessData = extractPreprocessData( trimmed.removePrefix("#").trim() );

            switch ( preprocessData.first() ) {
                case DirectiveType :: Include: {
                    try {
                        preprocessedLines.add ( include ( preprocessData.second(), currentDefines ) );
                    } catch ( Exception const & e ) {
                        std :: cerr << "Error : " << e.toString() << '\n';
                    }

                    break;
                }
                case DirectiveType :: Define:
                    try {
                        currentDefines.add ( new Define ( preprocessData.second() ) );
                    } catch ( Exception const & e ) {
                        std :: cerr << "Error : " << e.toString() << '\n';
                    }

                    break;
                case DirectiveType :: Undefine:
                    break;
                case DirectiveType :: If:
                    break;
                case DirectiveType :: ElseIf:
                    break;
                case DirectiveType :: Else:
                    break;
                case DirectiveType :: EndIf:
                    break;
                case DirectiveType :: IfDefined:
                    break;
                case DirectiveType :: IfNotDefined:
                    break;
                case DirectiveType :: Warning:
                    break;
                case DirectiveType :: Error:
                    break;
                case DirectiveType :: Pragma:
                    break;
                case DirectiveType :: InvalidDirective:
                    break;
            }
        } else {
            preprocessedLines.add(line);
        }
    }

    std :: cout << "Defines at end of parse source\n";
    std :: cout << currentDefines << '\n';

    return String :: join ( preprocessedLines, "\n" );
}

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

auto Preprocessor :: preprocess ( Path const & path, Collection < SharedPointer < Define > > const & defines ) noexcept -> String { // NOLINT(misc-no-recursion)
    return preprocessLines ( uncomment ( load ( path ) ) .split ( '\n' ), defines );
}