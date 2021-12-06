#ifndef CPP_PARSER_TOKEN_UTILS_H_
#define CPP_PARSER_TOKEN_UTILS_H_

#define CPP_PARSER_C_OBJECT_START TokenUtils
#include "cDefs.h"



typedef char *                  Type(Buffer);
typedef char const *            Type(InputText);
typedef char const *            Type(Separators);

extern Function(Type(InputText), skipUntilFirst)            ( Type(InputText), Type(Separators) );


typedef char const *            Type(ErrorMessageFormat);

extern Function(void, error)                                ( Type(ErrorMessageFormat), ... );


extern Function(char, anyCharacter)                         ( Type(InputText) );


typedef char const *            Type(Token);

extern Function(Type(Token), token)                         ( Type(InputText) );


typedef char const *            Type(StringLiteral);

extern Function(Type(StringLiteral), stringLiteral)         ( Type(InputText) );


typedef char                    Type(CharacterLiteral);

extern Function(Type(CharacterLiteral), characterLiteral)   ( Type(InputText) );


typedef double                  Type(DoubleLiteral);

extern Function(Type(DoubleLiteral), positiveDoubleLiteral) ( Type(InputText) );
extern Function(Type(DoubleLiteral), negativeDoubleLiteral) ( Type(InputText) );


typedef float                   Type(FloatLiteral);

extern Function(Type(FloatLiteral), positiveFloatLiteral)   ( Type(InputText) );
extern Function(Type(FloatLiteral), negativeFloatLiteral)   ( Type(InputText) );


typedef signed long long int    Type(SignedLongLiteral);
typedef unsigned long long int  Type(UnsignedLongLiteral);

extern Function(Type(SignedLongLiteral), positiveLongLiteral_Base16) ( Type(InputText) );
extern Function(Type(SignedLongLiteral), negativeLongLiteral_Base16) ( Type(InputText) );

extern Function(Type(SignedLongLiteral), positiveLongLiteral_Base16Long) ( Type(InputText) );
extern Function(Type(SignedLongLiteral), negativeLongLiteral_Base16Long) ( Type(InputText) );

extern Function(Type(SignedLongLiteral), positiveLongLiteral_Base16LongLong) ( Type(InputText) );
extern Function(Type(SignedLongLiteral), negativeLongLiteral_Base16LongLong) ( Type(InputText) );

extern Function(Type(UnsignedLongLiteral), positiveLongLiteral_Base16Unsigned) ( Type(InputText) );
extern Function(Type(UnsignedLongLiteral), positiveLongLiteral_Base16UnsignedLong) ( Type(InputText) );
extern Function(Type(UnsignedLongLiteral), positiveLongLiteral_Base16UnsignedLongLong) ( Type(InputText) );

extern Function(Type(UnsignedLongLiteral), positiveLongLiteral_Base16LongUnsigned) ( Type(InputText) );
extern Function(Type(UnsignedLongLiteral), positiveLongLiteral_Base16LongLongUnsigned) ( Type(InputText) );


extern Function(Type(SignedLongLiteral), positiveLongLiteral_Base10) ( Type(InputText) );
extern Function(Type(SignedLongLiteral), negativeLongLiteral_Base10) ( Type(InputText) );

extern Function(Type(SignedLongLiteral), positiveLongLiteral_Base10Long) ( Type(InputText) );
extern Function(Type(SignedLongLiteral), negativeLongLiteral_Base10Long) ( Type(InputText) );

extern Function(Type(SignedLongLiteral), positiveLongLiteral_Base10LongLong) ( Type(InputText) );
extern Function(Type(SignedLongLiteral), negativeLongLiteral_Base10LongLong) ( Type(InputText) );

extern Function(Type(UnsignedLongLiteral), positiveLongLiteral_Base10Unsigned) ( Type(InputText) );
extern Function(Type(UnsignedLongLiteral), positiveLongLiteral_Base10UnsignedLong) ( Type(InputText) );
extern Function(Type(UnsignedLongLiteral), positiveLongLiteral_Base10UnsignedLongLong) ( Type(InputText) );

extern Function(Type(UnsignedLongLiteral), positiveLongLiteral_Base10LongUnsigned) ( Type(InputText) );
extern Function(Type(UnsignedLongLiteral), positiveLongLiteral_Base10LongLongUnsigned) ( Type(InputText) );


extern Function(Type(SignedLongLiteral), positiveLongLiteral_Base8) ( Type(InputText) );
extern Function(Type(SignedLongLiteral), negativeLongLiteral_Base8) ( Type(InputText) );

extern Function(Type(SignedLongLiteral), positiveLongLiteral_Base8Long) ( Type(InputText) );
extern Function(Type(SignedLongLiteral), negativeLongLiteral_Base8Long) ( Type(InputText) );

extern Function(Type(SignedLongLiteral), positiveLongLiteral_Base8LongLong) ( Type(InputText) );
extern Function(Type(SignedLongLiteral), negativeLongLiteral_Base8LongLong) ( Type(InputText) );

extern Function(Type(UnsignedLongLiteral), positiveLongLiteral_Base8Unsigned) ( Type(InputText) );
extern Function(Type(UnsignedLongLiteral), positiveLongLiteral_Base8UnsignedLong) ( Type(InputText) );
extern Function(Type(UnsignedLongLiteral), positiveLongLiteral_Base8UnsignedLongLong) ( Type(InputText) );

extern Function(Type(UnsignedLongLiteral), positiveLongLiteral_Base8LongUnsigned) ( Type(InputText) );
extern Function(Type(UnsignedLongLiteral), positiveLongLiteral_Base8LongLongUnsigned) ( Type(InputText) );


extern Function(Type(SignedLongLiteral), positiveLongLiteral_Base2) ( Type(InputText) );
extern Function(Type(SignedLongLiteral), negativeLongLiteral_Base2) ( Type(InputText) );

extern Function(Type(SignedLongLiteral), positiveLongLiteral_Base2Long) ( Type(InputText) );
extern Function(Type(SignedLongLiteral), negativeLongLiteral_Base2Long) ( Type(InputText) );

extern Function(Type(SignedLongLiteral), positiveLongLiteral_Base2LongLong) ( Type(InputText) );
extern Function(Type(SignedLongLiteral), negativeLongLiteral_Base2LongLong) ( Type(InputText) );

extern Function(Type(UnsignedLongLiteral), positiveLongLiteral_Base2Unsigned) ( Type(InputText) );
extern Function(Type(UnsignedLongLiteral), positiveLongLiteral_Base2UnsignedLong) ( Type(InputText) );
extern Function(Type(UnsignedLongLiteral), positiveLongLiteral_Base2UnsignedLongLong) ( Type(InputText) );

extern Function(Type(UnsignedLongLiteral), positiveLongLiteral_Base2LongUnsigned) ( Type(InputText) );
extern Function(Type(UnsignedLongLiteral), positiveLongLiteral_Base2LongLongUnsigned) ( Type(InputText) );



#define CPP_PARSER_C_OBJECT_STOP
#include "cDefs.h"


#endif //CPP_PARSER_TOKEN_UTILS_H_