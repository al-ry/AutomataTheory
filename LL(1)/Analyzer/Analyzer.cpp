#include "Analyzerh.h"
#include <stack>
#include <optional>
#include <iostream>
#include "../../Lexer/Lexer/lex.h"
#include "../../Lexer/Lexer/Common.h"

void PrintStack(std::stack<size_t> s)
{
	// If stack is empty then return
	if (s.empty())
		return;


	auto x = s.top();

	// Pop the top element of the stack
	s.pop();

	// Recursively call the function PrintStack
	PrintStack(s);

	// Print the stack element starting
	// from the bottom
	std::cout << x << " ";

	// Push the same element onto the stack
	// to preserve the order
	s.push(x);
}

void IterateTable(Table table, std::vector<std::string> inputSequence, size_t ruleIndex, size_t sequenceIndex, std::stack<size_t> stack, std::vector<std::string>& pointerVector)
{
	pointerVector.push_back(std::to_string(ruleIndex));
	auto foundInGuideSet = std::find(table[ruleIndex].guideSet.begin(), table[ruleIndex].guideSet.end(), inputSequence[sequenceIndex]);

	if (foundInGuideSet != table[ruleIndex].guideSet.end())
	{
		if (table[ruleIndex].end && sequenceIndex == inputSequence.size() - 1 && stack.empty())
		{
			std::cout << "Ok\n";
		}
		else if (!table[ruleIndex].isInStack)
		{
			if (table[ruleIndex].shift)
			{
				if (sequenceIndex != inputSequence.size() - 1)
				{
					sequenceIndex++;
				}
				else
				{
					std::cout << "Error. Sequence is empty\n";
					return;
				}
			}

			if (table[ruleIndex].pointer.has_value())
			{
				IterateTable(table, inputSequence, table[ruleIndex].pointer.value(), sequenceIndex, stack, pointerVector);
			}
			else if (!stack.empty())
			{
				auto row = stack.top();
				std::cout << "From stack " + std::to_string(row) + "\n";
				stack.pop();
				std::cout << "Stack: \n";
				PrintStack(stack);
				std::cout << "\n";
				IterateTable(table, inputSequence, row, sequenceIndex, stack, pointerVector);
			}
		}
		else
		{
			std::cout << "To stack " + std::to_string(ruleIndex + 1) + "\n";
			std::cout << "Stack: \n";
			PrintStack(stack);
			std::cout << "\n";
			stack.push(ruleIndex + 1);
			IterateTable(table, inputSequence, table[ruleIndex].pointer.value(), sequenceIndex, stack, pointerVector);
		}
	}
	else if (!table[ruleIndex].error)
	{
		IterateTable(table, inputSequence, ruleIndex + 1, sequenceIndex, stack, pointerVector);
	}
	else
	{
		std::cout << "Error " + std::to_string(ruleIndex) + "\n";
	}
}

void AnalyzeTable(Table table, std::vector<std::string> inputSequence)
{
	std::cout << "Input sequence: ";
	for (auto index : inputSequence)
	{
		std::cout << index + " ";
	}

	std::cout << "\n";
	std::stack<size_t> stack;
	std::vector<std::string> pointerVector;
	IterateTable(table, inputSequence, 0, 0, stack, pointerVector);
	std::cout << "Pointer sequence: ";
	for (auto index : pointerVector)
	{
		std::cout << index + " ";
	}
}


void IterateTable(Table table, Lexer& lexer, size_t ruleIndex, Token currentToken, std::stack<size_t> stack, std::vector<std::string>& pointerVector)
{
	pointerVector.push_back(std::to_string(ruleIndex));
	
	auto foundInGuideSet = std::find(table[ruleIndex].guideSet.begin(), table[ruleIndex].guideSet.end(), TOKEN_ADAPTATION.find(currentToken.kind)->second);

	if (foundInGuideSet != table[ruleIndex].guideSet.end())
	{
		if (table[ruleIndex].end && currentToken.kind == TokenKind::TOKEN_EOF && stack.empty())
		{
			std::cout << "Ok\n";
		}
		else if (!table[ruleIndex].isInStack)
		{
			if (table[ruleIndex].shift)
			{
				if (currentToken.kind != TokenKind::TOKEN_EOF)
				{
					currentToken = lexer.GetNextToken();
				}
				else
				{
					std::cout << "Error. Sequence is empty\n";
					return;
				}
			}

			if (table[ruleIndex].pointer.has_value())
			{
				IterateTable(table, lexer, table[ruleIndex].pointer.value(), currentToken, stack, pointerVector);
			}
			else if (!stack.empty())
			{
				auto row = stack.top();
				std::cout << "From stack " + std::to_string(row) + "\n";
				stack.pop();
				std::cout << "Stack: \n";
				PrintStack(stack);
				std::cout << "\n";
				IterateTable(table, lexer, row, currentToken, stack, pointerVector);
			}
		}
		else
		{
			std::cout << "To stack " + std::to_string(ruleIndex + 1) + "\n";
			std::cout << "Stack: \n";
			PrintStack(stack);
			std::cout << "\n";
			stack.push(ruleIndex + 1);
			IterateTable(table, lexer, table[ruleIndex].pointer.value(), currentToken, stack, pointerVector);
		}
	}
	else if (!table[ruleIndex].error)
	{
		IterateTable(table, lexer, ruleIndex + 1, currentToken, stack, pointerVector);
	}
	else
	{
		std::cout << "Error " + std::to_string(ruleIndex) + "\n";
		std::cout << "In Line: " << currentToken.loc.lineNum << '\n';
		std::cout << "In Pos: " << currentToken.loc.lineOffset << '\n';
	}
}


void AnalyzeTable(Table table, Lexer& lexer)
{
	std::cout << "Input sequence: ";
	//for (auto index : inputSequence)
	//{
	//	std::cout << index + " ";
	//}

	std::cout << "\n";
	std::stack<size_t> stack;
	std::vector<std::string> pointerVector;
	
	IterateTable(table, lexer, 0, lexer.GetNextToken(), stack, pointerVector);
	//std::cout << "Pointer sequence: ";
	//for (auto index : pointerVector)
	//{
	//	std::cout << index + " ";
	//}
}

