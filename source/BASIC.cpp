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
		program[1] = parser.build("a=5");
		program[10] = parser.build("gosub 100");
		program[20] = parser.build("? 20");
		program[30] = parser.build("dim array(10,10,10)");
		program[31] = parser.build("array(5,5,5) = 9");
		program[32] = parser.build("? array(5,5,5)");
		program[33] = parser.build("input array(4,a,6)");
		program[34] = parser.build("? array(4,5,6)");
		program[35] = parser.build("? array(5,5,5)");
		program[39] = parser.build("end");
		program[40] = parser.build("? 40");
		program[100] = parser.build("?100");
		program[105] = parser.build("rem pop");
		program[110] = parser.build("return");
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
	catch (BASIC::error::next_without_for)
		{
		std::cout << "NEXT WITHOUT FOR\n\n";
		}
	catch (BASIC::error::return_without_gosub)
		{
		std::cout << "RETURN WITHOUT GOSUB\n\n";
		}
	catch (...)
		{
		std::cout << "ERROR\n\n";
		}

	return 0;
	}
