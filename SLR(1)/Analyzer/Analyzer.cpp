#include "Analyzer.h"
#include "../../Lexer/Lexer/Common.h"
#include <stack>
#include <stdexcept>
#include <iostream>
#include <sstream>
#include <fstream>

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

std::string GetTokenTypeMapping(const Token& token) {
	switch (token.kind)
	{
	case TokenKind::TOKEN_INT:
		return "int";
	case TokenKind::TOKEN_INT_KEYWORD:
		return "int";
	case TokenKind::TOKEN_FLOAT:
		return "float";
	case TokenKind::TOKEN_FLOAT_KEYWORD:
		return "float";
	case TokenKind::TOKEN_BOOL_KEYWORD:
		return "bool";
	case TokenKind::TOKEN_NAME:
		return token.name;
	case TokenKind::TOKEN_STR:
		return "str";
	case TokenKind::TOKEN_STR_KEYWORD:
		return "string";
	case TokenKind::TOKEN_CHAR_KEYWORD:
		return "char";
	default:
		return "unknown_type";
	}
}

bool IsTypedef(TokenKind kind) {
	if (kind == TokenKind::TOKEN_BOOL_KEYWORD
		|| kind == TokenKind::TOKEN_FLOAT_KEYWORD
		|| kind == TokenKind::TOKEN_INT_KEYWORD
		|| kind == TokenKind::TOKEN_STR_KEYWORD
		|| kind == TokenKind::TOKEN_CHAR_KEYWORD
		|| kind == TokenKind::TOKEN_NAME) {
		return true;
	}
	return false;
}


void CreateBlock(SymbolTable& symbolTable) {
	symbolTable.table.push_back({});
}


void InitFirstBlock(SymbolTable& symbolTable, std::ofstream & out) {
	if (symbolTable.table.empty()) {
		out << "Global block started\n";
		symbolTable.nestingLevel++;
		CreateBlock(symbolTable);
	}
}


void RemoveBlock(SymbolTable& symbolTable) {
	if (!symbolTable.table.empty()) {
		symbolTable.table.pop_back();
	}
}

void CloseGlobalBlock(SymbolTable& symbolTable, std::ofstream& out)
{
	if (!symbolTable.table.empty()) {
		symbolTable.table.pop_back();
		out << "Global block ended up\n";
	}

}


void AddTabsToOut(int count, std::ofstream& out)
{
	for (size_t i = 0; i < count; i++)
	{
		out << "  ";
	}

}

bool CheckIfVariableExist(std::vector<std::pair<std::string, std::string>>& currentScope,
	const std::string &name) {
	for (auto& var: currentScope)
	{
		if (var.first == name) {
			return true;
		}
	}
	return false;
}

bool IsDefinedStructWithName(const SymbolTable &symbolTable,const std::string& name)
{
	std::vector<StructInfo> structs = symbolTable.structs;
	for (auto& structInfo: structs)
	{
		if (structInfo.name == name)
		{
			return true;
		}
	}
	return false;
}

bool IsPrimitiveType(const std::string& name) {
	if (name == "int")
	{
		return true;
	}
	else if (name == "char")
	{
		return true;
	}
	else if (name == "float")
	{
		return true;
	}
	else if (name == "bool")
	{
		return true;
	}
	else if (name == "string") {
		return true;
	}
	return false;
}

std::string TryToProcessStruct(const std::string& name, SymbolTable& symbolTable)
{
	if (!IsPrimitiveType(name))
	{
		if (IsDefinedStructWithName(symbolTable, name))
		{
			return name;
		}
		else
		{
			std::cout << "Undefined struct: " << name;
			throw std::exception("Undefined struct");
		}
	}
	return name;
}

