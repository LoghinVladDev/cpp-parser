//
// Created by loghin on 07.12.2021.
//

#include "State.hpp"

#include <CDS/DoubleLinkedList>

auto State :: addState ( State * pState ) noexcept -> void {
    this->_children->add ( pState );
}

auto State :: toString() const noexcept -> cds::String {
    return
        "State "_s +
        "{ type = " + State :: typeToString ( this->_type ) +
        " }";
}

State :: State () noexcept :
        _type ( Undefined ),
        _children ( new StatesType ){

}

State :: State ( State const & state ) noexcept :
        _type ( state._type ),
        _children ( state._children->copy() ) {

}

State :: State ( State && state ) noexcept :
        _type ( cds :: exchange ( state._type, Undefined ) ),
        _children ( std :: move ( state._children ) ) {

}

auto State :: operator= ( State const & state ) noexcept -> State & {
    if ( this == & state ) {
        return * this;
    }

    this->_type = state._type;
    this->_children = state._children->copy();

    return * this;
}

auto State :: operator = ( State && state ) noexcept -> State & {
    if ( this == & state ) {
        return * this;
    }

    this->_type = cds :: exchange ( state._type, Undefined );
    this->_children = std :: move ( state._children );

    return * this;
}

State :: State ( Type type ) noexcept :
        _type ( type ),
        _children ( new StatesType ) {

}

auto State :: typeToString (Type type) noexcept -> cds :: StringLiteral {
    cds :: StringLiteral typeName;

    switch ( type ) {

        case Root:
            typeName = "Root";
            break;

        case Statement:
            typeName = "Statement";
            break;

        case Statements:
            typeName = "Statements";
            break;

        case Undefined:
        default:
            typeName = "Undefined";
            break;
    }

    return typeName;
}

auto State :: rootState () noexcept -> cds :: UniquePointer < State > & {
    static cds :: UniquePointer < State > root;

    if ( root == nullptr ) {
        root = new State ( Root );
    }

    return root;
}

State :: State ( Type type, std :: initializer_list < State * > const & states ) noexcept :
        _type ( type ),
        _children ( new StatesType ) {

    for ( auto * pState : states ) {
        this->_children->add(pState);
    }
}

auto State :: children() const noexcept -> StatesType const & {
    return * this->_children;
}