#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>

class parser{

private:
	//unordered map for the first set
	//Key will be the name of the nonterminal symbol
	//The value will be a vector containing the actual set.
	static std::unordered_map<std::string, std::vector<std::string>*> firstSet;
	static std::unordered_map<std::string, std::vector<std::string>*> followSet;
	static std::ifstream inputFileStream;
	static bool virgin;
	static std::string currentSymbol;
	static std::string currentWord;
	static std::string line;
	static int lineIndex;

	//Function to establish connection to the file to read.
	static bool connectFile(std::string);
	//Function to cut the connection to the file to read.
	static bool disconnectFile();
	//A boolean to check if this is the first time reading the txt file.

	//Function that runs the first time that we read from a text file. This should only be called a single time in the entire program.
	bool virginProtocol();

	//Function that moves the input stream forward if it reads white space. white space values are 9-10-11-12-13-32
	void getRidOfWhiteSpace();

	void generateFirstSet();
	//Finds the first non terminal symbol which will the symbol we are generating a first set for
	std::vector<std::string> * findNT();
	//Updates the currentCharacter
	bool checkForAssignment();
	//Checks if it is a terminal.
	bool checkT();
	bool checkNT();
	bool checkE();
	//Function that checks if the current symbol is in the vector that is provided as an argument
	bool inVector(std::vector<std::string> * v);
	void getSymbol(std::string t);

};

#endif
