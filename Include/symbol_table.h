#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

/* ============================================================
 *  symbol_table.h — Menu/Symbol Table management
 *
 *  In compiler design: the symbol table stores identifiers
 *  and their attributes. Here, identifiers = food item names,
 *  attributes = price and availability.
 * ============================================================ */

#include "types.h"

/* ── Public API ── */

/**
 * Populate the symbol table with the default menu.
 */
void   symtab_init(SymbolTable *st);

/**
 * Look up an item by lowercase name.
 * Returns price on success, -1.0 if not found or unavailable.
 */
double symtab_lookup(const SymbolTable *st, const char *name);

/**
 * Add or update an entry.
 * Returns 1 on success, 0 if table is full.
 */
int    symtab_insert(SymbolTable *st, const char *name,
                     const char *display, double price);

/**
 * Mark an item as unavailable (86'd from menu).
 * Returns 1 if found and toggled, 0 if not found.
 */
int    symtab_set_available(SymbolTable *st, const char *name, int available);

/**
 * Print the full symbol table (menu display).
 */
void   symtab_print(const SymbolTable *st, int colour);

/**
 * Return the display name for a given key, or NULL if not found.
 */
const char *symtab_display_name(const SymbolTable *st, const char *key);

#endif /* SYMBOL_TABLE_H */
