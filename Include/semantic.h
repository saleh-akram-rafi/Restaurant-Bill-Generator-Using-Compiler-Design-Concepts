#ifndef SEMANTIC_H
#define SEMANTIC_H

/* ============================================================
 *  semantic.h — Phase 3: Semantic Analysis
 *
 *  Checks meaning beyond syntax:
 *    1. Item must exist in the symbol table (menu).
 *    2. Item must currently be available.
 *    3. Quantity must be a positive integer (> 0).
 *    4. For cancel: quantity must not exceed what was ordered.
 *
 *  Maps to a real compiler's type-checker and scope resolver.
 * ============================================================ */

#include "parser.h"
#include "symbol_table.h"
#include "types.h"

/* Output of semantic analysis — enriched command */
typedef struct {
    CommandType  cmd_type;
    char         item_key[MAX_NAME_LEN];    /* Lowercase key         */
    char         item_display[MAX_NAME_LEN];/* Capitalised display   */
    int          quantity;
    double       unit_price;
    int          valid;
    char         error_msg[128];
} SemanticResult;

/* ── Public API ── */

/**
 * Perform semantic analysis on a parsed command.
 * Uses the symbol table for lookups and the session for cancel checks.
 * Returns 1 on success, 0 on semantic error.
 */
int semantic_analyse(const ParsedCommand *cmd,
                     const SymbolTable   *st,
                     const Session       *sess,
                     SemanticResult      *result,
                     int debug, int colour);

#endif /* SEMANTIC_H */
