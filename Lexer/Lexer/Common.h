#pragma once
#include "../../Lexer/Lexer/lex.h"
#include <map>


const std::map<TokenKind, std::string> TOKEN_ADAPTATION = {
	{TokenKing::TOKEN_EOF, "$"},//
	{TokenKing::TOKEN_COLON, ":"},//
	{TokenKing::TOKEN_LPAREN, "("},//
	{TokenKing::TOKEN_RPAREN, ")"},//
	{TokenKing::TOKEN_LBRACE, "{"},//
	{TokenKing::TOKEN_RBRACE, "}"},//
	{TokenKing::TOKEN_LBRACKET, "["},//
	{TokenKing::TOKEN_RBRACKET, "]"},//
	{TokenKing::TOKEN_COMMA, ","},//
	{TokenKing::TOKEN_DOT, "."},//
	{TokenKing::TOKEN_POUND, "#"},//
	{TokenKing::TOKEN_QUESTION, "?"},//
	{TokenKing::TOKEN_SEMICOLON, ";"},//
	{TokenKind::TOKEN_KEYWORD, "keyword"},//
	{TokenKind::TOKEN_ENUM_KEYWORD, "enum"},
	{TokenKind::TOKEN_INT_KEYWORD, "IntKeyword" },
	{TokenKind::TOKEN_FLOAT_KEYWORD, "FloatKeyword"},
	{TokenKind::TOKEN_STR_KEYWORD, "StrKeyword"},
	{TokenKind::TOKEN_STRUCT_KEYWORD, "struct"},
	{TokenKind::TOKEN_FUNC_KEYWORD,  "func"},
	{TokenKind::TOKEN_BREAK_KEYWORD,  "break"},
	{TokenKind::TOKEN_WHILE_KEYWORD, "while"},
	{TokenKind::TOKEN_SWITCH_KEYWORD, "switch"},
	{TokenKind::TOKEN_CASE_KEYWORD, "case"},
	{TokenKind::TOKEN_DO_KEYWORD, "do", },
	{TokenKind::TOKEN_IF_KEYWORD, "if" },
	{TokenKind::TOKEN_ELSE_KEYWORD,  "else"},
	{TokenKing::TOKEN_DEFAULT_KEYWORD, "default"},
	{TokenKind::TOKEN_RETURN_KEYWORD,  "return"},
	{TokenKind::TOKEN_VAR_KEYWORD, "var"},
	{TokenKind::TOKEN_FOR_KEYWORD, "for"},
	{TokenKind::TOKEN_CONTINUE_KEYWORD, "continue"},
	{TokenKind::TOKEN_CONST_KEYWORD, "const"},
	{TokenKind::TOKEN_SIZEOF_KEYWORD, "sizeof"},
	{TokenKind::TOKEN_TRUE_KEYWORD, "true", },
	{TokenKind::TOKEN_FALSE_KEYWORD, "false"},
	{TokenKind::TOKEN_BOOL_KEYWORD, "BoolKeyword"},
	{TokenKind::TOKEN_CHAR_KEYWORD, "CharKeyword"},
	{TokenKind::TOKEN_INT, "Int"},
	{TokenKind::TOKEN_FLOAT, "Float"},
	{TokenKind::TOKEN_STR, "Str"},
	{TokenKind::TOKEN_NAME, "name"},//
	{TokenKind::TOKEN_CHAR, "Char"},//
	{TokenKind::TOKEN_NOT, "Not"},//
	{TokenKind::TOKEN_MUL, "Mul"},//
	{TokenKind::TOKEN_DIV, "Div"},//
	{TokenKind::TOKEN_MOD, "Mod"},//
	{TokenKind::TOKEN_AND, "&"},//
	{TokenKind::TOKEN_ADD, "Plus"},//
	{TokenKind::TOKEN_SUB, "Minus"},//
	{TokenKind::TOKEN_OR, "|"},//
	{TokenKind::TOKEN_OR_OR, "OR"},//
	{TokenKind::TOKEN_INC, "INC"},//
	{TokenKind::TOKEN_DEC, "DEC"},//
	{TokenKind::TOKEN_AND_AND, "AND"},//
	{TokenKind::TOKEN_NOTEQ, "NotEq"},//
	{TokenKind::TOKEN_EQ, "Eq"},//
	{TokenKind::TOKEN_GT, "GT"},
	{TokenKind::TOKEN_LT, "LT"},
	{TokenKind::TOKEN_GTEQ, "GTEq"},
	{TokenKind::TOKEN_LTEQ, "LTEq"},
	{TokenKind::TOKEN_ASSIGN, "="},//
	{TokenKind::TOKEN_ASSIGN_DIV, "/="},//
	{TokenKind::TOKEN_ASSIGN_MOD, "%="},//
	{TokenKind::TOKEN_ASSIGN_ADD, "+="},//
	{TokenKind::TOKEN_ASSIGN_SUB, "-="},//
	{TokenKind::TOKEN_ASSIGN_MUL, "*="},//
	{TokenKind::TOKEN_ERROR, "error"}//
};

