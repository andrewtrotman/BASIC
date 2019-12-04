/*
	SYMBOL_TABLE.H
	--------------
*/
#pragma once

#include <unordered_map>
#include "symbol.h"

namespace BASIC
	{
	class symbol_table
		{
		private:
			std::unordered_map<std::string, symbol> table;

		public:
			symbol_table()
				{
				/* Nothing */
				}

			symbol &operator[](const std::string &name)
				{
				return table[name];
				}
		};
	}
