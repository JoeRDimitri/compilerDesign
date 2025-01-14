#include "lexor.h"
#include <iostream>
#include <unordered_map>
#include <string>
#include <iterator>

int main (){

	lexor* lex = new lexor();
	std::unordered_map <std::string, std::string> um = lex->tokenMap;
	auto map_iterator = um.begin();
	while(map_iterator != um.end()){
		std::cout<<"Dictionary key = "<<map_iterator->first<<". Dictonary value = "<<map_iterator->second<<"."<< std::endl;
		map_iterator++;
	}

	delete lex;
	return 0;
}
