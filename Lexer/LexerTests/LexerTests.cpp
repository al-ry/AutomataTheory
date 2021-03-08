#include "stdafx.h"
#include "../Lexer/lex.h"

void AssertToken(const Token& token, const Token& expToken)
{
	BOOST_CHECK(token.kind == expToken.kind);
	BOOST_CHECK(token.loc.lineNum == expToken.loc.lineNum);
	BOOST_CHECK(token.loc.lineOffset == expToken.loc.lineOffset);
}


BOOST_AUTO_TEST_CASE(brackets_tests)
{
	std::string str = " {}  \n[   ]  \n     ) (";
	Lexer lex(str);
	AssertToken(lex.GetNextToken(), { TokenKind::TOKEN_LBRACE, {1, 2} });
	AssertToken(lex.GetNextToken(), { TokenKind::TOKEN_RBRACE, {1, 3} });
	AssertToken(lex.GetNextToken(), { TokenKind::TOKEN_RBRACKET, {2, 1} });
	AssertToken(lex.GetNextToken(), { TokenKind::TOKEN_LBRACKET, {2, 5} });
	AssertToken(lex.GetNextToken(), { TokenKind::TOKEN_RPAREN, {3, 6} });
	AssertToken(lex.GetNextToken(), { TokenKind::TOKEN_LPAREN, {3, 8} });
	AssertToken(lex.GetNextToken(), { TokenKind::TOKEN_EOF, {3, 9} });
}


BOOST_AUTO_TEST_CASE(math_expr_tests)
{
	std::string str = "= += -= /= %= *= * - + -- ++ / %";
	Lexer lex(str);
	AssertToken(lex.GetNextToken(), { TokenKind::TOKEN_ASSIGN, {1, 1} });
	AssertToken(lex.GetNextToken(), { TokenKind::TOKEN_ASSIGN_ADD, {1, 3} });
	AssertToken(lex.GetNextToken(), { TokenKind::TOKEN_ASSIGN_SUB, {1, 6} });
	AssertToken(lex.GetNextToken(), { TokenKind::TOKEN_ASSIGN_DIV, {1, 9} });
	AssertToken(lex.GetNextToken(), { TokenKind::TOKEN_ASSIGN_MOD, {1, 12} });
	AssertToken(lex.GetNextToken(), { TokenKind::TOKEN_ASSIGN_MUL, {1, 15} });
	AssertToken(lex.GetNextToken(), { TokenKind::TOKEN_MUL, {1, 18} });
	AssertToken(lex.GetNextToken(), { TokenKind::TOKEN_SUB, {1, 20} });
	AssertToken(lex.GetNextToken(), { TokenKind::TOKEN_ADD, {1, 22} });
}

BOOST_AUTO_TEST_CASE(relops_tests)
{
	std::string str = ">   < >=\n  <=";
	Lexer lex(str);
	AssertToken(lex.GetNextToken(), { TokenKind::TOKEN_GT, {1, 1} });
	AssertToken(lex.GetNextToken(), { TokenKind::TOKEN_LT, {1, 5} });
	AssertToken(lex.GetNextToken(), { TokenKind::TOKEN_GTEQ, {1, 7} });
	AssertToken(lex.GetNextToken(), { TokenKind::TOKEN_LTEQ, {2, 3} });
}



BOOST_AUTO_TEST_CASE(_tests)
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
	AssertToken(lex.GetNextToken(), { TokenKind::TOKEN_ADD, {2, 18} });
	AssertToken(lex.GetNextToken(), { TokenKind::TOKEN_INC, {2, 20} });
	AssertToken(lex.GetNextToken(), { TokenKind::TOKEN_SUB, {2, 23} });
	AssertToken(lex.GetNextToken(), { TokenKind::TOKEN_DEC, {2, 25} });
	AssertToken(lex.GetNextToken(), { TokenKind::TOKEN_AND, {2, 28} });
	AssertToken(lex.GetNextToken(), { TokenKind::TOKEN_AND_AND, {2, 30} });
	AssertToken(lex.GetNextToken(), { TokenKind::TOKEN_NOT, {2, 33} });

}

BOOST_AUTO_TEST_CASE(separators_tests)
{
	std::string str = "\n . , ;";
	Lexer lex(str);
	AssertToken(lex.GetNextToken(), { TokenKind::TOKEN_DOT, {2, 2} });
	AssertToken(lex.GetNextToken(), { TokenKind::TOKEN_COMMA, {2, 4} });
	AssertToken(lex.GetNextToken(), { TokenKind::TOKEN_SEMICOLON, {2, 6} });
}


