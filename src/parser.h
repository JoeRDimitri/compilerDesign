#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>

class parser{

class first_and_follow{
private:
	//unordered map for the first set
	//Key will be the name of the nonterminal symbol
	//The value will be a vector containing the actual set.
	static std::ifstream inputFileStream;
	static bool virgin;
	static std::string currentSymbol;
	static std::string currentWord;
	static std::string line;
	static int lineIndex;
	static bool change;

	//Function to establish connection to the file to read.
	static bool connectFile(std::string);
	//Function to cut the connection to the file to read.
	static bool disconnectFile();
	//A boolean to check if this is the first time reading the txt file.

	//Function that runs the first time that we read from a text file. This should only be called a single time in the entire program.
	bool virginProtocol();

	//Function that moves the input stream forward if it reads white space. white space values are 9-10-11-12-13-32
	void getRidOfWhiteSpace(int & templineIndex = lineIndex);

	//Finds the first non terminal symbol which will the symbol we are generating a first set for
	std::vector<std::string> * findNT(int k);
	//Updates the currentCharacter
	bool checkForAssignment();
	//Checks if it is a terminal.
	bool checkT(int & tempLineIndex = lineIndex);
	bool checkNT(int & tempLineIndex = lineIndex);
	bool checkE();
	//Function that checks if the current symbol is in the vector that is provided as an argument
	bool inVector(std::vector<std::string> * v, const std::string s = currentSymbol);
	void getSymbol(std::string t,int & tempLineIndex = lineIndex);
	void addAndMove(bool & change, std::vector<std::string>* v, std::string s = currentSymbol);
	void compareSymbolVector(bool & change, std::vector<std::string>* v);
	void checkTheNextSymbol(bool& eps, std::vector<std::string>*v);
	void compareSymbolVectorRec(bool & eps, std::vector<std::string> *v);

	void getToAssignmentOp();
	void getFollowSetSymbol();
	void movePointer(int & templineIndex);
	std::vector<std::string> * findFollowSet(std::string);
	void compareAndAdd(std::vector<std::string>* v1, std::vector<std::string>* v2);
	bool hasEpsilon(std::vector<std::string> * v);
public:
	static std::unordered_map<std::string, std::vector<std::string>*> firstSet;
	static std::unordered_map<std::string, std::vector<std::string>*> followSet;

	void generateFirstSet();
	void generateFollowSet();

	first_and_follow(){};
	~first_and_follow(){
		for (auto& pair : firstSet) {
		        delete pair.second;  // Deallocate the vector pointer
		    }
		for (auto& pair : followSet) {
		        delete pair.second;  // Deallocate the vector pointer
		    }
	}
};


public:
	static first_and_follow faf;


		parser(){};
		~parser(){}

};

#endif
