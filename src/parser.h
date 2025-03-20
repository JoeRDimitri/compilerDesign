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

class abstractSyntaxTree{

public:
	class node{
	public:
		node * headOfSibling;
		node * leftSibling;
		node * rightSibling;
		node * parent;
		std::vector<node *> children;
		std::string nodeType;
		std::string semanticMeaning;
		std::string nodeValue;

		void copyNode(node* oldnode,node* newnode, std::string nameOfNewNode);

		node(){};
		node(std::string type,std::string semanticMeaning) : headOfSibling(nullptr),leftSibling(nullptr),rightSibling(nullptr),children(),parent(nullptr),nodeType(type),semanticMeaning(semanticMeaning){}
		node(std::string type,std::string semanticMeaning,std::string lastTokenValue) : headOfSibling(nullptr),leftSibling(nullptr),rightSibling(nullptr),children(),parent(nullptr),nodeType(type),semanticMeaning(semanticMeaning),nodeValue(lastTokenValue){}
		node(node * n, std::string type) : headOfSibling(n->headOfSibling),leftSibling(n->leftSibling),rightSibling(n->rightSibling),children(n->children),parent(n->parent),nodeType(type),semanticMeaning(n->semanticMeaning){}
		node(node * n, std::string type,std::string value) : headOfSibling(n->headOfSibling),leftSibling(n->leftSibling),rightSibling(n->rightSibling),children(n->children),parent(n->parent),nodeType(type),semanticMeaning(value){}
		node(std::vector<node*> v,std::string type) : headOfSibling(nullptr),leftSibling(nullptr),rightSibling(nullptr),children(v),parent(nullptr),nodeType(type),semanticMeaning(type){}
		node(std::vector<node*> v,std::string type,std::string value) : headOfSibling(nullptr),leftSibling(nullptr),rightSibling(nullptr),children(v),parent(nullptr),nodeType(type),semanticMeaning(value){}
	};
	class paramNode : public node{

	};
	class startNode : public node{

	};
	class intNode : public node{

	};
	class floatNode : public node{

	};
	class epsilonNode  : public node{

	};
	class aparamsNode  : public node{

	};
	class aparamstailNode  : public node{

	};
	class addopNode : public node{

	};
	class arithexprNode  : public node{

	};
	class arraysizeNode  : public node{

	};
	class assignNode  : public node{

	};
	class attributedeclNode  : public node{

	};
	class idNode  : public node{

	};
	class reptclassdecl4Node  : public node{

	};
	class classdeclNode  : public node{

	};
	class classNode  : public node{

	};
	class implNode  : public node{

	};
	class funcNode  : public node{

	};
	class exprNode  : public node{

	};
	class expr2Node  : public node{

	};
	class relopNode  : public node{

	};
	class reptfparams3Node  : public node{

	};
	class reptfparams4Node  : public node{

	};
	class fparamsNode  : public node{

	};
	class factorNode  : public node{

	};
	class factor2Node  : public node{

	};
	class selfNode  : public node{

	};
	class selfandidnest2Node  : public node{

	};
	class dotNode  : public node{

	};
	class idnest2Node  : public node{

	};
	class reptvariable2Node  : public node{

	};
	class aparamsandidNode  : public node{

	};
	class reptvariable2andidNode  : public node{

	};
	class reptfuncbody1Node  : public node{

	};
	class funcbodyNode  : public node{

	};
	class funcdeclNode  : public node{

	};
	class funcdefNode  : public node{

	};
	class funcheadNode  : public node{

	};
	class reptimpldef3Node  : public node{

	};
	class impldefNode  : public node{

	};
	class indiceNode  : public node{

	};
	class localvardeclNode  : public node{

	};
	class localvardeclorstatNode  : public node{

	};
	class multopNode  : public node{

	};
	class reptoptclassdecl22Node  : public node{

	};
	class optclassdecl2Node  : public node{

	};
	class reptprog0Node  : public node{

	};
	class progNode  : public node{

	};
	class relexprNode  : public node{

	};
	class funcdeclfamNode  : public node{

	};
	class attributedeclfamNode  : public node{

	};
	class voidNode  : public node{

	};
	class returntypeNode  : public node{

	};
	class rightrectermNode  : public node{

	};
	class signNode  : public node{

	};
	class reptstatblock1Node  : public node{

	};
	class statblockNode  : public node{

	};
	class ifstatementNode  : public node{

	};
	class ifNode  : public node{

	};
	class conditionNode  : public node{

	};
	class thenNode  : public node{

	};
	class felseNode  : public node{

	};
	class whilestatementNode  : public node{

	};
	class whileNode  : public node{

	};
	class readNode  : public node{

	};
	class readstatementNode  : public node{

	};
	class writeNode  : public node{

	};
	class writestatementNode  : public node{

	};
	class returnNode  : public node{

	};
	class freturnstatementNode  : public node{

	};
	class reptstatement4Node  : public node{

	};
	class termNode  : public node{

	};
	class typeNode  : public node{

	};
	class vardeclNode  : public node{

	};
	class visibilityNode  : public node{

	};
	class reptvardecl3Node  : public node{

	};
	class rightrecarithexprNode  : public node{

	};
	class floatnumNode  : public node{

	};


	node * treeHead;
	void printTree();
	void traverseTree(abstractSyntaxTree::node* head, int& counter,std::ofstream & ao);

};

class semanticActions{
public:
	void makeLeaf(std::string nodeType,std::string lastTokenValue,abstractSyntaxTree::node* newnode);
	void handleAction(std::string semanticAction,std::string lastTokenValue);
	void makeSubTree(std::string nodeType,std::string semanticMeaning, int amountOfPops,std::vector<std::string> v,abstractSyntaxTree::node * newnode);
	void passAlong(std::string nodeType,std::string semanticMeaning,abstractSyntaxTree::node * newnode);
	void passAlong(std::string nodeType,abstractSyntaxTree::node * newnode);
	void adoptChildren(abstractSyntaxTree:: node* newparent,abstractSyntaxTree:: node* oldparent);
	void makeBinarySubTree(std::string nodeType,int i,abstractSyntaxTree::node*newnode);
	void makeBinarySubTreeWithHead(std::string nodeType, int numOfPops,abstractSyntaxTree::node* newnode);



};

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
	static semanticActions semanticHandler;
	static std::stack<abstractSyntaxTree::node *> semanticStack;
	static abstractSyntaxTree AST;

	bool searchFirst(std::string lookahead, std::string topOfTheStack);
	bool parse(const  std::vector<token*> &);
	void inverseRHSMultiplePush(tableEntry t,std::vector<std::string>& vec,const int & lineIndex);
	void skipError(token * & currentToken,std::vector<token*>::const_iterator& vectorIterator,std::ofstream &,const std::string & lexeme,const std::string & topOfTheStack,const int & line, const int & column);
	bool search(const std::string & lexeme, const std::string & topOfTheStack);

		parser(){};
		~parser(){}

};

#endif
