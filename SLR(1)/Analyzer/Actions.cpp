#include "Actions.h"
#include "Node.h"

StructFieldAction::StructFieldAction(std::function<std::string(std::shared_ptr<StructInfo> info, const std::unique_ptr<Node>& node)> action, std::shared_ptr<StructInfo> info)
{
    this->m_info = info;
    m_action = action;
}

std::string StructFieldAction::DoAction(const std::unique_ptr<Node>& node)
{
    return m_action(m_info, node);
}


std::string GetStructFieldType(const std::shared_ptr<StructInfo>& info, const std::unique_ptr<Node>& node)
{
    auto currFieldInfo = info->fields;
    for (auto &field : currFieldInfo)
    {
        if (node->nodes.size() == 1 && node->nodes.front()->name == field.name)
        {
            //currNode = std::move(currNode->nodes.front());
            return *field.type;
        }
        else if (node->nodes.size() == 3 && node->nodes.front()->name == field.name)
        {
            //currNode = std::move(currNode->nodes.back());
            if (field.info != nullptr)
            {
                return  GetStructFieldType(field.info, node->nodes.back());
            }
            std::string error = "Field: " + field.name + " in struct: " + info->name + " is primitive type " + *field.type;
            throw std::exception(error.c_str());
        }
    }
}
