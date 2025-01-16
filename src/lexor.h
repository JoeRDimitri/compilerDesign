#ifndef LEXOR_H
#define LEXOR_H
#include <iostream>
#include <string>
#include <unordered_map>
#include <fstream>

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
private:
	static const std::unordered_map<std::string,std::string> tokenMap;
	static const std::string charArray [10];
	static const std::string nonZeroArray [9];
	static std::ifstream inputFileStream;

	//Token map will be initialized with the buildTokenMap function.
	//Will contain all of the mappings between the lexeme and the type. Except for intnum, floatnum, id, blockcmt since they need a more complex analysis.

public:
	lexor(){};
	~lexor(){};

	static bool connectFile(std::string);
	static bool disconnectFile();
	bool isReservedWord(std::string lexeme,std::string &);


};

class token{
private:
	std::string typeName;
	std::string lexeme;
	int line;
	int column;

public:
	token();
	token(std::string type, std::string lexeme, int line, int column);
	~token();

	std::string getTypeName(){return this->typeName;};
	std::string getLexeme(){return this->lexeme;};
	int getLine(){return this->line;};
	int getColumn(){return this->column;};

	void setTypeName(const std::string s){this->typeName = s;};
	void setLexeme(const std::string s){this->lexeme = s;};
	void setLine(const int i){this->line = i;};
	void setColumn(const int i){this->column = i;};
};


#endif
