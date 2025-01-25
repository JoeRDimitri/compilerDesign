#include "lexor.h"


std::ostream& operator<<(std::ostream& os, token& t)
{
	os<<"["<<t.getTypeName()<<","<<t.getLexeme()<<","<<t.getLine()<<","<<t.getColumn()<<"]";
	return os;
}

const std::unordered_map<std::string, std::string> lexor::tokenMap = {
    {"==","eq"},
    {"+","plus"},
    {"or","or"},
    {"(","openpar"},
	{";","semi"},
	{"int","int"},
	{"while","while"},
	{"local","local"},
	{"<>", "noteq"},
	{"-","minus"},
	{"and","and"},
	{")","closepar"},
	{",","comma"},
	{"float","float"},
	{"if","if"},
	{"constructor","constructor"},
	{"<","lt"},
	{"*","mult"},
	{"not","not"},
	{"{","opencubr"},
	{".","dot"},
	{"void","void"},
	{"then","then"},
	{"attribute","attribute"},
	{">","gt"},
	{"/","div"},
	{"}","closecubr"},
	{":","colon"},
	{"class","class"},
	{"else","else"},
	{"function","function"},
	{"<=","leq"},
	{":=","assign"},
	{"[","opensqbr"},
	{"=>","arrow"},
	{"self","self"},
	{"read","read"},
	{"public","public"},
	{">=","geq"},
	{"]","closeqbr"},
	{"isa","isa"},
	{"write","write"},
	{"private","private"},
	{"implementation","implementation"},
	{"return","return"}
};
const char lexor:: intArray [] = {'0','1','2','3','4','5','6','7','8','9'};
const char lexor:: nonZeroArray[] = {'1','2','3','4','5','6','7','8','9'};
const char lexor:: letterArray[] ={'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z','_'};
const char lexor:: alphanumArray[] = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z', 'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z','0','1','2','3','4','5','6','7','8','9','_'};
std::ifstream lexor::inputFileStream;
bool lexor::virgin = true;
int lexor:: line =0;
int lexor::column = 0;
char lexor::currentCharacter;
std::string lexor::currentLexeme;
std::string lexor::possibleType;
std::string errorHandler::fileName;
errorHandler lexor:: handler;



