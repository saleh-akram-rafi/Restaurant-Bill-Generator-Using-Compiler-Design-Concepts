#ifndef CONFIG_H
#define CONFIG_H

/* ============================================================
 *  config.h — Project-wide constants and configuration
 *  RestroDSL: Restaurant Bill Generator (Compiler Design)
 * ============================================================ */

/* ── Capacity limits ── */
#define MAX_ITEMS          30
#define MAX_NAME_LEN       40
#define MAX_LINE_LEN      128
#define MAX_ORDERS         60
#define MAX_CANCEL_LOG     20
#define MAX_HISTORY        80

/* ── Financial rates ── */
#define TAX_RATE          0.05    /* 5% VAT                         */
#define DISCOUNT_RATE     0.10    /* 10% loyalty discount           */
#define DISCOUNT_THRESH  1000.0   /* Discount kicks in above this   */

/* ── Token type constants ── */
#define TOK_KEYWORD      0
#define TOK_IDENTIFIER   1
#define TOK_NUMBER       2
#define TOK_UNKNOWN      3
#define TOK_EOF          4

/* ── ANSI colour codes ── */
#define COL_RED     "\033[31m"
#define COL_GREEN   "\033[32m"
#define COL_YELLOW  "\033[33m"
#define COL_CYAN    "\033[36m"
#define COL_MAGENTA "\033[35m"
#define COL_BOLD    "\033[1m"
#define COL_RESET   "\033[0m"

/* ── Bill filename ── */
#define BILL_FILENAME  "restaurant_bill.txt"
#define LOG_FILENAME   "order_log.txt"

/* ── Version string ── */
#define VERSION        "2.0.0"
#define APP_NAME       "RestroDSL"

#endif /* CONFIG_H */
