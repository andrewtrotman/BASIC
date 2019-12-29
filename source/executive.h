/*
	EXECUTIVE.H
	-----------
*/
#pragma once

#include <map>
#include <vector>

#include "symbol.h"
#include "symbol_table.h"
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
			class for_tuple
				{
				public:
					const std::string variable_name;
					program::const_iterator line;
					symbol &variable;
					double to;
					double step;

				public:
					for_tuple(const std::string &variable_name, symbol &variable, double to, double step, program::const_iterator line) :
						variable_name(variable_name),
						line(line),
						variable(variable),
						to(to),
						step(step)
						{
						/* Nothing */
						}
				};

		protected:
			symbol_table symbol_table;
			const program *parsed_code;
			program::const_iterator next_line;
			bool end;
			std::vector<for_tuple> for_stack;
			std::vector<program::const_iterator> gosub_stack;
			std::shared_ptr<parse_tree::node> data_pointer;
			program::const_iterator data_pointer_line;

		protected:
			symbol evaluate_read_one(void);
			bool step(int64_t which_for_loop);
			void evaluate_data(const std::shared_ptr<parse_tree::node> &root);
			void evaluate_restore(const std::shared_ptr<parse_tree::node> &root);
			void evaluate_read(const std::shared_ptr<parse_tree::node> &root);
			void evaluate_dim(const std::shared_ptr<parse_tree::node> &root);
			void evaluate_next(const std::shared_ptr<parse_tree::node> &root);
			void evaluate_for(const std::shared_ptr<parse_tree::node> &root);
			void evaluate_end(const std::shared_ptr<parse_tree::node> &root);
			void evaluate_goto(const std::shared_ptr<parse_tree::node> &root);
			void evaluate_gosub(const std::shared_ptr<parse_tree::node> &root);
			void evaluate_return(const std::shared_ptr<parse_tree::node> &root);
			void evaluate_rem(const std::shared_ptr<parse_tree::node> &root);
			void evaluate_pop(const std::shared_ptr<parse_tree::node> &root);
			void evaluate_if(const std::shared_ptr<parse_tree::node> &root);
			void evaluate_input(const std::shared_ptr<parse_tree::node> &root);
			bool evaluate_print(const std::shared_ptr<parse_tree::node> &root);

			void evaluate_command(const std::shared_ptr<parse_tree::node> &root);
			void evaluate(const std::shared_ptr<parse_tree::node> &root);

		public:
			executive() :
				symbol_table(*this)
				{
				data_pointer = nullptr;
				}
				
			void evaluate(const program &parsed_code, size_t start_line = 0);
			symbol evaluate_expression(const std::shared_ptr<parse_tree::node> &root);
		};
	}