bool lexor::connectFile(std::string fileName){

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

bool lexor::disconnectFile(){
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

void lexor::getRidOfWhiteSpace(){
	//Function that moves the input stream forward if it reads white space. white space values are 9-10-11-12-13-32
	//Will enter loop if the current character is a whitespace
	while((currentCharacter >= 9 && currentCharacter <=13) || currentCharacter == 32)
	{
		//Increase the column where the next character will be.
		column++;
		//If we go to the next line then we increase the line variable and set the column to 0
		if (currentCharacter == 10)
		{
			line ++;
			column = 0;
		}
		//Move the Input File pointer to the next character
		currentCharacter = inputFileStream.get();
	}
}

bool lexor::isWhiteSpace(){
	if((currentCharacter >= 9 && currentCharacter <=13)||currentCharacter==32)
		return true;
	else return false;
}

bool lexor::isInArray(const char * c, int n){
	for (int i = 0; i<n;i++){
		if(currentCharacter == c[i]) return true;
	}
	return false;

}

bool lexor::isReservedWord(std::string s){
	if(tokenMap.count(s)>0) return true;
	else return false;
}

bool lexor::isReservedWord(){
	std::string charAsString {currentCharacter};
	if(tokenMap.count(charAsString)>0||currentCharacter == '=') return true;
	else return false;
}

void lexor::setPossibleType(){
	std::string charAsString {currentCharacter};
	if(isInArray(letterArray,53))possibleType = "id";
	else if(isInArray(intArray,10))possibleType = "num";
	else if(tokenMap.count(charAsString)>0||currentCharacter == '=') possibleType ="res";
	else if(currentCharacter == '/')possibleType="cmt";

}

bool lexor::virginProtocol(){
	//Get the file location from the user since it is the first time he is using it given we are in the virgin protocol().
	std::string fileLocation;
	std::cout<<"File Location:";
	std::cin>>fileLocation;
	//File Location acquired, check if we can properly connect.
	if(connectFile(fileLocation))
	{
		handler.setFileName(fileLocation);

		//We've properly connected to the file now we need to get rid of the white space and then hand control back over getNextToken().
		return true;
	}
	else return false;
}

void lexor::addAndMove(){
	currentLexeme+=currentCharacter;
	currentCharacter = inputFileStream.get();
	column++;
}

token* lexor::id(){
	//lexeme is still empty at this point
	//Invalid Id case
	if(currentCharacter == '_'){
		while(!isWhiteSpace())addAndMove();
		std::string tempLexeme = currentLexeme;
		currentLexeme = "";
		handler.handleError("Lexical error", "Invalid Identifier",tempLexeme, line, column);
		return new token("invalidid",tempLexeme,line,column);
	}
	//We add our first character
	addAndMove();

	//While the current character is an alpha numeric value keep adding this to the current lexeme.
	while(isInArray(alphanumArray,63)){
		addAndMove();
	}
	/*There are numerous resons why we might be here.
	 * 1. We might've reached a whitespace. In that case we can simply return from this function since we have found a proper lexeme
	 * 2. We might've reached a special character, in that case we can simply return from thins functino since we have found a proper lexeme
	 * 3. else*/

	if(isWhiteSpace()){}
	else if(isReservedWord()){}
	else{
		while(!isWhiteSpace())addAndMove();
	}

	std::string tempLexeme = currentLexeme;
	currentLexeme = "";
	if(isReservedWord(tempLexeme)) return new token(tokenMap.at(tempLexeme),tempLexeme,line,column);
	else return new token("id",tempLexeme,line,column);


}

token* lexor::num(){
	//lexeme is still empty at this point
	//Check if it is 0
	if(currentCharacter == '0'){
		//We add our first character
		addAndMove();
		/*We need to make sure that the next character is either:
		 *1. A white space. in which case we are done successfully
		 *2. a '.' in which case we go to the float function.
		 *3. anything else is an invalid num
		 */
	}



}

token* lexor::res(){}

token* lexor::cmt(){}

token* lexor::invalidChar(){}

token* lexor:: getNextToken(){
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
			//Get rid of white spaces
			//set line and column to 0
			line = 0;
			column = 0;
			currentCharacter = inputFileStream.get();
			currentLexeme = "";
		}
	}
	//Get rid of all white space.
	getRidOfWhiteSpace();

	//We've set the possible type for the first character of our brand new lexeme.
	setPossibleType();

	//Check what the currentCharacter and go into the corresponding function
	if(possibleType == "id")return id();
	else if(possibleType == "num")return num();
	else if(possibleType == "res")return res();
	else if(possibleType =="cmt")return cmt();
	else return invalidChar();
}

token::token(std::string type, std::string lexeme, int line, int column){
	this->typeName = type;
	this->lexeme = lexeme;
	this->line = line;
	this->column = column;
}

std::vector<std::string> errorHandler:: splitString(const std::string& str, char delimiter) {
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

void errorHandler::setFileName(std::string s){
	std::vector<std::string> result = splitString(s,'/');
	std::string filenamewithperiod = result.back();
	std::string filename = filenamewithperiod.substr(0,filenamewithperiod.find("."));
	fileName = filename+".outlexerrors";
}

void errorHandler::handleError(const std::string& errorType,const std::string & invalidType, const std::string& lexeme, const int & line, const int & column) {

    // Open the file in append mode to write (creates the file if it doesn't exist)
    std::ofstream file(fileName, std::ios::app);

    // Write content to the file
    file << errorType << ":"<<invalidType<<"\""<<lexeme<<"\""<<": line "<<line<<": column "<<column<<std::endl;

    // Close the file
    file.close();
}


