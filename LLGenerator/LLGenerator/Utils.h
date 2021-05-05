#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <vector>

const std::string EMPTY_RULE = "e";
const std::string RULE_SEPARATOR = "|";
const std::string END_SEQUENCE = "_|_";
const std::string RULE_DEFINITION_SEPARATOR = "->";
const std::string GUIDE_SET_SEPARATOR = "/";


struct Rules
{
	std::string left;
	std::vector<std::string> right;
	std::vector<std::string> guideSet;
};

typedef std::vector<Rules> Grammar;
typedef std::pair<std::string, bool> HasTransitionPair;
typedef	std::pair<std::string, std::vector<HasTransitionPair>> Transition;




void AddRule(Grammar& grammar, const std::string& left, const Rules& right);

Grammar CreateGrammar(const std::string grammarStr);

std::string ReadGrammarFromFile(const std::string inputFile);

