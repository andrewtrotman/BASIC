/*
	SYMBOL.H
	--------
*/
#pragma once

#include <string>

namespace BASIC
	{
	class symbol
		{
		enum symbol_type
			{
			NUMERIC
			};

		public:
			symbol_type type;
			std::string name;
			double value;
		};
	}
