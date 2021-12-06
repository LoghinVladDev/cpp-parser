#include "tokenUtils.h"

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>



#define CPP_PARSER_C_OBJECT_START TokenUtils
#include "cDefs.h"



PrivateVariable(Type(StringLiteral), realNumberCharacters) = ".0123456789";

PrivateVariable(Type(StringLiteral), base2Characters) = "01";
PrivateVariable(Type(StringLiteral), base8Characters) = "01234567";
PrivateVariable(Type(StringLiteral), base10Characters) = "0123456789";
PrivateVariable(Type(StringLiteral), base16Characters) = "0123456789ABCDEFabcdef";


Function(Type(InputText), skipUntilFirst) ( Type(InputText) text, Type(Separators) separators ) {
    char const * p = text;
    for ( int i = 0, len = (int)strlen ( text ); i < len; ++ i ) {
        if ( strchr ( separators, * p ) != NULL ) {
            return p;
        } else {
            ++ p;
        }
    }

    return NULL;
}

Function(void, error) ( Type(ErrorMessageFormat) format, ... ) {
    va_list arguments;
    va_start( arguments, format );

    fprintf ( stderr, "TokenUtils Error : " );
    vfprintf ( stderr, format, arguments );

    va_end( arguments );

    fflush ( stderr );
}

Function(Type(Token), token) ( Type(InputText) inputText ) {
    return strdup ( inputText );
}

Function(Type(StringLiteral), stringLiteral) ( Type(InputText) inputText ) {
    Type(Buffer) temporaryBuffer = strdup ( inputText + 1 );
    temporaryBuffer [ strlen ( temporaryBuffer ) - 1 ] = '\0';

    return temporaryBuffer;
}

Function(Type(CharacterLiteral), characterLiteral) ( Type(InputText) inputText ) {
    return inputText[0];
}

Function(Type(DoubleLiteral), positiveDoubleLiteral) ( Type(InputText) inputText ) {
    return strtod ( inputText, NULL );
}

Function(Type(DoubleLiteral), negativeDoubleLiteral) ( Type(InputText) inputText ) {
    Type(InputText) firstOccurrence = Call(skipUntilFirst)( inputText, Variable(realNumberCharacters) );

    if ( firstOccurrence == NULL ) {
        Call(error)( "NegativeDoubleLiteral : %s", inputText );
        return 0.0;
    }

    return strtod ( inputText, NULL );
}

Function(Type(FloatLiteral), positiveFloatLiteral) ( Type(InputText) inputText ) {
    Type(Buffer) temporaryBuffer = strdup ( inputText );
    temporaryBuffer [ strlen ( temporaryBuffer ) - 1 ] = '\0';

    float returnValue = strtof ( temporaryBuffer, NULL );

    free ( temporaryBuffer );
    return returnValue;
}

Function(Type(FloatLiteral), negativeFloatLiteral) ( Type(InputText) inputText ) {
    Type(FloatLiteral) returnValue;
    Type(Buffer) temporaryBuffer = strdup ( inputText );
    temporaryBuffer [ strlen ( temporaryBuffer ) - 1 ] = '\0';

    Type(InputText) firstOccurrence = Call(skipUntilFirst)(inputText, Variable(realNumberCharacters));

    if ( firstOccurrence == NULL ) {
        Call(error)("NegativeFloatLiteral : %s", inputText);
        returnValue = 0.0f;
    } else {
        returnValue = strtof ( firstOccurrence, NULL );
    }

    free(temporaryBuffer);
    return returnValue;
}

Function(Type(SignedLongLiteral), positiveLongLiteral_Base16) ( Type(InputText) inputText ) {
    Type(SignedLongLiteral) returnValue;

    Type(InputText) firstOccurrence = Call(skipUntilFirst( Call(skipUntilFirst)( inputText, "xX" ), Variable(base16Characters) ) );

    if ( firstOccurrence == NULL ) {
        Call(error)( "PositiveLongLiteral : base = %d, modifiers = %s, value = %s", 16, "none", inputText );
        returnValue = 0x00;
    } else {
        returnValue = strtoll ( firstOccurrence, NULL, 16 );
    }

    return returnValue;
}

Function(Type(SignedLongLiteral), negativeLongLiteral_Base16) ( Type(InputText) inputText ) {
    Type(SignedLongLiteral) returnValue;

    Type(InputText) firstOccurrence = Call(skipUntilFirst( Call(skipUntilFirst)( inputText, "xX" ), Variable(base16Characters) ) );

    if ( firstOccurrence == NULL ) {
        Call(error)( "NegativeLongLiteral : base = %d, modifiers = %s, value = %s", 16, "none", inputText );
        returnValue = 0x00;
    } else {
        returnValue = - strtoll ( firstOccurrence, NULL, 16 );
    }

    return returnValue;
}

