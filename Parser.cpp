#pragma once
#include "Parser.h"
#define PARSER_H
#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <ctype.h>
#include <vector>
#include <stack>
std::string type;
int addr;
int memory_address = 5000;
std::string prevLex;
std::string prevToken;
int sym_address = 0;
int instr_address = 1;
int recIndex = 0; //Keeps track of the records
bool printSw = false; //Determines if the rules are printed or not
bool printCheck = false;
bool reset = true;
symbol sym_table[100];
instruction instr_table[1000];
std::stack <int> jumpstack;
int count;

std::vector<std::string> parseList;
std::string userLex;

std::vector<int> lineNum;
std::vector<std::string> token2;
int Iterator;

std::string lexerCall(std::ofstream& fileout) {
    std::string tempRec;
    if (recIndex < parseList.size() - 1)
    {
        recIndex++;
        tempRec = parseList[recIndex];
        //printSw = true;
        if (token2[recIndex] == "Invalid Input")
            lexerCall(fileout);
        return tempRec;
    }
    else
        Print(fileout);
    exit(0);
    return 0;
}

void gen_instr(std::string op, int oprnd)
{
    instr_table[instr_address].address = instr_address;
    instr_table[instr_address].op = op;
    instr_table[instr_address].oprnd = oprnd;
    instr_address++;
}

void gen_symbol(std::string& userLex, std::string type, std::ofstream& fileout)
{
    sym_table[sym_address].lex = userLex;
    sym_table[sym_address].mem_loc = memory_address;
    sym_table[sym_address].type = type;
    if (sym_table[sym_address].type == "")
    {
        std::cout << "Error: Identifier " << userLex << " has not been declared yet.\n";
        fileout << "Error: Identifier " << userLex << " has not been declared yet.\n";
    }
    sym_address++;
    memory_address++;
}

int get_address(std::string userLex)
{
    int address = 0;
    for (int i = 0; i < sym_address; i++) {
        if (sym_table[i].lex == userLex) {
            address = sym_table[i].mem_loc;
        }
    }
    return address;
}

void back_patch(int jump_addr)
{
    int addr = jumpstack.top();
    jumpstack.pop();
    instr_table[addr].oprnd = jump_addr;
}

void Print(std::ofstream& fileout)
{
    std::cout << std::endl << std::endl << "Symbol Table:" << std::endl
        << "Lexeme" << std::setw(32) << "\tMemory Address" << std::setw(27) << "Type" << std::endl
        << "----------------------------------------------------------------------------------"
        << std::endl;

    fileout << std::endl << std::endl << "Symbol Table:" << std::endl
        << "Lexeme" << std::setw(32) << "\tMemory Address" << std::setw(27) << "Type" << std::endl
        << "----------------------------------------------------------------------------------"
        << std::endl;

    for (int i = 0; i < sym_address; i++)
    {
        std::cout << std::setw(5) << sym_table[i].lex << std::setw(30) << sym_table[i].mem_loc << std::setw(30)
            << sym_table[i].type << std::endl;
        fileout << std::setw(5) << sym_table[i].lex << std::setw(30) << sym_table[i].mem_loc << std::setw(30)
            << sym_table[i].type << std::endl;
    }


    std::cout << std::endl << "Instruction Table:" << std::endl
        << "Instruction Address" << std::setw(15) << "\tOP" << std::setw(31) << "OPRND" << std::endl
        << "----------------------------------------------------------------------------------"
        << std::endl;
    fileout << std::endl << "Instruction Table:" << std::endl
        << "Instruction Address" << std::setw(15) << "\tOP" << std::setw(31) << "OPRND" << std::endl
        << "----------------------------------------------------------------------------------"
        << std::endl;

    for (int i = 1; i < instr_address; i++)
    {
        if (instr_table[i].oprnd == -1000) {
            std::cout << std::setw(5) << instr_table[i].address << std::setw(30) << instr_table[i].op << std::setw(30) << "nil" << std::endl;
            fileout << std::setw(5) << instr_table[i].address << std::setw(30) << instr_table[i].op << std::setw(30) << "nil" << std::endl;
        }
        else {
            std::cout << std::setw(5) << instr_table[i].address << std::setw(30) << instr_table[i].op << std::setw(30)
                << instr_table[i].oprnd << std::endl;
            fileout << std::setw(5) << instr_table[i].address << std::setw(30) << instr_table[i].op << std::setw(30)
                << instr_table[i].oprnd << std::endl;
        }

    }
}
std::string getTokenType(std::string lex)
{
    std::string temp;
    for (int i = 0; i < sym_address; i++) {
        if (sym_table[i].lex == lex) {
            temp = sym_table[i].type;
        }
    }
    return temp;
}

//Check Type( Match
void checkMatch(std::string prevLex, std::string userLex, std::string prevToken, std::string userToken, std::ostream& fileout)
{
    if ((getTokenType(prevLex) != getTokenType(userLex) )&& getTokenType(prevLex) != "")
    {
        std::cout << "Tokens do no match" << std::endl;
        fileout << "Tokens do no match" << std::endl;
    }
    if(getTokenType(prevLex) == "boolean" || getTokenType(userLex) == "boolean")
    {    
        std::cout << "Boolean may not be +,-,*, or / with each other" << std::endl;
        fileout << "Boolean may not be +,-,*, or / with each other" << std::endl;
    }
}

