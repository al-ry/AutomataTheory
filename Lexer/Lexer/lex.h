#pragma once
#include "util.h"
#include <string>
#include <map>
#include <string>
#include <vector>
#include <optional>
#include <set>

typedef enum class TokenKing
{
	TOKEN_EOF,
	TOKEN_COLON,
	TOKEN_LPAREN,
	TOKEN_RPAREN,
	TOKEN_LBRACE,
	TOKEN_RBRACE,
	TOKEN_LBRACKET,
	TOKEN_RBRACKET,
	TOKEN_COMMA,
	TOKEN_DOT,
	TOKEN_POUND,
	TOKEN_QUESTION,
	TOKEN_SEMICOLON,
	TOKEN_KEYWORD,
	TOKEN_INT,
	TOKEN_FLOAT,
	TOKEN_STR,
	TOKEN_NAME,
	TOKEN_CHAR,
	TOKEN_NOT,
	TOKEN_MUL,
	TOKEN_DIV,
	TOKEN_MOD,
	TOKEN_ADD,
	TOKEN_SUB,
	TOKEN_AND,
	TOKEN_OR,
	TOKEN_OR_OR,
	TOKEN_INC,
	TOKEN_DEC,
	TOKEN_AND_AND, 
	TOKEN_NOTEQ,
	TOKEN_ASSIGN,
	TOKEN_EQ,
	TOKEN_ASSIGN_DIV,
	TOKEN_ASSIGN_MUL,
	TOKEN_ASSIGN_SUB,
	TOKEN_ASSIGN_ADD,
	TOKEN_ASSIGN_MOD,
	TOKEN_GT,
	TOKEN_LT,
	TOKEN_GTEQ,
	TOKEN_LTEQ,

	TOKEN_ERROR
} TokenKind;

typedef enum class TokenSuffix {
	SUFFIX_NONE,
	SUFFIX_D,
	SUFFIX_U,
	SUFFIX_L,
	SUFFIX_UL,
	SUFFIX_LL,
	SUFFIX_ULL,
} TokenSuffix;

typedef enum class TokenMod {
	MOD_NONE,
	MOD_HEX,
	MOD_OCT,
	MOD_CHAR,
	MOD_BIN
} TokenMod;

std::string GetTokenName(TokenKind kind);

bool IsKeyword(const std::string& keyword);

const int HEX_BASE = 16;
const int OCT_BASE = 8;
const int BIN_BASE = 2;
const int DEC_BASE = 10;


const std::map<TokenKind, std::string> TOKEN_KIND_NAMES = {
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
	{TokenKind::TOKEN_INT, "int"},
	{TokenKind::TOKEN_FLOAT, "float"},
	{TokenKind::TOKEN_STR, "string"},
	{TokenKind::TOKEN_NAME, "name"},//
	{TokenKind::TOKEN_CHAR, "char"},//
	{TokenKind::TOKEN_NOT, "!"},//
	{TokenKind::TOKEN_MUL, "*"},//
	{TokenKind::TOKEN_DIV, "/"},//
	{TokenKind::TOKEN_MOD, "%"},//
	{TokenKind::TOKEN_AND, "&"},//
	{TokenKind::TOKEN_ADD, "+"},//
	{TokenKind::TOKEN_SUB, "-"},//
	{TokenKind::TOKEN_OR, "|"},//
	{TokenKind::TOKEN_OR_OR, "||"},//
	{TokenKind::TOKEN_INC, "++"},//
	{TokenKind::TOKEN_DEC, "--"},//
	{TokenKind::TOKEN_AND_AND, "&&"},//
	{TokenKind::TOKEN_NOTEQ, "!="},//
	{TokenKind::TOKEN_EQ, "=="},//
	{TokenKind::TOKEN_GT, ">"},
	{TokenKind::TOKEN_LT, "<"},
	{TokenKind::TOKEN_GTEQ, ">="},
	{TokenKind::TOKEN_LTEQ, "<="},
	{TokenKind::TOKEN_ASSIGN, "="},//
	{TokenKind::TOKEN_ASSIGN_DIV, "/="},//
	{TokenKind::TOKEN_ASSIGN_MOD, "%="},//
	{TokenKind::TOKEN_ASSIGN_ADD, "+="},//
	{TokenKind::TOKEN_ASSIGN_SUB, "-="},//
	{TokenKind::TOKEN_ASSIGN_MUL, "*="},//
	{TokenKind::TOKEN_ERROR, "error"}//
};



