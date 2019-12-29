/*
	ERROR.H
	-------
*/
#pragma once

namespace BASIC
	{
	/*
		CLASS ERROR
		-----------
	*/
	class error
		{
		public:
			class syntax 							{ /* Nothing */ };
			class division_by_zero 				{ /* Nothing */ };
			class runtime 							{ /* Nothing */ };
			class extra_ignored 					{ /* Nothing */ };
			class reenter 							{ /* Nothing */ };
			class type_mismatch 					{ /* Nothing */ };
			class undefined_statement 			{ /* Nothing */ };
			class next_without_for	 			{ /* Nothing */ };
			class return_without_gosub	 		{ /* Nothing */ };
			class bad_subscript_error	 		{ /* Nothing */ };
			class out_of_data_error		 		{ /* Nothing */ };
		};
	}
