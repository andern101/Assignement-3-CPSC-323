#pragma once //??

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "Lexer.h"
#include "Parser.h"


using namespace std;

class instruc {

private:
	std::string op, oprnd;
	int address;


public:
	std::string getOp() { return this->op; }
	std::string getOprnd() { return this->oprnd; }
	int getAddress() { return this->address; }

	void setOprnd(std::string s) { this->oprnd = s; }
	void setOp(std::string s) { this->op = s; }
	void setAddress(int s) { this->address = s; }

	instruc(int i, std::string s, std::string t) {
		setOprnd(t);
		setOp(s);
		setAddress(i);
	}
};

int instr_address = 1;
std::vector<instruc> Instr_table;

void gen_instr(std::string op, std::string oprnd)
//not sure if were suppose to include everything on that code for his partial solution document
{
	Instr_table.push_back(instruc(instr_address, op, oprnd));
	instr_address++;
};

void print_Instr(std::ofstream& out) {
	out << "\n";
	std::vector<instruc>::iterator i = Instr_table.begin();
	while (i != Instr_table.end()) {
		out << i->getAddress() << "\t" << i->getOp() << "\t" << i->getOprnd() << "\n";
		i++;
	}
}

std::vector<Lexer> arith_Table;

void arith_Error() {
	//message
	//cout << "error on:" << lineNum?? << endl;
}

void arith_Check() {
//dont know
}

std::vector<int> jumpStack;

void push_jumpStack(int address) {
	jumpStack.push_back(address);
}

int pop_jumpStack() {
	int save = jumpStack.back();
	jumpStack.pop_back();
	return save;
}

void back_patch(int jump_addr)
{
	int addr = pop_jumpStack();
	Instr_table[addr].setOprnd(std::to_string(jump_addr));
}
