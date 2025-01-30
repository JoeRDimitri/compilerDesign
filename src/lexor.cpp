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
std::string errorHandler::errorFileName;
std::string errorHandler::tokenFileName;

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
bool lexor::checkEndOfStream(){
	return inputFileStream.eof();
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
	std::cout<<tokenMap.count(charAsString)<<std::endl;
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

int lexor::addAndMove(std::string & templexeme){
	int tempLineCounter = 0;
	if(currentCharacter=='\r'){templexeme += "\\n";column = 0;}
	else if(currentCharacter == '\n'){tempLineCounter++;}
	else templexeme+=currentCharacter;
	currentCharacter = inputFileStream.get();
	column++;
	return tempLineCounter;
}


token* lexor::errorProtocol(std::string type){

	while(!isWhiteSpace()&&!isReservedWord()&&currentCharacter != '=')addAndMove();
	std::string tempLexeme = currentLexeme;
	currentLexeme = "";

	if(type.compare("id")==0){
		handler.handleError("Lexical error", "Invalid Identifier",tempLexeme, line, column);
		return new token("invalidid",tempLexeme,line,column);
	}
	else if(type.compare("intnum")==0){
		handler.handleError("Lexical error", "Invalid Number",tempLexeme, line, column);
		return new token("invalidnum",tempLexeme,line,column);
	}
	else if(type.compare("frac")==0){
		handler.handleError("Lexical error", "Invalid Number",tempLexeme, line, column);
		return new token("invalidnum",tempLexeme,line,column);
	}
}

token* lexor::validToken(std::string type, int lineCounter){

	std::string tempLexeme = currentLexeme;
	currentLexeme = "";

	if(type.compare("id")==0){
		if(isReservedWord(tempLexeme)) {
			token* t = new token(tokenMap.at(tempLexeme),tempLexeme,line,column);
			handler.writeToken(t);
			return t;
		}
		else{
			token* t = new token("id",tempLexeme,line,column);
			handler.writeToken(t);
			return t;
		}
	}
	else if (type.compare("intnum")==0){
		token* t = new token("intnum",tempLexeme,line,column);
		handler.writeToken(t);
		return t;
	}
	else if (type.compare("frac")==0){
		token* t = new token("floatnum",tempLexeme,line,column);
		handler.writeToken(t);
		return t;
	}
	else if(type.compare("res")==0){
		std::string value = tokenMap.at(tempLexeme);
		token* t =  new token(value,tempLexeme,line,column);
		handler.writeToken(t);
		return t;
	}
	else if(type.compare("linecomment")==0){
		getLine(tempLexeme);
		token* t = new token("linecomment",tempLexeme,line, column);
		handler.writeToken(t);
		column = 0;
		return t;
	}

	else if(type.compare("blockcomment")==0){
		token* t = new token("blockcmt",tempLexeme,line,column);
		handler.writeToken(t);
		line+=lineCounter;
		return t;
	}
}

void lexor::getLine(std::string &tempLexeme){
	while(currentCharacter != '\r' && currentCharacter!='\n'&&!inputFileStream.eof()){
		addAndMove(tempLexeme);
	}
}


token* lexor::id(){

	//lexeme is still empty at this point
	//Invalid Id case
	if(currentCharacter == '_'){return errorProtocol("id");}

	//We add our first character
	addAndMove();

	//While the current character is an alpha numeric value keep adding this to the current lexeme.
	while(isInArray(alphanumArray,63)){addAndMove();}

	/*There are numerous resons why we might be here.
	 * 1. We might've reached a whitespace. In that case we can simply return from this function since we have found a proper lexeme
	 * 2. We might've reached a special character, in that case we can simply return from thins functino since we have found a proper lexeme
	 * 3. else*/

	if(isWhiteSpace()){}
	else if(isReservedWord()){}
	else{return errorProtocol("id");}
	return validToken("id");
}

token* lexor::num(int decision){

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
		if(decision == 0){
			if(currentCharacter =='.')return fraction();
		}
		if(isWhiteSpace()||isReservedWord()){return validToken("intnum");}

		else{return errorProtocol("intnum");}
	}
	//Check if its nonzero
	else if(isInArray(nonZeroArray,9)){
		addAndMove();

		while(isInArray(intArray,10)){addAndMove();}

		/*
		 * At this point there are cases we need to consider
		 * 1. White space
		 * 2. anything else is an error
		 */
		if(decision == 0){
		if(currentCharacter =='.')return fraction();
		}
		if(isWhiteSpace()||isReservedWord()){return validToken("intnum");}

		else{return errorProtocol("intnum");}


	}

	std::cout<<"CODE SHOULD NOT REACH HERE :: TOKEN* LEXOR::NUM()\n returning an empty token.";
	return new token();

}

token* lexor::fraction(){
	//Double check that the currentCharacter == '.'
	if(currentCharacter == '.'){addAndMove();}
	else{
		std::cout<<"INCORRECT CHARACTER, EXPECTED '.' in FRACTION() \n RETURN EMPTY TOKEN";
		return new token();
	}

	//Continuing after confirming'.'
	if(isInArray(intArray,10)){
		//a digit was confirmed so we move on
		//We keep adding digits
		//When we've reached the end of the lexeme we need to check the final digit.
		while(isInArray(intArray,10)){addAndMove();}
		//Need to verify the last character of the lexeme is not a 0
		if(currentLexeme.back() == '0'){return errorProtocol("frac");}

		else if(currentCharacter == 'e')return flt();
		//If we reach a white space or a reserve word we've reached the end of the lexeme
		else if(isWhiteSpace()||isReservedWord()){return validToken("frac");}
		else{return errorProtocol("frac");}
	}
	//Anything that is not a number is an error.
	return errorProtocol("frac");
}

