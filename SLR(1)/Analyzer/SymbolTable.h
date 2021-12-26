#pragma once
#include <string>
#include <vector>
#include <optional>
#include "../../Lexer/Lexer/lex.h"

struct FunctionInfo {
	std::string name;
	std::string returnType;
	std::optional<std::vector<std::pair<std::string, std::string>>> parameters;
};


struct FieldInfo;

struct StructInfo
{
	std::string name;
	std::vector<FieldInfo> fields;
};

struct FieldInfo {
	FieldInfo(const std::string& name, const std::string& type) {
		this->name = name;
		this->type = type;
		info = nullptr;
	}
	FieldInfo() {

	}
	FieldInfo(const std::string& name, StructInfo* info) {
		this->name = name;
		this->info = info;
		this->type = std::nullopt;
	}
	std::string name;
	StructInfo* info;
	std::optional<std::string> type;
};



struct SymbolTable {

	int nestingLevel = 0;
	std::vector<FunctionInfo> functions;
	std::vector<StructInfo> structs;
	std::pair<std::string, std::string> currentVariable;
	std::vector<std::vector<std::pair<std::string, std::string>>> table;
	bool isVariableDeclared = false;
	bool isWaitingForParametersDecl = false;
	bool isWaitingForName = false;
	bool isWaitingForAssignment = false;
	bool isWaitingForFunctionDecl = false;
	bool isWaitingForStructDeclEnd = false;
	bool hasFunctionParam = false;
	bool isWaitingForCondition = false;
	bool isProcessingFor = false;
	bool skipFor = false;
	bool hasForParam = false;
	bool isProcessingStruct = false;
	bool isWaitingForStructDefinition = false;
};

bool IsPrimitiveType(const std::string& name);
bool StructAlreadyExist(const std::vector<StructInfo>& structs, const std::string& name);
void UpdateSymbolsTable(const std::vector<Token>& prevTokens,const Token& token, const Token& nextToken, SymbolTable& symbolTable, std::ofstream& output);
void CloseGlobalBlock(SymbolTable& symbolTable, std::ofstream& out);
void InitFirstBlock(SymbolTable& symbolTable, std::ofstream& out);
std::optional<std::string> GetVariableTypeFromSymbolTable(const SymbolTable& symbolTable, const std::string& name);