/*
	RESERVED_WORD.CPP
	-----------------
*/
#include "reserved_word.h"

namespace BASIC
	{
	/*
		Reserved words
	*/
	const char *reserved_word::THEN = "then";
	const char *reserved_word::GOTO = "goto";
	const char *reserved_word::IF = "if";
	const char *reserved_word::INPUT = "input";
	const char *reserved_word::PRINT = "print";
	const char *reserved_word::LET = "let";
	const char *reserved_word::EQUALS = "=";
	const char *reserved_word::NOT_EQUALS = "<>";
	const char *reserved_word::LESS_THAN = "<";
	const char *reserved_word::GREATER_THAN = ">";
	const char *reserved_word::LESS_THAN_EQUALS = "<=";
	const char *reserved_word::GREATER_THAN_EQUALS = ">=";
	const char *reserved_word::COMMA = ",";
	const char *reserved_word::QUESTIONMARK = "?";
	const char *reserved_word::SEMICOLON = ";";
	const char *reserved_word::OPEN_BRACKET = "(";
	const char *reserved_word::CLOSE_BRACKET = ")";
	const char *reserved_word::PLUS = "+";
	const char *reserved_word::MINUS = "-";
	const char *reserved_word::MULTIPLY = "*";
	const char *reserved_word::DIVIDE = "/";
	const char *reserved_word::POWER = "^";
	const char *reserved_word::UNARY_PLUS = " +";
	const char *reserved_word::UNARY_MINUS = " -";

	/*
		Translation matrix
	*/
	const std::unordered_map<std::string, const char *> reserved_word::all_reserved_words =
		{
		{std::string(COMMA), COMMA},
		{std::string(SEMICOLON), SEMICOLON},
		{std::string(QUESTIONMARK), QUESTIONMARK},
		{std::string(LET), LET},
		{std::string(IF), IF},
		{std::string(THEN), THEN},
		{std::string(GOTO), GOTO},
		{std::string(INPUT), INPUT},
		{std::string(PRINT), PRINT},
		{std::string(EQUALS), EQUALS},
		{std::string(NOT_EQUALS), NOT_EQUALS},
		{std::string(LESS_THAN), LESS_THAN},
		{std::string(GREATER_THAN), GREATER_THAN},
		{std::string(LESS_THAN_EQUALS), LESS_THAN_EQUALS},
		{std::string(GREATER_THAN_EQUALS), GREATER_THAN_EQUALS},
		{std::string(OPEN_BRACKET), OPEN_BRACKET},
		{std::string(CLOSE_BRACKET), CLOSE_BRACKET},
		{std::string(PLUS), PLUS},
		{std::string(MINUS), MINUS},
		{std::string(MULTIPLY), MULTIPLY},
		{std::string(DIVIDE), DIVIDE},
		{std::string(POWER), POWER},
		};

	/*
		operator precidence matrix
	*/
	const std::unordered_map<const char *, size_t> reserved_word::all_precidence =
		{
		{CLOSE_BRACKET, 1},
		{GREATER_THAN_EQUALS, 5},
		{LESS_THAN_EQUALS, 5},
		{GREATER_THAN, 6},
		{LESS_THAN, 6},
		{NOT_EQUALS, 6},
		{EQUALS, 6},
		{PLUS, 7},
		{MINUS, 7},
		{MULTIPLY, 8},
		{DIVIDE, 8},
		{POWER, 9},
		{OPEN_BRACKET, 10},
		};

	/*
		operator matrix
	*/
	const std::unordered_map<const char *, bool> reserved_word::all_operators =
		{
		{EQUALS, true},
		{NOT_EQUALS, true},
		{LESS_THAN, true},
		{GREATER_THAN, true},
		{LESS_THAN_EQUALS, true},
		{GREATER_THAN_EQUALS, true},
		{PLUS, true},
		{MINUS, true},
		{MULTIPLY, true},
		{DIVIDE, true},
		{POWER, true},
		};

	/*
		RESERVED_WORD::TRANSLATE()
		--------------------------
	*/
	const char *reserved_word::translate(const char *token)
		{
		auto got = all_reserved_words.find(std::string(token));

		if (got == all_reserved_words.end())
			return token;
		else
			return got->second;
		}

	/*
		RESERVED_WORD::PRECIDENCE()
		--------------------------
	*/
	size_t reserved_word::precidence(const char *token)
		{
		auto got = all_precidence.find(token);

		if (got == all_precidence.end())
			return 0;
		else
			return got->second;
		}

	/*
		RESERVED_WORD::ISOPERTOR()
		--------------------------
	*/
	bool reserved_word::isoperator(const char *token)
		{
		auto got = all_operators.find(token);

		if (got == all_operators.end())
			return false;
		else
			return true;
		}
	}
