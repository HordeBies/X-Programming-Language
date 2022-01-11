# if [ -z "$2" ]
#     then
#     echo "Too few arguments are provided"
#     exit 1
# fi
flex lexer.l
bison -dy --verbose parser.y
g++ -w -g y.tab.c lex.yy.c node_base.hpp
if [ -n "$1" ]
    then
    ./a.exe ./$1.c
fi