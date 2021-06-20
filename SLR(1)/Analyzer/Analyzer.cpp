#include "Analyzer.h"
#include <stack>
#include <stdexcept>
#include <iostream>
#include <sstream>

std::vector<std::string> ReadSymbols(std::istream& input) 
{
	std::vector<std::string> result;

	std::string line;
	std::getline(input, line);
	std::istringstream ss(line);

	while (ss >> line)
	{
		result.push_back(line);
	}

	return result;
}

AnalyzerReduction ParseReduction(std::string& value) 
{
	AnalyzerReduction reduction;
	std::string nonterminalName;
	std::string size;
	std::istringstream ss(value);
	std::string line;
	size_t sizeIndex = 0;

	for (size_t i = 0; i < value.size(); i++)
	{
		nonterminalName.push_back(value[i]);

		if (value[i] == '>')
		{
			sizeIndex = i + 1;
			break;
		}
	}

	for (size_t i = sizeIndex; i < value.size(); i++)
	{
		size.push_back(value[i]);
	}

	reduction.reductionNonterminal = nonterminalName;
	reduction.symbolCount = std::stoi(size);

	return reduction;
}

AnalyzerShift ParseShift(std::string& value)
{
	AnalyzerShift shift;
	std::istringstream ss(value);

	std::string line;

	ss >> line;
	shift.rowIndex = std::stoi(line);

	return shift;

}

std::optional<AnalyzerTableValue> GetCurrentValue(std::string value) 
{
	if (value == "OK")
	{
		return true;
	}

	else if (value == "-")
	{
		return std::nullopt;
	}

	else if (value[0] == 'R')
	{
		std::string reduction = value.substr(2, value.size() - 3);
		return ParseReduction(reduction);
	}

	else if (value[0] == 'S')
	{
		std::string shift = value.substr(2, value.size() - 3);
		return ParseShift(shift);
	}
}

std::vector<std::optional<AnalyzerTableValue>> GetNewTableRow(std::string row)
{
	std::vector<std::optional<AnalyzerTableValue>> values;
	std::string line;
	std::istringstream ss(row);

	while (ss >> line)
	{
		values.push_back(GetCurrentValue(line));
	}

	return values;
}



AnalyzerTable ReadTable(std::istream& input)
{
	AnalyzerTable table;
	std::vector<std::string> symbols = ReadSymbols(input);
	std::string tableRow;

	while (std::getline(input, tableRow))
	{
		if (!tableRow.empty())
		{
			table.push_back({ symbols, GetNewTableRow(tableRow) });
		}
	}

	return table;

}

void AnalyzeTable(AnalyzerTable const& table, Lexer& lexer)
{
	std::stack<AnalyzerRow> stateStack;
	std::stack<std::string> sequenceStack;
	std::stack<std::string> reductionStack;
	std::vector<std::string> inputSymbols = table[0].symbols;

	stateStack.push(table[0]);
	Token currentToken;
	currentToken = lexer.GetNextToken();
	while (currentToken.kind != TokenKind::TOKEN_EOF)
	{
		auto currentState = stateStack.top();

		auto it = std::find(inputSymbols.begin(), inputSymbols.end(), currentToken.name);
		auto itIndex = std::distance(inputSymbols.begin(), it);


		if (currentState.val[itIndex].has_value())
		{
			auto tableValue = currentState.val[itIndex].value();

			std::visit([&](auto&& arg)
				{
					using T = std::decay_t<decltype(arg)>;
					if constexpr (std::is_same_v<T, AnalyzerShift>)
					{
						auto shift = std::get<AnalyzerShift>(tableValue);
						auto foundRow = table[shift.rowIndex];
						stateStack.push(foundRow);
						//i++;
						currentToken = lexer.GetNextToken();
					}
					else if (std::is_same_v<T, AnalyzerReduction>)
					{
						auto reduction = std::get<AnalyzerReduction>(tableValue);

						size_t reductionSize = reduction.symbolCount;
						if (reduction.reductionNonterminal == table[0].symbols[0])
						{
							reductionSize--;
						}
						//else
						//{
						//	i--;
						//}

						for (size_t i = 0; i < reductionSize; i++)
						{
							stateStack.pop();
							sequenceStack.pop();
						}

						//inputSequence[i] = reduction.reductionNonterminal;
						reductionStack.push(reduction.reductionNonterminal);
						
					}
					else
					{
						std::cout << "Ok";
						//i++;

						currentToken = lexer.GetNextToken();
					}
				}, tableValue);
		}
		else
		{
			std::cout << "Error. " << " Found: " << currentToken.name << '\n';
			std::cout << "Expected: ";
			for (size_t j = 0; j < currentState.val.size(); j++)
			{

				if (currentState.val[j].has_value())
				{
					auto currentRow = currentState.val[j].value();
					std::visit([&](auto&& arg)
						{
							using T = std::decay_t<decltype(arg)>;
							if constexpr (std::is_same_v<T, AnalyzerShift>)
							{
								std::cout << currentState.symbols[j] << " ";
							}
							else if (std::is_same_v<T, AnalyzerReduction>)
							{
								std::cout << currentState.symbols[j] << " ";
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