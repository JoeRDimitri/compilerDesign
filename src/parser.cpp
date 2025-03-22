#include "parser.h"

parser::first_and_follow parser::faf;
parser::parsing_table parser::parsingTable;
std::unordered_map<std::string, std::vector<std::string>*> parser::first_and_follow::firstSet;
std::unordered_map<std::string, std::vector<std::string>*> parser::first_and_follow::followSet;
std::ifstream parser::first_and_follow::inputFileStream;
std::string parser::first_and_follow::currentWord;
std::string parser::first_and_follow::currentSymbol;
std::string parser::first_and_follow::line;
bool parser::first_and_follow::virgin = true;
bool parser::first_and_follow::change = true;
int parser::first_and_follow::lineIndex;


bool parser::first_and_follow::connectFile(std::string fileName){

	if(inputFileStream.is_open()){
			std::cout<<"The file is connected... Closing it first..."<<std::endl;
			inputFileStream.close();
		}

	inputFileStream.open(fileName,std::ios::in);

	if(inputFileStream){
		std::cout<<"Successfully opened file: "<<fileName<<"."<<std::endl;
		return true;
	}
	else{
		std::cout<<"Failed to open file: "<<fileName<<"."<<std::endl;
		return false;
	}
}

bool parser::first_and_follow::disconnectFile(){

	virgin = true;
	if(!inputFileStream.is_open()){
			std::cout<<"The file is not even connected. No need to close."<<std::endl;
			return true;
		}
	inputFileStream.close();

	if(!inputFileStream.is_open()){
		std::cout<<"Successfully closed the inputFileStream."<<std::endl;
		return true;
	}
	else{
		std::cout<<"Failed to close the inputFileStream."<<std::endl;
		return false;
	}
}

bool parser::first_and_follow::virginProtocol(){
	//Get the file location from the user since it is the first time he is using it given we are in the virgin protocol().
	std::string fileLocation;
	std::cout<<"File Location:";
	std::cin>>fileLocation;
	//File Location acquired, check if we can properly connect.
	if(connectFile("/home/giusuppe/eclipse-workspace/compilerDesign/Assignment2.COMP442-6421.paquet.2025.4/assignment2.COMP442-6421.paquet.2025.4 NEW/grammars/noLeftRec.grm")){
		//We've properly connected to the file now we need to get rid of the white space and then hand control back over getNextToken().
		return true;
	}
	else return false;
}

void parser::first_and_follow::getRidOfWhiteSpace(int&tempLineIndex){
	//Function that moves the input stream forward if it reads white space. white space values are 9-10-11-12-13-32
	//Will enter loop if the current character is a whitespace
	while(tempLineIndex<line.size()&&(line.at(tempLineIndex) == 9 || line.at(tempLineIndex) == 11|| line.at(tempLineIndex) == 32)){
		//Move the Input File pointer to the next character
		tempLineIndex++;
	}
}

void parser::first_and_follow::generateFirstSet(){

	//Check if we've run this file before. IF we have not then we begin the virgin protocol
	//Virgin protocol is used to get the establish file connection.
	//If the virginf protocol fails then we've already established a connection to the file.
	if(virgin == true){
		//Virgin set to false so that we never enter this code again.
		virgin = false;
		//If the virgin protocol fails, then we were not able to establish a connection to the file. The program should close
		if(!virginProtocol())throw std::invalid_argument("Invalid File location");
		else{
			//We are good to begin the process.
		}
	}
	//I need a while loop here to control the flow of change and see if we need to iterate over the file yet again.
	// If change = false then we do not iterate over the file again because there have been no changes.
	//otherwise we iterate again.
	change = true;
	while (change){
		change = false;

		line ="";
		currentSymbol ="";
		lineIndex = 0;
	   // Reset the file stream to read again
		inputFileStream.clear();  // Clear EOF flag
		inputFileStream.seekg(0); // Move back to the beginning
		//we should get line until the end of the file.
		while (std::getline(inputFileStream, line)){
//			std::cout << line << std::endl;
			lineIndex = 0;
			currentSymbol = "";
			currentWord = "";


			getRidOfWhiteSpace();

			if(lineIndex == line.size())
				continue;

			//We get the first set and store it in a temp variable.
			std::vector<std::string> * firstSetPtr = findNT(0);

			//We have the NT. Now we move passed the assignment operator.
			if(checkForAssignment()){
			//If we are here it is becaue we've passed the assignment operator.
			//We've gotten rid of the white space
			//We need a loop here for all of the symbols in the line: essentially while the line index != to length of the line

				currentSymbol = "";

				//We check if the first symbol after the assignment symbol is a terminal.
				if(checkT()){
					//A terminal symbol is found.
					//We simply add the terminal symbol to the first set if its not already there.
					//check if the terminal symbol exists in vector
					getSymbol("t");
					//If the terminal symbol is not in the first set.
					if(!inVector(firstSetPtr)){
						//We add it and move onto the next symbol
						addAndMove( change, firstSetPtr);
						currentSymbol ="";
						continue;

					}
					//if the terminal symbol is in the first set vector, then we need to reset the currentSymbol and move onto the next symbol.
					else {
						currentSymbol = "";
						continue;
					}


				}
				else if(checkNT()){
					//If we are here it is becaue we've passed the assignment operator.
					//We've gotten rid of the white space
					//We check if the first symbol after the assignment symbol is a non terminal, then we get symbol.
					getSymbol("nt");
					compareSymbolVector(change,firstSetPtr);

				}
				else if(checkE()){
					//If we are here it is becaue we've passed the assignment operator.
					//We've gotten rid of the white space
					//We check if the first symbol after the assignment symbol is a an epsilon symbol
					getSymbol("e");
					if(!inVector(firstSetPtr)){
						//We add it and move onto the next symbol
						addAndMove( change, firstSetPtr);
						currentSymbol ="";
						continue;

					}
					//if the terminal symbol is in the first set vector, then we need to reset the currentSymbol and move onto the next symbol.
					else {
						currentSymbol = "";
						continue;
					}

				}
				else{
					std::cout<<"Error in the grammar, neither NT or T found at: "<<currentWord<<std::endl<<"line is: "<<line<<std::endl<<"lineIndex is "<<lineIndex<<std::endl;
				}

			}

			else{
				//If we are here then we did not reach the assignment operator after identfying the noterminal symbol there for there is an error in the grammar
				std::cout<<"There is an error here in the grammar. Should have reach an assignment operator for the NT symbol: "<<currentWord<<std::endl;
				currentWord ="";
				continue;
			}


		}
	}

	disconnectFile();

}

void parser::first_and_follow:: compareSymbolVector(bool & change,std::vector<std::string>* v){
	//Check if this vector exists.
	bool eps;
	if(firstSet.count(currentSymbol)==0){
		change = true;
		return;
	}
	else{
		//We need to get the vector of the current symbol.
		std::vector<std::string> * firstSetPtrOfSymbol = firstSet.at(currentSymbol);

		//For each element in the firstSetPtrOfSymbol vector, we need check if its in our v ptr vector.
		for(std::string value : (*firstSetPtrOfSymbol)){
			if(inVector(v,value) && value != "EPSILON"){
				continue;
			}
			else if (!inVector(v,value) && value != "EPSILON"){
				addAndMove(change,v,value);
				continue;
			}
			if (value == "EPSILON"){
				eps = true;
			}
		}
		while(eps){
			if(lineIndex == line.size()){
				eps = false;
				if(!inVector(v,"EPSILON")){
					addAndMove(change,v,"EPSILON");
					change = true;
				}
			}
			else{
				checkTheNextSymbol(eps,v);
			}

		}
		if(eps &&lineIndex == line.size()){
			if(!inVector(v,"EPSILON")){
				change = true;
				addAndMove(change,v,"EPSILON");
			}
			return;
		}

	}
}

void parser::first_and_follow::checkTheNextSymbol(bool & eps, std::vector<std::string>*v){
	getRidOfWhiteSpace();
	currentSymbol = "";
	if(checkT()){
		eps = false;

		//A terminal symbol is found.
		//We simply add the terminal symbol to the first set if its not already there.
		//check if the terminal symbol exists in vector
		getSymbol("t");
		//If the terminal symbol is not in the first set.
		if(!inVector(v)){
			//We add it and move onto the next symbol
			addAndMove( change, v);
			currentSymbol ="";
			return;

		}
		//if the terminal symbol is in the first set vector, then we need to reset the currentSymbol and move onto the next symbol.
		else {
			currentSymbol = "";
		}

	}
	else if(checkNT()){
		//If we are here it is becaue we've passed the assignment operator.
		//We've gotten rid of the white space
		//We check if the first symbol after the assignment symbol is a non terminal, then we get symbol.
		getSymbol("nt");
		compareSymbolVectorRec(eps,v);

	}



}