void ProcessVariable(const Token& token, SymbolTable& symbolTable, std::ofstream& out)
{
	auto& back = symbolTable.table.back();
	if (IsTypedef(token.kind) && !symbolTable.isWaitingForName && !symbolTable.isVariableDeclared)
	{
		symbolTable.isWaitingForName = true;
		//symbolTable.currentVariable = std::make_pair("", type);
		symbolTable.currentVariable = std::make_pair("", GetTokenTypeMapping(token));
		return;
	}
	if (token.kind == TokenKind::TOKEN_NAME && symbolTable.isWaitingForName)
	{
		std::string type = TryToProcessStruct(symbolTable.currentVariable.second, symbolTable);
		symbolTable.isVariableDeclared = true;
		symbolTable.isWaitingForFunctionDecl = true;
		symbolTable.isWaitingForName = false;
		symbolTable.currentVariable.first = token.name;
		symbolTable.currentVariable.second = type;
		return;
	}
}

void PushVariable(const Token& token, SymbolTable& symbolTable, std::ofstream& out)
{
	auto& back = symbolTable.table.back();
	symbolTable.isVariableDeclared = false;
	symbolTable.isWaitingForName = false;
	if (CheckIfVariableExist(back, symbolTable.currentVariable.first))
	{
		std::cout << "declared: " << symbolTable.currentVariable.first << " " << symbolTable.currentVariable.second;
		throw std::exception("variable already declared");
	}
	back.push_back(symbolTable.currentVariable);
	AddTabsToOut(symbolTable.nestingLevel + 1, out);
	out << "Variable: " + back.back().first + " with type " + back.back().second << std::endl;
}

bool IsNullCurrentVariable(std::pair<std::string, std::string> var) {
	return var.first == "" && var.second == "";
}

void StoreFunctionVariable(FunctionInfo& fnInfo, const std::pair<std::string, std::string> &var) {
	if (fnInfo.parameters.has_value()) {
		fnInfo.parameters.value().push_back(var);
	}
	else {
		std::vector<std::pair<std::string, std::string>> param;
		fnInfo.parameters = std::make_optional(param);
		fnInfo.parameters.value().push_back(var);
	}

}

void ProcessFunction(const Token& token, SymbolTable& symbolTable, std::ofstream& out)
{
	auto& back = symbolTable.table.back();
	if (token.kind == TokenKind::TOKEN_LPAREN && symbolTable.isWaitingForFunctionDecl)
	{
		symbolTable.isWaitingForFunctionDecl = false;
		symbolTable.isWaitingForParametersDecl = true;
		symbolTable.isVariableDeclared = false;
		FunctionInfo fnInfo;
		fnInfo.name = symbolTable.currentVariable.first;
		fnInfo.returnType = symbolTable.currentVariable.second;
		symbolTable.currentVariable = std::make_pair("", "");
		symbolTable.functions.push_back(fnInfo);
	}
	else if (token.kind == TokenKind::TOKEN_COMMA && symbolTable.isWaitingForParametersDecl)
	{
		symbolTable.isVariableDeclared = false;
		StoreFunctionVariable(symbolTable.functions.back(), symbolTable.currentVariable);
	}
	else if (token.kind == TokenKind::TOKEN_RPAREN && symbolTable.isWaitingForParametersDecl)
	{
		symbolTable.isWaitingForParametersDecl = false;
		symbolTable.isVariableDeclared = false;
		if (!IsNullCurrentVariable(symbolTable.currentVariable)) {
			symbolTable.hasFunctionParam = true;
			StoreFunctionVariable(symbolTable.functions.back(), symbolTable.currentVariable);
		}
	}
	else if (token.kind == TokenKind::TOKEN_SEMICOLON && symbolTable.isVariableDeclared)
	{
		PushVariable(token, symbolTable, out);
	}
}

void CreateSymbolTableEntity(const Token& token, SymbolTable& symbolTable, std::ofstream& out)
{
	
	if (symbolTable.table.empty())
	{
		return;
	}

	if (!symbolTable.isWaitingForAssignment && !symbolTable.isWaitingForCondition) {
		ProcessVariable(token, symbolTable, out);
		ProcessFunction(token, symbolTable, out);
	}
}


