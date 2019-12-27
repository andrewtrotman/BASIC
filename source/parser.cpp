/*
	PARSER.CPP
	----------
*/
#include <string.h>

#include <algorithm>

#include "parser.h"
#include "reserved_word.h"

namespace BASIC
	{
	/*
		PARSER::GET_NEXT_TOKEN()
		------------------------
	*/
	const char *parser::get_next_token(void)
		{
		was = where;
		size_t length = ::strlen(next_token_buffer.data());
		std::copy(next_token_buffer.data(), next_token_buffer.data() + length, token_buffer.data());
		token_buffer[length] = '\0';

		if (where >= end)
			next_token_buffer[0] = '\0';
		else
			{
			while (::isspace(*where))
				where++;

			const char *token_start = where;
			if (::isalpha(*where))
				{
				where++;
				while (where < end && isalpha(*where))
					where++;
				}
			else if (::isdigit(*where))
				{
				where++;
				while (where < end && (isdigit(*where) || *where == '.'))
					where++;
				}
			else if (*where == '"')
				{
				where++;
				while (where < end && *where != '"')
					where++;
				if (where < end)
					where++;		// include the close quote
				}
			else if (*where == '<')
				{
				where++;
				if (*where == '=')
					where++;
				else if (*where == '>')
					where++;
				}
			else if (*where == '>')
				{
				where++;
				if (*where == '=')
					where++;
				else if (*where == '<')
					where++;
				}
			else if (*where == '=')
				{
				where++;
				if (*where == '<')
					where++;
				else if (*where == '>')
					where++;
				}
			else
				where++;

			length = std::min(size_t(where - token_start), next_token_buffer.size());
			std::copy(token_start, token_start + length, next_token_buffer.data());
			next_token_buffer[length] = '\0';
			}

		return token_buffer.data();
		}

	/*
		PARSER::COMMA_SEPERATED()
		-------------------------
		from [in/out], where to parse from, and where we ended up
	*/
	std::string parser::comma_seperated(const char **from)
		{
		const char *where = *from;

		while (::isspace(*where))
			where++;

		auto start_of_token = where;

		if (*where == '"')
				{
				where++;
				while (*where != '\0' && *where != '"')
					where++;
				}
		else if (*where == ',')
			where++;
		else
			while (*where != '\0' && *where != ',')
				where++;

		*from = where;

		return std::string(start_of_token, *from - start_of_token);
		}
	}
