//
// Created by loghin on 11.12.2021.
//

#ifndef CPP_PARSER_WARNING_HPP
#define CPP_PARSER_WARNING_HPP

#include <CDS/Exception>

class Warning : public cds :: Exception {
public:
    explicit Warning ( cds :: String const & message ) noexcept :
            Exception ( message ) {

    }
};

#endif //CPP_PARSER_WARNING_HPP