//Check if multiple symbols in the table
bool check_symbol(std::string userLex)
{
    count = 0;
    for (int i = 0; i < sym_address; i++)
    {
        if (userLex == sym_table[i].lex) {
            count++;
        }
    }
    if (count > 0)
        return true;
    else
        return false;
}

void Rat20F(std::vector<std::string> lexemeVec, std::vector<int> count, std::vector<std::string> tokenVec, std::ofstream& fileout)
{
    userLex = lexemeVec[recIndex];
    parseList = lexemeVec;
    lineNum = count;
    token2 = tokenVec;
    Parse(userLex, fileout);
}

void Parse(std::string& userLex, std::ofstream& fileout)
{
    if (printSw) {
        std::cout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
        fileout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
        printSw = false;
    }
    if (userLex == "$$")
    {
        if (printCheck) {
            std::cout << "<RAT20F> -> <Opt Function Definitions>"
                << " $$ <Opt Declaration List> <Statement List>\n";
            fileout << "<RAT20F> -> <Opt Function Definitions>"
                << " $$ <Opt Declaration List> <Statement List>\n";
        }
        userLex = lexerCall(fileout);
    }

    OptDeclarationList(userLex, fileout);
    StatementList(userLex, fileout);
}

//R2
void OptDeclarationList(std::string& userLex, std::ofstream& fileout)
{
    if (userLex == "integer" || userLex == "boolean" || userLex == "int")
    {
        type = userLex;
        if (printSw) {
            std::cout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
            fileout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
            printSw = false;
        }
        if (printCheck) {
            std::cout << "<Opt Declaration List> => <Declaration List>\n";
            fileout << "<Opt Declaration List> => <Declaration List>\n";
        }
        if (token2[recIndex] == "Integer")
        {
            gen_instr("PUSHI", stoi(userLex));
        }
        DeclarationList(userLex, fileout);
    }
    else {
        if (printSw) {
            std::cout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
            fileout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
            printSw = false;
        }
        if (printCheck) {
            std::cout << "<Opt Declaration List> => <Empty>\n";
            fileout << "<Opt Declaration List> => <Empty>\n";
        }
    }

}

//R3
void DeclarationList(std::string& userLex, std::ofstream& fileout)
{
    if (printSw) {
        std::cout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
        fileout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
        printSw = false;
    }
    if (printCheck) {
        std::cout << "<Declaration List> -> <Declaration> ; <Declaration List Prime>\n";
        fileout << "<Declaration List> -> <Declaration> ; <Declaration List Prime>\n";
    }
    Declaration(userLex, fileout);

    if (userLex == ";")
    {
        if (printSw) {
            std::cout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
            fileout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
            printSw = false;
        }
        userLex = lexerCall(fileout);

        DeclarationListPrime(userLex, fileout);
    }
    else
        if (userLex == ",")
        {
            if (printSw) {
                std::cout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
                fileout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
                printSw = false;
            }
            userLex = lexerCall(fileout);
            DeclarationList(userLex, fileout);
        }
        else {
            std::cout << "Declaration List error, expected ;\n";
            fileout << "Declaration List error, expected ;\n";
        }
}


void DeclarationListPrime(std::string& userLex, std::ofstream& fileout)
{
    if (userLex == "integer" || userLex == "boolean" || userLex == "int")
    {
        type = userLex;
        if (printSw) {
            std::cout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
            fileout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
            printSw = false;
        }
        if (printCheck) {
            std::cout << "<Declaration List Prime> -> <Declaration List> \n";
            fileout << "<Declaration List Prime> -> <Declaration List> \n";
        }
        if (token2[recIndex] == "Integer")
        {
            gen_instr("PUSHI", stoi(userLex));
        }
        DeclarationList(userLex, fileout);
    }

    else
    {
        if (printSw) {
            std::cout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
            fileout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
            printSw = false;
        }
        if (printCheck) {
            std::cout << "<Declaration List Prime> -> Epsilon \n";
            fileout << "<Declaration List Prime> -> Epsilon \n";
        }
    }
}


void Declaration(std::string& userLex, std::ofstream& fileout)
{
    if (printSw) {
        std::cout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
        fileout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
        printSw = false;
    }
    if (printCheck) {
        std::cout << "<Declaration> -> <Qualifier>  <Identifier>\n";
        fileout << "<Declaration> -> <Qualifier>  <Identifier>\n";
    }
    Qualifier(userLex, fileout);
    if (token2[recIndex] == "Identifier") {
        if (printCheck) {
            std::cout << "<Identifier>\n";
            std::cout << "Identifier detected " << parseList[recIndex] << "\n";
            fileout << "<Identifier>\n";
            fileout << "Identifier detected " << parseList[recIndex] << "\n";
        }
        if (!check_symbol(userLex))
        {
            gen_symbol(userLex, type, fileout);
        }


        if (count > 1) {
            std::cout << "Identifier " << userLex << " is already declared.\n";
            fileout << "Identifier " << userLex << " is already declared.\n";

        }

        //gen_instr("PUSHM", get_address(userLex)); //added
        userLex = lexerCall(fileout);
    }
    else {
        std::cout << "Error, identifier expected\n" << "Error at line " << lineNum[recIndex] << std::endl;
        fileout << "Error, identifier expected\n" << "Error at line " << lineNum[recIndex] << std::endl;
    }
}


