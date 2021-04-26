#include "Utils.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>

void AddRule(Grammar& grammar, const std::string& left, const Rules& right)
{
	//grammar.emplace(left, right);
}


std::vector<std::string> GetRules(const std::string& rules)
{
	std::stringstream ss(rules);
	std::string rule;
	std::vector<std::string> rulesArr;
	while (ss >> rule)
	{
		if (rule == RULE_SEPARATOR)
		{
			continue;
		}
		rulesArr.push_back(rule);
	}
	return rulesArr;
}

void FactorizeRule(Rules& rules)
{
	rules.right;
	std::vector<std::string> tmp;
	for (auto const &item:rules.right)
	{
		//std::find_if()
	}
}

void AddNonteminal(std::vector<std::string>& arr, const std::string& nonterminal)
{
	std::string str;
	auto it = std::find_if(arr.begin(), arr.end(), [&](std::string elem) {
		return nonterminal == elem;
	});
	if (it == arr.end())
	{
		arr.push_back(nonterminal);
	}
}

Grammar CreateGrammar(const std::string grammarStr)
{
	Grammar grammar;

	std::vector<std::string> nonterminals;
	std::stringstream ss(grammarStr);
	std::string line;
	while (std::getline(ss, line))
	{
		std::stringstream ruleLine(line);
		std::string separator;
		std::string rightPart;
		Rules rules;
		ruleLine >> rules.left;
		AddNonteminal(nonterminals, rules.left);
		ruleLine >> separator;
		std::getline(ruleLine, rightPart);
		rules.right = GetRules(rightPart);
		//FactorizeRule(rules);
		grammar.push_back(rules);
	}
	return grammar;
}


std::string ReadGrammarFromFile(const std::string inputFile)
{
	std::ifstream ifs(inputFile);
	std::string buffer((std::istreambuf_iterator<char>(ifs)),
		(std::istreambuf_iterator<char>()));
	return buffer;
}
