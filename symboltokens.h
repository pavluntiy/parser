#ifndef SYMBOLTOKENS.H
#define SYMBOLTOKENS.H
#include "systemtokens.h"
#include "token.h"
#include "alphabet.h"

class SymbolTokens: public SystemTokens {
protected:
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

public:
	SymbolTokens (std::string input):SystemTokens(input)
	{
	}

	SymbolTokens ()
	{
	}

};
#endif