void Qualifier(std::string& userLex, std::ofstream& fileout)
{
    if (userLex == "integer" || userLex == "int")
    {
        type = userLex;
        if (printSw) {
            std::cout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
            fileout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
            printSw = false;
        }
        if (printCheck) {
            std::cout << "<Qualifier> -> integer\n";
            fileout << "<Qualifier> -> integer\n";
        }
        if (token2[recIndex] == "Integer")
        {
            gen_instr("PUSHI", stoi(userLex));
        }
        userLex = lexerCall(fileout);

    }
    else if (userLex == "boolean")
    {
        type = userLex;
        if (printSw) {
            std::cout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
            fileout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
            printSw = false;
        }
        if (printCheck) {
            std::cout << "<Qualifier> -> boolean\n";
            fileout << "<Qualifier> -> boolean\n";
        }

        userLex = lexerCall(fileout);
    }
    else if (userLex == "," || userLex == ";")
    {
        if (printSw) {
            std::cout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
            fileout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
            printSw = false;
        }
        userLex = lexerCall(fileout);
    }
    else if(token2[recIndex] == "Identifier")
    { }
    else
    {
        std::cout << "Error: Qualifier expected.\n" << "Error at line " << lineNum[recIndex] << std::endl;
        fileout << "Error: Qualifier expected.\n" << "Error at line " << lineNum[recIndex] << std::endl;
    }
}

void Identifier(std::string& userLex, std::ofstream& fileout) {
    if (token2[recIndex] == "Identifier")
    {
        if (!check_symbol(userLex))
        {
            gen_symbol(userLex, type,fileout);
        }

        if (count > 1) {
            std::cout << "Identifier " << userLex << " is already declared.\n";
            fileout << "Identifier " << userLex << " is already declared.\n";

        }
        if (printSw) {
            std::cout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
            fileout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
            printSw = false;
        }
        if (printCheck) {
            std::cout << "<Identifier> -> identifier\n";
            fileout << "<Identifier> -> identifier\n";
        }
        userLex = lexerCall(fileout);
    }
}


void StatementList(std::string& userLex, std::ofstream& fileout) {

    if (printSw) {
        std::cout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
        fileout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
        printSw = false;
    }
    if (printCheck) {
        std::cout << "<Statement List> -> <Statement> <Statement List Prime>\n";
        fileout << "<Statement List> -> <Statement> <Statement List Prime>\n";
    }

    Statement(userLex, fileout);
    StatementListPrime(userLex, fileout);
}


void StatementListPrime(std::string& userLex, std::ofstream& fileout) { //?
    if (userLex == "if" || userLex == "fi" || userLex == "get" ||
        userLex == "put" || userLex == "while" || userLex == "{" || token2[recIndex] == "Identifier")
    {
        if (printSw) {
            std::cout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
            fileout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
            printSw = false;
        }
        if (printCheck) {
            std::cout << "<StatementListPrime> -> <StatementList>\n";
            fileout << "<StatementListPrime> -> <StatementList>\n";
        }
        if (token2[recIndex] == "Identifier")
        {
            if (!check_symbol(userLex))
            {
                gen_symbol(userLex, type,fileout);
            }

            if (count > 1) {
                std::cout << "Identifier " << userLex << " is already declared.\n";
                fileout << "Identifier " << userLex << " is already declared.\n";

            }
        }
        //("PUSHM", get_address(token2[recIndex])); //added

        //userLex = lexerCall(fileout);
        StatementList(userLex, fileout);
    }
    else if(userLex == "}")
    {
        gen_instr("JUMP", addr); //added
        back_patch(instr_address); //added
        userLex = lexerCall(fileout);
        Parse(userLex, fileout);
    }
    else
    {
        if (printCheck) {
            std::cout << "<Statement List Prime> -> Epsilon\n";
            fileout << "<Statement List Prime> -> Epsilon\n";
        }
        userLex = lexerCall(fileout);
        Parse(userLex, fileout);
    }
}


