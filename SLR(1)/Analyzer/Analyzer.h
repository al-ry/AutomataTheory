#pragma once
#include "../Generator/Generator.h"
#include "../../Lexer/Lexer/lex.h"
#include <memory>


struct AnalyzerReduction 
{
	std::string reductionNonterminal;
	size_t symbolCount;
};

struct AnalyzerShift 
{
	size_t rowIndex;
};

typedef std::variant<AnalyzerReduction, AnalyzerShift, bool> AnalyzerTableValue;

struct AnalyzerRow
{
	std::vector<std::string> symbols;
	std::vector<std::optional<AnalyzerTableValue>> val;
};



typedef std::vector<AnalyzerRow> AnalyzerTable;


AnalyzerTable ReadTable(std::istream& input);
void AnalyzeTable(AnalyzerTable const& table, Lexer& lexer);