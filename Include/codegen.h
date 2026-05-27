#ifndef CODEGEN_H
#define CODEGEN_H

/* ============================================================
 *  codegen.h — Phase 5: Code Generation (Bill Output)
 *
 *  Takes the fully validated IR (order list) and produces:
 *    a) A formatted bill on stdout (console output)
 *    b) A saved bill in restaurant_bill.txt
 *    c) An appended entry in order_log.txt
 *
 *  In compiler terms: reads the IR, applies final
 *  calculations (discount + VAT), and emits the target output.
 * ============================================================ */

#include "types.h"

/* ── Public API ── */

/**
 * Compute the BillSummary from the current session orders.
 */
void  codegen_compute_summary(const Session *sess, BillSummary *bs);

/**
 * Print the formatted bill to stdout.
 */
void  codegen_print_bill(const Session *sess, const BillSummary *bs, int colour);

/**
 * Save the bill to BILL_FILENAME.
 * Returns 1 on success, 0 on file error.
 */
int   codegen_save_bill(const Session *sess, const BillSummary *bs);

/**
 * Append a one-line summary to the session log file.
 * Returns 1 on success, 0 on file error.
 */
int   codegen_append_log(const Session *sess, const BillSummary *bs);

/**
 * Run the full bill generation pipeline (compute + print + save + log).
 */
void  codegen_generate(Session *sess, int colour);

#endif /* CODEGEN_H */
