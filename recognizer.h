#ifndef RECOGNIZER.H
#define RECOGNIZER.H
#include "common_header.h"
#include "token.h"
#include "alphabet.h"
class Recognizer {
protected:
	std::vector<Token> tokenList;
	std::string input;
	std::set<std::string> keywords;
	int currentPosition;
	char currentChar;
	Token currentToken;
	Token::Position sourcePosition;


	void terminate(std::string message){
		throw ParserException(message);
	}

	int currentTabDepth; 
	std::stack<int> previous;



	void freeBlocksStack(){	 
		while (!previous.empty()){			
			previous.pop();
			tokenList.push_back(makeToken(Token::BLOCK_END));
		}
	}

	bool isNewBlock(int tabCount){
		if (tabCount > currentTabDepth){
			//std::cout << "******\n";
			previous.push(currentTabDepth);
			currentTabDepth = tabCount;
			//std::cout << "now on (push)" << currentTabDepth << " after " << sourcePosition.toString() << "\n";
			return true;
		}
		return false;
	}

	bool isOldBlock (int tabCount){
		if (tabCount < currentTabDepth){
			while (!previous.empty() && previous.top() != tabCount){
										//std::cout << "!!!!!\n";
				tokenList.push_back(makeToken(Token::BLOCK_END));

				previous.pop();

			}

			if (previous.empty() && tabCount != 0){
					std::stringstream ss;
					ss << tabCount;
				    throw ParserException("No match for block on " + sourcePosition.toString() + " (only " + ss.str() + " tabs)\n");
			}
			else {

						currentTabDepth = previous.top();
						//std::cout << "now on (pop)" << currentTabDepth << " after " << sourcePosition.toString() << "\n";
						previous.pop();
					return true;
				}

		}
		return false;
	}

	Token makeToken (Token::TokenType type = Token::NONE, std::string text = ""){
		return Token(type, const_cast<char *> (text.c_str()), sourcePosition);
	}

	Token makeToken (Token::TokenType type, std::string text, Token::Position position){
		return Token(type, const_cast<char *> (text.c_str()), position);
	}

	void consume (){
		if (Alphabet::isNewline(currentChar)){
			sourcePosition.line ++;
			sourcePosition.linePosition = 0;
		}
		else {
			sourcePosition.linePosition++;
		}

		if (currentChar == EOF){
			throw ParserException("Unexpected end of file on " + sourcePosition.toString() + "\n");
		}
		
		currentPosition++; 
		
		
		
		if ( currentPosition >= (int)input.length() )
			currentChar = EOF;
		else {
			currentChar = input[currentPosition]; 
		}
	}

	bool get(std::string text){
		for (int i = 0; i < (int) text.size(); i++){
			if (text[i] != input[currentPosition + i])
				return false;
		}

		for (int i = 0; i < (int) text.size(); i++)
			consume();

		return true;
	}

	bool find(std::string text){
		for (int i = 0; i < (int) text.size(); i++){
			if (text[i] != input[currentPosition + i])
				return false;
		}
		return true;
	}

	void match (char x){
		if (currentChar == x)
			consume ();
		else
		    throw ParserException(std::string("Expected ") + x + std::string(" got ") + currentChar 
		    	+ sourcePosition.toString() + std::string("'\n"));
	}

	static bool isBoolConstant (const std::string& str){
		if (str == "true" || str == "false")
			return true;
		return false;
	}

	static bool isCommentBegin (const std::string& str){
		if (str == "/*" || str == "#")
			return true;
		return false;
	}

	bool isKeyWord (const std::string& str){
//		if(str == "if" || str == "else" || str == "for" || str == "int" || str == "auto" || str == "using" 
//			|| str == "return" || str == "end")
		if(keywords.count(str))
			return true;
		return false;
	}
/*
	static bool isEnd (const std::string& str){
		return (str == "end");
	}
*/
void keywordsInit(){
	std::ifstream keywordsIn ("keywords");
	std::string currentKeyword;
	while (keywordsIn>>currentKeyword){
		keywords.insert(currentKeyword);
	}

	keywordsIn.close();
}

public:
	Recognizer (std::string input) : currentPosition(0){
		this->input = "\n" + input;
		currentChar = this->input[currentPosition];
		currentTabDepth = 0;

		keywordsInit();

	}

	Recognizer(){}
};
#endif