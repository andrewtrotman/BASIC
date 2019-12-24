/*
	SYMBOL_TABLE.CPP
	--------------
*/
#include "symbol_table.h"
#include "executive.h"

namespace BASIC
	{
	/*
		SYMBOL_TABLE::SYMBOL_TABLE()
		----------------------------
	*/
	symbol_table::symbol_table(executive &evaluator) :
		evaluator(evaluator)
		{
		/* Nothing */
		}

	/*
		SYMBOL_TABLE::CREATE_ARRAY()
		----------------------------
	*/
	void symbol_table::create_array(const std::string &name, std::vector<size_t>sizes)
		{
		array_table.emplace(name, sizes);
		}

	/*
		SYMBOL_TABLE::OPERATOR[]()
		--------------------------
	*/
	symbol &symbol_table::operator[](const std::shared_ptr<parse_tree::node> &name)
		{
		if (name->right == nullptr)
			return table[name->symbol];
		else
			{
			/*
				array or function call
			*/
			std::string variable_name = name->symbol;
			std::vector<size_t>indexes;
			for (auto which = name->right; which != nullptr; which = which->right)
				indexes.push_back(evaluator.evaluate_expression(which->left));

			return array_table[variable_name][indexes];
			}
		}
	}