Function(Type(SignedLongLiteral), positiveLongLiteral_Base16Long) ( Type(InputText) inputText ) {
    Type(SignedLongLiteral) returnValue;

    Type(Buffer) temporaryBuffer = strdup ( inputText );
    temporaryBuffer [ strlen ( temporaryBuffer ) - 1 ] = '\0';

    Type(InputText) firstOccurrence = Call(skipUntilFirst)( Call(skipUntilFirst)( temporaryBuffer, "xX" ), Variable(base16Characters) );

    if ( firstOccurrence == NULL ) {
        Call(error)("PositiveLongLiteral : base = %d, modifiers = %s, value = %s", 16, "long", inputText);
        returnValue = 0x00L;
    } else {
        returnValue = strtoll ( firstOccurrence, NULL, 16 );
    }

    free ( temporaryBuffer );
    return returnValue;
}

Function(Type(SignedLongLiteral), negativeLongLiteral_Base16Long) ( Type(InputText) inputText ) {
    Type(SignedLongLiteral) returnValue;

    Type(Buffer) temporaryBuffer = strdup ( inputText );
    temporaryBuffer [ strlen ( temporaryBuffer ) - 1 ] = '\0';

    Type(InputText) firstOccurrence = Call(skipUntilFirst)( Call(skipUntilFirst) ( temporaryBuffer, "xX" ), Variable(base16Characters) );

    if ( firstOccurrence == NULL ) {
        Call(error)( "NegativeLongLiteral : base = %d, modifiers = %s, value = %s", 16, "long", inputText );
        returnValue = 0x00L;
    } else {
        returnValue = - strtoll ( firstOccurrence, NULL, 16 );
    }

    return returnValue;
}

Function(Type(SignedLongLiteral), positiveLongLiteral_Base16LongLong) ( Type(InputText) inputText ) {
    Type(SignedLongLiteral) returnValue;

    Type(Buffer) temporaryBuffer = strdup ( inputText );
    temporaryBuffer [ strlen ( temporaryBuffer ) - 2 ] = '\0';

    Type(InputText) firstOccurrence = Call(skipUntilFirst)( Call(skipUntilFirst)( temporaryBuffer, "xX" ), Variable(base16Characters) );

    if ( firstOccurrence == NULL ) {
        Call(error)("PositiveLongLiteral : base = %d, modifiers = %s, value = %s", 16, "long long", inputText);
        returnValue = 0x00LL;
    } else {
        returnValue = strtoll ( firstOccurrence, NULL, 16 );
    }

    free ( temporaryBuffer );
    return returnValue;
}

Function(Type(SignedLongLiteral), negativeLongLiteral_Base16LongLong) ( Type(InputText) inputText ) {
    Type(SignedLongLiteral) returnValue;

    Type(Buffer) temporaryBuffer = strdup ( inputText );
    temporaryBuffer [ strlen ( temporaryBuffer ) - 2 ] = '\0';

    Type(InputText) firstOccurrence = Call(skipUntilFirst)( Call(skipUntilFirst) ( temporaryBuffer, "xX" ), Variable(base16Characters) );

    if ( firstOccurrence == NULL ) {
        Call(error)( "NegativeLongLiteral : base = %d, modifiers = %s, value = %s", 16, "long long", inputText );
        returnValue = 0x00LL;
    } else {
        returnValue = - strtoll ( firstOccurrence, NULL, 16 );
    }

    return returnValue;
}

Function(Type(UnsignedLongLiteral), positiveLongLiteral_Base16Unsigned) ( Type(InputText) inputText ) {
    Type(UnsignedLongLiteral) returnValue;

    Type(Buffer) temporaryBuffer = strdup ( inputText );
    temporaryBuffer [ strlen ( temporaryBuffer ) - 1 ] = '\0';

    Type(InputText) firstOccurrence = Call(skipUntilFirst)( Call(skipUntilFirst)( temporaryBuffer, "xX" ), Variable(base16Characters) );

    if ( firstOccurrence == NULL ) {
        Call(error)("UnsignedLongLiteral : base = %d, modifiers = %s, value = %s", 16, "none", inputText);
        returnValue = 0x00U;
    } else {
        returnValue = strtoull ( firstOccurrence, NULL, 16 );
    }

    free ( temporaryBuffer );
    return returnValue;
}

