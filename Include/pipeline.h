#ifndef PIPELINE_H
#define PIPELINE_H

/* ============================================================
 *  pipeline.h — Full Compiler Pipeline Controller
 *
 *  Connects all phases in sequence for a single input line:
 *    Lexer -> Parser -> Semantic -> IR -> (ready for CodeGen)
 *
 *  This module is the "driver" that a real compiler front-end
 *  uses to coordinate all analysis phases.
 * ============================================================ */

#include "types.h"
#include "symbol_table.h"

/* ── Public API ── */

/**
 * Process one raw input line through all compiler phases.
 * Modifies sess (order list, error count, history).
 * Returns a ParseResult enum value.
 */
ParseResult pipeline_process_line(const char  *line,
                                   int          line_no,
                                   Session     *sess,
                                   SymbolTable *st);

#endif /* PIPELINE_H */
