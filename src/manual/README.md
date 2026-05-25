# Manual Parser

> **📄 Disclaimer: AI-Assisted Documentation**
> This documentation was co-authored with the assistance of an AI tool to ensure clarity, structured readability, and compliance with C++ best practices. However, the software architecture, the underlying grammar logic, and the code design were entirely conceived and verified by human developers.

The manual parser is a hand-written, tool-independent alternative to the Flex/Bison frontend. It lives entirely in `src/manual/` and requires no code generation step.

## Execution

The `-m` flag selects the manual parser at runtime:

```bash
./build/compiler -m path/to/source.d
```

Without `-m`, the Flex/Bison parser is used.

## Architecture

The manual frontend follows a classic two-phase compiler frontend design as described in Mogensen, *Introduction to Compiler Design* (Ch. 2–3).

### Phase 1 — Lexer (`MLexer`)

- **Direct-coded switch-based lexer.** `getNextToken()` inspects the next character and dispatches to `readNumber()`, `readIdentifierOrKeyword()`, `readString()`, or a single-character token branch.
- Keyword recognition happens **after** identifier scanning via hash-table lookup (Mogensen §2.4).
- Whitespace and `€€`-to-end-of-line comments are silently consumed.
- On invalid input, `LexerException` is thrown — carrying the `SourceLocation` and the offending character.

### Phase 2 — Parser (`MParser`)

- **Recursive-descent** with one parse method per nonterminal (e.g., `parse_function()`, `parse_body()`, `parse_var_decl()`).
- Uses a single-token lookahead via `peek()` / `advance()` (LL(1)).
- **First/follow sets** are defined in `Sets.hh` and drive predictive branching — the parser checks which alternatives can start the next production and dispatches accordingly.
- `consume(expected, msg)` validates the current token and advances on success, otherwise throws `ParserException` (location + expected token + actual token).
- The parser builds the AST top-down using `std::unique_ptr` ownership throughout.

### Data Flow

```
Source file (.d)
  → Lexer (character stream → token stream)
  → Parser (token stream → AST via recursive descent)
  → AST traversal (ASTPrinter, ASTSConstructor → Symboltable)
```

### Error Handling

Both phases use dedicated exception types that capture the error location:

| Exception          | Fields                                  |
|--------------------|-----------------------------------------|
| `LexerException`   | `SourceLocation loc`, `char invalid_char` |
| `ParserException`  | `SourceLocation loc`, `std::string expected`, `TokenType actual` |
