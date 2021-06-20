#include "../Generator/Generator.h"
#include "../../Lexer/Lexer/lex.h"
#include "Analyzer.h"
#include <iostream>
#include <fstream>

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

void AddEndSequence(std::vector<std::string> & sequence) 
{
	if (sequence.back() != "$")
	{
		sequence.push_back("$");
	}
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
	AnalyzerTable table = ReadTable(input);

	if (sequence.size() == 0)
	{
		std::cout << "Input sequence shoud not be empty\n";
		return 1;
	}
	std::string buffer = ReadFileToBuffer("test_program.txt");
	Lexer lex(buffer);
	

	AddEndSequence(sequence);

	//auto grammar = CreateGrammar(input);
	//std::cout << "Result grammar\n";
	//PrintGrammar(grammar, std::cout);

	//auto table = CreateSLRTable(grammar);
	//PrintTable(table, std::cout);
	//PrintTableForAnalyze(table);
	AnalyzeTable(table, lex);

	return 0;
}