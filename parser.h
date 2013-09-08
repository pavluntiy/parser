#ifndef PARSER.H
#define PARSER.H

#include "token.h"
#include "common_header.h"
#include "recognitionexception.h"


class Parser { 

protected:

	int position;
	Tokenstream ts;
	std::vector<int> markers;
	Token currentToken;

	void mark(){
		markers.push_back(position);
	}

	void release(){
		position = markers[(int) markers.size() - 1];
		markers.pop_back();
		currentToken = ts[position];
		//std::cout << position;
	}

	void funcCall(Tokenstream ts){

	}

	void consume(){
	++position;
	currentToken = ts[position];
}

void move(){
	++position;
	currentToken = ts[position];
}

void tryArg(){
		mark();

		if(	currentToken.getType() == Token::NAME || 
			currentToken.getType() == Token::INT ||
			currentToken.getType() == Token::FLOAT ||
			currentToken.getType() == Token::CHAR ||
			currentToken.getType() == Token::STRING ||
			currentToken.getType() == Token::BOOL
			)
		{
			//std::cout << "good\n";
			consume();
			return;
		}
		else {
			release();
		}

		throw RecognitionException();
}


void tryOperation(){

		//std::cout << "operation\n";
		if(currentToken.getType() == Token::OPERATOR){
			consume();
			return;
		}	
		throw RecognitionException();
}



void tryComma(){

	if(currentToken != Token(Token::COMMA)){
		throw RecognitionException();
	}
	consume();
}

void tryBraceLeft(){
	if(currentToken != Token(Token::BRACE_LEFT)){
		throw RecognitionException();
	}
	consume();
}

void tryBraceRight(){
	if(currentToken != Token(Token::BRACE_RIGHT)){
		throw RecognitionException();
	}
	consume();
}




void tryList (){
	try {
		mark();
		tryBraceLeft();
		tryEnum();
		tryBraceRight();
		return;
	}
	catch (RecognitionException re){
		release();
	}

	try {
		mark();
		tryBraceLeft();
		tryBraceRight();
		return;
	}
	catch (RecognitionException re){
		release();
	}


	throw RecognitionException();

}

void tryEnum (){

	try {
		mark();
		tryArg();
		tryEnum();
		return;
	}
	catch(RecognitionException re){
			release();
	} 

	try {
		mark();
		tryArg();


		return;
	}
	catch(RecognitionException re){
			release();
	}



	try {
		mark();
		tryComma();
		tryEnum();
		return;
	}
	catch(RecognitionException re){
			release();
	}

	try {
		mark();
		tryList();


		return;
	}
	catch(RecognitionException re){
			release();
	}

	throw RecognitionException();

}

void tryExpr (){

		try {
			mark();
			tryArg();
			tryOperation();
			tryArg();

			return;
		}
		catch(RecognitionException re){
			release();
		}

		
		try{
			mark();
			tryOperation(); 
			tryExpr();
			return;
		}
		catch(RecognitionException re){
			release();
		}


		try{
			mark();
			tryArg();
			tryOperation();
			tryExpr();

			return;
		}
		catch(RecognitionException re){	
			release();	
		}

		
		try{
			mark();
			tryArg();
			return;
		}
		catch(RecognitionException re){	
			release();
		}

	
		throw RecognitionException();

}





void tryAlternatives(){

	if(currentToken == Token(Token::BEGIN)){
		consume();
	}
	try {
		tryExpr();
		std::cout << "expression on " << position << "\n";
		return;
	}
	catch (RecognitionException re){
	}


	try {
		tryList();
		std::cout << "list on " << position << "\n";
		return;
	}
	catch (RecognitionException re){
	}


	throw RecognitionException(std::string ("Not recognized token ") + currentToken.typeToText() + " " 
		+  currentToken.getText() + std::string(" on postition ") + currentToken.position.toString());

}

public: 
	Parser(Tokenstream ts): ts(ts){
		position = 0;
		currentToken = Token();
		currentToken = ts[position];
	}

	void parse(){
		while (currentToken != Token(Token::END)){
			tryAlternatives();
		}

	}
		


};

#endif