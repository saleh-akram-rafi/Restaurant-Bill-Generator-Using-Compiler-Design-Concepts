# Restaurant-Bill-Generator-Using-Compiler-Design-Concepts
A complete 5-phase compiler pipeline built in C, applied to a real-world restaurant ordering system — developed as a CSE314 Compiler Design Lab project at Daffodil International University.

RestroDSL is a Domain-Specific Language (DSL) interpreter that treats restaurant orders as source code and processes them through every classical compiler phase — Lexical Analysis, Syntax Analysis, Symbol Table management, Semantic Analysis, Intermediate Representation, and Code Generation — producing a formatted, tax-calculated restaurant bill as its final output.
Instead of compiling a programming language, this system compiles commands like order burger 2 into a live bill. Every compiler concept from the textbook is mapped directly to a practical, working feature.

🔧 What It Demonstrates

Phase 1 — Lexer → Tokenises each input line into KEYWORD, IDENTIFIER, NUMBER, or UNKNOWN using a hand-written DFA
Phase 2 — Parser → Validates grammar with an LL(1) predictive parser; rejects 2 burger, pizza, burger two
Phase 3 — Symbol Table → 14-item menu stored as a key-value symbol table with availability flags
Phase 4 — Semantic Analysis → Checks item existence, availability, positive quantity, and cancel-limit enforcement
Phase 5 — IR → Validated orders stored in a live OrderRecord[] array (the Intermediate Representation)
Phase 6 — Code Generation → Produces a formatted bill with 5% VAT, 10% discount, saves to restaurant_bill.txt, and appends to order_log.txt
Theory Engine → Interactive display of CFG, FIRST/FOLLOW sets, NFA→DFA, LR(0) canonical items, and LL(1) parse table


📁 Architecture
11 modular C files across src/ and include/ — one module per compiler phase. Builds with a single make command. Zero external dependencies. ANSI colour-coded debug output makes every phase decision visible in the terminal.

👥 Team — Group 1
Saleh Akram · Kousik Ahammed · Md Hasib Ul Islam · Md Abdur Rouf Tanvir · Md Sifat Ullah
Course: CSE314 — Compiler Design Lab | University: Daffodil International University | Year: 2026
