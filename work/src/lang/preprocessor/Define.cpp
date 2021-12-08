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

}

auto Define::toString() const noexcept -> String {
    return "Define "_s +
        "{ name = " + name +
        ", paramsEnabled = " + paramsEnabled +
        ", parameters = " + parameters +
        ", body = " + body +
        " }";
}