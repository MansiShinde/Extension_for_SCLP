lex extract-grammar-from-yacc-script.l
gcc lex.yy.c -ll
./a.out < parser.y 
rm -f lex.yy.c a.out

