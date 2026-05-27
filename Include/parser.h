#ifndef PARSER_H
#define PARSER_H

/* ============================================================
 *  parser.h — Phase 2: Syntax Analysis
 *
 *  Validates that the token stream conforms to the grammar:
 *
 *  S  -> "order"  IDENTIFIER  NUMBER
 *      | "cancel" IDENTIFIER  NUMBER
 *      | "show"
 *      | "clear"
 *
 *  Recognised commands:
 *    CMD_ORDER   — add items
 *    CMD_CANCEL  — remove items
 *    CMD_SHOW    — display current bill preview
 *    CMD_CLEAR   — reset session
 *    CMD_UNKNOWN — syntax error
 * ============================================================ */

#include "lexer.h"

/* Parsed command types */
typedef enum {
    CMD_ORDER   = 0,
    CMD_CANCEL  = 1,
    CMD_SHOW    = 2,
    CMD_CLEAR   = 3,
    CMD_UNKNOWN = 4
} CommandType;

/* Parsed command (output of syntax analysis) */
typedef struct {
    CommandType type;
    char        item_name[MAX_NAME_LEN];  /* From IDENTIFIER token  */
    int         quantity;                 /* From NUMBER token      */
    int         valid;                    /* 1 = syntax OK          */
    char        error_msg[128];           /* Set if valid == 0      */
} ParsedCommand;

/* ── Public API ── */

/**
 * Validate the token stream and populate a ParsedCommand.
 * Returns 1 on success, 0 on syntax error.
 */
int  parser_validate(const TokenStream *stream, ParsedCommand *cmd,
                     int debug, int colour);

/**
 * Return a string name for a CommandType.
 */
const char *parser_command_name(CommandType t);

#endif /* PARSER_H */
