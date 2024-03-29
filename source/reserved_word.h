/*
	RESERVED_WORD.H
	---------------
*/
#pragma once

#include <unordered_map>
#include <string>

#include "executive.h"

namespace BASIC
	{
	/*
		CLASS RESERVED_WORD
		-------------------
	*/
	class reserved_word
		{
		public:
			/*
				List of known reserved words
			*/
			static const char *DATA;
			static const char *READ;
			static const char *RESTORE;
			static const char *FOR;
			static const char *DIM;
			static const char *TO;
			static const char *STEP;
			static const char *NEXT;
			static const char *END;
			static const char *IF;
			static const char *THEN;
			static const char *GOTO;
			static const char *GOSUB;
			static const char *RETURN;
			static const char *POP;
			static const char *REM;
			static const char *INPUT;
			static const char *PRINT;
			static const char *LET;
			static const char *COMMA;
			static const char *SEMICOLON;
			static const char *QUESTIONMARK;
			static const char *EQUALS;
			static const char *NOT_EQUALS;
			static const char *NOT_EQUALS_ALT;
			static const char *LESS_THAN;
			static const char *GREATER_THAN;
			static const char *LESS_THAN_EQUALS;
			static const char *EQUALS_LESS_THAN;
			static const char *GREATER_THAN_EQUALS;
			static const char *EQUALS_GREATER_THAN;
			static const char *OPEN_BRACKET;
			static const char *CLOSE_BRACKET;
			static const char *PLUS;
			static const char *MINUS;
			static const char *MULTIPLY;
			static const char *DIVIDE;
			static const char *POWER;
			static const char *UNARY_PLUS;
			static const char *UNARY_MINUS;

			/*
				Map from a string to a reserved word
			*/
			static const std::unordered_map<std::string, const char *> all_reserved_words;

			/*
				Map from an operator to its precidence (higher is higher precidence)
			*/
			static const std::unordered_map<const char *, size_t> all_precidence;
			static const std::unordered_map<decltype(&executive::evaluate_plus), size_t> all_precidence_functions;

			/*
				Map from a symbol to an operator
			*/
			static const std::unordered_map<const char *, decltype(&executive::evaluate_plus) > all_operators;

		public:
			/*
				RESERVED_WORD::TRANSLATE()
				--------------------------
				Take a token and convert into a reserved word if one exists
			*/
			static const char *translate(const char *token);

			/*
				RESERVED_WORD::PRECIDENCE()
				--------------------------
				Return the precidence of the given operator
			*/
			static size_t precidence(const char *token);

			/*
				RESERVED_WORD::PRECIDENCE()
				--------------------------
				Return the precidence of the given operator
			*/
			static size_t precidence(decltype(&executive::evaluate_plus));

			/*
				RESERVED_WORD::ISOPERATOR()
				---------------------------
				Return true if the symbol is an arithmetic operator
			*/
			static decltype(&executive::evaluate_plus) isoperator(const char *token);
		};
	}
