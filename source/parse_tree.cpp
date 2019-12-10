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
		std::shared_ptr<parse_tree::node> print(new node);

		parser.get_next_token();
		print->type = node::COMMAND;
		print->operation = reserved_word::PRINT;

		auto token = reserved_word::translate(parser.peek_next_token());
		if (*token == '\0')
			return print;

		if (token != reserved_word::COMMA && token != reserved_word::SEMICOLON)
			print->left = build();

		token = reserved_word::translate(parser.peek_next_token());
		if (token == reserved_word::COMMA)
			{
			print->right = parse_print();
			print->right->operation = reserved_word::COMMA;
			}
		else if (token == reserved_word::SEMICOLON)
			{
			print->right = parse_print();
			print->right->operation = reserved_word::SEMICOLON;
			}
		else if (*token == '\0')
			return print;
		else
			throw error::syntax();

		return print;
		}

	/*
		PARSE_TREE::PARSE_INPUT()
		-------------------------
	*/
	std::shared_ptr<parse_tree::node> parse_tree::parse_input(void)
		{
		std::shared_ptr<parse_tree::node> command(new node);

		parser.get_next_token();
		command->type = node::COMMAND;
		command->operation = reserved_word::INPUT;
		command->left = std::shared_ptr<parse_tree::node>(new node);

		auto token = reserved_word::translate(parser.peek_next_token());
		if (*token == '\0')
			throw error::syntax();
		else if (*token == '"')
			{
			command->left->string = parser.get_next_token();
			command->left->string = command->left->string.substr(1, command->left->string.size() - 2);
			}
		else
			command->left->string = "?";

		token = reserved_word::translate(parser.peek_next_token());
		if (token == reserved_word::SEMICOLON)
			{
			auto at = command;
			do
				{
				parser.get_next_token();			// read the semicolon or comma
				at->right = std::shared_ptr<parse_tree::node>(new node);
				at = at->right;
				at->type = node::SYMBOL;
				auto variable = parser.get_next_token();
				at->symbol = variable;
				token = reserved_word::translate(parser.peek_next_token());
				}
			while (token == reserved_word::COMMA);
			}
		else
			throw error::syntax();

		if (*token != '\0')
			throw error::syntax();

		return command;
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
		else if (command == reserved_word::QUESTIONMARK)
			return parse_print();
		else if (command == reserved_word::INPUT)
			return parse_input();
		else
			return parse_let();
		}

	/*
		PARSE_TREE::EVALUATE_PRINT()
		----------------------------
		return true on print cr/lf
	*/
	bool parse_tree::evaluate_print(std::shared_ptr<parse_tree::node> root)
		{
		if (root->operation == reserved_word::COMMA)
			std::cout << "\t";
		if (root->left == nullptr && root->right == nullptr)  // we end with a semiolon or a comma
			{
			if (root->operation == reserved_word::PRINT)
				return true;
			else
				return false;
			}

		if (root->left != nullptr)
			{
			auto value = evaluate(root->left);

			if (value.isstring())
				std::cout << (std::string)value;
			else
				std::cout << (double)value;
			}

		if (root->right != nullptr)
			return evaluate_print(root->right);

		return true;
		}

	/*
		PARSE_TREE::EVALUATE_INPUT()
		----------------------------
	*/
	void parse_tree::evaluate_input(std::shared_ptr<parse_tree::node> root)
		{
		char input[1024];
		memset(input, 'X', sizeof(input));

		std::cout << root->left->string;
		root = root->right;

		while (1)
			{
			fgets(input, sizeof(input), stdin);

			char *ch = input;
			do
				{
				if (root == nullptr)
					throw error::extra_ignored();

				/*
					remove whitespace before the data
				*/
				while (isspace(*ch))
					ch++;
				char *start = ch;

				/*
					extract the data
				*/
				if (isdigit(*ch))
					while (isdigit(*ch))
						ch++;
				else if (*ch == '"')
					{
					ch++;
					while (*ch != '"' && *ch != '\0')
						ch++;
					if (*ch == '"')
						ch++;
					}
				else
					while (*ch != ',' && *ch != '\0')
						ch++;

				/*
					remove whitespace after the data
				*/
				while (isspace(*ch))
					*ch++ = '\0';

				if (*ch != ',' && *ch != '\0')
					throw error::reenter();

				if (*ch != '\0')
					*ch++ = '\0';

				if (isdigit(*start))
					symbol_table[root->symbol] = atof(start);
				else if (*start == '"')
					symbol_table[root->symbol] = std::string(start + 1, strlen(start + 1) - 1);
				else
					symbol_table[root->symbol] = std::string(start);
					
				/*
					next variable.
				*/
				root = root->right;
				}
			while (*ch != '\0');

			if (root == nullptr)
				return;

			std::cout << "??";
			}
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
				if (evaluate_print(root))
					std::cout << "\n";
				}
			else if (root->operation == reserved_word::INPUT)
				while (1)
					try
						{
						evaluate_input(root);
						break;
						}
					catch (error::reenter)
						{
						/* Nothing */
						}
			else if (root->operation == reserved_word::EQUALS)
				{
				auto value = evaluate(root->right);
				symbol_table[root->left->symbol] = symbol(value);
				}
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
