#include "Generator.h"
#include <fstream>
#include <iostream>
#include <string>



int main(int argc, char** argv)
{
	if (argc != 3)
	{
		std::cout << "Invalid arguments count\n";
		return 1;
	}

	std::ifstream input("in\\" + std::string(argv[1]));
	std::ofstream outputGuideSet("out\\GuideSet" + std::string(argv[2]));
	//std::ofstream outputTable("out\\Table" + std::string(argv[2]));


	auto grammar = CreateGrammar(input);
	auto table = CreateSLRTable(grammar);
	PrintTable(table, std::cout);
	PrintGrammar(grammar, std::cout);
	//PrintGrammar(grammar, outputGuideSet);
	//CreateTable(grammar);

	//auto table = GenerateTable(grammar);
	//PrintTable(table, outputTable);

	return 0;
}