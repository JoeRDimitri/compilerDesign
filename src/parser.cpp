#include "parser.h"

std::ifstream parser::inputFileStream;
char parser::currentCharacter;
bool parser::virgin = true;
std::unordered_map<std::string, std::vector<std::string>> parser::firstSet;
std::string parser::line;
int parser::lineIndex;


bool parser::connectFile(std::string fileName){

	if(inputFileStream.is_open()){
			std::cout<<"The file is connected... Closing it first..."<<std::endl;
			inputFileStream.close();
		}

	inputFileStream.open(fileName);

	if(inputFileStream.is_open()){
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
	if(connectFile(fileLocation)){
		//We've properly connected to the file now we need to get rid of the white space and then hand control back over getNextToken().
		return true;
	}
	else return false;
}

void parser::getRidOfWhiteSpace(){
	//Function that moves the input stream forward if it reads white space. white space values are 9-10-11-12-13-32
	//Will enter loop if the current character is a whitespace
	while((line.at(lineIndex) == 9 || line.at(lineIndex) == 11|| line.at(lineIndex) == 32)){
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

	line ="";
	lineIndex = 0;

	//we should get line until the end of the file.
	while (std::getline(inputFileStream, line))
	{
	    std::cout << line << std::endl;
	    updateCurrentCharacter();
	    //We get the first set and store it in a temp variable.
	    findNT();

	    //We have the NT. Now we move passed the assignment operator.
		if(checkForAssignment()){
			//If we are here it is becaue we've passed the assignment operator.
			//We've gotten rid of the white space
			//We check if the first symbol after the assignment symbol is a terminal.
			if(checkT()){

			}
			else if(checkNT()){

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
			lineIndex = 0;
		}


	}

	//Now we have line variable. We will need to parse line
	// In the line we need to find the first occurence of <
}


bool parser::checkT(){
	if(line.at(lineIndex)=='\''){return true;}
	return false;
}

bool parser::checkNT(){
	if(line.at(lineIndex)=='<'){return true;}
	return false;
}

std::vector* parser::findNT(){
	while(line.at(lineIndex) != '<'){
		lineIndex++;
	}

	lineIndex++;

	//We build the word
	while(line.at(lineIndex) != '>'){
		currentWord += line.at(lineIndex);
		lineIndex++;
	}

	getRidOfWhiteSpace();


	//need to check if this nonterminal already exists.
	if(firstSet.count(currentWord) == 0){
		//it does not exist, then we need to create that entry in the dictionary
		firstSet.emplace(currentWord, new std::vector());
	}

	return firstSet.at(currentWord);

}

bool parser::checkForAssignment(){
	int tempIndex = lineIndex;
	if(line.at(tempIndex)==':'){
		tempIndex++;
		if(line.at(tempIndex)==':'){
			tempIndex++;
			if(line.at(tempIndex)=='='){
				lineIndex = tempIndex;
				getRidOfWhiteSpace();
				return true;
			}
		}
	}
	return false;
}


void parser::updateCurrentCharacter(){
	currentCharacter = line.at(lineIndex);
}
