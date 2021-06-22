#include "Utils.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <utility>
#include <iterator>
#include <set>

std::vector<std::pair<std::string, std::string>> IsLLGrammar(Grammar const& grammar)
{
	std::vector<std::pair<std::string, std::string>> nonLLRules;
	for (auto & rule : grammar)
	{
		for (auto currentRule : grammar)
		{
			if (rule.left == currentRule.left && rule.right == currentRule.right)
			{
				continue;
			}

			if (rule.left == currentRule.left)
			{
				for (auto & item : rule.guideSet)
				{
					auto foundSameItem = std::find_if(currentRule.guideSet.begin(), currentRule.guideSet.end(), [&](std::string const& foundSymbol)
						{ return foundSymbol == item; });

					if (foundSameItem != currentRule.guideSet.end())
					{
						auto alreadyExists = std::find_if(nonLLRules.begin(), nonLLRules.end(), [&](std::pair<std::string, std::string> pair) {
							return item == pair.first; });
						if (alreadyExists == nonLLRules.end())
						{
							nonLLRules.push_back(std::make_pair(currentRule.left, item));
						}

					}
				}
			}
		}
	}

	return nonLLRules;
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

void DeleteNonFactorizedRules(Grammar& grammar, const Grammar& similiarRules)
{
	Grammar tmpGrammar;
	Grammar copy = grammar;

	for (auto smrIt = similiarRules.begin(); smrIt != similiarRules.end(); smrIt++)
	{
		for (auto grmIt = grammar.begin(); grmIt != grammar.end();)
		{
			auto end = grammar.end();
			if (grmIt->right == smrIt->right && grmIt->left == smrIt->left)
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

bool Factorize(Grammar& grammar, Grammar& similiarRules)
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

	if (rulesWithSimPart.size() == 0)
	{
		return false;
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
			else
			{
				break;
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

	return true;
}


bool RemoveRecursion(Grammar& grammar, Grammar& outputGrammar, std::vector<Rules>& similiarRules)
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
			else if (rule.right[0] != EMPTY_RULE)
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

		return true;
	}
	else
	{
		std::copy(similiarRules.begin(), similiarRules.end(), std::back_inserter(outputGrammar));
		return false;
	}
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


void PrintTable(Table table, std::ostream& out)
{
	out << "Number" << TAB << "Symbol" << TAB << "GS" << TAB << "Shift" << TAB << "Error" << TAB << "Pointer" << TAB << "Stack" << TAB << "End" << TAB << std::endl;
	std::ostream_iterator<std::string> output_iterator(out, " ");


	for (size_t i = 0; i < table.size(); ++i)
	{
		size_t counter = i;

		out << counter << TAB << table[i].symbol << TAB;
		std::copy(table[i].guideSet.begin(), table[i].guideSet.end(), output_iterator);
		out << TAB << table[i].shift << TAB << table[i].error << TAB << (table[i].pointer.has_value() ? std::to_string(table[i].pointer.value()) : "null") << TAB << table[i].isInStack << TAB << table[i].end << TAB;
		out << "\n";
	}
};

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

		//std::cout << "\nGrammar after factorize\n";
		//PrintGrammar(grammar, std::cout);
		Grammar out;

		if (RemoveRecursion(grammar, out, similiarRules))
		{
			DeleteNonFactorizedRules(grammar, similiarRules);

			for (auto it = out.begin(); it != out.end(); it++)
			{
				grammar.push_back(*it);
			}
		};

		//std::cout << "\nGrammar after recursion\n";
		//PrintGrammar(grammar, std::cout);
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

std::vector<std::string> GetTerminals(Grammar &const grammar) 
{
	std::vector<std::string> terminals;

	for (auto rules: grammar)
	{
		for (auto it : rules.right)
		{
			if (it.front() != '<' &&
				std::find_if(terminals.begin(), terminals.end(), [&](std::string &terminal){ return terminal == it;}) == terminals.end())
			{
				terminals.push_back(it);
			}
		}
	}

	return terminals;
}

bool IsSameRules(Rules const& firstRule, Rules const& secondRule)
{
	if (firstRule.left != secondRule.left || firstRule.right.size() != secondRule.right.size())
	{
		return false;
	}


	for (size_t i = 0; i < firstRule.right.size(); i++)
	{
		if (firstRule.right[i] != secondRule.right[i])
		{
			return false;
		}
	}

	return true;
}


void FindTerminalsForEmptyRule(Grammar const& grammar, Rules parentRule, Rules rule, std::optional<Rules> prevRule,
	std::vector<Transition>& transitions, std::vector<HasTransitionPair>& const terminalsAndNonterminals, std::vector<Rules> & passedRules)
{
	for (auto& foundRule : grammar)
	{
		auto foundInOtherRule = std::find_if(foundRule.right.begin(), foundRule.right.end(), [&](std::string const& rightSideSymbol) { return rightSideSymbol == rule.left; });

		if (foundInOtherRule != foundRule.right.end())
		{
			size_t foundNonterminalIndex = std::distance(foundRule.right.begin(), foundInOtherRule);

			if (foundNonterminalIndex == foundRule.right.size() - 1)
			{
				auto isPassed = std::find_if(passedRules.begin(), passedRules.end(), [&](Rules const& anyRule) {
					return anyRule.left == foundRule.left && anyRule.right == foundRule.right; });

				if (!IsSameRules(rule, foundRule) && foundRule.left != parentRule.left && isPassed == passedRules.end())
				{
					passedRules.push_back(foundRule);
					FindTerminalsForEmptyRule(grammar, parentRule, foundRule, rule, transitions, terminalsAndNonterminals, passedRules);
				}
			}
			else
			{
				std::string nextSymbol = foundRule.right[foundNonterminalIndex + 1];

				size_t parentIndex = std::distance(transitions.begin(),
					std::find_if(transitions.begin(), transitions.end(), [&](Transition& transition) { return transition.first == parentRule.left; }));

				size_t transitionIndex = std::distance(terminalsAndNonterminals.begin(),
					std::find_if(terminalsAndNonterminals.begin(), terminalsAndNonterminals.end(),
						[&](HasTransitionPair& const hasTransitionPair) { return hasTransitionPair.first == nextSymbol; }));

				if (nextSymbol == foundRule.left)
				{
					nextSymbol = foundRule.right.front();

					for (auto &checkRule : grammar)
					{
						if (checkRule.left == foundRule.left)
						{
							size_t checkRuleTransitionId = std::distance(transitions.begin(),
								std::find_if(transitions.begin(), transitions.end(), [&](Transition& transition) { return transition.first == checkRule.left; }));

							for (size_t i = 0; i < transitions[checkRuleTransitionId].second.size(); i++)
							{
								if (transitions[checkRuleTransitionId].second[i].second != 0)
								{
									transitions[parentIndex].second[i].second += 1;
								}
							}
						}
					}
				}
				else
				{
					if (parentIndex < transitions.size() && transitionIndex < terminalsAndNonterminals.size() && nextSymbol != parentRule.left)
					{
						transitions[parentIndex].second[transitionIndex].second += 1;
					}
				}
			}
		}
	}
}

void CreateFirstRelation(std::vector<HasTransitionPair>& const terminalsAndNonterminals, 
	std::vector<Transition>& transitions, Grammar const& grammar)
{
	for (auto& rule: grammar)
	{
		size_t nonterminalTransitionIndex = std::distance(transitions.begin(), 
			std::find_if(transitions.begin(), transitions.end(), [&](Transition& transition) { return transition.first == rule.left; }));

		size_t destinationIndex = std::distance(terminalsAndNonterminals.begin(),
			std::find_if(terminalsAndNonterminals.begin(), terminalsAndNonterminals.end(), 
				[&](HasTransitionPair& const hasTransitionPair) { return hasTransitionPair.first == rule.right.front(); }));

		if (nonterminalTransitionIndex < transitions.size() && destinationIndex < terminalsAndNonterminals.size())
		{
			if (rule.right.front() == EMPTY_RULE)
			{
				std::vector<Rules> passedRules;
				FindTerminalsForEmptyRule(grammar, rule, rule, std::nullopt, transitions, terminalsAndNonterminals, passedRules);
			}
			else
			{
				transitions[nonterminalTransitionIndex].second[destinationIndex].second += 1;
			}
		}
	}
}


void CreateFirstPlusRelation(std::vector<Transition>& transitions) 
{
	for (auto it = transitions.rbegin(); it != transitions.rend(); ++it)
	{

		for (size_t i = 0; i < transitions.size(); i++)
		{
			if ((*it).second[i].second)
			{
				size_t foundNonterminalIndex = std::distance(transitions.begin(),
					std::find_if(transitions.begin(), transitions.end(), [&](Transition& transition) { return transition.first == transition.second[i].first; }));

				for (size_t j = 0; j < transitions[foundNonterminalIndex].second.size(); j++)
				{
					if (transitions[foundNonterminalIndex].second[j].second)
					{
						(*it).second[j].second += 1;
					}
				}
			}
		}
	}
}

bool IsNonterminal(std::string const& str)
{
	return !str.empty() && str.front() == '<' && str.back() == '>';
}


std::vector<std::string> GetAllTerminalsOf(std::string& nonterminal, std::vector<Transition> transitions)
{
	std::vector<std::string> terminals;

	size_t nonterminalTransitionIndex = std::distance(transitions.begin(),
		std::find_if(transitions.begin(), transitions.end(), [&](Transition& transition) { return transition.first == nonterminal; }));

	for (size_t i = 0; i < transitions[nonterminalTransitionIndex].second.size(); i++)
	{
		if (transitions[nonterminalTransitionIndex].second[i].second && !IsNonterminal(transitions[nonterminalTransitionIndex].second[i].first))
		{
			terminals.push_back(transitions[nonterminalTransitionIndex].second[i].first);
		}
	}

	return terminals;
}

std::vector<std::string> GetFollow(const Grammar& rules, std::string nonTerminal, std::set<std::string> way = {})
{
	std::vector<std::string> result;
	for (const auto& subRule : rules)
	{
		if (auto it = std::find_if(subRule.right.cbegin(), subRule.right.cend(), [&](std::string_view sv) {
			return sv == nonTerminal;
			}); it != subRule.right.cend())
		{
			size_t distance = std::distance(subRule.right.cbegin(), it);
			size_t size = subRule.right.size() - 1;

			if (const auto bla = (distance <= size)
				? ((distance < size) ? subRule.right[distance + 1] : subRule.right.back())
				: "e"; bla != nonTerminal)
			{
				result.push_back(bla);

				if ((bla == "$") && !way.count(subRule.left))
				{
					auto tmpWay(way);
					tmpWay.insert(subRule.left);
					const auto tmp = GetFollow(rules, subRule.left, tmpWay);
					for (auto symbol : tmp)
					{
						auto foundSame = std::find(result.begin(), result.end(), symbol);

						if (foundSame == result.end())
						{
							result.push_back(symbol);
						}
					}
				}
			}
			else if (!way.count(subRule.left))
			{
				auto tmpWay(way);
				tmpWay.insert(subRule.left);
				const auto tmp = GetFollow(rules, subRule.left, tmpWay);

				for (auto symbol : tmp)
				{
					auto foundSame = std::find(result.begin(), result.end(), symbol);

					if (foundSame == result.end())
					{
						result.push_back(symbol);
					}
				}
			}
		}
	}
	return result;
}

std::vector<std::string> GetFirst(const Grammar& rules, Rules processingRule)
{
	std::vector<std::string> result;

	const auto processingLeft = processingRule.left;
	const auto firstProcessingRight = processingRule.right.front();

	if (firstProcessingRight == "e")
	{
		const auto foundFollow = GetFollow(rules, processingLeft);
		for (auto symbol : foundFollow)
		{
			auto foundSame = std::find(result.begin(), result.end(), symbol);

			if (foundSame == result.end())
			{
				result.push_back(symbol);
			}
		}
		
	}
	else if (IsNonterminal(firstProcessingRight))
	{
		for (const auto& rule : rules)
		{
			if (IsNonterminal(firstProcessingRight))
			{
				if (rule.left == firstProcessingRight)
				{
					const auto foundFirsts = GetFirst(rules, rule);
					for (auto symbol : foundFirsts)
					{
						auto foundSame = std::find(result.begin(), result.end(), symbol);

						if (foundSame == result.end())
						{
							result.push_back(symbol);
						}
					}
				}
			}
		}
	}
	else
	{
		result = { firstProcessingRight };
	}
	return result;
}

std::vector<std::string> GetGuideCharsByRule(const Grammar& rules, Rules processingRule)
{
	auto result = GetFirst(rules, processingRule);

	decltype(result) terminals;
	decltype(result) nonTerminals;

	auto it = std::partition(result.begin(), result.end(), [](const std::string& sv) {
		return IsNonterminal(sv);
		});
	std::copy(result.begin(), it, std::back_inserter(nonTerminals));
	std::copy(it, result.end(), std::back_inserter(terminals));
	result = terminals;

	for (const auto& nonTerminal : nonTerminals)
	{
		for (const auto& rule : rules)
		{
			if (rule.left == nonTerminal)
			{
				const auto guideSetFound = GetGuideCharsByRule(rules, rule);
				for (auto symbol : guideSetFound)
				{
					auto foundSame = std::find(result.begin(), result.end(), symbol);

					if (foundSame == result.end())
					{
						result.push_back(symbol);
					}
				}
			}
		}
	}
	return result;
};


void FormGuideSet(Grammar& grammar, std::vector<std::string>& nonterminals)
{
	std::vector<std::string> terminals = GetTerminals(grammar);
	std::vector<HasTransitionPair> terminalsAndNonterminals;

	std::vector<Transition> transitions;

	std::for_each(nonterminals.begin(), nonterminals.end(), [&](std::string nonterminal) {terminalsAndNonterminals.push_back({ nonterminal, 0 }); });
	std::for_each(terminals.begin(), terminals.end(), [&](std::string terminal) {terminalsAndNonterminals.push_back({ terminal, 0 }); });


	std::for_each(nonterminals.begin(), nonterminals.end(),
		[&](std::string const& nontreminal) { transitions.push_back(std::make_pair(nontreminal, terminalsAndNonterminals)); });

	CreateFirstRelation(terminalsAndNonterminals, transitions, grammar);
	CreateFirstPlusRelation(transitions);

	for (auto &rule : grammar)
	{
		size_t nonterminalTransitionIndex = std::distance(transitions.begin(),
			std::find_if(transitions.begin(), transitions.end(), [&](Transition& transition) { return transition.first == rule.left; }));

		auto similarNonterminalWithEmptyRule = std::find_if(grammar.begin(), grammar.end(), [&](const Rules& similarRule) { return rule.left == similarRule.left && similarRule.right.front() == EMPTY_RULE; });

		if (nonterminalTransitionIndex < transitions.size())
		{
			std::vector<std::string> guideSet;
			std::string firstRuleSymbol = rule.right.front();

			if (IsNonterminal(firstRuleSymbol))
			{
				//rule.guideSet = GetAllTerminalsOf(firstRuleSymbol, transitions);

				if (similarNonterminalWithEmptyRule != grammar.end())
				{
					for (size_t i = 0; i < transitions[nonterminalTransitionIndex].second.size(); i++)
					{
						auto foundSimilarSymbol = std::find(rule.guideSet.begin(), rule.guideSet.end(), transitions[nonterminalTransitionIndex].second[i].first);

						if (foundSimilarSymbol != rule.guideSet.end())
						{
							transitions[nonterminalTransitionIndex].second[i].second -= 1;
						}
					}
				}
			}
			else
			{
				for (size_t i = nonterminals.size(); i < transitions[nonterminalTransitionIndex].second.size(); i++)
				{
					if (transitions[nonterminalTransitionIndex].second[i].second)
					{
						std::string transitionSymbol = transitions[nonterminalTransitionIndex].second[i].first;
						if ((firstRuleSymbol == transitionSymbol || firstRuleSymbol == EMPTY_RULE))
						{
							guideSet.push_back(transitionSymbol);
							transitions[nonterminalTransitionIndex].second[i].second -= 1;
						}


					}
				}
				//rule.guideSet = guideSet;
			}
		}
	}

	for (auto& outputData : grammar)
	{
		outputData.guideSet = GetGuideCharsByRule(grammar, outputData);
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

void SortGrammarByLeftNonterminal(Grammar &grammar, std::string const& axiom)
{
	std::sort(grammar.begin(), grammar.end(), [](Rules const& leftRule, Rules const& rightRule) {return leftRule.left > rightRule.left; });

	Grammar newGrammar;
	Grammar nonAxiomRules;

	for (auto &rule : grammar)
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

	for (auto &rule : nonAxiomRules)
	{
		newGrammar.push_back(rule);
	}

	grammar = newGrammar;

}

bool IsAxiomInRightSide(std::string const& axiom, Grammar grammar) {
	for (auto rule : grammar)
	{
		auto axiomIterator = std::find_if(rule.right.begin(), rule.right.end(), [&](std::string & currentSymbol) {return currentSymbol == axiom;});
		if (axiomIterator != rule.right.end())
		{
			return true;
		}
	}

	return false;
}

std::string CreateNewAxiom(std::string const& axiom, Grammar & grammar)
{
	Rules newAxiom;
	newAxiom.left = "<Y" + GenerateRandomNonTerminal() + ">";
	newAxiom.right.push_back(axiom);
	grammar.insert(grammar.begin(), 1, newAxiom);

	return newAxiom.left;
}

Grammar CreateGrammar(std::istream & input)
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
		for (auto &rule: rules)
		{
			rule.left = leftPart;
			grammar.push_back(rule);
		}
	}

	std::string axiom = grammar.front().left;

	FactorizeGrammar(grammar, nonterminals);

	if (IsAxiomInRightSide(axiom, grammar))
	{
		axiom = CreateNewAxiom(axiom, grammar);
	}

	AddEndSequenceToAxiom(grammar, axiom);

	AddNonterminals(nonterminals, grammar);
	grammar = GetSortedGrammar(grammar);
	FormGuideSet(grammar, nonterminals);

	SortGrammarByLeftNonterminal(grammar, axiom);

	return grammar;
}

std::string ReadGrammarFromFile(const std::string inputFile)
{
	std::ifstream ifs(inputFile);
	std::string buffer((std::istreambuf_iterator<char>(ifs)),
		(std::istreambuf_iterator<char>()));
	return buffer;
}