BOOST_AUTO_TEST_CASE(misc_tests)
{
	std::string str = "\n# . , : ; ? = == + ++ - -- & && !";
	Lexer lex(str);

}

BOOST_AUTO_TEST_SUITE(comments_test)
	BOOST_AUTO_TEST_SUITE(single_line_comments_test)
		BOOST_AUTO_TEST_CASE(when_comment_end_with_eof)
		{
			std::string str = "\n// some comment";
			Lexer lex(str);
			AssertToken(lex.GetNextToken(), { TokenKind::TOKEN_EOF, {2, 16} });
		}
		BOOST_AUTO_TEST_CASE(when_comment_end_with_lineend_symbol)
		{
			std::string str = "\n// some comment\n";
			Lexer lex(str);
			AssertToken(lex.GetNextToken(), { TokenKind::TOKEN_EOF, {3, 1} });
		}
		BOOST_AUTO_TEST_CASE(when_comment_end_with_lineend_symbol_and_there_are_tokens_in_next_line)
		{
			std::string str = "\n// some comment\n {}";
			Lexer lex(str);
			AssertToken(lex.GetNextToken(), { TokenKind::TOKEN_LBRACE, {3, 2} });
			AssertToken(lex.GetNextToken(), { TokenKind::TOKEN_RBRACE, {3, 3} });
		}
		BOOST_AUTO_TEST_CASE(when_comment_end_with_lineend_symbol_and_there_is_comment_in_next_line)
		{
			std::string str = "\n// some comment\n// another comment";
			Lexer lex(str);
			AssertToken(lex.GetNextToken(), { TokenKind::TOKEN_EOF, {3, 19} });
		}
	BOOST_AUTO_TEST_SUITE_END()
	BOOST_AUTO_TEST_SUITE(multiline_comment_tests)
		BOOST_AUTO_TEST_CASE(when_simple_comment)
		{
				std::string str = "/*multiline comment*/";
				Lexer lex(str);
				AssertToken(lex.GetNextToken(), { TokenKind::TOKEN_EOF, {1, 22} });
		}
		BOOST_AUTO_TEST_CASE(when_comment_with_endline)
		{
			std::string str = "/*first line\nsecond line\nthird line*/";
			Lexer lex(str);
			AssertToken(lex.GetNextToken(), { TokenKind::TOKEN_EOF, {3, 13} });
		}
		BOOST_AUTO_TEST_CASE(when_there_is_token_after_comment)
		{
			std::string str = "/*first line*/\n/";
			Lexer lex(str);
			AssertToken(lex.GetNextToken(), { TokenKind::TOKEN_DIV, {2, 1} });
		}
		BOOST_AUTO_TEST_CASE(when_there_is_in_next_line_token_after_comment)
		{
			std::string str = "/*first \n line*/\n/";
			Lexer lex(str);
			AssertToken(lex.GetNextToken(), { TokenKind::TOKEN_DIV, {3, 1} });
		}
		BOOST_AUTO_TEST_SUITE(when_unfinished_comment)
			BOOST_AUTO_TEST_CASE(when_without_start_symbol_should_return_error_token)
			{
				std::string str = "/*first \n line/\n/";
				Lexer lex(str);
				AssertToken(lex.GetNextToken(), { TokenKind::TOKEN_ERROR, {1, 1} });
			}
			BOOST_AUTO_TEST_CASE(when_without_slash_symbol_should_return_error_token)
			{
				std::string str = "\n/*first \n line *\n/";
				Lexer lex(str);
				AssertToken(lex.GetNextToken(), { TokenKind::TOKEN_ERROR, {2, 1} });
			}
			BOOST_AUTO_TEST_CASE(when_without_slash_and_star_symbols_should_return_error_token)
			{
				std::string str = "\n /*first \n line \n/";
				Lexer lex(str);
				AssertToken(lex.GetNextToken(), { TokenKind::TOKEN_ERROR, {2, 2} });
			}
			BOOST_AUTO_TEST_CASE(when_endl_line_before_comment_closing)
			{
				std::string str = "\n /*first \n line";
				Lexer lex(str);
				AssertToken(lex.GetNextToken(), { TokenKind::TOKEN_ERROR, {2, 2} });
			}
		BOOST_AUTO_TEST_SUITE_END()
	BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(identifiers_tests)
	BOOST_AUTO_TEST_CASE(simple_ids)
	{
		std::string str = "some_id __id id_123__";
		Lexer lex(str);
		auto token = lex.GetNextToken();
		AssertToken(token, { TokenKind::TOKEN_NAME, {1, 1} });
		BOOST_CHECK(token.name == str.substr(0, 7));
		token = lex.GetNextToken();
		AssertToken(token, { TokenKind::TOKEN_NAME, {1, 9} });
		BOOST_CHECK(token.name == str.substr(8, 4));

		token = lex.GetNextToken();
		AssertToken(token, { TokenKind::TOKEN_NAME, {1, 14} });
		BOOST_CHECK(token.name == str.substr(13, 8));
	}
	BOOST_AUTO_TEST_CASE(when_id_contains_keyword)
	{
		std::string str = "ifid whileid";
		Lexer lex(str);
		auto token = lex.GetNextToken();
		AssertToken(token, { TokenKind::TOKEN_NAME, {1, 1} });
		BOOST_CHECK(token.name == str.substr(0, 4));
		token = lex.GetNextToken();
		AssertToken(token, { TokenKind::TOKEN_NAME, {1, 6} });
		BOOST_CHECK(token.name == str.substr(5, 7));

	}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(keywords_tests)
	BOOST_AUTO_TEST_CASE(keywords)
	{
		std::string keywords = "if while else return var";
		Lexer lex(keywords);
		auto token = lex.GetNextToken();
		AssertToken(token, { TokenKind::TOKEN_KEYWORD, {1, 1} });
		BOOST_CHECK(token.name == "if");
		token = lex.GetNextToken();
		AssertToken(token, { TokenKind::TOKEN_KEYWORD, {1, 4} });
		BOOST_CHECK(token.name == "while");
		token = lex.GetNextToken();
		AssertToken(token, { TokenKind::TOKEN_KEYWORD, {1, 10} });
		BOOST_CHECK(token.name == "else");
		token = lex.GetNextToken();
		AssertToken(token, { TokenKind::TOKEN_KEYWORD, {1, 15} });
		BOOST_CHECK(token.name == "return");

		token = lex.GetNextToken();
		AssertToken(token, { TokenKind::TOKEN_KEYWORD, {1, 22} });
		BOOST_CHECK(token.name == "var");
	}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(char_tests)
	BOOST_AUTO_TEST_CASE(when_quotes_without_symbols)
	{
		std::string keywords = "\'\'";
		Lexer lex(keywords);
		AssertToken(lex.GetNextToken(), { TokenKind::TOKEN_ERROR, {1, 1} });
	}
	BOOST_AUTO_TEST_CASE(when_dont_have_closing_quote_at_the_end)
	{
		std::string keywords = "\'c";
		Lexer lex(keywords);
		AssertToken(lex.GetNextToken(), { TokenKind::TOKEN_ERROR, {1, 1} });
	}
	BOOST_AUTO_TEST_CASE(when_simple_char)
	{
		std::string keywords = "\'c\'";
		Lexer lex(keywords);
		auto token = lex.GetNextToken();
		AssertToken(token, { TokenKind::TOKEN_INT, {1, 1} });
		BOOST_CHECK(token.int_number = 'c');
	}
	BOOST_AUTO_TEST_CASE(char_with_escape_seq)
	{
		std::string keywords = "'\\t' '\\b' '\\'' '\"''";
		Lexer lex(keywords);
		auto token = lex.GetNextToken();
		AssertToken(token, { TokenKind::TOKEN_INT, {1, 1} });
		BOOST_CHECK(token.mod == TokenMod::MOD_CHAR);
		BOOST_CHECK(token.int_number = '\t');

		token = lex.GetNextToken();
		AssertToken(token, { TokenKind::TOKEN_INT, {1, 6} });
		BOOST_CHECK(token.mod == TokenMod::MOD_CHAR);
		BOOST_CHECK(token.int_number = '\b');

		token = lex.GetNextToken();
		AssertToken(token, { TokenKind::TOKEN_INT, {1, 11} });
		BOOST_CHECK(token.mod == TokenMod::MOD_CHAR);
		BOOST_CHECK(token.int_number = '\'');

		token = lex.GetNextToken();
		AssertToken(token, { TokenKind::TOKEN_INT, {1, 16} });
		BOOST_CHECK(token.mod == TokenMod::MOD_CHAR);
		BOOST_CHECK(token.int_number = '"');
	}
	BOOST_AUTO_TEST_CASE(when_unrecognized_escape_seq)
	{
		std::string keywords = "'\\y'";
		Lexer lex(keywords);

		auto token = lex.GetNextToken();
		AssertToken(token, { TokenKind::TOKEN_ERROR, {1, 1} });
		BOOST_CHECK(token.mod == TokenMod::MOD_NONE);
	}
	BOOST_AUTO_TEST_CASE(when_char_with_hex_rep)
	{
		std::string keywords = "'\\xFF' '\\xfe' '\\xA' ";
		Lexer lex(keywords);

		auto token = lex.GetNextToken();
		AssertToken(token, { TokenKind::TOKEN_INT, {1, 1} });
		BOOST_CHECK(token.mod == TokenMod::MOD_CHAR);
		BOOST_CHECK(token.int_number == 0xFF);

		token = lex.GetNextToken();
		AssertToken(token, { TokenKind::TOKEN_INT, {1, 8} });
		BOOST_CHECK(token.mod == TokenMod::MOD_CHAR);
		BOOST_CHECK(token.int_number == 0xFE);

		token = lex.GetNextToken();
		AssertToken(token, { TokenKind::TOKEN_INT, {1, 15} });
		BOOST_CHECK(token.mod == TokenMod::MOD_CHAR);
		BOOST_CHECK(token.int_number == 0xA);

	}
	BOOST_AUTO_TEST_CASE(error_cases)
	{
		std::string keywords = "'\\xfg' '\\xfff' 'c \n'' '\n' '\n'\\x'";
		Lexer lex(keywords);

		auto token = lex.GetNextToken();
		AssertToken(token, { TokenKind::TOKEN_ERROR, {1, 1} });
		BOOST_CHECK(token.mod == TokenMod::MOD_NONE);

		token = lex.GetNextToken();
		AssertToken(token, { TokenKind::TOKEN_ERROR, {1, 8} });
		BOOST_CHECK(token.mod == TokenMod::MOD_NONE);
		 
		token = lex.GetNextToken();
		AssertToken(token, { TokenKind::TOKEN_ERROR, {1, 16} });
		BOOST_CHECK(token.mod == TokenMod::MOD_NONE);

		token = lex.GetNextToken();
		AssertToken(token, { TokenKind::TOKEN_ERROR, {2, 1} });
		BOOST_CHECK(token.mod == TokenMod::MOD_NONE);

		token = lex.GetNextToken();
		AssertToken(token, { TokenKind::TOKEN_ERROR, {2, 4} });
		BOOST_CHECK(token.mod == TokenMod::MOD_NONE);

		token = lex.GetNextToken();
		AssertToken(token, { TokenKind::TOKEN_INT, {3, 1} });
		BOOST_CHECK(token.mod == TokenMod::MOD_CHAR);

		token = lex.GetNextToken();
		AssertToken(token, { TokenKind::TOKEN_ERROR, {4, 1} });
		BOOST_CHECK(token.mod == TokenMod::MOD_NONE);

		token = lex.GetNextToken();
		AssertToken(token, { TokenKind::TOKEN_EOF, {4, 5} });
		BOOST_CHECK(token.mod == TokenMod::MOD_NONE);

	}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(int_tests)
	BOOST_AUTO_TEST_CASE(int_with_10base)
	{
		std::string keywords = "8000 123 0";
		Lexer lex(keywords);
		auto token = lex.GetNextToken();
		AssertToken(token, { TokenKind::TOKEN_INT, {1, 1} });
		BOOST_CHECK(token.mod == TokenMod::MOD_NONE);
		BOOST_CHECK(token.suffix == TokenSuffix::SUFFIX_NONE);
		BOOST_CHECK(token.int_number == 8000);

		token = lex.GetNextToken();
		AssertToken(token, { TokenKind::TOKEN_INT, {1, 6} });
		BOOST_CHECK(token.mod == TokenMod::MOD_NONE);
		BOOST_CHECK(token.suffix == TokenSuffix::SUFFIX_NONE);
		BOOST_CHECK(token.int_number == 123);

		token = lex.GetNextToken();
		BOOST_CHECK(token.mod == TokenMod::MOD_NONE);
		BOOST_CHECK(token.suffix == TokenSuffix::SUFFIX_NONE);
		BOOST_CHECK(token.int_number == 0);
	}
	BOOST_AUTO_TEST_CASE(int_with_2base)
	{
		std::string keywords = "0b0101 0b";
		Lexer lex(keywords);
		auto token = lex.GetNextToken();

		BOOST_CHECK(token.mod == TokenMod::MOD_BIN);
		BOOST_CHECK(token.suffix == TokenSuffix::SUFFIX_NONE);
		BOOST_CHECK(token.int_number == 5);

		token = lex.GetNextToken();
		AssertToken(token, { TokenKind::TOKEN_ERROR, {1, 8} });
		BOOST_CHECK(token.mod == TokenMod::MOD_BIN);
		BOOST_CHECK(token.suffix == TokenSuffix::SUFFIX_NONE);
	}
	BOOST_AUTO_TEST_CASE(int_with_8base)
	{
		std::string keywords = "0101 ";
		Lexer lex(keywords);

		auto token = lex.GetNextToken();
		BOOST_CHECK(token.mod == TokenMod::MOD_OCT);
		BOOST_CHECK(token.suffix == TokenSuffix::SUFFIX_NONE);
		BOOST_CHECK_EQUAL(token.int_number, 65);
	}
	BOOST_AUTO_TEST_CASE(int_with_16base)
	{
		std::string keywords = "0x 0xFF 0xff";
		Lexer lex(keywords);

		auto token = lex.GetNextToken();
		AssertToken(token, { TokenKind::TOKEN_ERROR, {1, 1} });
		BOOST_CHECK(token.mod == TokenMod::MOD_HEX);
		BOOST_CHECK(token.suffix == TokenSuffix::SUFFIX_NONE);

		token = lex.GetNextToken();
		BOOST_CHECK(token.mod == TokenMod::MOD_HEX);
		BOOST_CHECK(token.suffix == TokenSuffix::SUFFIX_NONE);
		BOOST_CHECK(token.int_number == 255);

		token = lex.GetNextToken();
		BOOST_CHECK(token.mod == TokenMod::MOD_HEX);
		BOOST_CHECK(token.suffix == TokenSuffix::SUFFIX_NONE);
		BOOST_CHECK(token.int_number == 255);
	}
	BOOST_AUTO_TEST_CASE(int_with_suffix)
	{
		std::string keywords = "20u 0xFFl 15ll 0ull ";
		Lexer lex(keywords);

		auto token = lex.GetNextToken();
		BOOST_CHECK(token.suffix == TokenSuffix::SUFFIX_U);
		BOOST_CHECK(token.int_number == 20);

		token = lex.GetNextToken();
		BOOST_CHECK(token.suffix == TokenSuffix::SUFFIX_L);
		BOOST_CHECK(token.int_number == 255);

		token = lex.GetNextToken();
		BOOST_CHECK(token.suffix == TokenSuffix::SUFFIX_LL);
		BOOST_CHECK(token.int_number == 15);

		token = lex.GetNextToken();
		BOOST_CHECK(token.suffix == TokenSuffix::SUFFIX_ULL);
		BOOST_CHECK(token.int_number == 0);
	}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(float_tests)
	BOOST_AUTO_TEST_CASE(misc)
	{
		std::string keywords = "4. 89.56 66.666d .050";
		Lexer lex(keywords);


		auto token = lex.GetNextToken();
		AssertToken(token, { TokenKind::TOKEN_FLOAT, {1, 1} });
		BOOST_CHECK(token.kind == TokenKind::TOKEN_FLOAT);
		BOOST_CHECK(token.suffix == TokenSuffix::SUFFIX_NONE);
		BOOST_CHECK_CLOSE(token.real_number, 4.0, 0.1);


		token = lex.GetNextToken();
		AssertToken(token, { TokenKind::TOKEN_FLOAT, {1, 4} });
		BOOST_CHECK(token.kind == TokenKind::TOKEN_FLOAT);
		BOOST_CHECK(token.suffix == TokenSuffix::SUFFIX_NONE);
		BOOST_CHECK_CLOSE(token.real_number, 89.56, 0.01);

		token = lex.GetNextToken();
		AssertToken(token, { TokenKind::TOKEN_FLOAT, {1, 10} });
		BOOST_CHECK(token.kind == TokenKind::TOKEN_FLOAT);
		BOOST_CHECK(token.suffix == TokenSuffix::SUFFIX_D);
		BOOST_CHECK_CLOSE(token.real_number, 66.666, 0.001);

		token = lex.GetNextToken();
		AssertToken(token, { TokenKind::TOKEN_FLOAT, {1, 18} });
		BOOST_CHECK(token.kind == TokenKind::TOKEN_FLOAT);
		BOOST_CHECK(token.suffix == TokenSuffix::SUFFIX_NONE);
		BOOST_CHECK_CLOSE(token.real_number, 0.05, 0.01);
	}
BOOST_AUTO_TEST_SUITE_END()