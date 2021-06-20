#pragma once
#include "../../Lexer/Lexer/lex.h"
#include <map>


const std::map<TokenKind, std::string> TOKEN_ADAPTATION = {
	{TokenKing::TOKEN_EOF, "EOF"},//
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
	{TokenKind::TOKEN_INT, "Int"},
	{TokenKind::TOKEN_FLOAT, "Float"},
	{TokenKind::TOKEN_STR, "Str"},
	{TokenKind::TOKEN_NAME, "name"},//
	{TokenKind::TOKEN_CHAR, "char"},//
	{TokenKind::TOKEN_NOT, "Not"},//
	{TokenKind::TOKEN_MUL, "Mul"},//
	{TokenKind::TOKEN_DIV, "Div"},//
	{TokenKind::TOKEN_MOD, "Mod"},//
	{TokenKind::TOKEN_AND, "&"},//
	{TokenKind::TOKEN_ADD, "Plus"},//
	{TokenKind::TOKEN_SUB, "Minus"},//
	{TokenKind::TOKEN_OR, "|"},//
	{TokenKind::TOKEN_OR_OR, "OR"},//
	{TokenKind::TOKEN_INC, "++"},//
	{TokenKind::TOKEN_DEC, "--"},//
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

