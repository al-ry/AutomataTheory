#include "stdafx.h"
#include "util.h"
#include "lex.h"

#include <iostream>
#include <string>
#include <vector>
#include <cassert>



void print_token_info(const Token& token)
{
	std::cout << "token_str_rep: " << "\"" << GetTokenName(token.kind) << "\"" << "\n";
	if (token.kind == TokenKind::TOKEN_INT)
	{ 
		std::cout << "int val: " << std::to_string(token.int_number) << "\n";
		if (token.suffix != TokenSuffix::SUFFIX_NONE)
		{
			std::cout << "suffix: ";
			switch (token.suffix)
			{
			case TokenSuffix::SUFFIX_L:
				std::cout << "l";
				break;
			case TokenSuffix::SUFFIX_LL:
				std::cout << "ll";
				break;
			case TokenSuffix::SUFFIX_U:
				std::cout << "u";
				break;
			case TokenSuffix::SUFFIX_UL:
				std::cout << "ul";
				break;
			case TokenSuffix::SUFFIX_ULL:
				std::cout << "ull";
				break;
			default:
				break;
			}
			std::cout << "\n";
		}
	}
	if (token.kind == TokenKind::TOKEN_KEYWORD)
	{
		std::cout << "keyword name: " << token.name << "\n";
	}
	if (token.kind == TokenKind::TOKEN_FLOAT)
	{
		std::cout << "float val: " << std::to_string(token.real_number) << "\n";
		if (token.suffix == TokenSuffix::SUFFIX_D)
		{
			std::cout << "suffix: d\n";
		}
	}
	if (token.kind == TokenKind::TOKEN_NAME)
	{
		std::cout << "name: " << "\"" << token.name << "\"" << "\n";
	}
	std::cout << "line: " << token.loc.lineNum << "\n";
	std::cout << "offset: " << token.loc.lineOffset << "\n";
	std::cout << "-----------------------------------" << "\n";
}

int main(int argc, char** argv)
{
	std::string buffer = ReadFileToBuffer("test.txt");
	Lexer lex(buffer);
	auto token = lex.GetNextToken();
	while (token.kind != TokenKing::TOKEN_EOF)
	{
		print_token_info(token);
		token = lex.GetNextToken();
	}
	print_token_info(token);
	return 0;
}


//ограничение на кол-во символов в id
//




