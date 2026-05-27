#ifndef THEORY_H
#define THEORY_H

/* ============================================================
 *  theory.h — Compiler Theory Demonstrations
 *
 *  Educational output showing the formal theory behind each
 *  compiler phase used in this project:
 *    - Context-Free Grammar (CFG)
 *    - FIRST and FOLLOW sets
 *    - NFA to DFA conversion (subset construction)
 *    - LR(0) canonical items + parsing table
 *    - LL(1) parse table
 *    - Three-address code / IR walkthrough
 * ============================================================ */

#include "types.h"

/* ── Public API ── */
void theory_show_cfg(int colour);
void theory_show_first_follow(int colour);
void theory_show_nfa_dfa(int colour);
void theory_show_lr0(int colour);
void theory_show_ll1(int colour);
void theory_show_ir_example(const Session *sess, int colour);

#endif /* THEORY_H */