Function(Type(UnsignedLongLiteral), positiveLongLiteral_Base16UnsignedLong) ( Type(InputText) inputText ) {
    Type(UnsignedLongLiteral) returnValue;

    Type(Buffer) temporaryBuffer = strdup ( inputText );
    temporaryBuffer [ strlen ( temporaryBuffer ) - 2 ] = '\0';

    Type(InputText) firstOccurrence = Call(skipUntilFirst)( Call(skipUntilFirst)( temporaryBuffer, "xX" ), Variable(base16Characters) );

    if ( firstOccurrence == NULL ) {
        Call(error)("UnsignedLongLiteral : base = %d, modifiers = %s, value = %s", 16, "long", inputText);
        returnValue = 0x00UL;
    } else {
        returnValue = strtoull ( firstOccurrence, NULL, 16 );
    }

    free ( temporaryBuffer );
    return returnValue;
}

Function(Type(UnsignedLongLiteral), positiveLongLiteral_Base16UnsignedLongLong) ( Type(InputText) inputText ) {
    Type(UnsignedLongLiteral) returnValue;

    Type(Buffer) temporaryBuffer = strdup ( inputText );
    temporaryBuffer [ strlen ( temporaryBuffer ) - 3 ] = '\0';

    Type(InputText) firstOccurrence = Call(skipUntilFirst)( Call(skipUntilFirst)( temporaryBuffer, "xX" ), Variable(base16Characters) );

    if ( firstOccurrence == NULL ) {
        Call(error)("UnsignedLongLiteral : base = %d, modifiers = %s, value = %s", 16, "long long", inputText);
        returnValue = 0x00ULL;
    } else {
        returnValue = strtoull ( firstOccurrence, NULL, 16 );
    }

    free ( temporaryBuffer );
    return returnValue;
}

Function(Type(UnsignedLongLiteral), positiveLongLiteral_Base16LongUnsigned) ( Type(InputText) inputText ) {
    Type(UnsignedLongLiteral) returnValue;

    Type(Buffer) temporaryBuffer = strdup ( inputText );
    temporaryBuffer [ strlen ( temporaryBuffer ) - 2 ] = '\0';

    Type(InputText) firstOccurrence = Call(skipUntilFirst)( Call(skipUntilFirst)( temporaryBuffer, "xX" ), Variable(base16Characters) );

    if ( firstOccurrence == NULL ) {
        Call(error)("UnsignedLongLiteral : base = %d, modifiers = %s, value = %s", 16, "long", inputText);
        returnValue = 0x00LU;
    } else {
        returnValue = strtoull ( firstOccurrence, NULL, 16 );
    }

    free ( temporaryBuffer );
    return returnValue;
}

Function(Type(UnsignedLongLiteral), positiveLongLiteral_Base16LongLongUnsigned) ( Type(InputText) inputText ) {
    Type(UnsignedLongLiteral) returnValue;

    Type(Buffer) temporaryBuffer = strdup ( inputText );
    temporaryBuffer [ strlen ( temporaryBuffer ) - 3 ] = '\0';

    Type(InputText) firstOccurrence = Call(skipUntilFirst)( Call(skipUntilFirst)( temporaryBuffer, "xX" ), Variable(base16Characters) );

    if ( firstOccurrence == NULL ) {
        Call(error)("UnsignedLongLiteral : base = %d, modifiers = %s, value = %s", 16, "long long", inputText);
        returnValue = 0x00LLU;
    } else {
        returnValue = strtoull ( firstOccurrence, NULL, 16 );
    }

    free ( temporaryBuffer );
    return returnValue;
}

Function(Type(SignedLongLiteral), positiveLongLiteral_Base10) ( Type(InputText) inputText ) {
    return strtol ( inputText, NULL, 10 );
}

Function(Type(SignedLongLiteral), negativeLongLiteral_Base10) ( Type(InputText) inputText ) {
    Type(SignedLongLiteral) returnValue;
    Type(InputText) firstOccurrence = Call(skipUntilFirst)( inputText, Variable(base10Characters) );

    if ( firstOccurrence == NULL ) {
        Call(error) ( "NegativeLongLiteral Error : base = %d, modifiers = %s, value = %s", 10, "none", inputText );
        returnValue = 0;
    } else {
        returnValue = - strtoll ( inputText, NULL, 10 );
    }

    return returnValue;
}

Function(Type(SignedLongLiteral), positiveLongLiteral_Base10Long) ( Type(InputText) inputText ) {
    Type(SignedLongLiteral) returnValue;
    Type(Buffer) temporaryBuffer = strdup ( inputText );

    temporaryBuffer [ strlen ( temporaryBuffer ) - 1 ] = '\0';

    returnValue = strtoll ( temporaryBuffer, NULL, 10 );

    free ( temporaryBuffer );
    return returnValue;
}

