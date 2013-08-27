
#pragma once
#include "common_header.h"
#include "alphabet.h"
#include "token.h"


class Lexer: public Token {
public: 
	std::string input;
	std::vector<Token> tokenList;
	std::stack<Token> storage;

	bool successfullyRead;

	void terminate(std::string message){
			tokenList.push_back(Token(Token::END, message));
			successfullyRead = false;
		}

	struct BlockDetecter {
		Lexer *parent;
		int currentTabDepth; 
		std::stack<int> previous;

		

		Token freeBlocksStack(){
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
					parent->terminate("No match for this block!"); //How to make it?
					//std::cerr << "No match for this block!\n";
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
		successfullyRead = true;

	}


	void consume (){
		currentPosition++; 
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
			std::cerr << "Expexted " <<  x <<  " got " << currentChar << "\n";
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

	bool isBoolConstant (std::string str){
		if (str == "true" || str == "false")
			return true;
		return false;
	}

	bool isKeyWord (std::string str){
		if(str == "if" || str == "else" || str == "for")
			return true;
		return false;
	}

	bool isEnd (std::string str){
		return (str == "end");
	}

	Token getName () {
		std::string bufer;

		while(!isSeparator(currentChar) && !isSpecialSymbol(currentChar)){
			bufer += currentChar;
			consume();
		}

		if (isBoolConstant(bufer)){
			return Token(BOOL, bufer);
		}

		if (isEnd(bufer)){
			return Token(BLOCK_END);
		}

		if (isKeyWord(bufer)){
			return Token(KEYWORD, bufer);
		}

		return Token(NAME, bufer);


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

	Token plusVariants(){
		if (get("+="))
			return Token(OPERATOR, "+=");

		if (get("++"))
			return Token (OPERATOR, "++");

		if(get("+"))
			return Token (OPERATOR, "+");

	}

	Token minusVariants(){
		if (get("-="))
			return Token(OPERATOR, "-=");

		if (get("--"))
			return Token (OPERATOR, "--");

		if(get("-"))
			return Token (OPERATOR, "-");

	}

	Token colonVariants(){
		if (get(":=")){
				return Token(ASSIGN, ":=");
		}
			else {
				if (get(":"))
						return Token(OPERATOR, ":");
				terminate("What a trash!");
			}
	}
	

	Token getNextToken(){
		while (currentChar != EOF){
			if (isWhitespace(currentChar)){
				consumeWS();
			}
			if (!successfullyRead)
				break;

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
			if (!successfullyRead)
				break;

			if (isTab(currentChar))
				consumeTabs();

			if (!successfullyRead)
				break;

			if (isLetter(currentChar))
				return getName();


			if (!successfullyRead)
				break;
			switch (currentChar) {
				case '#' : return getComment();
				case ',' : get(","); return  Token(COMMA, "," ); break;
				case '.' : get("."); return  Token(DOT, "." ); break;
				case ';' : get(";"); return Token(SEMICOLON, ";");
				case ':' : return colonVariants(); 
				case '+' : return plusVariants();
				case '-' : return minusVariants();	
					
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


	std::vector<Token> getTokenList(){
		tokenize();
		return this->tokenList;
	}
	
};