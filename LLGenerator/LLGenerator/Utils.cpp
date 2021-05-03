#include "Utils.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>


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

void DeleteNonFactorizedRules(Grammar& grammar, const Grammar& similiarRules)
{
	Grammar tmpGrammar;
	Grammar copy = grammar;
	//std::copy(grammar.begin(), grammar.end(), copyGrammar);
	for (auto smrIt = similiarRules.begin(); smrIt != similiarRules.end(); smrIt++)
	{
		for (auto grmIt = grammar.begin(); grmIt != grammar.end();)
		{
			auto end = grammar.end();
			if (grmIt->right == smrIt->right)
			{	
				grmIt = grammar.erase(grmIt);
			}
			else
			{
				grmIt++;
			}
		}
	}
}


std::string GenerateRandomNonTerminal()
{
	static int counter = 0;
	std::string res = std::to_string(counter);
	counter++;
	return res;
}

void AddNewRulesToGrammar(Grammar& sourceGrammar, std::vector<std::string>& simPart, const std::vector<std::vector<std::string>>& nonsimPart, const std::string & leftSym)
{
	std::string newSymbol = "<F"+ GenerateRandomNonTerminal() + ">";
	Rules rule;
	simPart.push_back(newSymbol);
	rule.left = leftSym;
	rule.right = simPart;
	sourceGrammar.push_back(rule);
	for (auto it = nonsimPart.begin(); it != nonsimPart.end(); it++)
	{
		Rules rule;
		rule.left = newSymbol;
		rule.right = *it;
		sourceGrammar.push_back(rule);
	}
}

void Factorize(Grammar& grammar, Grammar& similiarRules)
{
	//std::vector<std::string> similiarPart;
	Grammar rulesWithSimPart;
	for (auto it = similiarRules.begin(); it != similiarRules.end(); it++)
	{
		for (size_t i = 0; i < similiarRules.size() ; i++)
		{
			if (similiarRules[i].right[0] == it->right[0] && similiarRules[i].right != it->right)
			{
				auto res = std::find_if(rulesWithSimPart.begin(), rulesWithSimPart.end(), [&](Rules& rule) {
					return similiarRules[i].right == rule.right;
					});
				if (res == rulesWithSimPart.end())
				{
					rulesWithSimPart.push_back(similiarRules[i]);
				}
			}
		}
	}

	DeleteNonFactorizedRules(grammar, rulesWithSimPart);

	std::sort(rulesWithSimPart.begin(), rulesWithSimPart.end(), [&](Rules& rule1, Rules& rule2) {
		return rule1.right[0] < rule2.right[0];
		});

	std::vector<Grammar> tmpGrmArr;

	for (auto it = rulesWithSimPart.begin(); it != rulesWithSimPart.end(); it++)
	{
		auto res = std::find_if(it, rulesWithSimPart.end(), [&](Rules& rule) {
			return rule.right[0] != it->right[0];
			});
		Grammar tmpGrm;
		if (res != rulesWithSimPart.end())
		{
			for (it; it != res; it++)
			{
				tmpGrm.push_back(*it);
			}
			tmpGrmArr.push_back(tmpGrm);
			it = --res;
		}
		else
		{
			for (it; it != rulesWithSimPart.end(); it++)
			{
				tmpGrm.push_back(*it);
			}
			tmpGrmArr.push_back(tmpGrm);
			break;
		}
	}

	for (auto grmIt = tmpGrmArr.begin(); grmIt != tmpGrmArr.end(); grmIt++)
	{
		std::sort(grmIt->begin(), grmIt->end(), [&](Rules& rule1, Rules& rule2) {
			return rule1.right.size() < rule2.right.size();
			});
		auto first = grmIt->begin()->right;
		std::vector<std::string> similiarPart;
		std::vector<std::vector<std::string>> nonsimiliarPart;
		for (size_t i = 0; i < first.size(); i++)
		{
			bool isEqual = std::all_of(grmIt->begin(), grmIt->end(), [&](Rules& rule) {
				return first[i] == rule.right[i];
				});
			if (isEqual)
			{
				similiarPart.push_back(first[i]);
			}
		}
		for (auto it = grmIt->begin(); it != grmIt->end(); it++)
		{
			if (it->right.size() == similiarPart.size())
			{
				nonsimiliarPart.push_back({ EMPTY_RULE });
			}
			else
			{
				std::vector<std::string> tmpSymbols;
				for (size_t i = similiarPart.size(); i < it->right.size(); i++)
				{
					tmpSymbols.push_back(it->right[i]);
				}
				nonsimiliarPart.push_back(tmpSymbols);
			}
		}
		AddNewRulesToGrammar(grammar, similiarPart, nonsimiliarPart, similiarRules[0].left);
		std::string sourceNonterminal = similiarRules[0].left;
		similiarRules.clear();
		for (auto it = grammar.begin(); it != grammar.end(); it++)
		{
			if (it->left == sourceNonterminal)
			{
				similiarRules.push_back(*it);
			}
		}
	}
}


