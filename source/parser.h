/*
	PARSER.H
	--------
*/
#pragma once

#include <array>
#include <string>

namespace BASIC
	{
	class parser
		{
		private:
			std::array<char, 1024> token_buffer;			// the buffer holding the current reserved word
			std::array<char, 1024> next_token_buffer;

			const char *program;									// the line being parsed
			const char *where;									// where in the parsing we currently are
			const char *was;										// where in the parsing we were at the start of this token
			const char *end;										// end of the current text being parsed

		public:
			/*
				PARSER::PARSER()
				----------------
				constructor
			*/
			parser()
				{
				/* Nothing */
				}

			/*
				PARSER::SET_DOCUMENT()
				----------------------
				Pass the thing to parse
			*/
			void set_document(const std::string &program)
				{
				this->program = program.data();
				where = program.data();
				end = where + program.length();

				token_buffer[0] = '\0';
				next_token_buffer[0] = '\0';
				get_next_token();			// prime the token_buffer
				}

			/*
				PARSER::GET_NEXT_TOKEN()
				------------------------
				return the next parsed token
			*/
			const char *get_next_token(void);

			/*
				PARSER::PEEK_NEXT_TOKEN()
				-------------------------
				return the next parsed token
			*/
			const char *peek_next_token(void)
				{
				return next_token_buffer.data();
				}

			 /*
				 PARSER::GET_CURSOR()
				 --------------------
				 return the current cursor position
			 */
			 const char *get_cursor(void) const
				{
				return was;
				}

			/*
				PARSER::SET_CURSOR()
				--------------------
				set the current cursor position
			*/
			void set_cursor(const char *to)
				{
				where = to;
				token_buffer[0] = '\0';
				next_token_buffer[0] = '\0';
				get_next_token();			// prime the token_buffer
				}

			/*
				PARSER::COMMA_SEPERATED()
				-------------------------
				return the next comma seperated token (for use in DATA statements)
			*/
			std::string comma_seperated(const char **from);
		};
	}
