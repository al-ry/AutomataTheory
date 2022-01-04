#pragma once
#include "Actions.h"
#include <string>
#include <memory>
#include <vector>
#include <functional>
#include <iostream>
#include <fstream>
#include <sstream>
#include <boost/uuid/uuid.hpp>            // uuid class
#include <boost/uuid/uuid_generators.hpp> // generators
#include <boost/uuid/uuid_io.hpp>         // streaming operators etc.

class ExprAST {
private:
	boost::uuids::uuid uuid;
public:
	ExprAST();
	virtual ~ExprAST() {}
	virtual void writeGraphRepresentation(std::stringstream& ss) = 0;
	virtual std::string getRelationName() = 0;
	virtual std::string getType() = 0;
	std::string getUuidAsString();
};

class VariableExprAST : public ExprAST {
	std::string Name;
	std::string Type;
public:
	VariableExprAST(const std::string& name, const std::string& type);

	void writeGraphRepresentation(std::stringstream& ss) override;
	std::string getRelationName() override;
	std::string getType() override;
};

class ValueExprAST : public ExprAST {
	std::string Val;
	std::string Type;
public:
	ValueExprAST(const std::string& val, const std::string& type);
	void writeGraphRepresentation(std::stringstream& ss) override;
	std::string getRelationName() override;
	std::string getType() override;
};

class BinaryExprAST : public ExprAST {
	std::string Op;
	std::unique_ptr<ExprAST> LHS, RHS;
public:
	BinaryExprAST(const std::string& op, std::unique_ptr<ExprAST> lhs, std::unique_ptr<ExprAST> rhs);

	void writeGraphRepresentation(std::stringstream& ss) override;
	std::string getRelationName() override;
	std::string getType() override;
};

class UnaryExprAST : public ExprAST
{
	std::string Op;
	std::unique_ptr<ExprAST> RHS;
public:
	UnaryExprAST(const std::string& op, std::unique_ptr<ExprAST> rhs);
	void writeGraphRepresentation(std::stringstream& ss) override;
	std::string getRelationName() override;
	std::string getType() override;
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
std::unique_ptr<ExprAST> CreateASTNode(const SyntaxTree& tree, int size, const std::string& nonterminal, int i);
std::string OperationTokenToChar(TokenKind kind);
