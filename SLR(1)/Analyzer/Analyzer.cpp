#include "Analyzer.h"
#include <stack>
#include <stdexcept>
#include <iostream>


void AnalyzeTable(Table const& table, std::vector<std::string> inputSequence)
{
	std::stack<Row> stateStack;
	std::stack<std::string> sequenceStack;
	std::vector<std::string> inputSymbols = table[0].symbols;

	stateStack.push(table[0]);

	for (size_t i = 0; i < inputSequence.size();)
	{
		auto currentState = stateStack.top();

		auto it = std::find(inputSymbols.begin(), inputSymbols.end(), inputSequence[i]);
		auto itIndex = std::distance(inputSymbols.begin(), it);


		if (currentState.val[itIndex].has_value())
		{
			auto tableValue = currentState.val[itIndex].value();

			std::visit([&](auto&& arg)
				{
					using T = std::decay_t<decltype(arg)>;
					if constexpr (std::is_same_v<T, std::vector<Shift>>)
					{
						auto shift = std::get<std::vector<Shift>>(tableValue);
						auto foundRow = std::find_if(table.begin(), table.end(), [&](Row const& row) { return row.state == shift; });
						stateStack.push(*foundRow);
						sequenceStack.push(inputSequence[i]);
						i++;
					}
					else if (std::is_same_v<T, Reduction>)
					{
						auto reduction = std::get<Reduction>(tableValue);

						size_t reductionSize = reduction.rule->right.size();
						if (reduction.rule->left == table[0].state[0].name)
						{
							reductionSize--;
						}
						else
						{
							i--;
						}

						for (size_t i = 0; i < reductionSize; i++)
						{
							stateStack.pop();
							sequenceStack.pop();
						}

						inputSequence[i] = reduction.rule->left;
						
					}
					else
					{
						std::cout << "Ok";
						i++;
					}
				}, tableValue);
		}
		else
		{
			std::cout << "Error. " << " Found: " << inputSequence[i] << '\n';
			std::cout << "Expected: ";
			for (size_t j = 0; j < currentState.val.size(); j++)
			{

				if (currentState.val[j].has_value())
				{
					auto currentRow = currentState.val[j].value();
					std::visit([&](auto&& arg)
						{
							using T = std::decay_t<decltype(arg)>;
							if constexpr (std::is_same_v<T, std::vector<Shift>>)
							{
								auto shifts = std::get<std::vector<Shift>>(currentRow);
								std::cout << shifts[0].name << " ";
							}
							else if (std::is_same_v<T, Reduction>)
							{
								std::cout << currentState.symbols[j];
							}
							else
							{
							}
						}, currentRow);
				}
			}
			return;
		}
	}
}