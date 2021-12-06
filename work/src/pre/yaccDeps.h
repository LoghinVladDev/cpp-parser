extern "C" {
    int yylex(void);
    int yyerror(char const *);
}

#ifdef __cplusplus

#include <State.hpp>

using StateType = State *;

#endif