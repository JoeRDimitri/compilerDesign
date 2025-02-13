#include "parser.h"
std::unordered_map<std::string, std::vector<std::string>*> parser::firstSet;
std::ifstream parser::inputFileStream;
std::string parser::currentWord;
std::string parser::currentSymbol;
std::string parser::line;
bool parser::virgin = true;
bool parser::change = true;
int parser::lineIndex;


bool parser::connectFile(std::string fileName){

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

bool parser::disconnectFile(){
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

bool parser::virginProtocol(){
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

void parser::getRidOfWhiteSpace(){
	//Function that moves the input stream forward if it reads white space. white space values are 9-10-11-12-13-32
	//Will enter loop if the current character is a whitespace
	while(lineIndex<line.size()&&(line.at(lineIndex) == 9 || line.at(lineIndex) == 11|| line.at(lineIndex) == 32)){
		//Move the Input File pointer to the next character
		lineIndex++;
	}
}


void parser::generateFirstSet(){

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
			std::cout << line << std::endl;
			lineIndex = 0;
			currentSymbol = "";
			currentWord = "";


			getRidOfWhiteSpace();

			if(lineIndex == line.size())
				continue;

			//We get the first set and store it in a temp variable.
			std::vector<std::string> * firstSetPtr = findNT();

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

void parser:: compareSymbolVector(bool & change,std::vector<std::string>* v){
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

void parser::checkTheNextSymbol(bool & eps, std::vector<std::string>*v){
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

void parser:: compareSymbolVectorRec(bool & eps,std::vector<std::string>* v){
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


void parser:: addAndMove(bool& change, std::vector<std::string> * v, std::string s){
	v->emplace_back(s);
	change = true;
	std::cout<<"Adding: "<<s<<" to the first Set of : "<<currentWord<<std::endl;
}

bool parser::checkE(){
	if(line.at(lineIndex)=='E'){
		return true;
	}
	return false;

}

void parser::getSymbol(std::string s){
	if(s == "t"){while(line.at(lineIndex)!= '\''){
		currentSymbol += line.at(lineIndex);
		lineIndex++;}
	}
	else if(s=="nt"){while(line.at(lineIndex)!= '>'){
		currentSymbol += line.at(lineIndex);
		lineIndex++;}
	}
	else if(s == "e"){while(line.at(lineIndex)!=' '){
		currentSymbol += line.at(lineIndex);
		lineIndex++;}
	}
	lineIndex++;
	getRidOfWhiteSpace();
}

bool parser::inVector(std::vector<std::string> * firstSet,std::string s ){
	for (std::string value : (*firstSet)) {
		if(value == s)
			return true;
	}
	return false;
}

bool parser::checkT(){
	if(line.at(lineIndex)=='\''){
		lineIndex++;
		return true;
	}
	return false;
}

bool parser::checkNT(){
	if(line.at(lineIndex)=='<'){
		lineIndex ++;
		return true;
	}
	return false;
}

std::vector<std::string> * parser::findNT(){
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
    if (firstSet.count(currentWord) == 0) {
        firstSet[currentWord] = new std::vector<std::string>();  // Allocate memory
    }

    return firstSet.at(currentWord);  // Dereference the pointer
}

bool parser::checkForAssignment(){
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