void Statement(std::string& userLex, std::ofstream& fileout) {
    if (printSw) {
        std::cout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
        fileout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
        printSw = false;
    }
    if (userLex == "{")
    {
        if (printCheck) {
            std::cout << "<Statement> -> <Compound>>\n";
            fileout << "<Statement> -> <Compound>>\n";
        }
        Compound(userLex, fileout);
    }
    else if (userLex == "if")
    {
        if (printCheck) {
            std::cout << "<Statement> -> <if>\n";
            fileout << "<Statement> -> <if>\n";
        }
        If(userLex, fileout);
    }
    else if (userLex == "get")
    {
        if (printCheck) {
            std::cout << "<Statement> -> <Get>\n";
            fileout << "<Statement> -> <Get>\n";
        }
        Get(userLex, fileout);
    }
    else if (userLex == "put")
    {
        if (printCheck) {
            std::cout << "<Statement> -> <Put>\n";
            fileout << "<Statement> -> <Put>\n";
        }
        Put(userLex, fileout);
    }
    else if (userLex == "while")
    {
        if (printCheck) {
            std::cout << "<Statement> -> <While>\n";
            fileout << "<Statement> -> <While>\n";
        }
        While(userLex, fileout);
    }
    else if (token2[recIndex] == "Identifier")
    {
        if (printCheck) {
            std::cout << "<Statement> -> <Assign>>\n";
            fileout << "<Statement> -> <Assign>>\n";
        }
        if (!check_symbol(userLex))
        {
            gen_symbol(userLex, type,fileout);
        }

        if (count > 1) {
            std::cout << "Identifier " << userLex << " is already declared.\n";
            fileout << "Identifier " << userLex << " is already declared.\n";

        }

        Assign(userLex, fileout);
    }
}




void Compound(std::string& userLex, std::ofstream& fileout) {
    if (userLex == "{") {
        if (printSw) {
            std::cout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
            fileout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
            printSw = false;
        }
        if (printCheck) {
            std::cout << "<Compound> -> {  <Statement List>  }\n";
            fileout << "<Compound> -> {  <Statement List>  }\n";
        }
        userLex = lexerCall(fileout);
        StatementList(userLex, fileout);

        if (userLex == "}") {
            userLex = lexerCall(fileout);
            if (printSw) {
                std::cout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
                fileout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
                printSw = false;
            }
        }
        else {
            std::cout << "Error: '}' expected\t\t" << "Error at line " << lineNum[recIndex] << std::endl;
            fileout << "Error: '}' expected\t\t" << "Error at line " << lineNum[recIndex] << std::endl;
        }
    }
    else {
        std::cout << "Error: '{' expected\t\t" << "Error at line " << lineNum[recIndex] << std::endl;
        fileout << "Error: '{' expected\t\t" << "Error at line " << lineNum[recIndex] << std::endl;
    }
}


void Assign(std::string& userLex, std::ofstream& fileout) {

    if (token2[recIndex] == "Identifier") {
        if (printSw) {
            std::cout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
            fileout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
            printSw = false;
        }
        if (printCheck) {
            std::cout << "<Assign> -> <Identifier> = <Expression> ;\n";
            fileout << "<Assign> -> <Identifier> = <Expression> ;\n";
        }
        if (!check_symbol(userLex))
        {
            gen_symbol(userLex, type,fileout);
        }

        if (count > 1) {
            std::cout << "Identifier " << userLex << " is already declared.\n";
            fileout << "Identifier " << userLex << " is already declared.\n";

        }
        //gen_instr("PUSHM", get_address(token2[recIndex])); //added

        std::string save = userLex;
        userLex = lexerCall(fileout);
        if (printSw) {
            std::cout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
            fileout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
            printSw = false;
        }

        if (userLex == "=") {
            userLex = lexerCall(fileout);
            if (printSw) {
                std::cout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
                fileout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
                printSw = false;
            }
            gen_instr("POPM", get_address(save)); //added
            Expression(userLex, fileout);
            if (userLex == ";") {
                if (printSw) {
                    std::cout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
                    fileout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
                    printSw = false;
                }
                userLex = lexerCall(fileout);
            }
            else
            {
                if (printCheck) {
                    std::cout << "Error: ';' expected\t\t" << "Error at line " << lineNum[recIndex] << std::endl;
                    fileout << "Error: ';' expected\t\t" << "Error at line " << lineNum[recIndex] << std::endl;
                }
            }
        }
        else if (userLex == ";" || userLex == "," || userLex == "(" || userLex == ")" || userLex == "-" || userLex == "+" || userLex == "!=") {
            if (printSw) {
                std::cout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
                fileout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
                printSw = false;
            }
            userLex = lexerCall(fileout);
        }
        else if (token2[recIndex] == "Keyword") {
            if (printSw) {
                std::cout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
                fileout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
                printSw = false;
            }
            userLex = lexerCall(fileout);
        }
        else
        {
            std::cout << "Error: '=' expected\t\t" << "Error at line " << lineNum[recIndex] << std::endl;
            fileout << "Error: '=' expected\t\t" << "Error at line " << lineNum[recIndex] << std::endl;
        }

    }
}