void parser::first_and_follow:: compareSymbolVectorRec(bool & eps,std::vector<std::string>* v){
	//Check if this vector exists.
	if(firstSet.count(currentSymbol)==0){
		change = true;
		eps = false;
		return;
	}
	else{
		//We need to get the vector of the current symbol.
		std::vector<std::string> * firstSetPtrOfSymbol = firstSet.at(currentSymbol);

		//For each element in the firstSetPtrOfSymbol vector, we need check if its in our v ptr vector.
		for(std::string value : (*firstSetPtrOfSymbol)){
			if(inVector(v,value) && value != "EPSILON"){
				continue;
			}
			else if (!inVector(v,value) && value != "EPSILON"){
				addAndMove(change,v,value);
				continue;
			}

			if (value == "EPSILON"){
				eps = true;
			}
		}
	}
}

void parser::first_and_follow:: addAndMove(bool& change, std::vector<std::string> * v, std::string s){
	v->emplace_back(s);
	change = true;
//	std::cout<<"Adding: "<<s<<" to the first Set of : "<<currentWord<<std::endl;
}

bool parser::first_and_follow::checkE(){
	if(line.at(lineIndex)=='E'){
		return true;
	}
	return false;

}

void parser::first_and_follow::getSymbol(std::string s,int & tempLineIndex){
	if(s == "t"){while(line.at(tempLineIndex)!= '\''){
		currentSymbol += line.at(tempLineIndex);
		tempLineIndex++;}
	}
	else if(s=="nt"){while(line.at(tempLineIndex)!= '>'){
		currentSymbol += line.at(tempLineIndex);
		tempLineIndex++;}
	}
	else if(s == "e"){while(line.at(tempLineIndex)!=' '){
		currentSymbol += line.at(tempLineIndex);
		tempLineIndex++;}
	}
	tempLineIndex++;
	getRidOfWhiteSpace(tempLineIndex);
}

bool parser::first_and_follow::inVector(std::vector<std::string> * firstSet,std::string s ){
//	std::cout<<(*firstSet)<<std::end
	for (std::string value : (*firstSet)) {
		if(value == s)
			return true;
	}
	return false;
}

bool parser::first_and_follow::checkT(int & tempLinIndex){
	if(line.at(tempLinIndex)=='\''){
		tempLinIndex++;
		return true;
	}
	return false;
}

bool parser::first_and_follow::checkNT(int & tempLinIndex){
	if(line.at(tempLinIndex)=='<'){
		tempLinIndex ++;
		return true;
	}
	return false;
}

std::vector<std::string> * parser::first_and_follow::findNT(int k){
	while(line.at(lineIndex) != '<'){
		lineIndex++;
	}

	lineIndex++;

	//We build the word
	while(line.at(lineIndex) != '>'){
		currentWord += line.at(lineIndex);
		lineIndex++;
	}
	lineIndex++;

	getRidOfWhiteSpace();


	//need to check if this nonterminal already exists.
	if(k == 0){
	    if (firstSet.count(currentWord) == 0) {
	        firstSet[currentWord] = new std::vector<std::string>();  // Allocate memory
	    }

	    return firstSet.at(currentWord);  // Dereference the pointer

	}
	else if(k == 1){
	    if (followSet.count(currentWord) == 0) {
	    	followSet[currentWord] = new std::vector<std::string>();  // Allocate memory
	    }

	    return followSet.at(currentWord);  // Dereference the pointer

	}
}

bool parser::first_and_follow::checkForAssignment(){
	int tempIndex = lineIndex;
	if(line.at(tempIndex)==':'){
		tempIndex++;
		if(line.at(tempIndex)==':'){
			tempIndex++;
			if(line.at(tempIndex)=='='){
				lineIndex = tempIndex;
				lineIndex++;
				getRidOfWhiteSpace();
				return true;
			}
		}
	}
	return false;
}

void parser::first_and_follow::generateFollowSet(){

	//Check if we've run this file before. IF we have not then we begin the virgin protocol
	//Virgin protocol is used to get the establish file connection.
	//If the virginf protocol fails then we've already established a connection to the file.
	if(virgin == true){
		//Virgin set to false so that we never enter this code again.
		virgin = false;
		//If the virgin protocol fails, then we were not able to establish a connection to the file. The program should close
		if(!virginProtocol())throw std::invalid_argument("Invalid File location");
		else{
			//We are good to begin the process.
		}
	}
	//I need a while loop here to control the flow of change and see if we need to iterate over the file yet again.
	// If change = false then we do not iterate over the file again because there have been no changes.
	//otherwise we iterate again.

	//Just add  '$' to the start follow start
	//First you have to create the vector list
	std::vector<std::string> *startFollowSet = new std::vector<std::string>;
	startFollowSet->emplace_back("$");
	followSet["START"] = startFollowSet;


	change = true;
	while (change){
		change = false;
		line ="";
		currentSymbol ="";
		lineIndex = 0;
	   // Reset the file stream to read again
		inputFileStream.clear();  // Clear EOF flag
		inputFileStream.seekg(0); // Move back to the beginning
		//we should get line until the end of the file.
		while (std::getline(inputFileStream, line)){

//			std::cout << line << std::endl;
			lineIndex = 0;
			currentSymbol = "";
			currentWord = "";


			getRidOfWhiteSpace();

			if(lineIndex == line.size())
				continue;

			//Build the first nt symbol because we might need its follow set.
			std::string nt;
			if(line.at(lineIndex =='<')){
				lineIndex++;
				while(line.at(lineIndex)!='>'){
					nt += line.at(lineIndex);
					lineIndex++;
				}
			}

			lineIndex++;
			getRidOfWhiteSpace();
			checkForAssignment();


			//In the firstset generation we were looking at the first nt symbol
			//In the followset generation we need to look at each symbol in the line.
			//sO we have a while loop that terminates when we reach teh end of the line.
			while(lineIndex != line.size()){
				getRidOfWhiteSpace();

				//We need to getFollowSetSymbol.
				getFollowSetSymbol();
				if(lineIndex == line.size())continue;

				currentWord ="";
				currentSymbol ="";
				//Now currentWord == the symbol that we need to analyze
				std::vector<std::string> * followSetVectorOfWord = findNT(1);

				int tempLineIndex = lineIndex;

				//Now we get the symbol after it.
				//We need to  move the pointer to the location of the next possible
				movePointer(tempLineIndex);
				getRidOfWhiteSpace(tempLineIndex);
				if(lineIndex == line.size()){
					//WE have reached the end of the string.
					// If this is the case then the follow set of of the original nt symbol is part followset of the currentWord
					std::vector<std::string> * followSetOfOriginalNT = findFollowSet(nt);
					compareAndAdd(followSetVectorOfWord,followSetOfOriginalNT);
					currentSymbol ="";
					break;
				}

				else if(checkT(tempLineIndex)){
					//A terminal symbol is found.
					//We simply add the terminal symbol to the first set if its not already there.
					//check if the terminal symbol exists in vector
					getSymbol("t",tempLineIndex);
					//If the terminal symbol is not in the first set.
					if(!inVector(followSetVectorOfWord)){
						//We add it and move onto the next symbol
						addAndMove( change, followSetVectorOfWord);
						currentSymbol ="";
						continue;

					}
					//if the terminal symbol is in the first set vector, then we need to reset the currentSymbol and move onto the next symbol.
					else {
						currentSymbol = "";
						continue;
					}


				}

				else if(checkNT(tempLineIndex)){
					//If we are here it is becaue we've passed the assignment operator.
					//We've gotten rid of the white space
					//We check if the first symbol after the assignment symbol is a non terminal, then we get symbol.
					getSymbol("nt",tempLineIndex);
					std::vector<std::string>*firstSetOfSymbol = firstSet[currentSymbol];
					compareAndAdd(followSetVectorOfWord,firstSetOfSymbol);
					if(hasEpsilon(firstSetOfSymbol)){
						while(tempLineIndex<=line.size()){
							getRidOfWhiteSpace();
							if(tempLineIndex==line.size()){
								std::vector<std::string> * followSetOfOriginalNT = findFollowSet(nt);
								compareAndAdd(followSetVectorOfWord,followSetOfOriginalNT);
								currentSymbol ="";
								break;
							}
							else if(checkT(tempLineIndex)){
								//A terminal symbol is found.
								//We simply add the terminal symbol to the first set if its not already there.
								//check if the terminal symbol exists in vector
								currentSymbol ="";
								getSymbol("t",tempLineIndex);
								//If the terminal symbol is not in the first set.
								if(!inVector(followSetVectorOfWord)){
									//We add it and move onto the next symbol
									addAndMove( change, followSetVectorOfWord);
									currentSymbol ="";
									break;

								}
								//if the terminal symbol is in the first set vector, then we need to reset the currentSymbol and move onto the next symbol.
								else {
									currentSymbol = "";
									break;
								}


							}
							else if(checkNT(tempLineIndex)){
								currentSymbol ="";
								getSymbol("nt",tempLineIndex);
								std::vector<std::string>*firstSetOfSymbol = firstSet[currentSymbol];
								compareAndAdd(followSetVectorOfWord,firstSetOfSymbol);
								if(hasEpsilon(firstSetOfSymbol))continue;
								else break;
							}


						}
						continue;
					}
					else continue;



				}




			}

		}
	}
	disconnectFile();
}

