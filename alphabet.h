#ifndef ALPHABET.H 
#define ALPHABET.H 

#include "common_header.h"


class Alphabet {
public:
	static	bool isLetter(char c){
	        return (('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') || c == '_');
	}

	static bool isDigit(char c){
		return ('0' <= c && c <= '9');
	}

	static bool isBinaryDigit(char c){
		return ('0' <= c && c <= '1');
	}

	static bool isOctalDigit(char c){
		return ('0' <= c && c <= '7');
	}

	static bool isHexadecimalDigit(char c){
		return ('0' <= c && c <= '1' || 'A' <= c && c <= 'F' || 'a' <= c && c <= 'f');
	}


	static bool isWhitespace(char c){
		return (c == ' ' || c == '\r');
	}

	static bool isNewline(char c){
		return (c == '\n');
	}

	static bool isTab(char c){
		return (c == '\t');
	} 

	static bool isEof(char c){
	        return (c == EOF);
	}

	static bool isSeparator (char c){
		return (isTab(c) || isNewline(c) || isWhitespace(c) || isEof(c));
	}

	static bool isCommentSign (char c){
		return (c == '#');
	}
	static bool isSpecialSymbol (char c){
		bool isTrue = false;


		isTrue = isTrue || (c == ';');
		isTrue = isTrue || (c == '[' || c ==']');
		isTrue = isTrue || (c == '+'|| c == '/' || c == '*' || c == '%' || c == '-');
		isTrue = isTrue || (c == '&' || c == '|' || c == '^' || c == '~' || c == '!');
		isTrue = isTrue || (c == ',' || c == '.');
		isTrue = isTrue || (c == '<' || c == '>');
		isTrue = isTrue || (c == '{' || c == '}');
		isTrue = isTrue || (c == '?' || c == ':');
		isTrue = isTrue || (c == '\\' || c == '\'' || c == '\"');
		isTrue = isTrue || (c == '@' || c == '$');

		return isTrue;

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

//it's just an example, you shouldn't think, that we'll store our keywords like this;
//I was just going to write a sample program in Pseudocode in order to prove correctness of my shit	
	static bool isKeyWord (const std::string& str){
		if(str == "if" || str == "else" || str == "for" || str == "int" || str == "auto" || str == "using" 
			|| str == "return" || str == "end")
			return true;
		return false;
	}

	static bool isEnd (const std::string& str){
		return (str == "end");
	}
};
#endif

