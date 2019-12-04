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
			double value;

		public:
			symbol()
				{
				/* Nothing */
				}

			symbol(double value) :
				type(NUMERIC),
				value(value)
				{
				/* Nothing */
				}
		};
	}
