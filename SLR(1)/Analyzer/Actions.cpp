#include "Actions.h"

StructFieldAction::StructFieldAction(std::function<std::string(const SymbolTable& table, const std::unique_ptr<Node>& node)> action, const SymbolTable& table)
{
    m_table = table;
    m_action = action;
}

std::string StructFieldAction::DoAction(const std::unique_ptr<Node>& node)
{
    return m_action(m_table, node);
}

std::string GetStructFieldType(const SymbolTable& table, const std::unique_ptr<Node>& node)
{
    return "int";
}
