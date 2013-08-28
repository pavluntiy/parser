#pragma once

#include "common_header.h"

bool isLetter(char c){
        return (('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') || c == '_');
}

bool isDigit(char c){
	return ('0' <= c && c <= '9');
}

bool isWhitespace(char c){
	return (c == ' ' || c == '\r');
}

bool isNewline(char c){
	return (c == '\n');
}

bool isTab(char c){
	return (c == '\t');
} 

bool isEof(char c){
        return (c == EOF);
}

bool isSeparator (char c){
	return (isTab(c) || isNewline(c) || isWhitespace(c) || isEof(c));
}

bool isCommentSign (char c){
	return (c == '#');
}

bool isSpecialSymbol (char c){
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
