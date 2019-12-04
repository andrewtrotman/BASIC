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
	const char *reserved_word::PRINT = "print";
	const char *reserved_word::LET = "let";

	const char *reserved_word::EQUALS = "=";
	const char *reserved_word::OPEN_BRACKET = "(";
	const char *reserved_word::CLOSE_BRACKET = ")";
	const char *reserved_word::PLUS = "+";
	const char *reserved_word::MINUS = "-";
	const char *reserved_word::MULTIPLY = "*";
	const char *reserved_word::DIVIDE = "/";
	const char *reserved_word::POWER = "^";

	/*
		Translation matrix
	*/
	const std::unordered_map<std::string, const char *> reserved_word::all_reserved_words =
		{
		{std::string(LET), LET},
		{std::string(PRINT), PRINT},
		{std::string(EQUALS), EQUALS},
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
		{PLUS, 2},
		{MINUS, 2},
		{MULTIPLY, 3},
		{DIVIDE, 3},
		{POWER, 4},
		{OPEN_BRACKET, 5},
		{EQUALS, 10},
		};

	/*
		operator matrix
	*/
	const std::unordered_map<const char *, bool> reserved_word::all_operators =
		{
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