void RemoveRecursion(Grammar& grammar, Grammar& outputGrammar, std::vector<Rules>& similiarRules)
{
	bool hasEmptyRule = std::find_if(similiarRules.begin(), similiarRules.end(), [&](const Rules& rule) {
		return rule.right.front() == "e"; }) != similiarRules.end();

	bool hasRecursion = false;
	for (size_t i = 0; i < similiarRules.size(); i++)
	{
		if (similiarRules[i].left == similiarRules[i].right[0])
		{
			hasRecursion = true;
		}
	}

	if (hasRecursion)
	{
		std::string randomedNonTerminal = "<R" + GenerateRandomNonTerminal() + ">";
		for (size_t i = 0; i < similiarRules.size(); i++)
		{
			Rules rule = similiarRules[i];
			Rules ruleWithoutRecursion;


			rule.right.push_back(randomedNonTerminal); // add new rule

			if (hasEmptyRule && i == 0)
			{
				outputGrammar.push_back({ rule.left, std::vector<std::string> {randomedNonTerminal} });
			}

			if (rule.left == rule.right.front())
			{
				ruleWithoutRecursion.left = randomedNonTerminal;
				ruleWithoutRecursion.right = std::vector<std::string>(rule.right.begin() + 1, rule.right.end());
			}
			else if (!hasEmptyRule)
			{
				ruleWithoutRecursion.left = rule.left;
				ruleWithoutRecursion.right = rule.right;
			}

			if (!ruleWithoutRecursion.left.empty())
			{
				outputGrammar.push_back(ruleWithoutRecursion);
			}
		}
		outputGrammar.push_back({ randomedNonTerminal, std::vector<std::string> {"e"} });
	}
	else
	{
		std::copy(similiarRules.begin(), similiarRules.end(), std::back_inserter(outputGrammar));
	}
}


void PrintGrammar(Grammar& const grammar)
{
	for (auto rules : grammar)
	{
		std::cout << rules.left << " -> ";

		for (auto rule : rules.right)
		{
			std::cout << rule << " ";
		}

		std::cout << "\n";
	}
}

void FactorizeGrammar(Grammar& grammar, const std::vector<std::string>& nonterminals)
{
	std::vector<std::string> tmp;
	bool hasEmptyRule = false;
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

		std::cout << "\nGrammar after factorize\n";
		PrintGrammar(grammar);
		Grammar out;

		RemoveRecursion(grammar, out, similiarRules);
		DeleteNonFactorizedRules(grammar, similiarRules);
		for (auto it = out.begin(); it != out.end(); it++)
		{
			grammar.push_back(*it);
		}


		std::cout << "\nGrammar after recursion\n";
		PrintGrammar(grammar);
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

void FormGuideSet(Grammar& grammar)
{

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


	std::sort(grammar.begin(), grammar.end(), [](Rules leftRules, Rules rightRules) {
		return leftRules.left < rightRules.left;
	});


	std::cout << "\nResult\n";
	PrintGrammar(grammar);

	//FormGuideSet(grammar);


	return grammar;
}


std::string ReadGrammarFromFile(const std::string inputFile)
{
	std::ifstream ifs(inputFile);
	std::string buffer((std::istreambuf_iterator<char>(ifs)),
		(std::istreambuf_iterator<char>()));
	return buffer;
}
