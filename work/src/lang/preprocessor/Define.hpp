//
// Created by loghin on 08.12.2021.
//

#ifndef CPP_PARSER_DEFINE_HPP
#define CPP_PARSER_DEFINE_HPP

#include <CDS/Pointer>

namespace cds {
    class Boolean;

    template < typename T >
    class Array;
}

class Define : public cds :: Object {
private:
    cds :: String _name;
    cds :: UniquePointer < cds :: Boolean > paramsEnabled;
    cds :: UniquePointer < cds :: Array < cds :: String > > parameters;
    cds :: String body;

    auto load ( cds :: String const & ) noexcept -> void;

public:
    __CDS_NoDiscard constexpr auto name () const noexcept -> cds :: String const & {
        return this->_name;
    }

    auto addToBody ( cds :: String const & ) noexcept -> void;

    explicit Define ( cds :: String const & ) noexcept (false);
    __CDS_NoDiscard auto toString() const noexcept -> cds :: String override;
};


#endif //CPP_PARSER_DEFINE_HPP
