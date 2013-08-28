#include "lexer.h"
#include "common_header.h"


int main (){
	std::string input;

	std::ifstream in ("input.in");
	std::ofstream out ("output.out");
	while ( !in.eof() ) { 
		input += in.get(); 
	}

	Lexer lexer(input);

	auto list = lexer.getTokenList();

	for (auto c: list){
		out << c;
	}

	return 0;
}