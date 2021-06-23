#include "Utils.h"
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
	std::ofstream outputGuideSet( "out\\GuideSet" + std::string(argv[2]));
	std::ofstream outputTable("out\\Table" + std::string(argv[2]));

	auto grammar = CreateGrammar(input);

	PrintGrammar(grammar, outputGuideSet);
	PrintGrammar(grammar, std::cout);

	auto nonLLRules = IsLLGrammar(grammar);
	if (nonLLRules.size() != 0)
	{
		for (auto rule : nonLLRules)
		{
			std::cout << rule.first << " { " << rule.second << " }" << std::endl;
		}
		return 1;
	}

	auto table = GenerateTable(grammar);
	PrintTable(table, outputTable);
	return 0;
}
