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
	if(position < ts.size()){
		currentToken = ts[position];
	}
	else {
		currentToken = Token(Token::END);
	}
}

void move(){
	++position;
	currentToken = ts[position];
}

void tryOperand(){
	std::cout << "entered operand\n";
	try {

		mark();
		std::cout << "trying function in operand\n";
		tryFunc();	
		std::cout << "found function in operand\n";
		std::cout << "Now on " << position  << " we have " << currentToken.typeToText()<< " in oper\n";
		return;
	}
	catch (RecognitionException re){
		release();
		std::cout << "failed function in operand\n";
	}

	try {
		mark();
		tryArg();	
		return;
	}
	catch (RecognitionException re){
		release();
	}

	try {
		mark();
		tryList();	
		return;
	}
	catch (RecognitionException re){
		release();
	}





	throw RecognitionException();
	
}
void tryArg(){

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

		throw RecognitionException();
}


void tryOperation(std::string text = ""){
	if(text == ""){
		if(currentToken.getType() == Token::OPERATOR){
			consume();
			return;
		}	
	}
	else {
		if(currentToken.getType() == Token::OPERATOR && currentToken.getText() == text){
			consume();
			return;
		}	
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
		tryOperand();
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

int magic;
void tryExpr (){

		try{
			mark();
			std::cout << "entering var 1 with: " << currentToken.typeToText() << "\n";
			magic = 20;
			std::cout << "trying " << currentToken.typeToText() <<  " as operand\n";
			tryOperand();
			std::cout << "found operand var1\n";
			tryOperation();
			std::cout << "found operation var1\n";
			magic = 10;
			tryExpr();
			magic = 20;
			std::cout << "var 1\n";
			std::cout << position  << currentToken.typeToText()<< " in expr\n";
			return;
		}
		catch(RecognitionException re){	
			release();	
		}

		try {
			std::cout << "entering var 2 with: " << currentToken.typeToText() << "\n";
			magic = 20;
			std::cout << "trying " << currentToken.typeToText() <<  " as operand\n";
			tryOperand();
			std::cout << "found operand var2\n";
			tryOperation();
			std::cout << "found operation var2\n";
			std::cout << "trying " << currentToken.typeToText() <<  " as second operand\n";
			tryOperand();
			std::cout << "found second operand var2\n";;
			std::cout << magic << " var 2\n";

			return;
		}
		catch(RecognitionException re){
			release();
		}



		
		try{
			mark();
			std::cout << position << "!!!!!!";
			tryOperation(); 
			std::cout << position << "asdf";
			tryOperand();
			std::cout << "var 3\n";
			return;
		}
		catch(RecognitionException re){
			release();
		}

		
		try{
			mark();
			tryOperand();
			std::cout << "var 4\n";
				std::cout << position  << currentToken.typeToText()<< " in expr\n";
			return;
		}
		catch(RecognitionException re){	
			release();
		}

	
		throw RecognitionException();

}

void tryIdle(){
	return;
}

void tryName(){
	if(currentToken == Token(Token::NAME)){
		consume();
		return;	
	}

	throw RecognitionException();
}

void tryNameList(){
	try {
		mark();
		tryName();
		tryNameList();
		//std::cout << position << '\n';
		return;
	}
	catch (RecognitionException re){
		release();
	}


	try {
		mark();
		tryName();
		return;
	}
	catch (RecognitionException re){
		release();
	}

	
	throw RecognitionException();
}

void tryFuncCall(){

	try {

		mark();
		tryNameList();
		std::cout << "trying namelist with :=\n";
		tryOperation(":=");
		std::cout << "found namelist with :=\n";
		tryArg();
		std::cout << "found arg\n";
		return;
	}
	catch (RecognitionException re){
		release();
	}

	try {
		mark();
		std::cout << "trying namelist\n";
		tryNameList();
		std::cout << "found namelist\n";
		return;
	}
	catch (RecognitionException re){
		release();
	}

	

	throw RecognitionException();

}

void trySemicolon(){
	if(currentToken.getType() == Token::SEMICOLON){
		consume();
		return;	
	}

	throw RecognitionException();
}

void tryNewBlock(){
	if(currentToken.getType() == Token::BLOCK_BEGIN){
		consume();
		return;	
	}

	throw RecognitionException();

}

void tryFuncDecl(){
	try {
		mark();
		tryName();
		tryName();
		tryBraceLeft();
		tryFuncCall();
		tryBraceRight();
		tryOperation(":");
		tryNewBlock();
		
		return;
	}
	catch (RecognitionException re){
		release();
	}

	try {
		mark();
		tryName();
		tryName();
		tryBraceLeft();
		tryFuncCall();
		tryBraceRight();
		trySemicolon();
		
		return;
	}
	catch (RecognitionException re){
		release();
	}

	throw RecognitionException();
}


void tryFunc(){
	try {
		mark();
		tryName();
		tryBraceLeft();
		tryFuncCall();
				std::cout <<position << " ololo!\n";
				std::cout << currentToken.typeToText();
		tryBraceRight();
		std::cout << currentToken.typeToText() << "??\n";
		
		return;
	}
	catch (RecognitionException re){
		release();
	}

	try {
		mark();
		tryName();
		tryBraceLeft();
		tryBraceRight();
		return;
	}
	catch (RecognitionException re){
		release();
	}


	throw RecognitionException();
}

void tryAlternatives(){

	if(currentToken == Token(Token::BEGIN)){
		consume();
		return;
	}

	if(currentToken == Token(Token::BLOCK_END)){
		consume();
		return;
	}

	if(currentToken == Token(Token::END)){
		consume();
		return;
	}

	if(currentToken == Token(Token::COMMENT)){
		consume();
		return;
	}

	try {
		tryFuncDecl();
		std::cout << "funcdecl on " << position << "\n";
		return;
	}
	catch (RecognitionException re){
	}

	try {
		tryFunc();
		std::cout << "func call on " << position << "\n";
		return;
	}
	catch (RecognitionException re){
	}




	try {
		std::cout << "entering expression\n";
		tryExpr();
		std::cout << "expression on " << position << "\n";
		std::cout << currentToken.typeToText();
		return;
	}
	catch (RecognitionException re){
	}

	try {
		tryNameList();
		std::cout << "Name List on " << position << "\n";
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