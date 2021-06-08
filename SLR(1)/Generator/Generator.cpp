#include "Generator.h"
#include <sstream>
#include <iostream>
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

bool IsNonterminal(std::string const& str)
{
	return !str.empty() && str.front() == '<' && str.back() == '>';
}

void PrintGrammar(Grammar& const grammar, std::ostream& output)
{
	for (auto rules : grammar)
	{
		output << rules.left << " -> ";

		for (auto rule : rules.right)
		{
			output << rule << " ";
		}

		output << " { ";
		for (auto guideSymbol : rules.guideSet)
		{
			output << guideSymbol << " ";
		}

		output << "}\n";

		output << "\n";
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

void AddNonterminals(std::vector<std::string>& nontemrinals, Grammar& const grammar)
{
	for (auto const& rule : grammar)
	{
		if (IsNonterminal(rule.left) && std::find(nontemrinals.begin(), nontemrinals.end(), rule.left) == nontemrinals.end())
		{
			nontemrinals.push_back(rule.left);
		}
	}
}


void AddEndSequenceToAxiom(Grammar& grammar, std::string const& axiom)
{
	for (auto& rule : grammar)
	{
		if (rule.left == axiom)
		{
			rule.right.push_back(END_SEQUENCE);
		}
	}
}


Grammar GetSortedGrammar(Grammar& const grammar)
{
	Grammar startsFromNonterminalsGrammar;
	Grammar startsFromTerminalsGrammar;
	Grammar emptyRulesGrammar;

	for (auto& rule : grammar)
	{
		if (IsNonterminal(rule.right.front()))
		{
			startsFromNonterminalsGrammar.push_back(rule);
		}
		else if (rule.right.front() == EMPTY_RULE)
		{
			emptyRulesGrammar.push_back(rule);
		}
		else
		{
			startsFromTerminalsGrammar.push_back(rule);
		}
	}

	std::copy(startsFromTerminalsGrammar.begin(), startsFromTerminalsGrammar.end(), std::back_inserter(startsFromNonterminalsGrammar));
	std::copy(emptyRulesGrammar.begin(), emptyRulesGrammar.end(), std::back_inserter(startsFromNonterminalsGrammar));



	return startsFromNonterminalsGrammar;
}

void SortGrammarByLeftNonterminal(Grammar& grammar, std::string const& axiom)
{
	std::sort(grammar.begin(), grammar.end(), [](Rules const& leftRule, Rules const& rightRule) {return leftRule.left > rightRule.left; });

	Grammar newGrammar;
	Grammar nonAxiomRules;

	for (auto& rule : grammar)
	{
		if (rule.left != axiom)
		{
			nonAxiomRules.push_back(rule);
		}
		else
		{
			newGrammar.push_back(rule);
		}
	}

	for (auto& rule : nonAxiomRules)
	{
		newGrammar.push_back(rule);
	}

	grammar = newGrammar;

}

Grammar CreateGrammar(std::istream& input)
{
	Grammar grammar;
	std::vector<std::string> nonterminals;
	std::string line;
	while (std::getline(input, line))
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
		for (auto& rule : rules)
		{
			rule.left = leftPart;
			grammar.push_back(rule);
		}
	}

	std::string axiom = grammar.front().left;

	//FactorizeGrammar(grammar, nonterminals);

	AddNonterminals(nonterminals, grammar);

	AddEndSequenceToAxiom(grammar, axiom);


	grammar = GetSortedGrammar(grammar);
	std::cout << "Formed grammar\n";
	//PrintGrammar(grammar, std::cout);

	//FormGuideSet(grammar, nonterminals);


	//PrintGrammar(grammar, std::cout);


	SortGrammarByLeftNonterminal(grammar, axiom);

	//std::cout << "\nResult\n";
	//PrintGrammar(grammar, std::cout);

	return grammar;
}

