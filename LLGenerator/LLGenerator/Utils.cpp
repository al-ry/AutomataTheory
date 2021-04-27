#include "Utils.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>

void AddRule(Grammar& grammar, const std::string& left, const Rules& right)
{
	//grammar.emplace(left, right);
}


std::vector<Rules> GetRules(const std::string& rules)
{
	std::stringstream ss(rules);
	std::string ruleStr;
	std::vector<Rules> rulesArr;
	Rules rule;
	while (ss >> ruleStr)
	{
		if (ruleStr == RULE_SEPARATOR)
		{
			rulesArr.push_back(rule);
			rule.right.clear();
			continue;
		}
		rule.right.push_back(ruleStr);
	}
	rulesArr.push_back(rule);
	return rulesArr;
}

void Factorize(Grammar& grammar, std::vector<Rules> similiarRules)
{
	for (auto& rule : similiarRules)
	{
		std::vector<std::string> similiarPart;
		auto it = std::find_if(similiarRules.begin(), similiarRules.end(), [&rule, &similiarPart](const Rules& ruleParam) {
			bool result = false;
			for (auto& symbl : rule.right)
			{
				for (auto& innerSymbl : ruleParam.right)
				{
					if (symbl == innerSymbl)
					{
						result = true;
						similiarPart.push_back(symbl);
					}
					else
					{
						return result;
					}
				}
			}
		});
		if (it != similiarRules.end())
		{
			return;
		}
	}
}
void RemoveRecursion(Grammar& grammar)
{

}

void FactorizeGrammar(Grammar& grammar, const std::vector<std::string>& nonterminals)
{
	std::vector<std::string> tmp;
	for (auto & nonterminal : nonterminals)
	{
		std::vector<Rules> similiarRules;
		for (auto it = grammar.begin(); it != grammar.end(); it++)
		{
			
			it = std::find_if(it, grammar.end(), [&](const Rules& rule) {
				return nonterminal == rule.left;
			});
			if (it == grammar.end())
			{
				break;
			}
			if (std::find_if(similiarRules.begin(), similiarRules.end(), [&](const Rules &rule) {
				return rule.right == it->right;
			}) == similiarRules.end())
			{
				similiarRules.push_back(*it);
			}
		}
		Factorize(grammar, similiarRules);
		//RemoveRecursion()
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
		std::string tmp;
		std::string rightPart;
		std::string leftPart;
		ruleLine >> leftPart;
		AddNonteminal(nonterminals, leftPart);
		ruleLine >> tmp; //  reading ->
		std::getline(ruleLine, rightPart);
		std::vector<Rules> rules;
		rules = GetRules(rightPart);
		for (auto &rule: rules)
		{
			rule.left = leftPart;
			grammar.push_back(rule);
		}
	}
	FactorizeGrammar(grammar, nonterminals);
	return grammar;
}


std::string ReadGrammarFromFile(const std::string inputFile)
{
	std::ifstream ifs(inputFile);
	std::string buffer((std::istreambuf_iterator<char>(ifs)),
		(std::istreambuf_iterator<char>()));
	return buffer;
}
