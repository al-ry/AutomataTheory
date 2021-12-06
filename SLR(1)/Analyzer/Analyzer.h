#pragma once
#include "../Generator/Generator.h"
#include "../../Lexer/Lexer/lex.h"


struct AnalyzerReduction 
{
	std::string reductionNonterminal;
	size_t symbolCount;
};

struct AnalyzerShift 
{
	size_t rowIndex;
};

typedef std::variant<AnalyzerReduction, AnalyzerShift, bool> AnalyzerTableValue;

struct AnalyzerRow
{
	std::vector<std::string> symbols;
	std::vector<std::optional<AnalyzerTableValue>> val;
};


struct FunctionInfo {
	std::string name;
	std::string returnType;
	std::optional<std::vector<std::pair<std::string, std::string>>> parameters;
};

typedef std::vector<AnalyzerRow> AnalyzerTable;

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
	FieldInfo(const std::string& name, StructInfo * info) {
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



struct Struct {
	std::string name;
	std::vector<std::pair<std::string, TokenKind>> fields;
};




AnalyzerTable ReadTable(std::istream& input);
void AnalyzeTable(AnalyzerTable const& table, Lexer& lexer);