Function(Type(SignedLongLiteral), negativeLongLiteral_Base10Long) ( Type(InputText) inputText ) {
    Type(SignedLongLiteral) returnValue;

    Type(Buffer) temporaryBuffer = strdup (inputText);
    temporaryBuffer [ strlen ( temporaryBuffer ) - 1 ] = '\0';

    Type(InputText) firstOccurrence = Call(skipUntilFirst)( temporaryBuffer, Variable(base10Characters) );

    if ( firstOccurrence == NULL ) {
        Call(error) ( "NegativeLongLiteral Error : base = %d, modifiers = %s, value = %s", 10, "long", inputText );
        returnValue = 0l;
    } else {
        returnValue = - strtoll ( inputText, NULL, 10 );
    }

    free ( temporaryBuffer );
    return returnValue;
}

Function(Type(SignedLongLiteral), positiveLongLiteral_Base10LongLong) ( Type(InputText) inputText ) {
    Type(SignedLongLiteral) returnValue;
    Type(Buffer) temporaryBuffer = strdup ( inputText );

    temporaryBuffer [ strlen ( temporaryBuffer ) - 2 ] = '\0';

    returnValue = strtoll ( temporaryBuffer, NULL, 10 );

    free ( temporaryBuffer );
    return returnValue;
}

Function(Type(SignedLongLiteral), negativeLongLiteral_Base10LongLong) ( Type(InputText) inputText ) {
    Type(SignedLongLiteral) returnValue;

    Type(Buffer) temporaryBuffer = strdup (inputText);
    temporaryBuffer [ strlen ( temporaryBuffer ) - 2 ] = '\0';

    Type(InputText) firstOccurrence = Call(skipUntilFirst)( temporaryBuffer, Variable(base10Characters) );

    if ( firstOccurrence == NULL ) {
        Call(error) ( "NegativeLongLiteral Error : base = %d, modifiers = %s, value = %s", 10, "long long", inputText );
        returnValue = 0ll;
    } else {
        returnValue = - strtoll ( inputText, NULL, 10 );
    }

    free ( temporaryBuffer );
    return returnValue;
}

Function(Type(UnsignedLongLiteral), positiveLongLiteral_Base10Unsigned) ( Type(InputText) inputText ) {
    Type(UnsignedLongLiteral) returnValue;
    Type(Buffer) temporaryBuffer = strdup ( inputText );

    temporaryBuffer [ strlen ( temporaryBuffer ) - 1 ] = '\0';

    returnValue = strtoull ( temporaryBuffer, NULL, 10 );

    free ( temporaryBuffer );
    return returnValue;
}

Function(Type(UnsignedLongLiteral), positiveLongLiteral_Base10UnsignedLong) ( Type(InputText) inputText ) {
    Type(UnsignedLongLiteral) returnValue;
    Type(Buffer) temporaryBuffer = strdup ( inputText );

    temporaryBuffer [ strlen ( temporaryBuffer ) - 2 ] = '\0';

    returnValue = strtoull ( temporaryBuffer, NULL, 10 );

    free ( temporaryBuffer );
    return returnValue;
}

Function(Type(UnsignedLongLiteral), positiveLongLiteral_Base10UnsignedLongLong) ( Type(InputText) inputText ) {
    Type(UnsignedLongLiteral) returnValue;
    Type(Buffer) temporaryBuffer = strdup ( inputText );

    temporaryBuffer [ strlen ( temporaryBuffer ) - 3 ] = '\0';

    returnValue = strtoull ( temporaryBuffer, NULL, 10 );

    free ( temporaryBuffer );
    return returnValue;
}

Function(Type(UnsignedLongLiteral), positiveLongLiteral_Base10LongUnsigned) ( Type(InputText) inputText ) {
    Type(UnsignedLongLiteral) returnValue;
    Type(Buffer) temporaryBuffer = strdup ( inputText );

    temporaryBuffer [ strlen ( temporaryBuffer ) - 2 ] = '\0';

    returnValue = strtoull ( temporaryBuffer, NULL, 10 );

    free ( temporaryBuffer );
    return returnValue;
}

Function(Type(UnsignedLongLiteral), positiveLongLiteral_Base10LongLongUnsigned) ( Type(InputText) inputText ) {
    Type(UnsignedLongLiteral) returnValue;
    Type(Buffer) temporaryBuffer = strdup ( inputText );

    temporaryBuffer [ strlen ( temporaryBuffer ) - 3 ] = '\0';

    returnValue = strtoull ( temporaryBuffer, NULL, 10 );

    free ( temporaryBuffer );
    return returnValue;
}

