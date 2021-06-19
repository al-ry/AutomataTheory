#include "lex.h"
#include <iostream>
#include <cassert>

std::string GetTokenName(TokenKind kind)
{
	auto it = TOKEN_KIND_NAMES.find(kind);
	if (it != TOKEN_KIND_NAMES.end())
	{
		return it->second;
	}
	return "<unknown>";
}

bool IsKeyword(const std::string& keyword)
{
	auto found = KEYWORDS_TOKENS.find(keyword);
	if (found != KEYWORDS_TOKENS.end())
	{
		return true;
	}
	return false;
}

TokenKind GetKeyword(const std::string& keyword)
{
	return KEYWORDS_TOKENS.find(keyword)->second;
}

std::optional<char> GetCharFromEscapeSeq(char ch)
{
	auto it = ESCAPE_SEQUENCE_TO_CHAR.find(ch);
	if (it != ESCAPE_SEQUENCE_TO_CHAR.end())
	{
		return it->second;
	}
	return std::nullopt;
}

bool IsDelimeter(char ch)
{
	if (DELIMETERS.find(ch) != DELIMETERS.end())
	{
		return true;
	}
	return false;
}

std::optional<uint8_t> GetDigitFromHex(char hexNum)
{
	auto it = HEX_TO_DEC_DIGIT.find(hexNum);
	if (it != HEX_TO_DEC_DIGIT.end())
	{
		return it->second;
	}
	return std::nullopt;
}

bool IsOctDigit(char digit)
{
	return digit == '0'
		|| digit == '1'
		|| digit == '2'
		|| digit == '3'
		|| digit == '4'
		|| digit == '5'
		|| digit == '6'
		|| digit == '7';
}


//public members

Lexer::Lexer(const std::string& buf)
{
	m_buf.data = buf;
	m_buf.pos = 0;
}

#define CASE1(ch, k) \
	case ch: \
		m_currToken.kind = k; \
		m_currToken.loc = m_loc; \
		ReadChar(); \
		break;

#define CASE2(ch1, k1, ch2, k2) \
	case ch1: \
		m_currToken.kind = k1; \
		m_currToken.loc = m_loc; \
		ReadChar(); \
		if (PeekChar() == ch2) \
		{ \
			m_currToken.kind = k2; \
			ReadChar(); \
		} \
		break;
#define CASE3(ch1, k1, ch2, k2, ch3, k3) \
	case ch1: \
		m_currToken.kind = k1; \
		m_currToken.loc = m_loc;\
		ReadChar(); \
		if (PeekChar() == ch2) \
		{ \
			m_currToken.kind = k2; \
			ReadChar(); \
		} else if (PeekChar() == ch3) \
		{ \
			m_currToken.kind = k3; \
			ReadChar(); \
		}\
		break;




