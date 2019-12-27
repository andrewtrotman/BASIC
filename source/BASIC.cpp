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

		program[10] = parser.build("read a, b");
		program[20] = parser.build("? a, b");
		program[100] = parser.build("data =   >  , 4");

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