Function(Type(SignedLongLiteral), positiveLongLiteral_Base8) ( Type(InputText) inputText ) {
    Type(SignedLongLiteral) returnValue;
    Type(InputText) firstOccurrence = Call(skipUntilFirst)(Call(skipUntilFirst)(inputText, "0") + 1, Variable(base8Characters));

    if ( firstOccurrence == NULL ) {
        Call(error)("PositiveLongLiteral error : base = %d, modifiers = %s, value = %s", 8, "none", inputText);
        returnValue = 00;
    } else {
        returnValue = strtoll ( firstOccurrence, NULL, 8 );
    }

    return returnValue;
}

Function(Type(SignedLongLiteral), negativeLongLiteral_Base8) ( Type(InputText) inputText ) {
    Type(SignedLongLiteral) returnValue;
    Type(InputText) firstOccurrence = Call(skipUntilFirst)(Call(skipUntilFirst)(inputText, "0") + 1, Variable(base8Characters));

    if ( firstOccurrence == NULL ) {
        Call(error)("NegativeLongLiteral error : base = %d, modifiers = %s, value = %s", 8, "none", inputText);
        returnValue = 00;
    } else {
        returnValue = - strtoll ( firstOccurrence, NULL, 8 );
    }

    return returnValue;
}

Function(Type(SignedLongLiteral), positiveLongLiteral_Base8Long) ( Type(InputText) inputText ) {
    Type(SignedLongLiteral) returnValue;

    Type(Buffer) temporaryBuffer = strdup ( inputText );
    temporaryBuffer [ strlen ( temporaryBuffer ) - 1 ] = '\0';

    Type(InputText) firstOccurrence = Call(skipUntilFirst)(Call(skipUntilFirst)(temporaryBuffer, "0") + 1, Variable(base8Characters));

    if ( firstOccurrence == NULL ) {
        Call(error)("PositiveLongLiteral error : base = %d, modifiers = %s, value = %s", 8, "long", inputText);
        returnValue = 00L;
    } else {
        returnValue = strtoll ( firstOccurrence, NULL, 8 );
    }

    free ( temporaryBuffer );

    return returnValue;
}

Function(Type(SignedLongLiteral), negativeLongLiteral_Base8Long) ( Type(InputText) inputText ) {
    Type(SignedLongLiteral) returnValue;

    Type(Buffer) temporaryBuffer = strdup ( inputText );
    temporaryBuffer [ strlen ( temporaryBuffer ) - 1 ] = '\0';

    Type(InputText) firstOccurrence = Call(skipUntilFirst)(Call(skipUntilFirst)(temporaryBuffer, "0") + 1, Variable(base8Characters));

    if ( firstOccurrence == NULL ) {
        Call(error)("NegativeLongLiteral error : base = %d, modifiers = %s, value = %s", 8, "long", inputText);
        returnValue = 00L;
    } else {
        returnValue = - strtoll ( firstOccurrence, NULL, 8 );
    }

    free ( temporaryBuffer );

    return returnValue;
}

Function(Type(SignedLongLiteral), positiveLongLiteral_Base8LongLong) ( Type(InputText) inputText ) {
    Type(SignedLongLiteral) returnValue;

    Type(Buffer) temporaryBuffer = strdup ( inputText );
    temporaryBuffer [ strlen ( temporaryBuffer ) - 2 ] = '\0';

    Type(InputText) firstOccurrence = Call(skipUntilFirst)(Call(skipUntilFirst)(temporaryBuffer, "0") + 1, Variable(base8Characters));

    if ( firstOccurrence == NULL ) {
        Call(error)("PositiveLongLiteral error : base = %d, modifiers = %s, value = %s", 8, "long long", inputText);
        returnValue = 00LL;
    } else {
        returnValue = strtoll ( firstOccurrence, NULL, 8 );
    }

    free ( temporaryBuffer );

    return returnValue;
}

Function(Type(SignedLongLiteral), negativeLongLiteral_Base8LongLong) ( Type(InputText) inputText ) {
    Type(SignedLongLiteral) returnValue;

    Type(Buffer) temporaryBuffer = strdup ( inputText );
    temporaryBuffer [ strlen ( temporaryBuffer ) - 2 ] = '\0';

    Type(InputText) firstOccurrence = Call(skipUntilFirst)(Call(skipUntilFirst)(temporaryBuffer, "0") + 1, Variable(base8Characters));

    if ( firstOccurrence == NULL ) {
        Call(error)("NegativeLongLiteral error : base = %d, modifiers = %s, value = %s", 8, "long long", inputText);
        returnValue = 00L;
    } else {
        returnValue = - strtoll ( firstOccurrence, NULL, 8 );
    }

    free ( temporaryBuffer );

    return returnValue;
}

