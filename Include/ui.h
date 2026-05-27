#ifndef UI_H
#define UI_H

/* ============================================================
 *  ui.h — Text User Interface (menus, prompts, screens)
 * ============================================================ */

#include "types.h"
#include "symbol_table.h"

/* ── Public API ── */

void ui_show_main_menu(const Session *sess);
void ui_run_ordering_mode(Session *sess, SymbolTable *st);
void ui_run_theory_menu(const Session *sess);
void ui_run_manage_menu(Session *sess, SymbolTable *st);
void ui_show_session_stats(const Session *sess);

#endif /* UI_H */
