#pragma once
#include "common_header.h"
#include "alphabet.h"
#include "token.h"

/*
  TODO

  1. Ничего из Token::TokenType здесь не в области видимости, надо пофиксить.
  2. В alphabet довольно странные функции, и потом Token не является Alphabet => зачем наследование?
  в алфавите по идее все должно быть статическим.
 */

class Lexer {

protected:
	std::string input;
	std::vector<Token> tokenList;
	std::stack<Token> storage;
	int currentPosition;
	char currentChar;
	Token currentToken;

	void terminate(std::string message){
			throw ParserException(message);
		}

	struct BlockDetecter {
		Lexer *parent;
		int currentTabDepth; 
		std::stack<int> previous;


		void freeBlocksStack(){
			while (!previous.empty()){			
				previous.pop();
				parent->tokenList.push_back(Token(Token::BLOCK_END));
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
					parent->tokenList.push_back(Token(Token::BLOCK_END));
					previous.pop();
				}

				if (previous.empty() && tabCount != 0){
				        //parent->terminate("No match for this block!");
				        //std::cerr << "No match for this block!\n";
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

		BlockDetecter(Lexer *parent){
			this->parent = parent;
			this->currentTabDepth = 0;
		}

		BlockDetecter(){
			this->currentTabDepth = 0;
		}

	} blockDetecter;
	




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

	void consumeWS(){
		while (Alphabet::isWhitespace(currentChar))
			consume();
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
			counter++;
		}
		return counter;
	}

	

	Token getName () {
		std::string buffer;

		while(Alphabet::isLetter(currentChar) || Alphabet::isDigit(currentChar)){
			buffer += currentChar;
			consume();
		}

		if (Alphabet::isBoolConstant(buffer)){
			return Token(Token::BOOL, buffer);
		}

		if (Alphabet::isEnd(buffer)){
			return Token(Token::BLOCK_END);
		}

		if (Alphabet::isKeyWord(buffer)){
			return Token(Token::KEYWORD, buffer);
		}

		return Token(Token::NAME, buffer);


	}

	bool tryComment (){
		return Alphabet::isCommentBegin(std::string("") + currentChar) 
			|| Alphabet::isCommentBegin(std::string("") + currentChar + input[currentPosition + (currentPosition < (int) input.size() - 1) ? 1 : 0]); 
	}
	Token getComment (){
		std::string buffer;
		while (!Alphabet::isNewline(currentChar)){
			buffer += currentChar;
			consume();
		}
		return Token(Token::COMMENT, buffer);
	}
	
	Token getMultyLineComment () {
		std::string buffer;
		while ((!Alphabet::isEof(currentChar) && !get("*/"))){
			buffer += currentChar;
			consume();
		}
		return Token(Token::COMMENT, buffer);
	}

	Token getSystemTokens (){
			while (Alphabet::isWhitespace(currentChar)){
				consume();
			}

			if (Alphabet::isNewline(currentChar)){


				int tabCount;
				while (Alphabet::isNewline(currentChar)){
					consumeNewlines();
					tabCount = countAndConsumeTabs();
					
				}
				
				if (!tryComment() && blockDetecter.isNewBlock(tabCount)){
				        return Token(Token::BLOCK_BEGIN, "");
				}
				
				if (!tryComment() && (get("end") || blockDetecter.isOldBlock(tabCount))){
				        return Token(Token::BLOCK_END, "");
				}
			}			

			if (Alphabet::isTab(currentChar))
				consumeTabs();

			if (Alphabet::isLetter(currentChar))
				return getName();

			if (get("/*"))
					return getMultyLineComment();


			return Token(Token::NONE);

	}

