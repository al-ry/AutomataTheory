#include <fstream>
#include <iostream>
#include "../LLGenerator/Utils.h"
#include "../LLGenerator/Generator.h"
#include "../../Lexer/Lexer/lex.h"
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
	std::cout << "Result grammar\n";
	//PrintGrammar(grammar, std::cout);
	auto nonLLRules = IsLLGrammar(grammar);
	if (nonLLRules.size() != 0)
	{
		for (auto rule: nonLLRules)
		{
			std::cout << rule.first << " { "<< rule.second << " }";
		}
		return 1;
	}
	std::string buffer = ReadFileToBuffer("test_program.txt");
	Lexer lex(buffer);

	auto table = GenerateTable(grammar);
	//PrintTable(table, std::cout);

	//AnalyzeTable(table, sequence);
	AnalyzeTable(table, lex);

	return 0;
}