Function(Type(UnsignedLongLiteral), positiveLongLiteral_Base8Unsigned) ( Type(InputText) inputText ) {
    Type(UnsignedLongLiteral) returnValue;

    Type(Buffer) temporaryBuffer = strdup ( inputText );
    temporaryBuffer [ strlen ( temporaryBuffer ) - 1 ] = '\0';

    Type(InputText) firstOccurrence = Call(skipUntilFirst)(Call(skipUntilFirst)(temporaryBuffer, "0") + 1, Variable(base8Characters));

    if ( firstOccurrence == NULL ) {
        Call(error)("UnsignedLongLiteral error : base = %d, modifiers = %s, value = %s", 8, "none", inputText);
        returnValue = 00U;
    } else {
        returnValue = strtoull ( firstOccurrence, NULL, 8 );
    }

    free ( temporaryBuffer );

    return returnValue;
}

Function(Type(UnsignedLongLiteral), positiveLongLiteral_Base8UnsignedLong) ( Type(InputText) inputText ) {
    Type(UnsignedLongLiteral) returnValue;

    Type(Buffer) temporaryBuffer = strdup ( inputText );
    temporaryBuffer [ strlen ( temporaryBuffer ) - 2 ] = '\0';

    Type(InputText) firstOccurrence = Call(skipUntilFirst)(Call(skipUntilFirst)(temporaryBuffer, "0") + 1, Variable(base8Characters));

    if ( firstOccurrence == NULL ) {
        Call(error)("UnsignedLongLiteral error : base = %d, modifiers = %s, value = %s", 8, "long", inputText);
        returnValue = 00UL;
    } else {
        returnValue = strtoull ( firstOccurrence, NULL, 8 );
    }

    free ( temporaryBuffer );

    return returnValue;
}

Function(Type(UnsignedLongLiteral), positiveLongLiteral_Base8UnsignedLongLong) ( Type(InputText) inputText ) {
    Type(UnsignedLongLiteral) returnValue;

    Type(Buffer) temporaryBuffer = strdup ( inputText );
    temporaryBuffer [ strlen ( temporaryBuffer ) - 3 ] = '\0';

    Type(InputText) firstOccurrence = Call(skipUntilFirst)(Call(skipUntilFirst)(temporaryBuffer, "0") + 1, Variable(base8Characters));

    if ( firstOccurrence == NULL ) {
        Call(error)("UnsignedLongLiteral error : base = %d, modifiers = %s, value = %s", 8, "long long", inputText);
        returnValue = 00ULL;
    } else {
        returnValue = strtoull ( firstOccurrence, NULL, 8 );
    }

    free ( temporaryBuffer );

    return returnValue;
}

Function(Type(UnsignedLongLiteral), positiveLongLiteral_Base8LongUnsigned) ( Type(InputText) inputText ) {
    Type(UnsignedLongLiteral) returnValue;

    Type(Buffer) temporaryBuffer = strdup ( inputText );
    temporaryBuffer [ strlen ( temporaryBuffer ) - 2 ] = '\0';

    Type(InputText) firstOccurrence = Call(skipUntilFirst)(Call(skipUntilFirst)(temporaryBuffer, "0") + 1, Variable(base8Characters));

    if ( firstOccurrence == NULL ) {
        Call(error)("UnsignedLongLiteral error : base = %d, modifiers = %s, value = %s", 8, "long", inputText);
        returnValue = 00LU;
    } else {
        returnValue = strtoull ( firstOccurrence, NULL, 8 );
    }

    free ( temporaryBuffer );

    return returnValue;
}

Function(Type(UnsignedLongLiteral), positiveLongLiteral_Base8LongLongUnsigned) ( Type(InputText) inputText ) {
    Type(UnsignedLongLiteral) returnValue;

    Type(Buffer) temporaryBuffer = strdup ( inputText );
    temporaryBuffer [ strlen ( temporaryBuffer ) - 3 ] = '\0';

    Type(InputText) firstOccurrence = Call(skipUntilFirst)(Call(skipUntilFirst)(temporaryBuffer, "0") + 1, Variable(base8Characters));

    if ( firstOccurrence == NULL ) {
        Call(error)("UnsignedLongLiteral error : base = %d, modifiers = %s, value = %s", 8, "long long", inputText);
        returnValue = 00LLU;
    } else {
        returnValue = strtoull ( firstOccurrence, NULL, 8 );
    }

    free ( temporaryBuffer );

    return returnValue;
}

