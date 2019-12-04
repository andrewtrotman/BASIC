/*
	PARSER.CPP
	----------
*/
#include <string.h>

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
		size_t length = ::strlen(next_token_buffer.data());
		std::copy(next_token_buffer.data(), next_token_buffer.data() + length, token_buffer.data());
		token_buffer[length] = '\0';

		if (where >= end)
			next_token_buffer[0] = '\0';
		else
			{
			while (isspace(*where))
				where++;

			const char *token_start = where;
			if (isalpha(*where))
				{
				where++;
				while (where < end && isalpha(*where))
					where++;
				}
			else if (isdigit(*where))
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
			else
				where++;

			size_t length = where - token_start < next_token_buffer.size() ? where - token_start : next_token_buffer.size();
			std::copy(token_start, token_start + length, next_token_buffer.data());
			next_token_buffer[length] = '\0';
			}

		return token_buffer.data();
		}
		
	/*
		PARSER::PEEK_NEXT_TOKEN()
		-------------------------
	*/
	const char *parser::peek_next_token(void)
		{
		return next_token_buffer.data();
		}
	}