typedef struct LexemeLocation
{
	int lineNum = 1;
	int lineOffset = 1;
} LexemeLocation;


class Token
{
public:
	Token(TokenKind kind, const LexemeLocation& loc)
		: kind(kind) 
		, loc(loc)
	{
	}
	Token()
	{
	}
	~Token()
	{
	}

	TokenKind kind;
	LexemeLocation loc;
	TokenMod mod = TokenMod::MOD_NONE;
	TokenSuffix suffix = TokenSuffix::SUFFIX_NONE;
	
	// lexer token variables
	double real_number = 0;
	unsigned long long   int_number = 0;
	std::string string;
	std::string name;

};

const std::vector<std::string> KEYWORDS = {
	"if",
	"else",
	"return",
	"continue",
	"break",
	"while",
	"do",
	"for",
	"switch",
	"case",
	"default",
	"func",
	"const",
	"struct",
	"enum",
	"sizeof",
	"var"
};


const std::map<char, char> ESCAPE_SEQUENCE_TO_CHAR = {
	{'0','\0'},
	{'\'','\''},
	{'"','"'},
	{'\\','\\'},
	{'n','\n'},
	{'r','\r'},
	{'t', '\t'},
	{'v','\v'},
	{'b','\b'},
	{'a','\a'},
};

std::optional<char> GetCharFromEscapeSeq(char ch);	

bool IsDelimeter(char ch);


const std::set<char> DELIMETERS = {
	'+',
	'-',
	'/',
	'%',
	' ',
	';',
	':',
	'/',
	'>',
	'<',
	'=',
	'(',
	')',
	'{',
	'}',
	'[',
	']',
	//'.',
	',',
	'!',
	'|',
	'&',
	'#',
	'\n',
	0
};

const std::map<char, uint8_t> HEX_TO_DEC_DIGIT = {
	{'0', 0},
	{'1', 1},
	{'2', 2},
	{'3', 3},
	{'4', 4},
	{'5', 5},
	{'6', 6},
	{'7', 7},
	{'8', 8},
	{'9', 9},
	{'A', 10}, {'a', 10},
	{'B', 11}, {'b', 11},
	{'C', 12}, {'c', 12},
	{'D', 13}, {'d', 13},
	{'E', 14}, {'e', 14},
	{'F', 15}, {'f', 15},
};

std::optional<uint8_t> GetDigitFromHex(char hexNum);

bool IsOctDigit(char digit);

class Lexer
{
public:
	Lexer(const std::string& buf);
	Token GetNextToken();
private:
	char PeekChar();
	char ReadChar();

	void SkipWhitespacesAndComments();
	void SkipWhiteSpaces();
	void ScanChar();
	std::optional<int> ScanHexToIntInCharLiteral();
	std::optional<int> ScanHexToIntInStrLiteral();
	void ScanInt();
	void ScanFloat();
	void ScanString();
	void ScanName();
	long long StrToLL(size_t startPos, size_t endPos, int base);
	int GetBase();
	void GetSuffix();
	bool IsNullIntLiteral(size_t start, size_t end);
	void SeekDelimeter();
	void ParseIntWithBase(int base);

	void Err(const std::string &description, const LexemeLocation& loc);
private:
	LexemeLocation m_loc;
	Buf m_buf;
	Token m_currToken;
};