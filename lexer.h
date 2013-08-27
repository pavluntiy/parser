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
public: 
	std::string input;
	std::vector<Token> tokenList;
	std::stack<Token> storage;

	void terminate(std::string message){
			throw ParserException(message);
		}

	struct BlockDetecter {
		Lexer *parent;
		int currentTabDepth; 
		std::stack<int> previous;


		void freeBlocksStack(){	    //у тебя здесь возращаемый тип был Token, но ты ничего не возвращал и я его сменил его на void
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

	void consumeWS(){
		while (isWhitespace(currentChar))
			consume();
	}

	void consumeNewlines(){
		while (isNewline(currentChar))
			consume();
	}

	void consumeTabs(){
		while (isTab(currentChar))
			consume();
	}

	int countAndConsumeTabs(){
		int counter = 0;
		while (isTab(currentChar)){
			consume();
			counter++;
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

		while(isLetter(currentChar) || isDigit(currentChar) || currentChar == '_' ){
			buffer += currentChar;
			consume();
		}

		if (isBoolConstant(buffer)){
			return Token(BOOL, buffer);
		}

		if (isEnd(buffer)){
			return Token(BLOCK_END);
		}

		if (isKeyWord(buffer)){
			return Token(KEYWORD, buffer);
		}

		return Token(NAME, buffer);


	}

	Token getComment (){
		std::string buf;
		while (!isNewline(currentChar)){
			buf += currentChar;
			consume();
		}
		std::cout << buf;
		return Token (COMMENT, buf);

	}

	Token getNextToken(){
		while (currentChar != EOF){
			while (isWhitespace(currentChar)){
				consume();
			}

			if (isNewline(currentChar)){

				int tabCount;
				while (isNewline(currentChar)){
					consumeNewlines();
					tabCount = countAndConsumeTabs();
					
				}
				
				if (blockDetecter.isNewBlock(tabCount)){
				        return Token(Token::BLOCK_BEGIN, "");
				}
				
				if (get("end") || blockDetecter.isOldBlock(tabCount)){
				        return Token(Token::BLOCK_END, "");
				}
			}			

			if (isTab(currentChar))
				consumeTabs();

			if (isLetter(currentChar))
				return getName();

			switch (currentChar) {
				case '#' : return getComment();
			        case ',' : match(','); return Token(COMMA, "," );
			        case '.' : match('.'); return Token(DOT, "." ); 
			        case ';' : match(';'); return Token(SEMICOLON, ";");

				case ':' : match(':'); 
					if ( currentChar == '=' ) 
						return Token(OPERATOR, ":="); 
					else 
						return Token(OPERATOR, ":");

				case '+' : match('+'); 
					if ( currentChar == '+' ) 
						return Token(OPERATOR, "++"); 
					else if ( currentChar == '=' )
						return Token(OPERATOR, "+=");
					else
						return Token(OPERATOR, "+");

				case '-' : match('-');
					if ( currentChar == '-' )
						return Token(OPERATOR, "--");
					else if ( currentChar == '=' )
						return Token(OPERATOR, "-=");
					else
						return Token(OPERATOR, "-");
					
				default: terminate("some trash");


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


	const std::vector<Token>& getTokenList(){
		tokenize();
		return this->tokenList;
	}
	
};
