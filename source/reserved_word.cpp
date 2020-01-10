/*
	RESERVED_WORD.CPP
	-----------------
*/
#include "reserved_word.h"

using func = BASIC::symbol (BASIC::executive::*)(const std::shared_ptr<BASIC::parse_tree_node> &);

namespace std
	{
	template<> struct std::hash<func>
		{
		size_t operator()(const func &s) const noexcept
			{
			return (size_t)&s;
			}
		};
	}


namespace BASIC
	{
	/*
		Reserved words
	*/
	const char *reserved_word::READ = "read";
	const char *reserved_word::DATA = "data";
	const char *reserved_word::RESTORE = "restore";
	const char *reserved_word::DIM = "dim";
	const char *reserved_word::FOR = "for";
	const char *reserved_word::TO = "to";
	const char *reserved_word::STEP = "step";
	const char *reserved_word::NEXT = "next";
	const char *reserved_word::THEN = "then";
	const char *reserved_word::GOTO = "goto";
	const char *reserved_word::GOSUB = "gosub";
	const char *reserved_word::RETURN = "return";
	const char *reserved_word::POP = "pop";
	const char *reserved_word::REM = "rem";
	const char *reserved_word::IF = "if";
	const char *reserved_word::END = "end";
	const char *reserved_word::INPUT = "input";
	const char *reserved_word::PRINT = "print";
	const char *reserved_word::LET = "let";
	const char *reserved_word::EQUALS = "=";
	const char *reserved_word::NOT_EQUALS = "<>";
	const char *reserved_word::NOT_EQUALS_ALT = "><";
	const char *reserved_word::LESS_THAN = "<";
	const char *reserved_word::GREATER_THAN = ">";
	const char *reserved_word::LESS_THAN_EQUALS = "<=";
	const char *reserved_word::EQUALS_LESS_THAN = "=<";
	const char *reserved_word::GREATER_THAN_EQUALS = ">=";
	const char *reserved_word::EQUALS_GREATER_THAN = "=>";
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
		{std::string(READ), READ},
		{std::string(DATA), DATA},
		{std::string(RESTORE), RESTORE},
		{std::string(LET), LET},
		{std::string(END), END},
		{std::string(DIM), DIM},
		{std::string(FOR), FOR},
		{std::string(TO), TO},
		{std::string(STEP), STEP},
		{std::string(NEXT), NEXT},
		{std::string(IF), IF},
		{std::string(THEN), THEN},
		{std::string(GOTO), GOTO},
		{std::string(GOSUB), GOSUB},
		{std::string(RETURN), RETURN},
		{std::string(POP), POP},
		{std::string(REM), REM},
		{std::string(INPUT), INPUT},
		{std::string(PRINT), PRINT},
		{std::string(EQUALS), EQUALS},
		{std::string(NOT_EQUALS), NOT_EQUALS},
		{std::string(NOT_EQUALS_ALT), NOT_EQUALS},
		{std::string(LESS_THAN), LESS_THAN},
		{std::string(GREATER_THAN), GREATER_THAN},
		{std::string(LESS_THAN_EQUALS), LESS_THAN_EQUALS},
		{std::string(EQUALS_LESS_THAN), LESS_THAN_EQUALS},
		{std::string(GREATER_THAN_EQUALS), GREATER_THAN_EQUALS},
		{std::string(EQUALS_GREATER_THAN), GREATER_THAN_EQUALS},
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
		operator precidence matrix
	*/
	const std::unordered_map<func, size_t> reserved_word::all_precidence_functions =
		{
		{&executive::evaluate_greater_than_equals, 5},
		{&executive::evaluate_less_than_equals, 5},
		{&executive::evaluate_greater_than, 6},
		{&executive::evaluate_less_than, 6},
		{&executive::evaluate_not_equals, 6},
		{&executive::evaluate_equals_equals, 6},
		{&executive::evaluate_plus, 7},
		{&executive::evaluate_minus, 7},
		{&executive::evaluate_multiply, 8},
		{&executive::evaluate_divide, 8},
		{&executive::evaluate_power, 9},
		};


	/*
		operator matrix
	*/
	const std::unordered_map<const char *, decltype(&executive::evaluate_plus) > reserved_word::all_operators =
		{
		{EQUALS, &executive::evaluate_equals_equals},
		{NOT_EQUALS, &executive::evaluate_not_equals},
		{LESS_THAN, &executive::evaluate_less_than},
		{GREATER_THAN, &executive::evaluate_greater_than},
		{LESS_THAN_EQUALS, &executive::evaluate_less_than_equals},
		{GREATER_THAN_EQUALS, &executive::evaluate_greater_than_equals},
		{PLUS, &executive::evaluate_plus},
		{MINUS, &executive::evaluate_minus},
		{MULTIPLY, &executive::evaluate_multiply},
		{DIVIDE, &executive::evaluate_divide},
		{POWER, &executive::evaluate_power},
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
		RESERVED_WORD::PRECIDENCE()
		--------------------------
	*/
	size_t reserved_word::precidence(const decltype(&executive::evaluate_plus) token)
		{
		auto got = all_precidence_functions.find(token);

		if (got == all_precidence_functions.end())
			return 0;
		else
			return got->second;
		}

	/*
		RESERVED_WORD::ISOPERTOR()
		--------------------------
	*/
	decltype(&executive::evaluate_plus) reserved_word::isoperator(const char *token)
		{
		auto got = all_operators.find(token);

		if (got == all_operators.end())
			return nullptr;
		else
			return got->second;
		}
	}