void If(std::string& userLex, std::ofstream& fileout) {
    if (userLex == "if")
    {
        if (printSw) {
            std::cout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
            fileout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
            printSw = false;
        }
        int addr = instr_address; //added
        userLex = lexerCall(fileout);
        if (printSw) {
            std::cout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
            fileout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
            printSw = false;
        }
        if (userLex == "(") {
            userLex = lexerCall(fileout);

            Condition(userLex, fileout);
            if (userLex == ")") {
                userLex = lexerCall(fileout);
                Statement(userLex, fileout);
                back_patch(instr_address);//added
                if (userLex == "fi") {
                    if (printSw) {
                        std::cout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
                        fileout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
                        if (printCheck) {
                            std::cout << "<If> -> if  ( <Condition>  ) <Statement>   fi \n";
                            fileout << "<If> -> if  ( <Condition>  ) <Statement>   fi \n";
                        }
                        userLex = lexerCall(fileout);
                    }
                    else {
                        std::cout << "Error: 'fi' expected\t\t" << "Error at line " << lineNum[recIndex] << std::endl;
                        fileout << "Error: 'fi' expected\t\t" << "Error at line " << lineNum[recIndex] << std::endl;
                    }
                }
                else if (userLex == "otherwise")
                {
                    if (printSw) {
                        std::cout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
                        fileout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
                        printSw = false;
                    }
                    userLex = lexerCall(fileout);
                    if (printSw) {
                        std::cout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
                        fileout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
                        printSw = false;
                    }
                    Statement(userLex, fileout);
                    if (userLex == "fi")
                    {
                        if (printCheck) {
                            std::cout << "if  ( <Condition>  ) <Statement>   otherwise  <Statement>  fi\n";
                            fileout << "if  ( <Condition>  ) <Statement>   otherwise  <Statement>  fi\n";
                        }
                        if (printSw) {
                            std::cout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
                            fileout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
                            printSw = false;
                        }
                        userLex = lexerCall(fileout);
                    }
                    else {
                        std::cout << "Error: 'fi' expected" << "Error at line " << lineNum[recIndex] << std::endl;
                        fileout << "Error: 'fi' expected" << "Error at line " << lineNum[recIndex] << std::endl;
                    }
                }
                else {
                    std::cout << "Error: 'otherwise' expected" << "Error at line " << lineNum[recIndex] << std::endl;
                    fileout << "Error: 'otherwise' expected" << "Error at line " << lineNum[recIndex] << std::endl;
                }
            }
            else {
                std::cout << "Error: ')' expected\t\t" << "Error at line " << lineNum[recIndex] << std::endl;
                fileout << "Error: ')' expected\t\t" << "Error at line " << lineNum[recIndex] << std::endl;
            }
        }
        else {
            std::cout << "Error: '(' expected\t\t" << "Error at line " << lineNum[recIndex] << std::endl;
            fileout << "Error: '(' expected\t\t" << "Error at line " << lineNum[recIndex] << std::endl;
        }
    }
    else {
        std::cout << "Error: 'if' keyword expected\t\t" << "Error at line " << lineNum[recIndex] << std::endl;
        fileout << "Error: 'if' keyword expected\t\t" << "Error at line " << lineNum[recIndex] << std::endl;
    }
}


void Put(std::string& userLex, std::ofstream& fileout) {
    if (userLex == "put") {
        if (printCheck) {
            std::cout << "<Put> ::=     put ( <identifier> );\n";
            fileout << "<Put> ::=     put ( <identifier> );\n";
        }

        userLex = lexerCall(fileout);
    }

    if (userLex == "(") {
        if (printSw) {
            std::cout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
            fileout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
            printSw = false;
        }
        userLex = lexerCall(fileout);
        Expression(userLex, fileout);
        gen_instr("STDOUT", -1000);

        {
            if (userLex == ")") {
                    if (printSw) {
                        std::cout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
                        fileout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
                        printSw = false;
                    }
                    userLex = lexerCall(fileout);
            }
            else if (userLex != ";") {
                std::cout << "Error: ')' expected\t\t" << "Error at line " << lineNum[recIndex] << std::endl;
                fileout << "Error: ')' expected\t\t" << "Error at line " << lineNum[recIndex] << std::endl;
            }

            if (userLex == ";") {
                if (printSw) {
                    std::cout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
                    fileout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
                    printSw = false;
                }
                userLex = lexerCall(fileout);
            }
            else {
                std::cout << "Error: ';' expected\t\t" << "Error at line " << lineNum[recIndex] << std::endl;
                fileout << "Error: ';' expected\t\t" << "Error at line " << lineNum[recIndex] << std::endl;
            }
        }
    }
    else {
        std::cout << "Error: '(' expected\t\t" << "Error at line " << lineNum[recIndex] << std::endl;
        fileout << "Error: '(' expected\t\t" << "Error at line " << lineNum[recIndex] << std::endl;
    }




}


