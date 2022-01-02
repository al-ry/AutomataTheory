#pragma once
#include "Actions.h"
#include <string>
#include <memory>
#include <vector>
#include <functional>
#include <iostream>
#include <fstream>
#include <sstream>


class ExprAST {
public:
	virtual ~ExprAST() {}
	virtual void writeGraphRepresentation(std::stringstream& ss) = 0;
	virtual std::string getRelationName() = 0;

};

class VariableExprAST : public ExprAST {
	std::string Name;
public:
	VariableExprAST(const std::string& name);

	void writeGraphRepresentation(std::stringstream& ss);
	std::string getRelationName();
};

class NumberExprAST : public ExprAST {
	double Val;
public:
	NumberExprAST(double val);

	void writeGraphRepresentation(std::stringstream& ss);
	std::string getRelationName();
};

class BinaryExprAST : public ExprAST {
	char Op;
	std::unique_ptr<ExprAST> LHS, RHS;
public:
	BinaryExprAST(char op, std::unique_ptr<ExprAST> lhs, std::unique_ptr<ExprAST> rhs);

	void writeGraphRepresentation(std::stringstream& ss);
	std::string getRelationName();
};

typedef std::vector<std::unique_ptr<ExprAST>> AST;


struct Node
{
	std::unique_ptr<IAction> action;
	std::string type;
	std::string name;
	TokenKind kind;
	LexemeLocation loc;
	std::vector<std::unique_ptr<Node>> nodes;
	std::unique_ptr<ExprAST> attribute;
};


typedef std::vector<std::unique_ptr<Node>> SyntaxTree;

void CreateNewNode(SyntaxTree& node, const SymbolTable& table, const Token& token);
void CheckTypes(SyntaxTree& tree);
std::shared_ptr<StructInfo> GetStructInfoWithName(const SymbolTable& symbolTable, const std::string& name);

char OperationTokenToChar(TokenKind kind);
