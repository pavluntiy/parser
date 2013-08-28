#include "common_header.h"
#include "alphabet.h"

#pragma once

enum TokenType {NONE, BEGIN, END, BLOCK_BEGIN, BLOCK_END, COMMENT, SEMICOLON, NAME, 
		OPERATOR, KEYWORD, COMMA, ASSIGN, DOT, INT, FLOAT, CHAR, BOOL
		};

std::string typeToText(TokenType type){
		switch (type){
			case NONE: return "NONE";
			case BEGIN: return "BEGIN";
			case END: return "END";
			case BLOCK_BEGIN: return "BLOCK_BEGIN";
			case BLOCK_END: return "BLOCK_END";
			case COMMENT: return "COMMENT";
			case SEMICOLON: return "SEMICOLON";
			case NAME: return "NAME";
			case OPERATOR: return "OPERATOR";
			case KEYWORD: return "KEYWORD";
			case COMMA: return "COMMA";
			case ASSIGN: return "ASSIGN";
			case DOT: return "DOT";
			case INT: return "INT";
			case FLOAT: return "FLOAT";
			case CHAR: return "CHAR";
			case BOOL: return "BOOL";
		}
	}

class Token { 
public:

	TokenType type;
	std::string text;

public:
	
	std::string getText (){
		return this->text;
	}

	TokenType getType (){
		return this->type;
	}

	Token (TokenType type = NONE, std::string text = ""): type(type), text(text)
	{

	}


	bool operator == (const Token &second)
	const{
		return (this->type == second.type);
	}

	bool operator != (const Token &second)
	const{
		return (this->type != second.type);
	}

	void print(std::ofstream &out, Token &currentToken);

	std::string toString (){
		return (typeToText(this->type) + ", text: " + this->getText() + '\n');
	}
};

std::ostream& operator<< (std::ofstream &out, Token &currentToken){
	out << typeToText(currentToken.type) << ", text: " << currentToken.getText() << '\n';
	return out;
} 

void Token::print(std::ofstream &out, Token &currentToken){
        out << typeToText(currentToken.type) << ", text: " << currentToken.getText() << '\n';
}
