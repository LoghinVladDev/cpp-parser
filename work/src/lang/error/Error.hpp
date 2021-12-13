//
// Created by loghin on 11.12.2021.
//

#ifndef CPP_PARSER_ERROR_HPP
#define CPP_PARSER_ERROR_HPP

#include <CDS/Exception>

class Error : public cds :: Exception {
public:
    explicit Error ( cds :: String const & error ) noexcept :
            Exception ( error ) {

    }
};

#endif //CPP_PARSER_ERROR_HPP
