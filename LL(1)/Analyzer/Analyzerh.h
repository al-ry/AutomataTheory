#pragma once

#include "../LLGenerator/Utils.h"
#include "../../Lexer/Lexer/lex.h"


void AnalyzeTable(Table table, std::vector<std::string> inputSequence);
void AnalyzeTable(Table table, Lexer& lexer);

