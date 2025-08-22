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
			spdlog::info("Entering main loop from the driver.cpp file.");
			while(true){
			try{
				token * t = lex->getNextToken();
				if(t->getTypeName().find("cmt")==std::string::npos && t->getTypeName().find("comment")==std::string::npos)
				vectorOfTokens.emplace_back(t);
			}
			catch (const std::invalid_argument& e) {
				spdlog::warn("Caught exception: {}",e.what());
				return 1;
			    }
			}


		}
		catch (const EndOfFileException& e) {
			token * lastToken = new token("$","$",-1,-1);
			vectorOfTokens.emplace_back(lastToken);
		    }
		catch(const std::exception & e){
	        std::cerr << "Caught a generic exception: " << e.what() << std::endl;
		}

		delete lex;

	std::cout<<"------------------------------------------------------------------------------------"<<std::endl;
	spdlog::info("Entering Second phase, the parsing phase.");

	parser p;
	try{
		p.faf.generateFirstSet();
	}
	catch (const std::invalid_argument& e) {
		spdlog::warn("Caught exception: {}",e.what());
		return 1;
	    }
	p.faf.writeToFirstSetFile();

	try{
		p.faf.generateFollowSet();
	}
	catch (const std::invalid_argument& e) {
		spdlog::warn("Caught exception: {}",e.what());
		return 1;
	    }
	p.faf.writeToFollowSetFile();


	std::cout<<"------------------------------------------------------------------------------------"<<std::endl;

	p.parsingTable.buildTable();
	p.parse(vectorOfTokens);
	std::cout<<"Finished Parsing"<<std::endl;
	p.AST.printTree();


	std::cout<<"------------------------------------------------------------------------------------"<<std::endl;
//
//	p.AST.treeHead->accept(p.first);
//	p.AST.printSymbolTable(p.AST.treeHead);
//	std::cout<<"Finished Building Symbol Table"<<std::endl;

	return 0;

}
