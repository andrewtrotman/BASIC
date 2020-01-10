/*
	EXECUTIVE.CPP
	-------------
*/
#include <math.h>
#include <ctype.h>

#include "error.h"
#include "executive.h"
#include "symbol_table.h"
#include "reserved_word.h"
#include "parse_tree_node.h"

namespace BASIC
	{

	/*
		EXECUTIVE::EVALUATE_PRINT_PART()
		--------------------------------
		return true on print cr/lf
	*/
	bool executive::evaluate_print_part(const std::shared_ptr<parse_tree_node> &root)
		{
		if (root->command == &executive::evaluate_print_comma)
			std::cout << "\t";
		if (root->left == nullptr && root->right == nullptr)  // we end with a semicolon or a comma
			{
			if (root->command == &executive::evaluate_print)
				return true;
			else
				return false;
			}

		if (root->left != nullptr)
			{
			auto value = evaluate_expression(root->left);

			if (value.isstring())
				std::cout << (std::string)value;
			else
				std::cout << (double)value;
			}

		if (root->right != nullptr)
			return evaluate_print_part(root->right);

		return true;
		}

	/*
		EXECUTIVE::EVALUATE_PRINT()
		---------------------------
	*/
	void executive::evaluate_print(const std::shared_ptr<parse_tree_node> &root)
		{
		if (evaluate_print_part(root))
			std::cout << "\n";
		}

	/*
		EXECUTIVE::EVALUATE_GOTO()
		--------------------------
	*/
	void executive::evaluate_goto(const std::shared_ptr<parse_tree_node> &root)
		{
		size_t next_line_number = static_cast<size_t>(root->left->number);
		next_line = parsed_code->lower_bound(next_line_number);
		if (next_line != parsed_code->end())
			if (next_line->first != next_line_number)
				throw error::undefined_statement();
		}

	/*
		EXECUTIVE::EVALUATE_GOSUB()
		---------------------------
	*/
	void executive::evaluate_gosub(const std::shared_ptr<parse_tree_node> &root)
		{
		auto return_address = next_line;
		size_t next_line_number = static_cast<size_t>(root->left->number);
		next_line = parsed_code->lower_bound(next_line_number);
		if (next_line != parsed_code->end())
			{
			if (next_line->first != next_line_number)
				throw error::undefined_statement();
			else
				gosub_stack.push_back(return_address);
			}
		}

	/*
		EXECUTIVE::EVALUATE_RETURN()
		----------------------------
	*/
	void executive::evaluate_return(const std::shared_ptr<parse_tree_node> &root)
		{
		if (gosub_stack.size() == 0)
			throw error::return_without_gosub();

		next_line = gosub_stack.back();
		gosub_stack.pop_back();
		}

	/*
		EXECUTIVE::EVALUATE_POP()
		-------------------------
	*/
	void executive::evaluate_pop(const std::shared_ptr<parse_tree_node> &root)
		{
		if (gosub_stack.size() <= 0)
			throw error::return_without_gosub();

		gosub_stack.pop_back();
		}

	/*
		EXECUTIVE::EVALUATE_REM()
		-------------------------
	*/
	void executive::evaluate_rem(const std::shared_ptr<parse_tree_node> &root)
		{
		/*
			Nothing
		*/
		}

	/*
		EXECUTIVE::EVALUATE_DATA()
		--------------------------
	*/
	void executive::evaluate_data(const std::shared_ptr<parse_tree_node> &root)
		{
		/*
			Nothing
		*/
		}

	/*
		EXECUTIVE::EVALUATE_RESTORE()
		-----------------------------
	*/
	void executive::evaluate_restore(const std::shared_ptr<parse_tree_node> &root)
		{
		data_pointer = nullptr;
		}

	/*
		EXECUTIVE::EVALUATE_READ_ONE()
		------------------------------
	*/
	symbol executive::evaluate_read_one(void)
		{
		if (data_pointer == nullptr)
			for (data_pointer_line = parsed_code->begin(); data_pointer_line != parsed_code->end(); ++data_pointer_line)
				if (data_pointer_line->second->command == &executive::evaluate_data)
					{
					data_pointer = data_pointer_line->second->right;
					break;
					}

		if (data_pointer == nullptr)
			throw error::out_of_data_error();

		auto source = data_pointer->left->string;
		data_pointer = data_pointer->right;

		if (data_pointer == nullptr)
			{
			while (data_pointer_line != parsed_code->end())
				if (data_pointer_line->second->command == &executive::evaluate_data)
					{
					data_pointer = data_pointer_line->second;
					break;
					}
			}
		if (data_pointer == nullptr)
			throw error::out_of_data_error();

		if (::isdigit(source[0]))
			return symbol(atof(source.c_str()));
		else if (source[0] == '"')
			return symbol(source.substr(1, source.size() - 2));
		else
			return symbol(source);
		}

	/*
		EXECUTIVE::EVALUATE_READ()
		--------------------------
	*/
	void executive::evaluate_read(const std::shared_ptr<parse_tree_node> &root)
		{
		for (auto node = root->right; node != nullptr; node = node->right)
			symbol_table[node->left] = evaluate_read_one();
		}

	/*
		EXECUTIVE::EVALUATE_END()
		-------------------------
	*/
	void executive::evaluate_end(const std::shared_ptr<parse_tree_node> &root)
		{
		end = true;
		}

	/*
		EXECUTIVE::EVALUATE_IF()
		------------------------
	*/
	void executive::evaluate_if(const std::shared_ptr<parse_tree_node> &root)
		{
		if (evaluate_expression(root->left))
			evaluate(root->right);
		}

	/*
		EXECUTIVE::EVALUATE_FOR()
		-------------------------
	*/
	void executive::evaluate_for(const std::shared_ptr<parse_tree_node> &root)
		{
		evaluate(root->left);
		symbol &from = symbol_table[root->left->left];
		symbol to = evaluate_expression(root->right->left);
		symbol step = evaluate_expression(root->right->right);
		for_stack.push_back(for_tuple(root->left->left->symbol, from, to, step, next_line));
		}

	/*
		EXECUTIVE::EVALUATE_DIM()
		-------------------------
	*/
	void executive::evaluate_dim(const std::shared_ptr<parse_tree_node> &root)
		{
		std::vector<size_t> sizes;

		auto name = root->left->string;
		for (auto current = root->right; current != nullptr; current = current->right)
			sizes.push_back(evaluate_expression(current->left));

		symbol_table.create_array(name, sizes);
		}

	/*
		EXECUTIVE::STEP()
		-----------------
		Returns true if a loop was iterated, or false if we leave a loop.
	*/
	bool executive::step(int64_t which_for_loop)
		{
		for_tuple &who = for_stack[which_for_loop];

		who.variable = static_cast<double>(who.variable) + who.step;
		double step = static_cast<double>(who.step);
		if (step > 0)
			{
			if (static_cast<double>(who.variable) <= who.to)
				next_line = who.line;
			else
				return false;
			}
		else if (step < 0)
			{
			if (static_cast<double>(who.variable) >= who.to)
				next_line = who.line;
			else
				return false;
			}
		else
			next_line = who.line;

		return true;
		}

	/*
		EXECUTIVE::EVALUATE_NEXT()
		--------------------------
	*/
	void executive::evaluate_next(const std::shared_ptr<parse_tree_node> &root)
		{
		int64_t which_for_loop = for_stack.size() - 1;
		if (which_for_loop < 0)
			throw error::next_without_for();

		if (root->string == "")
			{
			if (!step(which_for_loop))
				for_stack.pop_back();
			}
		else
			{
			const char *list = root->string.c_str();
			do
				{
				const char *from = list;
				const char *to = strchr(from, ',');
				if (to == nullptr)
					to = from + strlen(from);

				while (for_stack[which_for_loop].variable_name != std::string(from, to - from))
					if (--which_for_loop < 0)
						throw error::next_without_for();

				if (step(which_for_loop))
					return;
				else
					for_stack.pop_back();

				list = to + 1;		// skip over the ",".
				}
			while (*list != '\0');
			}
		}

	/*
		EXECUTIVE::EVALUATE_INPUT_PART()
		--------------------------------
	*/
	void executive::evaluate_input_part(const std::shared_ptr<parse_tree_node> &the_root)
		{
		char input[1024];

		std::cout << the_root->left->string;
		std::shared_ptr<parse_tree_node> root = the_root->right;

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
					while (*ch != '"' && *ch != '\0' && *ch != '\n' && *ch != '\r')
						ch++;
					if (*ch == '"')
						ch++;
					}
				else
					while (*ch != ',' && *ch != '\0' && *ch != '\n' && *ch != '\r')
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
					symbol_table[root->left] = atof(start);
				else if (*start == '"')
					symbol_table[root->left] = std::string(start + 1, strlen(start + 1) - 1);
				else
					symbol_table[root->left] = std::string(start);
					
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
		EXECUTIVE::EVALUATE_INPUT()
		---------------------------
	*/
	void executive::evaluate_input(const std::shared_ptr<parse_tree_node> &root)
		{
		while (1)
			try
				{
				evaluate_input_part(root);
				break;
				}
			catch (error::reenter)
				{
				/* Nothing */
				}
		}

	/*
		EXECUTIVE::EVALUATE_LET()
		-------------------------
	*/
	void executive::evaluate_let(const std::shared_ptr<parse_tree_node> &root)
		{
		symbol_table[root->left] = symbol(evaluate_expression(root->right));
		}


	/*
		EXECUTIVE::EVALUATE_PLUS()
		--------------------------
	*/
	symbol executive::evaluate_plus(const std::shared_ptr<parse_tree_node> &root)
		{
		auto left = evaluate_expression(root->left);
		auto right = evaluate_expression(root->right);
		return left + right;
		}

	/*
		EXECUTIVE::EVALUATE_MINUS()
		---------------------------
	*/
	symbol executive::evaluate_minus(const std::shared_ptr<parse_tree_node> &root)
		{
		auto left = evaluate_expression(root->left);
		auto right = evaluate_expression(root->right);
		return left - right;
		}

	/*
		EXECUTIVE::EVALUATE_MULTIPLY()
		------------------------------
	*/
	symbol executive::evaluate_multiply(const std::shared_ptr<parse_tree_node> &root)
		{
		auto left = evaluate_expression(root->left);
		auto right = evaluate_expression(root->right);
		return left * right;
		}

	/*
		EXECUTIVE::EVALUATE_MULTIPLY()
		------------------------------
	*/
	symbol executive::evaluate_divide(const std::shared_ptr<parse_tree_node> &root)
		{
		auto left = evaluate_expression(root->left);
		auto right = evaluate_expression(root->right);
		return left / right;
		}

	/*
		EXECUTIVE::EVALUATE_POWER()
		---------------------------
	*/
	symbol executive::evaluate_power(const std::shared_ptr<parse_tree_node> &root)
		{
		auto left = evaluate_expression(root->left);
		auto right = evaluate_expression(root->right);
		return pow(left, right);
		}

	/*
		EXECUTIVE::EVALUATE_UNARY_PLUS()
		--------------------------------
	*/
	symbol executive::evaluate_unary_plus(const std::shared_ptr<parse_tree_node> &root)
		{
		return evaluate_expression(root->right);
		}

	/*
		EXECUTIVE::EVALUATE_UNARY_MINUS()
		---------------------------------
	*/
	symbol executive::evaluate_unary_minus(const std::shared_ptr<parse_tree_node> &root)
		{
		return -(double)evaluate_expression(root->right);
		}

	/*
		EXECUTIVE::EVALUATE_EQUALS_EQUALS()
		-----------------------------------
	*/
	symbol executive::evaluate_equals_equals(const std::shared_ptr<parse_tree_node> &root)
		{
		auto left = evaluate_expression(root->left);
		auto right = evaluate_expression(root->right);
		return left == right;
		}

	/*
		EXECUTIVE::EVALUATE_NOT_EQUALS()
		--------------------------------
	*/
	symbol executive::evaluate_not_equals(const std::shared_ptr<parse_tree_node> &root)
		{
		auto left = evaluate_expression(root->left);
		auto right = evaluate_expression(root->right);
		return left != right;
		}

	/*
		EXECUTIVE::EVALUATE_GREATER_THAN()
		----------------------------------
	*/
	symbol executive::evaluate_greater_than(const std::shared_ptr<parse_tree_node> &root)
		{
		auto left = evaluate_expression(root->left);
		auto right = evaluate_expression(root->right);
		return left > right;
		}

	/*
		EXECUTIVE::EVALUATE_LESS_THAN()
		-------------------------------
	*/
	symbol executive::evaluate_less_than(const std::shared_ptr<parse_tree_node> &root)
		{
		auto left = evaluate_expression(root->left);
		auto right = evaluate_expression(root->right);
		return left < right;
		}

	/*
		EXECUTIVE::EVALUATE_GREATER_THAN_EQUALS()
		-----------------------------------------
	*/
	symbol executive::evaluate_greater_than_equals(const std::shared_ptr<parse_tree_node> &root)
		{
		auto left = evaluate_expression(root->left);
		auto right = evaluate_expression(root->right);
		return left >= right;
		}

	/*
		EXECUTIVE::EVALUATE_LESS_THAN_EQUALS()
		--------------------------------------
	*/

	symbol executive::evaluate_less_than_equals(const std::shared_ptr<parse_tree_node> &root)
		{
		auto left = evaluate_expression(root->left);
		auto right = evaluate_expression(root->right);
		return left <= right;
		}

	/*
		EXECUTIVE::EVALUATE_EXPRESSION()
		--------------------------------
	*/
	symbol executive::evaluate_expression(const std::shared_ptr<parse_tree_node> &root)
		{
		if (root == nullptr)
			return 0;
		else if (root->type == parse_tree_node::STRING)
			return root->string;
		else if (root->type == parse_tree_node::NUMBER)
			return root->number;
		else if (root->type == parse_tree_node::SYMBOL)
			return symbol_table[root];
		else if (root->type == parse_tree_node::OPERATOR)
			return (this->*(root->operation))(root);
		else
			return 0;
		}

	/*
		EXECUTIVE::EVALUATE()
		---------------------
	*/
	void executive::evaluate(const std::shared_ptr<parse_tree_node> &root)
		{
		if (root == nullptr)
			return;
		else if (root->type == parse_tree_node::COMMAND)
			(this->*(root->command))(root);
		else
			evaluate_expression(root);
		}

	/*
		EXECUTIVE::EVALUATE()
		---------------------
	*/
	void executive::evaluate(const executive::program &parsed_code, size_t start_line)
		{
		this->parsed_code = &parsed_code;
		auto current_line = parsed_code.lower_bound(start_line);

		while (current_line != parsed_code.end())
			{
			try
				{
				end = false;
//				std::cout << current_line->second << "\n";
				next_line = current_line;					// done like this so that GOTO works
				next_line++;
				evaluate(current_line->second);
				current_line = next_line;
				if (end)
					break;
				}
			catch (BASIC::error::extra_ignored)
				{
				std::cout << "EXTRA IGNORED";
				}
			}
		}
	}
