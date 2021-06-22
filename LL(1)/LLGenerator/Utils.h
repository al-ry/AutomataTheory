#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <vector>
#include <optional>

const std::string EMPTY_RULE = "e";
const std::string RULE_SEPARATOR = "|";
const std::string END_SEQUENCE = "$";
const std::string RULE_DEFINITION_SEPARATOR = "->";
const std::string GUIDE_SET_SEPARATOR = "/";
const std::string TAB = "\t\t\t";


struct Rules
{
	std::string left;
	std::vector<std::string> right;
	std::vector<std::string> guideSet;
};

struct TableRow
{
	std::string symbol;
	std::vector<std::string> guideSet;
	bool error;
	std::optional<size_t> pointer;
	bool isInStack;
	bool end;
	bool shift;
};

typedef std::vector<Rules> Grammar;
typedef std::pair<std::string, int> HasTransitionPair;
typedef	std::pair<std::string, std::vector<HasTransitionPair>> Transition;
typedef std::pair<TableRow, Rules> TableRowWithRule;
typedef std::vector<TableRowWithRule> TableWithRules;
typedef std::vector<TableRow> Table;



void AddRule(Grammar& grammar, const std::string& left, const Rules& right);

Grammar CreateGrammar(std::istream& input);

std::string ReadGrammarFromFile(const std::string inputFile);
bool IsNonterminal(std::string const& str);

void PrintGrammar(Grammar& const grammar, std::ostream& output);

void PrintTable(Table table, std::ostream& out);
std::vector<std::pair<std::string, std::string>> IsLLGrammar(Grammar const& grammar);


