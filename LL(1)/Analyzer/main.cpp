#include <fstream>
#include <iostream>
#include "../LLGenerator/Utils.h"
#include "../LLGenerator/Generator.h"
#include "Analyzerh.h"



int main(int argc, char** argv)
{
	if (argc != 3)
	{
		std::cout << "Invalid arguments count\n";
		return 1;
	}

	std::ifstream input("in\\" + std::string(argv[1]));

	std::string inputSequence = argv[2];


	auto grammar = CreateGrammar(input);
	auto table = GenerateTable(grammar);

	PrintGrammar(grammar, std::cout);
	PrintTable(table, std::cout);

	AnalyzeTable(table, {"while", "id", "+", "id", "do", "id",  "=", "id", "$"} );


	return 0;
}