bool parser::first_and_follow::hasEpsilon(std::vector <std::string> * b){
	for(std::string value : (*b) ){
		if(value == "EPSILON")
			return true;
	}
	return false;
}

void parser::first_and_follow::getToAssignmentOp(){
	while(lineIndex!= line.size() && line.at(lineIndex) != ':')
		lineIndex++;
}

void parser::first_and_follow::getFollowSetSymbol(){
	while(lineIndex!= line.size() && line.at(lineIndex)!='<'){
		lineIndex ++;
	}
	if(lineIndex +1 < line.size()){
		if(line.at(lineIndex+1) == '>'||line.at(lineIndex+1) == '=' || line.at(lineIndex+1) == '\''){
			lineIndex++;
			while(lineIndex!= line.size() && line.at(lineIndex)!='<'){
				lineIndex ++;
			}

		}

	}

}

void parser::first_and_follow::movePointer(int & templineIndex){
	while(templineIndex!=line.size()&&line.at(templineIndex)!='<' &&line.at(templineIndex)!='\''&&line.at(templineIndex)!='e' ){
		templineIndex++;
	}
}

std::vector<std::string> * parser::first_and_follow:: findFollowSet(std::string s){
	if(followSet.count(s)==0){
		followSet[s] = new std::vector<std::string>();  // Allocate memory
    	change = true;

	}

	return followSet[s];

}

void parser::first_and_follow:: compareAndAdd(std::vector<std::string> * followSetOfWord, std::vector<std::string> * followSetOfOriginalNT){
	if(followSetOfOriginalNT->size()==0){
		change =true;
		return;
	}
	if(followSetOfWord->size()==0){
		for(std::string value: *followSetOfOriginalNT){
			if(value!="EPSILON")
			followSetOfWord->emplace_back(value);

		}
		change = true;
		return;
	}

	auto iteratorNT = followSetOfOriginalNT->begin();
	auto iteratorW = followSetOfWord->begin();

	for( ;iteratorNT != followSetOfOriginalNT->end(); iteratorNT++){

		for(iteratorW = followSetOfWord->begin(); iteratorW != followSetOfWord->end();iteratorW ++){
			if((*iteratorNT) == (*iteratorW)){
				break;
			}
		}

		if((*iteratorNT) == (*iteratorW)){
			continue;
		}
		else if(iteratorW == followSetOfWord->end() && (*iteratorNT)!="EPSILON"){
			change = true;
			followSetOfWord->emplace_back(*iteratorNT);

		}

	}
}

/*
 *
 *
 *
 * PARSING TABLE SECTION BELOW
 *
 *
 *
 */

int parser::parsing_table:: TABLEROWSIZE;
int parser::parsing_table:: TABLECOLUMNSIZE;
int parser::parsing_table:: UNIT;
std::ifstream parser::parsing_table::inputFileStream;
std::unordered_set<std::string> parser::parsing_table::uniqueTerminalSymbols;
std::unordered_map<std::string, int> parser::parsing_table::tableRows;
std::unordered_map<std::string, int> parser::parsing_table::tableColumns;  // Ensure values are ints, not references

bool parser::parsing_table::connectFile(std::string fileName){

	if(inputFileStream.is_open()){
			std::cout<<"The file is connected... Closing it first..."<<std::endl;
			inputFileStream.close();
		}

	inputFileStream.open(fileName,std::ios::in);

	if(inputFileStream){
		std::cout<<"Successfully opened file: "<<fileName<<"."<<std::endl;
		return true;
	}
	else{
		std::cout<<"Failed to open file: "<<fileName<<"."<<std::endl;
		return false;
	}
}

bool parser::parsing_table::disconnectFile(){

	if(!inputFileStream.is_open()){
			std::cout<<"The file is not even connected. No need to close."<<std::endl;
			return true;
		}
	inputFileStream.close();

	if(!inputFileStream.is_open()){
		std::cout<<"Successfully closed the inputFileStream."<<std::endl;
		return true;
	}
	else{
		std::cout<<"Failed to close the inputFileStream."<<std::endl;
		return false;
	}
}

void parser::parsing_table:: buildTable(){
	setUniqueTerminalSymbols();
	setRowAndColumnSize();
	setTheTableSize();
	setTableRow();
	setTableColumn();
	fillTableWithErrors();
	createParsingTable();
}

void parser::parsing_table::fillTableWithErrors(){
	for (int i = 0; i < TABLEROWSIZE; i++) {
	    for (int j = 0; j < TABLECOLUMNSIZE; j++) {
			tableEntry currentEntry = {i,j,"error"};
	    	table[i][j] = currentEntry;
	    }
	}
}

void parser::parsing_table:: setUniqueTerminalSymbols(){
    for (const auto& pair : faf.firstSet) {
        for (const auto& value : (*pair.second)) {
        	if(value == "EPSILON"){
        		continue;
        	}
        	uniqueTerminalSymbols.insert(value);  // insert automatically handles duplicates
        }
    }
    for (const auto& pair : faf.followSet) {
        for (const auto& value : (*pair.second)) {
        	uniqueTerminalSymbols.insert(value);  // insert automatically handles duplicates
        }
    }

    uniqueTerminalSymbols.insert(":");
    uniqueTerminalSymbols.insert("=>");
    uniqueTerminalSymbols.insert(":=");
    uniqueTerminalSymbols.insert("then");
    uniqueTerminalSymbols.insert("invalidnum");




}
//THE TABLESROWS REPRESENT THE VALUE OF THE FIRST ARRAY
int parser::parsing_table:: getTableRowSize(){
	return faf.firstSet.size();
}
//TABLE COLUMNS REPRESENT THE VALUE OF THE SECOND ARRAY
int parser::parsing_table:: getTableColumnSize(){
	return (uniqueTerminalSymbols.size());
}

void parser::parsing_table:: setRowAndColumnSize(){
	TABLEROWSIZE = getTableRowSize();
	TABLECOLUMNSIZE = getTableColumnSize();
	UNIT = sizeof(struct tableEntry);
}

void parser::parsing_table:: setTheTableSize(){
	table = new tableEntry*[TABLEROWSIZE]; // Allocate row pointers
	for (int i = 0; i < TABLEROWSIZE; ++i) {
	    table[i] = new tableEntry[TABLECOLUMNSIZE]; // Allocate each row
	}
}

void parser::parsing_table::setTableRow(){
	int i = 0;
//	std::cout<< "FOR THE ROWS"<<std::endl;
	for(const auto& value : faf.firstSet){
		tableRows[value.first] = i;
//		std::cout<<"Row = "<< value.first <<", value = "<<i<<std::endl;
		i++;
	}
}

void parser::parsing_table::setTableColumn(){
	int i = 0;
//	std::cout<< "FOR THE COLUMNS"<<std::endl;
	for(const auto& value: uniqueTerminalSymbols){
		tableColumns[value]=i;
//		std::cout<<"Column = "<< value<<", value = "<<i<<std::endl;

		i++;
	}

}

void parser::parsing_table::getRule(std::string & rule, std::string line, int & lineIndex){
	 if(lineIndex == line.size())
		 return;

	 while(line.at(lineIndex)!='<'){
		 if(lineIndex == line.size())
			 return;

		 lineIndex++;
	 }
	 lineIndex++;
	 while(line.at(lineIndex)!='>'){
		 rule+=line.at(lineIndex);
		 lineIndex++;
	 }
	 while(line.at(lineIndex)!='='){
			 lineIndex++;
	 }

	 lineIndex++;
}

void parser::parsing_table::getAlpha(std::string & alpha, const std::string line, int & lineIndex){
	while(lineIndex!=line.size()){
		alpha+=line.at(lineIndex);
		lineIndex++;
	}
}