void PushParametersInBlock(SymbolTable& table, std::ofstream& output)
{
	auto& back = table.table.back();
	auto params = table.functions.back().parameters;
	for (auto& var : params.value())
	{
		back.push_back(var);
		AddTabsToOut(table.nestingLevel + 1, output);
		output << "Function parameter: " + back.back().first + " with type " + back.back().second << std::endl;
	}
}


bool SkipAssignment(const Token& token, SymbolTable& symbolTable, std::ofstream& output)
{
	if (token.kind == TokenKind::TOKEN_ASSIGN || token.kind == TokenKind::TOKEN_RETURN_KEYWORD)
	{
		if (!symbolTable.isVariableDeclared && symbolTable.currentVariable.first == "") {
			symbolTable.currentVariable = std::make_pair("", "");
			symbolTable.isWaitingForName = false;
		}
		symbolTable.isWaitingForAssignment = true;
		return true;
	}
	if (token.kind == TokenKind::TOKEN_SEMICOLON && symbolTable.isWaitingForAssignment)
	{
		symbolTable.isWaitingForAssignment = false;
	}

	return false;
}

bool SkipCondition(const Token& token, SymbolTable& symbolTable, std::ofstream& output)
{
	if (token.kind == TokenKind::TOKEN_WHILE_KEYWORD || token.kind == TokenKind::TOKEN_IF_KEYWORD || token.kind == TokenKind::TOKEN_ELSE_KEYWORD)
	{
		AddTabsToOut(symbolTable.nestingLevel + 1, output);
		if (token.kind == TokenKind::TOKEN_WHILE_KEYWORD)
		{
			output << "While block started:" << std::endl;
		}
		else if (token.kind == TokenKind::TOKEN_IF_KEYWORD)
		{
			output << "If block started:" << std::endl;
		}
		else if (token.kind == TokenKind::TOKEN_ELSE_KEYWORD)
		{
			output << "Else block started:" << std::endl;
		}
		symbolTable.isWaitingForCondition = true;
		return true;
	}
	if (token.kind == TokenKind::TOKEN_LBRACE && symbolTable.isWaitingForCondition)
	{
		symbolTable.isWaitingForCondition = false;
	}

	return false;
}

void ClearStates(SymbolTable& symbolTable)
{
	symbolTable.hasFunctionParam = false;
	symbolTable.isProcessingFor= false;
	symbolTable.isVariableDeclared = false;
	symbolTable.isWaitingForAssignment = false;
	symbolTable.isWaitingForCondition= false;
	symbolTable.isWaitingForFunctionDecl = false;
	symbolTable.isWaitingForName = false;
	symbolTable.isWaitingForParametersDecl = false;
	symbolTable.isWaitingForStructDeclEnd = false;
	symbolTable.hasForParam = false;
}

bool TryToProcessForLoop(const Token& token, SymbolTable& symbolTable, std::ofstream& output)
{

	if (token.kind == TokenKind::TOKEN_FOR_KEYWORD)
	{
		AddTabsToOut(symbolTable.nestingLevel + 1, output);
		output << "For block started: "<< std::endl;
		symbolTable.currentVariable = std::make_pair("", "");
		symbolTable.isProcessingFor = true;
		return true;
	}
	if (token.kind == TokenKind::TOKEN_RPAREN && symbolTable.isProcessingFor && symbolTable.skipFor)
	{
		ClearStates(symbolTable);
		if (!IsNullCurrentVariable(symbolTable.currentVariable)) {
			symbolTable.hasForParam = true;
		}
		symbolTable.skipFor = false;
		//PushVariable(token, symbolTable, output);
		symbolTable.isProcessingFor = false;
		return true;
	}
	if (symbolTable.isProcessingFor && !symbolTable.isWaitingForAssignment && !symbolTable.skipFor)
	{
		if (token.kind == TokenKind::TOKEN_SEMICOLON) {
			symbolTable.skipFor = true;
			return true;
		}
		ProcessVariable(token, symbolTable, output);

		return true;
	}
	if (symbolTable.skipFor) {
		return true;
	}
	return false;
}


