#
# CMAKELISTS.TXT
# --------------
#

set(BASIClib_FILES
	array.h
	error.h
	executive.h
	executive.cpp
	parser.h
	parser.cpp
	parse_tree.h
	parse_tree.cpp
	parse_tree_node.h
	reserved_word.h
	reserved_word.cpp
	symbol.h
	symbol_table.h
	symbol_table.cpp
	)

add_library(BASIClib ${BASIClib_FILES})
include_directories(.)

source_group ("Source Files" FILES ${BASIClib_FILES})
