/*
	PARSE_TREE.H
	------------
*/
#pragma once

#include <memory>
#include <iostream>

#include "parser.h"
#include "reserved_word.h"

namespace BASIC
	{
	/*
		CLASS PARSE_TREE
		----------------
	*/
	class parse_tree
		{
		public:
			/*
				CLASS PARSE_TREE::NODE
				----------------------
			*/
			class node
				{
				public:
					/*
						ENUM PARSE_TREE::NODE::NODE_TYPE
						--------------------------------
					*/
					enum node_type
						{
						OPERATOR,
						NUMBER,
						STRING,
						SYMBOL,
						COMMAND
						};


				public:
					std::shared_ptr<node> left;
					std::shared_ptr<node> right;
					node_type type;
					const char *operation;		// OPERATOR: is a reserved_word operator or command
					double number;					// NUMBER: is a float
					std::string symbol;			// SYMBOL: is a variable or other symbol (function call?)
					std::string string;			// STRING: is a string
					
				public:
					/*
						PARSE_TREE::NODE::NODE()
						------------------------
					*/
					node() :
						left(nullptr),
						right(nullptr),
						type(SYMBOL),
						operation(nullptr),
						number(0),
						symbol("")
						{
						/* Nothing */
						}
				};

		protected:
			parser parser;
			
		protected:
			std::shared_ptr<node> build(void);
			std::shared_ptr<node> build_command(void);
			std::shared_ptr<node> build_tree(std::shared_ptr<node> left = nullptr);
			std::shared_ptr<node> build_operator(void);
			std::shared_ptr<node> build_operand(void);
			std::shared_ptr<node> build_variable(const std::string &name, bool can_be_parameterised = true);

			std::shared_ptr<node> parse_dim(void);
			std::shared_ptr<node> parse_next(void);
			std::shared_ptr<node> parse_for(void);
			std::shared_ptr<node> parse_end(void);
			std::shared_ptr<node> parse_goto(void);
			std::shared_ptr<node> parse_gosub(void);
			std::shared_ptr<node> parse_return(void);
			std::shared_ptr<node> parse_pop(void);
			std::shared_ptr<node> parse_rem(void);
			std::shared_ptr<node> parse_if(void);
			std::shared_ptr<node> parse_input(void);
			std::shared_ptr<node> parse_print(void);
			std::shared_ptr<node> parse_let(bool can_be_parameterised = true);

			std::shared_ptr<node> parse_parameterless_statement(const char *command);

		friend std::ostream &operator<<(std::ostream &stream, const std::shared_ptr<parse_tree::node> root);

		public:
			parse_tree()
				{
				/* Nothing */
				}

			std::shared_ptr<node> build(const std::string &string);
		};

	/*
		OPERATOR<<()
		------------
	*/
	inline std::ostream &operator<<(std::ostream &stream, const std::shared_ptr<parse_tree::node> root);

	}
