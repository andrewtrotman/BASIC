/*
	BASIC.CPP
	---------
*/
#include <map>
#include <iostream>

#include "parse_tree.h"

int main(int argc, char *argv[])
	{
	BASIC::parse_tree parser;
	std::map<size_t, std::shared_ptr<BASIC::parse_tree::node>> program;

	try
		{
		std::string line;

	//	line = "1 + 2^2 + 3 + 6 * 4^2 * 5 + 6 + 7 * 8 * 9 + 12 + 11 + 33";		// ((((((((1+(2^2))+3)+((6*(4^2))*5))+6)+((7*8)*9))+12)+11)+33)
	//	line = "1 + 2 + 3 + 4 * 5 * 6 * 7^8^9 + 9 + 10";
		line = "2^2 + 3 * 6^6 + 1";			// (((2^2)+(3*(6^6)))+1)
	//	line = "3^5^6*1+2^5+5/6^7*5";		// (((((3^5)^6)*1)+(2^5))+((5/(6^7))*5))
	//	line = "1 + 3^5 + 1";					// ((1+(3^5))+1)
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
#ifdef NEVER
		program[10] = parser.build("x = 2^6");
		program[20] = parser.build("print x + 1");
		program[30] = parser.build("x = x + x");
		program[40] = parser.build("print x + 1");
		program[50] = parser.build("print \"Hello\"");
		program[60] = parser.build("z = \"Hello\"");
		program[65] = parser.build("print z");
		program[70] = parser.build("z = z + \" X \"");
		program[80] = parser.build("print z + x");
		program[85] = parser.build("a = 1");
		program[90] = parser.build("print -1 / a");
		program[100] = parser.build("print --1");
		program[105] = parser.build("print \".\"");
		program[110] = parser.build("print \"H\";\"E\";X,");
		program[115] = parser.build("print \"H\";\"E\";X");
		program[120] = parser.build("?x,x;x;x");
		program[130] = parser.build("?,x,x;x;x");
		program[140] = parser.build("?,,x,x;x;x");
		program[150] = parser.build("?");
		program[160] = parser.build("?");
		program[200] = parser.build("? \"END\"");
#endif

//			program[10] = parser.build("input");
//			program[20] = parser.build("input \"->\"");
//			program[30] = parser.build("input \"->\"; a,b, c");
			program[20] = parser.build("a = 1");
			program[30] = parser.build("b = a = 1");
			program[40] = parser.build("? a");
			program[50] = parser.build("? b");
			program[60] = parser.build("? c");

			program[100] = parser.build("if a = 1 then print \"true\"");




		for (const auto &[line, command] : program)
			{
			try
				{
//				std::cout << command << "\n";
				parser.evaluate(command);
				}
			catch (BASIC::error::extra_ignored)
				{
				std::cout << "EXTRA IGNORED";
				}
			}
		}
	catch (BASIC::error::syntax)
		{
		std::cout << "SYNTAX ERROR\n\n";
		}
	catch (BASIC::error::runtime)
		{
		std::cout << "RUNTIME ERROR\n\n";
		}
	catch (BASIC::error::division_by_zero)
		{
		std::cout << "DIVISION BY ZERO\n\n";
		}
	catch (BASIC::error::extra_ignored)
		{
		std::cout << "EXTRA IGNORED\n\n";
		}
	catch (...)
		{
		std::cout << "ERROR\n\n";
		}

	return 0;
	}
