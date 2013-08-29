

#include "token.h"
class Consument {
public:
	std::string input;	
	int currentPosition;
	char currentChar;

public:
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



};