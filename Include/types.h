#ifndef TYPES_H
#define TYPES_H

/* ============================================================
 *  types.h — All shared data structures for RestroDSL
 *  Each struct maps directly to a compiler design concept.
 * ============================================================ */

#include "config.h"

/* ── Token (Lexer output) ── */
typedef struct {
    int  type;                    /* TOK_* constant              */
    char value[MAX_NAME_LEN];     /* Actual string value         */
    int  line_no;                 /* Source line number          */
    int  col_no;                  /* Column position             */
} Token;

/* ── Symbol Table entry ── */
typedef struct {
    char   name[MAX_NAME_LEN];    /* Lowercase item key          */
    char   display[MAX_NAME_LEN]; /* Capitalised display name    */
    double price;                 /* Unit price in Taka          */
    int    available;             /* 1 = on menu, 0 = 86'd       */
} SymbolEntry;

/* ── Symbol Table ── */
typedef struct {
    SymbolEntry entries[MAX_ITEMS];
    int         count;
} SymbolTable;

/* ── Order record (Intermediate Representation) ── */
typedef struct {
    char   item[MAX_NAME_LEN];    /* Capitalised display name    */
    int    quantity;
    double unit_price;
    double subtotal;
} OrderRecord;

/* ── Cancel log entry ── */
typedef struct {
    char item[MAX_NAME_LEN];
    int  qty_cancelled;
} CancelEntry;

/* ── Session state (aggregates all live data) ── */
typedef struct {
    OrderRecord orders[MAX_ORDERS];
    int         order_count;

    CancelEntry cancels[MAX_CANCEL_LOG];
    int         cancel_count;

    char        history[MAX_HISTORY][MAX_LINE_LEN]; /* Raw input history */
    int         history_count;

    int         error_count;
    int         debug_mode;       /* 1 = verbose phase output    */
    int         colour_mode;      /* 1 = ANSI colour enabled     */
} Session;

/* ── Parse result (unified return from compiler pipeline) ── */
typedef enum {
    PARSE_OK = 0,
    PARSE_ERR_LEXER,
    PARSE_ERR_SYNTAX,
    PARSE_ERR_SEMANTIC,
    PARSE_ERR_OVERFLOW
} ParseResult;

/* ── Bill summary (code generation output) ── */
typedef struct {
    double subtotal;
    double discount;
    double after_discount;
    double tax;
    double grand_total;
    char   timestamp[64];
} BillSummary;

#endif /* TYPES_H */
