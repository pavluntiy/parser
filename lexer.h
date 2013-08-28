#pragma once
#include "common_header.h"
#include "alphabet.h"
#include "token.h"

/* 
  TODO
  
  Надо что-то сделать чтобы пробелы тоже считались отступом

*/

class Lexer {
public: 
	std::string input;
	std::vector<Token> tokenList;
	std::stack<Token> storage;

	void terminate(std::string message){
		throw ParserException(message);
	}

	int currentTabDepth; 
	std::stack<int> previous;

	void freeBlocksStack(){	
		while (!previous.empty()){			
			previous.pop();
			parent->tokenList.push_back(Token(TokenType::BLOCK_END));
		}
	}

	bool isNewBlock(int tabCount){
		if (tabCount > currentTabDepth){
			previous.push(currentTabDepth);
			currentTabDepth = tabCount;
			return true;
		}
		return false;
	}

	bool isOldBlock (int tabCount){
		if (tabCount < currentTabDepth){
			
			while (!previous.empty() && previous.top() != tabCount){
				parent->tokenList.push_back(Token(TokenType::BLOCK_END));
				previous.pop();
			}

			if (previous.empty() && tabCount != 0){
			        throw ParserException("No match for this block!");
			}
			else{
				currentTabDepth = previous.top();
				previous.pop();
				return true;
			}
			
		}
		return false;
	}

	int currentPosition;
	char currentChar;
	Token currentToken;
	Lexer (std::string input): input(input), currentPosition(0){
		currentChar = input[currentPosition];
	}


	void consume (){
		currentPosition++; 

		if ( currentPosition >= (int)input.length() )
			currentChar = EOF;
		else
			currentChar = input[currentPosition]; 
	}

	bool get (std::string text){
		for (int i = 0; i < (int) text.size(); i++){
			if (text[i] != input[currentPosition + i])
				return false;
		}

		for (int i = 0; i < (int) text.size(); i++)
			consume();

		return true;
	}

	void match (char x){
		if (currentChar == x)
			consume ();
		else
		    throw ParserException(std::string("Expected '") + x + std::string("' got '") + currentChar + std::string("'\n"));
	}

	void consumeNewlines(){
		while (Alphabet::isNewline(currentChar))
			consume();
	}

	void consumeTabs(){
		while (Alphabet::isTab(currentChar))
			consume();
	}

	int countAndConsumeTabs(){
		int counter = 0;
		while (Alphabet::isTab(currentChar)){
			consume();
			++counter;
		}
		return counter;
	}

	bool isBoolConstant (const std::string& str){
		if (str == "true" || str == "false")
			return true;
		return false;
	}

	bool isKeyWord (const std::string& str){
		if(str == "if" || str == "else" || str == "for")
			return true;
		return false;
	}

	bool isEnd (const std::string& str){
		return (str == "end");
	}

	Token getName () {
		std::string buffer;

		while(Alphabet::isLetter(currentChar) || Alphabet::isDigit(currentChar) || currentChar == '_' ){
			buffer += currentChar;
			consume();
		}

		if (isBoolConstant(buffer)){
			return Token(TokenType::BOOL, buffer);
		}

		if (isEnd(buffer)){
			return Token(TokenType::BLOCK_END);
		}

		if (isKeyWord(buffer)){
			return Token(TokenType::KEYWORD, buffer);
		}

		return Token(TokenType::NAME, buffer);


	}

	Token getComment (){
		std::string buf;
		while (!Alphabet::isNewline(currentChar)){
			buf += currentChar;
			consume();
		}
		return Token (TokenType::COMMENT, buf);
	}

	Token getNextToken(){
		while (currentChar != EOF){
			while (Alphabet::isWhitespace(currentChar)){
				consume();
			}

			if (Alphabet::isNewline(currentChar)){
				int tabCount;
				while (Alphabet::isNewline(currentChar)){
					consumeNewlines();
					tabCount = countAndConsumeTabs();
				}	
				
				if (isNewBlock(tabCount)) {
					return Token(TokenType::BLOCK_BEGIN, "");
				}
				
				if (get("end") || isOldBlock(tabCount)) {
					return Token(TokenType::BLOCK_END, "");
				}
			}			

			if (Alphabet::isTab(currentChar))
				consumeTabs();

			if (Alphabet::isLetter(currentChar))
				return getName();

			switch (currentChar) {
				case '#' : return getComment();
			        case ',' : match(','); return Token(TokenType::COMMA, "," );
			        case '.' : match('.'); return Token(TokenType::DOT, "." ); 
			        case ';' : match(';'); return Token(TokenType::SEMICOLON, ";");

				case ':' : match(':'); 
					if ( currentChar == '=' ) 
						return Token(TokenType::OPERATOR, ":="); 
					else 
						return Token(TokenType::OPERATOR, ":");

				case '+' : match('+'); 
					if ( currentChar == '+' ) 
						return Token(TokenType::OPERATOR, "++"); 
					else if ( currentChar == '=' )
						return Token(TokenType::OPERATOR, "+=");
					else
						return Token(TokenType::OPERATOR, "+");

				case '-' : match('-');
					if ( currentChar == '-' )
						return Token(TokenType::OPERATOR, "--");
					else if ( currentChar == '=' )
						return Token(TokenType::OPERATOR, "-=");
					else
						return Token(TokenType::OPERATOR, "-");
					
				default: terminate("some trash");


			}
		}
		return Token (TokenType::END, "");

	}

	void tokenize (){
		currentToken = Token(TokenType::BEGIN, "");
		tokenList.push_back(currentToken);

		do {
			currentToken = getNextToken();
			tokenList.push_back(currentToken);

		}
		while (currentToken != Token(TokenType::END, ""));
	}


	const std::vector<Token>& getTokenList(){
		tokenize();
		return this->tokenList;
	}
	
};
