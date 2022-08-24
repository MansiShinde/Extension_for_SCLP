DEST_DIR="sclp-level-6"
REG_DIR="regression-testing"
OTHER_SCRIPTS="extract-grammar-from-yacc-script.l join-lines-in-extracted-grammar.l print-rule-numbers.awk substitute-banner-lines.awk"

SHELL_SCRIPTS="build.sh check-compiler.sh extract-grammar.sh fetch-from-github.sh push-to-github.sh update-banner.sh create-tar-ball.sh build-resources.sh"

OTHER_FILES="README Makefile TODO"

PARSE_SCRIPTS="parser.y scanner.l"

HEADER_FILES="ast.hh common-headers.hh common-classes.hh error-display.hh procedure.hh program.hh reg-alloc.hh rtl.hh store-tokens.hh symbol-table.hh tac.hh user-options.hh versioning.hh basic-block.hh cfg.hh stacktrace.h lv-dfv.hh av-dfv.hh type-expressions.hh"

CPP_FILES="ast.cc error-display.cc main.cc procedure.cc procedure-compile.cc program.cc program-compile.cc reg-alloc.cc rtl.cc rtl-gen.cc store-tokens.cc symbol-table.cc symbol-table-compile.cc tac.cc tac-gen.cc type-expressions.cc type-checking.cc user-options.cc versioning.cc basic-block.cc cfg.cc cfg-gen.cc lv-dfv.cc lv-dfv-gen.cc av-dfv.cc av-dfv-gen.cc"

rm -rf $REG_DIR/test-output
rm -rf $DEST_DIR
mkdir $DEST_DIR
cp -r $REG_DIR $OTHER_SCRIPTS $SHELL_SCRIPTS $OTHER_FILES $PARSE_SCRIPTS $HEADER_FILES $CPP_FILES $DEST_DIR

tar cvfz $DEST_DIR.tgz $DEST_DIR

