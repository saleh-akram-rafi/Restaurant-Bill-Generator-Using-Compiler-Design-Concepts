/* ============================================================
 *  utils.c - Utility functions implementation
 * ============================================================ */

/* RestroDSL utils.c - requires POSIX for isatty/fileno */
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <unistd.h>
#include "utils.h"

/* ── String helpers ── */

void str_to_lower(char *dest, const char *src, int max_len) {
    int i;
    for (i = 0; src[i] && i < max_len - 1; i++)
        dest[i] = (char)tolower((unsigned char)src[i]);
    dest[i] = '\0';
}

void str_capitalize(char *s) {
    if (s && s[0])
        s[0] = (char)toupper((unsigned char)s[0]);
}

int str_is_alpha(const char *s) {
    if (!s || !*s) return 0;
    for (int i = 0; s[i]; i++)
        if (!isalpha((unsigned char)s[i])) return 0;
    return 1;
}

int str_is_digit(const char *s) {
    if (!s || !*s) return 0;
    for (int i = 0; s[i]; i++)
        if (!isdigit((unsigned char)s[i])) return 0;
    return 1;
}

void str_trim(char *s) {
    /* Trim trailing whitespace */
    int len = (int)strlen(s);
    while (len > 0 && isspace((unsigned char)s[len - 1]))
        s[--len] = '\0';
    /* Trim leading whitespace */
    char *start = s;
    while (*start && isspace((unsigned char)*start)) start++;
    if (start != s) {
        memmove(s, start, strlen(start) + 1);
    }
}

/* ── Console display ── */

void print_line_sep(void) {
    printf("============================================================\n");
}

void print_sub_sep(void) {
    printf("------------------------------------------------------------\n");
}

void print_colored(const char *text, const char *color, int colour_enabled) {
    if (colour_enabled)
        printf("%s%s%s", color, text, COL_RESET);
    else
        printf("%s", text);
}

void clear_screen(void) {
    printf("\033[2J\033[H");
}

void press_enter(void) {
    /* Only wait if stdin is a terminal (not piped) */
    if (isatty(fileno(stdin))) {
        printf("\nPress Enter to continue...");
        fflush(stdout);
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
    }
}

/* ── Safe input ── */

int safe_get_int(int minv, int maxv, const char *prompt) {
    printf("%s", prompt);
    fflush(stdout);
    char buf[32];
    if (!fgets(buf, sizeof(buf), stdin)) return -1;
    int val;
    if (sscanf(buf, "%d", &val) != 1) return -1;
    return (val >= minv && val <= maxv) ? val : -1;
}

void safe_get_line(char *buf, int max_len, const char *prompt) {
    printf("%s", prompt);
    fflush(stdout);
    if (!fgets(buf, max_len, stdin)) buf[0] = '\0';
    buf[strcspn(buf, "\n")] = '\0';
}

/* ── Time helper ── */

void get_timestamp(char *buf, int max_len) {
    time_t now = time(NULL);
    strftime(buf, (size_t)max_len, "%Y-%m-%d  %H:%M:%S", localtime(&now));
}