Function(Type(SignedLongLiteral), positiveLongLiteral_Base2) ( Type(InputText) inputText ) {
    Type(SignedLongLiteral) returnValue;
    Type(InputText) firstOccurrence = Call(skipUntilFirst)(Call(skipUntilFirst)(inputText, "bB") + 1, Variable(base8Characters));

    if ( firstOccurrence == NULL ) {
        Call(error)("PositiveLongLiteral error : base = %d, modifiers = %s, value = %s", 2, "none", inputText);
        returnValue = 0b00;
    } else {
        returnValue = strtoll ( firstOccurrence, NULL, 2 );
    }

    return returnValue;
}

Function(Type(SignedLongLiteral), negativeLongLiteral_Base2) ( Type(InputText) inputText ) {
    Type(SignedLongLiteral) returnValue;
    Type(InputText) firstOccurrence = Call(skipUntilFirst)(Call(skipUntilFirst)(inputText, "bB") + 1, Variable(base8Characters));

    if ( firstOccurrence == NULL ) {
        Call(error)("NegativeLongLiteral error : base = %d, modifiers = %s, value = %s", 2, "none", inputText);
        returnValue = 0b00;
    } else {
        returnValue = - strtoll ( firstOccurrence, NULL, 2 );
    }

    return returnValue;
}

Function(Type(SignedLongLiteral), positiveLongLiteral_Base2Long) ( Type(InputText) inputText ) {
    Type(SignedLongLiteral) returnValue;

    Type(Buffer) temporaryBuffer = strdup ( inputText );
    temporaryBuffer [ strlen ( temporaryBuffer ) - 1 ] = '\0';

    Type(InputText) firstOccurrence = Call(skipUntilFirst)(Call(skipUntilFirst)(temporaryBuffer, "bB") + 1, Variable(base8Characters));

    if ( firstOccurrence == NULL ) {
        Call(error)("PositiveLongLiteral error : base = %d, modifiers = %s, value = %s", 2, "long", inputText);
        returnValue = 0b00L;
    } else {
        returnValue = strtoll ( firstOccurrence, NULL, 2 );
    }

    free ( temporaryBuffer );

    return returnValue;
}

Function(Type(SignedLongLiteral), negativeLongLiteral_Base2Long) ( Type(InputText) inputText ) {
    Type(SignedLongLiteral) returnValue;

    Type(Buffer) temporaryBuffer = strdup ( inputText );
    temporaryBuffer [ strlen ( temporaryBuffer ) - 1 ] = '\0';

    Type(InputText) firstOccurrence = Call(skipUntilFirst)(Call(skipUntilFirst)(temporaryBuffer, "bB") + 1, Variable(base8Characters));

    if ( firstOccurrence == NULL ) {
        Call(error)("NegativeLongLiteral error : base = %d, modifiers = %s, value = %s", 2, "long", inputText);
        returnValue = 0b00L;
    } else {
        returnValue = - strtoll ( firstOccurrence, NULL, 2 );
    }

    free ( temporaryBuffer );

    return returnValue;
}

Function(Type(SignedLongLiteral), positiveLongLiteral_Base2LongLong) ( Type(InputText) inputText ) {
    Type(SignedLongLiteral) returnValue;

    Type(Buffer) temporaryBuffer = strdup ( inputText );
    temporaryBuffer [ strlen ( temporaryBuffer ) - 2 ] = '\0';

    Type(InputText) firstOccurrence = Call(skipUntilFirst)(Call(skipUntilFirst)(temporaryBuffer, "bB") + 1, Variable(base8Characters));

    if ( firstOccurrence == NULL ) {
        Call(error)("PositiveLongLiteral error : base = %d, modifiers = %s, value = %s", 2, "long long", inputText);
        returnValue = 0b00LL;
    } else {
        returnValue = strtoll ( firstOccurrence, NULL, 2 );
    }

    free ( temporaryBuffer );

    return returnValue;
}

Function(Type(SignedLongLiteral), negativeLongLiteral_Base2LongLong) ( Type(InputText) inputText ) {
    Type(SignedLongLiteral) returnValue;

    Type(Buffer) temporaryBuffer = strdup ( inputText );
    temporaryBuffer [ strlen ( temporaryBuffer ) - 2 ] = '\0';

    Type(InputText) firstOccurrence = Call(skipUntilFirst)(Call(skipUntilFirst)(temporaryBuffer, "bB") + 1, Variable(base8Characters));

    if ( firstOccurrence == NULL ) {
        Call(error)("NegativeLongLiteral error : base = %d, modifiers = %s, value = %s", 2, "long long", inputText);
        returnValue = 0b00L;
    } else {
        returnValue = - strtoll ( firstOccurrence, NULL, 2 );
    }

    free ( temporaryBuffer );

    return returnValue;
}

