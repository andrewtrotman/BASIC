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
			
		protected:
			void evaluate_if(std::shared_ptr<parse_tree::node> root);
			void evaluate_input(std::shared_ptr<parse_tree::node> root);
			bool evaluate_print(std::shared_ptr<parse_tree::node> root);

			symbol evaluate(std::shared_ptr<parse_tree::node> root);

		public:
			executive()
				{
				/* Nothing */
				}
			void evaluate(const program &parsed_code, size_t start_line = 0);
		};
	}
