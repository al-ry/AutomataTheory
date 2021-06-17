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
	std::ofstream output("out\\" + std::string(argv[2]));

	auto grammar = CreateGrammar(input);

	try
	{
		auto table = CreateSLRTable(grammar);
		PrintTable(table, std::cout);
		PrintGrammar(grammar, std::cout);
		PrintTableForAnalyze(table, output);
	}
	catch (const std::invalid_argument& e)
	{
		std::cout << e.what();
	}


	return 0;
}