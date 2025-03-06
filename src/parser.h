#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <unordered_set>
#include <stack>
#include "lexor.h";
#include <deque>

class parser{
	struct tableEntry{
		int row;
		int column;
		std::string derivationRule;
	};

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
	bool static inVector(std::vector<std::string> * v, const std::string s = currentSymbol);

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

class parsing_table{

	 int static TABLEROWSIZE;
	 int static TABLECOLUMNSIZE;
	 int static UNIT;
	 static std::ifstream inputFileStream;



	//Function to establish connection to the file to read.
	static bool connectFile(std::string);
	//Function to cut the connection to the file to read.
	static bool disconnectFile();

	void setUniqueTerminalSymbols();
	int getTableRowSize();
	int getTableColumnSize();
	void setRowAndColumnSize();
	void setTheTableSize();
	void setTableRow();
	void setTableColumn();
	void createParsingTable();
	void getRule(std::string & rule, const std::string line, int & lineIndex);
	void getAlpha(std::string & alpha, const std::string line, int & lineIndex);
	bool checkFirstOfAlpha(const std::string alpha,std::unordered_set <std::string> & firstOfAlpha,bool & isTerminal);
	bool hasEpsilon(std::unordered_set <std::string> & firstOfAlpha, bool isTerminal);
	void getFollowOfRule(std::unordered_set <std::string> & followOfRule,const std::string rule);
	void updateTable(const std::string rule, std::unordered_set <std::string>  firstOfAlpha,std::string line);
	void fillTableWithErrors();

public:

	void buildTable();
	//Im going to use a 2d array and stor tableInfo structs.
	tableEntry ** table;
	//A set is used in order to have unique values of terminal symbols
    static std::unordered_set<std::string> uniqueTerminalSymbols;

	static std::unordered_map<std::string, int> tableRows;
	static std::unordered_map<std::string, int> tableColumns;  // Ensure values are ints, not references

	parsing_table(){};
	~parsing_table(){
		for (int i = 0; i < TABLEROWSIZE; ++i) {
			delete[] table[i];  // Free each row
		}
		delete[] table;  // Free row pointers};
	};

	std::unordered_set <std::string> getUniqueTerminalSymbols(){return this -> uniqueTerminalSymbols;}
};


public:
	static first_and_follow faf;
	static parsing_table parsingTable;
	static std::stack<std::string> parsingStack;

	bool searchFirst(std::string lookahead, std::string topOfTheStack);
	bool parse(const  std::vector<token*> &);
	void inverseRHSMultiplePush(tableEntry t,std::vector<std::string>& vec,const int & lineIndex);
	void skipError(token * & currentToken,std::vector<token*>::const_iterator& vectorIterator,std::ofstream &,const std::string & lexeme,const std::string & topOfTheStack,const int & line, const int & column);
	bool search(const std::string & lexeme, const std::string & topOfTheStack);

		parser(){};
		~parser(){}

};

#endif
