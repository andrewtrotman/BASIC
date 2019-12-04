/*
	PARSE_TREE.CPP
	--------------
*/
#include <math.h>
#include <ctype.h>

#include "parser.h"
#include "parse_tree.h"
#include "reserved_word.h"

namespace BASIC
	{
	/*
		PARSE_TREE::BUILD_OPERAND()
		---------------------------
	*/
	std::shared_ptr<parse_tree::node> parse_tree::build_operand(void)
		{
		const char *token = reserved_word::translate(parser.get_next_token());

		if (token == reserved_word::OPEN_BRACKET)
			{
			std::shared_ptr<node> child = build();
			auto close = reserved_word::translate(parser.get_next_token());		//consume the close bracket.
			if (close != reserved_word::CLOSE_BRACKET)
				throw error_syntax();
			return child;
			}
		else if (::isdigit(*token))
			{
			std::shared_ptr<node> new_node(new node);
			new_node->type = node::NUMBER;
			new_node->number = atof(token);
			return new_node;
			}
		else if (::isalpha(*token))
			{
			std::shared_ptr<node> new_node(new node);
			new_node->type = node::SYMBOL;
			new_node->symbol = std::string(token);
			return new_node;
			}
		else
			throw error_syntax();
		}

	/*
		PARSE_TREE::BUILD_OPERATOR()
		----------------------------
	*/
	std::shared_ptr<parse_tree::node> parse_tree::build_operator(void)
		{
		const char *token = reserved_word::translate(parser.get_next_token());
		if (reserved_word::isoperator(token))
			{
			std::shared_ptr<node> new_node(new node);
			new_node->type = node::OPERATOR;
			new_node->operation = token;
			return new_node;
			}
		else
			throw error_syntax();
		}

	/*
		PARSE_TREE::BUILD_TREE()
		------------------------
	*/
	std::shared_ptr<parse_tree::node> parse_tree::build_tree(std::shared_ptr<node> left)
		{
		while (1)
			{
			const char *operation = reserved_word::translate(parser.peek_next_token());
			if (reserved_word::precidence(operation) == 0)
				return left;
			if (operation == reserved_word::CLOSE_BRACKET)
				return left;

			std::shared_ptr<parse_tree::node> middle = build_operator();
			std::shared_ptr<parse_tree::node> right = build_operand();

			const char *next_operator  = reserved_word::translate(parser.peek_next_token());

			if (reserved_word::precidence(next_operator) > reserved_word::precidence(middle->operation))
				{
				right = build_tree(right);
				next_operator = reserved_word::translate(parser.peek_next_token());
				}

			middle->left = left;
			middle->right = right;

			if (reserved_word::precidence(next_operator) < reserved_word::precidence(middle->operation))
				return middle;

			left = middle;
			}
		}

	/*
		PARSE_TREE::BUILD()
		-------------------
	*/
	std::shared_ptr<parse_tree::node> parse_tree::build(void)
		{
		std::shared_ptr<parse_tree::node> left = build_operand();

		while (1)
			{
 			std::shared_ptr<parse_tree::node> root = build_tree(left);

			auto token = reserved_word::translate(parser.peek_next_token());
			if (reserved_word::precidence(token) == 0)
				return root;
			if (token == reserved_word::CLOSE_BRACKET)
				return root;

			left = root;
 			}
		}

	/*
		PARSE_TREE::BUILD()
		-------------------
	*/
	std::shared_ptr<parse_tree::node> parse_tree::build(const std::string &string)
		{
		parser.set_document(string);
		auto answer = build();
		if (*parser.peek_next_token() != '\0')
			throw error_syntax();

		return answer;
		}

	/*
		PARSE_TREE::EVALUATE()
		----------------------
	*/
	double parse_tree::evaluate(std::shared_ptr<parse_tree::node> root)
		{
		if (root->type == node::NUMBER)
			return root->number;
		else if (root->type == node::SYMBOL)
			return 1;											// FIX resolve against the symbol table
		else if (root->type == node::OPERATOR)
			{
			auto left = evaluate(root->left);
			auto right = evaluate(root->right);
			if (root->operation == reserved_word::PLUS)
				return left + right;
			else if (root->operation == reserved_word::MINUS)
				return left - right;
			else if (root->operation == reserved_word::MULTIPLY)
				return left * right;
			else if (root->operation == reserved_word::DIVIDE)
				{
				if (right == 0)								// FIX check properly.
					throw error_division_by_zero();
				return left / right;
				}
			else if (root->operation == reserved_word::POWER)
				return pow(left, right);
			else
				throw error_runtime();
			}
		else
			return 0;
		}
	}
