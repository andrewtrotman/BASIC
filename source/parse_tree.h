/*
	PARSE_TREE.H
	------------
*/
#pragma once

#include <memory>
#include <iostream>

#include "parser.h"
#include "reserved_word.h"
#include "parse_tree_node.h"

namespace BASIC
	{
	/*
		CLASS PARSE_TREE
		----------------
	*/
	class parse_tree
		{
		protected:
			parser parser;
			
		protected:
			std::shared_ptr<parse_tree_node> build(void);
			std::shared_ptr<parse_tree_node> build_command(void);
			std::shared_ptr<parse_tree_node> build_tree(std::shared_ptr<parse_tree_node> left = nullptr);
			std::shared_ptr<parse_tree_node> build_operator(void);
			std::shared_ptr<parse_tree_node> build_operand(void);
			std::shared_ptr<parse_tree_node> build_variable(const std::string &name, bool can_be_parameterised = true);

			std::shared_ptr<parse_tree_node> parse_read(void);
			std::shared_ptr<parse_tree_node> parse_data(void);
			std::shared_ptr<parse_tree_node> parse_restore(void);
			std::shared_ptr<parse_tree_node> parse_dim(void);
			std::shared_ptr<parse_tree_node> parse_next(void);
			std::shared_ptr<parse_tree_node> parse_for(void);
			std::shared_ptr<parse_tree_node> parse_end(void);
			std::shared_ptr<parse_tree_node> parse_goto(void);
			std::shared_ptr<parse_tree_node> parse_gosub(void);
			std::shared_ptr<parse_tree_node> parse_return(void);
			std::shared_ptr<parse_tree_node> parse_pop(void);
			std::shared_ptr<parse_tree_node> parse_rem(void);
			std::shared_ptr<parse_tree_node> parse_if(void);
			std::shared_ptr<parse_tree_node> parse_input(void);
			std::shared_ptr<parse_tree_node> parse_print(void);
			std::shared_ptr<parse_tree_node> parse_let(bool can_be_parameterised = true);

			std::shared_ptr<parse_tree_node> parse_parameterless_statement(void (executive::*statement)(const std::shared_ptr<parse_tree_node> &root));


		friend std::ostream &operator<<(std::ostream &stream, const std::shared_ptr<parse_tree_node> root);

		public:
			parse_tree()
				{
				/* Nothing */
				}

			std::shared_ptr<parse_tree_node> build(const std::string &string);
		};
	}