Token Lexer::GetNextToken()
{
	m_currToken = Token();
	SkipWhitespacesAndComments();
	if (m_currToken.kind == TokenKind::TOKEN_ERROR)
	{
		return m_currToken;
	}

	switch (PeekChar())
	{
	case '.': {
		auto pos = m_buf.pos;
		m_currToken.loc = m_loc;
		pos++;
		if (isdigit(m_buf.data[pos]) || m_buf.data[pos] == 'e')
		{
			ScanFloat();
		}
		else
		{
			m_currToken.kind = TokenKind::TOKEN_DOT;
			ReadChar();
		}
		break;
	}
	case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9': {
		auto digitPos = m_buf.pos;
		auto lineOffset = m_loc.lineOffset;
		while (isdigit(PeekChar()))
		{
			ReadChar();
		}
		char c = PeekChar();
		m_buf.pos = digitPos;
		m_loc.lineOffset = lineOffset;
		if (c == '.' || tolower(c) == 'e')
		{
			ScanFloat();
		}
		else
		{
			ScanInt();
		}
		break;
	}
	case '\'':
		ScanChar();
		break;
	case '"':
		ScanString();
		break;
	case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g': case 'h': case 'i': case 'j':
	case 'k': case 'l': case 'm': case 'n': case 'o': case 'p': case 'q': case 'r': case 's': case 't':
	case 'u': case 'v': case 'w': case 'x': case 'y': case 'z':
	case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G': case 'H': case 'I': case 'J':
	case 'K': case 'L': case 'M': case 'N': case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T':
	case 'U': case 'V': case 'W': case 'X': case 'Y': case 'Z':
	case '_': {
		ScanName();
		break;
	}
	CASE1('\0', TokenKind::TOKEN_EOF)
	CASE1(':', TokenKind::TOKEN_COLON)
	CASE1('(', TokenKind::TOKEN_LPAREN)
	CASE1(')', TokenKind::TOKEN_RPAREN)
	CASE1('{', TokenKind::TOKEN_LBRACE)
	CASE1('}', TokenKind::TOKEN_RBRACE)
	CASE1('[', TokenKind::TOKEN_RBRACKET)
	CASE1(']', TokenKind::TOKEN_LBRACKET)
	CASE1(',', TokenKind::TOKEN_COMMA)
	CASE1('#', TokenKind::TOKEN_POUND)
	CASE1('?', TokenKind::TOKEN_QUESTION)
	CASE1(';', TokenKind::TOKEN_SEMICOLON)
	CASE2('>', TokenKing::TOKEN_GT, '=', TokenKind::TOKEN_GTEQ)
	CASE2('<', TokenKind::TOKEN_LT, '=', TokenKind::TOKEN_LTEQ)
	CASE2('=', TokenKind::TOKEN_ASSIGN, '=', TokenKind::TOKEN_EQ)
	CASE2('/', TokenKind::TOKEN_DIV, '=', TokenKind::TOKEN_ASSIGN_DIV)
	CASE2('%', TokenKind::TOKEN_MOD, '=', TokenKind::TOKEN_ASSIGN_MOD)
	CASE2('*', TokenKind::TOKEN_MUL, '=', TokenKind::TOKEN_ASSIGN_MUL)
	CASE2('!', TokenKind::TOKEN_NOT, '=', TokenKind::TOKEN_NOTEQ)
	CASE2('|', TokenKind::TOKEN_OR, '|', TokenKind::TOKEN_OR_OR)
	CASE2('&', TokenKind::TOKEN_AND, '&', TokenKind::TOKEN_AND_AND)
	CASE3('+', TokenKind::TOKEN_ADD, '=', TokenKind::TOKEN_ASSIGN_ADD, '+', TokenKind::TOKEN_INC)
	CASE3('-', TokenKind::TOKEN_SUB, '=', TokenKind::TOKEN_ASSIGN_SUB, '-', TokenKind::TOKEN_DEC)
	default:
		Err("Undefined token", m_loc);
		m_currToken.kind = TokenKind::TOKEN_ERROR;
		m_currToken.loc = m_loc;
		ReadChar();
		break;
	}
	return m_currToken;
}


//private members
char Lexer::PeekChar()
{
	if (m_buf.pos >= m_buf.data.size())
	{
		return '\0';
	}
	return m_buf.data[m_buf.pos];
}

char Lexer::ReadChar()
{
	if (m_buf.pos >= m_buf.data.size())
	{
		return '\0';
	}
	m_loc.lineOffset++;
	return m_buf.data[++m_buf.pos];
}

void Lexer::SkipWhitespacesAndComments()
{
	for (;;)
	{
		SkipWhiteSpaces();
		char ch = PeekChar();
		if (ch == '/')
		{
			LexemeLocation loc = { m_loc.lineNum, m_loc.lineOffset };
			auto currPos = m_buf.pos;
			ch = m_buf.data[++currPos];
			if (ch == '*')
			{
				m_buf.pos += 2;
				m_loc.lineOffset += 2;
				ch = PeekChar();
				while (ch != '\0' && (ch != '*' || m_buf.data[++currPos] != '/'))
				{
					if (ch == '\n')
					{
						m_loc.lineNum++;
						m_loc.lineOffset = 0;
					}
					ch = ReadChar();
					currPos = m_buf.pos;
				}
				if (ch == '\0')
				{
					Err("Not closed multiline comment", loc);
					m_currToken.kind = TokenKind::TOKEN_ERROR;
					m_currToken.loc = loc;
				}
				m_buf.pos += 2;
				m_loc.lineOffset += 2;
			}
			else if (ch == '/')
			{
				ch = ReadChar();
				while (ch != '\n' && ch != '\0')
				{
					ch = ReadChar();
				}
				if (ch == '\n')
				{
					m_loc.lineNum++;
					m_loc.lineOffset = 0;
					ReadChar();
				}
			}
			else
			{
				break;
			}
		}
		else
		{
			break;
		}
	}
}