	Token getSymbolicTokens(){

		switch (currentChar) {
					case '#' : return getComment();
			        case ',' : match(','); return Token(Token::COMMA, "," );
			       
			        case ';' : match(';'); return Token(Token::SEMICOLON, ";");

			        case '.' : match('.'); 
			        if (currentChar == '.'){
			         	match ('.');
			         	if (currentChar == '.'){
			         		match('.');
			         		return Token(Token::OPERATOR, "...");
			         	}
			         	else {
			         		return Token(Token::OPERATOR, "..");
			         	}
			        }
			        else {
			         	return Token(Token::DOT, "." ); 
			     	}

				case ':' : match(':'); 
					if ( currentChar == '=' ) {
						match('=');
						return Token(Token::OPERATOR, ":="); 
					}
					else if (currentChar == ':'){
						match(':');
						if (currentChar == '='){
							return Token(Token::OPERATOR, "::=");
						}
						else {
							return Token(Token::OPERATOR, "::");
						}
					}
					else {
						return Token(Token::OPERATOR, ":");
					}
					

				case '+' : match('+'); 
					if ( currentChar == '+' ){
						match('+');
						return Token(Token::OPERATOR, "++"); 
					}
					else if ( currentChar == '=' ){
						match('=');
						return Token(Token::OPERATOR, "+=");
					}
					else {
						return Token(Token::OPERATOR, "+");
					}

				case '-' : match('-');
					if (currentChar == '>'){
						return Token(Token::OPERATOR, "->");
					}
					else if (currentChar == '-' ){
						match('-');
						if (currentChar == '>'){
							return Token(Token::OPERATOR, "-->");
						}
						else {
							return Token(Token::OPERATOR, "--");
						}
					}
					else if ( currentChar == '=' ){
						match('=');
						return Token(Token::OPERATOR, "-=");
					}
					else {
						return Token(Token::OPERATOR, "-");
					}
					

					case '=': match('=');
					if (currentChar == '='){
						match('=');
						return Token(Token::OPERATOR, "==");
					}
					else {
						return Token(Token::OPERATOR, "=");
					}

					case '<':match('<');
					if (currentChar == '='){
						match('=');
						return Token(Token::OPERATOR, "<=");
					}
					else if (currentChar == '<'){
						match('<');
						return Token(Token::OPERATOR, "<<");
					}
					else {
						return Token(Token::OPERATOR, "<");

					}

					case '>':match('>');
					if (currentChar == '='){
						match('=');
						return Token(Token::OPERATOR, ">=");
					}
					else if (currentChar == '>'){
						match('>');
						if (currentChar == '>'){
							match('>');
							return Token(Token::OPERATOR, ">>>");
						}
						else {
							return Token(Token::OPERATOR, ">>");
						}
					}
					else {
						return Token(Token::OPERATOR, ">");

					}
					case EOF: return Token (Token::END, "");

				default: 
					if (Alphabet::isSpecialSymbol(currentChar)){
						char c = currentChar; 
						consume();
						return Token(Token::OPERATOR, std::string("") + c);
					}
					else{
						std::cout << (int) currentChar;
						throw ParserException(std::string("Unknown character ") + currentChar + "\n");
					}

			}

			return Token(Token::NONE);

	}

	Token getNextToken(){
		while (currentChar != EOF){
		

			currentToken = getSystemTokens();
			if (currentToken != Token(Token::NONE)){
				return currentToken;
			}

			currentToken = getSymbolicTokens();
			if (currentToken != Token(Token::NONE)){
				return currentToken;
			}


			
		}
		return Token (Token::END, "");

	}

	void tokenize (){
		blockDetecter = BlockDetecter(this);
		currentToken = Token(Token::BEGIN, "");
		tokenList.push_back(currentToken);

		do {
			currentToken = getNextToken();
			tokenList.push_back(currentToken);

		}
		while (currentToken != Token(Token::END, ""));
	}

public: 


	Lexer (std::string input): input(input), currentPosition(0){
		currentChar = input[currentPosition];
	}

	const std::vector<Token>& getTokenList(){
		tokenize();
		return this->tokenList;
	}
	
};
