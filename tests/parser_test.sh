#!/bin/bash

MANUAL_FLAG=""
if [ "$1" = "-m" ]; then
    MANUAL_FLAG="-m"
fi

COMPILER="build/compiler"
GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m'

PASSED=0
FAILED=0


run_test_suite() {
    local FOLDER=$1
    local EXPECTED_CODE=$2
    
    echo "=== Testing $FOLDER Programme (Expected Exit-Code: $EXPECTED_CODE) ==="
    
    for input_file in $FOLDER/*.d; do
        [ -e "$input_file" ] || continue 
        
        $COMPILER $MANUAL_FLAG $input_file > /dev/null 2>&1
        
        
        ACTUAL_CODE=$?
        
        if [ $ACTUAL_CODE -eq $EXPECTED_CODE ]; then
            echo -e "${GREEN}[PASS]${NC} $input_file"
            PASSED=$((PASSED+1))
        else
            echo -e "${RED}[FAIL]${NC} $input_file (Expected: $EXPECTED_CODE, Actual: $ACTUAL_CODE)"
            FAILED=$((FAILED+1))
        fi
    done
    echo ""
}
COMPILER="../build/compiler"
...
echo "Building Compiler in project root..."
cd ..
mkdir -p build
cd build


CMAKE_ARGS=""

if [[ "$OSTYPE" == "darwin"* ]]; then
    if command -v brew &> /dev/null; then
        BREW_BISON="$(brew --prefix bison)/bin/bison"
        if [ -x "$BREW_BISON" ]; then
            CMAKE_ARGS="-DBISON_EXECUTABLE=$BREW_BISON"
            echo "macOS recognized: Homebrew Bison ($BREW_BISON)"
        fi
    fi
elif [[ "$OSTYPE" == "linux-gnu"* ]] || [[ "$OSTYPE" == "msys" ]] || [[ "$OSTYPE" == "cygwin" ]]; then
    if [ -x "/usr/local/bin/bison" ]; then
        CMAKE_ARGS="-DBISON_EXECUTABLE=/usr/local/bin/bison"
        echo "Linux/Windows recognized: Bison (/usr/local/bin/bison)"
    fi
fi

echo "Running: cmake $CMAKE_ARGS .."
cmake $CMAKE_ARGS ..


make
cd ../tests

echo ""
echo "=== Starting Tests (MANUAL MODE) ==="
run_test_suite "correct" 0
run_test_suite "incorrect" 1

echo "=== Summary ==="
echo -e "${GREEN}Passed: $PASSED${NC}"
if [ $FAILED -gt 0 ]; then
    echo -e "${RED}Failed: $FAILED${NC}"
    exit 1
else
    exit 0
fi


