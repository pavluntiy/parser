#include "lexer.h"
#include "common_header.h"


int main (){
	std::string input;

	std::ifstream in ("input.in");
	std::ofstream out ("output.out");
	while ( !in.eof() ) { 
		input += in.get(); 
	}

	input += '\0';
	Lexer lexer(input);

	lexer.tokenize();
	auto list = lexer.getTokenList();

	for (auto c: list){
		out << c.toString();
	}

	return 0;
}