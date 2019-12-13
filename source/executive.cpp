/*
	EXECUTIVE.CPP
	-------------
*/
#include <math.h>
#include <ctype.h>

#include "error.h"
#include "executive.h"
#include "reserved_word.h"

namespace BASIC
	{

	/*
		EXECUTIVE::EVALUATE_PRINT()
		---------------------------
		return true on print cr/lf
	*/
	bool executive::evaluate_print(std::shared_ptr<parse_tree::node> root)
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
		EXECUTIVE::EVALUATE_IF()
		------------------------
	*/
	void executive::evaluate_if(std::shared_ptr<parse_tree::node> root)
		{
		auto got = evaluate(root->left);
		if (got)
			evaluate(root->right);
		}

	/*
		EXECUTIVE::EVALUATE_INPUT()
		---------------------------
	*/
	void executive::evaluate_input(std::shared_ptr<parse_tree::node> root)
		{
		char input[1024];

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
		EXECUTIVE::EVALUATE()
		---------------------
	*/
	symbol executive::evaluate(std::shared_ptr<parse_tree::node> root)
		{
		if (root == nullptr)
			return 0;

		if (root->type == parse_tree::node::COMMAND)
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
			else if (root->operation == reserved_word::IF)
				evaluate_if(root);
			else if (root->operation == reserved_word::EQUALS)
				{
				auto value = evaluate(root->right);
				symbol_table[root->left->symbol] = symbol(value);
				}
			return 0;
			}
		else if (root->type == parse_tree::node::STRING)
			return root->string;
		else if (root->type == parse_tree::node::NUMBER)
			return root->number;
		else if (root->type == parse_tree::node::SYMBOL)
			return symbol_table[root->symbol];
		else if (root->type == parse_tree::node::OPERATOR)
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
			else if (root->operation == reserved_word::EQUALS)
				return left == right;
			else if (root->operation == reserved_word::NOT_EQUALS)
				return left != right;
			else if (root->operation == reserved_word::GREATER_THAN)
				return left > right;
			else if (root->operation == reserved_word::LESS_THAN)
				return left < right;
			else if (root->operation == reserved_word::GREATER_THAN_EQUALS)
				return left >= right;
			else if (root->operation == reserved_word::LESS_THAN_EQUALS)
				return left <= right;
			else
				throw error::runtime();
			}
		else
			return 0;
		}

	/*
		EXECUTIVE::EVALUATE()
		---------------------
	*/
	void executive::evaluate(const executive::program &parsed_code, size_t start_line)
		{
		auto current_line = parsed_code.lower_bound(start_line);

		while (current_line != parsed_code.end())
			{
			try
				{
//				std::cout << current_line->second << "\n";
				evaluate(current_line->second);
				current_line++;
				}
			catch (BASIC::error::extra_ignored)
				{
				std::cout << "EXTRA IGNORED";
				}
			}
		}
	}
