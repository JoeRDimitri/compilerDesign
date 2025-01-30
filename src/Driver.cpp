#include "lexor.h"
#include <filesystem>
#include <iostream>


int main (){

	lexor* lex = new lexor();
	std::cout << "Current working directory: " << std::filesystem::current_path() << std::endl;
	try{
		while(true){
		try{
			token * t = lex->getNextToken();
			std::cout<<(*t)<<std::endl;
			delete t;
		}
		catch (const std::invalid_argument& e) {
		        std::cerr << "Caught exception: " << e.what() << std::endl;
		    }
		}


	}
	catch (const EndOfFileException& e) {
	        std::cerr << "EOF reached, terminating program normally.\n";
	    }
	catch(const std::exception & e){
        std::cerr << "Caught a generic exception: " << e.what() << '\n';
	}

	delete lex;
	return 0;
}
