#include "Node.h"
#include "SymbolTable.h"
#include "../../Lexer/Lexer/Common.h"
#include "../Generator/Generator.h"
#include "Actions.h"
#include <fstream>
#include <sstream>


std::string MakeNode(const std::string& label1, const std::string& label2)
{
	return label1 + " ["
		+ "shape=\"" + "circle" + "\""
		+ "label=\"" + label2 + "\"];\n";
}

std::string MakeRelation(const std::string& from, const std::string& to, const std::string& label)
{
	return  from + "->" + to + " [label=\"" + " \"];\n";
}

void ValueExprAST::writeGraphRepresentation(std::stringstream& ss) {
	std::string node = MakeNode(getUuidAsString(), Val);
	ss << node;
}

std::string ValueExprAST::getRelationName()
{
	return Val;
}

std::string getOperatorString(const std::string& op) {
	if (op == "=")
	{
		return "<&#61;>";
	}
	if (op == "*")
	{
		return "<&#1645;>";
	}
	if (op == "+") 
	{
		return "<&#43;>";		
	}
	if (op == "-")
	{
		return "<&#727;>";
	}
	if (op == "/")
	{
		return "<&#8260;>";
	}
	if (op == "%")
	{
		return "<&#37;>";
	}
	if (op == "!")
	{
		return "<&#33;>";
	}
	if (op == "||")
	{
		return "<&#124;&#124;>";
	}
	if (op == "&&")
	{
		return "<&#38;&#38;>";
	}
	if (op == "==")
	{
		return "<&#61;&#61;>";
	}
	if (op == "!=")
	{
		return "<&#33;&#61;>";
	}
	if (op == "<")
	{
		return "<&#60;>";
	}
	if (op == "<=")
	{
		return "<&#60;&#61;>";
	}
	if (op == ">")
	{
		return "<&#62;>";
	}
	if (op == ">=")
	{
		return "<&#62;&#61;>";
	}
}

void BinaryExprAST::writeGraphRepresentation(std::stringstream& ss) {
	LHS->writeGraphRepresentation(ss);
	RHS->writeGraphRepresentation(ss);

	std::string leftRelation = LHS->getUuidAsString();
	std::string rightRelation = RHS->getUuidAsString();

	ss << MakeNode(getUuidAsString(), getRelationName());

	ss << MakeRelation(getUuidAsString(), leftRelation, "1");
	ss << MakeRelation(getUuidAsString(), rightRelation, "1");
}

std::string BinaryExprAST::getRelationName()
{
	return getOperatorString(Op);
}


void VariableExprAST::writeGraphRepresentation(std::stringstream& ss) {
	std::string node = MakeNode(getUuidAsString(), Name);
	ss << node;
}

std::string VariableExprAST::getRelationName()
{
	return Name;
}

bool IsLiteral(TokenKind kind)
{
	return kind == TokenKind::TOKEN_INT
		|| kind == TokenKind::TOKEN_FLOAT
		|| kind == TokenKind::TOKEN_STR
		|| kind == TokenKind::TOKEN_CHAR
		|| kind == TokenKind::TOKEN_AND
		|| kind == TokenKind::TOKEN_OR
		|| kind == TokenKind::TOKEN_NOT
		|| kind == TokenKind::TOKEN_SUB
		|| kind == TokenKind::TOKEN_TRUE_KEYWORD
		|| kind == TokenKind::TOKEN_FALSE_KEYWORD
		|| kind == TokenKind::TOKEN_LT
		|| kind == TokenKind::TOKEN_LTEQ
		|| kind == TokenKind::TOKEN_GTEQ
		|| kind == TokenKind::TOKEN_GT
		|| kind == TokenKind::TOKEN_NOTEQ
		|| kind == TokenKind::TOKEN_EQ;
}



std::shared_ptr<StructInfo> GetStructInfoWithName(const SymbolTable& symbolTable, const std::string& name)
{
	std::vector<std::shared_ptr<StructInfo>> structs = symbolTable.structs;
	std::shared_ptr<StructInfo> info;
	for (auto& structInfo : structs)
	{
		if (structInfo->name == name)
		{
			return structInfo;
		}
	}
	return info;
}

std::string OperationTokenToChar(TokenKind kind)
{
	if (kind == TokenKind::TOKEN_ADD)
	{
		return "+";
	}
	else if (kind == TokenKind::TOKEN_NOT)
	{
		return "!";
	}
	else if (kind == TokenKind::TOKEN_SUB)
	{
		return "-";
	}
	else if (kind == TokenKind::TOKEN_MUL)
	{
		return "*";
	}
	else if (kind == TokenKind::TOKEN_MOD)
	{
		return "%";
	}
	else if (kind == TokenKind::TOKEN_DIV)
	{
		return "/";
	}
	else if (kind == TokenKind::TOKEN_ASSIGN)
	{
		return "=";
	}
	else if (kind == TokenKind::TOKEN_OR_OR)
	{
		return "||";
	}
	else if (kind == TokenKind::TOKEN_AND_AND)
	{
		return "&&";
	}
	else if (kind == TokenKind::TOKEN_LT)
		return "<";
	else if (kind == TokenKind::TOKEN_LTEQ)
		return "<=";
	else if (kind == TokenKind::TOKEN_GTEQ)
		return ">=";
	else if (kind == TokenKind::TOKEN_GT)
		return ">";
	else if (kind == TokenKind::TOKEN_NOTEQ)
		return "!=";
	else if (kind == TokenKind::TOKEN_EQ)
		return "==";
}


