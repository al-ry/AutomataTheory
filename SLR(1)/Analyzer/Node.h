#pragma once
#include "SymbolTable.h"
#include <string>
#include <memory>
#include <vector>

struct Node
{
	std::string type;
	std::string name;
	TokenKind kind;
	LexemeLocation loc;
	std::vector<std::unique_ptr<Node>> nodes;
};


typedef std::vector<std::unique_ptr<Node>> ASTTree;

void CreateNewNode(ASTTree& node, const SymbolTable& table, const Token& token);
void CheckTypes(ASTTree& tree);
