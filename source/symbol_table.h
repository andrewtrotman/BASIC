/*
	SYMBOL_TABLE.H
	--------------
*/
#pragma once

#include <iostream>
#include <unordered_map>

#include "symbol.h"
#include "array.h"

namespace BASIC
	{
	class executive;
	class parse_tree_node;

	/*
		CLASS SYMBOL_TABLE
		------------------
	*/
	class symbol_table
		{
		private:
			std::unordered_map<std::string, symbol> table;
			std::unordered_map<std::string, array> array_table;
			executive &evaluator;

		public:
			symbol_table(executive &evaluator);
			void create_array(const std::string &name, std::vector<size_t>sizes);
			symbol &operator[](const std::shared_ptr<parse_tree_node> &name);
		};
	}