bool parser::parsing_table::checkFirstOfAlpha(const std::string alpha,std::unordered_set<std::string> & firstOfAlpha,bool & isTerminal){
//	std::cout<<"hello"<<std::endl;
	int tempLineIndex = 0;
	while(alpha.at(tempLineIndex)!= '\'' && alpha.at(tempLineIndex)!='<' && alpha.at(tempLineIndex)!= 'E'){
		tempLineIndex++;
	}
	if (alpha.at(tempLineIndex)=='\''){
		isTerminal = true;
		tempLineIndex++;
		std::string terminal;
		while(alpha.at(tempLineIndex)!='\''){
			terminal +=alpha.at(tempLineIndex);
			tempLineIndex++;
		}
		firstOfAlpha.insert(terminal);
		return false;
	}

	else if(alpha.at(tempLineIndex)== '<'){
		isTerminal = false;
		bool containsEpsilon = false;
		tempLineIndex++;
		std::string NonTerminal;
		while(alpha.at(tempLineIndex)!='>'){
			NonTerminal += alpha.at(tempLineIndex);
			tempLineIndex++;
		}

		std::vector<std::string> * NonTerminalFirstSet = faf.firstSet[NonTerminal];
		for(const auto & value:(*NonTerminalFirstSet)){
			firstOfAlpha.insert(value);
			if(value == "EPSILON"){
				containsEpsilon = true;
			}
		}
		if(containsEpsilon){
			while(containsEpsilon){
				containsEpsilon = false;
				firstOfAlpha.erase("EPSILON");
				std::string nextSymbol;

				while(tempLineIndex != alpha.size() && alpha.at(tempLineIndex)!= '\'' && alpha.at(tempLineIndex)!='<' && alpha.at(tempLineIndex)!= 'E'){
					tempLineIndex++;
				}

				if(tempLineIndex == alpha.size()){
					containsEpsilon = true;
					return true;
				}

				if (alpha.at(tempLineIndex)=='\''){
						tempLineIndex++;
						while(alpha.at(tempLineIndex)!='\''){
							nextSymbol +=alpha.at(tempLineIndex);
							tempLineIndex++;
						}
						firstOfAlpha.insert(nextSymbol);
						return false;
					}
				else if(alpha.at(tempLineIndex)== '<'){
					tempLineIndex++;
					while(alpha.at(tempLineIndex)!='>'){
						nextSymbol += alpha.at(tempLineIndex);
						tempLineIndex++;
					}
					if(nextSymbol =="rept-statement4"){

					}

					std::vector<std::string> * NonTerminalFirstSetSecond = faf.firstSet[nextSymbol];
					for(const auto & value:(*NonTerminalFirstSetSecond)){
						if(value == "EPSILON"){
							containsEpsilon = true;
						}
						firstOfAlpha.insert(value);
					}


				}


			}
			if(!containsEpsilon)return true;

		}
		else return false;
//		for (const auto& val : firstOfAlpha) {
//		        std::cout << val << std::endl;
//		    }

	}
	else if(alpha.at(tempLineIndex)=='E'){
		firstOfAlpha.insert("EPSILON");
		return true;
	}
	else{
		std::cout<<"Code should not reach here. You are in parser::parsing_table::checkFirstOfAlpha"<<std::endl;
	}

}

void parser::parsing_table::updateTable(const std::string rule, std::unordered_set <std::string> firstOfAlpha,std::string line){
//
	for (const auto& value : firstOfAlpha){
		int rowValue = tableRows[rule];
		int columnValue = tableColumns[value];
//		std::cout<<value<<rowValue<<","<<columnValue<<std::endl;
		tableEntry currentEntry = {rowValue,columnValue,line};
		table[rowValue][columnValue] = currentEntry;
	}
}

bool parser::parsing_table::hasEpsilon(std::unordered_set <std::string> & firstOfAlpha, bool isTerminal){
	for (auto value : firstOfAlpha){
		if(value == "EPSILON")
			return true;
	}
	return false;

}

void parser::parsing_table::getFollowOfRule(std::unordered_set <std::string> & followOfRule,const std::string rule ){
	std::vector<std::string> * ruleFollowVector = faf.followSet[rule];
	for (auto value : (*ruleFollowVector)){
		followOfRule.insert(value);
	}
}

void parser::parsing_table::createParsingTable(){
	if(connectFile("/home/giusuppe/eclipse-workspace/compilerDesign/AttributeGrammar.txt")){
//	if(connectFile("/home/giusuppe/eclipse-workspace/compilerDesign/Assignment2.COMP442-6421.paquet.2025.4/assignment2.COMP442-6421.paquet.2025.4 NEW/grammars/noLeftRec.grm")){

		//We've properly connected to the file now we need to get rid of the white space and then hand control back over getNextToken().
	}
	std::string line;

	while (std::getline(inputFileStream, line)){
		int lineIndex=0;
		bool hasEpsilon;
		std::string rule;
		std::string alpha;
		std::unordered_set <std::string> firstOfAlpha;
		std::unordered_set <std::string> followOfRule;
		bool isTerminal;

		getRule(rule,line,lineIndex);
		 if(lineIndex == line.size())
			 continue;
//		 std::cout<<"THE RULE IS: "<<rule<<std::endl<<std::endl;
		getAlpha(alpha,line,lineIndex);
//		std::cout<<"THE ALPHA IS: "<<alpha<<std::endl;
		hasEpsilon = checkFirstOfAlpha(alpha,firstOfAlpha,isTerminal);
//		 for (const auto& elem : firstOfAlpha) {
//		        std::cout << elem << " ";
//
//		    }
//		 std::cout<<std::endl;

//		for (const auto& val : firstOfAlpha) {
//		        std::cout << val << std::endl;
//		    }

		updateTable(rule,firstOfAlpha,line);


		if(hasEpsilon){
//			std::cout<<"ALPHA HAS EPSILON NEED TO ADD FOLLOW SET OF "<<rule<<std::endl;
			getFollowOfRule(followOfRule,rule);
			updateTable(rule,followOfRule,line);
		}

//		insertErrors();

	}
    std::ofstream file("output.txt");
	auto rowit = tableRows.begin();

    for (int i = 0; i < TABLEROWSIZE; i++) {
    	auto columnit = tableColumns.begin();

        for (int j = 0; j < TABLECOLUMNSIZE; j++) {

            file << "ROW: " << rowit ->first<< ", at value: " << rowit->second<<std::endl
                      << "COLUMN : " << columnit -> first<<", at value: "<<columnit -> second <<std::endl
                      << "Table entry: " << table[rowit->second][columnit -> second].derivationRule << std::endl;
            columnit++;
        }
        rowit++;
    }
    file.close();

	disconnectFile();


}

/*
 *
 *
 *
 * MAIN PARSING SECTION BELOW
 *
 *
 *
 */
//Forward Declaration
std::stack<std::string> parser::parsingStack;
std::stack<parser::abstractSyntaxTree::node *> parser::semanticStack;
parser::semanticActions parser::semanticHandler;
parser::abstractSyntaxTree parser::AST;

bool parser::parse(const std::vector<token*>  & vectorOfTokens){
    std::ofstream derivationOut("Derivation"); // Open file for writing
    int i =0;
	std::vector<std::string> derivation;
	int lineIndex =0;
	bool error;
	parsingStack.push("$");
	parsingStack.push("START");
	derivation.emplace_back("START");
	auto vectorIterator = vectorOfTokens.begin();
	token* currentToken = (*vectorIterator);

	while(parsingStack.top()!="$"){
		if(i >= 1280)
		{
//			std::cout<<i<<std::endl;

		}
		i++;
		std::string topOfTheStack = parsingStack.top();
		if(parsingTable.uniqueTerminalSymbols.count(topOfTheStack)!=0){
			if(topOfTheStack==(currentToken->getTypeName())){
				parsingStack.pop();
				derivation.erase(derivation.begin()+lineIndex);
				derivation.insert(derivation.begin()+lineIndex,currentToken->getLexeme());
				lineIndex++;
				vectorIterator++;
				currentToken = (*vectorIterator);
//				for(auto const & value:derivation){
//					derivationOut<<value<<" ";
//				}
//				derivationOut<<std::endl;
			}
			else{
				//Handle error;
				std::string topOfStack;
				topOfStack = parsingStack.top();
				skipError(currentToken,vectorIterator,derivationOut,topOfStack,topOfTheStack,currentToken->getLine(),currentToken->getColumn());
				error = true;
			}

		}

		else if(topOfTheStack.at(0) == '#'){
			std::string lastTokenValue;
			if(vectorIterator!= vectorOfTokens.begin()){
				lastTokenValue = (*(vectorIterator - 1))->getLexeme();
			}
			std::string semanticFunction;
			parsingStack.pop();
			for(int i = 1; i<topOfTheStack.size();i++){
				semanticFunction +=topOfTheStack[i];
			}
			semanticHandler.handleAction(semanticFunction,lastTokenValue);
			derivation.erase(derivation.begin()+lineIndex);

		}
		else {
			if(topOfTheStack == "EPSILON"){
				parsingStack.pop();
				derivation.erase(derivation.begin()+lineIndex);
				for(auto const & value:derivation){
					derivationOut<<value<<" ";
				}
				derivationOut<<std::endl;
				continue;
			}

			//Need to find the row and column value for the table
			//row will be whatever the top of the stack is.
			int rowValue = parsingTable.tableRows[topOfTheStack];
			int columnValue = parsingTable.tableColumns[((currentToken)->getTypeName())];
			tableEntry currentTableEntry = parsingTable.table[rowValue][columnValue];
			if(currentTableEntry.derivationRule!="error"){
				parsingStack.pop();
				derivation.erase(derivation.begin()+lineIndex);
				inverseRHSMultiplePush(currentTableEntry,derivation,lineIndex);
				for(auto const & value:derivation){
					derivationOut<<value<<" ";
				}
				derivationOut<<std::endl;
			}
			else{
				//Need to check the follow set of the current the top of the stack.
				std::string topOfStack;
				topOfStack = parsingStack.top();
				//Handle error;
				skipError(currentToken,vectorIterator,derivationOut,topOfStack,topOfTheStack,currentToken->getLine(),currentToken->getColumn());
				error = true;
			}
		}


	}
	if((currentToken->getTypeName()!="$")||(error ==true)){
		return false;
	}
	else return true;
}

