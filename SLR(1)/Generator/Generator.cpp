#include "Generator.h"
#include <sstream>
#include <iostream>
#include <algorithm>
#include <set>
#include <unordered_set>
#include <iomanip>

std::vector<Rule> GetRules(const std::string& rules)
{
	std::stringstream ss(rules);
	std::string ruleStr;
	std::vector<Rule> rulesArr;
	Rule rule;
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

bool operator<(const Rule& lhs, const Rule& rhs) noexcept
{
	return lhs.left < rhs.left;
}

bool operator<(const Shift& lhs, const Shift& rhs) noexcept
{
	return lhs.ruleIndex < rhs.ruleIndex && lhs.indexInRule < rhs.ruleIndex;
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
	std::sort(grammar.begin(), grammar.end(), [](Rule const& leftRule, Rule const& rightRule) {return leftRule.left > rightRule.left; });

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

bool IsAxiomInRightSide(std::string const& axiom, Grammar grammar) {
	for (auto rule : grammar)
	{
		auto axiomIterator = std::find_if(rule.right.begin(), rule.right.end(), [&](std::string& currentSymbol) {return currentSymbol == axiom; });
		if (axiomIterator != rule.right.end())
		{
			return true;
		}
	}

	return false;
}

std::string GenerateRandomNonTerminal()
{
	static int counter = 0;
	std::string res = std::to_string(counter);
	counter++;
	return res;
}


std::string CreateNewAxiom(std::string const& axiom, Grammar& grammar)
{
	Rule newAxiom;
	newAxiom.left = "<Y" + GenerateRandomNonTerminal() + ">";
	newAxiom.right.push_back(axiom);
	grammar.insert(grammar.begin(), 1, newAxiom);

	return newAxiom.left;
}

std::vector<std::string> GetTerminals(const Grammar& grammar)
{
	std::vector<std::string> terminals;

	for (auto rules = grammar.rbegin(); rules != grammar.rend(); rules++)
	{
		for (auto it : rules->right)
		{
			if (it.front() != '<' &&
				std::find_if(terminals.begin(), terminals.end(), [&](std::string& terminal) { return terminal == it; }) == terminals.end())
			{
				terminals.push_back(it);
			}
		}
	}

	return terminals;
}

std::vector<std::string> GetNonterminals(const Grammar&grammar)
{
	std::vector<std::string> nonterminals;
	for (auto const& rule : grammar)
	{
		if (IsNonterminal(rule.left) && std::find(nonterminals.begin(), nonterminals.end(), rule.left) == nonterminals.end())
		{
			nonterminals.push_back(rule.left);
		}
	}
	return nonterminals;
}


std::vector<std::string> GetTerminalsAndNonterminals(const Grammar& grammar)
{
	auto terminals = GetTerminals(grammar);
	auto nonterminals = GetNonterminals(grammar);

	std::vector<std::string> inputSymbols;
	std::copy(nonterminals.begin(), nonterminals.end(), std::back_inserter(inputSymbols));
	std::copy(terminals.begin(), terminals.end(), std::back_inserter(inputSymbols));
	
	return inputSymbols;
}



std::vector<Shift> GetFirst(const Grammar& grammar, const Rule& rule, const std::set<Rule>& processedRules = {})
{
	std::vector<Shift> result;
	std::string firstRight = rule.right[0];
	size_t rulePos = std::distance(grammar.cbegin(), std::find(grammar.cbegin(), grammar.cend(), rule));


	if (processedRules.count(rule))
	{
		return {};
	}

	if (IsNonterminal(rule.right[0]))
	{
		for (const auto& currentRule: grammar)
		{
			if (currentRule.left == firstRight)
			{
				if (currentRule != rule)
				{
					auto tmpRules = processedRules;
					tmpRules.insert(rule);

					auto first = GetFirst(grammar, currentRule, tmpRules);

					std::copy(first.begin(), first.end(), std::back_inserter(result));
				}
				result.push_back({ rulePos, 0, currentRule.left });
			}
		}
	}
	else
	{
		result.push_back({ rulePos, 0, firstRight });
	}
	return result;
}

std::vector<Shift> GetFirstByNonterminal(const Grammar& grammar, const std::string& nonterminal)
{
	std::vector<Shift> shift;
	if (IsNonterminal(nonterminal))
	{
		for (auto& rule : grammar)
		{
			if (rule.left != nonterminal)
			{
				continue;
			}
			auto first = GetFirst(grammar, rule);
			std::set<Shift> res(first.begin(), first.end());
			std::copy(res.begin(), res.end(), std::back_inserter(shift));
		}
	}
	std::set<Shift> res(shift.begin(), shift.end());
	shift.assign(res.begin(), res.end());
	return shift;
}

template <typename T>
bool HasConflict(std::optional<TableValue> value)
{
	if (value.has_value() && std::holds_alternative<T>(value.value()))
	{
		return true;
	}

	return false;
}


void AssignRowValue(std::vector<std::optional<TableValue>>& values, const Row& row, const std::vector<Shift>& firsts)
{
	for (size_t i = 0; i < row.symbols.size(); i++)
	{
		std::vector<Shift> tmp;

		for (size_t j = 0; j < firsts.size(); j++)
		{
			if (row.symbols[i] == firsts[j].name)
			{
				if (HasConflict<Reduction>(values[i]))
				{
					throw std::invalid_argument("Shift reduction conflict");
				}
				tmp.push_back(firsts[j]);
			}
		}

		if (tmp.size() > 0)
		{
			if (values[i].has_value())
			{
				auto valuesArray = std::get<std::vector<Shift>>(values[i].value());
				for (auto tmpValue : tmp)
				{
					auto sameElement = std::find(valuesArray.begin(), valuesArray.end(), tmpValue);

					if (sameElement == valuesArray.end())
					{
						valuesArray.push_back(tmpValue);
					}
				}

				values[i] = valuesArray;
			}
			else
			{
				values[i] = tmp;
			}
			
		}
	}
}


Row CreateFirstRow(const Grammar& grammar, const std::vector<std::string>& inputSymbols)
{
	Row firstRow;
	Shift shift = { 0, 0, grammar[0].left };
	firstRow.state.push_back(shift);
	firstRow.symbols = inputSymbols;
	std::vector<std::optional<TableValue>> result;
	result.resize(firstRow.symbols.size());
	std::vector<Shift> firstsVec;
	firstsVec.push_back({ 0,0, grammar[0].right[0] });



	auto firsts = GetFirstByNonterminal(grammar, grammar[0].right[0]);
	std::copy(firsts.begin(), firsts.end(), std::back_inserter(firstsVec));

	result[0].emplace(true);
	AssignRowValue(result, firstRow, firstsVec);
	firstRow.val = result;
	//firstRow.val[0] = result;
	return firstRow;
}


std::vector<std::vector<Shift>> CreateNewTransitions(const std::vector<std::optional<TableValue>>& values, std::vector<std::vector<Shift>>& transitions)
{
	std::vector<std::vector<Shift>> newTransitions;
	for (auto& value : values)
	{
		if (value.has_value() &&
			std::visit([&](auto&& arg)
				{
					using T = std::decay_t<decltype(arg)>;
					if constexpr (std::is_same_v<T, std::vector<Shift>>)
					{
						return true;
					}
					else
					{
						return false;
					}
				}, *value))
		{
			auto shift = std::get<std::vector<Shift>>(*value);
			if (std::find(transitions.begin(), transitions.end(), shift) == transitions.end())
			{
				transitions.push_back(shift);
				newTransitions.push_back(shift);
			}
		}
	}
	return newTransitions;
}

bool IsEndRule(const std::string& symbol)
{
	return symbol == END_SEQUENCE;
}

std::vector<std::string> GetFollow(const Grammar& grammar, const std::string& nonterminal, std::set<std::string> follow = {})
{
	std::vector<std::string> result;

	for (const auto& [left, right] : grammar)
	{
		auto it = std::find_if(right.begin(), right.end(), [&](const std::string& symbol) {
			return symbol == nonterminal;
			});
		if (it != right.end())
		{
			for (auto symbolIt = it; symbolIt != right.end();)
			{
				if (symbolIt != --right.end())
				{
					auto next = ++symbolIt;
					if (!IsNonterminal(*next))
					{
						result.push_back(*next);

					}
					else
					{
						auto first = GetFirstByNonterminal(grammar, *next);
						result.push_back(*next);
						for (auto& item: first)
						{
							result.push_back(item.name);
						}
					}
					next = std::find_if(next, right.end(), [&](const std::string& symbol) {
						return symbol == nonterminal;
						});
					symbolIt = next;
				}
				else
				{
					if (!follow.count(left))
					{
						auto tmpFollow(follow);
						tmpFollow.insert(left);
						const auto tmp = GetFollow(grammar, left, tmpFollow);
						std::copy(tmp.cbegin(), tmp.cend(), std::back_inserter(result));
					}
					symbolIt++;
				}
			}
		}
	}

	return result;
}

void AssignReductionToTable(std::vector<std::optional<TableValue>> &result, const Rule& rule, size_t ruleIndex, const std::vector<std::string>&reductionSymbols, const std::vector<std::string>rowSymbols)
{
	for (size_t i = 0; i < rowSymbols.size(); i++)
	{
		Reduction red;
		for (size_t j = 0; j < reductionSymbols.size(); j++)
		{
			if (rowSymbols[i] == reductionSymbols[j])
			{
				if (HasConflict<std::vector<Shift>>(result[i]))
				{
					throw std::invalid_argument("Shift reduction conflict");
				}

				red.rule = &rule;
				red.index = ruleIndex;
				result[i].emplace(red);
			}
		}
	}
}

std::vector<std::optional<TableValue>> CreateNextTableValues(const Grammar& grammar, const Row& row, const std::vector<std::vector<Shift>>& shifts, const std::vector<Shift>& currentShift)
{
	std::vector<std::optional<TableValue>> result;
	result.resize(row.symbols.size());
	for (auto& shift : currentShift)
	{
		if (shift.indexInRule + 1 == grammar[shift.ruleIndex].right.size())
		{
			Reduction red;
			red.rule = &grammar[shift.ruleIndex];
			auto symbols = GetFollow(grammar, grammar[shift.ruleIndex].left);
			AssignReductionToTable(result, grammar[shift.ruleIndex], shift.ruleIndex, symbols, row.symbols);
		}
		else if (shift.indexInRule + 2 == grammar[shift.ruleIndex].right.size() && IsEndRule(grammar[shift.ruleIndex].right.back()))
		{
			Reduction red;
			red.rule = &grammar[shift.ruleIndex];
			red.index = shift.ruleIndex;
			result[row.symbols.size() - 1] = red;
		}
		else
		{
			auto symbol = grammar[shift.ruleIndex].right[shift.indexInRule + 1];
			if (IsNonterminal(symbol))
			{
				auto res = GetFirstByNonterminal(grammar, symbol);
				res.push_back({shift.ruleIndex, shift.indexInRule + 1, symbol});
				AssignRowValue(result, row, res);
			}
			else
			{

				auto pos = std::distance(row.symbols.begin(), std::find_if(row.symbols.begin(), row.symbols.end(), [&](const std::string& arg) { return  symbol == arg; }));
				std::vector<Shift> shifts;
				shifts.push_back({ shift.ruleIndex, shift.indexInRule + 1, symbol });

				if (HasConflict<Reduction>(result[pos]))
				{
					throw std::invalid_argument("Shift reduction conflict");
				}

				if (result[pos].has_value())
				{
					auto shiftValue = std::get<std::vector<Shift>>(result[pos].value());
					shiftValue.push_back(shifts[0]);
					result[pos] = shiftValue;
				}
				else
				{
					result[pos].emplace(shifts);
				}
			}
		}
	}
	return result;
}


std::vector<std::optional<TableValue>> GetAllValues(const std::vector<std::vector<std::optional<TableValue>>>& vec)
{
	std::vector<std::optional<TableValue>> result;
	for (const auto&values: vec)
	{
		for (auto&item: values)
		{
			result.push_back(item);
		}
	}
	return result;
}

Table CreateSLRTable(const Grammar& grammar)
{
	Table table;
	std::vector<std::string> inputSymbols = GetTerminalsAndNonterminals(grammar);
	

	Row firstRow = CreateFirstRow(grammar, inputSymbols);
	std::vector<std::vector<Shift>> shifts;
	auto newTransitions = CreateNewTransitions(firstRow.val, shifts);
	table.push_back(firstRow);

	do
	{
		std::vector<std::vector<std::optional<TableValue>>> newValuesVec;
		for (auto &trans :newTransitions)
		{
			Row row;
			row.state = trans;
			row.symbols = inputSymbols;
			auto newValues = CreateNextTableValues(grammar, row, shifts, trans);
			row.val = newValues;
			table.push_back(row);
			newValuesVec.push_back(newValues);
		}
		auto values = GetAllValues(newValuesVec);
		newTransitions = CreateNewTransitions(values, shifts);
	} while (!newTransitions.empty());

	return table;
}

std::vector<Rule> GetEmptyRules(Grammar const& grammar) {
	std::vector<Rule> result;
	for (auto rule : grammar)
	{
		if (rule.right[0] == EMPTY_RULE)
		{
			result.push_back(rule);
		}
	}

	return result;
}

void RemoveEmptyRule(Grammar &grammar)
{
	auto emptyRules = GetEmptyRules(grammar);

	for (size_t j = 0; j < emptyRules.size(); j++)
	{
		for (size_t i = 0; i < grammar.size(); i++)
		{
			std::vector<size_t> indexes;
			auto foundEmptyRule = std::find(grammar[i].right.begin(), grammar[i].right.end(), emptyRules[j].left);
			if (foundEmptyRule != grammar[i].right.end())
			{
				for (auto it = foundEmptyRule; it != grammar[i].right.end();)
				{
					auto foundEmptyRuleIndex = std::distance(grammar[i].right.begin(), it);
					indexes.push_back(foundEmptyRuleIndex);

					Rule newRule = grammar[i];
					newRule.right.erase(newRule.right.begin() + foundEmptyRuleIndex);

					if (newRule.right.size() == 0) 
					{
						newRule.right.push_back(EMPTY_RULE);
						auto foundInEmptyRules = std::find(emptyRules.begin(), emptyRules.end(), newRule);
						if (foundInEmptyRules == emptyRules.end()) { emptyRules.push_back(newRule); }
					}

					auto foundSameRule = std::find(grammar.begin(), grammar.end(), newRule);
					if (foundSameRule == grammar.end())
					{
						grammar.push_back(newRule);
					}

					it = std::find(++it, grammar[i].right.end(), emptyRules[j].left);
					if (it == grammar[i].right.end())
					{
						break;
					}
				}
			}
		}
	}

	for (auto smrIt = emptyRules.begin(); smrIt != emptyRules.end(); smrIt++)
	{
		for (auto grmIt = grammar.begin(); grmIt != grammar.end();)
		{
			auto end = grammar.end();
			if ((grmIt->right == smrIt->right && grmIt->left == smrIt->left) || (grmIt->right.size() == 1 && grmIt->right[0] == EMPTY_RULE ))
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

//void RemoveEmptyRigth(Grammar& grammar) {
//	for ()
//	{
//
//	}
//}

Grammar CreateGrammar(std::istream& input)
{
	Grammar grammar;
	std::vector<std::string> nonterminals;
	std::string line;
	while (std::getline(input, line))
	{
		if (line.size() == 0) continue;
		std::stringstream ruleLine(line);
		std::string tmp;
		std::string rightPart;
		std::string leftPart;
		ruleLine >> leftPart;
		AddNonteminal(nonterminals, leftPart);
		ruleLine >> tmp; //  reading ->
		std::getline(ruleLine, rightPart);
		std::vector<Rule> rules;
		rules = GetRules(rightPart);
		for (auto& rule : rules)
		{
			rule.left = leftPart;
			grammar.push_back(rule);
		}
	}
	
	RemoveEmptyRule(grammar);
	//RemoveEmptyRigth(grammar);
	std::string axiom = grammar.front().left;

	axiom = CreateNewAxiom(axiom, grammar);

	AddNonterminals(nonterminals, grammar);

	AddEndSequenceToAxiom(grammar, axiom);

	return grammar;
}


std::string StateToString(const std::vector<Shift>& state)
{
	std::stringstream ss;
	for (auto& st: state)
	{
		ss << st.name << st.ruleIndex + 1 << st.indexInRule + 1;
	}
	return ss.str();
}

void PrintTableValue(const TableValue & value)
{
	std::visit([&](auto&& arg)
		{
			using T = std::decay_t<decltype(arg)>;
			if constexpr (std::is_same_v<T, std::vector<Shift>>)
			{
				auto shift = std::get<std::vector<Shift>>(value);
				std::cout  << std::right  << StateToString(shift);
			}
			else if (std::is_same_v<T, Reduction>)
			{
				auto reduction = std::get<Reduction>(value);
				std::cout << std::right << "R" + std::to_string(reduction.index + 1) << std::setw(10) << std::setfill(' ');
			}
			else
			{
				std::cout << std::setw(20) << std::right << "OK";
			}
		}, value);
}

void PrintTableValueForAnalyzer(const TableValue& value, const Table & table, std::ostream& output)
{
	std::visit([&](auto&& arg)
		{
			using T = std::decay_t<decltype(arg)>;
			if constexpr (std::is_same_v<T, std::vector<Shift>>)
			{
				auto shift = std::get<std::vector<Shift>>(value);

				auto index = std::distance(table.begin(), std::find_if(table.begin(), table.end(), [&](Row row) {
					return row.state == shift;
				}));

				output << std::right << "S(" << index << ")";
			}
			else if (std::is_same_v<T, Reduction>)
			{
				auto reduction = std::get<Reduction>(value);
				output << std::right << "R(" << reduction.rule->left << reduction.rule->right.size() << ")" << std::setw(10) << std::setfill(' ');
			}
			else
			{
				output << std::setw(20) << std::right << "OK";
			}
		}, value);
}

void PrintInputSymbols(const std::vector<std::string>& symbols, std::ostream& output)
{
	for (size_t i = 0; i < symbols.size(); i++)
	{
		if (i == 0)
		{
			output << std::setw(30) << symbols[i] << std::setw(30);
		}
		else
		{
			output << std::setw(20) << symbols[i] << std::setw(30);
		}
	}
	output << "\n\n";
}

void PrintTable(const Table&table, std::ostream& output)
{
	PrintInputSymbols(table[0].symbols, output);
	for (const auto& row : table)
	{
		std::cout <<std::setw(30) << std::left << StateToString(row.state);
		for (size_t i = 0; i < row.symbols.size(); i++)
		{
			std::cout << std::setw(30) << std::setfill(' ');
			if (row.val[i].has_value())
			{
				PrintTableValue(row.val[i].value());
			}
			else
			{
				std::cout << std::right << "-" << std::setw(30) << std::setfill(' ');
			}
		}
		std::cout << "\n\n";
	}
}

void PrintTableForAnalyze(const Table& table, std::ostream& output)
{
	PrintInputSymbols(table[0].symbols, output);
	for (const auto& row : table)
	{
		//output << std::setw(10) << std::left << StateToString(row.state);
		for (size_t i = 0; i < row.symbols.size(); i++)
		{
			output << std::setw(30) << std::setfill(' ');
			if (row.val[i].has_value())
			{
				PrintTableValueForAnalyzer(row.val[i].value(), table, output);
			}
			else
			{
				output << std::right << "-" << std::setw(30) << std::setfill(' ');
			}
		}
		output << "\n\n";
	}
}
