#ifdef CPP_PARSER_C_OBJECT_STOP

#undef Type
#undef Function
#undef Call
#undef Token
#undef PublicFunction
#undef PrivateFunction
#undef Variable
#undef VariableDeclaration
#undef PublicVariable
#undef PrivateVariable

#undef _Token
#undef _Function

#undef __Function
#undef __Token

#undef CPP_PARSER_C_OBJECT_START
#undef CPP_PARSER_C_OBJECT_STOP

#else 

#ifdef CPP_PARSER_C_OBJECT_START

#define __Token(_className, _name) _className ## _ ## _name
#define __Function(_className, _type, _name) _type _className ## _ ## _name

#define _Token(_className, _name) __Token(_className, _name)
#define _Function(_className, _type, _name) __Function(_className, _type, _name)

#define Token(_type) _Token(CPP_PARSER_C_OBJECT_START, _type)

#define Function(_type, _name) _Function(CPP_PARSER_C_OBJECT_START, _type, _name)
#define PublicFunction(_type, _name) extern Function(_type, _name)
#define PrivateFunction(_type, _name) static Function(_type, _name)

#define Type(_type) Token(_type)
#define Call(_name) Token(_name)

#define Variable(_name) Token(_name)
#define VariableDeclaration(_type, _name) _type Token(_name)
#define PublicVariable(_type, _name) extern VariableDeclaration(_type, _name)
#define PrivateVariable(_type, _name) static VariableDeclaration(_type, _name)

#else

#error No object name defined, cannot create macros

#endif

#endif