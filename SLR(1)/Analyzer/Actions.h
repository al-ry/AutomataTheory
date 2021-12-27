#pragma once
#include "SymbolTable.h"
#include <memory>
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
	std::function<std::string(std::shared_ptr<StructInfo> table, const std::unique_ptr<Node>& node)> m_action;
	std::shared_ptr<StructInfo> m_info;
public:
	StructFieldAction(std::function<std::string(std::shared_ptr<StructInfo> info, const std::unique_ptr<Node>& node)> action, std::shared_ptr<StructInfo> info );
	std::string DoAction(const std::unique_ptr<Node>& node) override;
};

std::string GetStructFieldType(const std::shared_ptr<StructInfo>& info, const std::unique_ptr<Node>& node);