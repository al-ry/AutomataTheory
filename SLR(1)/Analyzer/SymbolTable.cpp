#include "SymbolTable.h"
#include <stdexcept>
#include <iostream>
#include <sstream>
#include <fstream>


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


//should be tested
std::string GetVariableTypeFromSymbolTable(const SymbolTable& symbolTable, const std::string& name)
{
	for (auto it = symbolTable.table.rbegin(); it != symbolTable.table.rend(); ++it)
	{
		auto codeBlock = *it;
		for (auto& variable : codeBlock)
			if (variable.first == name) return variable.second;
	}
	std::string error = "unknown variable: " + name;
	std::cout << error;
	throw std::exception(error.c_str());
}


bool IsTypedef(TokenKind kind) {
	if (kind == TokenKind::TOKEN_BOOL_KEYWORD
		|| kind == TokenKind::TOKEN_FLOAT_KEYWORD
		|| kind == TokenKind::TOKEN_INT_KEYWORD
		|| kind == TokenKind::TOKEN_STR_KEYWORD
		|| kind == TokenKind::TOKEN_CHAR_KEYWORD
		|| kind == TokenKind::TOKEN_NAME
		) {
		return true;
	}
	return false;
}


void CreateBlock(SymbolTable& symbolTable) {
	symbolTable.table.push_back({});
}


void InitFirstBlock(SymbolTable& symbolTable, std::ofstream& out) 
{
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
	const std::string& name) {
	for (auto& var : currentScope)
	{
		if (var.first == name) {
			return true;
		}
	}
	return false;
}

bool IsDefinedStructWithName(const SymbolTable& symbolTable, const std::string& name)
{
	std::vector<StructInfo> structs = symbolTable.structs;
	for (auto& structInfo : structs)
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

void PushVariable(SymbolTable& symbolTable, std::ofstream& out)
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

bool StructAlreadyExist(const std::vector<StructInfo>& structs, const std::string& name)
{
	for (const auto& it : structs)
	{
		if (name == it.name) return true;
	}
	return false;
}

void ProcessVariable(const Token& token, const Token& nextToken, SymbolTable& symbolTable, std::ofstream& out)
{
	auto& back = symbolTable.table.back();
	if (IsTypedef(token.kind) && nextToken.kind == TokenKind::TOKEN_NAME)
	{
		std::string variableType = token.name;
		if (token.kind == TokenKind::TOKEN_NAME)
		{
			if (!StructAlreadyExist(symbolTable.structs, token.name))
			{
				std::cout << "Struct haven't been delcared";
				throw std::exception("Struct haven't been delcared");
			}
		}
		symbolTable.currentVariable = std::make_pair(nextToken.name, GetTokenTypeMapping(token));
		symbolTable.isVariableDeclared = true;
		symbolTable.isWaitingForFunctionDecl = true;
		PushVariable(symbolTable, out);
	}
}


bool IsNullCurrentVariable(std::pair<std::string, std::string> var) {
	return var.first == "" && var.second == "";
}

void StoreFunctionVariable(FunctionInfo& fnInfo, const std::pair<std::string, std::string>& var) {
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
		PushVariable(symbolTable, out);
	}
}

