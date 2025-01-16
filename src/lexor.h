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
	//Token map will be initialized with the buildTokenMap function.
	//Will contain all of the mappings between the lexeme and the type. Except for intnum, floatnum, id, blockcmt since they need a more complex analysis.
	static const std::unordered_map<std::string,std::string> tokenMap;
	//int array will hold the string of each int from 0 to 9
	static const std::string intArray [10];
	//non Zero Array will hold the string of each int from 1 to 9 (excluding 0)
	static const std::string nonZeroArray [9];
	//inputFileStream to read the file.
	static std::ifstream inputFileStream;


public:
	//Constructor
	lexor(){};
	//Destructor
	~lexor(){};

	//Function to establish connection to the file to read.
	static bool connectFile(std::string);
	//Function to cut the connection to the file to read.
	static bool disconnectFile();
	//Function to check if the lexeme is a reserved word by looking into tokenMap.
	//Returns true if reserved word is found and false if none are found.
	bool isReservedWord(std::string lexeme,std::string &);


};

//A token is defined as a data structure that has a lexeme, a type, and a location (line + column)
class token{
private:
	//Type name is either a reserved word, an intnum, a floatnum, an id, inline comment or a block comment.
	std::string typeName;
	//Lexeme is the actual characters that will be defined
	std::string lexeme;
	//line is the line that the lexeme is found on in the file.
	int line;
	//Column is the column that that the lexeme is found on the line in the file.
	int column;

public:
	//Constructor
	token();
	token(std::string type, std::string lexeme, int line, int column);
	//Destructor
	~token();

	//Getters
	std::string getTypeName(){return this->typeName;};
	std::string getLexeme(){return this->lexeme;};
	int getLine(){return this->line;};
	int getColumn(){return this->column;};

	//Setters
	void setTypeName(const std::string s){this->typeName = s;};
	void setLexeme(const std::string s){this->lexeme = s;};
	void setLine(const int i){this->line = i;};
	void setColumn(const int i){this->column = i;};
};


#endif
