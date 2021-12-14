//
// Created by loghin on 07.12.2021.
//

#ifndef CPP_PARSER_PREPROCESSOR_HPP
#define CPP_PARSER_PREPROCESSOR_HPP

#include <CDS/Pointer>
#include <CDS/Pair>

namespace cds {
    class Path;
    class String;

    template < typename T >
    class Collection;

    template < typename T >
    class Array;

    template < typename T >
    class Stack;
}

class Define;

class Preprocessor : cds :: Object {
private:

    struct ConditionNode {
        cds :: String _operator; // NOLINT(clion-misra-cpp2008-11-0-1)

        cds :: sint64 operand { 0 }; // NOLINT(clion-misra-cpp2008-11-0-1)

        ConditionNode * pLeft   { nullptr }; // NOLINT(clion-misra-cpp2008-11-0-1)
        ConditionNode * pRight  { nullptr }; // NOLINT(clion-misra-cpp2008-11-0-1)
    };

    cds :: UniquePointer < cds :: Array < cds :: SharedPointer < cds :: Path > > > _includedDirectories;

    Preprocessor () noexcept;

public:
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

    enum class BranchType {
        IfTrue,
        IfFalse,
        ElseIfTrue,
        ElseIfFalseEvaluated,
        ElseIfFalseUnevaluated,
        ElseTrue,
        ElseFalse
    };

    constexpr auto static branchTypeToString ( BranchType type ) noexcept -> cds :: StringLiteral {
        cds :: StringLiteral name;

        switch ( type ) {
            case BranchType::IfTrue:                    { name = "IfTrue";                  break; }
            case BranchType::IfFalse:                   { name = "IfFalse";                 break; }
            case BranchType::ElseIfTrue:                { name = "ElsIfTrue";               break; }
            case BranchType::ElseIfFalseEvaluated:      { name = "ElseIfFalseEvaluated";    break; }
            case BranchType::ElseIfFalseUnevaluated:    { name = "ElseIfFalseUnevalueted";  break; }
            case BranchType::ElseTrue:                  { name = "ElseTrue";                break; }
            case BranchType::ElseFalse:                 { name = "ElseFalse";               break; }
        }

        return name;
    }

    constexpr auto static isTrueBranchType ( BranchType type ) noexcept -> bool {
        bool value;

        switch ( type ) {
            case BranchType::IfTrue:
            case BranchType::ElseIfTrue:
            case BranchType::ElseTrue:
                value = true;
                break;
            case BranchType::IfFalse:
            case BranchType::ElseIfFalseEvaluated:
            case BranchType::ElseIfFalseUnevaluated:
            case BranchType::ElseFalse:
                value = false;
                break;
        }

        return value;
    }

    constexpr auto static directiveTypeAsString ( DirectiveType type ) noexcept -> cds :: StringLiteral {
        cds :: StringLiteral code;

        switch ( type ) {
            case DirectiveType :: Include:          { code = "include";     break; }
            case DirectiveType :: Define:           { code = "define";      break; }
            case DirectiveType :: Undefine:         { code = "undef";       break; }
            case DirectiveType :: If:               { code = "if";          break; }
            case DirectiveType :: ElseIf:           { code = "elif";        break; }
            case DirectiveType :: Else:             { code = "else";        break; }
            case DirectiveType :: EndIf:            { code = "endif";       break; }
            case DirectiveType :: IfDefined:        { code = "ifdef";       break; }
            case DirectiveType :: IfNotDefined:     { code = "ifndef";      break; }
            case DirectiveType :: Warning:          { code = "warning";     break; }
            case DirectiveType :: Error:            { code = "error";       break; }
            case DirectiveType :: Pragma:           { code = "pragma";      break; }
            case DirectiveType :: InvalidDirective: { code = "invalid";     break; }
        }

        return code;
    }

    auto static extractPreprocessData ( cds :: String const & line ) noexcept -> cds :: Pair < DirectiveType, cds :: String > {
        if ( line.startsWith ( directiveTypeAsString ( DirectiveType :: Include ) ) ) {
            return { DirectiveType :: Include, line.removePrefix( directiveTypeAsString ( DirectiveType :: Include ) ).trim() };
        } else if ( line.startsWith ( directiveTypeAsString ( DirectiveType :: Define ) ) ) {
            return { DirectiveType :: Define, line.removePrefix( directiveTypeAsString ( DirectiveType :: Define ) ).trim() };
        } else if ( line.startsWith ( directiveTypeAsString ( DirectiveType :: Undefine ) ) ) {
            return { DirectiveType :: Undefine, line.removePrefix( directiveTypeAsString ( DirectiveType :: Undefine ) ).trim() };
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
        } else if ( line.startsWith ( directiveTypeAsString ( DirectiveType :: If ) ) ) {
            return { DirectiveType :: If, line.removePrefix( directiveTypeAsString ( DirectiveType :: If ) ).trim() };
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

    auto static instance () noexcept -> Preprocessor &;

    class Job : public cds :: Object {
    private:
        cds :: UniquePointer < cds :: Collection < cds :: SharedPointer < Define > > >  definitions;
        cds :: UniquePointer < cds :: Stack < cds :: SharedPointer < cds :: Path > > >  includeBacktrace;
        cds :: UniquePointer < cds :: Stack < BranchType > >                            branchBacktrace;

        auto preprocessLines ( cds :: Collection < cds :: String > const & ) noexcept -> cds :: String;
        auto include ( cds :: String const & ) noexcept ( false ) -> cds :: String;

        auto evaluateTree ( ConditionNode * ) const noexcept (false) -> cds :: sint64;

    public:
        explicit Job () noexcept;

        auto start ( cds :: Path const & ) noexcept -> cds :: String;

        auto isDefined ( cds :: String const & ) const noexcept -> bool;
        auto define ( cds :: String, Define * & ) noexcept (false) -> void;
        auto undefine ( cds :: String const & ) noexcept (false) -> void;

        auto ifDefined ( cds :: String const & ) noexcept -> void;
        auto ifNotDefined ( cds :: String const & ) noexcept -> void;
        auto endIf () noexcept (false) -> void;
        auto _else () noexcept (false) -> void;

        auto ifStart ( cds :: String const &, cds :: String & ) noexcept (false) -> void;
        auto elseIfStart ( cds :: String const &, cds :: String & ) noexcept (false) -> void;

        auto _if ( cds :: String const & ) noexcept (false) -> void;
        auto elseIf ( cds :: String const & ) noexcept (false) -> void;

        auto inTrueBranch () noexcept -> bool;

        auto evaluate ( cds :: String const & ) const noexcept (false) -> bool;

#ifndef NDEBUG
        auto printBranchTrace ( std :: ostream &, cds :: String const & = "" ) noexcept -> void;
#endif

    };

    auto static preprocess ( cds :: Path const & ) noexcept -> cds :: String;
    auto addIncludeDirectory ( cds :: Path const & ) noexcept -> void;
};


#endif //CPP_PARSER_PREPROCESSOR_HPP
