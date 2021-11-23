#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include <vector>
#include <ctype.h>

#include "Lexer.h"
#include "Parser.h"

using namespace std;

int main() {
	ifstream textFile; //Input Text File
	string outputFileName, filename;
	std::vector<int> count;

	do {
		int lineCount = 1;
		count.clear();
		cout << "Enter a file name without .txt (Example: For Test.txt enter Test)"
			<< endl << "To Quit Please Enter: Quit" << endl;
		cin >> filename;

		if (filename == "quit" || filename == "Quit")
			exit(0);
		else
		{
			filename = filename + ".txt";

			textFile.open(filename); //Open File

			if (!textFile.good()) //Check if file is good
				cout << endl << "File cannot be Opened. Please Try Again" << endl << endl;
			else
			{
				outputFileName = "output" + filename;
				ofstream outputFile(outputFileName); //Creates Output File
				outputFile << "File to be opened: " << filename << endl;

				cout << "Source Code:" << endl << textFile.rdbuf() << endl;
				textFile.seekg(0);
				outputFile << "Source Code:" << endl << textFile.rdbuf() << endl;
				textFile.seekg(0);

				Lexer analysis;

				//cout << endl << "Lexical Analysis:" << endl
				//	<< "Token" << setw(30) << "\tLexeme" << endl
				//	<< "-------------------------------------------------------"
				//	<< endl;

				//outputFile << endl << "Lexical Analysis:" << endl
				//	<< "Token" << setw(30) << "\tLexeme" << endl
				//	<< "-------------------------------------------------------"
				//	<< endl;


				while (!textFile.eof())
				{
					string thing; //Lexeme String
					bool foundLexme = false;
					char peek;
					do //Gets the Char and adds to the String while ignoring spaces, comments and eof
					{
						peek = textFile.get();

						//Ignore comments when using /* that ends with */
						if (peek == '/' && textFile.peek() == '*')
						{
							peek = ' ';
							while (peek != '*' && textFile.peek() != '/')
							{
								textFile.ignore();
							}
							textFile.ignore();
						}

						//Checks for space, eof, seperator or operator and ends loop if found.
						if (analysis.isSeparator(peek) || analysis.isOperator(peek) || isspace(peek) || peek == -1)
							foundLexme = true;

						//Checks if it is a seperator/operator and not empty and moves file back one. To make sure ever seperator/operator is counted
						if ((analysis.isSeparator(peek) || analysis.isOperator(peek)) && !thing.empty())
							textFile.putback(peek);
						else if (!isspace(peek) && peek != -1) // Checks if it is not empty and add to string
							thing += peek;

						if (thing.empty() && peek != -1) //Checks if empty and not eof to repeat the loop.
							foundLexme = false;

						if ((thing.empty() && peek == '\n'))
							lineCount++;


					} while (!foundLexme);

					//Set Token and lexeme empty if file is empty and string is empty
					if (thing.empty() && peek == -1)
						textFile.ignore();

					count.push_back(lineCount);
					if (peek == '\n')
						lineCount++;

					analysis.analyzer(textFile, thing); //Start Lexical Anaylsis
				}
				//analysis.print(outputFile);
				std::vector<string> parseList = analysis.getLexeme();
				std::vector<string> token = analysis.getToken();
				Rat20F(parseList, count, token, outputFile);
				Print(outputFile);

				textFile.close();
				outputFile.close();
			}
		}


	} while (!(filename == "quit.txt") || !(filename == "Quit.txt"));
}
