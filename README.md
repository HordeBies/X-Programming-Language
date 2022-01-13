# X Programming Language
 A simple c like programming language compiler built using Lex and Yacc

# Requirements
1. GCC
2. Bison
3. Flex
4. make

# Compile

To compile language interpreter and run test file run this command using any terminal
````
make all src="{Path}"
````
e.g.
````
make all src="data/tests/test.c"
````

After compiling executable you can run this command to skip compiling step.
````
make run src="{Path}"
````
e.g.
````
make run src="data/tests/test.c"
````