void Get(std::string& userLex, std::ofstream& fileout) {
    if (userLex == "get") {
        if (printCheck)
        {
            std::cout << "<Get> ::=     get ( <Identifier> );\n";
            fileout << "<Get> ::=     get ( <Identifier> );\n";
        }
        if (printSw) {
            std::cout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
            fileout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
            printSw = false;
        }
        userLex = lexerCall(fileout);
        if (userLex == "(") {
            if (printSw) {
                std::cout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
                fileout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
                printSw = false;
            }
            std::string save = userLex;
            userLex = lexerCall(fileout);
            Identifier(userLex, fileout);
            if (userLex == ")") {
                if (printSw) {
                    std::cout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
                    fileout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
                    printSw = false;
                }
                gen_instr("STDIN", -1000); //added
                gen_instr("POPM", get_address(save)); //added
                userLex = lexerCall(fileout);
            }
            else {
                std::cout << "Error: ')' expected\t\t" << "Error at line " << lineNum[recIndex] << std::endl;
                fileout << "Error: ')' expected\t\t" << "Error at line " << lineNum[recIndex] << std::endl;
            }

            if (userLex == ";") {
                if (printSw) {
                    std::cout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
                    fileout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
                    printSw = false;
                }
                userLex = lexerCall(fileout);
            }
            else {
                std::cout << "Error: ';' expected\t\t" << "Error at line " << lineNum[recIndex] << std::endl;
                fileout << "Error: ';' expected\t\t" << "Error at line " << lineNum[recIndex] << std::endl;
            }
        }
        else {
            std::cout << "Error: '(' expected\t\t" << "Error at line " << lineNum[recIndex] << std::endl;
            fileout << "Error: '(' expected\t\t" << "Error at line " << lineNum[recIndex] << std::endl;
        }
    }
    else {
        std::cout << "Error: 'get' expected\t\t" << "Error at line " << lineNum[recIndex] << std::endl;
        fileout << "Error: 'get' expected\t\t" << "Error at line " << lineNum[recIndex] << std::endl;
    }

}


void While(std::string& userLex, std::ofstream& fileout) {
    if (userLex == "while") {
        if (printSw) {
            std::cout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
            fileout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
            printSw = false;
        }
        if (printCheck) {
            std::cout << "<While> ::=  while ( <Condition>  )  <Statement>\n";
            fileout << "<While> ::=  while ( <Condition>  )  <Statement>\n";
        }
        addr = instr_address; //added
        gen_instr("LABEL", -1000); //added
        userLex = lexerCall(fileout);

        if (userLex == "(") { userLex = lexerCall(fileout); }
        else {
            std::cout << "Error: '(' expected\t\t" << "Error at line " << lineNum[recIndex] << std::endl;
            fileout << "Error: '(' expected\t\t" << "Error at line " << lineNum[recIndex] << std::endl;
        }
        Condition(userLex, fileout);
        if (userLex == ")") {
            userLex = lexerCall(fileout);
            Statement(userLex, fileout);
            gen_instr("JUMP", addr); //added
            back_patch(instr_address); //added
            userLex = lexerCall(fileout);
        }
        else {
            std::cout << "Error: ')' expected\t\t" << "Error at line " << lineNum[recIndex] << std::endl;
            fileout << "Error: ')' expected\t\t" << "Error at line " << lineNum[recIndex] << std::endl;
        }
    }
    else {
        std::cout << "Error: 'while' expected\t\t" << "Error at line " << lineNum[recIndex] << std::endl;
        fileout << "Error: 'while' expected\t\t" << "Error at line " << lineNum[recIndex] << std::endl;
    }

}


void Condition(std::string& userLex, std::ofstream& fileout) {
    if (printSw) {
        std::cout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
        fileout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
        printSw = false;
    }
    if (printCheck) {
        std::cout << "<Condition> ::=     <Expression>  <Relop>   <Expression>\n";
        fileout << "<Condition> ::=     <Expression>  <Relop>   <Expression>\n";
    }
    Expression(userLex, fileout);
    if (userLex == "<")
    {
        gen_instr("LES", -1000); //added
        jumpstack.push(instr_address); //added
        gen_instr("JUMPZ", -1000); //added
    }
    else if (userLex == "!=")
    {
        gen_instr("NEQ", -1000); //added
        jumpstack.push(instr_address); //added
        gen_instr("JUMPZ", -1000); //added
    }
    else if (userLex == ">")
    {
        gen_instr("GRT", -1000); //added
        jumpstack.push(instr_address); //added
        gen_instr("JUMPZ", -1000); //added
    }
    else if (userLex == "=")
    {
        gen_instr("EQ", -1000); //added
        jumpstack.push(instr_address); //added
        gen_instr("JUMPZ", -1000); //added
    }
    else if (userLex == "<=")
    {
        gen_instr("LES", -1000); //added
        jumpstack.push(instr_address); //added
        gen_instr("EQ", -1000); //added
        jumpstack.push(instr_address); //added
        gen_instr("JUMPZ", -1000); //added
    }
    else if (userLex == "=>")
    {
        gen_instr("GRT", -1000); //added
        jumpstack.push(instr_address); //added
        gen_instr("EQ", -1000); //added
        jumpstack.push(instr_address); //added
        gen_instr("JUMPZ", -1000); //added
    }
    Relop(userLex, fileout);
    Expression(userLex, fileout);

}



