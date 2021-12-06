//
// Created by loghin on 07.12.2021.
//

#ifndef CPP_PARSER_STATE_HPP
#define CPP_PARSER_STATE_HPP

#include <CDS/Pointer>

class State : public cds :: Object {
public:
    enum Type : cds :: uint16 {

        Undefined,

        Root,

        Statements,
        Statement

    };

private:
    using StatesType = cds :: DoubleLinkedList < cds :: SharedPointer < State > >;

    Type _type;
    cds :: UniquePointer < StatesType > _children;

public:

    static auto typeToString ( Type ) noexcept -> cds :: StringLiteral;

    State () noexcept;
    State ( State const & state ) noexcept;
    State ( State && state ) noexcept;

    explicit State ( Type ) noexcept;
    State ( Type, std :: initializer_list < State * > const & ) noexcept;

    auto operator = ( State const & state ) noexcept -> State &;
    auto operator = ( State && state ) noexcept -> State &;

    auto addState ( State * pState ) noexcept -> void;

    __CDS_NoDiscard auto toString () const noexcept -> cds :: String override;

    static auto rootState () noexcept -> cds :: UniquePointer < State > &;

    __CDS_NoDiscard auto children () const noexcept -> StatesType const &;
};


#endif //CPP_PARSER_STATE_HPP
