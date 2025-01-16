#include "lexor.h"
#include <filesystem> // C++17 and later
#include <iostream>


int main (){

	lexor* lex = new lexor();
	std::cout << "Current working directory: " << std::filesystem::current_path() << std::endl;
	lex->connectFile("./assignment1.COMP442-6421.paquet.2025.4/lexpositivegrading.src");
	lex->disconnectFile();
	delete lex;
	return 0;
}
