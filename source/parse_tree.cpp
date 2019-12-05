/*
	PARSE_TREE.CPP
	--------------
*/
#include <math.h>
#include <ctype.h>

#include "error.h"
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
				throw error::syntax();
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
		else if (*token == '"')
			{
			std::shared_ptr<node> new_node(new node);
			new_node->type = node::STRING;
			new_node->string = std::string(token);
			new_node->string = new_node->string.substr(1, new_node->string.length() - 2);
			return new_node;
			}
		else if (token == reserved_word::MINUS)			// Unary minus
			{
			std::shared_ptr<node> new_node(new node);
			new_node->type = node::OPERATOR;
			new_node->operation = reserved_word::UNARY_MINUS;
			new_node->right = build_operand();
			return new_node;
			}
		else if (token == reserved_word::PLUS)			// Unary plus
			{
			std::shared_ptr<node> new_node(new node);
			new_node->type = node::OPERATOR;
			new_node->operation = reserved_word::UNARY_PLUS;
			new_node->right = build_operand();
			return new_node;
			}
		else
			throw error::syntax();
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
			throw error::syntax();
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

		const char *token;
		do
			{
 			std::shared_ptr<parse_tree::node> root = build_tree(left);

			token = reserved_word::translate(parser.peek_next_token());
			if (reserved_word::precidence(token) == 0)
				return root;
			if (token == reserved_word::CLOSE_BRACKET)
				return root;

			left = root;
 			}
		while (reserved_word::isoperator(token));

		return left;
		}

	/*
		PARSE_TREE::PARSE_PRINT()
		-------------------------
	*/
	std::shared_ptr<parse_tree::node> parse_tree::parse_print(void)
		{
		parser.get_next_token();
		std::shared_ptr<parse_tree::node> print(new node);
		print->type = node::COMMAND;
		print->operation = reserved_word::PRINT;
		print->right = build();

		auto token = parser.peek_next_token();
		if (*token == '\0')
			return print;
		else if (*token == ';')
			{
			parser.get_next_token();
			print->attribute = node::NO_CR_LF;
			}
		else
			throw error::syntax();

		return print;
		}

	/*
		PARSE_TREE::PARSE_LET()
		-----------------------
	*/
	std::shared_ptr<parse_tree::node> parse_tree::parse_let(void)
		{
		/*
			We've got a symbol so its an assignment statemnent.
		*/
		std::string variable = reserved_word::translate(parser.get_next_token());
		auto equal = reserved_word::translate(parser.get_next_token());
		if (equal != reserved_word::EQUALS)
			throw error::syntax();

		std::shared_ptr<parse_tree::node> let(new node);
		let->type = node::COMMAND;
		let->operation = reserved_word::EQUALS;
		let->left = std::make_shared<parse_tree::node>();
		let->left->type = node::SYMBOL;
		let->left->symbol = variable;

		let->right = build();
		if (*parser.peek_next_token() != '\0')
			throw error::syntax();

		return let;
		}

	/*
		PARSE_TREE::BUILD()
		-------------------
	*/
	std::shared_ptr<parse_tree::node> parse_tree::build(const std::string &string)
		{
		parser.set_document(string);

		auto command = reserved_word::translate(parser.peek_next_token());
		if (command == reserved_word::PRINT)
			return parse_print();
		else
			return parse_let();

		}

	/*
		PARSE_TREE::EVALUATE()
		----------------------
	*/
	symbol parse_tree::evaluate(std::shared_ptr<parse_tree::node> root)
		{
		if (root == nullptr)
			return 0;

		if (root->type == node::COMMAND)
			{
			if (root->operation == reserved_word::PRINT)
				{
				auto value = evaluate(root->right);
				if (value.isstring())
					std::cout << (std::string)value;
				else
					std::cout << (double)value;
				if (!(root->attribute & node::NO_CR_LF))
					std::cout << '\n';
				return value;
				}
			else if (root->operation == reserved_word::EQUALS)
				{
				auto value = evaluate(root->right);
				symbol_table[root->left->symbol] = symbol(value);
				return value;
				}
			else
				return 0;
			}
		else if (root->type == node::STRING)
			return root->string;
		else if (root->type == node::NUMBER)
			return root->number;
		else if (root->type == node::SYMBOL)
			return symbol_table[root->symbol];
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
				return left / right;
			else if (root->operation == reserved_word::POWER)
				return pow(left, right);
			else if (root->operation == reserved_word::UNARY_PLUS)
				return right;
			else if (root->operation == reserved_word::UNARY_MINUS)
				return -(double)right;
			else
				throw error::runtime();
			}
		else
			return 0;
		}
	}