void parser::inverseRHSMultiplePush(tableEntry currentTableEntry,std::vector<std::string>&derivation,const int & lineIndex){
	std::string lineToInverseAndSplitThenPush = currentTableEntry.derivationRule;
	std::stack<std::string> items;
	int tempLineIndex =0;
	while(lineToInverseAndSplitThenPush.at(tempLineIndex)!= '>'){tempLineIndex++;}
	while(lineToInverseAndSplitThenPush.at(tempLineIndex)!= '='){tempLineIndex++;}
	tempLineIndex++;
	while(tempLineIndex<lineToInverseAndSplitThenPush.size()){
		std::string currentString;
		while(tempLineIndex != lineToInverseAndSplitThenPush.size() && lineToInverseAndSplitThenPush.at(tempLineIndex)!= '\'' && lineToInverseAndSplitThenPush.at(tempLineIndex)!='<' &&lineToInverseAndSplitThenPush.at(tempLineIndex)!='#'  && lineToInverseAndSplitThenPush.at(tempLineIndex)!= 'E'){
			tempLineIndex++;
		}
		if(tempLineIndex==lineToInverseAndSplitThenPush.size())break;
		if (lineToInverseAndSplitThenPush.at(tempLineIndex)=='\''){
			tempLineIndex++;

			while(lineToInverseAndSplitThenPush.at(tempLineIndex)!='\''){
				currentString +=lineToInverseAndSplitThenPush.at(tempLineIndex);
				tempLineIndex++;

			}
			tempLineIndex++;
			items.push(currentString);

			continue;
		}
		else if(lineToInverseAndSplitThenPush.at(tempLineIndex)=='<'){
			tempLineIndex++;

			while(lineToInverseAndSplitThenPush.at(tempLineIndex)!='>'){
				currentString +=lineToInverseAndSplitThenPush.at(tempLineIndex);
				tempLineIndex++;

			}
			tempLineIndex++;
			items.push(currentString);

			continue;
		}
		else if(lineToInverseAndSplitThenPush.at(tempLineIndex)=='#' ){

					while(tempLineIndex!=lineToInverseAndSplitThenPush.size()&&lineToInverseAndSplitThenPush.at(tempLineIndex)!='\ '){
						currentString +=lineToInverseAndSplitThenPush.at(tempLineIndex);
						tempLineIndex++;

					}
					tempLineIndex++;
					items.push(currentString);

					continue;
		}
		else if(lineToInverseAndSplitThenPush.at(tempLineIndex)=='E'){

			while(tempLineIndex!=lineToInverseAndSplitThenPush.size()&&currentString != "EPSILON"){
				currentString +=lineToInverseAndSplitThenPush.at(tempLineIndex);
				tempLineIndex++;

			}
			tempLineIndex++;

			//items.push("EPSILON");
//			break;
		}



	}

	while(!items.empty()){
		 parsingStack.push(items.top());
			derivation.insert(derivation.begin()+lineIndex,items.top());

		 items.pop();
	}
}

void parser::skipError(token * & currentToken,std::vector<token*>::const_iterator& vectorIterator,std::ofstream & derivationOut,const std::string &lookahead, const std::string &topOfTheStack,const int & line, const int & column){
	derivationOut<<"Syntax error at ("<< line<<", "<<column<<")"<<std::endl;
	std::string nextToken = (*(vectorIterator +1))->getLexeme();
	if(nextToken == "$"|| search(nextToken,topOfTheStack)){
		parsingStack.pop();
		vectorIterator++;
	}
	else{
		while((!searchFirst(nextToken,topOfTheStack))&&(searchFirst("EPSILON",topOfTheStack)&&!search(nextToken,topOfTheStack))){
			vectorIterator++;
			nextToken = (*(vectorIterator+1))->getLexeme();
			currentToken = (*vectorIterator);
		}
		vectorIterator++;
		currentToken = (*vectorIterator);

	}

}

bool parser::search(const std::string & nextToken, const std::string & topOfTheStack){
	if(first_and_follow::inVector(faf.followSet[topOfTheStack],nextToken)){
		return true;
	}
	else return false;
}

bool parser::searchFirst(std::string nextToken, std::string topOfTheStack){
	if(first_and_follow::inVector(faf.firstSet[topOfTheStack],nextToken)){
		return true;
	}
	else return false;
}

