#ifndef LEXOR_H
#define LEXOR_H
#include <iostream>
#include <string>
#include <unordered_map>

//ENUM that lists all of the possible types that a token can be.
enum type{
	ID,
	INTNUM,
	FLOATNUM,
	EQ,
	PLUS,
	OR,
	OPENPAR,
	SEMI,
	INT,
	WHILE,
	LOCAL,
	NOTEQ,
	MINUS,
	AND,
	CLOSEPAR,
	COMMA,
	FLOAT,
	IF,
	CONSTRUCTOR,
	LT,
	MULT,
	NOT,
	OPENCUBR,
	DOT,
	VOID,
	THEN,
	ATTRIBUTE,
	GT,
	DIV,
	CLOSECUBR,
	COLON,
	CLASS,
	ELSE,
	FUNCTION,
	LEQ,
	ASSIGN,
	OPENSQBR,
	ARROW,
	SELF,
	READ,
	PUBLIC,
	GEQ,
	CLOSESQBR,
	ISA,
	WRITE,
	PRIVATE,
	IMPLEMENTATION,
	RETURN,
	INLINECMT,
	BLOCKCMT
};

//Lexor class
class lexor{
private :
	//Token map will be initialized with the buildTokenMap function.
	//Will contain all of the mappings between the lexeme and the type. Except for intnum, floatnum, id, blockcmt since they need a more complex analysis.

public:
	static const std::unordered_map<std::string,std::string> tokenMap;
	lexor(){};

};

class token{};


#endif
