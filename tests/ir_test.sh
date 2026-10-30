#!/bin/bash

GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m'

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
PROJECT_DIR="$(dirname "$SCRIPT_DIR")"
BUILD_DIR="$PROJECT_DIR/build"
IR_DIR="$SCRIPT_DIR/ir"
WORK_DIR="$IR_DIR/build"

echo "=== Building Compiler ==="
cd "$PROJECT_DIR"
rm -rf "$BUILD_DIR"
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

CMAKE_ARGS=""
if [[ "$OSTYPE" == "darwin"* ]]; then
    if command -v brew &> /dev/null; then
        BREW_BISON="$(brew --prefix bison)/bin/bison"
        if [ -x "$BREW_BISON" ]; then
            CMAKE_ARGS="-DBISON_EXECUTABLE=$BREW_BISON"
            echo "macOS: using Homebrew Bison ($BREW_BISON)"
        fi
        BREW_LLVM="$(brew --prefix llvm)/lib/cmake/llvm"
        if [ -d "$BREW_LLVM" ]; then
            CMAKE_ARGS="$CMAKE_ARGS -DLLVM_DIR=$BREW_LLVM"
            echo "macOS: using Homebrew LLVM ($BREW_LLVM)"
        fi
    fi
fi

cmake $CMAKE_ARGS .. 2>&1 | tail -1
make 2>&1 | tail -1

if [ ! -f "$BUILD_DIR/compiler" ]; then
    echo -e "${RED}[FAIL]${NC} Compiler build failed"
    exit 1
fi
echo -e "${GREEN}[PASS]${NC} Compiler built"

echo ""
echo "=== Generating LLVM IR ==="
mkdir -p "$WORK_DIR"
cd "$WORK_DIR"

"$BUILD_DIR/compiler" "$IR_DIR/compiler_test.d" > /dev/null 2>&1
COMPILER_EXIT=$?

if [ $COMPILER_EXIT -ne 0 ]; then
    echo -e "${RED}[FAIL]${NC} Compiler exited with code $COMPILER_EXIT"
    exit 1
fi

if [ ! -f "output.ll" ]; then
    echo -e "${RED}[FAIL]${NC} output.ll not generated"
    exit 1
fi
echo -e "${GREEN}[PASS]${NC} LLVM IR generated"

echo ""
echo "=== Linking and Running Tests ==="

clang++ "$IR_DIR/test_runner.cc" output.ll -o test_runner > /dev/null 2>&1
LINK_EXIT=$?

if [ $LINK_EXIT -ne 0 ]; then
    echo -e "${RED}[FAIL]${NC} Linking failed (exit code $LINK_EXIT)"
    exit 1
fi

./test_runner > actual_output.txt 2>&1
RUN_EXIT=$?

echo ""
echo "=== Comparing Output ==="

diff actual_output.txt "$IR_DIR/expected_output.txt" > /dev/null 2>&1
DIFF_EXIT=$?

if [ $DIFF_EXIT -eq 0 ]; then
    echo -e "${GREEN}[PASS]${NC} Output matches expected"
    echo ""
    echo "=== Summary ==="
    echo -e "${GREEN}All IR tests passed${NC}"
    exit 0
else
    echo -e "${RED}[FAIL]${NC} Output differs from expected"
    echo ""
    echo "--- Expected ---"
    cat "$IR_DIR/expected_output.txt"
    echo ""
    echo "--- Actual ---"
    cat actual_output.txt
    echo ""
    echo "--- Diff ---"
    diff "$IR_DIR/expected_output.txt" actual_output.txt
    echo ""
    echo "=== Summary ==="
    echo -e "${RED}IR tests failed${NC}"
    exit 1
fi
