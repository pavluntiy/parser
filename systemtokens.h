#ifndef SYSTEMTOKENS.H
#define SYSTEMTOKENS.H
#include "common_header.h"
#include "token.h"
#include "alphabet.h"
#include "recognizer.h"


class SystemTokens: public Recognizer {
protected:
	Token getWord() {
		std::string buffer;

		while(Alphabet::isLetter(currentChar) || Alphabet::isDigit(currentChar)){
			buffer += currentChar;
			consume();
		}

		if (isBoolConstant(buffer)){
			return makeToken(Token::BOOL, buffer);
		}

		if (isKeyWord(buffer)){
			return makeToken(Token::KEYWORD, buffer);
		}

		return makeToken(Token::NAME, buffer);

	}

	bool tryComment (){
		//std::cout << ((find("#") || find("/*"))?"Found comment on " + sourcePosition.toString() + "\n": "");
		return (find("#") || find("/*")); 
	}

	Token getComment (){
		std::string buffer;
		while (!Alphabet::isNewline(currentChar) && !Alphabet::isEof(currentChar)){
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


	Token tryAndGetHexadecimal(bool zeroFound){
		Token::Position startPosition = sourcePosition;
		std::string buffer;
		bool isFloat = false;
		if (currentChar == 'X' || currentChar == 'x'){
			if (zeroFound){
			buffer = "0x";
				consume();
				if (!Alphabet::isHexadecimalDigit(currentChar)){
					throw ParserException("Hexadecimal not recognized " + startPosition.toString() + "\n");
				}
				while (Alphabet::isHexadecimalDigit(currentChar) || currentChar == '_' || currentChar == '.' && !find("..")){
					if (currentChar == '.'){
						isFloat = true;
					}
					buffer += currentChar;
					consume();
				}
			}
			buffer += std::string(" (hexadecimal)");
			if (isFloat){
				return makeToken(Token::FLOAT, buffer);
			}
			return makeToken(Token::INT, buffer);
		}

		return makeToken(Token::NONE);
	}

	Token tryAndGetBinar(bool zeroFound){
		Token::Position startPosition = sourcePosition;
		std::string buffer;
		bool isFloat = false;
		if (currentChar == 'B' || currentChar == 'b'){
			if (zeroFound){
				buffer = "0b";
				consume();
				if (!Alphabet::isBinaryDigit(currentChar)){
					throw ParserException("Binar not recognized " + startPosition.toString()  + "\n");
				}
				while (Alphabet::isBinaryDigit(currentChar) || currentChar == '_' || currentChar == '.' && !find("..")){
					if (currentChar == '.'){
						isFloat = true;
					}
					buffer += currentChar;
					consume();
				}
			}
			buffer += std::string(" (binar)");
			if (isFloat){
				return makeToken(Token::FLOAT, buffer, startPosition);
			}
			return makeToken(Token::INT, buffer, startPosition);
		}

		return makeToken(Token::NONE);
	}

	Token tryAndGetOctal(bool zeroFound){
		Token::Position startPosition = sourcePosition;
		std::string buffer;
		bool isFloat = false;

		if (zeroFound){
			buffer = "0";
			while (Alphabet::isOctalDigit(currentChar)|| currentChar == '_' || currentChar == '.' && !find("..")){
					if (currentChar == '.'){
						isFloat = true;
					}
					buffer += currentChar;
					consume();							
			}
			buffer += std::string(" (octal)");
			if (isFloat){
				return makeToken(Token::FLOAT, buffer, startPosition);
			}
			return makeToken(Token::INT, buffer, startPosition);
		}

		return makeToken(Token::NONE);
	}

	Token tryAndGetDecimal(bool zeroFound){
		Token::Position startPosition = sourcePosition;
		std::string buffer;
		bool isFloat = false;
		if (Alphabet::isDigit(currentChar)){
			while (Alphabet::isDigit(currentChar)|| currentChar == '_' || currentChar == '.' && !find("..")){
				if (currentChar == '.'){
							isFloat = true;
						}
						buffer += currentChar;
						consume();							
			}


			if (isFloat){
				return makeToken(Token::FLOAT, buffer, startPosition);
			}
			return makeToken(Token::INT, buffer, startPosition);

		}

		return makeToken(Token::NONE);
	}

	Token tryAndGetNumeric(){
		Token numeric;
		bool zeroFound = false;

		if (currentChar == '0'){
			zeroFound = true;
			consume();
		}

		numeric = tryAndGetHexadecimal(zeroFound);
		if (numeric != makeToken(Token::NONE)){
			return numeric;
		}

		numeric = tryAndGetBinar(zeroFound);
		if (numeric != makeToken(Token::NONE)){
			return numeric;
		}

		numeric = tryAndGetOctal(zeroFound);
		if (numeric != makeToken(Token::NONE)){
			return numeric;
		}
		

		numeric = tryAndGetDecimal(zeroFound);
		if (numeric != makeToken(Token::NONE)){
			return numeric;
		}

		return makeToken(Token::NONE);
	}
	
	Token getSystemTokens (){
			

			if (Alphabet::isNewline(currentChar)){
				int tabCount = 0, whitespaceCount = 0;
				while (Alphabet::isNewline(currentChar)){
					whitespaceCount = 0;
					tabCount = 0;
					while (Alphabet::isNewline(currentChar)){
						consume();
					}
					while (Alphabet::isTab(currentChar) || Alphabet::isWhitespace(currentChar)){
						if (Alphabet::isWhitespace(currentChar)){
							++whitespaceCount;
							consume();
						}

						if (whitespaceCount % WS_IN_TAB == 0 && whitespaceCount != 0){
							++tabCount;
						}

						if (Alphabet::isTab(currentChar)){
							++tabCount;
							whitespaceCount = 0;
							consume();
						}
						
					}
				}	

				if (!tryComment()  && isNewBlock(tabCount)){
					{
					//std::cout << "tried comment"  << tryComment() << "\n";
				        return makeToken(Token::BLOCK_BEGIN, "");
				    }
				}
				
				if (!tryComment() && isOldBlock(tabCount)){

				        return makeToken(Token::BLOCK_END, "");
				 }

				while (Alphabet::isWhitespace(currentChar)){
					consume();
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
				return getWord();
			}

			return tryAndGetNumeric();

	}

		static bool isCommentBegin (const std::string& str){
			if (str == "/*" || str == "#")
			return true;
			return false;
		}
public:
	SystemTokens (std::string input): Recognizer(input)
	{
	}

	SystemTokens ()
	{		
	}

};

#endif