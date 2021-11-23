#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <ctype.h>
#include <vector>


using namespace std;

class Lexer
{
public:
	bool isSeparator(char thing);	//Checks Separator
	bool isOperator(char thing);	//Checks Operator
	bool isKeyword(string thing);	//Checks Keyword

	void print(ofstream& testFile); //Prints Token and Lexeme

	void addToken(string newToken);		//Add Token to Vector
	void addLexeme(string newLexeme);	//Add Lexeme to Vector

	std::vector<string> getToken();	//Retrieve Token
	std::vector<string> getLexeme(); //Retrieve Lexeme

	int RealorIntFSM(string thing);		//Real or Integer Finite State Machine
	bool identifierFSM(string thing);	//Identifier Finite State Machine

	void analyzer(ifstream& textFile, string thing); //Reads in file and runs lexical analysis on the contents

private:
	char things;
	std::vector<string> token, lexeme;
};
