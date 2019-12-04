/*
	SYMBOL.H
	--------
*/
#pragma once

#include <ctype.h>

#include <string>

#include "error.h"

namespace BASIC
	{
	/*
		CLASS SYMBOL
		------------
	*/
	class symbol
		{
		/*
			ENUM SYMBOL_TYPE
			----------------
		*/
		enum symbol_type
			{
			NUMERIC,
			STRING
			};

		private:
			symbol_type type;
			double value;
			std::string string;

		public:
			/*
				SYMBOL::SYMBOL()
				----------------
			*/
			symbol() :
				type(NUMERIC),
				value(0)
				{
				/* Nothing */
				}

			/*
				SYMBOL::SYMBOL()
				----------------
			*/
			symbol(double value) :
				type(NUMERIC),
				value(value)
				{
				/* Nothing */
				}

			/*
				SYMBOL::SYMBOL()
				----------------
			*/
			symbol(const std::string &value) :
				type(STRING),
				string(value)
				{
				/* Nothing */
				}

			/*
				SYMBOL::OPERATOR DOUBLE()
				-------------------------
			*/
			operator double()
				{
				if (type == NUMERIC)
					return value;
				else
					return ::atof(string.c_str());
				}

			/*
				SYMBOL::OPERATOR STD::STRING()
				------------------------------
			*/
			operator std::string()
				{
				if (type == NUMERIC)
					return std::to_string(value);
				else
					return string;
				}

			/*
				SYMBOL::ISSTRING()
				------------------
			*/
			bool isstring(void)
				{
				return type == STRING;
				}

			/*
				SYMBOL::OPERATOR+()
				-------------------
			*/
			symbol operator+(const symbol &additive)
				{
				if (type == NUMERIC && additive.type == NUMERIC)
					return symbol(value + additive.value);
				else if (type == NUMERIC && additive.type == STRING)
					return symbol(std::to_string(value) + additive.string);
				else if (type == STRING && additive.type == NUMERIC)
					return symbol(string + std::to_string(additive.value));
				else
					return symbol(string + additive.string);
				}

			/*
				SYMBOL::OPERATOR-()
				-------------------
			*/
			symbol operator-(const symbol &additive)
				{
				if (type == NUMERIC && additive.type == NUMERIC)
					return symbol(value + additive.value);
				else
					throw error::runtime();
				}

			/*
				SYMBOL::OPERATOR*()
				-------------------
			*/
			symbol operator*(const symbol &additive)
				{
				if (type == NUMERIC && additive.type == NUMERIC)
					return symbol(value * additive.value);
				else
					throw error::runtime();
				}

			/*
				SYMBOL::OPERATOR/()
				-------------------
			*/
			symbol operator/(const symbol &additive)
				{
				if (type != NUMERIC || additive.type != NUMERIC)
					throw error::runtime();
				if (additive.value == 0)
					throw error::division_by_zero();

				return symbol(value / additive.value);
				}
		};
	}
