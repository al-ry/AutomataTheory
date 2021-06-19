#pragma once
#include "../../Lexer/Lexer/lex.h"
#include <map>


const std::map<std::string, TokenKind> TOKEN_ADAPTATION = {
	{ "EOF", TokenKing::TOKEN_EOF},//
	{ ":", TokenKing::TOKEN_COLON},//
	{ "(", TokenKing::TOKEN_LPAREN},//
	{ ")", TokenKing::TOKEN_RPAREN},//
	{ "{", TokenKing::TOKEN_LBRACE},//
	{ "}", TokenKing::TOKEN_RBRACE},//
	{ "[", TokenKing::TOKEN_LBRACKET},//
	{ "]", TokenKing::TOKEN_RBRACKET},//
	{ ",", TokenKing::TOKEN_COMMA},//
	{ ".", TokenKing::TOKEN_DOT},//
	{ "#", TokenKing::TOKEN_POUND},//
	{ "?", TokenKing::TOKEN_QUESTION},//
	{  ";", TokenKing::TOKEN_SEMICOLON},//
	{ "keyword", TokenKind::TOKEN_KEYWORD},//
	{ "enum", TokenKind::TOKEN_ENUM_KEYWORD},
	{ "Int", TokenKind::TOKEN_INT},
	{ "Float", TokenKind::TOKEN_FLOAT},
	{ "Str", TokenKind::TOKEN_STR},
	{ "name", TokenKind::TOKEN_NAME},//
	{ "char", TokenKind::TOKEN_CHAR },//
	{ "Not", TokenKind::TOKEN_NOT},//
	{ "Mul", TokenKind::TOKEN_MUL},//
	{ "Div", TokenKind::TOKEN_DIV},//
	{ "Mod", TokenKind::TOKEN_MOD},//
	{  "&", TokenKind::TOKEN_AND},//
	{"Plus", TokenKind::TOKEN_ADD},//
	{"Minus", TokenKind::TOKEN_SUB, },//
	{"|", TokenKind::TOKEN_OR},//
	{"OR", TokenKind::TOKEN_OR_OR},//
	{"++", TokenKind::TOKEN_INC},//
	{"--", TokenKind::TOKEN_DEC},//
	{"AND", TokenKind::TOKEN_AND_AND},//
	{"NotEq", TokenKind::TOKEN_NOTEQ},//
	{"Eq", TokenKind::TOKEN_EQ},//
	{"GT", TokenKind::TOKEN_GT},
	{"LT", TokenKind::TOKEN_LT},
	{"GTEq", TokenKind::TOKEN_GTEQ},
	{"LTEq", TokenKind::TOKEN_LTEQ},
	{"=", TokenKind::TOKEN_ASSIGN},//
	{"/=", TokenKind::TOKEN_ASSIGN_DIV},//
	{"%=", TokenKind::TOKEN_ASSIGN_MOD},//
	{"+=", TokenKind::TOKEN_ASSIGN_ADD},//
	{"-=", TokenKind::TOKEN_ASSIGN_SUB},//
	{"*=", TokenKind::TOKEN_ASSIGN_MUL},//
	{"error", TokenKind::TOKEN_ERROR}//
};

