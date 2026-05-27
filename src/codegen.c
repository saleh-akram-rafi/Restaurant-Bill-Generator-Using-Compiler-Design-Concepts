/* ============================================================
 *  codegen.c - Phase 5: Code Generation (Bill Output)
 *
 *  Reads the validated IR (order list), applies financial
 *  calculations, and emits the final restaurant bill to
 *  both stdout and a persistent text file.
 * ============================================================ */

#include <stdio.h>
#include <string.h>
#include "codegen.h"
#include "utils.h"

/* ── Compute summary ── */
void codegen_compute_summary(const Session *sess, BillSummary *bs) {
    bs->subtotal = 0.0;
    for (int i = 0; i < sess->order_count; i++)
        bs->subtotal += sess->orders[i].subtotal;

    bs->discount = (bs->subtotal >= DISCOUNT_THRESH)
                   ? bs->subtotal * DISCOUNT_RATE : 0.0;
    bs->after_discount = bs->subtotal - bs->discount;
    bs->tax             = bs->after_discount * TAX_RATE;
    bs->grand_total     = bs->after_discount + bs->tax;

    get_timestamp(bs->timestamp, (int)sizeof(bs->timestamp));
}

/* ── Print bill to stdout ── */
void codegen_print_bill(const Session *sess, const BillSummary *bs,
                        int colour) {
    if (colour) printf("%s%s", COL_BOLD, COL_YELLOW);
    printf("\n========================================================\n");
    printf("|            *   GRAND RESTAURANT   *                |\n");
    printf("|          Serving Excellence Since 2000              |\n");
    printf("========================================================\n");
    printf("|  Date : %-44s|\n", bs->timestamp);
    printf("========================================================\n");
    if (colour) printf("%s", COL_RESET);

    /* Column headers */
    if (colour) printf("%s", COL_CYAN);
    printf("|  %-18s %5s %10s %13s  |\n",
           "Item", "Qty", "Unit(Tk)", "Total(Tk)");
    printf("========================================================\n");
    if (colour) printf("%s", COL_RESET);

    /* Order rows */
    for (int i = 0; i < sess->order_count; i++) {
        printf("|  %-18s %5d %10.2f %13.2f  |\n",
               sess->orders[i].item,
               sess->orders[i].quantity,
               sess->orders[i].unit_price,
               sess->orders[i].subtotal);
    }

    printf("========================================================\n");
    printf("|  %-38s %13.2f  |\n", "Subtotal (Tk):",        bs->subtotal);
    if (bs->discount > 0.0)
        printf("|  %-38s %13.2f  |\n", "Discount 10%% (Tk):", -bs->discount);
    printf("|  %-38s %13.2f  |\n", "VAT 5%% (Tk):",          bs->tax);
    printf("========================================================\n");

    if (colour) printf("%s%s", COL_BOLD, COL_YELLOW);
    printf("|  %-38s %13.2f  |\n", "GRAND TOTAL (Tk):", bs->grand_total);
    if (colour) printf("%s", COL_RESET);

    printf("==============================================\n");
    printf("|         Thank you for dining with us!         |\n");
    printf("|              Please visit again  *            |\n");
    printf("==============================================\n\n");

    /* Cancellation summary */
    if (sess->cancel_count > 0) {
        if (colour) printf("%s", COL_YELLOW);
        printf("  Cancelled items this session:\n");
        for (int i = 0; i < sess->cancel_count; i++)
            printf("    - %s  × %d\n",
                   sess->cancels[i].item,
                   sess->cancels[i].qty_cancelled);
        if (colour) printf("%s", COL_RESET);
        printf("\n");
    }
}

/* ── Save bill to file ── */
int codegen_save_bill(const Session *sess, const BillSummary *bs) {
    FILE *fp = fopen(BILL_FILENAME, "w");
    if (!fp) return 0;

    fprintf(fp, "============================================================\n");
    fprintf(fp, "              GRAND RESTAURANT\n");
    fprintf(fp, "         Serving Excellence Since 2000\n");
    fprintf(fp, "============================================================\n");
    fprintf(fp, "  Date : %s\n", bs->timestamp);
    fprintf(fp, "------------------------------------------------------------\n");
    fprintf(fp, "  %-18s %5s %10s %13s\n",
            "Item", "Qty", "Unit(Tk)", "Total(Tk)");
    fprintf(fp, "------------------------------------------------------------\n");

    for (int i = 0; i < sess->order_count; i++) {
        fprintf(fp, "  %-18s %5d %10.2f %13.2f\n",
                sess->orders[i].item,
                sess->orders[i].quantity,
                sess->orders[i].unit_price,
                sess->orders[i].subtotal);
    }

    fprintf(fp, "------------------------------------------------------------\n");
    fprintf(fp, "  %-38s %13.2f\n", "Subtotal (Tk):",        bs->subtotal);
    if (bs->discount > 0.0)
        fprintf(fp, "  %-38s %13.2f\n", "Discount 10%% (Tk):", -bs->discount);
    fprintf(fp, "  %-38s %13.2f\n", "VAT 5%% (Tk):",          bs->tax);
    fprintf(fp, "============================================================\n");
    fprintf(fp, "  %-38s %13.2f\n", "GRAND TOTAL (Tk):", bs->grand_total);
    fprintf(fp, "============================================================\n");
    fprintf(fp, "  Thank you for dining with us! Please visit again.\n");
    fprintf(fp, "============================================================\n");

    if (sess->cancel_count > 0) {
        fprintf(fp, "\n  Cancelled items:\n");
        for (int i = 0; i < sess->cancel_count; i++)
            fprintf(fp, "    - %s × %d\n",
                    sess->cancels[i].item,
                    sess->cancels[i].qty_cancelled);
    }

    fclose(fp);
    return 1;
}

/* ── Append one summary line to the session log ── */
int codegen_append_log(const Session *sess, const BillSummary *bs) {
    FILE *fp = fopen(LOG_FILENAME, "a");
    if (!fp) return 0;
    fprintf(fp, "[%s]  orders=%d  errors=%d  subtotal=%.2f"
                "  discount=%.2f  tax=%.2f  total=%.2f\n",
            bs->timestamp,
            sess->order_count,
            sess->error_count,
            bs->subtotal,
            bs->discount,
            bs->tax,
            bs->grand_total);
    fclose(fp);
    return 1;
}

/* ── Full pipeline ── */
void codegen_generate(Session *sess, int colour) {
    if (sess->order_count == 0) {
        if (colour) printf("%s", COL_RED);
        printf("  [CODEGEN] No valid orders to bill.\n");
        if (colour) printf("%s", COL_RESET);
        return;
    }

    BillSummary bs;
    codegen_compute_summary(sess, &bs);
    codegen_print_bill(sess, &bs, colour);

    if (codegen_save_bill(sess, &bs)) {
        if (colour) printf("%s", COL_GREEN);
        printf("  [CODEGEN] Bill saved to -> %s\n", BILL_FILENAME);
        if (colour) printf("%s", COL_RESET);
    } else {
        if (colour) printf("%s", COL_RED);
        printf("  [CODEGEN] WARNING: Could not save bill to file.\n");
        if (colour) printf("%s", COL_RESET);
    }

    if (codegen_append_log(sess, &bs)) {
        if (colour) printf("%s", COL_GREEN);
        printf("  [CODEGEN] Session logged to  -> %s\n", LOG_FILENAME);
        if (colour) printf("%s", COL_RESET);
    }
}