void parser::semanticActions::handleAction(std::string semanticFunction,std::string lastTokenValue){


	if(semanticFunction == "start"){abstractSyntaxTree::startNode * newnode = new abstractSyntaxTree::startNode(); semanticHandler.passAlong("start",newnode);}
	else if(semanticFunction == "aparamstail"){abstractSyntaxTree::aparamstailNode * newnode = new abstractSyntaxTree::aparamstailNode(); semanticHandler.passAlong("aparamstail",newnode);}
	else if(semanticFunction == "arithexpr"){abstractSyntaxTree::arithexprNode * newnode = new abstractSyntaxTree::arithexprNode(); semanticHandler.passAlong("arithexpr",newnode);}
	else if(semanticFunction == "arraysize"){abstractSyntaxTree::arraysizeNode * newnode = new abstractSyntaxTree::arraysizeNode(); semanticHandler.passAlong("arraysize",newnode);}
	else if(semanticFunction == "attributedecl"){abstractSyntaxTree::attributedeclNode * newnode = new abstractSyntaxTree::attributedeclNode(); semanticHandler.passAlong("attributedecl",newnode);}
	else if(semanticFunction == "class"){abstractSyntaxTree::classNode * newnode = new abstractSyntaxTree::classNode(); semanticHandler.passAlong("class",newnode);}
	else if(semanticFunction == "impl"){abstractSyntaxTree::implNode * newnode = new abstractSyntaxTree::implNode(); semanticHandler.passAlong("impl",newnode);}
	else if(semanticFunction == "func"){abstractSyntaxTree::funcNode * newnode = new abstractSyntaxTree::funcNode(); semanticHandler.passAlong("func",newnode);}
	else if(semanticFunction == "factor"){abstractSyntaxTree::factorNode * newnode = new abstractSyntaxTree::factorNode(); semanticHandler.passAlong("factor",newnode);}
	else if(semanticFunction == "idnest2"){abstractSyntaxTree::idnest2Node * newnode = new abstractSyntaxTree::idnest2Node(); semanticHandler.passAlong("idnest2",newnode);}
	else if(semanticFunction == "funcbody"){abstractSyntaxTree::funcbodyNode * newnode = new abstractSyntaxTree::funcbodyNode(); semanticHandler.passAlong("funcbody",newnode);}
	else if(semanticFunction == "funcdecl"){abstractSyntaxTree::funcdeclNode * newnode = new abstractSyntaxTree::funcdeclNode(); semanticHandler.passAlong("funcdecl",newnode);}
	else if(semanticFunction == "indice"){abstractSyntaxTree::indiceNode * newnode = new abstractSyntaxTree::indiceNode(); semanticHandler.passAlong("indice",newnode);}
	else if(semanticFunction == "localvardecl"){abstractSyntaxTree::localvardeclNode * newnode = new abstractSyntaxTree::localvardeclNode(); semanticHandler.passAlong("localvardecl",newnode);}
	else if(semanticFunction == "localvardeclorstat"){abstractSyntaxTree::localvardeclorstatNode * newnode = new abstractSyntaxTree::localvardeclorstatNode(); semanticHandler.passAlong("localvardeclorstat",newnode);}
	else if(semanticFunction == "prog"){abstractSyntaxTree::progNode * newnode = new abstractSyntaxTree::progNode(); semanticHandler.passAlong("prog",newnode);}
	else if(semanticFunction == "condition"){abstractSyntaxTree::conditionNode * newnode = new abstractSyntaxTree::conditionNode(); semanticHandler.passAlong("condition",newnode);}
	else if(semanticFunction == "then"){abstractSyntaxTree::thenNode * newnode = new abstractSyntaxTree::thenNode(); semanticHandler.passAlong("then",newnode);}
	else if(semanticFunction == "felse"){abstractSyntaxTree::felseNode * newnode = new abstractSyntaxTree::felseNode(); semanticHandler.passAlong("felse",newnode);}
	else if(semanticFunction == "statblock"){abstractSyntaxTree::statblockNode * newnode = new abstractSyntaxTree::statblockNode(); semanticHandler.passAlong("statblock",newnode);}
	else if(semanticFunction == "returntype"){abstractSyntaxTree::returntypeNode * newnode = new abstractSyntaxTree::returntypeNode(); semanticHandler.passAlong("returntype","returntype",newnode);}
	else if(semanticFunction == "term"){abstractSyntaxTree::termNode * newnode = new abstractSyntaxTree::termNode(); semanticHandler.passAlong("term",newnode);}


	else if(semanticFunction == "intnum"){abstractSyntaxTree::intNode * newnode = new abstractSyntaxTree::intNode();semanticHandler.makeLeaf("intnum",lastTokenValue,newnode);}
	else if(semanticFunction == "p"){abstractSyntaxTree::epsilonNode * newnode = new abstractSyntaxTree::epsilonNode();semanticHandler.makeLeaf("epsilon",lastTokenValue,newnode);}
	else if(semanticFunction == "addop"){abstractSyntaxTree::addopNode * newnode = new abstractSyntaxTree::addopNode();semanticHandler.makeLeaf("addop",lastTokenValue,newnode);}
	else if(semanticFunction == "assign"){abstractSyntaxTree::assignNode * newnode = new abstractSyntaxTree::assignNode();semanticHandler.makeLeaf("assign",lastTokenValue,newnode);}
	else if(semanticFunction == "id"){abstractSyntaxTree::idNode * newnode = new abstractSyntaxTree::idNode();semanticHandler.makeLeaf("id",lastTokenValue,newnode);}
	else if(semanticFunction == "relop"){abstractSyntaxTree::relopNode * newnode = new abstractSyntaxTree::relopNode();semanticHandler.makeLeaf("relop",lastTokenValue,newnode);}
	else if(semanticFunction == "self"){abstractSyntaxTree::selfNode * newnode = new abstractSyntaxTree::selfNode();semanticHandler.makeLeaf("self",lastTokenValue,newnode);}
	else if(semanticFunction == "multop"){abstractSyntaxTree::multopNode * newnode = new abstractSyntaxTree::multopNode();semanticHandler.makeLeaf("multop",lastTokenValue,newnode);}
	else if(semanticFunction == "void"){abstractSyntaxTree::voidNode * newnode = new abstractSyntaxTree::voidNode();semanticHandler.makeLeaf("void",lastTokenValue,newnode);}
	else if(semanticFunction == "sign"){abstractSyntaxTree::signNode * newnode = new abstractSyntaxTree::signNode();semanticHandler.makeLeaf("sign",lastTokenValue,newnode);}
	else if(semanticFunction == "if"){abstractSyntaxTree::ifNode * newnode = new abstractSyntaxTree::ifNode();semanticHandler.makeLeaf("if",lastTokenValue,newnode);}
	else if(semanticFunction == "while"){abstractSyntaxTree::whileNode * newnode = new abstractSyntaxTree::whileNode();semanticHandler.makeLeaf("while",lastTokenValue,newnode);}
	else if(semanticFunction == "read"){abstractSyntaxTree::readNode * newnode = new abstractSyntaxTree::readNode();semanticHandler.makeLeaf("read",lastTokenValue,newnode);}
	else if(semanticFunction == "write"){abstractSyntaxTree::writeNode * newnode = new abstractSyntaxTree::writeNode();semanticHandler.makeLeaf("write",lastTokenValue,newnode);}
	else if(semanticFunction == "return"){abstractSyntaxTree::returnNode * newnode = new abstractSyntaxTree::returnNode();semanticHandler.makeLeaf("return",lastTokenValue,newnode);}
	else if(semanticFunction == "type"){abstractSyntaxTree::typeNode * newnode = new abstractSyntaxTree::typeNode();semanticHandler.makeLeaf("type",lastTokenValue,newnode);}
	else if(semanticFunction == "visibility"){abstractSyntaxTree::visibilityNode * newnode = new abstractSyntaxTree::visibilityNode();semanticHandler.makeLeaf("visibility",lastTokenValue,newnode);}
	else if(semanticFunction == "floatnum"){abstractSyntaxTree::floatnumNode * newnode = new abstractSyntaxTree::floatnumNode();semanticHandler.makeLeaf("floatnum",lastTokenValue,newnode);}


	else if(semanticFunction == "reptclassdecl4"){abstractSyntaxTree::reptclassdecl4Node * newnode = new abstractSyntaxTree::reptclassdecl4Node();std::vector<std::string> v = {"epsilon"}; semanticHandler.makeSubTree("reptclassdecl4","{",'E',v,newnode);}
	else if(semanticFunction == "classdecl"){abstractSyntaxTree::classdeclNode * newnode = new abstractSyntaxTree::classdeclNode();std::vector<std::string> v = {"id","optclassdecl2","reptclassdecl4"}; semanticHandler.makeSubTree("classdecl","classdecl",3,v,newnode);}
	else if(semanticFunction == "aparams"){abstractSyntaxTree::aparamsNode * newnode = new abstractSyntaxTree::aparamsNode(); std::vector<std::string> v = {"epsilon"}; semanticHandler.makeSubTree("aparams","aparams",'E',v,newnode);}
	else if(semanticFunction == "funcdef"){abstractSyntaxTree::classdeclNode * newnode = new abstractSyntaxTree::classdeclNode();std::vector<std::string> v = {"funchead","funcbody"}; semanticHandler.makeSubTree("funcdef","funcdef",2,v,newnode);}
	else if(semanticFunction == "expr"){abstractSyntaxTree::exprNode*newnode = new abstractSyntaxTree::exprNode(); std::vector<std::string> v = {"arithexpr","expr2","epsilon"}; semanticHandler.makeSubTree("expr","expr",2,v,newnode);}
	else if(semanticFunction == "expr2"){abstractSyntaxTree::expr2Node*newnode = new abstractSyntaxTree::expr2Node();std::vector<std::string> v = {"relop","arithexpr"}; semanticHandler.makeSubTree("expr2","expr2",2,v,newnode);}
	else if(semanticFunction == "reptfparams3"){abstractSyntaxTree::reptfparams3Node*newnode = new abstractSyntaxTree::reptfparams3Node();std::vector<std::string> v = {"epsilon"}; semanticHandler.makeSubTree("reptfparams3","dimlist",'E',v,newnode);}
	else if(semanticFunction == "reptfparams4"){abstractSyntaxTree::reptfparams4Node*newnode = new abstractSyntaxTree::reptfparams4Node();std::vector<std::string> v = {"epsilon"}; semanticHandler.makeSubTree("reptfparams4","param",'E',v,newnode);}
	else if(semanticFunction == "param"){abstractSyntaxTree::paramNode*newnode = new abstractSyntaxTree::paramNode();std::vector<std::string> v = {"epsilon"}; semanticHandler.makeSubTree("param","param",'E',v,newnode);}
	else if(semanticFunction == "fparams"){abstractSyntaxTree::fparamsNode*newnode = new abstractSyntaxTree::fparamsNode();std::vector<std::string> v = {"param","reptfparams4"}; semanticHandler.makeSubTree("fparams","funcparams",'E',v,newnode);}
	else if(semanticFunction == "factor2"){abstractSyntaxTree::factor2Node*newnode = new abstractSyntaxTree::factor2Node();std::vector<std::string> v = {"not","sign","factor"}; semanticHandler.makeSubTree("factor2","factor2",2,v,newnode);}
	else if(semanticFunction == "selfandidnest2"){abstractSyntaxTree::selfandidnest2Node*newnode = new abstractSyntaxTree::selfandidnest2Node();std::vector<std::string> v = {"self","idnest2"}; semanticHandler.makeSubTree("selfandidnest2","selfandidnest2",2,v,newnode);}
	else if(semanticFunction == "reptvariable2"){abstractSyntaxTree::reptvariable2Node*newnode = new abstractSyntaxTree::reptvariable2Node();std::vector<std::string> v = {"epsilon"}; semanticHandler.makeSubTree("reptvariable2","reptvariable2",'E',v,newnode);}
	else if(semanticFunction == "aparamsandid"){abstractSyntaxTree::aparamsandidNode*newnode = new abstractSyntaxTree::aparamsandidNode();std::vector<std::string> v = {"id","aparams"}; semanticHandler.makeSubTree("aparamsandid","functioncall",2,v,newnode);}
	else if(semanticFunction == "reptvariable2andid"){abstractSyntaxTree::reptvariable2andidNode*newnode = new abstractSyntaxTree::reptvariable2andidNode();std::vector<std::string> v = {"id","reptvariable2"}; semanticHandler.makeSubTree("reptvariable2andid","term",2,v,newnode);}
	else if(semanticFunction == "reptfuncbody1"){abstractSyntaxTree::reptfuncbody1Node*newnode = new abstractSyntaxTree::reptfuncbody1Node();std::vector<std::string> v = {"epsilon"}; semanticHandler.makeSubTree("reptfuncbody1","funcbody",'E',v,newnode);}
	else if(semanticFunction == "funchead"){abstractSyntaxTree::funcheadNode*newnode = new abstractSyntaxTree::funcheadNode();std::vector<std::string> v = {"epsilon"}; semanticHandler.makeSubTree("funchead","funchead",'E',v,newnode);}
	else if(semanticFunction == "reptimpldef3"){abstractSyntaxTree::reptimpldef3Node*newnode = new abstractSyntaxTree::reptimpldef3Node();std::vector<std::string> v = {"epsilon"}; semanticHandler.makeSubTree("reptimpldef3","reptimpldef3",'E',v,newnode);}
	else if(semanticFunction == "impldef"){abstractSyntaxTree::impldefNode*newnode = new abstractSyntaxTree::impldefNode();std::vector<std::string> v = {"id","reptimpldef3"}; semanticHandler.makeSubTree("impldef","impldef",2,v,newnode);}
	else if(semanticFunction == "reptoptclassdecl22"){abstractSyntaxTree::reptoptclassdecl22Node*newnode = new abstractSyntaxTree::reptoptclassdecl22Node();std::vector<std::string> v = {"epsilon"}; semanticHandler.makeSubTree("reptoptclassdecl22","reptoptclassdecl22",'E',v,newnode);}
	else if(semanticFunction == "optclassdecl2"){abstractSyntaxTree::optclassdecl2Node*newnode = new abstractSyntaxTree::optclassdecl2Node();std::vector<std::string> v = {"epsilon"}; semanticHandler.makeSubTree("optclassdecl2","isalist",'E',v,newnode);}
	else if(semanticFunction == "reptprog0"){abstractSyntaxTree::reptprog0Node*newnode = new abstractSyntaxTree::reptprog0Node();std::vector<std::string> v = {"epsilon"}; semanticHandler.makeSubTree("reptprog0","prog",'E',v,newnode);}
	else if(semanticFunction == "relexpr"){abstractSyntaxTree::relexprNode*newnode = new abstractSyntaxTree::relexprNode();std::vector<std::string> v = {"arithexpr","relop","arithexpr"}; semanticHandler.makeSubTree("relexpr","relexpr",3,v,newnode);}
	else if(semanticFunction == "funcdeclfam"){abstractSyntaxTree::funcdeclfamNode*newnode = new abstractSyntaxTree::funcdeclfamNode();std::vector<std::string> v = {"visibility","funcdecl"}; semanticHandler.makeSubTree("funcdecl","funcdecl",2,v,newnode);}
	else if(semanticFunction == "attributedeclfam"){abstractSyntaxTree::attributedeclfamNode*newnode = new abstractSyntaxTree::attributedeclfamNode();std::vector<std::string> v = {"visibility","attributedecl"}; semanticHandler.makeSubTree("attributedecl","attributedecl",2,v,newnode);}
	else if(semanticFunction == "reptstatblock1"){abstractSyntaxTree::reptstatblock1Node*newnode = new abstractSyntaxTree::reptstatblock1Node();std::vector<std::string> v = {"epsilon"}; semanticHandler.makeSubTree("reptstatblock1","reptstatblock1",'E',v,newnode);}
	else if(semanticFunction == "vardecl"){abstractSyntaxTree::vardeclNode*newnode = new abstractSyntaxTree::vardeclNode();std::vector<std::string> v = {"id","type","reptvardecl3"}; semanticHandler.makeSubTree("vardecl","vardecl",3,v,newnode);}
	else if(semanticFunction == "reptvardecl3"){abstractSyntaxTree::reptvardecl3Node*newnode = new abstractSyntaxTree::reptvardecl3Node();std::vector<std::string> v = {"epsilon"}; semanticHandler.makeSubTree("reptvardecl3","dimlist",'E',v,newnode);}


	else if(semanticFunction == "dot"){abstractSyntaxTree::dotNode*newnode = new abstractSyntaxTree::dotNode();semanticHandler.makeBinarySubTree("dot",0,newnode);}
	else if(semanticFunction == "rightrecarithexpr"){abstractSyntaxTree::rightrecarithexprNode*newnode = new abstractSyntaxTree::rightrecarithexprNode();semanticHandler.makeBinarySubTree("rightrecarithexpr",1,newnode);}
	else if(semanticFunction == "rightrecterm"){abstractSyntaxTree::rightrectermNode*newnode = new abstractSyntaxTree::rightrectermNode();semanticHandler.makeBinarySubTree("rightrecterm",1,newnode);}
	else if(semanticFunction == "reptstatement4"){abstractSyntaxTree::reptstatement4Node*newnode = new abstractSyntaxTree::reptstatement4Node();semanticHandler.makeBinarySubTree("reptstatement4",1,newnode);}


	else if(semanticFunction == "whilestatement"){abstractSyntaxTree::whilestatementNode*newnode = new abstractSyntaxTree::whilestatementNode();semanticHandler.makeBinarySubTreeWithHead("whilestatement",3,newnode);}
	else if(semanticFunction == "readstatement"){abstractSyntaxTree::readstatementNode*newnode = new abstractSyntaxTree::readstatementNode();semanticHandler.makeBinarySubTreeWithHead("readstatement",2,newnode);}
	else if(semanticFunction == "writestatement"){abstractSyntaxTree::writestatementNode*newnode = new abstractSyntaxTree::writestatementNode();semanticHandler.makeBinarySubTreeWithHead("writestatement",2,newnode);}
	else if(semanticFunction == "freturnstatement"){abstractSyntaxTree::freturnstatementNode*newnode = new abstractSyntaxTree::freturnstatementNode();semanticHandler.makeBinarySubTreeWithHead("freturnstatement",2,newnode);}
	else if(semanticFunction == "ifstatement"){abstractSyntaxTree::ifstatementNode*newnode = new abstractSyntaxTree::ifstatementNode();semanticHandler.makeBinarySubTreeWithHead("ifstatement",4,newnode);}


}

