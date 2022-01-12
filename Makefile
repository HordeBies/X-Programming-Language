SHELL=CMD
src = "data/tests/test.c"
clear:
	cls

gen:
	flex -o rules/lexer.cpp rules/lexer.l
	bison -d -o rules/parser.cpp rules/parser.y
	g++ -w -g -o MPL.exe main.cpp \
		rules/lexer.cpp \
		rules/parser.cpp

build:
#	@make -s clear
	@make -s gen

run:
#	@make -s clear
	./MPL.exe  $(src) -o data/out.quad -s data/symbol.table

all:
	@make -s build
	@make -s run