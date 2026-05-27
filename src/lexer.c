/* ============================================================
 *  lexer.c - Phase 1: Lexical Analysis
 *
 *  Implements a hand-written DFA-based tokeniser.
 *  Recognised keywords: order, cancel, show, clear, done, help
 *  All other alphabetic strings -> IDENTIFIER
 *  Pure digit strings -> NUMBER
 *  Anything else -> UNKNOWN
 * ============================================================ */

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "lexer.h"
#include "utils.h"

/* Keywords table */
static const char *KEYWORDS[] = {
    "order", "cancel", "show", "clear", "done", "help", "menu", NULL
};

static int is_keyword(const char *s) {
    char lower[MAX_NAME_LEN];
    str_to_lower(lower, s, MAX_NAME_LEN);
    for (int i = 0; KEYWORDS[i]; i++)
        if (strcmp(lower, KEYWORDS[i]) == 0) return 1;
    return 0;
}

/* Classify a single raw word into a token */
static int classify(const char *word) {
    if (is_keyword(word))       return TOK_KEYWORD;
    if (str_is_alpha(word))     return TOK_IDENTIFIER;
    if (str_is_digit(word))     return TOK_NUMBER;
    return TOK_UNKNOWN;
}

int lexer_tokenise(const char *line, TokenStream *stream, int debug, int colour) {
    stream->count        = 0;
    stream->extra_tokens = 0;

    /* Copy line so we can mutate it with strtok */
    char buf[MAX_LINE_LEN];
    strncpy(buf, line, MAX_LINE_LEN - 1);
    buf[MAX_LINE_LEN - 1] = '\0';
    str_trim(buf);

    if (buf[0] == '\0' || buf[0] == '#') /* empty or comment */
        return 0;

    int col = 1;
    char *tok = strtok(buf, " \t");
    while (tok) {
        if (stream->count < MAX_TOKENS) {
            Token *t = &stream->tokens[stream->count];
            t->type    = classify(tok);
            strncpy(t->value, tok, MAX_NAME_LEN - 1);
            t->value[MAX_NAME_LEN - 1] = '\0';
            t->col_no  = col;
            t->line_no = 0; /* set by caller if needed */
            stream->count++;
        } else {
            stream->extra_tokens++;
        }
        col += (int)strlen(tok) + 1;
        tok = strtok(NULL, " \t");
    }

    if (debug) {
        if (colour) printf("%s", COL_CYAN);
        printf("  [LEXER] Input  : \"%s\"\n", line);
        lexer_print_stream(stream, colour);
        if (stream->extra_tokens > 0)
            printf("  [LEXER] WARNING: %d extra token(s) ignored\n",
                   stream->extra_tokens);
        if (colour) printf("%s", COL_RESET);
    }
    return stream->count;
}

const char *lexer_token_type_name(int type) {
    switch (type) {
        case TOK_KEYWORD:    return "KEYWORD";
        case TOK_IDENTIFIER: return "IDENTIFIER";
        case TOK_NUMBER:     return "NUMBER";
        case TOK_UNKNOWN:    return "UNKNOWN";
        case TOK_EOF:        return "EOF";
        default:             return "?";
    }
}

void lexer_print_stream(const TokenStream *stream, int colour) {
    for (int i = 0; i < stream->count; i++) {
        const Token *t = &stream->tokens[i];
        if (colour) {
            const char *col = COL_RESET;
            if (t->type == TOK_KEYWORD)    col = COL_YELLOW;
            if (t->type == TOK_IDENTIFIER) col = COL_GREEN;
            if (t->type == TOK_NUMBER)     col = COL_CYAN;
            if (t->type == TOK_UNKNOWN)    col = COL_RED;
            printf("  [LEXER] Token[%d] %s%-10s%s -> \"%s\"\n",
                   i, col, lexer_token_type_name(t->type), COL_RESET, t->value);
        } else {
            printf("  [LEXER] Token[%d] %-10s -> \"%s\"\n",
                   i, lexer_token_type_name(t->type), t->value);
        }
    }
}
