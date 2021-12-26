#include "Node.h"
#include "SymbolTable.h"
#include "../../Lexer/Lexer/Common.h"
#include "../Generator/Generator.h"
#include "Actions.h"

bool IsLiteral(TokenKind kind)
{
	return kind == TokenKind::TOKEN_INT
		|| kind == TokenKind::TOKEN_FLOAT
		|| kind == TokenKind::TOKEN_STR
		|| kind == TokenKind::TOKEN_CHAR
		|| kind == TokenKind::TOKEN_AND
		|| kind == TokenKind::TOKEN_OR
		|| kind == TokenKind::TOKEN_NOT
		|| kind == TokenKind::TOKEN_SUB;
}


void CreateNewNode(ASTTree& tree, const SymbolTable& table, const Token& token)
{
	auto node = std::make_unique<Node>();
	node->name = token.name;
	node->kind = token.kind;
	node->loc = token.loc;
	if (token.kind == TokenKind::TOKEN_NAME && !StructAlreadyExist(table.structs, token.name))
	{ 
		auto result = GetVariableTypeFromSymbolTable(table, token.name);
		if (result.has_value())
		{
			node->type = result.value();
			if (!IsPrimitiveType(node->type))
			{
				node->action = std::make_unique<StructFieldAction>(GetStructFieldType, table);
			}
		}
	}
	else if (IsLiteral(token.kind))
	{
		node->name = TOKEN_ADAPTATION.find(token.kind)->second;
	}
	tree.push_back(std::move(node));
}

std::string GetType(std::string const& type1, std::string const& type2)
{
	if (type1 == type2)
	{
		return type1;
	}
	if (type1 == "float" && type2 == "int")
	{
		return type1;
	}
	if (type2 == "float" && type1 == "int")
	{
		return type2;
	}
	if ((type1 == "char" && type2 == "string") || (type1 == "string" && type2 == "char"))
	{
		return "string";
	}
	if (type1 == "char" && type2 == "int")
	{
		return "char";
	}
	if (type1 == "int" && type2 == "char")
	{
		return "int";
	}
	throw std::exception("types are not compatible");
}

std::string GetType(const std::unique_ptr<Node>& node);


std::string ProcessAssign(const std::unique_ptr<Node>& node)
{
	if (node->nodes.size() == 4)
	{
		return GetType(GetType(node->nodes.at(1)), GetType(node->nodes.back()));
	}
	else if (node->nodes.size() == 3)
	{
		return GetType(GetType(node->nodes.at(0)), GetType(node->nodes.back()));
	}
	return "";
}

std::string ProcessCondition(const std::unique_ptr<Node>& node)
{
	if (node->nodes.size() == 1)
	{
		return GetType(node->nodes.at(0));
	}
	if (node->nodes.size() == 3)
	{
		node->type =  GetType(GetType(node->nodes.front()), GetType(node->nodes.back()));
		std::string name = node->nodes.at(1)->name;
		if (name == "OR" || name == "AND" || name == "<CmpOp>")
		{
			return "bool";
		}
		return node->type;
	}

}

std::string ProcessOperation(const std::unique_ptr<Node>& node)
{
	return std::string();
}


bool IsBoolOperation(const std::string& op)
{
	return op == "<AndExpr>" || op == "<OrExpr>" || op == "<CmpExpr>";
}


std::string ValueToType(const std::string& value)
{
	if (value == "Int")
	{
		return "int";
	}
	else if (value == "Float")
	{
		return "float";
	}
	else if (value == "Str")
	{
		return "string";
	}
	else if (value == "Char")
	{
		return "char";
	}
	else if (value == "true" || value == "false")
	{
		return "bool";
	}
	throw std::exception("unexistent type");
}

std::string ProcessNotCondition(const std::unique_ptr<Node>& node)
{
	return GetType("bool", GetType(node->nodes.back()));
}
std::string ProcessUnaryCondition(const std::unique_ptr<Node>& node)
{
	std::string name = GetType(node->nodes.back());
	if (name == "float")
	{
		return  "float";
	}
	else if (name == "int")
	{
		return "int";
	}
	std::string error =
		"It's not an unary expression. Type for unary minus should be either float or int";
	throw std::exception(error.c_str());
}

std::string GetType(const std::unique_ptr<Node>& node)
{
	if (!node->type.empty())
	{
		return node->type;
	}
	if (node->name == "<Assign>")
	{
		node->type = ProcessAssign(node);
	}
	else if (node->name == "<Condition>")
	{
		node->type = ProcessCondition(node);
	}
	else if (IsBoolOperation(node->name))
	{
		node->type = ProcessCondition(node);
	}
	else if (node->name == "<E>" || node->name == "<T>")
	{
		if (node->nodes.size() == 3)
		{
			node->type = ProcessCondition(node);
			if (node->type == "bool" || node->type == "string")
			{
				std::string error = "Not compatible operation for type "
					+ node->type;
				throw std::exception(error.c_str());
			}
		}
		else
		{
			node->type = ProcessCondition(node);
		}
	}
	else if (node->name == "<F>")
	{
		if (node->name == "<F>" && node->nodes.size() == 2)
		{
			if (node->nodes.front()->name == "Not")
			{
				node->type = ProcessNotCondition(node->nodes.back());
			}
			else if (node->nodes.front()->name == "Minus")
			{
				node->type = ProcessUnaryCondition(node->nodes.back());
			}			
		}
		else
		{
			node->type = ProcessCondition(node);
		}
	}
	else if (node->name == "<Value>")
	{
		node->type = ValueToType(node->nodes.front()->name);
	}
	else if (node->name == "<TypeFieldCall>")
	{
		if (node->nodes.size() == 1)
		{
			node->type = node->nodes.front()->type;
		}
		else if (node->nodes.size() == 3)
		{
			node->type = node->nodes.front()->action->DoAction(node->nodes.back());
		}
	}
	return node->type;
}



void CheckTypes(ASTTree& tree)
{
	for (auto& node: tree)
	{
		if (node->name == "<Assign>")
		{
			GetType(node);
		}
		else if (node->name == "<If>")
		{
			GetType("bool", GetType(node->nodes.at(2)));
		}
		else
		if (node->name == "<Loop>")
		{
			if (node->nodes.front()->name == "<While>")
			{
				GetType("bool", GetType(node->nodes.front()->nodes.at(2)));
			}
			else
			if (node->nodes.front()->name == "<For>" && node->nodes.front()->nodes.size() > 8)
			{
				GetType(node->nodes.front()->nodes.at(2));
				GetType(node->nodes.front()->nodes.at(4));
				GetType(node->nodes.front()->nodes.at(6));
			}
		}

		CheckTypes(node->nodes);
	}
}
