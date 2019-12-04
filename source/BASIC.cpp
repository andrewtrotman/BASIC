/*
	BASIC.CPP
	---------
*/
#include <iostream>

#include "parse_tree.h"

int main(int argc, char *argv[])
	{
	BASIC::parse_tree parser;

try
	{
	std::string line;

	//	line = "1 + 2^2 + 3 + 6 * 4^2 * 5 + 6 + 7 * 8 * 9 + 12 + 11 + 33";		// ((((((((1+(2^2))+3)+((6*(4^2))*5))+6)+((7*8)*9))+12)+11)+33)
	//	line = "1 + 2 + 3 + 4 * 5 * 6 * 7^8^9 + 9 + 10";
	//	line = "2^2 + 3 * 6^6 + 1";			// (((2^2)+(3*(6^6)))+1)
	//	line = "3^5^6*1+2^5+5/6^7*5";		// (((((3^5)^6)*1)+(2^5))+((5/(6^7))*5))
		line = "1 + 3^5 + 1";					// ((1+(3^5))+1)
	//	line = "1 + 2 + 3 + 4";				// (((1+2)+3)+4)
	//	line = "2 + 3 * 4^5 + 1";				// ((2+(3*(4^5)))+1)
	//	line = "2 ^ 3 * 4 * 5 + 6 * 7";
	//	line = "2 * (3 + 4) * 5";				// ((2*(3+4))*5)
	//	line = "(2 * (3 + (4)) * 5)";		// ((2*(3+4))*5)
	//	line = "(((2 * (((3)) + (4)) * 5)))";		// ((2*(3+4))*5)
	//	line = "2 * (((3)) + (4)) * 5";				// ((2*(3+4))*5)
	//	line = "2 * (3 + (4)) * 5";				// ((2*(3+4))*5)
	//	line = "2 * (something + (else)) * more";
	//	line = "2 * something + (else)";


		auto tree = parser.build(line);

		auto answer = parser.evaluate(tree);

		std::cout << tree << " = " << answer << "\n\n";

		}
	catch (BASIC::parse_tree::error_syntax)
		{
		std::cout << "SYNTAX ERROR\n\n";
		}
	catch (BASIC::parse_tree::error_runtime)
		{
		std::cout << "RUNTIME ERROR\n\n";
		}
	catch (BASIC::parse_tree::error_division_by_zero)
		{
		std::cout << "DIVISION BY ZERO\n\n";
		}
	catch (...)
		{
		std::cout << "ERROR\n\n";
		}

	return 0;
	}