const std::map<TokenKind, std::string> TOKEN_ADAPTATION2 = {
	{TokenKing::TOKEN_EOF, "$"},//
	{TokenKing::TOKEN_COLON, ":"},//
	{TokenKing::TOKEN_LPAREN, "("},//
	{TokenKing::TOKEN_RPAREN, ")"},//
	{TokenKing::TOKEN_LBRACE, "{"},//
	{TokenKing::TOKEN_RBRACE, "}"},//
	{TokenKing::TOKEN_LBRACKET, "["},//
	{TokenKing::TOKEN_RBRACKET, "]"},//
	{TokenKing::TOKEN_COMMA, ","},//
	{TokenKing::TOKEN_DOT, "."},//
	{TokenKing::TOKEN_POUND, "#"},//
	{TokenKing::TOKEN_QUESTION, "?"},//
	{TokenKing::TOKEN_SEMICOLON, ";"},//
	{TokenKind::TOKEN_KEYWORD, "keyword"},//
	{TokenKind::TOKEN_ENUM_KEYWORD, "enum"},
	{TokenKind::TOKEN_INT_KEYWORD, "IntKeyword" },
	{TokenKind::TOKEN_FLOAT_KEYWORD, "FloatKeyword"},
	{TokenKind::TOKEN_STR_KEYWORD, "StrKeyword"},
	{TokenKind::TOKEN_STRUCT_KEYWORD, "struct"},
	{TokenKind::TOKEN_FUNC_KEYWORD,  "func"},
	{TokenKind::TOKEN_BREAK_KEYWORD,  "break"},
	{TokenKind::TOKEN_WHILE_KEYWORD, "while"},
	{TokenKind::TOKEN_SWITCH_KEYWORD, "switch"},
	{TokenKind::TOKEN_CASE_KEYWORD, "case"},
	{TokenKind::TOKEN_DO_KEYWORD, "do", },
	{TokenKind::TOKEN_IF_KEYWORD, "if" },
	{TokenKind::TOKEN_ELSE_KEYWORD,  "else"},
	{TokenKing::TOKEN_DEFAULT_KEYWORD, "default"},
	{TokenKind::TOKEN_RETURN_KEYWORD,  "return"},
	{TokenKind::TOKEN_VAR_KEYWORD, "var"},
	{TokenKind::TOKEN_FOR_KEYWORD, "for"},
	{TokenKind::TOKEN_CONTINUE_KEYWORD, "continue"},
	{TokenKind::TOKEN_CONST_KEYWORD, "const"},
	{TokenKind::TOKEN_SIZEOF_KEYWORD, "sizeof"},
	{TokenKind::TOKEN_TRUE_KEYWORD, "true", },
	{TokenKind::TOKEN_FALSE_KEYWORD, "false"},
	{TokenKind::TOKEN_BOOL_KEYWORD, "BoolKeyword"},
	{TokenKind::TOKEN_CHAR_KEYWORD, "CharKeyword"},
	{TokenKind::TOKEN_INT, "Int"},
	{TokenKind::TOKEN_FLOAT, "Float"},
	{TokenKind::TOKEN_STR, "Str"},
	{TokenKind::TOKEN_NAME, "name"},//
	{TokenKind::TOKEN_CHAR, "Char"},//
	{TokenKind::TOKEN_NOT, "Not"},//
	{TokenKind::TOKEN_MUL, "Mul"},//
	{TokenKind::TOKEN_DIV, "Div"},//
	{TokenKind::TOKEN_MOD, "Mod"},//
	{TokenKind::TOKEN_AND, "&"},//
	{TokenKind::TOKEN_ADD, "Plus"},//
	{TokenKind::TOKEN_SUB, "Minus"},//
	{TokenKind::TOKEN_OR, "|"},//
	{TokenKind::TOKEN_OR_OR, "OR"},//
	{TokenKind::TOKEN_INC, "INC"},//
	{TokenKind::TOKEN_DEC, "DEC"},//
	{TokenKind::TOKEN_AND_AND, "AND"},//
	{TokenKind::TOKEN_NOTEQ, "NotEq"},//
	{TokenKind::TOKEN_EQ, "Eq"},//
	{TokenKind::TOKEN_GT, "GT"},
	{TokenKind::TOKEN_LT, "LT"},
	{TokenKind::TOKEN_GTEQ, "GTEq"},
	{TokenKind::TOKEN_LTEQ, "LTEq"},
	{TokenKind::TOKEN_ASSIGN, "="},//
	{TokenKind::TOKEN_ASSIGN_DIV, "/="},//
	{TokenKind::TOKEN_ASSIGN_MOD, "%="},//
	{TokenKind::TOKEN_ASSIGN_ADD, "+="},//
	{TokenKind::TOKEN_ASSIGN_SUB, "-="},//
	{TokenKind::TOKEN_ASSIGN_MUL, "*="},//
	{TokenKind::TOKEN_ERROR, "error"}//
};

//enum Errors {
//	VARIABLE_DECLARED,
//	FIELD_DECLARED,
//	STRUCT_DELCARED,
//	FUNCTION_DECLARED,
//	UNDEFINED_VARIABLE,
//	UNDEFINED_STRUCT
//};
//
//
//const std::map<Errors, std::string> ERROR_MAP =
//{
//	{VARIABLE_DECLARED, "Variable already declared"},
//	{FIELD_DECLARED, "Field declared"},
//	{STRUCT_DELCARED, "Struct alredy declared"},
//	{FUNCTION_DECLARED, "Function alredy delcared"},
//	{UNDEFINED_VARIABLE, "Undefined variable"},
//	{UNDEFINED_STRUCT, "Undefined struct"},
//}


