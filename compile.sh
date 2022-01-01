if [ -z "$2" ]
    then
    echo "Too few arguments are provided"
    exit 1
fi
flex $1.l
bison -dy --verbose $2.y
g++ -w -g y.tab.c
if [ -n "$3" ]
    then
    ./a.exe ./$3.c
fi