#include "Generator.h"
#include <iterator>


std::vector<std::string> GetGuideSetForRule(Grammar &grammar, std::string nonterminal)
{
	std::vector<std::string> guideSet;

	for (auto &rule : grammar)
	{
		if (rule.left == nonterminal)
		{
			std::copy(rule.guideSet.begin(), rule.guideSet.end(), std::back_inserter(guideSet));
		}
	}

	return guideSet;
}

Table GenerateTable(Grammar grammar)
{
	TableWithRules tableWithRules;
	Table resultTable;
	bool isFirst = true;

	for (size_t i = 0; i < grammar.size(); i++)
	{
		if (isFirst)
		{
			TableRow row = { grammar[i].left, grammar[i].guideSet, true, std::nullopt, false, false, false };
			tableWithRules.push_back(std::make_pair(row, grammar[i]));
			resultTable.push_back(row);
			isFirst = false;
		}
		else
		{
			if (grammar[i].left == tableWithRules[i - 1].first.symbol)
			{
				tableWithRules[i - 1].first.error = false;
				resultTable[i - 1].error = false;
			}

			TableRow row = { grammar[i].left, grammar[i].guideSet, true, std::nullopt, false, false, false };
			tableWithRules.push_back(std::make_pair(row, grammar[i]));
			resultTable.push_back(row);
		}
	}

	TableWithRules tableWithRulesCopy = tableWithRules;

	for (size_t rowIndex = 0; rowIndex < tableWithRulesCopy.size(); rowIndex++)
	{

		resultTable[rowIndex].pointer = resultTable.size();

		auto tableRowWithRule = tableWithRulesCopy[rowIndex];
		for (size_t ruleSymbolIndex = 0; ruleSymbolIndex < tableRowWithRule.second.right.size(); ruleSymbolIndex++)
		{
			TableRow row;
			row.symbol = tableRowWithRule.second.right[ruleSymbolIndex];;

			if (IsNonterminal(row.symbol))
			{
				row.end = false;
				row.shift = false;
				row.guideSet = GetGuideSetForRule(grammar, row.symbol);
				row.pointer = std::distance(tableWithRules.begin(),
					std::find_if(tableWithRules.begin(), tableWithRules.end(), [&](TableRowWithRule& rowWithRule) {return row.symbol == rowWithRule.first.symbol; }));
				row.isInStack = (ruleSymbolIndex == tableRowWithRule.second.right.size() - 1) ? false : true;
				row.error = true;
			}
			else if (row.symbol == EMPTY_RULE)
			{
				row.guideSet = tableRowWithRule.second.guideSet;
				row.shift = false;
				row.pointer = std::nullopt;
				row.error = true;
				row.isInStack = false;
				row.end = false;
			}
			else
			{
				if (ruleSymbolIndex == tableRowWithRule.second.right.size() - 1)
				{
					row.pointer = std::nullopt;
				}
				else
				{
					row.pointer = resultTable.size() + 1;
				}

				row.guideSet = { row.symbol };
				row.end = row.symbol == END_SEQUENCE ? true : false;
				row.isInStack = false;
				row.shift = true;
				row.error = true;
			}

			resultTable.push_back(row);
		}
	}
	return resultTable;
};