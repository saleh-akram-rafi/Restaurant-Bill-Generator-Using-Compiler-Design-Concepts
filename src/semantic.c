/* ============================================================
 *  semantic.c - Phase 3: Semantic Analysis
 *
 *  Checks meaning of syntactically valid commands:
 *    1. Item must exist in symbol table (menu).
 *    2. Item must be marked available.
 *    3. Quantity must be positive (> 0).
 *    4. Cancel quantity must not exceed what was ordered.
 * ============================================================ */

#include <stdio.h>
#include <string.h>
#include "semantic.h"
#include "utils.h"

static int sem_error(SemanticResult *r, const char *msg, int colour) {
    r->valid = 0;
    strncpy(r->error_msg, msg, 127);
    if (colour) printf("%s", COL_RED);
    printf("  [SEMANTIC] ERROR: %s\n", msg);
    if (colour) printf("%s", COL_RESET);
    return 0;
}

int semantic_analyse(const ParsedCommand *cmd,
                     const SymbolTable   *st,
                     const Session       *sess,
                     SemanticResult      *result,
                     int debug, int colour) {
    result->valid      = 0;
    result->cmd_type   = cmd->type;
    result->quantity   = cmd->quantity;
    result->unit_price = 0.0;
    result->error_msg[0] = '\0';

    /* SHOW / CLEAR have no items to validate */
    if (cmd->type == CMD_SHOW || cmd->type == CMD_CLEAR) {
        result->valid = 1;
        return 1;
    }

    /* ── Normalise item name ── */
    str_to_lower(result->item_key, cmd->item_name, MAX_NAME_LEN);

    /* ── Check 1: item exists in symbol table ── */
    double price = symtab_lookup(st, result->item_key);
    if (price < 0.0) {
        char msg[128];
        snprintf(msg, sizeof(msg),
                 "'%s' is not on the menu. Type 'show' to see available items.",
                 cmd->item_name);
        return sem_error(result, msg, colour);
    }
    result->unit_price = price;

    /* ── Check 2: quantity > 0 ── */
    if (cmd->quantity <= 0)
        return sem_error(result, "Quantity must be greater than 0.", colour);

    /* ── Check 3 (cancel only): can't cancel more than ordered ── */
    if (cmd->type == CMD_CANCEL) {
        int ordered = 0;
        for (int i = 0; i < sess->order_count; i++) {
            char lower[MAX_NAME_LEN];
            str_to_lower(lower, sess->orders[i].item, MAX_NAME_LEN);
            if (strcmp(lower, result->item_key) == 0) {
                ordered = sess->orders[i].quantity;
                break;
            }
        }
        if (ordered == 0) {
            char msg[128];
            snprintf(msg, sizeof(msg),
                     "'%s' has not been ordered yet.", cmd->item_name);
            return sem_error(result, msg, colour);
        }
        if (cmd->quantity > ordered) {
            char msg[128];
            snprintf(msg, sizeof(msg),
                     "Cannot cancel %d × %s - only %d ordered.",
                     cmd->quantity, cmd->item_name, ordered);
            return sem_error(result, msg, colour);
        }
    }

    /* ── Set display name ── */
    const char *disp = symtab_display_name(st, result->item_key);
    strncpy(result->item_display,
            disp ? disp : cmd->item_name, MAX_NAME_LEN - 2);
    result->item_display[MAX_NAME_LEN - 2] = '\0';

    result->valid = 1;

    if (debug) {
        if (colour) printf("%s", COL_GREEN);
        printf("  [SEMANTIC] OK -> \"%s\"  price=%.2f  qty=%d\n",
               result->item_display, result->unit_price, result->quantity);
        if (colour) printf("%s", COL_RESET);
    }
    return 1;
}