void Lexer::SkipWhiteSpaces()
{
	char ch = PeekChar();
	while (isspace(ch))
	{
		if (ch == '\n')
		{
			m_loc.lineNum++;
			m_loc.lineOffset = 0;
		}
		ch = ReadChar();
	}

}

void Lexer::ScanChar()
{
	assert(PeekChar() == '\'');
	m_currToken.loc = m_loc;
	int value = 0;

	if (ReadChar() == '\'')
	{
		Err("Char literal cannot be empty.", m_currToken.loc);
		m_currToken.kind = TokenKind::TOKEN_ERROR;
		ReadChar();
		return;
	}
	else if (PeekChar() == '\n')
	{
		Err("Char literal cannot contain newline.", m_currToken.loc);
		m_currToken.kind = TokenKind::TOKEN_ERROR;
		ReadChar();
		m_loc.lineOffset = 1;
		m_loc.lineNum++;
		return;
	}
	else if (PeekChar() == '\\')
	{
		if (ReadChar() == 'x')
		{
			auto optVal = ScanHexToIntInCharLiteral();
			if (!optVal)
			{
				ReadChar();
				return;
			}
			value = *optVal;
		}
		else
		{
			auto optVal = GetCharFromEscapeSeq(PeekChar());
			if (!optVal)
			{
				Err("Unrecognized character escape sequence in char literal", m_currToken.loc);
				m_currToken.kind = TokenKind::TOKEN_ERROR;
				m_buf.pos += 2;
				m_loc.lineOffset += 2;
				return;
			}
			value = *optVal;
			ReadChar();
		}
	}
	else
	{
		value = PeekChar();
		ReadChar();
	}

	if (PeekChar() != '\'')
	{
		while (PeekChar() != 0 && PeekChar() != '\n' && PeekChar() != '\'')
		{
			ReadChar();
		}
		if (PeekChar() == '\n')
		{
			m_loc.lineNum++;
			m_loc.lineOffset = 0;
		}
		if (PeekChar() == '\'')
		{
			Err("Char literal contains greater than 1 symbol", m_currToken.loc);
		}
		else
		{
			Err("Char literal hasn't got closing quote.", m_currToken.loc);
		}
		ReadChar();
		m_currToken.kind = TokenKind::TOKEN_ERROR;
		return;
	}
	m_currToken.kind = TokenKind::TOKEN_INT;
	m_currToken.mod = TokenMod::MOD_CHAR;
	m_currToken.int_number = value;
	ReadChar();
}

std::optional<int> Lexer::ScanHexToIntInCharLiteral()
{
	assert(PeekChar() == 'x');
	int realValue = 0;
	char ch = ReadChar();
	auto val = GetDigitFromHex(ch);
	if (!val)
	{
		Err("Unrecognized hex number in char literal", m_currToken.loc);
		m_currToken.kind = TokenKind::TOKEN_ERROR;
		ReadChar();
		return std::nullopt;
	}
	realValue = *val;
	auto dig = GetDigitFromHex(ReadChar());
	if (dig)
	{
		realValue = *val * 16;
		realValue += *dig;
		ReadChar();
	}
	else
	{
		if (PeekChar() != '\'')
		{
			Err("Unrecognized hex number in char literal", m_currToken.loc);
			m_currToken.kind = TokenKind::TOKEN_ERROR;
			ReadChar();
			return std::nullopt;
		}
	}
	return realValue;
}

std::optional<int> Lexer::ScanHexToIntInStrLiteral()
{
	assert(PeekChar() == 'x');
	int realValue = 0;
	char ch = ReadChar();
	auto val = GetDigitFromHex(ch);
	if (!val)
	{
		Err("Unrecognized hex number in string literal", m_currToken.loc);
		m_currToken.kind = TokenKind::TOKEN_ERROR;
		ReadChar();
		return std::nullopt;
	}
	realValue = *val;
	auto dig = GetDigitFromHex(ReadChar());
	if (dig)
	{
		realValue = *val * 16;
		realValue += *dig;
		ReadChar();
	}
	else
	{
		ReadChar();
	}
	return realValue;
}

