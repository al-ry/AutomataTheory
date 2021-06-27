#include <fstream>
#include <iostream>
#include <sstream>
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

std::vector<std::string> GetGuideSet(std::istringstream& stream) {
	std::string line;
	std::vector<std::string> guideSet;
	stream >> line;
	while (line != "'}'")
	{
		guideSet.push_back(line);
		stream >> line;
	}

	return guideSet;
}

void ParseTableRow(std::string line, Table& table)
{
	std::istringstream stream(line);
	TableRow row;

	stream >> line;
	stream >> line;
	row.symbol = line;

	stream >> line;
	row.guideSet = GetGuideSet(stream);

	stream >> line;
	row.shift = line == "0" ? false : true;
	stream >> line;
	row.error = line == "0" ? false : true;
	stream >> line;

	if (line == "null")
	{
		row.pointer = std::nullopt;
	}
	else
	{
		row.pointer = std::stoi(line);
	}

	stream >> line;
	row.isInStack = line == "0" ? false : true;
	stream >> line;
	row.end = line == "0" ? false : true;


	table.push_back(row);
}

Table ReadTable(std::istream& input)
{
	Table table;
	std::string line;
	std::getline(input, line);

	while (std::getline(input, line))
	{
		if (!line.empty())
		{
			ParseTableRow(line, table);
		}
	}

	return table;
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

	//auto grammar = CreateGrammar(input);
	//std::cout << "Result grammar\n";
	////PrintGrammar(grammar, std::cout);
	//auto nonLLRules = IsLLGrammar(grammar);
	//if (nonLLRules.size() != 0)
	//{
	//	for (auto rule: nonLLRules)
	//	{
	//		std::cout << rule.first << " { "<< rule.second << " }";
	//	}
	//	return 1;
	//}

	std::string buffer = ReadFileToBuffer("in\\" + std::string(argv[2]));
	Lexer lex(buffer);
	auto table = ReadTable(input);
	//auto table = GenerateTable(grammar);
	//PrintTable(table, std::cout);
	//AnalyzeTable(table, sequence);
	AnalyzeTable(table, lex);

	return 0;
}