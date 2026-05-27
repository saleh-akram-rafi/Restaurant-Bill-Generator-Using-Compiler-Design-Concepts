# ============================================================
#  Makefile — RestroDSL Build System
#  Restaurant Bill Generator: Compiler Design Project
# ============================================================

CC      = gcc
CFLAGS  = -Wall -Wextra -Wpedantic -std=c11 -Iinclude
DBGFLAGS= -g -DDEBUG
RELFLAGS= -O2

TARGET  = restrodsl
SRCDIR  = src
INCDIR  = include
OBJDIR  = obj
TESTDIR = tests

# ── Source files ──────────────────────────────────────────────
SRCS = $(SRCDIR)/main.c        \
       $(SRCDIR)/utils.c       \
       $(SRCDIR)/symbol_table.c\
       $(SRCDIR)/lexer.c       \
       $(SRCDIR)/parser.c      \
       $(SRCDIR)/semantic.c    \
       $(SRCDIR)/ir.c          \
       $(SRCDIR)/codegen.c     \
       $(SRCDIR)/theory.c      \
       $(SRCDIR)/pipeline.c    \
       $(SRCDIR)/ui.c

OBJS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRCS))

# ── Default target ────────────────────────────────────────────
.PHONY: all
all: $(OBJDIR) $(TARGET)
	@echo ""
	@echo "  Build successful: ./$(TARGET)"
	@echo "  Run with:         ./$(TARGET)"
	@echo ""

# ── Link ──────────────────────────────────────────────────────
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(RELFLAGS) -o $@ $^

# ── Compile each .c -> .o ─────────────────────────────────────
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) $(RELFLAGS) -c $< -o $@

# ── Create obj directory ──────────────────────────────────────
$(OBJDIR):
	mkdir -p $(OBJDIR)

# ── Debug build ───────────────────────────────────────────────
.PHONY: debug
debug: RELFLAGS = $(DBGFLAGS)
debug: $(OBJDIR) $(TARGET)
	@echo "  Debug build complete: ./$(TARGET)"

# ── Run ───────────────────────────────────────────────────────
.PHONY: run
run: all
	./$(TARGET)

# ── Clean ─────────────────────────────────────────────────────
.PHONY: clean
clean:
	rm -rf $(OBJDIR) $(TARGET) $(TARGET).exe \
	       restaurant_bill.txt order_log.txt
	@echo "  Cleaned."

# ── Show project structure ────────────────────────────────────
.PHONY: tree
tree:
	@echo ""
	@echo "  restrodsl/"
	@echo "  ├── Makefile"
	@echo "  ├── include/"
	@echo "  │   ├── config.h         constants & ANSI colours"
	@echo "  │   ├── types.h          all shared structs"
	@echo "  │   ├── utils.h          string/IO helpers"
	@echo "  │   ├── lexer.h          Phase 1: tokenisation"
	@echo "  │   ├── parser.h         Phase 2: syntax analysis"
	@echo "  │   ├── symbol_table.h   menu / symbol table"
	@echo "  │   ├── semantic.h       Phase 3: meaning checks"
	@echo "  │   ├── ir.h             Phase 4: intermediate rep"
	@echo "  │   ├── codegen.h        Phase 5: bill generation"
	@echo "  │   ├── theory.h         theory engine display"
	@echo "  │   ├── pipeline.h       phase coordinator"
	@echo "  │   └── ui.h             text user interface"
	@echo "  └── src/"
	@echo "      ├── main.c           entry point"
	@echo "      ├── utils.c"
	@echo "      ├── symbol_table.c"
	@echo "      ├── lexer.c"
	@echo "      ├── parser.c"
	@echo "      ├── semantic.c"
	@echo "      ├── ir.c"
	@echo "      ├── codegen.c"
	@echo "      ├── theory.c"
	@echo "      ├── pipeline.c"
	@echo "      └── ui.c"
	@echo ""

# ── Help ─────────────────────────────────────────────────────
.PHONY: help
help:
	@echo ""
	@echo "  make          — build release binary"
	@echo "  make debug    — build with -g debug symbols"
	@echo "  make run      — build and run"
	@echo "  make clean    — remove build artifacts"
	@echo "  make tree     — show project file structure"
	@echo "  make help     — show this help"
	@echo ""
