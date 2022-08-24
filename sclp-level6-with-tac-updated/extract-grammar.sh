# REQUIREMENTS:
# - All actions in the yacc script must begin on a new line for this script 
#   to produce new lines correctly in the output. 
# - There should be no C style block comments of grammar rules
#
lex extract-grammar-from-yacc-script.l
gcc lex.yy.c -ll
./a.out < parser.y >| temp.txt
lex join-lines-in-extracted-grammar.l
gcc lex.yy.c -ll
./a.out < temp.txt | awk -f print-rule-numbers.awk 
rm -f lex.yy.c a.out #temp.txt

