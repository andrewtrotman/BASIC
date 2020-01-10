/*
	EXECUTIVE.H
	-----------
*/
#pragma once

#include <map>
#include <vector>

#include "symbol.h"
#include "symbol_table.h"

namespace BASIC
	{
	class parse_tree_node;
	/*
		CLASS EXECUTIVE
		---------------
	*/
	class executive
		{
		public:
			typedef std::map<size_t, std::shared_ptr<parse_tree_node>> program;

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
			std::shared_ptr<parse_tree_node> data_pointer;
			program::const_iterator data_pointer_line;

		protected:
			symbol evaluate_read_one(void);
			bool step(int64_t which_for_loop);
			bool evaluate_print_part(const std::shared_ptr<parse_tree_node> &root);
			void evaluate_input_part(const std::shared_ptr<parse_tree_node> &root);

		public:
			executive() :
				symbol_table(*this)
				{
				data_pointer = nullptr;
				}

			void evaluate(const program &parsed_code, size_t start_line = 0);
			symbol evaluate_expression(const std::shared_ptr<parse_tree_node> &root);

			void evaluate_print_comma(const std::shared_ptr<parse_tree_node> &root) {}
			void evaluate_print_semicolon(const std::shared_ptr<parse_tree_node> &root) {}

			void evaluate_data(const std::shared_ptr<parse_tree_node> &root);
			void evaluate_restore(const std::shared_ptr<parse_tree_node> &root);
			void evaluate_read(const std::shared_ptr<parse_tree_node> &root);
			void evaluate_dim(const std::shared_ptr<parse_tree_node> &root);
			void evaluate_next(const std::shared_ptr<parse_tree_node> &root);
			void evaluate_for(const std::shared_ptr<parse_tree_node> &root);
			void evaluate_end(const std::shared_ptr<parse_tree_node> &root);
			void evaluate_goto(const std::shared_ptr<parse_tree_node> &root);
			void evaluate_gosub(const std::shared_ptr<parse_tree_node> &root);
			void evaluate_return(const std::shared_ptr<parse_tree_node> &root);
			void evaluate_rem(const std::shared_ptr<parse_tree_node> &root);
			void evaluate_pop(const std::shared_ptr<parse_tree_node> &root);
			void evaluate_if(const std::shared_ptr<parse_tree_node> &root);
			void evaluate_input(const std::shared_ptr<parse_tree_node> &root);
			void evaluate_print(const std::shared_ptr<parse_tree_node> &root);
			void evaluate_let(const std::shared_ptr<parse_tree_node> &root);

			symbol evaluate_plus(const std::shared_ptr<parse_tree_node> &root);
			symbol evaluate_minus(const std::shared_ptr<parse_tree_node> &root);
			symbol evaluate_multiply(const std::shared_ptr<parse_tree_node> &root);
			symbol evaluate_divide(const std::shared_ptr<parse_tree_node> &root);
			symbol evaluate_power(const std::shared_ptr<parse_tree_node> &root);
			symbol evaluate_unary_plus(const std::shared_ptr<parse_tree_node> &root);
			symbol evaluate_unary_minus(const std::shared_ptr<parse_tree_node> &root);
			symbol evaluate_equals_equals(const std::shared_ptr<parse_tree_node> &root);
			symbol evaluate_not_equals(const std::shared_ptr<parse_tree_node> &root);
			symbol evaluate_greater_than(const std::shared_ptr<parse_tree_node> &root);
			symbol evaluate_less_than(const std::shared_ptr<parse_tree_node> &root);
			symbol evaluate_greater_than_equals(const std::shared_ptr<parse_tree_node> &root);
			symbol evaluate_less_than_equals(const std::shared_ptr<parse_tree_node> &root);

			void evaluate(const std::shared_ptr<parse_tree_node> &root);
		};
	}