void Relop(std::string& userLex, std::ofstream& fileout) {
    if (userLex == "==") {
        if (printSw) {
            std::cout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
            fileout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
            printSw = false;
        }
        if (printCheck) {
            std::cout << "<Relop> -> ==" << "\n";
            fileout << "<Relop> -> ==" << "\n";
        }
        userLex = lexerCall(fileout);
        if (printSw) {
            std::cout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
            fileout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
            printSw = false;
        }
    }
    else if (userLex == ">") {
        if (printSw) {
            std::cout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
            fileout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
            printSw = false;
        }
        if (printCheck) {
            std::cout << "<Relop> -> >" << "\n";
            fileout << "<Relop> -> >" << "\n";
        }
        userLex = lexerCall(fileout);
        if (printSw) {
            std::cout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
            fileout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
            printSw = false;
        }
    }
    else if (userLex == "<") {
        if (printSw) {
            std::cout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
            fileout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
            printSw = false;
        }
        if (printCheck) {
            std::cout << "<Relop> -> <" << "\n";
            fileout << "<Relop> -> <" << "\n";
        }
        userLex = lexerCall(fileout);
        if (printSw) {
            std::cout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
            fileout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
            printSw = false;
        }
    }
    else if (userLex == "<=") {
        if (printSw) {
            std::cout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
            fileout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
            printSw = false;
        }
        if (printCheck) {
            std::cout << "<Relop> -> <=" << "\n";
            fileout << "<Relop> -> <=" << "\n";
        }
        userLex = lexerCall(fileout);
        if (printSw) {
            std::cout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
            fileout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
            printSw = false;
        }
    }
    else if (userLex == "=>") {
        if (printSw) {
            std::cout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
            fileout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
            printSw = false;
        }
        if (printCheck) {
            std::cout << "<Relop> -> =>" << "\n";
            fileout << "<Relop> -> =>" << "\n";
        }
        userLex = lexerCall(fileout);
        if (printSw) {
            std::cout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
            fileout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
            printSw = false;
        }
    }
    else {
        if (printSw) {
            std::cout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
            fileout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
            printSw = false;
        }
        std::cout << "Error, expected an operator '==', ,'<=', '=>', '>', or '<'\n" << "Error at line " << lineNum[recIndex] << std::endl;
        fileout << "Error, expected an operator '==', ,'<=', '=>', '>', or '<'\n" << "Error at line " << lineNum[recIndex] << std::endl;
    }
}



void Expression(std::string& userLex, std::ofstream& fileout)
{
    if (printSw) {
        std::cout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
        fileout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
        printSw = false;
    }
    if (printCheck) {
        std::cout << "<Expression> -> <Term> <Expression Prime>\n";
        fileout << "<Expression> -> <Term> <Expression Prime>\n";
    }
    Term(userLex, fileout);

    ExpressionPrime(userLex, fileout);

}

void ExpressionPrime(std::string& userLex, std::ofstream& fileout) {
    if (printSw) {
        std::cout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
        fileout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
        printSw = false;
    }

    if (userLex == "+")
    {
        if (printSw) {
            std::cout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
            fileout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
            printSw = false;
        }
        if (printCheck) {
            std::cout << "<Expression Prime>-> + <Term> <Expression Prime>\n";
            fileout << "<Expression Prime>-> + <Term> <Expression Prime>\n";
        }
        userLex = lexerCall(fileout);
        if (printSw) {
            std::cout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
            fileout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
            printSw = false;
        }
        checkMatch(prevLex, userLex, prevToken, token2[recIndex], fileout);
        Term(userLex, fileout);
        gen_instr("ADD", -1000); //added
        ExpressionPrime(userLex, fileout);
    }
    else if (userLex == "-")
    {
        if (printSw) {
            std::cout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
            fileout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
            printSw = false;
        }
        if (printCheck) {
            std::cout << "<Expression Prime> -> - <Term> <Expression Prime>\n";
            fileout << "<Expression Prime> -> - <Term> <Expression Prime>\n";
        }
        userLex = lexerCall(fileout);
        if (printSw) {
            std::cout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
            fileout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
            printSw = false;
        }
        checkMatch(prevLex, userLex, prevToken, token2[recIndex], fileout);
        Term(userLex, fileout);
        gen_instr("SUB", -1000); //added
        ExpressionPrime(userLex, fileout);
    }
    else {
        if (printSw) {
            std::cout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
            fileout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
            printSw = false;
        }
        if (printCheck) {
            std::cout << "<Expression Prime> -> Epsilon\n";
            fileout << "<Expression Prime> -> Epsilon\n";
        }
    }
}


void Term(std::string& userLex, std::ofstream& fileout) {
    if (printSw) {
        std::cout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
        fileout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
        printSw = false;
    }
    if (printCheck) {
        std::cout << "<Term> -> <Factor> <Term Prime>\n";
        fileout << "<Term> -> <Factor> <Term Prime>\n";
    }
    Factor(userLex, fileout);
    TermPrime(userLex, fileout);
}



