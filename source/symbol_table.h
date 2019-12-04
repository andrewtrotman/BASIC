/*
	SYMBOL_TABLE.H
	--------------
*/
#pragma once

#include <unordered_map>
#include "symbol.h"

namespace BASIC
	{
	/*
		CLASS SYMBOL_TABLE
		------------------
	*/
	class symbol_table
		{
		private:
			std::unordered_map<std::string, symbol> table;

		public:
			/*
				SYMBOL_TABLE::SYMBOL_TABLE()
				----------------------------
			*/
			symbol_table()
				{
				/* Nothing */
				}

			/*
				SYMBOL_TABLE::OPERATOR[]()
				--------------------------
			*/
			symbol &operator[](const std::string &name)
				{
				return table[name];
				}
		};
	}
