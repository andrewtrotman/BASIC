/*
	PARSE_TREE_NODE.H
	-----------------
*/
#pragma once

#include "symbol.h"
#include "executive.h"

namespace BASIC
	{
	/*
		CLASS PARSE_TREE_NODE
		---------------------
	*/
	class parse_tree_node
		{
		public:
			/*
				ENUM PARSE_TREE_NODE::NODE_TYPE
				-------------------------------
			*/
			enum node_type
				{
				OPERATOR,
				NUMBER,
				STRING,
				SYMBOL,
				COMMAND
				};

		public:
			std::shared_ptr<parse_tree_node> left;
			std::shared_ptr<parse_tree_node> right;
			node_type type;
			symbol (executive::*operation)(const std::shared_ptr<parse_tree_node> &root);		// OPERATOR: is an  operator
			void (executive::*command)(const std::shared_ptr<parse_tree_node> &root);		// COMMAND: is a  command
			double number;					// NUMBER: is a float
			std::string symbol;			// SYMBOL: is a variable or other symbol (function call?)
			std::string string;			// STRING: is a string

		public:
			/*
				PARSE_TREE_NODE::NODE()
				-----------------------
			*/
			parse_tree_node() :
				left(nullptr),
				right(nullptr),
				type(SYMBOL),
				operation(nullptr),
				command(nullptr),
				number(0),
				symbol("")
				{
				/* Nothing */
				}
		};
	}
