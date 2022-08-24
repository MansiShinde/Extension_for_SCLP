DEST_DIR="sclp-level-5"
REG_DIR="regression-testing"
AUX_SCRIPTS="extract-grammar-from-yacc-script.l substitute-banner-lines.awk build.sh check-compiler.sh extract-grammar.sh fetch-from-github.sh push-to-github.sh update-banner.sh create-tar-ball.sh README Makefile TODO"

PARSE_SCRIPTS="parser.y scanner.l"

HEADER_FILES="ast.hh common-classes.hh error-display.hh procedure.hh program.hh reg-alloc.hh rtl.hh store-tokens.hh symbol-table.hh tac.hh user-options.hh versioning.hh"

CPP_FILES="ast.cc error-display.cc main.cc procedure.cc procedure-compile.cc program.cc program-compile.cc reg-alloc.cc rtl.cc rtl-gen.cc store-tokens.cc symbol-table.cc symbol-table-compile.cc tac.cc tac-gen.cc type-checking.cc user-options.cc versioning.cc"

rm -rf $REG_DIR/test-output
rm -rf $DEST_DIR
mkdir $DEST_DIR
cp -r $REG_DIR $AUX_SCRIPTS $PARSE_SCRIPTS $HEADER_FILES $CPP_FILES $DEST_DIR

tar cvfz $DEST_DIR.tgz $DEST_DIR

