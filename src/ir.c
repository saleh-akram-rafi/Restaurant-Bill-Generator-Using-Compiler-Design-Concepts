/* ============================================================
 *  ir.c - Intermediate Representation: Order Manager
 *
 *  Maintains the session's order list (the IR). Supports:
 *    - add/merge  (CMD_ORDER)
 *    - reduce/remove (CMD_CANCEL)
 *    - preview    (print current state)
 *    - clear      (wipe all records)
 * ============================================================ */

#include <stdio.h>
#include <string.h>
#include "ir.h"
#include "utils.h"

int ir_find(const Session *sess, const char *item_key) {
    for (int i = 0; i < sess->order_count; i++) {
        char lower[MAX_NAME_LEN];
        str_to_lower(lower, sess->orders[i].item, MAX_NAME_LEN);
        if (strcmp(lower, item_key) == 0) return i;
    }
    return -1;
}

int ir_apply(Session *sess, const SemanticResult *sr, int colour) {
    if (!sr->valid) return 0;

    /* SHOW and CLEAR are handled by the pipeline/UI, not IR */
    if (sr->cmd_type == CMD_SHOW || sr->cmd_type == CMD_CLEAR) return 1;

    if (sr->cmd_type == CMD_ORDER) {
        int idx = ir_find(sess, sr->item_key);
        if (idx >= 0) {
            /* Merge into existing record */
            sess->orders[idx].quantity += sr->quantity;
            sess->orders[idx].subtotal  =
                sess->orders[idx].quantity * sess->orders[idx].unit_price;
            if (colour) printf("%s", COL_GREEN);
            printf("  [IR] Merged -> %s  (total qty: %d)\n",
                   sr->item_display, sess->orders[idx].quantity);
            if (colour) printf("%s", COL_RESET);
        } else {
            /* Add new record */
            if (sess->order_count >= MAX_ORDERS) {
                if (colour) printf("%s", COL_RED);
                printf("  [IR] ERROR: Order list is full (max %d).\n", MAX_ORDERS);
                if (colour) printf("%s", COL_RESET);
                return 0;
            }
            OrderRecord *rec = &sess->orders[sess->order_count++];
            snprintf(rec->item, MAX_NAME_LEN, "%s", sr->item_display);
            rec->quantity   = sr->quantity;
            rec->unit_price = sr->unit_price;
            rec->subtotal   = sr->quantity * sr->unit_price;
            if (colour) printf("%s", COL_GREEN);
            printf("  [IR] Added -> %s x %d = Tk %.2f\n",
                   rec->item, rec->quantity, rec->subtotal);
            if (colour) printf("%s", COL_RESET);
        }
    }
    else if (sr->cmd_type == CMD_CANCEL) {
        int idx = ir_find(sess, sr->item_key);
        if (idx < 0) return 0; /* Should be caught by semantic, but be safe */

        sess->orders[idx].quantity -= sr->quantity;
        if (sess->orders[idx].quantity <= 0) {
            /* Remove record by compacting array */
            for (int i = idx; i < sess->order_count - 1; i++)
                sess->orders[i] = sess->orders[i + 1];
            sess->order_count--;
            if (colour) printf("%s", COL_YELLOW);
            printf("  [IR] Removed -> %s (all units cancelled)\n", sr->item_display);
            if (colour) printf("%s", COL_RESET);
        } else {
            sess->orders[idx].subtotal =
                sess->orders[idx].quantity * sess->orders[idx].unit_price;
            if (colour) printf("%s", COL_YELLOW);
            printf("  [IR] Reduced -> %s × %d remaining\n",
                   sr->item_display, sess->orders[idx].quantity);
            if (colour) printf("%s", COL_RESET);
        }
        /* Log the cancel */
        if (sess->cancel_count < MAX_CANCEL_LOG) {
            snprintf(sess->cancels[sess->cancel_count].item,
                     MAX_NAME_LEN, "%s", sr->item_display);
            sess->cancels[sess->cancel_count].qty_cancelled = sr->quantity;
            sess->cancel_count++;
        }
    }
    return 1;
}

void ir_print_preview(const Session *sess, int colour) {
    if (sess->order_count == 0) {
        printf("  (No orders yet.)\n");
        return;
    }
    printf("\n");
    if (colour) printf("%s", COL_CYAN);
    printf("  %-18s %6s %10s %12s\n",
           "Item", "Qty", "Unit(Tk)", "Total(Tk)");
    printf("  ");
    for (int i = 0; i < 50; i++) printf("-");
    printf("\n");
    if (colour) printf("%s", COL_RESET);

    double subtotal = 0.0;
    for (int i = 0; i < sess->order_count; i++) {
        printf("  %-18s %6d %10.2f %12.2f\n",
               sess->orders[i].item,
               sess->orders[i].quantity,
               sess->orders[i].unit_price,
               sess->orders[i].subtotal);
        subtotal += sess->orders[i].subtotal;
    }
    printf("  ");
    for (int i = 0; i < 50; i++) printf("-");
    if (colour) printf("%s", COL_YELLOW);
    printf("\n  %-36s %12.2f\n", "Current Subtotal (Tk):", subtotal);
    if (colour) printf("%s", COL_RESET);
    printf("\n");
}

void ir_clear(Session *sess) {
    sess->order_count  = 0;
    sess->cancel_count = 0;
    sess->error_count  = 0;
}
