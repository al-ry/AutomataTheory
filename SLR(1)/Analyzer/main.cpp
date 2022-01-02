#include "../Generator/Generator.h"
#include "../../Lexer/Lexer/lex.h"
#include "Analyzer.h"
#include <iostream>
#include <fstream>
#include "Node.h"
#include <fstream>
#include <sstream>

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

void PrintTree(const AST& tree) {
	std::stringstream nodes;
	std::stringstream relations;
	nodes << "digraph G{\n";
	tree.begin()->get()->writeGraphRepresentation(nodes);

	nodes << relations.str();
	nodes << "}\n";
	const std::string pathToDot = "../../Graphviz/bin/";
	const std::string dotName = "graph.dot";
	const std::string imageName = "../../graph.png";

	std::ofstream dotFile(dotName);

	dotFile << nodes.str();
}

void AddChild() {

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
	AnalyzerTable table = ReadTable(input);

	std::string buffer = ReadFileToBuffer("in\\" + std::string(argv[2]));
	Lexer lex(buffer);
	
	try
	{
		AST astTree;
		AnalyzeTable(table, lex, astTree);
		PrintTree(astTree);
		system("print_graph.cmd");

	}
	catch (const std::exception& e)
	{
		std::cout << e.what();
	}

	return 0;
}