#include "handler.h"

std::ifstream fileHandler::inputFileStream;
bool fileHandler::virgin = true;

bool fileHandler::virginProtocol(){
	std::string fileLocation;
	std::cout<<"File Location:";
	//Get the file location from the user since it is the first time he is using it given we are in the virgin protocol().
	std::cin>>fileLocation;

	//File Location acquired, check if we can properly c\onnect.
	if(connectFile(fileLocation))
	{
		setFileName(fileLocation);
		//We've properly connected to the file now we need to get rid of the white space and then hand control back over getNextToken().
		return true;
	}
	else return false;
}

bool fileHandler::connectFile(std::string fileName){
	if(inputFileStream.is_open()){
		spdlog::info("The file is connected... Closing it first...");     // info level
		inputFileStream.close();
	}

	inputFileStream.open(fileName);

	if(inputFileStream.is_open()){
		spdlog::info("Successfully opened file: {}.", fileName);     // info level
		return true;
	}
	else{
		spdlog::info("Failed to open file: {}.", fileName);     // info level
		return false;
	}
}

bool fileHandler::disconnectFile(){
	if(!inputFileStream.is_open()){
			spdlog::info("The file is not even connected. No need to close.");     // info level
			return true;
		}
	inputFileStream.close();

	if(!inputFileStream.is_open()){
		spdlog::info("Successfully closed the inputFileStream.");     // info level

		return true;
	}
	else{
		spdlog::info("Failed to close the inputFileStream.");     // info level
		return false;
	}
}

bool fileHandler::checkEndOfStream(){
	return inputFileStream.eof();
}


void fileHandler::setFileName(std::string s){
	std::vector<std::string> result = splitString(s,'/');
	std::string filenamewithperiod = result.back();
	std::string tokenfilename = filenamewithperiod.substr(0,filenamewithperiod.find("."));
	std::string errorfilename = filenamewithperiod.substr(0,filenamewithperiod.find("."));
	errorHandler::errorFileName = errorfilename+".outlexerrors";
	tokenFileName = tokenfilename+".outlextokens";
    std::ofstream file(errorHandler::errorFileName, std::ios::app);
    file.close();
    std::ofstream file2(tokenFileName, std::ios::app);
    file2.close();
}
void errorHandler::handleError(const std::string& errorType,const std::string & invalidType, const std::string& lexeme, const int & line, const int & column, token * t) {

    // Open the file in append mode to write (creates the file if it doesn't exist)
    std::ofstream file1(errorFileName, std::ios::app);
    std::ofstream file2(fileHandler::tokenFileName, std::ios::app);
    spdlog::warn("Warning: lexeme '{}' is problematic. The compiler believes it is of type {}, but does not conform.", lexeme,invalidType);        // warn level
    // Write error content to the file
    file1 << errorType << ": "<<invalidType<<"\": "<<lexeme<<"\""<<": line "<<line<<": column "<<column<<std::endl;
    //write error token to the token file
    file2<<(*t)<<std::endl;

    // Close the file
    file1.close();
    file2.close();
}

std::vector<std::string> fileHandler::splitString(const std::string& str, char delimiter) {
    std::vector<std::string> result;
    size_t start = 0;
    size_t end = str.find(delimiter);

    while (end != std::string::npos) {
        result.push_back(str.substr(start, end - start));
        start = end + 1;  // Move past the delimiter
        end = str.find(delimiter, start);
    }
    result.push_back(str.substr(start));  // Add the last token
    return result;
}

void fileHandler::writeToken(token* t){
    std::ofstream file(tokenFileName, std::ios::app);
    file<<(*t)<<std::endl;
    file.close();
}

