#include "lexer.h"
#include "common_header.h"


int main (){
	std::string input;

	std::ifstream in ("input.in");
	std::ofstream out ("output.out");
	while (!in.eof()) { 
		input += in.get(); 
	}

	Lexer lexer(input);

	try {
		auto list = lexer.getTokenList();
		for (auto currentToken: list){
			out << currentToken;
		}

	}
	catch ( ParserException& e ) {
		std::cerr << e.what() << '\n';
	}
	
	return 0;
}