void TermPrime(std::string& userLex, std::ofstream& fileout)
{
    if (userLex == "*")
    {
        if (printSw) {
            std::cout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
            fileout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
            printSw = false;
        }
        if (printCheck) {
            std::cout << "<Term Prime> -> * <Factor> <Term Prime>\n";
            fileout << "<Term Prime> -> * <Factor> <Term Prime>\n";
        }
        userLex = lexerCall(fileout);
        if (printSw) {
            std::cout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
            fileout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
            printSw = false;
        }
        checkMatch(prevLex, userLex, prevToken, token2[recIndex], fileout);
        Factor(userLex, fileout);
        gen_instr("MUL", -1000); //added
        TermPrime(userLex, fileout);
    }
    else if (userLex == "/")
    {
        if (printSw) {
            std::cout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
            fileout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
            printSw = false;
        }
        if (printCheck) {
            std::cout << "<Term Prime> -> * <Factor> <Term Prime>\n";
            fileout << "<Term Prime> -> * <Factor> <Term Prime>\n";
        }
        userLex = lexerCall(fileout);
        if (printSw) {
            std::cout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
            fileout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
            printSw = false;
        }
        checkMatch(prevLex, userLex, prevToken, token2[recIndex], fileout);
        Factor(userLex, fileout);
        gen_instr("DIV", -1000); //added
        TermPrime(userLex, fileout);
    }
    else
    {
        if (printSw) {
            std::cout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
            fileout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
            printSw = false;
        }
        if (printCheck) {
            std::cout << "<Term Prime> -> Epsilon\n";
            fileout << "<Term Prime> -> Epsilon\n";
        }
    }
}



void Factor(std::string& userLex, std::ofstream& fileout) {
    if (userLex == "-")
    {
        if (printSw) {
            std::cout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
            fileout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
            printSw = false;
        }
        if (printCheck) {
            std::cout << "<Factor> -> - <Primary>\n";
            fileout << "<Factor> -> - <Primary>\n";
        }
        prevLex = userLex;
        prevToken = token2[recIndex];
        userLex = lexerCall(fileout);
        if (printSw) {
            std::cout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
            fileout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
            printSw = false;
        }
        Primary(userLex, fileout);
    }
    else
    {
        if (printSw) {
            std::cout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
            fileout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
            printSw = false;
        }
        if (printCheck) {
            std::cout << "<Factor> -> <Primary>\n";
            fileout << "<Factor> -> <Primary>\n";
        }
        Primary(userLex, fileout);
    }
}


void Primary(std::string& userLex, std::ofstream& fileout) {


    if (token2[recIndex] == "Identifier")
    {
        if (printCheck) {
            std::cout << "<Primary> -> <Identifier>\n";
            fileout << "<Primary> -> <Identifier>\n";
        }
        if (!check_symbol(userLex))
        {
            gen_symbol(userLex, type,fileout);
        }

        if (count > 1) {
            std::cout << "Identifier " << userLex << " is already declared.\n";
            fileout << "Identifier " << userLex << " is already declared.\n";

        }
        gen_instr("PUSHM", get_address(userLex)); //added
        userLex = lexerCall(fileout);
    }
    else if (token2[recIndex] == "Integer")
    {
        if (printCheck) {
            std::cout << "<Primary> -> <Integer>\n";
            fileout << "<Primary> -> <Integer>\n";
        }
        gen_instr("PUSHI", stoi(userLex));
        userLex = lexerCall(fileout); //?
    }
    else if (token2[recIndex] == "Real")
    {
        if (printCheck) {
            std::cout << "<Primary> -> <Real>\n";
            fileout << "<Primary> -> <Real>\n";
        }
        userLex = lexerCall(fileout); //?
    }
    else if (userLex == "(")
    {
        if (printCheck) {
            std::cout << "<Primary> -> (<Expression>)\n";
            fileout << "<Primary> -> (<Expression>)\n";
        }
        userLex = lexerCall(fileout);
        if (printSw) {
            std::cout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
            fileout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
            printSw = false;
        }
        Expression(userLex, fileout);
        if (userLex == ")")
        {
            if (printSw) {
                std::cout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
                fileout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
                printSw = false;
            }
            userLex = lexerCall(fileout);
            if (printSw) {
                std::cout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
                fileout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
                printSw = false;
            }
        }
        else {
            std::cout << "Error, expected a )" << "\n" << "Error at line " << lineNum[recIndex] << std::endl;
            fileout << "Error, expected a )" << "\n" << "Error at line " << lineNum[recIndex] << std::endl;
        }
    }
    else if (userLex == "true") {
        if (printSw) {
            std::cout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
            fileout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
            printSw = false;
        }
        if (printCheck) {
            std::cout << "<Primary> -> true\n";
            fileout << "<Primary> -> true\n";
        }
        gen_instr("PUSHI", 1);
        userLex = lexerCall(fileout);
        if (printSw) {
            std::cout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
            fileout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
            printSw = false;
        }
    }
    else if (userLex == "false") {
        if (printSw) {
            std::cout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
            fileout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
            printSw = false;
        }
        if (printCheck) {
            std::cout << "<Primary> -> false\n";
            fileout << "<Primary> -> false\n";
        }
        gen_instr("PUSHI", 0);
        userLex = lexerCall(fileout);
        if (printSw) {
            std::cout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
            fileout << "Lexeme: " << parseList[recIndex] << "\t\tToken: " << token2[recIndex] << "\n";
            printSw = false;
        }
    }
    else {
        std::cout << "Error, expected either identifier, integer, real, expression, true or false \n" << "Error at line " << lineNum[recIndex] << std::endl;
        fileout << "Error, expected either identifier, integer, real, expression, true or false \n" << "Error at line " << lineNum[recIndex] << std::endl;
    }
}
