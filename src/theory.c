/* ============================================================
 *  theory.c - Compiler Theory Demonstrations
 *
 *  Educational display of the formal theory behind each
 *  compiler phase used in this project.
 * ============================================================ */

#include <stdio.h>
#include "theory.h"
#include "utils.h"

/* -- CFG + FIRST / FOLLOW -- */
void theory_show_cfg(int colour) {
    if (colour) printf("%s", COL_CYAN);
    printf("\n");
    print_line_sep();
    printf("  COMPILER THEORY: Context-Free Grammar (CFG)\n");
    print_line_sep();
    if (colour) printf("%s", COL_RESET);

    printf("\n  Grammar G = (V, T, P, S)\n\n");
    printf("  Non-terminals V:\n");
    printf("    S, Cmd, Item, Qty\n\n");
    printf("  Terminals T:\n");
    printf("    'order' | 'cancel' | 'show' | 'clear'\n");
    printf("    identifier  (letters only)\n");
    printf("    number      (digits only)\n\n");
    printf("  Production rules P:\n");
    printf("    S    ->  Cmd\n");
    printf("    Cmd  ->  'order'  Item Qty\n");
    printf("    Cmd  ->  'cancel' Item Qty\n");
    printf("    Cmd  ->  'show'\n");
    printf("    Cmd  ->  'clear'\n");
    printf("    Item ->  identifier\n");
    printf("    Qty  ->  number\n\n");
    printf("  Start symbol: S\n");
    printf("\n  This is a Regular Grammar (Type 3 in Chomsky hierarchy).\n");
    printf("  Every production has the form  A -> aB  or  A -> a.\n");
    print_line_sep();
}

/* -- FIRST / FOLLOW sets -- */
void theory_show_first_follow(int colour) {
    if (colour) printf("%s", COL_CYAN);
    printf("\n");
    print_line_sep();
    printf("  COMPILER THEORY: FIRST and FOLLOW Sets\n");
    print_line_sep();
    if (colour) printf("%s", COL_RESET);

    printf("\n  FIRST sets (terminals that can begin a derivation):\n\n");
    printf("  %-10s  %s\n", "Symbol", "FIRST set");
    print_sub_sep();
    printf("  %-10s  { order, cancel, show, clear }\n", "S");
    printf("  %-10s  { order, cancel, show, clear }\n", "Cmd");
    printf("  %-10s  { identifier }\n",                  "Item");
    printf("  %-10s  { number }\n\n",                    "Qty");

    printf("  FOLLOW sets (terminals that can follow a non-terminal):\n\n");
    printf("  %-10s  %s\n", "Symbol", "FOLLOW set");
    print_sub_sep();
    printf("  %-10s  { $ }\n",          "S");
    printf("  %-10s  { $ }\n",          "Cmd");
    printf("  %-10s  { number }\n",     "Item");
    printf("  %-10s  { $ }\n\n",        "Qty");

    printf("  Usage in LL(1) parsing:\n");
    printf("  The parser reads Token[0] and selects the production rule\n");
    printf("  whose FIRST set contains that terminal.\n");
    printf("  e.g.  Token[0] = 'order'  ->  expand  Cmd -> order Item Qty\n");
    print_line_sep();
}

/* -- NFA -> DFA -- */
void theory_show_nfa_dfa(int colour) {
    if (colour) printf("%s", COL_CYAN);
    printf("\n");
    print_line_sep();
    printf("  COMPILER THEORY: NFA to DFA (Lexer Design)\n");
    print_line_sep();
    if (colour) printf("%s", COL_RESET);

    printf("\n  NFA for RestroDSL token recognition:\n\n");
    printf("  q0 --[o]--> q1 --[r]--> q2 --[d]--> q3\n");
    printf("              --[e]--> q4 --[r]--> q5  (ACCEPT: 'order')\n\n");
    printf("  q0 --[c]--> q6 --[a]--> q7 --[n]--> q8\n");
    printf("              --[c]--> q9 --[e]--> q10 --[l]--> q11 (ACCEPT: 'cancel')\n\n");
    printf("  q0 --[a-z]--> q12 --[a-z]*--> q12  (ACCEPT: IDENTIFIER)\n\n");
    printf("  q0 --[0-9]--> q13 --[0-9]*--> q13  (ACCEPT: NUMBER)\n\n");

    printf("  Subset Construction (NFA -> DFA):\n\n");
    printf("  DFA State  On Input         Next State   Accepts\n");
    print_sub_sep();
    printf("  D0         'order'          D1           KEYWORD\n");
    printf("  D0         'cancel'         D2           KEYWORD\n");
    printf("  D0         'show'/'clear'   D3           KEYWORD\n");
    printf("  D0         [a-z]+           D4           IDENTIFIER\n");
    printf("  D0         [0-9]+           D5           NUMBER\n");
    printf("  D0         other            D6           UNKNOWN\n\n");
    printf("  The DFA is implemented in lexer.c: classify() function.\n");
    print_line_sep();
}

