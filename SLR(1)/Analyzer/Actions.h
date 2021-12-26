#pragma once
#include "SymbolTable.h"
#include <string>
#include <functional>

struct Node;

class IAction
{
public:
	virtual ~IAction() {};
	virtual std::string DoAction(const std::unique_ptr<Node>& node) = 0;
};

class StructFieldAction : public IAction
{
private:
	std::function<std::string(const SymbolTable& table, const std::unique_ptr<Node>& node)> m_action;
	SymbolTable m_table;
public:
	StructFieldAction(std::function<std::string(const SymbolTable& table, const std::unique_ptr<Node>& node)> action, const SymbolTable& table);
	std::string DoAction(const std::unique_ptr<Node>& node) override;
};

std::string GetStructFieldType(const SymbolTable& table, const std::unique_ptr<Node>& node);