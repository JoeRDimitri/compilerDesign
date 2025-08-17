#include "lexor.h"
#include "parser.h"
#include <filesystem>
#include <iostream>


int main (){

//	/*	CODE TO TEST THE LEXOR
		lexor* lex = new lexor();
		std::cout << "Current working directory: " << std::filesystem::current_path() << std::endl;
		std::vector<token*> vectorOfTokens;
		try{
			while(true){
			try{
				token * t = lex->getNextToken();
				if(t->getTypeName().find("cmt")==std::string::npos && t->getTypeName().find("comment")==std::string::npos)
				vectorOfTokens.emplace_back(t);
				std::cout<<(*t)<<std::endl;
			}
			catch (const std::invalid_argument& e) {
			        std::cerr << "Caught exception: " << e.what() << std::endl;
			    }
			}


		}
		catch (const EndOfFileException& e) {
			token * lastToken = new token("$","$",-1,-1);
			vectorOfTokens.emplace_back(lastToken);
		        std::cerr << "EOF reached."<<std::endl;
		    }
		catch(const std::exception & e){
	        std::cerr << "Caught a generic exception: " << e.what() << std::endl;
		}

		delete lex;
//		*/
	std::cout<<"------------------------------------------------------------------------------------"<<std::endl;
	std::cout<<std::endl;

	parser p;
	p.faf.generateFirstSet();
    std::ofstream outFile("firstSet"); // Open file for writing
    for (const auto& pair : p.faf.firstSet) {
    	outFile << "Key: " << pair.first << " -> Values: "<<std::endl;

        // Check if the pointer is valid before dereferencing
        if (pair.second) {
            for (const auto& value : *(pair.second)) {
            	outFile<< value << " "<<std::endl;
            }
        }
        outFile << std::endl;
    }
    outFile.close();

	std::cout<<"------------------------------------------------------------------------------------"<<std::endl;
	p.faf.generateFollowSet();
    std::ofstream secondOutFile("followSet"); // Open file for writing

	// Iterate over the map
	    for (const auto& pair : p.faf.followSet) {
	    	secondOutFile << "Key: " << pair.first << " -> Values: "<<std::endl;

	        // Check if the pointer is valid before dereferencing
	        if (pair.second) {
	            for (const auto& value : *(pair.second)) {
	            	secondOutFile << value << " "<<std::endl;
	            }
	        }
	        secondOutFile << std::endl;
	    }

	    secondOutFile.close();
	std::cout<<"------------------------------------------------------------------------------------"<<std::endl;

	p.parsingTable.buildTable();
	p.parse(vectorOfTokens);
	std::cout<<"Finished Parsing"<<std::endl;
	p.AST.printTree();


	std::cout<<"------------------------------------------------------------------------------------"<<std::endl;

	p.AST.treeHead->accept(p.first);
	for (auto it = p.AST.treeHead->stMap.begin(); it != p.AST.treeHead->stMap.end(); ++it) {
	    std::cout << it->first << ": " << it->second.type<< std::endl;
	}
	std::cout<<"Finished Building Symbol Table"<<std::endl;

	return 0;

}
