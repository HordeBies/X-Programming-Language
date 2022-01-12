# # if [ -z "$2" ]
# #     then
# #     echo "Too few arguments are provided"
# #     exit 1
# # fi
# flex lexer.l
# bison -dy --verbose parser.y
# g++ -w -g y.tab.c lex.yy.c
# if [ -n "$1" ]
#     then
#     ./a.exe ./$1.c
# fi

flex -o rules/lexer.cpp rules/lexer.l
bison -d --verbose -o rules/parser.cpp rules/parser.y

g++ -w -g -o MPL.exe main.cpp rules/lexer.cpp rules/parser.cpp

if [ -n "$1" ]
    then
    ./MPL.exe ./$1.c
fi