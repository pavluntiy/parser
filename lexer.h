#ifndef LEXER.H 
#define LEXER.H 
#include "common_header.h"
#include "alphabet.h"
#include "token.h"
#include "recognizer.h"
#include "systemtokens.h"
#include "symboltokens.h"

class Lexer: public SymbolTokens{

/* 
  TODO
  
  Надо что-то сделать чтобы пробелы тоже считались отступом

*/
  	Token getNextToken(){
		while (currentChar != EOF){
		

			currentToken = getSystemTokens();
			if (currentToken != makeToken(Token::NONE)){
				return currentToken;
			}

			currentToken = getSymbolicTokens();
			if (currentToken != makeToken(Token::NONE)){
				return currentToken;
			}


			
		}
		return makeToken (Token::END, "");

	}

	void tokenize (){
		currentToken = makeToken(Token::BEGIN, "");
		do {
			tokenList.push_back(currentToken);
			currentToken = getNextToken();
		}
		while (currentToken != makeToken(Token::END, ""));
		//std::cout << "kk";
		freeBlocksStack();
		tokenList.push_back(currentToken);
	}

public: 


	Lexer (std::string input): SymbolTokens(input)
	{}

	const std::vector<Token>& getTokenList(){
		tokenize();
		return this->tokenList;
	}
	
};

#endif
