#include <fstream>
#include <iostream>
#include "../LLGenerator/Utils.h"
#include "../LLGenerator/Generator.h"
#include "Analyzerh.h"

std::vector<std::string> GetInputSequence(std::istream& input)
{
	std::vector<std::string> sequence;
	std::string line;

	while (input >> line)
	{
		sequence.push_back(line);
	}

	return sequence;
}

int main(int argc, char** argv)
{
	if (argc != 3)
	{
		std::cout << "Invalid arguments count\n";
		return 1;
	}

	std::cout << "\n";
	std::ifstream input("in\\" + std::string(argv[1]));
	std::ifstream sequenceFile("in\\" + std::string(argv[2]));
	std::vector<std::string> sequence = GetInputSequence(sequenceFile);


	auto grammar = CreateGrammar(input);
	auto table = GenerateTable(grammar);

	PrintGrammar(grammar, std::cout);
	PrintTable(table, std::cout);

	AnalyzeTable(table, sequence);

	return 0;
}