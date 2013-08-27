
#pragma once
#include "common_header.h"
#include "alphabet.h"
#include "token.h"


class Lexer: public Token {
public: 
	std::string input;
	std::vector<Token> tokenList;
	std::stack<Token> storage;

	struct BlockDetecter {
		std::vector<Token> *parentTokenList;
		//Lexer &parent; I had an idea to keep a reference to my parent (lexer), in order to be able to 
		//terminate tokeniztion in case of an error,
		//but I didn't manage with circle references etc
		int currentTabDepth; 
		std::stack<int> previous;

		Token freeBlocksStack(){
			while (!previous.empty()){			
				previous.pop();
				parentTokenList->push_back(Token(Token::BLOCK_END));
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
					parentTokenList->push_back(Token(Token::BLOCK_END));
					previous.pop();
				}

				if (previous.empty() && tabCount != 0 || previous.top() != tabCount){
					parentTokenList->push_back(Token(Token::BLOCK_END, "No match for this block!"));
					//parent.terminate("No match for this block!"); //How to make it?
					std::cerr << "No match for this block!\n";
				}
				else{
					currentTabDepth = previous.top();
					previous.pop();
					return true;
				}
				
			}
			return false;
		}

		/*BlockDetecter(Lexer parent){
			this->parent = parent;
			this->currentTabDepth = 0;
		}*/

		BlockDetecter(std::vector<Token> *parentTokenList){
			this->parentTokenList =  parentTokenList;
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
		//std::cout << currentPosition << ' '; 
		currentChar = input[currentPosition]; 
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

	

	Token getNextToken(){
		while (currentChar != (char) -1){
			if (isWhitespace(currentChar)){
				consumeWS();
			}

			if (isNewline(currentChar)){
				int tabCount;
				while (isNewline(currentChar)){
					consumeNewlines();
					tabCount = countAndConsumeTabs();
				}
				
					std::cout << tabCount << '\n';
					if (blockDetecter.isNewBlock(tabCount)){
						return Token(Token::BLOCK_BEGIN, "");
						//std::cout << "asdf";
					}
					
					if (blockDetecter.isOldBlock(tabCount)){
						return Token(Token::BLOCK_END, "");
					}
			}
			

			if (isTab(currentChar))
				consumeTabs();



			;
			switch (currentChar) {
				case ',' : consume(); return  Token(COMMA, "," ); break;
				case '.' : consume(); return  Token(DOT, "." ); break;
			}
		}
		return Token (Token::END, "");

	}
	void terminate(std::string text){
		currentToken = Token(END, text);
	}

	void tokenize (){
		blockDetecter = BlockDetecter(& (this->tokenList)); /////////////////HELP!!!!
		currentToken = Token(Token::BEGIN, "");

		do {
			tokenList.push_back(currentToken);
			currentToken = getNextToken();
			std::cout << "eh\n";
		}
		while (currentToken != Token(Token::END, ""));

		blockDetecter.freeBlocksStack();
		

		tokenList.push_back(currentToken);
	}


	std::vector<Token> getTokenList(){
		return this->tokenList;
	}
	
};