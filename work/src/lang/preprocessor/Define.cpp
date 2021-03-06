//
// Created by loghin on 08.12.2021.
//

#include "Define.hpp"

#include <CDS/Boolean>
#include <CDS/Array>

using namespace cds; // NOLINT(clion-misra-cpp2008-7-3-4)

Define :: Define ( String const & definition ) noexcept(false):
        paramsEnabled ( new Boolean() ),
        parameters ( new Array < String > ){

    this->load ( definition );
}

auto Define::toString() const noexcept -> String {
    return "Define "_s +
        "{ name = " + _name +
        ", paramsEnabled = " + paramsEnabled +
        ", parameters = " + parameters +
        ", body = " + body +
        " }";
}

auto Define :: load ( String const & definition ) noexcept -> void {
    _name = definition.trim().split(" (")[0];

    auto bodyWithParams = definition.removePrefix(_name).trim();

    if ( ! bodyWithParams.empty() ) {

        if ( bodyWithParams.startsWith("(") ) {
            * paramsEnabled = true;
            (void) bodyWithParams
                .substr ( 0, bodyWithParams.findFirst(')') )
                .removePrefix("(").removeSuffix(")")
                .trim().split(",")
                .sequence().onEach([](auto & str){ (void) str.trim(); })
                .toArray( * parameters );

            bodyWithParams = bodyWithParams.substr(bodyWithParams.findFirst(')') + 1).trim();
        }

    }

    body = bodyWithParams;
}

auto Define :: addToBody( String const & text ) noexcept -> void {
    this->body += " " + text;
}