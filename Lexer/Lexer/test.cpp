#include <cassert>
#include <string>
#include "lex.h"
#include <iostream>

void AssertToken(const Token& token, const Token& expToken)
{
	assert(token.kind == expToken.kind);
	assert(token.loc.lineNum == expToken.loc.lineNum);
	assert(token.loc.lineOffset == expToken.loc.lineOffset);
}

void BracketsTest()
{
	std::string str = " {}  \n[   ]  \n     ) (";
	Lexer lex(str);
	AssertToken(lex.GetNextToken(), { TokenKind::TOKEN_LBRACE, {1, 2}});
	AssertToken(lex.GetNextToken(), { TokenKind::TOKEN_RBRACE, {1, 3}});
	AssertToken(lex.GetNextToken(), { TokenKind::TOKEN_RBRACKET, {2, 1}});
	AssertToken(lex.GetNextToken(), { TokenKind::TOKEN_LBRACKET, {2, 5}});
	AssertToken(lex.GetNextToken(), { TokenKind::TOKEN_RPAREN, {3, 6} });
	AssertToken(lex.GetNextToken(), { TokenKind::TOKEN_LPAREN, {3, 8} });
	AssertToken(lex.GetNextToken(), { TokenKind::TOKEN_EOF, {3, 9} });
}


void AssignTest()
{
	std::string str = "= += -= /= %= *=";
	Lexer lex(str);
	AssertToken(lex.GetNextToken(), { TokenKind::TOKEN_ASSIGN, {1, 1} });
	AssertToken(lex.GetNextToken(), { TokenKind::TOKEN_ASSIGN_ADD, {1, 3} });
	AssertToken(lex.GetNextToken(), { TokenKind::TOKEN_ASSIGN_SUB, {1, 6} });
	AssertToken(lex.GetNextToken(), { TokenKind::TOKEN_ASSIGN_DIV, {1, 9} });
	AssertToken(lex.GetNextToken(), { TokenKind::TOKEN_ASSIGN_MOD, {1, 12} });
	AssertToken(lex.GetNextToken(), { TokenKind::TOKEN_ASSIGN_MUL, {1, 15} });
}

void MiscTest()
{
	std::string str = "\n# . , : ; ? = == + ++ - -- & && !";
	Lexer lex(str);
	AssertToken(lex.GetNextToken(), { TokenKind::TOKEN_POUND, {2, 1} });
	AssertToken(lex.GetNextToken(), { TokenKind::TOKEN_DOT, {2, 3} });
	AssertToken(lex.GetNextToken(), { TokenKind::TOKEN_COMMA, {2, 5} });
	AssertToken(lex.GetNextToken(), { TokenKind::TOKEN_COLON, {2, 7} });
	AssertToken(lex.GetNextToken(), { TokenKind::TOKEN_SEMICOLON, {2, 9} });
	AssertToken(lex.GetNextToken(), { TokenKind::TOKEN_QUESTION, {2, 11} });
    AssertToken(lex.GetNextToken(), { TokenKind::TOKEN_ASSIGN, {2, 13} });
	AssertToken(lex.GetNextToken(), { TokenKind::TOKEN_EQ, {2, 15} });
	AssertToken(lex.GetNextToken(), { TokenKind::TOKEN_ADD, {2, 17} });
	AssertToken(lex.GetNextToken(), { TokenKind::TOKEN_INC, {2, 19} });
	AssertToken(lex.GetNextToken(), { TokenKind::TOKEN_SUB, {2, 21} });
	AssertToken(lex.GetNextToken(), { TokenKind::TOKEN_DEC, {2, 23} });
	AssertToken(lex.GetNextToken(), { TokenKind::TOKEN_AND, {2, 25} });
	AssertToken(lex.GetNextToken(), { TokenKind::TOKEN_AND_AND, {2, 27} });
	AssertToken(lex.GetNextToken(), { TokenKind::TOKEN_NOT, {2, 29} });
}

void RelopTest()
{
	std::string str = "";
	Lexer lex(str);
}