void CreateNewNode(SyntaxTree& tree, const SymbolTable& table, const Token& token)
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
				node->action = std::make_unique<StructFieldAction>(GetStructFieldType, GetStructInfoWithName(table, node->type));
			}
			node->attribute = std::move(std::make_unique<VariableExprAST>(node->name));
		}
	}
	else if (IsLiteral(token.kind))
	{
		node->name = TOKEN_ADAPTATION.find(token.kind)->second;
		if (token.kind == TokenKind::TOKEN_INT)
		{
			node->attribute = std::move(std::make_unique<ValueExprAST>(std::to_string(token.int_number)));
		}
		else if (token.kind == TokenKind::TOKEN_FLOAT)
		{
			node->attribute = std::move(std::make_unique<ValueExprAST>(std::to_string(token.real_number)));
		}
		else if (token.kind == TokenKind::TOKEN_STR)
		{
			node->attribute = std::move(std::make_unique<ValueExprAST>(token.string));
		}
		else if (token.kind == TokenKind::TOKEN_CHAR)
		{
			node->attribute = std::move(std::make_unique<ValueExprAST>((std::to_string(token.int_number))));
		}
		else if (token.kind == TokenKind::TOKEN_TRUE_KEYWORD || token.kind == TokenKind::TOKEN_FALSE_KEYWORD)
		{
			node->attribute = std::move(std::make_unique<ValueExprAST>((token.name)));
		}
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

std::string ProcessOperationInBrackets(const std::unique_ptr<Node>& node)
{
	if (node->nodes.size() == 1)
	{
		return GetType(node->nodes.at(0));
	}
	if (node->nodes.size() == 3)
	{
		return GetType(node->nodes.at(1));
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
	std::string type;
	if (node->nodes.size() == 1)
	{
		type = GetType(node->nodes.back());
	}
	else if (node->nodes.size() == 3)
	{
		type = GetType(node->nodes.at(1));
	}
	if (type == "float")
	{
		return  "float";
	}
	else if (type == "int")
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
			node->type = ProcessOperationInBrackets(node);
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



void CheckTypes(SyntaxTree& tree)
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

ExprAST::ExprAST()
{
	boost::uuids::random_generator generator;
	uuid = generator();
}

VariableExprAST::VariableExprAST(const std::string& name) : Name(name) {}

ValueExprAST::ValueExprAST(std::string val) : Val(val) {}

BinaryExprAST::BinaryExprAST(const std::string& op, std::unique_ptr<ExprAST> lhs, std::unique_ptr<ExprAST> rhs) : Op(op), LHS(std::move(lhs)), RHS(std::move(rhs)) {}

std::string ExprAST::getUuidAsString()
{
	return  "<" + boost::uuids::to_string(uuid) + ">";
}

std::unique_ptr<ExprAST> CreateASTNode(const SyntaxTree& tree, int size, const std::string &nonterminal, int i)
{
	if (nonterminal == "<Assign>" && i == 0 && size != 2)
	{
		return
			std::make_unique<BinaryExprAST>(
				OperationTokenToChar(tree.at(tree.size() - 2)->kind),
				std::move(tree.at(tree.size() - 3)->attribute),
				std::move(tree.back()->attribute));
		//astTree.push_back(std::move(node->attribute));
	}
	else
	if ((nonterminal == "<E>" || nonterminal == "<T>") && size == 3 && i == 0)
	{
		return
			std::make_unique<BinaryExprAST>(
				OperationTokenToChar(tree.at(tree.size() - 2)->kind),
				std::move(tree.at(tree.size() - 3)->attribute),
				std::move(tree.back()->attribute));
	}
	else if (i == 0)
	{
		return std::move(tree.back()->attribute);
	}
	return nullptr;
}

UnaryExprAST::UnaryExprAST(const std::string& op, std::unique_ptr<ExprAST> rhs): Op(op), RHS(std::move(rhs))
{
}

void UnaryExprAST::writeGraphRepresentation(std::stringstream& ss)
{
	RHS->writeGraphRepresentation(ss);

	std::string rightRelation = RHS->getUuidAsString();

	ss << MakeNode(getUuidAsString(), getRelationName());

	ss << MakeRelation(getUuidAsString(), rightRelation, "1");
}

std::string UnaryExprAST::getRelationName()
{
	return getOperatorString(Op);
}