void parser::semanticActions:: makeLeaf(std::string nodeType,std::string lastTokenValue,abstractSyntaxTree::node * newnode){
	newnode->nodeType = nodeType;
	newnode->semanticMeaning = nodeType;
	newnode->nodeValue = lastTokenValue;
	newnode->isLeaf = true;

	semanticStack.push(newnode);
}

void parser::semanticActions::makeBinarySubTreeWithHead(std::string nodeType,int numOfPops,abstractSyntaxTree::node* newnode){
	std::vector <abstractSyntaxTree::node *>nodesOnStack;
	std::vector <abstractSyntaxTree::node *>children;

	for(int i = numOfPops -1; i>=0; i--){
		nodesOnStack.emplace_back(semanticStack.top());
		semanticStack.pop();
	}
	abstractSyntaxTree::node * parentNode = nodesOnStack[nodesOnStack.size()-1];
	newnode->copyNode(parentNode, newnode, nodeType);
	delete parentNode;

	for(int k = nodesOnStack.size()-2; k>=0; k--){
		children.emplace_back(nodesOnStack.at(k));
	}
	for(int i = 0; i<children.size();i++){
		if(i ==0){
			if(i + 1<children.size()){
				children.at(i)->headOfSibling = children.at(i);
				children.at(i)->rightSibling = children.at(i+1);
			}
		}
		else if(i==children.size()-1){
			if(i -1 > -1 ){
				children.at(i)->headOfSibling = children.at(0);
				children.at(i)->leftSibling = children.at(i-1);
			}
		}
		else{
			children.at(i)->headOfSibling = children.at(0);
			children.at(i)->rightSibling = children.at(i+1);
			children.at(i)->leftSibling = children.at(i-1);

		}

	}
	for(abstractSyntaxTree::node * value : children){
		value->parent = newnode;
	}
	newnode->children = children;
	semanticStack.push(newnode);


}

