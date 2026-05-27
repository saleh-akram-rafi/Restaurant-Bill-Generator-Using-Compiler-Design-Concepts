/* ============================================================
 *  symbol_table.c - Symbol Table implementation
 *  Compiler phase: pre-pass / semantic support structure
 * ============================================================ */

#include <stdio.h>
#include <string.h>
#include "symbol_table.h"
#include "utils.h"

/* Default menu data - easily extended */
static const struct { const char *key; const char *display; double price; } DEFAULT_MENU[] = {
    { "burger",    "Burger",    100.0 },
    { "pizza",     "Pizza",     300.0 },
    { "coffee",    "Coffee",     50.0 },
    { "pasta",     "Pasta",     250.0 },
    { "salad",     "Salad",     120.0 },
    { "sandwich",  "Sandwich",  150.0 },
    { "juice",     "Juice",      60.0 },
    { "icecream",  "Icecream",   80.0 },
    { "soup",      "Soup",       90.0 },
    { "steak",     "Steak",     450.0 },
    { "noodles",   "Noodles",   130.0 },
    { "fries",     "Fries",      70.0 },
    { "coke",      "Coke",       40.0 },
    { "water",     "Water",      20.0 },
};
static const int DEFAULT_MENU_SIZE =
    (int)(sizeof(DEFAULT_MENU) / sizeof(DEFAULT_MENU[0]));

void symtab_init(SymbolTable *st) {
    st->count = 0;
    for (int i = 0; i < DEFAULT_MENU_SIZE && i < MAX_ITEMS; i++) {
        strncpy(st->entries[i].name,    DEFAULT_MENU[i].key,     MAX_NAME_LEN - 1);
        strncpy(st->entries[i].display, DEFAULT_MENU[i].display, MAX_NAME_LEN - 1);
        st->entries[i].price     = DEFAULT_MENU[i].price;
        st->entries[i].available = 1;
        st->count++;
    }
}

double symtab_lookup(const SymbolTable *st, const char *name) {
    for (int i = 0; i < st->count; i++) {
        if (st->entries[i].available &&
            strcmp(st->entries[i].name, name) == 0)
            return st->entries[i].price;
    }
    return -1.0;
}

int symtab_insert(SymbolTable *st, const char *name,
                  const char *display, double price) {
    /* Update if already exists */
    for (int i = 0; i < st->count; i++) {
        if (strcmp(st->entries[i].name, name) == 0) {
            strncpy(st->entries[i].display, display, MAX_NAME_LEN - 1);
            st->entries[i].price     = price;
            st->entries[i].available = 1;
            return 1;
        }
    }
    if (st->count >= MAX_ITEMS) return 0;
    strncpy(st->entries[st->count].name,    name,    MAX_NAME_LEN - 1);
    strncpy(st->entries[st->count].display, display, MAX_NAME_LEN - 1);
    st->entries[st->count].price     = price;
    st->entries[st->count].available = 1;
    st->count++;
    return 1;
}

int symtab_set_available(SymbolTable *st, const char *name, int available) {
    for (int i = 0; i < st->count; i++) {
        if (strcmp(st->entries[i].name, name) == 0) {
            st->entries[i].available = available;
            return 1;
        }
    }
    return 0;
}

const char *symtab_display_name(const SymbolTable *st, const char *key) {
    for (int i = 0; i < st->count; i++)
        if (strcmp(st->entries[i].name, key) == 0)
            return st->entries[i].display;
    return NULL;
}

void symtab_print(const SymbolTable *st, int colour) {
    printf("\n");
    if (colour) printf("%s", COL_CYAN);
    printf("========================================\n");
    printf("|        RESTAURANT MENU               |\n");
    printf("========================================\n");
    printf("|  %-18s  %10s  %-4s|\n", "Item", "Price (Tk)", "Avl");
    printf("========================================\n");
    if (colour) printf("%s", COL_RESET);

    for (int i = 0; i < st->count; i++) {
        const char *avl = st->entries[i].available ? "Yes" : " No";
        if (!st->entries[i].available && colour)
            printf("%s", COL_RED);
        printf("|  %-18s  %10.2f  %-4s|\n",
               st->entries[i].display,
               st->entries[i].price,
               avl);
        if (!st->entries[i].available && colour)
            printf("%s", COL_RESET);
    }
    printf("========================================\n\n");
}
