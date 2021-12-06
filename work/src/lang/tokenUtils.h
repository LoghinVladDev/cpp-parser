#ifndef CPP_PARSER_TOKEN_UTILS_H_
#define CPP_PARSER_TOKEN_UTILS_H_

#define CPP_PARSER_C_OBJECT_START TokenUtils
#include "cDefs.h"



typedef char *                  Type(Buffer);
typedef char const *            Type(InputText);
typedef char const *            Type(Separators);

PublicFunction(Type(InputText), skipUntilFirst)            ( Type(InputText), Type(Separators) );


typedef char const *            Type(ErrorMessageFormat);

PublicFunction(void, error)                                ( Type(ErrorMessageFormat), ... );


PublicFunction(char, anyCharacter)                         ( Type(InputText) );


typedef char const *            Type(Token);

PublicFunction(Type(Token), token)                         ( Type(InputText) );


typedef char const *            Type(StringLiteral);

PublicFunction(Type(StringLiteral), stringLiteral)         ( Type(InputText) );


typedef char                    Type(CharacterLiteral);

PublicFunction(Type(CharacterLiteral), characterLiteral)   ( Type(InputText) );


typedef double                  Type(DoubleLiteral);

PublicFunction(Type(DoubleLiteral), positiveDoubleLiteral) ( Type(InputText) );
PublicFunction(Type(DoubleLiteral), negativeDoubleLiteral) ( Type(InputText) );


typedef float                   Type(FloatLiteral);

PublicFunction(Type(FloatLiteral), positiveFloatLiteral)   ( Type(InputText) );
PublicFunction(Type(FloatLiteral), negativeFloatLiteral)   ( Type(InputText) );


typedef signed long long int    Type(SignedLongLiteral);
typedef unsigned long long int  Type(UnsignedLongLiteral);

PublicFunction(Type(SignedLongLiteral), positiveLongLiteral_Base16) ( Type(InputText) );
PublicFunction(Type(SignedLongLiteral), negativeLongLiteral_Base16) ( Type(InputText) );

PublicFunction(Type(SignedLongLiteral), positiveLongLiteral_Base16Long) ( Type(InputText) );
PublicFunction(Type(SignedLongLiteral), negativeLongLiteral_Base16Long) ( Type(InputText) );

PublicFunction(Type(SignedLongLiteral), positiveLongLiteral_Base16LongLong) ( Type(InputText) );
PublicFunction(Type(SignedLongLiteral), negativeLongLiteral_Base16LongLong) ( Type(InputText) );

PublicFunction(Type(UnsignedLongLiteral), positiveLongLiteral_Base16Unsigned) ( Type(InputText) );
PublicFunction(Type(UnsignedLongLiteral), positiveLongLiteral_Base16UnsignedLong) ( Type(InputText) );
PublicFunction(Type(UnsignedLongLiteral), positiveLongLiteral_Base16UnsignedLongLong) ( Type(InputText) );

PublicFunction(Type(UnsignedLongLiteral), positiveLongLiteral_Base16LongUnsigned) ( Type(InputText) );
PublicFunction(Type(UnsignedLongLiteral), positiveLongLiteral_Base16LongLongUnsigned) ( Type(InputText) );


PublicFunction(Type(SignedLongLiteral), positiveLongLiteral_Base10) ( Type(InputText) );
PublicFunction(Type(SignedLongLiteral), negativeLongLiteral_Base10) ( Type(InputText) );

PublicFunction(Type(SignedLongLiteral), positiveLongLiteral_Base10Long) ( Type(InputText) );
PublicFunction(Type(SignedLongLiteral), negativeLongLiteral_Base10Long) ( Type(InputText) );

PublicFunction(Type(SignedLongLiteral), positiveLongLiteral_Base10LongLong) ( Type(InputText) );
PublicFunction(Type(SignedLongLiteral), negativeLongLiteral_Base10LongLong) ( Type(InputText) );

PublicFunction(Type(UnsignedLongLiteral), positiveLongLiteral_Base10Unsigned) ( Type(InputText) );
PublicFunction(Type(UnsignedLongLiteral), positiveLongLiteral_Base10UnsignedLong) ( Type(InputText) );
PublicFunction(Type(UnsignedLongLiteral), positiveLongLiteral_Base10UnsignedLongLong) ( Type(InputText) );

PublicFunction(Type(UnsignedLongLiteral), positiveLongLiteral_Base10LongUnsigned) ( Type(InputText) );
PublicFunction(Type(UnsignedLongLiteral), positiveLongLiteral_Base10LongLongUnsigned) ( Type(InputText) );


PublicFunction(Type(SignedLongLiteral), positiveLongLiteral_Base8) ( Type(InputText) );
PublicFunction(Type(SignedLongLiteral), negativeLongLiteral_Base8) ( Type(InputText) );

PublicFunction(Type(SignedLongLiteral), positiveLongLiteral_Base8Long) ( Type(InputText) );
PublicFunction(Type(SignedLongLiteral), negativeLongLiteral_Base8Long) ( Type(InputText) );

PublicFunction(Type(SignedLongLiteral), positiveLongLiteral_Base8LongLong) ( Type(InputText) );
PublicFunction(Type(SignedLongLiteral), negativeLongLiteral_Base8LongLong) ( Type(InputText) );

PublicFunction(Type(UnsignedLongLiteral), positiveLongLiteral_Base8Unsigned) ( Type(InputText) );
PublicFunction(Type(UnsignedLongLiteral), positiveLongLiteral_Base8UnsignedLong) ( Type(InputText) );
PublicFunction(Type(UnsignedLongLiteral), positiveLongLiteral_Base8UnsignedLongLong) ( Type(InputText) );

PublicFunction(Type(UnsignedLongLiteral), positiveLongLiteral_Base8LongUnsigned) ( Type(InputText) );
PublicFunction(Type(UnsignedLongLiteral), positiveLongLiteral_Base8LongLongUnsigned) ( Type(InputText) );


PublicFunction(Type(SignedLongLiteral), positiveLongLiteral_Base2) ( Type(InputText) );
PublicFunction(Type(SignedLongLiteral), negativeLongLiteral_Base2) ( Type(InputText) );

PublicFunction(Type(SignedLongLiteral), positiveLongLiteral_Base2Long) ( Type(InputText) );
PublicFunction(Type(SignedLongLiteral), negativeLongLiteral_Base2Long) ( Type(InputText) );

PublicFunction(Type(SignedLongLiteral), positiveLongLiteral_Base2LongLong) ( Type(InputText) );
PublicFunction(Type(SignedLongLiteral), negativeLongLiteral_Base2LongLong) ( Type(InputText) );

PublicFunction(Type(UnsignedLongLiteral), positiveLongLiteral_Base2Unsigned) ( Type(InputText) );
PublicFunction(Type(UnsignedLongLiteral), positiveLongLiteral_Base2UnsignedLong) ( Type(InputText) );
PublicFunction(Type(UnsignedLongLiteral), positiveLongLiteral_Base2UnsignedLongLong) ( Type(InputText) );

PublicFunction(Type(UnsignedLongLiteral), positiveLongLiteral_Base2LongUnsigned) ( Type(InputText) );
PublicFunction(Type(UnsignedLongLiteral), positiveLongLiteral_Base2LongLongUnsigned) ( Type(InputText) );



#define CPP_PARSER_C_OBJECT_STOP
#include "cDefs.h"


#endif //CPP_PARSER_TOKEN_UTILS_H_