void CreateSymbolTableEntity(const Token& token, const Token& nextToken, SymbolTable& symbolTable, std::ofstream& out)
{
	if (symbolTable.table.empty())
	{
		return;
	}
	ProcessVariable(token, nextToken, symbolTable, out);
	//if (!symbolTable.isWaitingForAssignment && !symbolTable.isWaitingForCondition) {
	//	ProcessVariable(token, nextToken, symbolTable, out);
	//	ProcessFunction(token, symbolTable, out);
	//}
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
			PushVariable(symbolTable, output);
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

StructInfo* GetComplexType(SymbolTable& symbolTable, const std::string& type)
{
	auto& structs = symbolTable.structs;
	for (auto& structInfo : structs)
	{
		if (structInfo.name == type)
		{
			return &structInfo;
		}
	}
	return nullptr;
}

bool CheckIfStructFieldAlreadyExist(StructInfo& structInfo, FieldInfo& fieldInfo)
{
	for (auto& var : structInfo.fields)
	{
		if (fieldInfo.name == var.name) {
			return true;
		}
	}
	return false;
}

void TryToProcessStructField(SymbolTable& symbolTable, const std::vector<std::pair<std::string, std::string>>& lastBlock, StructInfo& lastStruct)
{
	for (auto& var: lastBlock)
	{
		StructInfo* type = GetComplexType(symbolTable, var.second);
		FieldInfo info;
		if (type != nullptr)
		{
			info = FieldInfo(var.first, type);
		}
		else
		{
			info = FieldInfo(var.first, var.second);
		}
		lastStruct.fields.push_back(info);
	}
}

bool TryToProcessStruct(const Token& token, const Token& nextToken, SymbolTable& symbolTable, std::ofstream& output) {
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
		//TryToProcessStructField(symbolTable);
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
		ProcessVariable(token, nextToken, symbolTable, output);
		return true;
	}
	return false;
}



void UpdateSymbolsTable(const std::vector<Token>& prevTokens, const Token& token, const Token& nextToken, SymbolTable& symbolTable, std::ofstream& output)
{
	//if (SkipAssignment(token, symbolTable, output))
	//{
	//	return;
	//}
	//if (SkipCondition(token, symbolTable, output))
	//{
	//	return;
	//}
	//if (TryToProcessStruct(token, nextToken, symbolTable, output))
	//{
	//	return;
	//}
	//if (TryToProcessForLoop(token, nextToken, symbolTable, output))
	//{
	//	return;
	//}
	//if (TryToCreateBlock(token, symbolTable, output))
	//{
	//	return;
	//}
	//if (TryToCloseBlock(token, symbolTable, output))
	//{
	//	return;
	//}
	CreateSymbolTableEntity(token, nextToken, symbolTable, output);

	if (token.kind == TokenKind::TOKEN_LPAREN)
	{
		symbolTable.nestingLevel++;
		CreateBlock(symbolTable);
		AddTabsToOut(symbolTable.nestingLevel, output);
		output << "{\n";
	}
	else if (token.kind == TokenKind::TOKEN_LBRACE && !prevTokens.empty() && prevTokens.back().kind != TokenKind::TOKEN_RPAREN)
	{
		symbolTable.nestingLevel++;
		CreateBlock(symbolTable);
		AddTabsToOut(symbolTable.nestingLevel, output);
		output << "{\n";
	}
	else if (token.kind != TokenKind::TOKEN_LBRACE && !prevTokens.empty() && prevTokens.back().kind == TokenKind::TOKEN_RPAREN)
	{
		AddTabsToOut(symbolTable.nestingLevel, output);
		output << "}\n\n";
		symbolTable.nestingLevel--;
		RemoveBlock(symbolTable);
	}
	else if (token.kind == TokenKind::TOKEN_RBRACE)
	{
		AddTabsToOut(symbolTable.nestingLevel, output);
		output << "}\n\n";
		symbolTable.nestingLevel--;
		if (symbolTable.isProcessingStruct)
		{
			symbolTable.isProcessingStruct = false;
			auto& lastBlock = symbolTable.table.back();
			auto& lastStruct = symbolTable.structs.back();
			TryToProcessStructField(symbolTable, lastBlock, lastStruct);
			PrintStructsVariables(symbolTable, output);

			//lastStruct.fields.
			//FillStructFields();
			//handle fields later
		}
		RemoveBlock(symbolTable);
	}

	if (token.kind == TokenKind::TOKEN_STRUCT_KEYWORD)
	{
		if (!StructAlreadyExist(symbolTable.structs, nextToken.name)) {
			StructInfo info;
			info.name = nextToken.name;
			symbolTable.structs.push_back(info);
			symbolTable.isProcessingStruct = true;
		}
		else
		{
			std::cout << "Struct already delcared";
			throw std::exception("Struct already declared");
		}
	}
}
