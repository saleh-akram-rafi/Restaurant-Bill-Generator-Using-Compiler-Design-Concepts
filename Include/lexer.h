#ifndef LEXER_H
#define LEXER_H

/* ============================================================
 *  lexer.h — Phase 1: Lexical Analysis
 *
 *  Converts a raw input line into a stream of Tokens.
 *  Grammar terminal symbols recognised:
 *    KEYWORD    : "order" | "cancel" | "show" | "clear"
 *    IDENTIFIER : [a-zA-Z]+
 *    NUMBER     : [0-9]+
 *    UNKNOWN    : anything else
 *    EOF        : token slot is empty
 * ============================================================ */

#include "types.h"

/* Maximum tokens produced per line */
#define MAX_TOKENS  8

/* Token stream produced by the lexer for one input line */
typedef struct {
    Token tokens[MAX_TOKENS];
    int   count;                  /* Number of tokens found      */
    int   extra_tokens;           /* >0 means too many tokens    */
} TokenStream;

/* ── Public API ── */

/**
 * Tokenise a single input line.
 * Fills `stream` and returns the number of tokens found.
 * Sets stream->extra_tokens if more than MAX_TOKENS were present.
 */
int  lexer_tokenise(const char *line, TokenStream *stream, int debug, int colour);

/**
 * Return a human-readable name for a token type constant.
 */
const char *lexer_token_type_name(int type);

/**
 * Print the full token stream (debug helper).
 */
void lexer_print_stream(const TokenStream *stream, int colour);

#endif /* LEXER_H */