token* lexor::flt(){
	//Double check currentCharacter == 'e'
	if(currentCharacter == 'e'){addAndMove();}
	else{
		std::cout<<"INCORRECT CHARACTER, EXPECTED 'e' in FLT()\n RETURN EMPTY TOKEN";
		return new token();
	}
	//Continuing after confirming'e'
	/*
	 *There are a few cases that we need to consider
	 *1. we have a 0 after the e
	 *2. we have a nonzero number after the e
	 *3. we have a +/- after e
	 */
	//If we habe a digit just use num () to verify
	if(isInArray(intArray,10)){
		token* t = num(1);
		t->setTypeName("floatnum");
		return t ;
	}

	//If we have a + or a - we add it to the lexeme and then use num() to verify
	else if(currentCharacter == '+' || currentCharacter == '-'){
		addAndMove();
		if(isInArray(intArray,10)){
			token* t = num(1);
			t->setTypeName("floatnum");
			return t ;
		}
	}

	//If we have anything else we have an error.
	return errorProtocol("frac");
}

token* lexor::res(){
	std::string charAsString1 {currentCharacter};
	//confirming the decision
	if(tokenMap.count(charAsString1)>0||currentCharacter == '='){addAndMove();}
	else{
		std::cout<<"INCORRECT CHARACTER, EXPECTED a map item in RES()\n RETURN EMPTY TOKEN";
		return new token();
	}
	//Check if it is possibly a comment
	if(charAsString1.compare("/")==0){
		//Could be a comment but need to confirm
		std::string charAsString2 = charAsString1 + currentCharacter;
		if(charAsString2.compare("//")==0||charAsString2.compare("/*")==0){
			addAndMove();
			return cmt();
		}
	}
	//Continue natural flow
	std::string charAsString2 = charAsString1 + currentCharacter;
	//
	if(tokenMap.count(charAsString2)>0){
		addAndMove();
	}

	else{}

	return validToken("res");
}

token* lexor::cmt(){
	if(currentLexeme.compare("//")==0){
		return validToken("linecomment");
	}
	else if(currentLexeme.compare("/*")==0){
		int lineCounter = 0;
		std::stack<int> commentStackCounter;
		commentStackCounter.push(0);

		while(true){
			if(commentStackCounter.empty()){
				return validToken("blockcomment",lineCounter);
			}

			if(inputFileStream.eof())
				return errorProtocol("eof");

			while(currentCharacter != '*'&&currentCharacter != '/'&&!inputFileStream.eof()){
				lineCounter += addAndMove(currentLexeme);
			}
			if(inputFileStream.eof())
				continue;
			else if(currentCharacter == '*'){
				addAndMove(currentLexeme);
				if(currentCharacter == '/'){
					addAndMove(currentLexeme);
					commentStackCounter.pop();
					continue;
				}
				continue;
			}
			else if(currentCharacter == '/'){
				addAndMove(currentLexeme);
				if(currentCharacter == '*'){
					addAndMove(currentLexeme);
					commentStackCounter.push(0);
					continue;
					}
			}

			else{
				addAndMove(currentLexeme);
				continue;
			}
		}
	}

	else{
		std::cout<<"ERROR THE CODE SHOULD NOT REACH HERE, END OF CMT() FUNCTION"<<std::endl;
	}
}

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
			line = 1;
			column = 0;
			currentCharacter = inputFileStream.get();
			currentLexeme = "";
		}
	}
	//Get rid of all white space.
	getRidOfWhiteSpace();

	if(checkEndOfStream()){
		throw EndOfFileException();
	}

	//We've set the possible type for the first character of our brand new lexeme.
	setPossibleType();

	//Check what the currentCharacter and go into the corresponding function
	if(possibleType == "id")return id();
	else if(possibleType == "num")return num(0);
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
void errorHandler::writeToken(token* t){
    std::ofstream file(tokenFileName, std::ios::app);
    file<<(*t)<<std::endl;
    file.close();
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
	std::string tokenfilename = filenamewithperiod.substr(0,filenamewithperiod.find("."));
	std::string errorfilename = filenamewithperiod.substr(0,filenamewithperiod.find("."));
	errorFileName = errorfilename+".outlexerrors";
	tokenFileName = tokenfilename+".outlextokens";
    std::ofstream file(errorFileName, std::ios::app);
    file.close();
    std::ofstream file2(tokenFileName, std::ios::app);
    file2.close();


}

void errorHandler::handleError(const std::string& errorType,const std::string & invalidType, const std::string& lexeme, const int & line, const int & column) {

    // Open the file in append mode to write (creates the file if it doesn't exist)
    std::ofstream file(errorFileName, std::ios::app);

    // Write content to the file
    file << errorType << ": "<<invalidType<<"\": "<<lexeme<<"\""<<": line "<<line<<": column "<<column<<std::endl;

    // Close the file
    file.close();
}


