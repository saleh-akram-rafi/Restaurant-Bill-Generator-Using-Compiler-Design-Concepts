/* ============================================================
 *  parser.c - Phase 2: Syntax Analysis
 *
 *  Validates token stream against the grammar:
 *    S -> "order"  IDENTIFIER NUMBER
 *       | "cancel" IDENTIFIER NUMBER
 *       | "show"
 *       | "clear"
 *
 *  Simulates an LL(1) predictive parser with a simple
 *  look-ahead on Token[0] (the keyword).
 * ============================================================ */

#include <stdio.h>
#include <string.h>
#include "parser.h"
#include "utils.h"

const char *parser_command_name(CommandType t) {
    switch (t) {
        case CMD_ORDER:   return "ORDER";
        case CMD_CANCEL:  return "CANCEL";
        case CMD_SHOW:    return "SHOW";
        case CMD_CLEAR:   return "CLEAR";
        default:          return "UNKNOWN";
    }
}

/* Helper: format an error into the ParsedCommand */
static int syntax_error(ParsedCommand *cmd, const char *msg,
                        int colour) {
    cmd->valid = 0;
    strncpy(cmd->error_msg, msg, 127);
    if (colour) printf("%s", COL_RED);
    printf("  [PARSER] SYNTAX ERROR: %s\n", msg);
    if (colour) printf("%s", COL_RESET);
    return 0;
}

int parser_validate(const TokenStream *stream, ParsedCommand *cmd,
                    int debug, int colour) {
    cmd->valid    = 0;
    cmd->type     = CMD_UNKNOWN;
    cmd->item_name[0] = '\0';
    cmd->quantity = 0;
    cmd->error_msg[0] = '\0';

    if (stream->count == 0)
        return syntax_error(cmd, "Empty input.", colour);

    /* Token[0] must be a keyword */
    if (stream->tokens[0].type != TOK_KEYWORD)
        return syntax_error(cmd,
            "Commands must start with: order | cancel | show | clear", colour);

    char kw[MAX_NAME_LEN];
    str_to_lower(kw, stream->tokens[0].value, MAX_NAME_LEN);

    /* ── Dispatch on keyword ── */
    if (strcmp(kw, "show") == 0 || strcmp(kw, "menu") == 0) {
        if (stream->count > 1)
            return syntax_error(cmd, "'show' takes no arguments.", colour);
        cmd->type  = CMD_SHOW;
        cmd->valid = 1;
    }
    else if (strcmp(kw, "clear") == 0) {
        if (stream->count > 1)
            return syntax_error(cmd, "'clear' takes no arguments.", colour);
        cmd->type  = CMD_CLEAR;
        cmd->valid = 1;
    }
    else if (strcmp(kw, "order") == 0 || strcmp(kw, "cancel") == 0) {
        cmd->type = (strcmp(kw, "order") == 0) ? CMD_ORDER : CMD_CANCEL;

        /* Must have exactly 3 tokens: keyword IDENTIFIER NUMBER */
        if (stream->count < 2 || stream->tokens[1].type == TOK_EOF)
            return syntax_error(cmd, "Missing item name. Usage: order <item> <qty>", colour);
        if (stream->tokens[1].type != TOK_IDENTIFIER)
            return syntax_error(cmd, "Item name must contain letters only.", colour);
        if (stream->count < 3 || stream->tokens[2].type == TOK_EOF)
            return syntax_error(cmd, "Missing quantity. Usage: order <item> <qty>", colour);
        if (stream->tokens[2].type != TOK_NUMBER)
            return syntax_error(cmd, "Quantity must be a whole number (digits only).", colour);
        if (stream->extra_tokens > 0 || stream->count > 3)
            return syntax_error(cmd, "Too many tokens. Usage: order <item> <qty>", colour);

        strncpy(cmd->item_name, stream->tokens[1].value, MAX_NAME_LEN - 2);
        cmd->item_name[MAX_NAME_LEN - 2] = '\0';
        cmd->quantity = 0; /* parsed by semantic phase from token string */
        /* Store raw number string in item_name temporarily - no, use quantity */
        /* Actually store qty directly here for convenience */
        {
            const char *qstr = stream->tokens[2].value;
            int q = 0;
            for (int i = 0; qstr[i]; i++) q = q * 10 + (qstr[i] - '0');
            cmd->quantity = q;
        }
        cmd->valid = 1;
    }
    else {
        return syntax_error(cmd, "Unknown command. Use: order | cancel | show | clear", colour);
    }

    if (debug) {
        if (colour) printf("%s", COL_GREEN);
        printf("  [PARSER] Syntax OK -> Command: %s", parser_command_name(cmd->type));
        if (cmd->type == CMD_ORDER || cmd->type == CMD_CANCEL)
            printf("  item=\"%s\"  qty=%d", cmd->item_name, cmd->quantity);
        printf("\n");
        if (colour) printf("%s", COL_RESET);
    }
    return 1;
}
