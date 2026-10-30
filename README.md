# D-Kompilierer

A compiler for a German-keyword programming language (`.d` files) built with Flex/Bison. Generates LLVM IR for further compilation to native code.

> **University Project** — This was a compiler construction course project. The codebase is complete and serves as an educational reference implementation. No further development is expected.

## Prerequisites

- **Bison** ≥ 3.8.2 (macOS: install via Homebrew, system version is too old)
- **Flex**
- **CMake** ≥ 3.28
- **C++17** compiler (GCC or Clang)
- **LLVM** (for IR generation and native compilation)

## Building

```bash
mkdir build && cd build
cmake ..
make
```

**macOS users:** Homebrew Bison is required:
```bash
cmake -DBISON_EXECUTABLE=$(brew --prefix bison)/bin/bison ..
```

## Docker Usage

Using Docker Compose is the easiest way to build and run the compiler without installing Bison, Flex, CMake, or LLVM locally. The container image bundles the `compiler` binary alongside `llvm` (`llc`) and `clang` (`clang++`).

### Build the Image
```bash
docker compose build
```

### Compile a `.d` File
The repository root is mounted at `/workspace` inside the container, so `output.ll` is written directly to your project directory:
```bash
docker compose run --rm compiler tests/correct/minimal.d
```

Command-line flags (`-p`, `-s`) can be passed before the input file:
```bash
docker compose run --rm compiler -p tests/correct/minimal.d
```

### Link & Run Generated IR in Docker
You can also link the generated `output.ll` with a C/C++ wrapper and execute the resulting binary inside the container:
```bash
docker compose run --rm --entrypoint bash compiler -c \
  "compiler tests/ir/compiler_test.d && clang++ tests/ir/test_runner.cc output.ll -o runner && ./runner"
```

To open an interactive shell inside the container environment:
```bash
docker compose run --rm --entrypoint bash compiler
```

## Usage & LLVM Workflow

The compiler parses `.d` source files and generates LLVM IR (`.ll` files). To produce an executable binary, you need to compile the IR and link it with a C/C++ wrapper.

### Basic Usage
```bash
./build/compiler program.d
```
This generates `output.ll` in the current directory.

### Full Compilation Workflow

```bash
# Step 1: Compile D source to LLVM IR
./build/compiler program.d

# Step 2: Compile IR to object file
llc -filetype=obj output.ll -o program.o

# Step 3: Link with C/C++ wrapper
clang wrapper.cc program.o -o program

# Step 4: Run
./program
```

See `src/llvm/` for example C++ wrapper files and test IR.

### Command-Line Flags
- `-p` — Trace parsing (Flex/Bison parser only)
- `-s` — Trace scanning (Flex/Bison parser only)

## Project Structure

| Directory | Purpose |
|-----------|---------|
| `src/parser/` | Flex/Bison parser + Driver |
| `src/manual/` | Hand-written recursive-descent parser (⚠️ no longer maintained) |
| `src/ast/` | AST node types, Visitor pattern, ASTPrinter, AST→Symboltable |
| `src/symboltable/` | Symbol table implementation and printer |
| `src/semantic/` | Semantic analysis passes (GlobalDeclPass, SemanticChecker) |
| `src/ir-generator/` | LLVM IR code generation (CodegenVisitor) |
| `src/llvm/` | Example C++ wrappers and test IR files |
| `src/common/` | Shared types: SourceLocation, Token, Types |
| `tests/` | Test suite with correct/incorrect programs |

## Testing

```bash
cd tests && bash parser_test.sh
```

The test script:
- Rebuilds the compiler from scratch
- Runs all `.d` files in `tests/correct/` (expects exit code 0)
- Runs all `.d` files in `tests/incorrect/` (expects exit code 1)
- Automatically detects Homebrew Bison on macOS

## Manual Parser Note

The manual parser in `src/manual/` is **no longer maintained**. Use the default Flex/Bison parser for all compilation tasks. See `src/manual/README.md` for architectural documentation.

## Language Features

The source language uses German keywords such as:
- `funk` (function)
- `leer` (void)
- `zurück` (return)
- and others

See test files in `tests/correct/` for example programs.
