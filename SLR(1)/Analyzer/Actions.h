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
	std::function<std::string(const StructInfo& table, const std::unique_ptr<Node>& node)> m_action;
	StructInfo m_info;
public:
	StructFieldAction(std::function<std::string(const StructInfo& info, const std::unique_ptr<Node>& node)> action, const StructInfo& info);
	std::string DoAction(const std::unique_ptr<Node>& node) override;
};

std::string GetStructFieldType(const StructInfo& info, const std::unique_ptr<Node>& node);