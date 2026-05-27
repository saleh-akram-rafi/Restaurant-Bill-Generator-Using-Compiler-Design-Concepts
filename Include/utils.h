#ifndef UTILS_H
#define UTILS_H

/* ============================================================
 *  utils.h — String helpers, display helpers, I/O utilities
 * ============================================================ */

#include "types.h"

/* ── String helpers ── */
void  str_to_lower(char *dest, const char *src, int max_len);
void  str_capitalize(char *s);
int   str_is_alpha(const char *s);
int   str_is_digit(const char *s);
void  str_trim(char *s);

/* ── Console display ── */
void  print_line_sep(void);
void  print_sub_sep(void);
void  print_colored(const char *text, const char *color, int colour_enabled);
void  clear_screen(void);
void  press_enter(void);

/* ── Safe input ── */
int   safe_get_int(int minv, int maxv, const char *prompt);
void  safe_get_line(char *buf, int max_len, const char *prompt);

/* ── Time helper ── */
void  get_timestamp(char *buf, int max_len);

#endif /* UTILS_H */
