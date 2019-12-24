/*
	PARSE_TREE.CPP
	--------------
*/
#include <math.h>
#include <ctype.h>

#include "error.h"
#include "parser.h"
#include "parse_tree.h"
#include "reserved_word.h"
#include "symbol_table.h"


namespace BASIC
	{
	/*
		PARSE_TREE::BUILD_VARIABLE()
		----------------------------
	*/
	std::shared_ptr<parse_tree::node> parse_tree::build_variable(const std::string &name, bool can_be_parameterised)
		{
		std::shared_ptr<node> new_node(new node);
		new_node->type = node::SYMBOL;
		new_node->symbol = name;
		if (can_be_parameterised && reserved_word::translate(parser.peek_next_token()) == reserved_word::OPEN_BRACKET)
			{
			/*
				An array reference or a function call
			*/
			parser.get_next_token();
			auto next = new_node->right = std::shared_ptr<node>(new node);
			std::string token;
			do
				{
				next->left = build();
				token = reserved_word::translate(parser.get_next_token());			// comma
				if (token == reserved_word::CLOSE_BRACKET)
					break;
				next->right = std::shared_ptr<parse_tree::node>(new node);
				next = next->right;
				}
			while (token == reserved_word::COMMA);

			if (token != reserved_word::CLOSE_BRACKET)
				throw error::syntax();
			}
		return new_node;
		}

	/*
		PARSE_TREE::BUILD_OPERAND()
		---------------------------
	*/
	std::shared_ptr<parse_tree::node> parse_tree::build_operand(void)
		{
		const char *token = reserved_word::translate(parser.get_next_token());

		if (token == reserved_word::OPEN_BRACKET)
			{
			std::shared_ptr<node> child = build();
			auto close = reserved_word::translate(parser.get_next_token());		//consume the close bracket.
			if (close != reserved_word::CLOSE_BRACKET)
				throw error::syntax();
			return child;
			}
		else if (::isdigit(*token))
			{
			std::shared_ptr<node> new_node(new node);
			new_node->type = node::NUMBER;
			new_node->number = atof(token);
			return new_node;
			}
		else if (::isalpha(*token))
			return build_variable(token);
		else if (*token == '"')
			{
			std::shared_ptr<node> new_node(new node);
			new_node->type = node::STRING;
			new_node->string = std::string(token);
			new_node->string = new_node->string.substr(1, new_node->string.length() - 2);
			return new_node;
			}
		else if (token == reserved_word::MINUS)			// Unary minus
			{
			std::shared_ptr<node> new_node(new node);
			new_node->type = node::OPERATOR;
			new_node->operation = reserved_word::UNARY_MINUS;
			new_node->right = build_operand();
			return new_node;
			}
		else if (token == reserved_word::PLUS)			// Unary plus
			{
			std::shared_ptr<node> new_node(new node);
			new_node->type = node::OPERATOR;
			new_node->operation = reserved_word::UNARY_PLUS;
			new_node->right = build_operand();
			return new_node;
			}
		else
			throw error::syntax();
		}

	/*
		PARSE_TREE::BUILD_OPERATOR()
		----------------------------
	*/
	std::shared_ptr<parse_tree::node> parse_tree::build_operator(void)
		{
		const char *token = reserved_word::translate(parser.get_next_token());
		if (reserved_word::isoperator(token))
			{
			std::shared_ptr<node> new_node(new node);
			new_node->type = node::OPERATOR;
			new_node->operation = token;
			return new_node;
			}
		else
			throw error::syntax();
		}

	/*
		PARSE_TREE::BUILD_TREE()
		------------------------
	*/
	std::shared_ptr<parse_tree::node> parse_tree::build_tree(std::shared_ptr<node> left)
		{
		while (1)
			{
			const char *operation = reserved_word::translate(parser.peek_next_token());
			if (reserved_word::precidence(operation) == 0)
				return left;
			if (operation == reserved_word::CLOSE_BRACKET)
				return left;

			std::shared_ptr<parse_tree::node> middle = build_operator();
			std::shared_ptr<parse_tree::node> right = build_operand();

			const char *next_operator  = reserved_word::translate(parser.peek_next_token());

			if (reserved_word::precidence(next_operator) > reserved_word::precidence(middle->operation))
				{
				right = build_tree(right);
				next_operator = reserved_word::translate(parser.peek_next_token());
				}

			middle->left = left;
			middle->right = right;

			if (reserved_word::precidence(next_operator) < reserved_word::precidence(middle->operation))
				return middle;

			left = middle;
			}
		}

	/*
		PARSE_TREE::BUILD()
		-------------------
	*/
	std::shared_ptr<parse_tree::node> parse_tree::build(void)
		{
		std::shared_ptr<parse_tree::node> left = build_operand();

		const char *token;
		do
			{
 			std::shared_ptr<parse_tree::node> root = build_tree(left);

			token = reserved_word::translate(parser.peek_next_token());
			if (reserved_word::precidence(token) == 0)
				return root;
			if (token == reserved_word::CLOSE_BRACKET)
				return root;

			left = root;
 			}
		while (reserved_word::isoperator(token));

		return left;
		}

	/*
		PARSE_TREE::PARSE_PRINT()
		-------------------------
		PRINT [{expr} [{,|; [{expr}]}]] [,|;]
		PRINT {;}
		PRINT {,}
	*/
	std::shared_ptr<parse_tree::node> parse_tree::parse_print(void)
		{
		std::shared_ptr<parse_tree::node> print(new node);

		parser.get_next_token();
		print->type = node::COMMAND;
		print->operation = reserved_word::PRINT;

		auto token = reserved_word::translate(parser.peek_next_token());
		if (*token == '\0')
			return print;

		if (token != reserved_word::COMMA && token != reserved_word::SEMICOLON)
			print->left = build();

		token = reserved_word::translate(parser.peek_next_token());
		if (token == reserved_word::COMMA)
			{
			print->right = parse_print();
			print->right->operation = reserved_word::COMMA;
			}
		else if (token == reserved_word::SEMICOLON)
			{
			print->right = parse_print();
			print->right->operation = reserved_word::SEMICOLON;
			}
		else if (*token == '\0')
			return print;
		else
			throw error::syntax();

		return print;
		}

	/*
		PARSE_TREE::PARSE_INPUT()
		-------------------------
		INPUT [string;] var [{,var}]
	*/
	std::shared_ptr<parse_tree::node> parse_tree::parse_input(void)
		{
		std::shared_ptr<parse_tree::node> command(new node);

		parser.get_next_token();
		command->type = node::COMMAND;
		command->operation = reserved_word::INPUT;
		command->left = std::shared_ptr<parse_tree::node>(new node);

		auto token = reserved_word::translate(parser.peek_next_token());
		if (*token == '\0')
			throw error::syntax();
		else if (*token == '"')
			{
			command->left->string = parser.get_next_token();
			command->left->string = command->left->string.substr(1, command->left->string.size() - 2);
			token = reserved_word::translate(parser.get_next_token());
			if (token != reserved_word::SEMICOLON)
				throw error::syntax();
			}
		else
			command->left->string = "?";

		command->right = std::shared_ptr<parse_tree::node>(new node);
		auto at = command->right;
		do
			{
			auto variable = parser.get_next_token();
			if (!isalpha(*variable))
				throw error::syntax();

			at->left = build_variable(variable);
			token = reserved_word::translate(parser.peek_next_token());
			if (*token == '\0')
				break;
			if (token != reserved_word::COMMA)
				throw error::syntax();
			reserved_word::translate(parser.get_next_token());
			at->right = std::shared_ptr<parse_tree::node>(new node);
			at = at->right;
			}
		while (1);

		return command;
		}

	/*
		PARSE_TREE::PARSE_FOR()
		-----------------------
		FOR real avar = aexpr1 TO aexpr2 [STEP expr3]
	*/
	std::shared_ptr<parse_tree::node> parse_tree::parse_for(void)
		{
		std::shared_ptr<parse_tree::node> command(new node);

		parser.get_next_token();			// FOR
		command->type = node::COMMAND;
		command->operation = reserved_word::FOR;
		command->left = parse_let(false);
		auto token = reserved_word::translate(parser.peek_next_token());
		if (token == reserved_word::TO)
			{
			parser.get_next_token();
			std::shared_ptr<parse_tree::node> to_step(new node);
			command->right = to_step;
			to_step->left = build();		// the to amount
			token = reserved_word::translate(parser.peek_next_token());
			if (token == reserved_word::STEP)
				{
				parser.get_next_token();
				to_step->right = build();
				}
			else
				{
				to_step->right = std::make_shared<node>();
				to_step->right->type = node::NUMBER;
				to_step->right->number = 1;
				}
			}
		else
			throw error::syntax();

		return command;
		}

	/*
		PARSE_TREE::PARSE_NEXT()
		-----------------------
		NEXT [avar]
		NEXT avar [{,avar}]
	*/
	std::shared_ptr<parse_tree::node> parse_tree::parse_next(void)
		{
		std::shared_ptr<parse_tree::node> command(new node);

		parser.get_next_token();			// FOR

		command->type = node::COMMAND;
		command->operation = reserved_word::NEXT;

		auto token = parser.peek_next_token();
		while (isalpha(*token))
			{
			/*
				we have a named variable
			*/
			command->string += token;
			parser.get_next_token();
			token = reserved_word::translate(parser.peek_next_token());
			if (token == reserved_word::COMMA)
				{
				parser.get_next_token();
				command->string += ",";
				}
			token = parser.peek_next_token();
			}

		return command;
		}

	/*
		PARSE_TREE::PARSE_DIM()
		------------------------
		DIM var subscript [{, var subscript}]
		where
		subscript := (aexpr [{, aexpr}])
	*/
	std::shared_ptr<parse_tree::node> parse_tree::parse_dim(void)
		{
		std::shared_ptr<parse_tree::node> command(new node);

		parser.get_next_token();
		command->type = node::COMMAND;
		command->operation = reserved_word::DIM;

		auto variable = reserved_word::translate(parser.get_next_token());
		if (!isalpha(*variable))
			throw error::syntax();

		command->left = std::shared_ptr<parse_tree::node>(new node);
		command->left->type = node::STRING;
		command->left->string = variable;

		auto token = reserved_word::translate(parser.get_next_token());
		if (token != reserved_word::OPEN_BRACKET)
			throw error::syntax();

		command->right = std::shared_ptr<parse_tree::node>(new node);

		auto next = command->right;
		do
			{
			next->left = build();
			token = reserved_word::translate(parser.get_next_token());			// comma
			if (token == reserved_word::CLOSE_BRACKET)
				break;
			next->right = std::shared_ptr<parse_tree::node>(new node);
			next = next->right;
			}
		while (token == reserved_word::COMMA);

		if (token != reserved_word::CLOSE_BRACKET)
			throw error::syntax();

		return command;
		}

	/*
		PARSE_TREE::PARSE_GOTO()
		------------------------
		GOTO linenum
	*/
	std::shared_ptr<parse_tree::node> parse_tree::parse_goto(void)
		{
		std::shared_ptr<parse_tree::node> command(new node);

		parser.get_next_token();
		auto line_number = parser.get_next_token();
		if (!isdigit(*line_number))
			throw error::syntax();

		command->type = node::COMMAND;
		command->operation = reserved_word::GOTO;
		std::shared_ptr<parse_tree::node> target_line(new node);
		command->left = target_line;
		target_line->type = node::NUMBER;
		target_line->number = atof(line_number);

		return command;
		}

	/*
		PARSE_TREE::PARSE_GOSUB()
		-------------------------
		GOSUB linenum
	*/
	std::shared_ptr<parse_tree::node> parse_tree::parse_gosub(void)
		{
		std::shared_ptr<parse_tree::node> command(new node);

		parser.get_next_token();
		auto line_number = parser.get_next_token();
		if (!isdigit(*line_number))
			throw error::syntax();

		command->type = node::COMMAND;
		command->operation = reserved_word::GOSUB;
		std::shared_ptr<parse_tree::node> target_line(new node);
		command->left = target_line;
		target_line->type = node::NUMBER;
		target_line->number = atof(line_number);

		return command;
		}

	/*
		PARSE_TREE::PARSE_RETURN()
		--------------------------
		RETURN
	*/
	std::shared_ptr<parse_tree::node> parse_tree::parse_return(void)
		{
		return parse_parameterless_statement(reserved_word::RETURN);
		}

	/*
		PARSE_TREE::PARSE_POP()
		-----------------------
		POP
	*/
	std::shared_ptr<parse_tree::node> parse_tree::parse_pop(void)
		{
		return parse_parameterless_statement(reserved_word::POP);
		}

	/*
		PARSE_TREE::PARSE_END()
		-----------------------
		END
	*/
	std::shared_ptr<parse_tree::node> parse_tree::parse_end(void)
		{
		return parse_parameterless_statement(reserved_word::END);
		}

	/*
		PARSE_TREE::PARSE_REM()
		-----------------------
		REM {character|"}
	*/
	std::shared_ptr<parse_tree::node> parse_tree::parse_rem(void)
		{
		return parse_parameterless_statement(reserved_word::REM);
		}


	/*
		PARSE_TREE::PARSE_PARAMETERLESS_STATEMENT()
		-------------------------------------------
		construct a node for a command that has no parameters (RETURN, END, etc.)
	*/
	std::shared_ptr<parse_tree::node> parse_tree::parse_parameterless_statement(const char *statement)
		{
		std::shared_ptr<parse_tree::node> command(new node);

		parser.get_next_token();
		command->type = node::COMMAND;
		command->operation = statement;
		return command;
		}

	/*
		PARSE_TREE::PARSE_IF()
		----------------------
		IF expr THEN instruction [{:instruction}]
		IF expr THEN [GOTO] linenum
		IF expr [THEN] GOTO linenum
	*/
	std::shared_ptr<parse_tree::node> parse_tree::parse_if(void)
		{
		std::shared_ptr<parse_tree::node> command(new node);

		parser.get_next_token();
		command->type = node::COMMAND;
		command->operation = reserved_word::IF;
		command->left = build();

		auto token = reserved_word::translate(parser.peek_next_token());

		if (token == reserved_word::THEN)
			{
			parser.get_next_token();			// THEN
			auto token = parser.peek_next_token();
			if (isdigit(*token))
				{
				token = parser.get_next_token();

				std::shared_ptr<parse_tree::node> target(new node);
				command->right = target;
				target->type = node::COMMAND;
				target->operation = reserved_word::GOTO;
				std::shared_ptr<parse_tree::node> target_line(new node);
				target->left = target_line;
				target_line->type = node::NUMBER;
				target_line->number = atof(token);
				}
			else
				command->right = build_command();
			}
		else if (token == reserved_word::GOTO)
			command->right = parse_goto();
		else
			throw error::syntax();

		return command;
		}

	/*
		PARSE_TREE::PARSE_LET()
		-----------------------
	*/
	std::shared_ptr<parse_tree::node> parse_tree::parse_let(bool can_be_parameterised)
		{
		std::shared_ptr<parse_tree::node> let(new node);
		let->type = node::COMMAND;
		let->operation = reserved_word::EQUALS;

		/*
			We've got a symbol so its an assignment statemnent.
		*/
		std::string variable = reserved_word::translate(parser.get_next_token());
		let->left = build_variable(variable, can_be_parameterised);

		auto equal = reserved_word::translate(parser.get_next_token());
		if (equal != reserved_word::EQUALS)
			throw error::syntax();

		let->right = build();

		return let;
		}

	/*
		PARSE_TREE::BUILD_COMMAND()
		---------------------------
	*/
	std::shared_ptr<parse_tree::node> parse_tree::build_command(void)
		{
		auto command = reserved_word::translate(parser.peek_next_token());
		if (command == reserved_word::PRINT)
			return parse_print();
		else if (command == reserved_word::QUESTIONMARK)
			return parse_print();
		else if (command == reserved_word::INPUT)
			return parse_input();
		else if (command == reserved_word::IF)
			return parse_if();
		else if (command == reserved_word::GOTO)
			return parse_goto();
		else if (command == reserved_word::GOSUB)
			return parse_gosub();
		else if (command == reserved_word::RETURN)
			return parse_return();
		else if (command == reserved_word::POP)
			return parse_pop();
		else if (command == reserved_word::END)
			return parse_end();
		else if (command == reserved_word::FOR)
			return parse_for();
		else if (command == reserved_word::NEXT)
			return parse_next();
		else if (command == reserved_word::REM)
			return parse_rem();
		else if (command == reserved_word::DIM)
			return parse_dim();
		else
			return parse_let();
		}

	/*
		PARSE_TREE::BUILD()
		-------------------
	*/
	std::shared_ptr<parse_tree::node> parse_tree::build(const std::string &string)
		{
		parser.set_document(string);
		return build_command();
		}

	/*
		OPERATOR<<()
		------------
	*/
	std::ostream &operator<<(std::ostream &stream, const std::shared_ptr<parse_tree::node> root)
		{
		if (root->left != nullptr || root->right != nullptr)
			std::cout << "(";

		if (root->left != nullptr)
			stream << root->left;

		if (root->type == parse_tree::node::SYMBOL)
			stream << root->symbol;
		else if (root->type == parse_tree::node::NUMBER)
			stream << root->number;
		else if (root->type == parse_tree::node::STRING)
			stream << '"' << root->string << '"';
		else if (root->type == parse_tree::node::OPERATOR)
			stream << root->operation;
		else if (root->type == parse_tree::node::COMMAND)
			{
			if (root->operation == reserved_word::PRINT)
				stream << "PRINT ";
			if (root->operation == reserved_word::IF)
				stream << "IF ";
			if (root->operation == reserved_word::EQUALS)
				stream << " = ";
			}

		if (root->right != nullptr)
			stream << root->right;

		if (root->left != nullptr || root->right != nullptr)
			std::cout << ")";

		return stream;
		}
	}
