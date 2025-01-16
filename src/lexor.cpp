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
const std::string intArray [10] = {"0","1","2","3","4","5","6","7","8","9"};
const std::string nonZeroArray[9] = {"1","2","3","4","5","6","7","8","9"};
std::ifstream lexor::inputFileStream;

bool lexor::connectFile(std::string fileName){

	if(inputFileStream.is_open()){
			std::cout<<"The file is even connected... Closing it first..."<<std::endl;
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

token::token(std::string type, std::string lexeme, int line, int column){
	this->typeName = type;
	this->lexeme = lexeme;
	this->line = line;
	this->column = column;
}





