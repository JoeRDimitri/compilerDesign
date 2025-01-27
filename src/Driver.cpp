#include "lexor.h"
#include <filesystem>
#include <iostream>


int main (){

	lexor* lex = new lexor();
	std::cout << "Current working directory: " << std::filesystem::current_path() << std::endl;
	for (int k = 0;k<75;k++){
	try{
		token * t = lex->getNextToken();
		std::cout<<(*t)<<std::endl;
		delete t;
	}
	catch (const std::exception& e) {
	        std::cerr << "Caught exception: " << e.what() << std::endl;
	    }
	}
	delete lex;
	return 0;
}
