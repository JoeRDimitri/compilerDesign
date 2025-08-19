#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <spdlog/spdlog.h>

//Forward Declaration of the token class
class token{};

class fileHandler{
public:
	static std::string tokenFileName;
	friend class errorHandler;

private:

	//A boolean to check if this is the first time reading the txt file.
	static bool virgin;
	//inputFileStream to read the file.
	static std::ifstream inputFileStream;
	bool virginProtocol();
	//Function to establish connection to the file to read.
	static bool connectFile(std::string);
	//Function to cut the connection to the file to read.
	static bool disconnectFile();
	//function that checks for the end of the input stream
	bool checkEndOfStream();
	void setFileName(std::string s);
	void writeToken(token* t);
	std::vector <std::string> splitString(const std::string& str, char delimiter);

};

class errorHandler{

public:
	static std::string errorFileName;
	friend class fileHandler;

	errorHandler(){};
	errorHandler(std::string filename1,std::string filename2){errorFileName = filename1;fileHandler::tokenFileName = filename2;};
	void handleError(const std::string & errorType,const std::string & invalidType,const std::string & lexeme, const int & line, const int & column, token * t);

private:

};

class EndOfFileException : public std::exception {
public:
    const char* what() const noexcept override {
        return "End of file reached";
    }
};
