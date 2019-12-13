/*
	EXECUTIVE.H
	-----------
*/
#pragma once

#include <map>
#include "parse_tree.h"

namespace BASIC
	{
	/*
		CLASS EXECUTIVE
		---------------
	*/
	class executive
		{
		public:
			typedef std::map<size_t, std::shared_ptr<BASIC::parse_tree::node>> program;
			
		protected:
			symbol_table symbol_table;
			const program *parsed_code;
			program::const_iterator next_line;
			bool end;

		protected:
			void evaluate_end(const std::shared_ptr<parse_tree::node> &root);
			void evaluate_goto(const std::shared_ptr<parse_tree::node> &root);
			void evaluate_if(const std::shared_ptr<parse_tree::node> &root);
			void evaluate_input(const std::shared_ptr<parse_tree::node> &root);
			bool evaluate_print(const std::shared_ptr<parse_tree::node> &root);

			void evaluate_command(const std::shared_ptr<parse_tree::node> &root);
			symbol evaluate_expression(const std::shared_ptr<parse_tree::node> &root);
			void evaluate(const std::shared_ptr<parse_tree::node> &root);

		public:
			executive()
				{
				/* Nothing */
				}
			void evaluate(const program &parsed_code, size_t start_line = 0);
		};
	}
