#include "lexor.h"

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
const char lexor:: letterArray[] ={'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};
const char lexor:: alphanumArray[] = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z', 'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z','0','1','2','3','4','5','6','7','8','9','_'};
std::ifstream lexor::inputFileStream;
bool lexor::virgin = true;
int lexor:: line =0;
int lexor::column = 0;
char lexor::currentCharacter = 'z';

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

bool lexor::virginProtocol(){
	//Get the file location from the user since it is the first time he is using it given we are in the virgin protocol().
	std::string fileLocation;
	std::cout<<"File Location:";
	std::cin>>fileLocation;
	//File Location acquired, check if we can properly connect.
	if(connectFile(fileLocation))
	{
		//We've properly connected to the file now we need to get rid of the white space and then hand control back over getNextToken().
		//Get rid of white spaces
		//set line and column to 0
		line = 0;
		column = 0;
		currentCharacter = inputFileStream.get();
		getRidOfWhiteSpace();
		return true;
	}
	else return false;
}

token* lexor:: getNextToken(){
	//Check if we've run this file before. IF we have not then we begin the virgin protocol
	//Virgin protocol is used to get the establish file connection.
	//If the virginf protocol fails then we've already established a connection to the file.
	if(virgin == true){
		//Virgin set to false so that we never enter this code again.
		virgin = false;
		//If the virgin protocol fails, then we were not able to establish a connection to the file. The program should close
		if(!virginProtocol())
	        throw std::invalid_argument("Invalid File location");
	}

}

token::token(std::string type, std::string lexeme, int line, int column){
	this->typeName = type;
	this->lexeme = lexeme;
	this->line = line;
	this->column = column;
}





