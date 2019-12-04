/*
	RESERVED_WORD.H
	---------------
*/
#pragma once

#include <unordered_map>
#include <string>

namespace BASIC
	{
	class reserved_word
		{
		public:
			/*
				List of known reserved words
			*/
			static const char *PRINT;
			static const char *EQUALS;
			static const char *OPEN_BRACKET;
			static const char *CLOSE_BRACKET;
			static const char *PLUS;
			static const char *MINUS;
			static const char *MULTIPLY;
			static const char *DIVIDE;
			static const char *POWER;

			/*
				Map from a string to a reserved word
			*/
			static const std::unordered_map<std::string, const char *> all_reserved_words;

			/*
				Map from an operator to its precidence (higher is higher precidence)
			*/
			static const std::unordered_map<const char *, size_t> all_precidence;

			/*
				Map from a symbol to an operator
			*/
			static const std::unordered_map<const char *, bool> all_operators;

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
				RESERVED_WORD::ISOPERATOR()
				---------------------------
				Return true if the symbol is an arithmetic operator
			*/
			static bool isoperator(const char *token);
		};
	}
