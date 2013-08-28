#ifndef LEXER.H 
#define LEXER.H 
#include "common_header.h"
#include "alphabet.h"
#include "token.h"
#include "consument.h"


class Lexer{

/* 
  TODO
  
  Надо что-то сделать чтобы пробелы тоже считались отступом

*/

protected:
	std::vector<Token> tokenList;
	std::string input;
	
	int currentPosition;
	char currentChar;
	Token currentToken;


	void terminate(std::string message){
		throw ParserException(message);
	}

	int currentTabDepth; 
	std::stack<int> previous;



	void freeBlocksStack(){	
		std::cout << "freeBlocksStack\n"; 
		while (!previous.empty()){			
			previous.pop();
			tokenList.push_back(Token(Token::BLOCK_END));
		}
	}

	bool isNewBlock(int tabCount){
		if (tabCount > currentTabDepth){
			previous.push(currentTabDepth);
			currentTabDepth = tabCount;
			std::cout << previous.size() << " isNewBlock \n";
			return true;
		}
		return false;
	}

	bool isOldBlock (int tabCount){
		std::cout << currentTabDepth << ':';
		if (tabCount < currentTabDepth){
			std::cout << currentTabDepth << ':';
			while (!previous.empty()){ //&& previous.top() != tabCount){
				
				tokenList.push_back(Token(Token::BLOCK_END));

				previous.pop();

				std::cout << previous.size() << " isOldBlock  " << tabCount << "\n";
			}

			if (previous.empty() && tabCount != 0){
				        throw ParserException("No match for this block!");
			}
			else{
					//std::cout << "*****";
					if (!previous.empty()){
						currentTabDepth = previous.top();
						previous.pop();
					}			
					else {
						throw ParserException("asdf");
					}
					return true;
				}

		}
		return false;
	}


public:
	void consume (){
		currentPosition++; 

		if (currentChar == EOF){
			throw ParserException("Unfinished char or string");
		}
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
			++counter;
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

		if (Alphabet::isKeyWord(buffer)){
			return Token(Token::KEYWORD, buffer);
		}

		return Token(Token::NAME, buffer);

	}
/////TODO
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
		consume();
		return Token(Token::COMMENT, buffer);
	}

	Token getChar(){
		std::string buffer;
		buffer += '\'';
		while (currentChar != '\''){
			buffer += currentChar;
			consume();
		}
		consume();
		buffer += '\'';
		return Token(Token::CHAR, buffer); 
	}

	Token getString(){
		std::string buffer;
		buffer += '\"';
		while (currentChar != '\"'){
			buffer += currentChar;
			consume();
		}
		consume();
		buffer += '\"';
		return Token(Token::STRING, buffer); 
	}

	Token tryAndGetNumeric(){
		std::string buffer;
		bool isFloat = false;
		bool zeroFound = false;
		bool numericDetected = false;

<<<<<<< HEAD
		if (currentChar == '0'){
			zeroFound = true;
=======
		while(Alphabet::isLetter(currentChar) || Alphabet::isDigit(currentChar) || currentChar == '_' ){
>>>>>>> 7595de36625bbe107d0799aefc4c22af4d87f27d
			buffer += currentChar;
			numericDetected = true;
			consume();
		}

		if (currentChar == 'X' || currentChar == 'x'){
			if (zeroFound){
			buffer += 'x';
				consume();
				if (!Alphabet::isHexadecimalDigit(currentChar)){
					throw ParserException("Hexadecimal not recognized");
				}
				while (Alphabet::isHexadecimalDigit(currentChar) || currentChar == '_' || currentChar == '.'){
					if (currentChar == '.'){
						isFloat = true;
					}
					buffer += currentChar;
					consume();
				}
			}
		}


		if (currentChar == 'B' || currentChar == 'b'){
			if (zeroFound){
				buffer += 'b';
				consume();
				if (!Alphabet::isBinaryDigit(currentChar)){
					throw ParserException("Binar not recognized");
				}
				while (Alphabet::isBinaryDigit(currentChar) || currentChar == '_' || currentChar == '.'){
					if (currentChar == '.'){
						isFloat = true;
					}
					buffer += currentChar;
					consume();
				}
			}
		}

		if (zeroFound){
			while (Alphabet::isOctalDigit(currentChar)|| currentChar == '_' || currentChar == '.'){
			if (currentChar == '.'){
						isFloat = true;
					}
					buffer += currentChar;
					consume();							
			}
		}

		if (Alphabet::isDigit(currentChar)){
			numericDetected = true;
		}
		while (Alphabet::isDigit(currentChar)|| currentChar == '_' || currentChar == '.'){
			if (currentChar == '.'){
						isFloat = true;
					}
					buffer += currentChar;
					consume();							
		}

<<<<<<< HEAD
		if (numericDetected){
			if (isFloat){
				return Token(Token::FLOAT, buffer);
			}
			return Token(Token::INT, buffer);
=======
	Token getComment (){
		std::string buf;
		while (!Alphabet::isNewline(currentChar)){
			buf += currentChar;
			consume();
>>>>>>> 7595de36625bbe107d0799aefc4c22af4d87f27d
		}
		else return Token(Token::NONE);		
	}
<<<<<<< HEAD
	
	Token getSystemTokens (){
=======

	Token getNextToken(){
		while (currentChar != EOF){
>>>>>>> 7595de36625bbe107d0799aefc4c22af4d87f27d
			while (Alphabet::isWhitespace(currentChar)){
				consume();
			}

			if (Alphabet::isNewline(currentChar)){
				int tabCount;
				while (Alphabet::isNewline(currentChar)){
					consumeNewlines();
					tabCount = countAndConsumeTabs();
				}	
				

				if (!tryComment() && isNewBlock(tabCount)){
				        return Token(Token::BLOCK_BEGIN, "");
				}
				
				if (!tryComment() && isOldBlock(tabCount)){
				        return Token(Token::BLOCK_END, "");
				 }

				if (isNewBlock(tabCount)) {
					return Token(Token::BLOCK_BEGIN, "");
				}
				
				if (isOldBlock(tabCount)) {
					return Token(Token::BLOCK_END, "");
				}
			}

<<<<<<< HEAD
			if (Alphabet::isTab(currentChar)){
=======
			if (Alphabet::isTab(currentChar))
>>>>>>> 7595de36625bbe107d0799aefc4c22af4d87f27d
				consumeTabs();
			}

<<<<<<< HEAD
			if (get("/*")){
				return getMultyLineComment();
			}

			if (Alphabet::isLetter(currentChar)){
=======
			if (Alphabet::isLetter(currentChar))
>>>>>>> 7595de36625bbe107d0799aefc4c22af4d87f27d
				return getName();
			}

			return tryAndGetNumeric();

	}

	Token getDotVariants (){
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
	}

	Token getPlusVariants (){
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
	}


	Token getColonVariants(){
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
	}

	Token getMinusVariats(){
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
	}

	Token getEqualsVariants(){
		if (currentChar == '='){
						match('=');
						return Token(Token::OPERATOR, "==");
					}
					else {
						return Token(Token::OPERATOR, "=");
					}
	}

	Token getLessVariants(){
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

	}

	Token getGreaterVariants(){
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
	}

	Token getSlashVariants(){
			if (currentChar == '/'){
						match('/');
						return Token(Token::OPERATOR, "//");
					}
					else if (currentChar == '='){
						match('=');
						return Token(Token::OPERATOR, "/=");
					}
					else {
						return Token(Token::OPERATOR, "/");
					}
	}

	Token getProcentVariants(){
			if (currentChar == '='){
						match('=');
						return Token(Token::OPERATOR, "%%=");
					}
					else {
						return Token(Token::OPERATOR, "%%");
					}
	}

	Token getStarVariants(){
			if ( currentChar == '*' ){
						match('*');
						return Token(Token::OPERATOR, "**"); 
					}
					else if ( currentChar == '=' ){
						match('=');
						return Token(Token::OPERATOR, "*=");
					}
					else {
						return Token(Token::OPERATOR, "*");
					}

	}

	Token getAmpersandVariants(){
			if ( currentChar == '&' ){
				match('&');
				if (currentChar == '='){
					match('=');
					return Token(Token::OPERATOR, "&&="); 
				}
				return Token(Token::OPERATOR, "&&"); 
			}
			else if ( currentChar == '=' ){
				match('=');
				return Token(Token::OPERATOR, "&=");
			}
			else {
				return Token(Token::OPERATOR, "&");
			}

	}

	Token getDashVariants(){
			if (currentChar == '|' ){
				match('|');
				if (currentChar == '='){
					match('=');
					return Token(Token::OPERATOR, "||="); 
				}
				return Token(Token::OPERATOR, "||"); 
			}
			else if ( currentChar == '=' ){
				match('=');
				return Token(Token::OPERATOR, "|=");
			}
			else {
				return Token(Token::OPERATOR, "|");
			}

	}

	Token getWaveVariants(){
			if ( currentChar == '=' ){
				match('=');
				return Token(Token::OPERATOR, "~=");
			}
			else {
				return Token(Token::OPERATOR, "~");
			}

	}

	Token getExclamationMarkVariants(){
		if ( currentChar == '=' ){
				match('=');
				return Token(Token::OPERATOR, "!=");
			}
			else {
				return Token(Token::OPERATOR, "!");
			}
	}




	Token getSymbolicTokens(){

		switch (currentChar) {
					case '#' : match('#'); return getComment();
					case '(' : match('('); return Token(Token::BRACE_LEFT);
					case ')' : match(')'); return Token(Token::BRACE_RIGHT);
					case '[' : match('['); return Token(Token::BRACKET_LEFT);
					case ']' : match(']'); return Token(Token::BRACKET_RIGHT);
					case '{' : match('{'); return Token(Token::CURL_LEFT);
					case '}' : match('}'); return Token(Token::CURL_RIGHT);

			        case ',' : match(','); return Token(Token::COMMA);			       
			        case ';' : match(';'); return Token(Token::SEMICOLON);
			        case '?' : match('?'); return Token(Token::OPERATOR, "?");

			        case EOF: return Token (Token::END, "");

			        case '\'': match ('\''); return getChar();
			        case '\"': match ('\"'); return getString();

			        case '.' : match('.'); return getDotVariants();
					case ':' : match(':'); return getColonVariants();
					case '+' : match('+'); return getPlusVariants();				
					case '-' : match('-'); return getMinusVariats();									
					case '=' : match('='); return getEqualsVariants();		
					case '<' : match('<'); return getLessVariants();					
					case '>' : match('>'); return getGreaterVariants();				
					case '/' : match('/'); return getSlashVariants();
					case '%' : match('%'); return getProcentVariants();
					case '*' : match('*'); return getStarVariants();
					case '&' : match('&'); return getAmpersandVariants();
					case '|' : match('|'); return getDashVariants();
					case '~' : match('~'); return getWaveVariants();
					case '!' : match('!'); return getExclamationMarkVariants();
				default: 
					if (Alphabet::isSpecialSymbol(currentChar)){
						char c = currentChar; 
						consume();
						return Token(Token::OPERATOR, std::string("") + c);
					}
					else{
						throw ParserException(std::string("Unknown character ") + currentChar + '\n');
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
<<<<<<< HEAD
		currentToken = Token(Token::BEGIN, "");
=======
		currentToken = Token(TokenType::BEGIN, "");
>>>>>>> 7595de36625bbe107d0799aefc4c22af4d87f27d
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

#endif