void Lexer::ScanInt()
{
	m_currToken.loc = m_loc;
	int base = GetBase();
	try
	{
		ParseIntWithBase(base);
	}
	catch (const std::out_of_range&)
	{
		Err("Integer overflow", m_currToken.loc);
		m_currToken.kind = TokenKind::TOKEN_ERROR;
	}
}

int Lexer::GetBase()
{
	PeekChar();
	int base = DEC_BASE;
	if (PeekChar() == '0')
	{
		ReadChar();
		if (tolower(PeekChar()) == 'x')
		{
			ReadChar();
			m_currToken.mod = TokenMod::MOD_HEX;
			base = HEX_BASE;
		} 
		else if (tolower(PeekChar()) == 'b')
		{
			ReadChar();
			m_currToken.mod = TokenMod::MOD_BIN;
			base = BIN_BASE;
		}
		else if (isdigit(PeekChar()))
		{
			m_currToken.mod = TokenMod::MOD_OCT;
			base = OCT_BASE;
		}
	}
	return base;
}

void Lexer::GetSuffix()
{
	if (tolower(PeekChar()) == 'u')
	{
		m_currToken.suffix = TokenSuffix::SUFFIX_U;
		if (tolower(ReadChar()) == 'l')
		{
			m_currToken.suffix = TokenSuffix::SUFFIX_UL;
			if (tolower(ReadChar()) == 'l')
			{
				m_currToken.suffix = TokenSuffix::SUFFIX_ULL;
				ReadChar();
			}

		}

	}
	else if (tolower(PeekChar()) == 'l')
	{
		m_currToken.suffix = TokenSuffix::SUFFIX_L;
		if (tolower(ReadChar() == 'l'))
		{
			m_currToken.suffix = TokenSuffix::SUFFIX_LL;
			ReadChar();
		}
	}
}

bool Lexer::IsNullIntLiteral(size_t start, size_t end)
{
	return start - end == 0;
}

void Lexer::SeekDelimeter()
{
	while (!IsDelimeter(PeekChar()))
	{
		ReadChar();
	}
}

void Lexer::ParseIntWithBase(int base)
{
	size_t start = m_buf.pos;
	auto digit = GetDigitFromHex(PeekChar());
	while (digit)
	{
		if (digit >= base)
		{
			std::string errorMsg = " Digit " + std::to_string(*GetDigitFromHex(PeekChar())) + " out of range for base " + std::to_string(base);
			SeekDelimeter();
			Err(errorMsg, m_loc);
			m_currToken.kind = TokenKind::TOKEN_ERROR;
			return;
		}
		digit = GetDigitFromHex(ReadChar());
	}
	size_t end = m_buf.pos;
	GetSuffix();
	if (!IsDelimeter(PeekChar()))
	{
		SeekDelimeter();
		Err("Undefined number in interger literal", m_currToken.loc);
		m_currToken.kind = TokenKind::TOKEN_ERROR;
	}
	else
	{
		if (IsNullIntLiteral(start, end))
		{
			if (base == DEC_BASE)
			{
				m_currToken.int_number = 0;
				m_currToken.kind = TokenKind::TOKEN_INT;
			}
			else
			{
				Err("Empty integer literal", m_currToken.loc);
				m_currToken.kind = TokenKind::TOKEN_ERROR;
			}
		}
		else
		{
			m_currToken.int_number = StrToLL(start, end, base);
			m_currToken.kind = TokenKind::TOKEN_INT;
		}
	}

}

