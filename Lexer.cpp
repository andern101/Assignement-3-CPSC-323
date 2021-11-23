#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <ctype.h>
#include <vector>
#include "Lexer.h"

bool Lexer::isKeyword(string thing)
{
	if (thing == "int" || thing == "if" || thing == "else" || thing == "fi" || thing == "while"
		|| thing == "return" || thing == "get" || thing == "put" || thing == "do" || thing == "for"
		|| thing == "and" || thing == "or" || thing == "real" || thing == "bool" || thing == "function"
	   	|| thing == "true" || thing == "false" || thing == "boolean")
		return true;
	else
		return false;
}

bool Lexer::isSeparator(char thing)
{
	if (thing == ';' || thing == ')' || thing == ':' || thing == '(' || thing == '{' || thing == '}'
		|| thing == ',' || thing == '[' || thing == ']' || thing == '?' || thing == '@' || thing == '$')
		return true;
	else
		return false;
}

bool Lexer::isOperator(char thing)
{
	if (thing == '+' || thing == '-' || thing == '*' || thing == '=' || thing == '<' || thing == '>'
		|| thing == '%' || thing == '!' || thing == '/')
		return true;
	else
		return false;
}

void Lexer::print(ofstream& outputFile)
{
	for (int i = 0; i < token.size(); i++)
	{
		cout << left << setw(30) << token[i] << "\t" << lexeme[i] << endl;
		outputFile << left << setw(30) << token[i] << "\t" << lexeme[i] << endl;
	}
}

void Lexer::addToken(string newToken)
{
	token.push_back(newToken);
}

void Lexer::addLexeme(string newLexeme)
{
	lexeme.push_back(newLexeme);
}

std::vector<string> Lexer::getToken()
{
	return token;
}

std::vector<string> Lexer::getLexeme()
{
	return lexeme;
}


//Finite State Machine for Real or Integer
int Lexer::RealorIntFSM(string thing)
{
	//Finite State Machine Table
	int FSM[5][3] = {   //Accepting States is 4
		0, 'i', '.',    //Finite State Machine Table
		1,  2,   0,     //Starting State
		2,  2,   3,     // Integer
		3,  4,   0,     // Period
		4,  4,   0 };   // Digit with Period Already

	int curState = 1; //Starting State

	for (int i = 0; i < thing.size(); i++)
	{
		if (isdigit(thing[i])) //Convert Current State to table for each item
			curState = FSM[curState][1];
		else if (thing[i] == '.')
			curState = FSM[curState][2];
		else
			return 0;
	}

	if (curState == 4) //Checks Accepting States is 4 for Real
		return 1;
	else if (curState == 2) //Checks Accepting States is 2 for Integer
		return 2;
	else
		return 0;
}

//Finite State Machine for identifier
bool Lexer::identifierFSM(string thing)
{
	//Finite State Machine Table
	int FSM[5][4] = {
		0, 'i', 'l', '_',
		1,  0,   2,   0,   //Starting State
		2,  3,   2,   4,   //Upper/Lower Case Letter
		3,  2,   3,   4,   //Integer
		4,  2,   2,   4 }; // '_' 

	int curState = 1; //Starting State

	for (int i = 0; i < thing.size(); i++)
	{
		if (isdigit(thing[i])) //Convert Current State to table for each item
			curState = FSM[curState][1];
		else if (isalpha(thing[i]))
			curState = FSM[curState][2];
		else if (thing[i] == '_')
			curState = FSM[curState][3];
		else
			return false;
	}

	if (curState == 2 || curState == 3 || curState == 4) //Checks Accepting States is 2, 3 and 4
		return true;
	else
		return false;
}

void Lexer::analyzer(ifstream& textFile, string thing)
{
	bool curState = false;
	char peek;

	if (isSeparator(thing[0])||thing == "$$")  //Checks for Separator
	{
		peek = textFile.peek();
		if (thing[0] == '$' && peek == '$') //Checks for $$ and adds to string 
		{
			thing += peek;
			textFile.ignore();
		}

		addLexeme(thing); //Set Lexmeme
		addToken("Separator");
	}
	else if (isOperator(thing[0]))	//Checks for Operator
	{
		peek = textFile.peek();
		//Check for valid operators: ==, !=,<=,=> and add peek to string
		if ((thing[0] == '=' && peek == '=') || (thing[0] == '!' && peek == '=')
			|| (thing[0] == '<' && peek == '=') || (thing[0] == '=' && peek == '>'))
		{
			thing += peek;
			textFile.ignore();
		}
		addLexeme(thing); //Set Lexmeme
		addToken("Operator");
	}
	else if (isalpha(thing[0])) //Checks for Keyword,or Identifier
	{
		addLexeme(thing); //Set Lexmeme

		if (identifierFSM(thing) == true) //Declare whether Keyword, Identifier, or Invalid
		{
			if (isKeyword(thing))
				addToken("Keyword");
			else
				addToken("Identifier");
		}
		else
			addToken("Invalid Identifier");
	}
	else if (isdigit(thing[0])) //Checks for Real Numbers or Integer
	{
		int digitCheck = RealorIntFSM(thing);
		addLexeme(thing); //Set Lexmeme
		if (digitCheck == 1) //Declare whether Real Number, Integer or Invalid
			addToken("Real");
		else if (digitCheck == 2)
			addToken("Integer");
		else
			addToken("Invalid Real/Integer");
	}
	else
	{
		addToken("Invalid Input");
		addLexeme(thing);
	}
}