Function(Type(UnsignedLongLiteral), positiveLongLiteral_Base2Unsigned) ( Type(InputText) inputText ) {
    Type(UnsignedLongLiteral) returnValue;

    Type(Buffer) temporaryBuffer = strdup ( inputText );
    temporaryBuffer [ strlen ( temporaryBuffer ) - 1 ] = '\0';

    Type(InputText) firstOccurrence = Call(skipUntilFirst)(Call(skipUntilFirst)(temporaryBuffer, "bB") + 1, Variable(base8Characters));

    if ( firstOccurrence == NULL ) {
        Call(error)("UnsignedLongLiteral error : base = %d, modifiers = %s, value = %s", 2, "none", inputText);
        returnValue = 0b00U;
    } else {
        returnValue = strtoull ( firstOccurrence, NULL, 2 );
    }

    free ( temporaryBuffer );

    return returnValue;
}

Function(Type(UnsignedLongLiteral), positiveLongLiteral_Base2UnsignedLong) ( Type(InputText) inputText ) {
    Type(UnsignedLongLiteral) returnValue;

    Type(Buffer) temporaryBuffer = strdup ( inputText );
    temporaryBuffer [ strlen ( temporaryBuffer ) - 2 ] = '\0';

    Type(InputText) firstOccurrence = Call(skipUntilFirst)(Call(skipUntilFirst)(temporaryBuffer, "bB") + 1, Variable(base8Characters));

    if ( firstOccurrence == NULL ) {
        Call(error)("UnsignedLongLiteral error : base = %d, modifiers = %s, value = %s", 2, "long", inputText);
        returnValue = 0b00UL;
    } else {
        returnValue = strtoull ( firstOccurrence, NULL, 2 );
    }

    free ( temporaryBuffer );

    return returnValue;
}

Function(Type(UnsignedLongLiteral), positiveLongLiteral_Base2UnsignedLongLong) ( Type(InputText) inputText ) {
    Type(UnsignedLongLiteral) returnValue;

    Type(Buffer) temporaryBuffer = strdup ( inputText );
    temporaryBuffer [ strlen ( temporaryBuffer ) - 3 ] = '\0';

    Type(InputText) firstOccurrence = Call(skipUntilFirst)(Call(skipUntilFirst)(temporaryBuffer, "bB") + 1, Variable(base8Characters));

    if ( firstOccurrence == NULL ) {
        Call(error)("UnsignedLongLiteral error : base = %d, modifiers = %s, value = %s", 2, "long long", inputText);
        returnValue = 0b00ULL;
    } else {
        returnValue = strtoull ( firstOccurrence, NULL, 2 );
    }

    free ( temporaryBuffer );

    return returnValue;
}

Function(Type(UnsignedLongLiteral), positiveLongLiteral_Base2LongUnsigned) ( Type(InputText) inputText ) {
    Type(UnsignedLongLiteral) returnValue;

    Type(Buffer) temporaryBuffer = strdup ( inputText );
    temporaryBuffer [ strlen ( temporaryBuffer ) - 2 ] = '\0';

    Type(InputText) firstOccurrence = Call(skipUntilFirst)(Call(skipUntilFirst)(temporaryBuffer, "bB") + 1, Variable(base8Characters));

    if ( firstOccurrence == NULL ) {
        Call(error)("UnsignedLongLiteral error : base = %d, modifiers = %s, value = %s", 2, "long", inputText);
        returnValue = 0b00LU;
    } else {
        returnValue = strtoull ( firstOccurrence, NULL, 2 );
    }

    free ( temporaryBuffer );

    return returnValue;
}

Function(Type(UnsignedLongLiteral), positiveLongLiteral_Base2LongLongUnsigned) ( Type(InputText) inputText ) {
    Type(UnsignedLongLiteral) returnValue;

    Type(Buffer) temporaryBuffer = strdup ( inputText );
    temporaryBuffer [ strlen ( temporaryBuffer ) - 3 ] = '\0';

    Type(InputText) firstOccurrence = Call(skipUntilFirst)(Call(skipUntilFirst)(temporaryBuffer, "bB") + 1, Variable(base8Characters));

    if ( firstOccurrence == NULL ) {
        Call(error)("UnsignedLongLiteral error : base = %d, modifiers = %s, value = %s", 2, "long long", inputText);
        returnValue = 0b00LLU;
    } else {
        returnValue = strtoull ( firstOccurrence, NULL, 2 );
    }

    free ( temporaryBuffer );

    return returnValue;
}




#define CPP_PARSER_C_OBJECT_STOP
#include "cDefs.h"
