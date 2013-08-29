#ifndef TOKEN.H 
#define TOKEN.H 
#include "common_header.h"


class Token{ 
public:
	enum TokenType {NONE, BEGIN, END, BLOCK_BEGIN, 
					BLOCK_END, COMMENT, SEMICOLON, 
					NAME, 
					OPERATOR, 
					BRACE_LEFT, BRACE_RIGHT,
					BRACKET_LEFT, BRACKET_RIGHT,
					CURL_LEFT, CURL_RIGHT,
					KEYWORD, 
					COMMA, DOT,
					ASSIGN,  
					INT, FLOAT, CHAR, BOOL, STRING};

struct Position {
	int line, linePosition;

	Position (int line = 0, int linePosition = 0): line(line), linePosition(linePosition){

	} 
};
Position position;
std::string typeToText(){
		switch (this->type){
			case NONE: return "NONE";
			case BEGIN: return "BEGIN";
			case END: return "END";
			case BLOCK_BEGIN: return "BLOCK_BEGIN";
			case BLOCK_END: return "BLOCK_END";
			case COMMENT: return "COMMENT";
			case SEMICOLON: return "SEMICOLON";
			case NAME: return "NAME";
			case OPERATOR: return "OPERATOR";
			case BRACE_RIGHT: return "BRACE_RIGHT";
			case BRACE_LEFT: return "BRACE_LEFT";
			case BRACKET_RIGHT: return "BRACKET_RIGHT";
			case BRACKET_LEFT: return "BRACKET_LEFT";
			case CURL_RIGHT: return "CURL_RIGHT";
			case CURL_LEFT: return "CURL_LEFT";
			case KEYWORD: return "KEYWORD";
			case COMMA: return "COMMA";
			case ASSIGN: return "ASSIGN";
			case DOT: return "DOT";
			case INT: return "INT";
			case FLOAT: return "FLOAT";
			case CHAR: return "CHAR";
			case BOOL: return "BOOL";
			case STRING: return "STRING";
		}
		return "ERROR";
	}


	TokenType type;
	std::string text;

public:
	
	std::string getText (){
		return this->text;
	}

	TokenType getType (){
		return this->type;
	}

/*	Token (TokenType type = NONE, std::string text = ""): type(type), text(text)
	{

	}
 //I'd like to write something like this, but I don't know C++!
/*	Token (TokenType type = NONE, std::string text = "", Position position = {-1, -1}): type(type), text(text), position(position)
	{

	}
	*/


/*	Token (TokenType type = NONE, char* text = "", Position position = {-1, -1}): type(type), text(std::string(text)), position(position)
	{
	}*/

	Token (){
		
	}

	Token (TokenType type, char* text, Position position): type(type), text(std::string(text)), position(position)
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

};

std::ofstream& operator<< (std::ofstream &out, Token::Position &position){
	out << "line: " << position.line << ", position: " << position.linePosition;
	return out;
}

std::ofstream& operator<< (std::ofstream &out, Token &currentToken){

	out << currentToken.typeToText();
	if (currentToken.getText() != "") {
		out << ", text: " << currentToken.getText();
	}
	out << " (";
	out << currentToken.position; //I'm to lazy to investigate: I can't compile it if I join this three lines in one "out" operator.
	out << ")\n";
	return out;
} 
#endif

