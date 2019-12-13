/*
	BASIC.CPP
	---------
*/
#include <map>
#include <iostream>

#include "parse_tree.h"
#include "executive.h"

int main(int argc, char *argv[])
	{
	BASIC::parse_tree parser;
	BASIC::executive eval;
	BASIC::executive::program program;

	try
		{
		std::string line;
		program[20] = parser.build("a = 1");
		program[30] = parser.build("b = a = 1");
		program[40] = parser.build("? a");
		program[50] = parser.build("? b");
		program[60] = parser.build("? c");
		program[70] = parser.build("if a >= 1 then goto 90");
		program[80] = parser.build("goto 100");
		program[90] = parser.build("print \"true\"");
		program[100] = parser.build("end");
		eval.evaluate(program);
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
	catch (BASIC::error::undefined_statement)
		{
		std::cout << "UNDEFINED STATEMENT\n\n";
		}
	catch (...)
		{
		std::cout << "ERROR\n\n";
		}

	return 0;
	}
