#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <vector>
#include <optional>
#include <variant>

const std::string EMPTY_RULE = "e";
const std::string RULE_SEPARATOR = "|";
const std::string END_SEQUENCE = "$";
const std::string RULE_DEFINITION_SEPARATOR = "->";
const std::string GUIDE_SET_SEPARATOR = "/";
const std::string TAB = "\t\t\t";
const std::string OK_RESULT = "OK";

struct Rule
{
	std::string left;
	std::vector<std::string> right;
public:
	bool operator==(const Rule& rhs) const
	{
		return this->left == rhs.left && this->right == rhs.right;
	}
	bool operator!=(const Rule& rhs) const
	{
		return !(*this == rhs);
	}
};

bool operator<(const Rule& lhs, const Rule& rhs) noexcept;

struct Reduction
{
	size_t index;
	Rule const* rule;
};

struct Shift
{
	size_t ruleIndex;
	size_t indexInRule;
	std::string name;
public:
	bool operator==(const Shift& rhs) const
	{
		return this->ruleIndex == rhs.ruleIndex && this->indexInRule == rhs.indexInRule && this->name == rhs.name;
	}
	bool operator!=(const Shift& rhs) const
	{
		return !(*this == rhs);
	}
};

bool operator<(const Shift& lhs, const Shift& rhs) noexcept;

typedef std::variant<Reduction, std::vector<Shift>, bool> TableValue;


struct Row
{
	std::vector<std::string> symbols;
	std::vector<Shift> state;
	std::vector<std::optional<TableValue>> val;
};

typedef std::vector<Row> Table;
typedef std::vector<Rule> Grammar;

Table CreateSLRTable(const Grammar& grammar);

void PrintTable(const Table& table, std::ostream& output);

Grammar CreateGrammar(std::istream& input);

bool IsNonterminal(std::string const& str);

void PrintGrammar(Grammar& const grammar, std::ostream& output);
void PrintTableForAnalyze(const Table& table, std::ostream& output);

