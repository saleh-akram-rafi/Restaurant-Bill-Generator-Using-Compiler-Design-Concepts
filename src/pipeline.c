/* ============================================================
 *  pipeline.c - Full Compiler Pipeline Controller
 *
 *  Chains all phases in sequence:
 *    Lexer -> Parser -> Semantic -> IR
 *
 *  This is the "driver" function that processes one input line.
 * ============================================================ */

#include <stdio.h>
#include <string.h>
#include "pipeline.h"
#include "lexer.h"
#include "parser.h"
#include "semantic.h"
#include "ir.h"
#include "codegen.h"
#include "utils.h"

ParseResult pipeline_process_line(const char  *line,
                                   int          line_no,
                                   Session     *sess,
                                   SymbolTable *st) {
    int dbg = sess->debug_mode;
    int col = sess->colour_mode;

    if (dbg) {
        printf("\n");
        if (col) printf("%s", COL_BOLD);
        printf("-Line %-3d", line_no);
        for (int i = 0; i < 46; i++) printf("-");
        printf("--\n");
        printf("| Input: %-51s|\n", line);
        printf("--");
        for (int i = 0; i < 58; i++) printf("-");
        printf("\n");
        if (col) printf("%s", COL_RESET);
    }

    /* -- Record in history -- */
    if (sess->history_count < MAX_HISTORY)
        strncpy(sess->history[sess->history_count++],
                line, MAX_LINE_LEN - 1);

    /* ══ PHASE 1: LEXICAL ANALYSIS ══ */
    TokenStream stream;
    int tok_count = lexer_tokenise(line, &stream, dbg, col);
    if (tok_count == 0) {
        sess->error_count++;
        return PARSE_ERR_LEXER;
    }

    /* ══ PHASE 2: SYNTAX ANALYSIS ══ */
    ParsedCommand cmd;
    if (!parser_validate(&stream, &cmd, dbg, col)) {
        sess->error_count++;
        return PARSE_ERR_SYNTAX;
    }

    /* -- Handle non-order commands immediately -- */
    if (cmd.type == CMD_SHOW) {
        ir_print_preview(sess, col);
        return PARSE_OK;
    }
    if (cmd.type == CMD_CLEAR) {
        ir_clear(sess);
        if (col) printf("%s", COL_YELLOW);
        printf("  [PIPELINE] Session cleared.\n");
        if (col) printf("%s", COL_RESET);
        return PARSE_OK;
    }

    /* ══ PHASE 3: SEMANTIC ANALYSIS ══ */
    SemanticResult sr;
    if (!semantic_analyse(&cmd, st, sess, &sr, dbg, col)) {
        sess->error_count++;
        return PARSE_ERR_SEMANTIC;
    }

    /* ══ PHASE 4: IR UPDATE ══ */
    if (!ir_apply(sess, &sr, col)) {
        sess->error_count++;
        return PARSE_ERR_OVERFLOW;
    }

    if (dbg) {
        if (col) printf("%s", COL_GREEN);
        printf("  [PIPELINE] Line %d -> OK\n", line_no);
        if (col) printf("%s", COL_RESET);
    }
    return PARSE_OK;
}