/* -- LR(0) Parsing Table -- */
void theory_show_lr0(int colour) {
    if (colour) printf("%s", COL_CYAN);
    printf("\n");
    print_line_sep();
    printf("  COMPILER THEORY: LR(0) Canonical Items + Parse Table\n");
    print_line_sep();
    if (colour) printf("%s", COL_RESET);

    printf("\n  Augmented grammar:  S' -> S\n\n");
    printf("  Canonical LR(0) Item Sets:\n\n");
    printf("  I0:  S'  -> . S\n");
    printf("       S   -> . Cmd\n");
    printf("       Cmd -> . 'order' Item Qty\n");
    printf("       Cmd -> . 'cancel' Item Qty\n");
    printf("       Cmd -> . 'show'\n");
    printf("       Cmd -> . 'clear'\n\n");
    printf("  I1:  S' -> S .                    (ACCEPT)\n\n");
    printf("  I2:  Cmd -> 'order' . Item Qty\n");
    printf("       Item -> . identifier\n\n");
    printf("  I3:  Cmd -> 'order' Item . Qty\n");
    printf("       Qty -> . number\n\n");
    printf("  I4:  Cmd -> 'order' Item Qty .     (REDUCE r1)\n\n");
    printf("  I5:  Cmd -> 'cancel' . Item Qty    (similar to I2-I4)\n\n");
    printf("  I6:  Cmd -> 'show' .               (REDUCE r3)\n\n");
    printf("  I7:  Cmd -> 'clear' .              (REDUCE r4)\n\n");

    printf("  LR(0) ACTION / GOTO table (condensed):\n\n");
    printf("  State | order  | cancel | show | clear | id   | num  | $       | Cmd\n");
    print_sub_sep();
    printf("  0     | s2     | s5     | s6   | s7    |      |      |         | 1\n");
    printf("  1     |        |        |      |       |      |      | ACCEPT  |\n");
    printf("  2     |        |        |      |       | s3   |      |         |\n");
    printf("  3     |        |        |      |       |      | s4   |         |\n");
    printf("  4     |        |        |      |       |      |      | r1      |\n");
    printf("  5     |        |        |      |       | s..  |      |         |\n");
    printf("  6     |        |        |      |       |      |      | r3      |\n");
    printf("  7     |        |        |      |       |      |      | r4      |\n\n");
    printf("  s = shift, r = reduce, ACCEPT = success\n");
    printf("  Implemented in parser.c: parser_validate() function.\n");
    print_line_sep();
}

/* -- LL(1) Parse Table -- */
void theory_show_ll1(int colour) {
    if (colour) printf("%s", COL_CYAN);
    printf("\n");
    print_line_sep();
    printf("  COMPILER THEORY: LL(1) Predictive Parse Table\n");
    print_line_sep();
    if (colour) printf("%s", COL_RESET);

    printf("\n  LL(1) uses FIRST sets to select the production to expand.\n");
    printf("  One token of lookahead is sufficient (no ambiguity).\n\n");
    printf("  Parse Table  M[Non-terminal, Terminal]:\n\n");
    printf("  %-8s | %-25s | %-25s | %-10s | %-10s\n",
           "Non-term", "order", "cancel", "show", "clear");
    print_sub_sep();
    printf("  %-8s | %-25s | %-25s | %-10s | %-10s\n",
           "S",
           "S -> Cmd",
           "S -> Cmd",
           "S -> Cmd",
           "S -> Cmd");
    printf("  %-8s | %-25s | %-25s | %-10s | %-10s\n",
           "Cmd",
           "Cmd -> order Item Qty",
           "Cmd -> cancel Item Qty",
           "Cmd -> show",
           "Cmd -> clear");
    printf("  %-8s | %-25s | %-25s | %-10s | %-10s\n",
           "Item",
           "Item -> identifier",
           "Item -> identifier",
           "-",
           "-");
    printf("  %-8s | %-25s | %-25s | %-10s | %-10s\n",
           "Qty",
           "Qty -> number",
           "Qty -> number",
           "-",
           "-");
    printf("\n  Empty cells = error (unexpected token).\n");
    print_line_sep();
}

/* -- Three-address code IR walkthrough -- */
void theory_show_ir_example(const Session *sess, int colour) {
    if (colour) printf("%s", COL_CYAN);
    printf("\n");
    print_line_sep();
    printf("  COMPILER THEORY: Intermediate Representation (IR)\n");
    print_line_sep();
    if (colour) printf("%s", COL_RESET);

    printf("\n  In a compiler, the IR stores computed values between\n");
    printf("  analysis (front-end) and output (back-end).\n\n");
    printf("  Three-address code equivalent for current orders:\n\n");

    if (sess->order_count == 0) {
        printf("  (No orders in session - run some orders first.)\n");
    } else {
        int tmp = 1;
        double total = 0.0;
        for (int i = 0; i < sess->order_count; i++) {
            printf("  t%-3d = %.2f * %d          ;; %s unit_price * qty\n",
                   tmp, sess->orders[i].unit_price,
                   sess->orders[i].quantity,
                   sess->orders[i].item);
            tmp++;
            total += sess->orders[i].subtotal;
        }
        printf("  subtotal = ");
        for (int i = 0; i < sess->order_count; i++) {
            printf("t%d", i + 1);
            if (i < sess->order_count - 1) printf(" + ");
        }
        printf("   ;; sum all subtotals = %.2f\n", total);

        double disc = (total >= DISCOUNT_THRESH) ? total * DISCOUNT_RATE : 0.0;
        if (disc > 0.0) {
            printf("  t%-3d = subtotal * %.2f    ;; 10%% discount = %.2f\n",
                   tmp, DISCOUNT_RATE, disc);
            printf("  after_disc = subtotal - t%d\n", tmp);
            tmp++;
        } else {
            printf("  after_disc = subtotal\n");
        }
        double after = total - disc;
        printf("  t%-3d = after_disc * %.2f   ;; 5%% VAT = %.2f\n",
               tmp, TAX_RATE, after * TAX_RATE);
        printf("  grand_total = after_disc + t%d\n\n", tmp);
        printf("  Final grand_total = %.2f Tk\n", after + after * TAX_RATE);
    }
    printf("\n  This IR is stored in the Session->orders[] array (ir.h).\n");
    print_line_sep();
}
