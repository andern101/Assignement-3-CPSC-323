#pragma once
#define PARSER_H
#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <ctype.h>
#include <vector>
#include <stack>
#include <utility>
#include <algorithm>

void Rat20F(std::vector<std::string> lexemeVec, std::vector<int> count, std::vector<std::string> tokenVec, std::ofstream& output);
void Parse(std::string& analyzerlexeme, std::ofstream& output);
void OptDeclarationList(std::string& analyzerlexeme, std::ofstream& output);
void DeclarationList(std::string& analyzerlexeme, std::ofstream& output);
void DeclarationListPrime(std::string& analyzerlexeme, std::ofstream& output);
void Declaration(std::string& analyzerlexeme, std::ofstream& output);
void Qualifier(std::string& analyzerlexeme, std::ofstream& output);
void StatementList(std::string& analyzerlexeme, std::ofstream& output);
void Statement(std::string& analyzerlexeme, std::ofstream& output);
void StatementListPrime(std::string& analyzerlexeme, std::ofstream& output);
void Compound(std::string& analyzerlexeme, std::ofstream& output);
void Assign(std::string& analyzerlexeme, std::ofstream& output);
void If(std::string& analyzerlexeme, std::ofstream& output);
void Get(std::string& analyzerlexeme, std::ofstream& output);
void Put(std::string& analyzerlexeme, std::ofstream& output);
void While(std::string& analyzerlexeme, std::ofstream& output);
void Condition(std::string& analyzerlexeme, std::ofstream& output);
void Relop(std::string& analyzerlexeme, std::ofstream& output);

void Expression(std::string& analyzerlexeme, std::ofstream& output);
void ExpressionPrime(std::string& analyzerlexeme, std::ofstream& output);
void Term(std::string& analyzerlexeme, std::ofstream& output);
void TermPrime(std::string& analyzerlexeme, std::ofstream& output);
void Factor(std::string& analyzerlexeme, std::ofstream& output);
void Primary(std::string& analyzerlexeme, std::ofstream& output);


void Empty(std::string& analyzerlexeme, std::ofstream& output);
void Print(std::ofstream& output);

void gen_instr(std::string op, int oprnd);
void back_patch(int jump_addr);
void gen_symbol(std::string& userLex, std::string type, std::ofstream& fileout);
int get_address(std::string userLex);
std::string getTokenType(std::string lex);
void checkMatch(std::string prevLex, std::string userLex, std::string prevToken, std::string userToken, std::ostream& fileout);
bool check_symbol(std::string userLex);

struct symbol
{
	std::string lex;
	int mem_loc;
	std::string type;
};

struct instruction
{
	int address;
	std::string op;
	int oprnd;
};