void parser::semanticActions::makeBinarySubTree(std::string nodeType,int i,abstractSyntaxTree::node*newnode){
	if(i ==0){
		abstractSyntaxTree::node * first = semanticStack.top();
		semanticStack.pop();
		abstractSyntaxTree::node * second = semanticStack.top();
		semanticStack.pop();

		std::vector<abstractSyntaxTree::node* > v = {first,second};
		newnode->children=v;
		newnode->nodeType=nodeType;
		newnode->semanticMeaning=nodeType;

		first->headOfSibling = first;
		first->leftSibling = nullptr;
		first->rightSibling=second;
		first->parent = newnode;
		second ->headOfSibling = first;
		second ->leftSibling = first;
		second -> rightSibling = nullptr;
		second ->parent = newnode;
		newnode ->children =v;
		semanticStack.push(newnode);

	}
	else if(i == 1){
		abstractSyntaxTree::node * first = semanticStack.top();
		semanticStack.pop();
		abstractSyntaxTree::node * second = semanticStack.top();
		semanticStack.pop();
		abstractSyntaxTree::node * third  = semanticStack.top();
		semanticStack.pop();



		std::vector<abstractSyntaxTree::node* > v = {first,third};
		newnode->nodeType=nodeType;
		newnode->semanticMeaning=nodeType;
		newnode->copyNode(second, newnode, nodeType);
		newnode->children=v;
		delete second;
		first->headOfSibling = first;
		first->leftSibling = nullptr;
		first->rightSibling=third;
		first->parent = newnode;
		third ->headOfSibling = first;
		third ->leftSibling = first;
		third -> rightSibling = nullptr;
		third ->parent = newnode;
		semanticStack.push(newnode);

	}

}

void parser::semanticActions:: makeSubTree(std::string nodeType, std::string semanticMeaning,int amountOfPops,std::vector<std::string> match, abstractSyntaxTree::node* newnode){
	std::vector<abstractSyntaxTree::node *> nodesFromTheStack;
	if(amountOfPops == 'E'){
		abstractSyntaxTree::node * n = semanticStack.top();
		while(n->nodeType!="epsilon"){
			nodesFromTheStack.emplace_back(n);
			semanticStack.pop();
			n = semanticStack.top();
		}
		semanticStack.pop();


	}
	//First if statement handles if the amount that we are about to pop matches the amount we need to match.
	else if(amountOfPops == match.size()){
		for(int i = amountOfPops-1; i>=0;i--){
			abstractSyntaxTree::node * n = semanticStack.top();
			if(n->nodeType==match.at(i)){
				nodesFromTheStack.emplace_back(n);
				semanticStack.pop();
			}
			else{
				std::cout<<"Mistake matching in " + nodeType + "with : " + match.at(i);
				exit(1);
			}
		}
	}
	//If it does not match then we need to match just one of them
	else{
		for(int i = 0; i<amountOfPops;i++){
			abstractSyntaxTree::node * n = semanticStack.top();
			int k = 0;
			for(; k<match.size(); k++){
				if(n->nodeType==match.at(k)){
					nodesFromTheStack.emplace_back(n);
					semanticStack.pop();
					break;
				}
				else{
					continue;
				}
			}

			if(k == match.size()){
				std::cout<<"problem with stack cant find the matching items  : " + nodeType;
				exit(1);
			}
		}
	}


	for(int i = 0; i<nodesFromTheStack.size();i++){
		if(i ==0){
			if(i + 1<nodesFromTheStack.size()){
				nodesFromTheStack.at(i)->headOfSibling = nodesFromTheStack.at(i);
				nodesFromTheStack.at(i)->rightSibling = nodesFromTheStack.at(i+1);
			}
		}
		else if(i==nodesFromTheStack.size()-1){
			if(i -1 > -1 ){
				nodesFromTheStack.at(i)->headOfSibling = nodesFromTheStack.at(0);
				nodesFromTheStack.at(i)->leftSibling = nodesFromTheStack.at(i-1);
			}
		}
		else{
			nodesFromTheStack.at(i)->headOfSibling = nodesFromTheStack.at(0);
			nodesFromTheStack.at(i)->rightSibling = nodesFromTheStack.at(i+1);
			nodesFromTheStack.at(i)->leftSibling = nodesFromTheStack.at(i-1);

		}

	}


	newnode->children = nodesFromTheStack;
	newnode->nodeType = nodeType;
	newnode->semanticMeaning = semanticMeaning;

	for(abstractSyntaxTree::node * value : nodesFromTheStack){
		value->parent = newnode;
	}

	if(nodesFromTheStack.size()==0){
		newnode->semanticMeaning="epsilon";
		}
	semanticStack.push(newnode);
}

void parser::abstractSyntaxTree::node::copyNode(node* oldnode,node* newnode, std::string nameOfNewNode){
	newnode ->headOfSibling = oldnode ->headOfSibling;
	newnode->leftSibling = oldnode->leftSibling;
	newnode->rightSibling = oldnode->rightSibling;
	newnode->parent = oldnode->rightSibling;
	newnode->children = oldnode->children;
	newnode->nodeType = nameOfNewNode;
	newnode->semanticMeaning = oldnode ->semanticMeaning;
	newnode->nodeValue = oldnode->nodeValue;
}

void parser::semanticActions:: passAlong(std::string nodeType,abstractSyntaxTree::node * newnode){
	abstractSyntaxTree::node * topofthestack = semanticStack.top();
	semanticStack.pop();
	newnode->copyNode(topofthestack,newnode,nodeType);
	adoptChildren(newnode,topofthestack);

	if(topofthestack->nodeType =="epsilon" || topofthestack->semanticMeaning == "epsilon"){
		newnode->semanticMeaning ="epsilon";
	}
	delete topofthestack;
	if(nodeType == "start"){AST.treeHead = newnode;}
	else {semanticStack.push(newnode);}

}

void parser::semanticActions:: passAlong(std::string nodeType,std::string semanticMeaning,abstractSyntaxTree::node * newnode){
	abstractSyntaxTree::node * topofthestack = semanticStack.top();
	semanticStack.pop();
	newnode->copyNode(topofthestack,newnode,nodeType);
	newnode->semanticMeaning = semanticMeaning;
	adoptChildren(newnode,topofthestack);
	if(topofthestack->nodeType =="epsilon" || topofthestack->semanticMeaning == "epsilon"){
		newnode->semanticMeaning ="epsilon";
	}
	delete topofthestack;
	if(nodeType == "start"){
		AST.treeHead = newnode;

	}
	else {
		semanticStack.push(newnode);
	}

}

void parser::semanticActions::adoptChildren(abstractSyntaxTree::node* newparent,abstractSyntaxTree::node* oldparent){
	if(oldparent->children.size()==0){
		newparent->isLeaf = true;
		return;
	}

	else if(oldparent->children.size()>0){
		for(abstractSyntaxTree::node* child : oldparent->children){
			child->parent = newparent;
		}
	}
}

void parser::abstractSyntaxTree::printTree(){
    std::ofstream astout("example1.ast.outast"); // Open file for writing
	abstractSyntaxTree::node * head = AST.treeHead;
	std::vector<abstractSyntaxTree::node*> v = head->children;
	int counter = 0;
	astout<<head->nodeType<<std::endl;
	for(int k = v.size()-1;k>=0;k--){
		traverseTree(v.at(k),counter,astout);
	}
}

void parser::abstractSyntaxTree::traverseTree(abstractSyntaxTree::node* head, int& counter,std::ofstream & ao){
	counter++;
	if(head->children.size()!= 0){
		for(int i = 0; i<counter; i++){
			ao<<"| ";
		}
		if(head->semanticMeaning !="epsilon")ao<<head->nodeType<<std::endl;
		std::vector<abstractSyntaxTree::node*> v = head->children;
		for(int k = v.size()-1;k>=0;k--){
			traverseTree(v.at(k),counter,ao);
		}
		counter--;
	}
	else if(head->children.size() == 0){
		if(head->semanticMeaning !="epsilon"){


		for(int i = 0; i<counter; i++){
					ao<<"| ";
				}
		ao<<head->nodeType<<std::endl;
		std::cout << typeid(*head).name() << std::endl; // Dereference to get dynamic type

		}
		counter--;
	}

}


