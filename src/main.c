/* ============================================================
 *  main.c - Entry point for RestroDSL
 *
 *  Initialises the session and symbol table, then drives
 *  the main UI loop.
 *
 *  Compiler phases demonstrated:
 *    Phase 1 - Lexical Analysis        (lexer.c)
 *    Phase 2 - Syntax Analysis         (parser.c)
 *    Phase 3 - Semantic Analysis       (semantic.c)
 *    Phase 4 - Intermediate Rep.       (ir.c)
 *    Phase 5 - Code Generation         (codegen.c)
 *    Support - Symbol Table            (symbol_table.c)
 *    Theory  - CFG/NFA/DFA/LR0/LL1    (theory.c)
 * ============================================================ */

#include <stdio.h>
#include <string.h>
#include "config.h"
#include "types.h"
#include "utils.h"
#include "symbol_table.h"
#include "codegen.h"
#include "ir.h"
#include "ui.h"

int main(void) {
    /* ── Initialise session ── */
    Session sess;
    memset(&sess, 0, sizeof(sess));
    sess.debug_mode  = 1;   /* ON by default for lab demo */
    sess.colour_mode = 1;   /* ON by default              */

    /* ── Initialise symbol table (menu) ── */
    SymbolTable st;
    symtab_init(&st);

    /* ── Splash screen ── */
    clear_screen();
    print_line_sep();
    if (sess.colour_mode) printf("%s%s", COL_BOLD, COL_YELLOW);
    printf("  Welcome to RestroDSL v%s\n", VERSION);
    printf("  Restaurant Bill Generator - Compiler Design Project\n");
    if (sess.colour_mode) printf("%s", COL_RESET);
    printf("  Menu loaded: %d items in symbol table.\n", st.count);
    print_line_sep();
    press_enter();

    /* ── Main loop ── */
    int running = 1;
    while (running) {
        ui_show_main_menu(&sess);
        int choice = safe_get_int(1, 9, "  Enter choice: ");

        switch (choice) {
            case 1:
                ui_run_ordering_mode(&sess, &st);
                press_enter();
                break;

            case 2:
                ui_run_theory_menu(&sess);
                break;

            case 3:
                if (sess.order_count > 0) {
                    codegen_generate(&sess, sess.colour_mode);
                } else {
                    if (sess.colour_mode) printf("%s", COL_RED);
                    printf("  No orders yet. Use option 1 first.\n");
                    if (sess.colour_mode) printf("%s", COL_RESET);
                }
                press_enter();
                break;

            case 4:
                printf("\n  Current Orders (IR Preview):\n");
                ir_print_preview(&sess, sess.colour_mode);
                press_enter();
                break;

            case 5:
                symtab_print(&st, sess.colour_mode);
                press_enter();
                break;

            case 6:
                ui_run_manage_menu(&sess, &st);
                break;

            case 7:
                sess.debug_mode = !sess.debug_mode;
                printf("  Debug mode: %s\n",
                       sess.debug_mode ? "ON" : "OFF");
                press_enter();
                break;

            case 8:
                sess.colour_mode = !sess.colour_mode;
                printf("  Colour mode: %s\n",
                       sess.colour_mode ? "ON" : "OFF");
                press_enter();
                break;

            case 9:
                running = 0;
                break;

            default:
                if (sess.colour_mode) printf("%s", COL_RED);
                printf("  Invalid choice. Enter 1–9.\n");
                if (sess.colour_mode) printf("%s", COL_RESET);
                press_enter();
        }
    }

    /* ── Exit message ── */
    if (sess.colour_mode) printf("%s%s", COL_BOLD, COL_CYAN);
    printf("\n  Thank you for using RestroDSL!\n");
    printf("  Phases demonstrated: Lexer, Parser, Semantic,\n");
    printf("  IR, CodeGen, CFG, FIRST/FOLLOW, NFA/DFA, LR(0), LL(1).\n");
    if (sess.colour_mode) printf("%s\n", COL_RESET);

    return 0;
}
