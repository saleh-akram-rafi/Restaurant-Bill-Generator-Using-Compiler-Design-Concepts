/* ============================================================
 *  ui.c - Text User Interface
 *
 *  All menu screens, prompts, and interactive loops.
 *  No business logic lives here - this module only calls
 *  the pipeline and display functions.
 * ============================================================ */

#include <stdio.h>
#include <string.h>
#include "ui.h"
#include "pipeline.h"
#include "codegen.h"
#include "theory.h"
#include "ir.h"
#include "symbol_table.h"
#include "utils.h"

/* ── Main menu ── */
void ui_show_main_menu(const Session *sess) {
    clear_screen();
    print_line_sep();
    if (sess->colour_mode) printf("%s%s", COL_BOLD, COL_YELLOW);
    printf("   RestroDSL v%s - Restaurant Compiler Design Project\n", VERSION);
    if (sess->colour_mode) printf("%s", COL_RESET);
    print_line_sep();
    printf("   1.  Interactive Ordering  (Lexer -> Parser -> Semantic -> IR)\n");
    printf("   2.  Theory Engine         (CFG | FIRST/FOLLOW | NFA/DFA | LR(0) | LL(1))\n");
    printf("   3.  Generate Final Bill   (Code Generation Phase)\n");
    printf("   4.  View Current Orders   (IR Preview)\n");
    printf("   5.  View Menu             (Symbol Table)\n");
    printf("   6.  Session Management    (Clear / Stats)\n");
    printf("   7.  Toggle Debug Mode     [%s]\n",
           sess->debug_mode ? "ON " : "OFF");
    printf("   8.  Toggle Colour Mode    [%s]\n",
           sess->colour_mode ? "ON " : "OFF");
    printf("   9.  Exit\n");
    print_line_sep();
    printf("   Orders in session: %d  |  Errors: %d\n",
           sess->order_count, sess->error_count);
    print_line_sep();
}

/* ── Interactive ordering loop ── */
void ui_run_ordering_mode(Session *sess, SymbolTable *st) {
    char line[MAX_LINE_LEN];
    clear_screen();
    print_line_sep();
    if (sess->colour_mode) printf("%s", COL_CYAN);
    printf("   INTERACTIVE ORDERING MODE\n");
    if (sess->colour_mode) printf("%s", COL_RESET);
    print_line_sep();
    printf("   Commands:\n");
    printf("     order  <item> <qty>   - add to bill\n");
    printf("     cancel <item> <qty>   - remove from bill\n");
    printf("     show                  - preview current bill\n");
    printf("     clear                 - reset all orders\n");
    printf("     menu                  - display menu\n");
    printf("     done                  - finish ordering\n");
    printf("     # <text>              - comment line (ignored)\n");
    print_line_sep();

    int line_no = sess->history_count + 1;

    while (1) {
        if (sess->colour_mode) printf("%s", COL_YELLOW);
        printf("  restro> ");
        if (sess->colour_mode) printf("%s", COL_RESET);
        fflush(stdout);

        if (!fgets(line, sizeof(line), stdin)) break;
        line[strcspn(line, "\n")] = '\0';
        str_trim(line);

        if (line[0] == '\0') continue;
        if (strcmp(line, "done") == 0 || strcmp(line, "DONE") == 0) break;

        if (strcmp(line, "menu") == 0 || strcmp(line, "MENU") == 0) {
            symtab_print(st, sess->colour_mode);
            continue;
        }

        pipeline_process_line(line, line_no++, sess, st);
    }

    if (sess->colour_mode) printf("%s", COL_GREEN);
    printf("\n  Ordering session ended. %d item(s) in bill.\n",
           sess->order_count);
    if (sess->colour_mode) printf("%s", COL_RESET);
}

/* ── Theory engine submenu ── */
void ui_run_theory_menu(const Session *sess) {
    int col = sess->colour_mode;
    while (1) {
        clear_screen();
        print_line_sep();
        if (col) printf("%s", COL_CYAN);
        printf("   COMPILER THEORY ENGINE\n");
        if (col) printf("%s", COL_RESET);
        print_line_sep();
        printf("   1. Context-Free Grammar (CFG)\n");
        printf("   2. FIRST and FOLLOW sets\n");
        printf("   3. NFA -> DFA conversion (Lexer)\n");
        printf("   4. LR(0) Canonical Items + Parse Table\n");
        printf("   5. LL(1) Predictive Parse Table\n");
        printf("   6. Intermediate Representation (with current orders)\n");
        printf("   7. Back to Main Menu\n");
        print_line_sep();

        int ch = safe_get_int(1, 7, "  Choice: ");
        switch (ch) {
            case 1: theory_show_cfg(col);                     break;
            case 2: theory_show_first_follow(col);            break;
            case 3: theory_show_nfa_dfa(col);                 break;
            case 4: theory_show_lr0(col);                     break;
            case 5: theory_show_ll1(col);                     break;
            case 6: theory_show_ir_example(sess, col);        break;
            case 7: return;
            default:
                if (col) printf("%s", COL_RED);
                printf("  Invalid choice.\n");
                if (col) printf("%s", COL_RESET);
        }
        press_enter();
    }
}

/* ── Session management submenu ── */
void ui_run_manage_menu(Session *sess, SymbolTable *st) {
    int col = sess->colour_mode;
    while (1) {
        clear_screen();
        print_line_sep();
        printf("   SESSION MANAGEMENT\n");
        print_line_sep();
        printf("   1. Clear all orders (reset IR)\n");
        printf("   2. View session statistics\n");
        printf("   3. View order history (raw input log)\n");
        printf("   4. Back to Main Menu\n");
        print_line_sep();

        int ch = safe_get_int(1, 4, "  Choice: ");
        if (ch == 1) {
            ir_clear(sess);
            if (col) printf("%s", COL_YELLOW);
            printf("  All orders cleared.\n");
            if (col) printf("%s", COL_RESET);
            press_enter();
        }
        else if (ch == 2) {
            ui_show_session_stats(sess);
            press_enter();
        }
        else if (ch == 3) {
            printf("\n  Input history (%d lines):\n", sess->history_count);
            print_sub_sep();
            for (int i = 0; i < sess->history_count; i++)
                printf("  %3d: %s\n", i + 1, sess->history[i]);
            print_sub_sep();
            press_enter();
        }
        else if (ch == 4) return;
        else {
            printf("  Invalid choice.\n");
        }
        (void)st; /* Reserved for future "add menu item" feature */
    }
}

/* ── Session statistics ── */
void ui_show_session_stats(const Session *sess) {
    printf("\n");
    print_line_sep();
    printf("  SESSION STATISTICS\n");
    print_line_sep();
    printf("  %-30s %d\n", "Valid order lines:",   sess->order_count);
    printf("  %-30s %d\n", "Error lines skipped:", sess->error_count);
    printf("  %-30s %d\n", "Cancellations made:",  sess->cancel_count);
    printf("  %-30s %d\n", "Total input lines:",   sess->history_count);

    double total = 0.0;
    for (int i = 0; i < sess->order_count; i++)
        total += sess->orders[i].subtotal;
    printf("  %-30s %.2f Tk\n", "Current subtotal:", total);
    print_line_sep();
}
