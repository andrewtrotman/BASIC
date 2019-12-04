/*
	PARSE_TREE.H
	------------
*/
#pragma once

#include <memory>
#include <iostream>

#include "parser.h"

namespace BASIC
	{
	class parse_tree
		{
		private:
			class node
				{
				public:
					enum node_type
						{
						OPERATOR,
						NUMBER,
						SYMBOL
						};
				public:
					std::shared_ptr<node> left;
					std::shared_ptr<node> right;
					node_type type;
					const char *operation;		// OPERATOR: is a reserved_word or a symbol
					double number;					// NUMBER: is a float
					std::string symbol;			// SYMBOL: is a variable or other symbol (function call?)
					
				public:
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

		public:
			class error_syntax 				{ /* Nothing */ };
			class error_division_by_zero 	{ /* Nothing */ };
			class error_runtime 				{ /* Nothing */ };

		protected:
			parser parser;
			
		protected:
			std::shared_ptr<node> build(void);
			std::shared_ptr<node> build_tree(std::shared_ptr<node> left = nullptr);
			std::shared_ptr<node> build_operator(void);
			std::shared_ptr<node> build_operand(void);

		friend std::ostream &operator<<(std::ostream &stream, const std::shared_ptr<parse_tree::node> root);

		public:
			parse_tree()
				{
				/* Nothing */
				}

			std::shared_ptr<node> build(const std::string &string);
			double evaluate(std::shared_ptr<parse_tree::node> root);
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
		else if (root->type == parse_tree::node::OPERATOR)
			stream << root->operation;

		if (root->right != nullptr)
			stream << root->right;

		if (root->left != nullptr || root->right != nullptr)
			std::cout << ")";

		return stream;
		}
	}
