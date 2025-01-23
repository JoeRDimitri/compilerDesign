#include "lexor.h"
#include <filesystem> // C++17 and later
#include <iostream>


int main (){

	lexor* lex = new lexor();
	std::cout << "Current working directory: " << std::filesystem::current_path() << std::endl;
	try{
		lex->getNextToken();
	}
	catch (const std::exception& e) {
	        std::cerr << "Caught exception: " << e.what() << std::endl;
	    }
	delete lex;
	return 0;
}