bool TryToCreateBlock(const Token& token, SymbolTable& symbolTable, std::ofstream& output)
{
	if (token.kind == TokenKind::TOKEN_LBRACE)
	{
		//output << "\n";
		symbolTable.nestingLevel++;
		CreateBlock(symbolTable);
		AddTabsToOut(symbolTable.nestingLevel, output);
		output << "{\n";
		if (symbolTable.hasFunctionParam) {
			symbolTable.hasFunctionParam = false;
			PushParametersInBlock(symbolTable, output);
		}
		if (symbolTable.hasForParam) {
			symbolTable.hasForParam = false;
			PushVariable(token, symbolTable, output);
		}
		return true;
	}
	return false;
}

bool TryToCloseBlock(const Token& token, SymbolTable& symbolTable, std::ofstream& output)
{
	if (token.kind == TokenKind::TOKEN_RBRACE)
	{
		AddTabsToOut(symbolTable.nestingLevel, output);
		output << "}\n\n";
		symbolTable.nestingLevel--;

		RemoveBlock(symbolTable);
		return true;
	}
	return false;
}

void PushStructVariable(SymbolTable& symbolTable)
{
	//symbolTable.structs.push_back
}

void PrintStructsVariables(const SymbolTable& symbolTable, std::ofstream& output)
{
	StructInfo info = symbolTable.structs.back();
	AddTabsToOut(symbolTable.nestingLevel + 1, output);
	output << "Struct with name: " << info.name << std::endl;
	for (auto& var : info.fields)
	{
		AddTabsToOut(symbolTable.nestingLevel + 2, output);
		output << "Field: " << var.name << " with type " << (var.type.has_value() ? var.type.value() : var.info->name) << std::endl;
	}
	output << std::endl;
}

StructInfo* GetComplexType(SymbolTable& symbolTable)
{
	auto& structs = symbolTable.structs;
	for (auto& structInfo : structs)
	{
		if (structInfo.name == symbolTable.currentVariable.second)
		{
			return &structInfo;
		}
	}
	return nullptr;
}

bool CheckIfStructFieldAlreadyExist(StructInfo& structInfo, FieldInfo& fieldInfo)
{
	for (auto& var: structInfo.fields)
	{
		if (fieldInfo.name == var.name) {
			return true;
		}
	}
	return false;
}

void TryToProcessStructField(SymbolTable& symbolTable)
{
	StructInfo *type = GetComplexType(symbolTable);
	FieldInfo info;
	if (type != nullptr)
	{
		info = FieldInfo(symbolTable.currentVariable.first, type);
	}
	else
	{
		info = FieldInfo(symbolTable.currentVariable.first, symbolTable.currentVariable.second);
	}
	if (CheckIfStructFieldAlreadyExist(symbolTable.structs.back(), info)) {
		std::cout << "Field already delcared: " << info.name << ". In struct: " << symbolTable.structs.back().name;
		throw std::exception("Field already declared");
	}
	symbolTable.structs.back().fields.push_back(info);
	symbolTable.isVariableDeclared = false;

}

