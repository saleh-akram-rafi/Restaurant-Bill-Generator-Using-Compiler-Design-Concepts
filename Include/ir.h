#ifndef IR_H
#define IR_H

/* ============================================================
 *  ir.h — Intermediate Representation: Order Manager
 *
 *  After semantic analysis, validated commands are stored in
 *  the IR (the session's order list).  This mirrors a real
 *  compiler's IR (three-address code / AST nodes) that sits
 *  between analysis and code generation.
 *
 *  Operations:
 *    ir_add_order    — append or merge an order record
 *    ir_cancel_order — reduce or remove an order record
 *    ir_clear        — wipe all records
 *    ir_find         — search for an item by key
 * ============================================================ */

#include "types.h"
#include "semantic.h"

/* ── Public API ── */

/**
 * Apply a validated semantic result to the session's IR.
 * Handles both CMD_ORDER (add/merge) and CMD_CANCEL (reduce).
 * Returns 1 on success, 0 on failure (e.g. table full).
 */
int  ir_apply(Session *sess, const SemanticResult *sr, int colour);

/**
 * Find an order record index by item key. Returns -1 if not found.
 */
int  ir_find(const Session *sess, const char *item_key);

/**
 * Print the current IR (order list) as a preview table.
 */
void ir_print_preview(const Session *sess, int colour);

/**
 * Clear all order records and cancel log.
 */
void ir_clear(Session *sess);

#endif /* IR_H */