void Lexer::ScanFloat()
{
	size_t start = m_buf.pos;
	m_currToken.loc = m_loc;
	int digitCount = 0;
	while (isdigit(PeekChar())) {
		digitCount++;
		ReadChar();
	}
	if (digitCount == 0)
	{
		m_currToken.kind = TokenKind::TOKEN_ERROR;
		Err("Invalid float representation", m_currToken.loc);
	}
	if (PeekChar() == '.') {
		ReadChar();
	}
	while (isdigit(PeekChar())) {
		ReadChar();
	}

	if (tolower(PeekChar()) == 'e') {
		ReadChar();
		if (PeekChar() == '+' || PeekChar() == '-') {
			ReadChar();
		}
		if (!isdigit(PeekChar())) {
			Err("Expected digit after float literal exponent", m_currToken.loc);
			m_currToken.kind = TokenKind::TOKEN_ERROR;
		}
		while (isdigit(PeekChar())) {
			ReadChar();
		}
	}
	if (tolower(PeekChar()) == 'd') {
		m_currToken.suffix = TokenSuffix::SUFFIX_D;
		ReadChar();
	}
	if (!IsDelimeter(PeekChar()))
	{
		while (!IsDelimeter(PeekChar()))
		{
			ReadChar();
		}
		if (m_currToken.kind == TokenKind::TOKEN_ERROR)
		{
			return;
		}
		Err("Cannot identify correct double number", m_currToken.loc);
		m_currToken.kind = TokenKind::TOKEN_ERROR;
		return;
	}
	try
	{
		std::string num = m_buf.data.substr(start, m_buf.pos - start);
		m_currToken.real_number = std::stod(num, nullptr);
		m_currToken.kind = TokenKind::TOKEN_FLOAT;
	}
	catch (const std::out_of_range&)
	{
		Err("Overflow float literal", m_currToken.loc);
		m_currToken.kind = TokenKind::TOKEN_ERROR;
	}
}

void Lexer::ScanString()
{
	ReadChar();
	size_t start = m_buf.pos;
	std::string value;
	while (PeekChar() != '\"' && PeekChar() != '\0')
	{
		if (PeekChar() == '\n')
		{
			Err("String literal cannot contain new line", m_loc);
			m_currToken.kind = TokenKind::TOKEN_ERROR;
			return;
		}
		else if (PeekChar() == '\\')
		{
			ReadChar();
			if (PeekChar() == 'x')
			{
				auto optVal = ScanHexToIntInStrLiteral();
				if (!optVal)
				{
					while (PeekChar() != '"' && PeekChar() != '\n' && PeekChar() != 0)
						ReadChar();
					if (PeekChar() == 0)
					{
						Err("Missing closing quote in string literal", m_loc);
					}
					else if (PeekChar() == '\n')
					{
						Err("String literal cannot contain new line", m_loc);
					}

					m_currToken.kind = TokenKind::TOKEN_ERROR;
					ReadChar();
					return;
				}
				value.push_back(*optVal);
			}
			else
			{
				auto optVal = GetCharFromEscapeSeq(PeekChar());
				if (!optVal)
				{
					Err("Unrecognized character escape sequence in string literal", m_currToken.loc);
					m_currToken.kind = TokenKind::TOKEN_ERROR;
					m_buf.pos += 2;
					m_loc.lineOffset += 2;
					return;
				}
				value.push_back(*optVal);
				ReadChar();
			}
		}
		else
		{
			value.push_back(PeekChar());
			ReadChar();
		}
	}
	if (PeekChar() == '\0')
	{
		Err("Unexpected end of file in string literal", m_loc);
		m_currToken.kind = TokenKind::TOKEN_ERROR;
		return;
	}
	ReadChar();
	m_currToken.kind = TokenKind::TOKEN_STR;
	m_currToken.string = value;
}

void Lexer::ScanName()
{
	size_t startPos = m_buf.pos;
	char ch = PeekChar();
	m_currToken.loc = m_loc;
	int chCount = 1;
	while (isalnum(ch) || ch == '_')
	{
		ch = ReadChar();
		chCount++;
	}
	if (chCount >= 64)
	{
		m_currToken.kind = TokenKind::TOKEN_ERROR;
		Err("Too long identifier", m_currToken.loc);
		ReadChar();
		return;
	}
	m_currToken.name = m_buf.data.substr(startPos, m_buf.pos - startPos);
	m_currToken.kind = IsKeyword(m_currToken.name) ? GetKeyword(m_currToken.name) : TokenKing::TOKEN_NAME;
}

long long Lexer::StrToLL(size_t startPos, size_t endPos, int base)
{
	std::string number = m_buf.data.substr(startPos, endPos - startPos);
	return std::stoll(number, nullptr, base);
}


void Lexer::Err(const std::string& description, const LexemeLocation& loc)
{
	std::cout << "Err: " << description << '\n';
	std::cout << "At line: " << loc.lineNum << '\n';
	std::cout << "At position: " << loc.lineOffset << '\n';
}