bool TryToProcessStruct(const Token& token, SymbolTable& symbolTable, std::ofstream& output) {
	if (token.kind == TokenKind::TOKEN_STRUCT_KEYWORD)
	{
		StructInfo structInfo;
		symbolTable.structs.push_back(structInfo);
		symbolTable.isWaitingForStructDefinition = true;
		return true;
	}
	if (symbolTable.isWaitingForStructDefinition && token.kind == TokenKind::TOKEN_NAME)
	{
		symbolTable.structs.back().name = token.name; //?
		symbolTable.isWaitingForStructDefinition = false;
		symbolTable.isProcessingStruct = true;
		return true;
	}
	if (token.kind == TokenKind::TOKEN_SEMICOLON && symbolTable.isProcessingStruct)
	{
		TryToProcessStructField(symbolTable);
		return true;
	}
	if (token.kind == TokenKind::TOKEN_RBRACE && symbolTable.isProcessingStruct)
	{

		PrintStructsVariables(symbolTable, output);
		symbolTable.isProcessingStruct = false;
		return true;
	}
	if (symbolTable.isProcessingStruct)
	{                          
		ProcessVariable(token, symbolTable, output);
		return true;
	}
	return false;
}

void UpdateSymbolsTable(const Token& token, SymbolTable& symbolTable, std::ofstream & output)
{
	if (SkipAssignment(token, symbolTable, output))
	{
		return;
	}
	if (SkipCondition(token, symbolTable, output))
	{
		return;
	}
	if (TryToProcessStruct(token, symbolTable, output))
	{
		return;
	}
	if (TryToProcessForLoop(token, symbolTable, output))
	{
		return;
	}
	if (TryToCreateBlock(token, symbolTable, output))
	{
		return; 
	}
	if (TryToCloseBlock(token, symbolTable, output))
	{
		return;
	}	
	CreateSymbolTableEntity(token, symbolTable, output);
}


void AnalyzeTable(AnalyzerTable const& table, Lexer& lexer)
{
	std::ofstream output("in\\symbolTable.txt");
	SymbolTable symbolTable;
	InitFirstBlock(symbolTable, output);


	std::stack<AnalyzerRow> stateStack;
	std::stack<std::string> reductionStack;
	std::stack<Token> reductionNext;
	std::vector<std::string> inputSymbols = table[0].symbols;


	stateStack.push(table[0]);
	Token currentToken;
	currentToken = lexer.GetNextToken();
	while (currentToken.kind != TokenKind::TOKEN_EOF || !stateStack.empty())
	{

		std::vector<std::string>::iterator it;
		ptrdiff_t itIndex;
		AnalyzerRow currentState;
		currentState = stateStack.top();
		if (!reductionStack.empty())
		{
			auto nonterminal = reductionStack.top();
			it = std::find(inputSymbols.begin(), inputSymbols.end(), nonterminal);
			itIndex = std::distance(inputSymbols.begin(), it);
			reductionStack.pop();
		}
		else
		{
			auto tokenStr = TOKEN_ADAPTATION.find(currentToken.kind);
			it = std::find(inputSymbols.begin(), inputSymbols.end(), tokenStr->second);
			itIndex = std::distance(inputSymbols.begin(), it);
		}

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
						if (!reductionNext.empty())
						{
							currentToken = reductionNext.top();
							reductionNext.pop();
						}
						else 
						{
							UpdateSymbolsTable(currentToken, symbolTable, output);
							currentToken = lexer.GetNextToken();
						}
					}
					else if (std::is_same_v<T, AnalyzerReduction>)
					{
						auto reduction = std::get<AnalyzerReduction>(tableValue);

						size_t reductionSize = reduction.symbolCount;
						for (size_t i = 0; i < reductionSize; i++)
						{
							stateStack.pop();
						}

						reductionNext.push(currentToken);
						reductionStack.push(reduction.reductionNonterminal);		
					}
					else
					{
						std::cout << "Ok";
					}
				}, tableValue);
		}
		else
		{
			std::cout << "In Line: " << currentToken.loc.lineNum << '\n';
			std::cout << "In Pos: " << currentToken.loc.lineOffset << '\n';
			std::cout << "Error. " << " Found: " << TOKEN_ADAPTATION.find(currentToken.kind)->second << '\n';
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

	CloseGlobalBlock(symbolTable, output);

	if (!stateStack.empty()) 
	{
		std::cout << "Stack is not empty\n";
		return;
	}
	std::cout << "Ok1";
}


