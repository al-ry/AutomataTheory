#include "Analyzerh.h"
#include <stack>
#include <optional>
#include <iostream>

void IterateTable(Table table, std::vector<std::string> inputSequence, size_t ruleIndex, size_t sequenceIndex, std::stack<size_t> stack)
{
	auto foundInGuideSet = std::find(table[ruleIndex].guideSet.begin(), table[ruleIndex].guideSet.end(), inputSequence[sequenceIndex]);

	if (foundInGuideSet != table[ruleIndex].guideSet.end())
	{
		if (table[ruleIndex].end)
		{
			std::cout << "Ok";
		}
		else if (!table[ruleIndex].isInStack)
		{
			if (table[ruleIndex].shift)
			{
				sequenceIndex++;
			}

			if (table[ruleIndex].pointer.has_value())
			{
				IterateTable(table, inputSequence, table[ruleIndex].pointer.value(), sequenceIndex, stack);
			}
			else if (!stack.empty())
			{
				auto row = stack.top();
				stack.pop();
				IterateTable(table, inputSequence, row, sequenceIndex, stack);
			}
		}
		else
		{
			stack.push(ruleIndex + 1);
			IterateTable(table, inputSequence, table[ruleIndex].pointer.value(), sequenceIndex, stack);

		}
	}
	else if (!table[ruleIndex].error)
	{
		IterateTable(table, inputSequence, ruleIndex + 1, sequenceIndex, stack);
	}
	else
	{
		std::cout << "Error " + std::to_string(ruleIndex);
	}
}

void AnalyzeTable(Table table, std::vector <std::string> inputSequence)
{
	std::stack<size_t> stack;
	IterateTable(table, inputSequence, 0, 0, stack);
}