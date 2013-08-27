#include "common_header.h"
#include "alphabet.h"

#pragma once
class Token: public Alphabet { 
public:
	enum TokenType {NONE, BEGIN, END, BLOCK_BEGIN, BLOCK_END, NAME, OPERATOR,COMMA, ASSIGN, DOT, INT, FLOAT, CHAR};

protected:
	TokenType type;
	std::string text;

public:
	std::string typeToText(){
		switch (this->type){
			case NONE: return "NONE";
			case BEGIN: return "BEGIN";
			case END: return "END";
			case BLOCK_BEGIN: return "BLOCK_BEGIN";
			case BLOCK_END: return "BLOCK_END";
			case NAME: return "NAME";
			case OPERATOR: return "OPERATOR";
			case COMMA: return "COMMA";
			case ASSIGN: return "ASSIGN";
			case DOT: return "DOT";
			case INT: return "INT";
			case FLOAT: return "FLOAT";
			case CHAR: return "CHAR";
		}
	}
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

/*	void print(std::ofstream &out, const Token &currentToken ){
		out << currentToken.typeToText << ", text: " << currentToken.getText() << '\n';
	}*/

	std::string toString (){
		return (this->typeToText() + ", text: " + this->getText() + '\n');
	}
};

/*std::ostream& operator<< (std::ofstream &out, const Token &currentToken){
	out << currentToken.typeToText << ", text: " << currentToken.getText() << '\n';
	return out;
} */
