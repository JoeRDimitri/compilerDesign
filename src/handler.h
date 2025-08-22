#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <spdlog/spdlog.h>

//Forward Declaration of the token class
class token;
class lexor;  // Add this forward declaration

std::ostream& operator<<(std::ostream& os, token& t);

class fileHandler{
public:
	fileHandler(){};

	static std::string tokenFileName;
	friend class errorHandler;
	char move();
	void writeToken(token* t);
	bool checkEndOfStream();
	static bool virgin;
	bool virginProtocol();

private:

	//A boolean to check if this is the first time reading the txt file.
	//inputFileStream to read the file.
	static std::ifstream inputFileStream;
	//Function to establish connection to the file to read.
	static bool connectFile(std::string);
	//Function to cut the connection to the file to read.
	static bool disconnectFile();
	//function that checks for the end of the input stream
	void setFileName(std::string s);
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
