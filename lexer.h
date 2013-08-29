#ifndef LEXER.H 
#define LEXER.H 
#include "common_header.h"
#include "alphabet.h"
#include "token.h"
//#include "consument.h"


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
			previous.push(currentTabDepth);
			currentTabDepth = tabCount;
			return true;
		}
		return false;
	}

	bool isOldBlock (int tabCount){
		if (tabCount < currentTabDepth){
			while (!previous.empty() && previous.top() != tabCount){
				
				tokenList.push_back(makeToken(Token::BLOCK_END));

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


public:
	Token makeToken (Token::TokenType type = Token::NONE, std::string text = ""){
		return Token(type, const_cast<char *> (text.c_str()), sourcePosition);
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
			throw ParserException("Unexpected end of file");
		}
		
		currentPosition++; 
		
		
		
		if ( currentPosition >= (int)input.length() )
			currentChar = EOF;
		else {
			currentChar = input[currentPosition]; 
		}
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
			return makeToken(Token::BOOL, buffer);
		}

		if (Alphabet::isKeyWord(buffer)){
			return makeToken(Token::KEYWORD, buffer);
		}

		return makeToken(Token::NAME, buffer);

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
		return makeToken(Token::COMMENT, buffer);
	}
	
	Token getMultyLineComment () {
		Token::Position startPosition = sourcePosition;
		std::string buffer;
		while (!get("*/")){
			if (Alphabet::isEof(currentChar)){
				throw ParserException("Unfinished multiline comment on " + startPosition.toString());
			}
			buffer += currentChar;
			consume();
		}
		if (!Alphabet::isEof(currentChar)){
			consume();
		}
		return makeToken(Token::COMMENT, buffer);
	}

	Token getChar(){
		Token::Position startPosition = sourcePosition;
		std::string buffer;
		buffer += '\'';
		while (currentChar != '\''){
			if (Alphabet::isEof(currentChar)){
				throw ParserException("Unfinished charecter symbol on " + startPosition.toString());
			}
			buffer += currentChar;
			consume();		
		}
		consume();
		buffer += '\'';
		return makeToken(Token::CHAR, buffer); 
	}

	Token getString(){
		Token::Position startPosition = sourcePosition;
		std::string buffer;
		buffer += '\"';
		while (currentChar != '\"'){
			if (Alphabet::isEof(currentChar)){
				throw ParserException("Unfinished string on " + startPosition.toString());
			}
			buffer += currentChar;
			consume();
		}
		consume();
		buffer += '\"';
		return makeToken(Token::STRING, buffer); 
	}

	Token tryAndGetNumeric(){
		Token::Position startPosition = sourcePosition;
		std::string buffer;
		bool isFloat = false;
		bool zeroFound = false;
		bool numericDetected = false;

		if (currentChar == '0'){
			zeroFound = true;
			buffer += currentChar;
			numericDetected = true;
			consume();
		}

		if (currentChar == 'X' || currentChar == 'x'){
			if (zeroFound){
			buffer += 'x';
				consume();
				if (!Alphabet::isHexadecimalDigit(currentChar)){
					throw ParserException("Hexadecimal not recognized " + startPosition.toString());
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
					throw ParserException("Binary not recognized " + startPosition.toString());
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

		if (numericDetected){
			if (isFloat){
				return makeToken(Token::FLOAT, buffer);
			}
			return makeToken(Token::INT, buffer);
		}
		else return makeToken(Token::NONE);		
	}
	
	Token getSystemTokens (){
			while (Alphabet::isWhitespace(currentChar)){
				consume();
			}

			if (Alphabet::isNewline(currentChar)){
				int tabCount;
				while (Alphabet::isNewline(currentChar)){
					while (Alphabet::isNewline(currentChar)){
						consume();
					}
					tabCount = countAndConsumeTabs();
				}	
				

				if (!tryComment() && isNewBlock(tabCount)){
				        return makeToken(Token::BLOCK_BEGIN, "");
				}
				
				if (!tryComment() && isOldBlock(tabCount)){
				        return makeToken(Token::BLOCK_END, "");
				 }

				if (isNewBlock(tabCount)) {
					return makeToken(Token::BLOCK_BEGIN, "");
				}
				
				if (isOldBlock(tabCount)) {
					return makeToken(Token::BLOCK_END, "");
				}
			}

			while (Alphabet::isWhitespace(currentChar)){
				consume();
			}

			while (Alphabet::isTab(currentChar)){
				consume();
			}

			if (get("/*")){
				return getMultyLineComment();
			}

			if (Alphabet::isLetter(currentChar)){
				return getName();
			}

			return tryAndGetNumeric();

	}

	Token getDotVariants (){
		 if (currentChar == '.'){
			         	match ('.');
			         	if (currentChar == '.'){
			         		match('.');
			         		return makeToken(Token::OPERATOR, "...");
			         	}
			         	else {
			         		return makeToken(Token::OPERATOR, "..");
			         	}
			        }
			        else {
			         	return makeToken(Token::DOT, "." ); 
			     	}
	}

	Token getPlusVariants (){
		if ( currentChar == '+' ){
						match('+');
						return makeToken(Token::OPERATOR, "++"); 
					}
					else if ( currentChar == '=' ){
						match('=');
						return makeToken(Token::OPERATOR, "+=");
					}
					else {
						return makeToken(Token::OPERATOR, "+");
					}
	}


	Token getColonVariants(){
		if ( currentChar == '=' ) {
						match('=');
						return makeToken(Token::OPERATOR, ":="); 
					}
					else if (currentChar == ':'){
						match(':');
						if (currentChar == '='){
							return makeToken(Token::OPERATOR, "::=");
						}
						else {
							return makeToken(Token::OPERATOR, "::");
						}
					}
					else {
						return makeToken(Token::OPERATOR, ":");
					}
	}

	Token getMinusVariats(){
		if (currentChar == '>'){
						return makeToken(Token::OPERATOR, "->");
					}
					else if (currentChar == '-' ){
						match('-');
						if (currentChar == '>'){
							return makeToken(Token::OPERATOR, "-->");
						}
						else {
							return makeToken(Token::OPERATOR, "--");
						}
					}
					else if ( currentChar == '=' ){
						match('=');
						return makeToken(Token::OPERATOR, "-=");
					}
					else {
						return makeToken(Token::OPERATOR, "-");
					}
	}

	Token getEqualsVariants(){
		if (currentChar == '='){
						match('=');
						return makeToken(Token::OPERATOR, "==");
					}
					else {
						return makeToken(Token::OPERATOR, "=");
					}
	}

	Token getLessVariants(){
		if (currentChar == '='){
						match('=');
						return makeToken(Token::OPERATOR, "<=");
					}
					else if (currentChar == '<'){
						match('<');
						return makeToken(Token::OPERATOR, "<<");
					}
					else {
						return makeToken(Token::OPERATOR, "<");

					}

	}

	Token getGreaterVariants(){
		if (currentChar == '='){
						match('=');
						return makeToken(Token::OPERATOR, ">=");
					}
					else if (currentChar == '>'){
						match('>');
						if (currentChar == '>'){
							match('>');
							return makeToken(Token::OPERATOR, ">>>");
						}
						else {
							return makeToken(Token::OPERATOR, ">>");
						}
					}
					else {
						return makeToken(Token::OPERATOR, ">");

					}
	}

	Token getSlashVariants(){
			if (currentChar == '/'){
						match('/');
						return makeToken(Token::OPERATOR, "//");
					}
					else if (currentChar == '='){
						match('=');
						return makeToken(Token::OPERATOR, "/=");
					}
					else {
						return makeToken(Token::OPERATOR, "/");
					}
	}

	Token getProcentVariants(){
			if (currentChar == '='){
						match('=');
						return makeToken(Token::OPERATOR, "%%=");
					}
					else {
						return makeToken(Token::OPERATOR, "%%");
					}
	}

	Token getStarVariants(){
			if ( currentChar == '*' ){
						match('*');
						return makeToken(Token::OPERATOR, "**"); 
					}
					else if ( currentChar == '=' ){
						match('=');
						return makeToken(Token::OPERATOR, "*=");
					}
					else {
						return makeToken(Token::OPERATOR, "*");
					}

	}

	Token getAmpersandVariants(){
			if ( currentChar == '&' ){
				match('&');
				if (currentChar == '='){
					match('=');
					return makeToken(Token::OPERATOR, "&&="); 
				}
				return makeToken(Token::OPERATOR, "&&"); 
			}
			else if ( currentChar == '=' ){
				match('=');
				return makeToken(Token::OPERATOR, "&=");
			}
			else {
				return makeToken(Token::OPERATOR, "&");
			}

	}

	Token getDashVariants(){
			if (currentChar == '|' ){
				match('|');
				if (currentChar == '='){
					match('=');
					return makeToken(Token::OPERATOR, "||="); 
				}
				return makeToken(Token::OPERATOR, "||"); 
			}
			else if ( currentChar == '=' ){
				match('=');
				return makeToken(Token::OPERATOR, "|=");
			}
			else {
				return makeToken(Token::OPERATOR, "|");
			}

	}

	Token getWaveVariants(){
			if ( currentChar == '=' ){
				match('=');
				return makeToken(Token::OPERATOR, "~=");
			}
			else {
				return makeToken(Token::OPERATOR, "~");
			}

	}

	Token getExclamationMarkVariants(){
		if ( currentChar == '=' ){
				match('=');
				return makeToken(Token::OPERATOR, "!=");
			}
			else {
				return makeToken(Token::OPERATOR, "!");
			}
	}




	Token getSymbolicTokens(){

		switch (currentChar) {
					case '#' : match('#'); return getComment();
					case '(' : match('('); return makeToken(Token::BRACE_LEFT);
					case ')' : match(')'); return makeToken(Token::BRACE_RIGHT);
					case '[' : match('['); return makeToken(Token::BRACKET_LEFT);
					case ']' : match(']'); return makeToken(Token::BRACKET_RIGHT);
					case '{' : match('{'); return makeToken(Token::CURL_LEFT);
					case '}' : match('}'); return makeToken(Token::CURL_RIGHT);

			        case ',' : match(','); return makeToken(Token::COMMA);			       
			        case ';' : match(';'); return makeToken(Token::SEMICOLON);
			        case '?' : match('?'); return makeToken(Token::OPERATOR, "?");

			        case EOF: return makeToken (Token::END, "");

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
						return makeToken(Token::OPERATOR, std::string("") + c);
					}
					else{
						if (!Alphabet::isAcceptableCharacter(currentChar)){
							throw ParserException(std::string("Unknown character ") + currentChar + '\n');
						}
					}

			}

			return makeToken(Token::NONE);

	}

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
		freeBlocksStack();
		tokenList.push_back(currentToken);
	}

public: 


	Lexer (std::string input): input(input), currentPosition(0){
		currentChar = input[currentPosition];
		currentTabDepth = 0;
	}

	const std::vector<Token>& getTokenList(){
		tokenize();
		return this->tokenList;
	}
	
};

#endif
