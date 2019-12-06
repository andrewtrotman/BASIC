/*
	PARSE_TREE.H
	------------
*/
#pragma once

#include <memory>
#include <iostream>

#include "parser.h"
#include "symbol_table.h"
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
			symbol_table symbol_table;
			
		protected:
			std::shared_ptr<node> build(void);
			std::shared_ptr<node> build_tree(std::shared_ptr<node> left = nullptr);
			std::shared_ptr<node> build_operator(void);
			std::shared_ptr<node> build_operand(void);

			std::shared_ptr<node> parse_print(void);
			std::shared_ptr<node> parse_let(void);

			bool evaluate_print(std::shared_ptr<node> root);

		friend std::ostream &operator<<(std::ostream &stream, const std::shared_ptr<parse_tree::node> root);

		public:
			parse_tree()
				{
				/* Nothing */
				}

			std::shared_ptr<node> build(const std::string &string);
			symbol evaluate(std::shared_ptr<parse_tree::node> root);
		};

	/*
		OPERATOR<<()
		------------
	*/
	inline std::ostream &operator<<(std::ostream &stream, const std::shared_ptr<parse_tree::node> root)
		{
		if (root->left != nullptr || root->right != nullptr)
			std::cout << "(";

		if (root->left != nullptr)
			stream << root->left;

		if (root->type == parse_tree::node::SYMBOL)
			stream << root->symbol;
		else if (root->type == parse_tree::node::NUMBER)
			stream << root->number;
		else if (root->type == parse_tree::node::STRING)
			stream << '"' << root->string << '"';
		else if (root->type == parse_tree::node::OPERATOR)
			stream << root->operation;
		else if (root->type == parse_tree::node::COMMAND)
			{
			if (root->operation == reserved_word::PRINT)
				stream << "PRINT ";
			if (root->operation == reserved_word::EQUALS)
				stream << " = ";
			}

		if (root->right != nullptr)
			stream << root->right;

		if (root->left != nullptr || root->right != nullptr)
			std::cout << ")";

		return stream;
		}
	}
