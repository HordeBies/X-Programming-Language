SHELL=CMD
src = "data/tests/test.c"
clear:
	cls

gen:
	flex -o rules/lexer.cpp rules/lexer.l
	bison -d --verbose -o rules/parser.cpp rules/parser.y

comp:
	g++ -w -g -o MPL.exe main.cpp \
		rules/lexer.cpp \
		rules/parser.cpp \
		parse_tree/branches/branch_analyzer.cpp \
		parse_tree/branches/branch_generator.cpp \
		parse_tree/expressions/expression_analyzer.cpp \
		parse_tree/expressions/expression_evaluator.cpp \
		parse_tree/expressions/expression_generator.cpp \
		parse_tree/functions/function_generator.cpp \
		parse_tree/functions/function_analyzer.cpp \
		parse_tree/statements/statement_analyzer.cpp \
		parse_tree/statements/statement_generator.cpp \

build:
#	@make -s clear
	@make -s gen
	@make -s comp

run:
#	@make -s clear
	./MPL.exe  $(src) -o data/out.quad -s data/symbol.table

all:
	@make -s build
	@make -s run